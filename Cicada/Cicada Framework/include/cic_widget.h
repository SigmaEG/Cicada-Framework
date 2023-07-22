#ifndef CIC_WIDGET_H
#define CIC_WIDGET_H

#include "cic_globals.h"

#ifdef __cplusplus
extern "C" {
#endif

  cic_widget* cic_createWidget(
    cic_widget* _PARENT,
    wchar_t* _TITLE,
    wchar_t* _ID,
    cic_point _POSITION,
    cic_size _SIZE,
    bool _APICALL
  );

  HWND cic_getWidgetHandle(cic_widget* _SELF);

  bool cic_toCenterXWidget(
    cic_widget** _SELF,
    cic_widget* _SRC
  );
  bool cic_toCenterYWidget(
    cic_widget** _SELF,
    cic_widget* _SRC
  );
  bool cic_toBelowOfWidget(
    cic_widget** _SELF,
    cic_widget* _SRC,
    cic_point _OFFSET
  );
  bool cic_toAboveOfWidget(
    cic_widget** _SELF,
    cic_widget* _SRC,
    cic_point _OFFSET
  );
  bool cic_toRightOfWidget(
    cic_widget** _SELF,
    cic_widget* _SRC,
    cic_point _OFFSET
  );
  bool cic_toLeftOfWidget(
    cic_widget** _SELF,
    cic_widget* _SRC,
    cic_point _OFFSET
  );
  bool cic_toCenterOfParentWidget(
    cic_widget** _SELF,
    cic_point _OFFSET
  );
  bool cic_toSizeOfParentWidget(
    cic_widget** _SELF,
    cic_size _OFFSET
  );
  bool cic_toCenterOfWidget(
    cic_widget** _SELF,
    cic_widget* _SRC,
    cic_point _OFFSET
  );
  bool cic_toSizeOfWidget(
    cic_widget** _SELF,
    cic_widget* _SRC,
    cic_size _OFFSET
  );
  bool cic_setWidgetPosition(
    cic_widget** _SELF,
    cic_point _POSITION
  );
  bool cic_setWidgetX(
    cic_widget** _SELF,
    signed int _X
  );
  bool cic_setWidgetY(
    cic_widget** _SELF,
    signed int _Y
  );
  bool cic_setWidgetSize(
    cic_widget** _SELF,
    cic_size _SIZE
  );
  bool cic_setWidgetWidth(
    cic_widget** _SELF,
    signed int _WIDTH
  );
  bool cic_setWidgetHeight(
    cic_widget** _SELF,
    signed int _HEIGHT
  );
  bool cic_setWidgetW(
    cic_widget** _SELF,
    signed int _WIDTH
  );
  bool cic_setWidgetH(
    cic_widget** _SELF,
    signed int _HEIGHT
  );
  bool cic_setWidgetLabel(
    cic_widget** _SELF,
    const wchar_t* _TITLE
  );
  bool cic_setWidgetTooltip(
    cic_widget** _SELF,
    const wchar_t* _TOOLTIP
  );
  bool cic_setWidgetLabelAlignment(
    cic_widget** _SELF,
    cic_textAlignment _ALIGNMENT
  );
  bool cic_setWidgetForegroundColor(
    cic_widget** _SELF,
    cic_color _COLOR
  );
  bool cic_setWidgetBackgroundColor(
    cic_widget** _SELF,
    cic_color _COLOR
  );
  bool cic_setWidgetSelectionColor(
    cic_widget** _SELF,
    cic_color _COLOR
  );
  bool cic_setWidgetLabelFont(
    cic_widget** _SELF,
    cic_font _FONT
  );
  bool cic_setWidgetFontSize(
    cic_widget** _SELF,
    signed int SIZE
  );
  bool cic_setWidgetShape(
    cic_widget** _WIDGET,
    cic_region* _REGION
  );

  bool cic_redrawWidget(cic_widget** _SELF);

  cic_point cic_getWidgetPosition(cic_widget* _SELF);
  signed int cic_getWidgetX(cic_widget* _SELF);
  signed int cic_getWidgetY(cic_widget* _SELF);

  cic_size cic_getWidgetSize(cic_widget* _SELF);
  signed int cic_getWidgetWidth(cic_widget* _SELF);
  signed int cic_getWidgetHeight(cic_widget* _SELF);

  signed int cic_getWidgetW(cic_widget* _SELF);
  signed int cic_getWidgetH(cic_widget* _SELF);

  signed int cic_getWidgetMouseWheelDelta(cic_widget* _SELF);

  const wchar_t* cic_getWidgetLabel(cic_widget* _SELF);
  const wchar_t* cic_getWidgetTooltip(cic_widget* _SELF);
  cic_textAlignment cic_getWidgetLabelAlignment(cic_widget* _SELF);
  cic_color cic_getWidgetForegroundColor(cic_widget* _SELF);
  cic_color cic_getWidgetBackgroundColor(cic_widget* _SELF);
  cic_color cic_getWidgetSelectionColor(cic_widget* _SELF);
  cic_font cic_getWidgetLabelFont(cic_widget* _SELF);
  signed int cic_getWidgetFontSize(cic_widget* _SELF);

  cic_point cic_getWidgetCenter(cic_widget* _SELF);
  signed int cic_getWidgetCenterX(cic_widget* _SELF);
  signed int cic_getWidgetCenterY(cic_widget* _SELF);

  bool cic_isWidgetFocused(cic_widget* _SELF);
  bool cic_isWidgetVisible(cic_widget* _SELF);
  bool cic_isWidgetEnabled(cic_widget* _SELF);
  bool cic_isWidgetDraggable(cic_widget* _SELF);

  cic_widgetType cic_getWidgetType(cic_widget* _SELF);

  cic_widget* cic_getWidgetParent(cic_widget* _SRC_WIDGET);
  cic_widget* cic_getWidgetWindow(cic_widget* _SRC_WIDGET);
  cic_widget* cic_getWidgetTopWindow(cic_widget* _SRC_WIDGET);

  bool cic_destroyWidget(cic_widget** _SRC_WIDGET);

#ifdef __cplusplus
}
#endif

#endif
