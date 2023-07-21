#ifndef CIC_WINDOW_H
#define CIC_WINDOW_H

#include "cic_globals.h"

#ifdef __cplusplus
extern "C" {
#endif

  cic_window* cic_createWindow(
    const wchar_t* _TITLE,
    const wchar_t* _ID,
    cic_point _POSITION,
    cic_size _SIZE
  );
  cic_window* cic_createWindowCRT(
    const wchar_t* _TITLE,
    const wchar_t* _ID,
    cic_point _POSITION,
    cic_size _SIZE,
    CIC_CALLBACK _CRT_FUNC_REF
  );

  HWND cic_getWindowHandle(cic_window* _SELF);
  cic_widget** cic_getWindowWidgetRef(cic_window* _SELF);

  int cic_startWindowMessageLoop(cic_window** _SELF);

  bool cic_windowToCenterScreen(cic_window** _SELF);
  bool cic_windowFullscreenMode(cic_window** _SELF);
  bool cic_maximizeWindow(cic_window** _SELF);
  bool cic_minimizeWindow(cic_window** _SELF);
  bool cic_iconizeWindow(cic_window** _SELF);
  bool cic_setWindowTitle(cic_window** _SELF, const wchar_t* _TITLE);

  bool cic_showWindow(cic_window** _WINDOW);
  bool cic_hideWindow(cic_window** _WINDOW);

  cic_size cic_getWindowSize(cic_window* _WINDOW);
  int cic_getWindowWidth(cic_window* _WINDOW);
  int cic_getWindowHeight(cic_window* _WINDOW);
  int cic_getWindowW(cic_window* _WINDOW);
  int cic_getWindowH(cic_window* _WINDOW);

  cic_point cic_getWindowPosition(cic_window* _WINDOW);
  int cic_getWindowX(cic_window* _WINDOW);
  int cic_getWindowY(cic_window* _WINDOW);

  bool cic_isWindowFullscreenMode(cic_window* _WINDOW);
  bool cic_isWindowMaximized(cic_window** _WINDOW);
  bool cic_isWindowMinimized(cic_window** _WINDOW);
  bool cic_isWindowIconized(cic_window** _WINDOW);

  void cic_exitWindow(cic_window** _WINDOW);

  bool cic_restoreWindow(cic_window** _WINDOW);
  bool cic_redrawWindow(cic_window** _WINDOW);
  bool cic_destroyWindow(cic_window** _WINDOW);

#ifdef __cplusplus
}
#endif

#endif
