#pragma once

#include <windows.h>
#include "Dialog.hpp"
#include "Draw.hpp"

class App {
public:
  typedef struct {
  #if defined(_WINDOWS)
    ::HINSTANCE InstanceHandle;
    ::HINSTANCE PrevInstanceHandle;
    ::PWSTR CmdLine;
    int CmdShow;
  #elif defined(_WINDOWS)
    int Argc;
    ::PWSTR Argv[];
    ::PWSTR Envp[];
  #endif
  } Param;

  App() {}

  ~App() {}

  /// <summary>
  /// https://learn.microsoft.com/ja-jp/windows/win32/api/winbase/nf-winbase-winmain
  /// https://learn.microsoft.com/ja-jp/cpp/c-language/using-wmain?view=msvc-170
  /// </summary>
  /// <param name="Param"></param>
  /// <returns></returns>
  int Run(Param &Param) {
    auto _dialog = new Dialog();
    Dialog::Param _dp{
      .parentWindowHandle = nullptr,
      .title = L"ƒeƒXƒg",
      .modal = true,
      .showNum = 0,
      .showMode = Dialog::Normal,
      .x = 0,
      .y = 0,
      .width = 300,
      .height = 200,
      .initDialogCb = _onInitDialog,
      .paintCb = _onPaint,
    };
    _dialog->Create(_dp);
    delete _dialog;
    return 0;
  }

private:
  Draw *_draw = nullptr;
};
