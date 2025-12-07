#pragma once

#include <windows.h>

/// <summary>
/// ダイアログクラス
/// </summary>
class Dialog {
public:
  typedef enum : int {
    Inactive = WA_INACTIVE,
    Active = WA_ACTIVE,
    ClickActive = WA_CLICKACTIVE
  } ActiveState;

  typedef enum : int {
    Back = VK_BACK,
    Return = VK_RETURN,
    Escape = VK_ESCAPE,
    Space = VK_SPACE,
    Exclamation = '!',
    Quotation = '"',
    Number = '#',
    Dollar = '$',
    Percent = '%',
    Ampersand = '&',
    Apostrophe = '\'',
    LeftParenthesis = '(',
    RightParenthesis = ')',
    Asterisk = '*',
    Plus = '+',
    Comma = ',',
    Hyphen = '-',
    Period = '.',
    Slash = '/',
    Digit0 = '0',
    Digit1 = '1',
    Digit2 = '2',
    Digit3 = '3',
    Digit4 = '4',
    Digit5 = '5',
    Digit6 = '6',
    Digit7 = '7',
    Digit8 = '8',
    Digit9 = '9',
    Colon = ':',
    Semicolon = ';',
    LessThan = '<',
    Equal = '=',
    GreaterThan = '>',
    Question = '?',
    At = '@',
    A = 'A',
    B = 'B',
    C = 'C',
    D = 'D',
    E = 'E',
    F = 'F',
    G = 'G',
    H = 'H',
    I = 'I',
    J = 'J',
    K = 'K',
    L = 'L',
    M = 'M',
    N = 'N',
    O = 'O',
    P = 'P',
    Q = 'Q',
    R = 'R',
    S = 'S',
    T = 'T',
    U = 'U',
    V = 'V',
    W = 'W',
    X = 'X',
    Y = 'Y',
    Z = 'Z',
    LeftSquare = '[',
    BackSlash = '\\',
    RightSquare = ']',
    Caret = '^',
    Underscore = '_',
    GraveAccent = '`',
    a = 'a',
    b = 'b',
    c = 'c',
    d = 'd',
    e = 'e',
    f = 'f',
    g = 'g',
    h = 'h',
    i = 'i',
    j = 'j',
    k = 'k',
    l = 'l',
    m = 'm',
    n = 'n',
    o = 'o',
    p = 'p',
    q = 'q',
    r = 'r',
    s = 's',
    t = 't',
    u = 'u',
    v = 'v',
    w = 'w',
    x = 'x',
    y = 'y',
    z = 'z',
    LeftCurly = '{',
    VerticalBar = '|',
    RightCurly = '}',
    Tilde = '~',
    Unknown
  } KeyCode;

  typedef enum : int {
    ParentClosing = SW_PARENTCLOSING,
    OtherZoom = SW_OTHERZOOM,
    ParentOpening = SW_PARENTOPENING,
    OtherUnzoom = SW_OTHERUNZOOM
  } ShowState;

  typedef enum : int {
    Restored = SIZE_RESTORED,
    Minimized = SIZE_MINIMIZED,
    Maximized = SIZE_MAXIMIZED,
    MaxShow = SIZE_MAXSHOW,
    MaxHide = SIZE_MAXHIDE
  } SizeState;

  typedef enum : int {
    Normal,
    Maximize,
    Fullscreen
  } ShowMode;

  /// <summary>
  /// パラメーター構造体
  /// </summary>
  typedef struct {
    ::HWND parentWindowHandle;
    ::LPCWSTR title;
    bool modal;
    int showNum;
    ShowMode showMode;
    short x;
    short y;
    short width;
    short height;
    void (*activateCb)(::HWND windowHandle, ActiveState activeState, ::HWND activatedWindowHandle);
    void (*charCb)(::HWND windowHandle, KeyCode code, ::WORD repeatCount, ::WORD scanCode, bool isExtended, bool dialogMode, bool menuMode, bool altDown, bool repeat, bool up);
    bool (*closeCb)(::HWND windowHandle);
    void (*destroyCb)(::HWND windowHandle);
    void (*enableCb)(::HWND windowHandle, bool enabled);
    bool (*eraseBkGndCb)(::HWND windowHandle, ::HDC dcHandle);
    bool (*initDialogCb)(::HWND windowHandle);
    void (*killFocusCb)(::HWND windowHandle, ::HWND receivedKbFocusWindowHandle);
    void (*moveCb)(::HWND windowHandle, int x, int y);
    void (*paintCb)(::HWND windowHandle);
    void (*setFocusCb)(::HWND windowHandle, ::HWND lostKbFocusWindowHandle);
    void (*setRedrawCb)(::HWND windowHandle, bool redraw);
    void (*showWindowCb)(::HWND windowHandle, bool enabled, ShowState showState);
    void (*sizeCb)(::HWND windowHandle, SizeState sizeState, int width, int height);
  } Param;

