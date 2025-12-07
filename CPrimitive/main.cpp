#include "App.hpp"

#if defined(_WINDOWS)
int WINAPI wWinMain(_In_ HINSTANCE inst, _In_opt_ HINSTANCE prevInst, _In_ PWSTR cmdLine, _In_ int cmdShow) {
#elif defined(_CONSOLE)
int _wmain(int argc, PWSTR argv[], PWSTR envp[]) {
#endif
  App *_app = new App();
  auto _ret = _app->Run(inst, prevInst, cmdLine, cmdShow);
  delete _app;
  return _ret;
}
