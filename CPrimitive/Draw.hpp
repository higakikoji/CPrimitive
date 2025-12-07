#pragma once

#include <windows.h>
#include <stdio.h>
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <wrl/client.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")

using namespace Microsoft::WRL;

/// <summary>
/// 描画クラス
/// </summary>
class Draw {
public:
  typedef enum : int {
    Black = D2D1::ColorF::Black,
    Blue = D2D1::ColorF::Blue,
    Green = D2D1::ColorF::Green,
    Cyan = D2D1::ColorF::Cyan,
    DimGray = D2D1::ColorF::DimGray,
    Gray = D2D1::ColorF::Gray,
    Red = D2D1::ColorF::Red,
    Magenta = D2D1::ColorF::Magenta,
    Yellow = D2D1::ColorF::Yellow,
    White = D2D1::ColorF::White
  } Color;

  /// <summary>
  /// レンダリングクラス
  /// </summary>
  class Render {
  public:
    Render(::HWND windowHandle, ::ComPtr<::ID2D1Factory> factory, ::ComPtr<::IDWriteFactory> writeFactory, ::LPCWSTR fontFamily, ::FLOAT fontSize, ::ComPtr<::IWICImagingFactory> wicFactory, ::LPCWSTR *images, int imageNum) {
      ::RECT _rc = {};
      GetClientRect(windowHandle, &_rc);
      auto _ret = factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(windowHandle, D2D1::SizeU(_rc.right - _rc.left, _rc.bottom - _rc.top)), &_target);
      if (FAILED(_ret)) {
        return;
      }
      _fontSize = fontSize;
      _ret = writeFactory->CreateTextFormat(fontFamily, nullptr, ::DWRITE_FONT_WEIGHT_NORMAL, ::DWRITE_FONT_STYLE_NORMAL, ::DWRITE_FONT_STRETCH_NORMAL, fontSize, _locale, &_textFormat);
      if (FAILED(_ret)) {
        return;
      }
      for (auto _i = 0; _i < imageNum; _i++) {
        _bitmap[_i] = _loadImage(_target.Get(), wicFactory.Get(), images[_i]);
      }
    }

    ~Render() {}

    void Begin() {
      if (_target) {
        _target->BeginDraw();
      }
    }

    bool End() {
      auto _ret = false;
      if (_target) {
        auto _dret = _target->EndDraw();
        if (_dret != D2DERR_RECREATE_TARGET) {
          _ret = true;
        }
      }
      return _ret;
    }

    void Clear(Color color) {
      if (_target) {
        _target->Clear(D2D1::ColorF(color));
      }
    }

    void Line(int sx, int sy, int ex, int ey, Color color, int stroke = 1) {
      if (_target) {
        ::ComPtr<::ID2D1SolidColorBrush> _b;
        _target->CreateSolidColorBrush(D2D1::ColorF(color), &_b);
        _target->DrawLine(D2D1::Point2F((::FLOAT)sx, (::FLOAT)sy), D2D1::Point2F((::FLOAT)ex, (::FLOAT)ey), _b.Get(), (::FLOAT)stroke);
      }
    }

    void Rectangle(int x, int y, int w, int h, Color color, bool fill = true, int stroke = 1, bool round = false, int rw = 2, int rh = 2) {
      if (_target) {
        ::ComPtr<::ID2D1SolidColorBrush> _b;
        _target->CreateSolidColorBrush(D2D1::ColorF(color), &_b);
        if (fill) {
          if (round) {
            _target->FillRoundedRectangle(D2D1::RoundedRect(D2D1::RectF((::FLOAT)x, (::FLOAT)y, (::FLOAT)(x + w), (::FLOAT)(y + h)), (::FLOAT)(rw / 2.0F), (::FLOAT)(rh / 2.0F)), _b.Get());
          } else {
            _target->FillRectangle(D2D1::RectF((::FLOAT)x, (::FLOAT)y, (::FLOAT)(x + w), (::FLOAT)(y + h)), _b.Get());
          }
        } else {
          if (round) {
            _target->DrawRoundedRectangle(D2D1::RoundedRect(D2D1::RectF((::FLOAT)x, (::FLOAT)y, (::FLOAT)(x + w), (::FLOAT)(y + h)), (::FLOAT)(rw / 2.0F), (::FLOAT)(rh / 2.0F)), _b.Get(), (::FLOAT)stroke);
          } else {
            _target->DrawRectangle(D2D1::RectF((::FLOAT)x, (::FLOAT)y, (::FLOAT)(x + w), (::FLOAT)(y + h)), _b.Get(), (::FLOAT)stroke);
          }
        }
      }
    }

    void Ellipse(int x, int y, int w, int h, Color color, bool fill = true, int stroke = 1) {
      if (_target) {
        ::ComPtr<::ID2D1SolidColorBrush> _b;
        _target->CreateSolidColorBrush(D2D1::ColorF(color), &_b);
        if (fill) {
          _target->FillEllipse(D2D1::Ellipse(D2D1::Point2F((::FLOAT)(x + (w / 2.0F)), (::FLOAT)(y + (h / 2.0F))), (::FLOAT)(w / 2.0F), (::FLOAT)(h / 2.0F)), _b.Get());
        } else {
          _target->DrawEllipse(D2D1::Ellipse(D2D1::Point2F((::FLOAT)(x + (w / 2.0F)), (::FLOAT)(y + (h / 2.0F))), (::FLOAT)(w / 2.0F), (::FLOAT)(h / 2.0F)), _b.Get(), (::FLOAT)stroke);
        }
      }
    }