  /// <summary>
  /// コンストラクター
  /// </summary>
  Dialog() {}

  /// <summary>
  /// デストラクター
  /// </summary>
  ~Dialog() {
    destroy();
  }

  /// <summary>
  /// ダイアログの生成
  /// </summary>
  /// <param name="param"></param>
  /// <returns></returns>
  bool Create(const Param &param) {
    // 作成済みの場合は、成功を返す
    if (_dialog != nullptr) {
      return true;
    }

    // ダイアログのサイズを算出
    auto _size = sizeof(::DLGTEMPLATE);
    _size += sizeof(wchar_t);
    _size += sizeof(wchar_t);
    _size += (::wcslen(param.title) + 1) * sizeof(wchar_t);

    // ダイアログのメモリーを確保
    _dialog = (::LPDLGTEMPLATEW)new ::byte[_size];
    if (_dialog == nullptr) {
      return false;
    }

    // ダイアログを設定
    if (param.modal) {
      if (param.showMode == Fullscreen) {
        _dialog->style = WS_VISIBLE;
      } else {
        _dialog->style = WS_OVERLAPPEDWINDOW;
      }
    } else {
      if (param.showMode == Fullscreen) {
        _dialog->style = WS_POPUP | WS_VISIBLE;
      } else {
        _dialog->style = WS_POPUP | WS_OVERLAPPEDWINDOW | WS_VISIBLE;
      }
    }
    _dialog->dwExtendedStyle = 0;
    _dialog->cdit = 0;
    _dialog->x = param.x;
    _dialog->y = param.y;
    _dialog->cx = param.width;
    _dialog->cy = param.height;
    auto _p = (::WORD*)(_dialog + 1);
    *_p++ = 0;
    *_p++ = 0;
    wcsncpy_s((::LPWSTR)_p, wcslen(param.title) + 1, param.title, _TRUNCATE);

    // コールバックを設定
    _activateCb = param.activateCb;
    _charCb = param.charCb;
    _closeCb = param.closeCb;
    _destroyCb = param.destroyCb;
    _enableCb = param.enableCb;
    _eraseBkGndCb = param.eraseBkGndCb;
    _initDialogCb = param.initDialogCb;
    _killFocusCb = param.killFocusCb;
    _moveCb = param.moveCb;
    _paintCb = param.paintCb;
    _setFocusCb = param.setFocusCb;
    _setRedrawCb = param.setRedrawCb;
    _showWindowCb = param.showWindowCb;
    _sizeCb = param.sizeCb;

    // ダイアログの生成(thisポインターを渡す)
    _modal = param.modal;
    _showNum = param.showNum;
    _showMode = param.showMode;
    if (param.modal) {
      return ::DialogBoxIndirectParamW(::GetModuleHandleW(nullptr), _dialog, param.parentWindowHandle, _dialogBaseProc, (::LPARAM)this);
    } else {
      if (::CreateDialogIndirectParamW(::GetModuleHandleW(nullptr), _dialog, param.parentWindowHandle, _dialogBaseProc, (::LPARAM)this) == nullptr) {
        return false;
      }
    }

    return true;
  }

  /// <summary>
  /// ダイアログの削除
  /// </summary>
  void destroy() {
    if (_dialog != nullptr) {
      delete _dialog;
      _dialog = nullptr;
    }
  }

private:
  typedef struct {
    ::RECT rc;
    int num;
  } _Monitor;

  static constexpr byte _extendedPrefix = 0xE0;
  ::LPDLGTEMPLATE _dialog = nullptr;
  bool _modal = false;
  int _showNum = 0;
  ShowMode _showMode = Normal;

