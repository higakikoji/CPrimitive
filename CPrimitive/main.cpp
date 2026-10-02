#include "App.hpp"

#if defined(_WINDOWS)
int WINAPI wWinMain(_In_ ::HINSTANCE InstanceHandle, _In_opt_ ::HINSTANCE PrevInstanceHandle, _In_ ::PWSTR CmdLine, _In_ int CmdShow) {
#elif defined(_CONSOLE)
int _wmain(int Argc, ::PWSTR Argv[], ::PWSTR Envp[]) {
#endif
  auto _ret = -1;
  auto _app = new App();
  if (_app) {
    App::Param _ap{
  #if defined(_WINDOWS)
      .InstanceHandle = InstanceHandle,
      .PrevInstanceHandle = PrevInstanceHandle,
      .CmdLine = CmdLine,
      .CmdShow = CmdShow,
  #elif defined(_CONSOLE)
      .Argc = Argc,
      .Argv = Argv,
      .Envp = Envp
  #endif
    };  
    _ret = _app->Run(_ap);
    delete _app;
  }
  return _ret;
}
