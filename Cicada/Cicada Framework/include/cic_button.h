#ifndef CIC_BUTTON_H
#define CIC_BUTTON_H

#include "cic_globals.h"

#ifdef __cplusplus
extern "C" {
#endif

  cic_button* cic_createButton(
    cic_widget* _PARENT,
    wchar_t* _TEXT,
    wchar_t* _ID,
    const cic_point _POSITION,
    const cic_size _SIZE,
    bool _OWNER_DRAW
  );

  HWND cic_getButtonHandle(cic_button* _SELF);
  cic_widget** cic_getButtonWidgetRef(cic_button* _SELF);

  bool cic_setButtonPosition(
    cic_button** _SELF,
    cic_point _POINT
  );

  bool cic_setButtonX(
    cic_button** _SELF,
    signed int _X
  );
  bool cic_setButtonY(
    cic_button** _SELF,
    signed int _Y
  );
  bool cic_setButtonSize(
    cic_button** _SELF,
    cic_size _SIZE
  );

  bool cic_setButtonWidth(
    cic_button** _SELF,
    signed int _WIDTH
  );
  bool cic_setButtonHeight(
    cic_button** _SELF,
    signed int _HEIGHT
  );
  bool cic_setButtonW(
    cic_button** _SELF,
    signed int _WIDTH
  );
  bool cic_setButtonH(
    cic_button** _SELF,
    signed int _HEIGHT
  );

  bool cic_setButtonShortcut(
    cic_button** _SELF,
    DWORD* _VK_KEYCODES,
    size_t _nVK_KEYCODES
  );

  bool cic_setButtonStyle(
    cic_button** _SELF,
    DWORD _STYLE
  );

  cic_size cic_getButtonSize(cic_button* _SELF);
  signed int cic_getButtonWidth(cic_button* _SELF);
  signed int cic_getButtonHeight(cic_button* _SELF);
  signed int cic_getButtonW(cic_button* _SELF);
  signed int cic_getButtonH(cic_button* _SELF);

  cic_point cic_getButtonPosition(cic_button* _SELF);
  signed int cic_getButtonX(cic_button* _SELF);
  signed int cic_getButtonY(cic_button* _SELF);

  cic_point cic_getButtonCenter(cic_button* _SELF);
  signed int cic_getButtonCenterX(cic_button* _SELF);
  signed int cic_getButtonCenterY(cic_button* _SELF);

  DWORD cic_getButtonCheckState(cic_button* _SELF);

  bool cic_destroyButton(cic_button** _SELF);

#ifdef __cplusplus
}
#endif

#endif