  KeyCode _getKeyCode(::WPARAM wp, ::WORD scanCode) {
    switch (LOWORD(wp)) {
      case VK_BACK: return Back;
      case VK_RETURN: return Return;
      case VK_SHIFT:
      case VK_CONTROL:
      case VK_MENU: return (KeyCode)LOWORD(MapVirtualKeyW(scanCode, MAPVK_VSC_TO_VK_EX));
      case VK_ESCAPE: return Escape;
      case VK_SPACE: return Space;
      case '!': return Exclamation;
      case '"': return Quotation;
      case '#': return Number;
      case '$': return Dollar;
      case '%': return Percent;
      case '&': return Ampersand;
      case '\'': return Apostrophe;
      case '(': return LeftParenthesis;
      case ')': return RightParenthesis;
      case '*': return Asterisk;
      case '+': return Plus;
      case ',': return Comma;
      case '-': return Hyphen;
      case '.': return Period;
      case '/': return Slash;
      case '0': return Digit0;
      case '1': return Digit1;
      case '2': return Digit2;
      case '3': return Digit3;
      case '4': return Digit4;
      case '5': return Digit5;
      case '6': return Digit6;
      case '7': return Digit7;
      case '8': return Digit8;
      case '9': return Digit9;
      case ':': return Colon;
      case ';': return Semicolon;
      case '<': return LessThan;
      case '=': return Equal;
      case '>': return GreaterThan;
      case '?': return Question;
      case '@': return At;
      case 'A': return A;
      case 'B': return B;
      case 'C': return C;
      case 'D': return D;
      case 'E': return E;
      case 'F': return F;
      case 'G': return G;
      case 'H': return H;
      case 'I': return I;
      case 'J': return J;
      case 'K': return K;
      case 'L': return L;
      case 'M': return M;
      case 'N': return N;
      case 'O': return O;
      case 'P': return P;
      case 'Q': return Q;
      case 'R': return R;
      case 'S': return S;
      case 'T': return T;
      case 'U': return U;
      case 'V': return V;
      case 'W': return W;
      case 'X': return X;
      case 'Y': return Y;
      case 'Z': return Z;
      case '[': return LeftSquare;
      case '\\': return BackSlash;
      case ']': return RightSquare;
      case '^': return Caret;
      case '_': return Underscore;
      case '`': return GraveAccent;
      case 'a': return a;
      case 'b': return b;
      case 'c': return c;
      case 'd': return d;
      case 'e': return e;
      case 'f': return f;
      case 'g': return g;
      case 'h': return h;
      case 'i': return i;
      case 'j': return j;
      case 'k': return k;
      case 'l': return l;
      case 'm': return m;
      case 'n': return n;
      case 'o': return o;
      case 'p': return p;
      case 'q': return q;
      case 'r': return r;
      case 's': return s;
      case 't': return t;
      case 'u': return u;
      case 'v': return v;
      case 'w': return w;
      case 'x': return x;
      case 'y': return y;
      case 'z': return z;
      case '{': return LeftCurly;
      case '|': return VerticalBar;
      case '}': return RightCurly;
      case '~': return Tilde;
      default: return Unknown;
    }
  }

  void (*_activateCb)(::HWND windowHandle, ActiveState activeState, ::HWND activatedWindowHandle) = nullptr;
  void (*_charCb)(::HWND windowHandle, KeyCode code, ::WORD repeatCount, ::WORD scanCode, bool isExtended, bool dialogMode, bool menuMode, bool altDown, bool repeat, bool up) = nullptr;
  bool (*_closeCb)(::HWND windowHandle) = nullptr;
  void (*_destroyCb)(::HWND windowHandle) = nullptr;
  void (*_enableCb)(::HWND windowHandle, bool enabled) = nullptr;
  bool (*_eraseBkGndCb)(::HWND windowHandle, ::HDC dcHandle) = nullptr;
  bool (*_initDialogCb)(::HWND windowHandle) = nullptr;
  void (*_killFocusCb)(::HWND windowHandle, ::HWND receivedKbFocusWindowHandle) = nullptr;
  void (*_moveCb)(::HWND windowHandle, int x, int y) = nullptr;
  void (*_paintCb)(::HWND windowHandle) = nullptr;
  void (*_setFocusCb)(::HWND windowHandle, ::HWND lostKbFocusWindowHandle) = nullptr;
  void (*_setRedrawCb)(::HWND windowHandle, bool redraw) = nullptr;
  void (*_showWindowCb)(::HWND windowHandle, bool enabled, ShowState showState) = nullptr;
  void (*_sizeCb)(::HWND windowHandle, SizeState sizeState, int width, int height) = nullptr;