    void Text(int x, int y, Color color, ::LPCWSTR text, ...) {
      if (_target) {
        ::ComPtr<::ID2D1SolidColorBrush> _b;
        _target->CreateSolidColorBrush(D2D1::ColorF(color), &_b);
        va_list _ap;
        va_start(_ap, text);
        _vsnwprintf_s(_textBuf, _countof(_textBuf), _TRUNCATE, text, _ap);
        va_end(_ap);
        _target->DrawTextW(_textBuf, (::UINT32)wcslen(_textBuf), _textFormat.Get(), D2D1::RectF((::FLOAT)x, (::FLOAT)y, (::FLOAT)_fontSize * wcslen(_textBuf), (::FLOAT)y), _b.Get());
      }
    }

    void Image(int index, int x, int y, int w, int h, float opacity = 1.0F) {
      if (_target) {
        if (_bitmap[index]) {
          //auto _s = _bitmap[index]->GetSize();
          _target->DrawBitmap(_bitmap[index].Get(), D2D1::RectF((::FLOAT)x, (::FLOAT)y, (::FLOAT)(x + w), (::FLOAT)(y + h)), opacity);
        }
      }
    }

    void Resize(int w, int h) {
      if (_target) {
        _target->Resize(D2D1::SizeU(w, h));
      }
    }

  private:
    ::LPCWSTR _locale = L"ja_jp";
    static constexpr int _textBufNum = 256;
    wchar_t _textBuf[_textBufNum] = {};
    ::FLOAT _fontSize = 0.0F;
    ::ComPtr<::ID2D1HwndRenderTarget> _target;
    ::ComPtr<::IDWriteTextFormat> _textFormat;
    ::ComPtr<::ID2D1Bitmap> _bitmap[10];

    ::ComPtr<::ID2D1Bitmap> _loadImage(::ID2D1RenderTarget *render, ::IWICImagingFactory *wicFactory, ::LPCWSTR uri) {
      ::ComPtr<::IWICBitmapDecoder> _decoder;
      ::ComPtr<::IWICBitmapFrameDecode> _frame;
      ::ComPtr<::IWICFormatConverter> _converter;
      ::ComPtr<::ID2D1Bitmap> _bitmap;

      // 画像デコード
      wicFactory->CreateDecoderFromFilename(uri, nullptr, GENERIC_READ, ::WICDecodeMetadataCacheOnLoad, &_decoder);
      _decoder->GetFrame(0, &_frame);

      // Direct2Dが扱えるPixelFormatに変換
      wicFactory->CreateFormatConverter(&_converter);
      _converter->Initialize(_frame.Get(), GUID_WICPixelFormat32bppPBGRA, ::WICBitmapDitherTypeNone, nullptr, 0.0, ::WICBitmapPaletteTypeCustom);

      // Direct2D Bitmap作成
      render->CreateBitmapFromWicBitmap(_converter.Get(), nullptr, &_bitmap);
      return _bitmap;
    }
  };

  /// <summary>
  /// コンストラクター
  /// </summary>
  Draw() {
    auto _ret = ::CoInitialize(NULL);
    if (FAILED(_ret)) {
      return;
    }
    // Direct2Dファクトリーの作成
    _ret = D2D1CreateFactory(::D2D1_FACTORY_TYPE_SINGLE_THREADED, IID_PPV_ARGS(&_d2d1Factory));
    if (FAILED(_ret)) {
      return;
    }
    // WICファクトリーの作成
    _ret = CoCreateInstance(CLSID_WICImagingFactory, nullptr, ::CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&_wicFactory));
    if (FAILED(_ret)) {
      return;
    }
    // DirectWriteファクトリーの作成
    _ret = DWriteCreateFactory(::DWRITE_FACTORY_TYPE_SHARED, __uuidof(::IDWriteFactory), (::IUnknown**)_dWriteFactory.GetAddressOf());
    if (FAILED(_ret)) {
      return;
    }
  }

  /// <summary>
  /// デストラクター
  /// </summary>
  ~Draw() {
    ::CoUninitialize();
  }

  Render *CreateRender(::HWND windowHandle, ::LPCWSTR fontFamily, float fontSize, ::LPCWSTR *images, int imageNum) {
    // レンダリングターゲットを作成
    auto _render = new Render(windowHandle, _d2d1Factory, _dWriteFactory, fontFamily, fontSize, _wicFactory, images, imageNum);
    if (_render == nullptr) {
      return nullptr;
    }
    return _render;
  }

  void DiscardRender(Render *render) {
    delete render;
    render = nullptr;
  }

private:
  ::ComPtr<::ID2D1Factory> _d2d1Factory;
  ::ComPtr<::IWICImagingFactory> _wicFactory;
  ::ComPtr<::IDWriteFactory> _dWriteFactory;
};
