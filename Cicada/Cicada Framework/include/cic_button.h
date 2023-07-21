#ifndef CIC_BUTTON_H
#define CIC_BUTTON_H

#include "cic_globals.h"

#ifdef __cplusplus
extern "C" {
#endif

  cic_button* cic_createButton(
    cic_widget* _PARENT,
    const wchar_t* _TEXT,
    const wchar_t* _ID,
    cic_point _POSITION,
    cic_size _SIZE,
    bool _OWNER_DRAW
  );

  HWND cic_getButtonHandle(cic_button* _SELF);
  cic_widget** cic_getButtonWidgetRef(cic_button* _SELF);

  bool cic_setButtonShortcut(
    cic_button** _SELF,
    DWORD* _VK_KEYCODES,
    size_t _nVK_KEYCODES
  );

  bool cic_setButtonStyle(
    cic_button** _SELF,
    DWORD _STYLE
  );

  bool cic_destroyButton(cic_button** _SELF);

#ifdef __cplusplus
}
#endif

#endif