  ::INT_PTR _onActivate(::HWND windowHandle, ::WPARAM wp, ::LPARAM lp) {
    if (_activateCb != nullptr) {
      _activateCb(windowHandle, (ActiveState)LOWORD(wp), (HWND)lp);
    }
    return false;
  }

  ::INT_PTR _onChar(::HWND windowHandle, ::WPARAM wp, ::LPARAM lp) {
    if (_charCb != nullptr) {
      auto _flags = HIWORD(lp);
      auto _isExtended = (_flags & KF_EXTENDED) == KF_EXTENDED;
      auto _scanCode = _isExtended ? MAKEWORD(LOBYTE(_flags), _extendedPrefix) : (::WORD)LOBYTE(_flags);
      auto _dialogMode = (_flags & KF_DLGMODE) == KF_DLGMODE;
      auto _menuMode = (_flags & KF_MENUMODE) == KF_MENUMODE;
      auto _altDown = (_flags & KF_ALTDOWN) == KF_ALTDOWN;
      auto _repeat = (_flags & KF_REPEAT) == KF_REPEAT;
      auto _up = (_flags & KF_UP) == KF_UP;
      _charCb(windowHandle, _getKeyCode(wp, _scanCode), LOWORD(lp), _scanCode, _isExtended, _dialogMode, _menuMode, _altDown, _repeat, _up);
    }
    return false;
  }

  ::INT_PTR _onClose(::HWND windowHandle) {
    auto _ret = false;
    if (_closeCb != nullptr) {
      _ret = _closeCb(windowHandle);
    }
    _modal ? ::EndDialog(windowHandle, _ret) : ::DestroyWindow(windowHandle);
    return false;
  }

  ::INT_PTR _onDestroy(::HWND windowHandle) {
    if (_destroyCb != nullptr) {
      _destroyCb(windowHandle);
    }
    destroy();
    return false;
  }

  ::INT_PTR _onEnable(::HWND windowHandle, ::WPARAM wp) {
    if (_enableCb != nullptr) {
      _enableCb(windowHandle, (bool)wp);
    }
    return false;
  }

  ::INT_PTR _onEraseBkGnd(::HWND windowHandle, ::WPARAM wp) {
    auto _ret = false;
    if (_eraseBkGndCb != nullptr) {
      _ret = _eraseBkGndCb(windowHandle, (HDC)wp);
    }
    return _ret;
  }

  ::INT_PTR _onInitDialog(::HWND windowHandle) {
    auto _ret = false;
    _Monitor _m { .num = _showNum };
    ::EnumDisplayMonitors(nullptr, nullptr, _monitorEnumProc, (::LPARAM)&_m);
    if (_showMode == Fullscreen) {
      ::MoveWindow(windowHandle, _m.rc.left, _m.rc.top, _m.rc.right, _m.rc.bottom, false);
    } else if (_showMode == Maximize) {
      ::MoveWindow(windowHandle, _m.rc.left, _m.rc.top, _m.rc.right, _m.rc.bottom, false);
      ::ShowWindow(windowHandle, SW_MAXIMIZE);
    }
    if (_initDialogCb != nullptr) {
      _ret = _initDialogCb(windowHandle);
    }
    return _ret;
  }

  ::INT_PTR _onKillFocus(::HWND windowHandle, ::WPARAM wp) {
    if (_killFocusCb != nullptr) {
      _killFocusCb(windowHandle, (HWND)wp);
    }
    return false;
  }

  ::INT_PTR _onMove(::HWND windowHandle, ::LPARAM lp) {
    if (_moveCb != nullptr) {
      _moveCb(windowHandle, (int)LOWORD(lp), (int)HIWORD(lp));
    }
    return false;
  }

  ::INT_PTR _onPaint(::HWND windowHandle) {
    if (_paintCb != nullptr) {
      _paintCb(windowHandle);
    }
    return false;
  }

