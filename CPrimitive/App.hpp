#pragma once

#include <windows.h>

class App {
public:
  App() {}

  ~App() {}

  /// <summary>
  /// https://learn.microsoft.com/ja-jp/windows/win32/api/winbase/nf-winbase-winmain
  /// </summary>
  /// <param name="inst"></param>
  /// <param name="prevInst"></param>
  /// <param name="cmdLine"></param>
  /// <param name="cmdShow"></param>
  /// <returns></returns>
  int Run(HINSTANCE inst, HINSTANCE prevInst, PWSTR cmdLine, int cmdShow) {
    return 0;
  }

  /// <summary>
  /// https://learn.microsoft.com/ja-jp/cpp/c-language/using-wmain?view=msvc-170
  /// </summary>
  /// <returns></returns>
  int Run(int argc, PWSTR argv[], PWSTR envp[]) {
    return 0;
  }

  int Run(int argc, PWSTR argv[]) {
    return Run(argc, argv, nullptr);
  }

private:
};
