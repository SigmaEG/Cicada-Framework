#ifndef CIC_WINDOW_H
#define CIC_WINDOW_H

#include "cic_globals.h"

#ifdef __cplusplus
extern "C" {
#endif

  cic_window* cic_createWindow(
    wchar_t* _TITLE,
    wchar_t* _ID,
    cic_point _POSITION,
    cic_size _SIZE
  );
  cic_window* cic_createWindowCRT(
    wchar_t* _TITLE,
    wchar_t* _ID,
    cic_point _POSITION,
    cic_size _SIZE,
    CIC_CALLBACK _CRT_FUNC_REF
  );

  HWND cic_getWindowHandle(cic_window* _SELF);
  cic_widget** cic_getWindowWidgetRef(cic_window* _SELF);

  signed int cic_startWindowMessageLoop(cic_window** _SELF);

  bool cic_setWindowPosition(
    cic_window** _SELF,
    cic_point _POINT
  );

  bool cic_setWindowX(
    cic_window** _SELF,
    signed int _X
  );
  bool cic_setWindowY(
    cic_window** _SELF,
    signed int _Y
  );
  bool cic_setWindowSize(
    cic_window** _SELF,
    cic_size _SIZE
  );

  bool cic_setWindowWidth(
    cic_window** _SELF,
    signed int _WIDTH
  );
  bool cic_setWindowHeight(
    cic_window** _SELF, 
    signed int _HEIGHT
  );
  bool cic_setWindowW(
    cic_window** _SELF,
    signed int _WIDTH
  );
  bool cic_setWindowH(
    cic_window** _SELF,
    signed int _HEIGHT
  );

  bool cic_windowToCenterScreen(cic_window** _SELF);
  bool cic_windowFullscreenMode(cic_window** _SELF);
  bool cic_maximizeWindow(cic_window** _SELF);
  bool cic_minimizeWindow(cic_window** _SELF);
  bool cic_iconizeWindow(cic_window** _SELF);
  bool cic_setWindowTitle(
    cic_window** _SELF,
    const wchar_t* _TITLE
  );

  bool cic_showWindow(cic_window** _SELF);
  bool cic_hideWindow(cic_window** _SELF);


  cic_size cic_getWindowSize(cic_window* _SELF);
  signed int cic_getWindowWidth(cic_window* _SELF);
  signed int cic_getWindowHeight(cic_window* _SELF);
  signed int cic_getWindowW(cic_window* _SELF);
  signed int cic_getWindowH(cic_window* _SELF);

  cic_point cic_getWindowPosition(cic_window* _SELF);
  signed int cic_getWindowX(cic_window* _SELF);
  signed int cic_getWindowY(cic_window* _SELF);

  cic_point cic_getWindowCenter(cic_window* _SELF);
  signed int cic_getWindowCenterX(cic_window* _SELF);
  signed int cic_getWindowCenterY(cic_window* _SELF);

  bool cic_isWindowFullscreenMode(cic_window* _SELF);
  bool cic_isWindowMaximized(cic_window* _SELF);
  bool cic_isWindowMinimized(cic_window* _SELF);
  bool cic_isWindowIconized(cic_window* _SELF);

  void cic_exitWindow(cic_window** _SELF);

  bool cic_restoreWindow(cic_window** _SELF);
  bool cic_redrawWindow(cic_window** _SELF);
  bool cic_destroyWindow(cic_window** _SELF);

#ifdef __cplusplus
}
#endif

#endif