  ::INT_PTR _onSetFocus(::HWND windowHandle, ::WPARAM wp) {
    if (_setFocusCb != nullptr) {
      _setFocusCb(windowHandle, (HWND)wp);
    }
    return false;
  }

  ::INT_PTR _onSetRedraw(::HWND windowHandle, ::WPARAM wp) {
    if (_setRedrawCb != nullptr) {
      _setRedrawCb(windowHandle, (bool)wp);
    }
    return false;
  }

  ::INT_PTR _onShowWindow(::HWND windowHandle, ::WPARAM wp, ::LPARAM lp) {
    if (_showWindowCb != nullptr) {
      _showWindowCb(windowHandle, (bool)wp, (ShowState)lp);
    }
    return false;
  }

  ::INT_PTR _onSize(::HWND windowHandle, ::WPARAM wp, ::LPARAM lp) {
    if (_sizeCb != nullptr) {
      _sizeCb(windowHandle, (SizeState)wp, (int)LOWORD(lp), (int)HIWORD(lp));
    }
    return false;
  }

  ::INT_PTR CALLBACK _dialogProc(::HWND windowHandle, ::UINT msg, ::WPARAM wp, ::LPARAM lp) {
    auto _ret = false;
    switch (msg) {
      case WM_ACTIVATE: _ret = _onActivate(windowHandle, wp, lp); break;
      case WM_CHAR: _ret = _onChar(windowHandle, wp, lp); break;
      case WM_CLOSE: _ret = _onClose(windowHandle); break;
      case WM_DESTROY: _ret = _onDestroy(windowHandle); break;
      case WM_ENABLE: _ret = _onEnable(windowHandle, wp); break;
      case WM_ERASEBKGND: _ret = _onEraseBkGnd(windowHandle, wp); break;
      //case WM_GETDLGCODE: _ret = DLGC_WANTALLKEYS; break;
      case WM_INITDIALOG: _ret = _onInitDialog(windowHandle); break;
      case WM_KEYDOWN: _ret = false;  break;
      case WM_KILLFOCUS: _ret = _onKillFocus(windowHandle, wp); break;
      case WM_MOVE: _ret = _onMove(windowHandle, lp); break;
      case WM_PAINT: _ret = _onPaint(windowHandle); break;
      case WM_SETFOCUS: _ret = _onSetFocus(windowHandle, wp); break;
      case WM_SETREDRAW: _ret = _onSetRedraw(windowHandle, wp); break;
      case WM_SHOWWINDOW: _ret = _onShowWindow(windowHandle, wp, lp); break;
      case WM_SIZE: _ret = _onSize(windowHandle, wp, lp); break;
      default: break;
    }
    return _ret;
  }

  static ::INT_PTR CALLBACK _dialogBaseProc(::HWND windowHandle, ::UINT msg, ::WPARAM wp, ::LPARAM lp) {
    auto _ret = false;
    // thisポインター取得
    auto _this = (Dialog*)(::GetWindowLongPtrW(windowHandle, GWLP_USERDATA));
    if (_this == nullptr) {
      if (msg == WM_INITDIALOG) {
        // ダイアログの生成時に渡したthisポインターを設定
        ::SetWindowLongPtrW(windowHandle, GWLP_USERDATA, lp);
        _this = (Dialog*)lp;
      }
    }
    if (_this != nullptr) {
      // メッセージ処理
      _ret = _this->_dialogProc(windowHandle, msg, wp, lp);
    }
    return _ret;
  }

  static ::BOOL CALLBACK _monitorEnumProc(::HMONITOR monitorHandle, ::HDC dcHandle, ::LPRECT lr, ::LPARAM lp) {
    static auto _i = 0;
    ::MONITORINFOEX _mi {};
    _mi.cbSize = sizeof(_mi);
    if (::GetMonitorInfoW(monitorHandle, &_mi)) {
      auto _m = (_Monitor*)lp;
      if (_i == 0) {
        _m->rc = _mi.rcMonitor;
        if (_i == _m->num) {
          return FALSE;
        }
      } else if (_i == _m->num) {
        _m->rc = _mi.rcMonitor;
        return FALSE;
      }
    }
    return TRUE;
  }
};
