#ifndef CIC_GLOBALS_H
#define CIC_GLOBALS_H

#define SGTM_FULLSCREEN 71409
#define SGTM_RESTORE_FS 71410
#define SGTM_MAXIMIZE 71411
#define SGTM_RESTORE_MXZ 71412
#define SGTM_MINIMIZE 71413
#define SGTM_RESTORE_MIZ 71414

#include <Windows.h>
#include <CommCtrl.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

  typedef enum _cic_WIDGET_STYLE_ENUM {
    STYLE_BORDER = WS_BORDER,
    STYLE_CAPTION = WS_CAPTION,
    STYLE_CHILD = WS_CHILD,
    STYLE_CLIPCHILDREN = WS_CLIPCHILDREN,
    STYLE_CLIPSIBLINGS = WS_CLIPSIBLINGS,
    STYLE_DISABLED = WS_DISABLED,
    STYLE_DLGFRAME = WS_DLGFRAME,
    STYLE_GROUP = WS_GROUP,
    STYLE_HSCROLL = WS_HSCROLL,
    STYLE_VSCROLL = WS_VSCROLL,
    STYLE_MINIMIZED = WS_ICONIC,
    STYLE_MAXIMIZED = WS_MAXIMIZE,
    STYLE_MAXIMIZE_BOX = WS_MAXIMIZEBOX,
    STYLE_MINIMIZE_BOX = WS_MINIMIZEBOX,
    STYLE_TITLE_AND_BORDER = WS_OVERLAPPED,
    STYLE_FULL_WINDOW = WS_OVERLAPPEDWINDOW,
    STYLE_POPUP = WS_POPUP,
    STYLE_POPUP_WINDOW = WS_POPUPWINDOW,
    STYLE_SIZING_BOX = WS_SIZEBOX,
    STYLE_MENUBAR = WS_SYSMENU,
    STYLE_TABSTOP = WS_TABSTOP,
    STYLE_VISIBLE = WS_VISIBLE,
  } cic_widgetStyle;
  typedef enum _cic_EVENT_ENUM {
    EVENT_CREATE,
    EVENT_CLOSE,
    EVENT_KEYDOWN,
    EVENT_KEYUP,
    EVENT_MOVE,
    EVENT_RESIZE,
    EVENT_FOCUS,
    EVENT_UNFOCUS,
    EVENT_ENABLE,
    EVENT_DISABLE,
    EVENT_SHOW,
    EVENT_HIDE,
    EVENT_PASTE,
    EVENT_MOUSE_ENTER,
    EVENT_MOUSE_LEAVE,
    EVENT_MOUSE_WHEEL,
    EVENT_MOUSE_MOVE,
    EVENT_MOUSE_LMB_DOWN,
    EVENT_MOUSE_LMB_UP,
    EVENT_MOUSE_LMB_DBLCLK,
    EVENT_MOUSE_RMB_DOWN,
    EVENT_MOUSE_RMB_UP,
    EVENT_MOUSE_RMB_DBLCLK,
    EVENT_MOUSE_MMB_DOWN,
    EVENT_MOUSE_MMB_UP,
    EVENT_MOUSE_MMB_DBLCLK,
    EVENT_FULLSCREEN,
    EVENT_MAXIMIZED,
    EVENT_MINIMIZED,
    EVENT_RESTORE_FROM_FULLSCREEN,
    EVENT_RESTORE_FROM_MAXIMIZED,
    EVENT_RESTORE_FROM_MINIMIZED
  } cic_event;
  typedef enum _cic_WIDGET_TYPE_ENUM {
    WTYPE_STATIC,
    WTYPE_BROWSER,
    WTYPE_BUTTON,
    WTYPE_FILE_DIALOG_OPTIONS,
    WTYPE_FILE_DIALOG,
    WTYPE_SCROLLFRAME,
    WTYPE_INPUT,
    WTYPE_RICH_INPUT,
    WTYPE_MENU_BUTTON,
    WTYPE_CHART,
    WTYPE_CLOCK,
    WTYPE_OUTPUT,
    WTYPE_TABLE_ROW_SELECT_MODE,
    WTYPE_COUNTER,
    WTYPE_DIAL,
    WTYPE_SCROLLBAR,
    WTYPE_SLIDER,
    WTYPE_WINDOW
  } cic_widgetType;
  typedef enum _cic_FONT_WEIGHT_ENUM {
    WEIGHT_DEFAULT = FW_DONTCARE,
    WEIGHT_THIN = FW_THIN,
    WEIGHT_EXTRALIGHT = FW_EXTRALIGHT,
    WEIGHT_LIGHT = FW_LIGHT,
    WEIGHT_NORMAL = FW_NORMAL,
    WEIGHT_MEDIUM = FW_MEDIUM,
    WEIGHT_SEMIBOLD = FW_SEMIBOLD,
    WEIGHT_BOLD = FW_BOLD,
    WEIGHT_EXTRABOLD = FW_EXTRABOLD,
    WEIGHT_HEAVY = FW_HEAVY,
  } cic_fontWeight;
  typedef enum _cic_FONT_QUALITY_ENUM {
    QUALITY_DEFAULT = DEFAULT_QUALITY,
    QUALITY_NON_ANTIALIASED = NONANTIALIASED_QUALITY,
    QUALITY_ANTIALIASED = ANTIALIASED_QUALITY,
    QUALITY_CLEARTYPE = CLEARTYPE_QUALITY,
    QUALITY_DRAFT = DRAFT_QUALITY,
    QUALITY_PROOF = PROOF_QUALITY,
  } cic_fontQuality;
  typedef enum _cic_CHAR_SET_ENUM {
    CHARSET_LOCALE = DEFAULT_CHARSET,
    CHARSET_ANSI = ANSI_CHARSET,
    CHARSET_BALTIC = BALTIC_CHARSET,
    CHARSET_CHINESEBIG5 = CHINESEBIG5_CHARSET,
    CHARSET_EASTEUROPE = EASTEUROPE_CHARSET,
    CHARSET_GB2312 = GB2312_CHARSET,
    CHARSET_GREEK = GREEK_CHARSET,
    CHARSET_HANGUL = HANGUL_CHARSET,
    CHARSET_MAC = MAC_CHARSET,
    CHARSET_OEM = OEM_CHARSET,
    CHARSET_RUSSIAN = RUSSIAN_CHARSET,
    CHARSET_SHIFTJIS = SHIFTJIS_CHARSET,
    CHARSET_SYMBOL = SYMBOL_CHARSET,
    CHARSET_TURKISH = TURKISH_CHARSET,
    CHARSET_VIETNAMESE = VIETNAMESE_CHARSET,
    CHARSET_JOHAB = JOHAB_CHARSET,
    CHARSET_ARABIC = ARABIC_CHARSET,
    CHARSET_HEBREW = HEBREW_CHARSET,
    CHARSET_THAI = THAI_CHARSET,
  } cic_charSet;
  typedef enum _cic_TEXT_ALIGNMENT_ENUM {
    TEXTALIGN_BASELINE = TA_BASELINE,
    TEXTALIGN_BOTTOM = TA_BOTTOM,
    TEXTALIGN_TOP = TA_TOP,
    TEXTALIGN_CENTER = TA_CENTER,
    TEXTALIGN_LEFT = TA_LEFT,
    TEXTALIGN_RIGHT = TA_RIGHT,
  } cic_textAlignment;
  typedef enum _cic_COLOR_DEPTH_ENUM {
    // Luminance/Grayscale
    CD_L8,
    // Luminance w/ Alpha Channel
    CD_La8,
    // RGB888
    CD_RGB8,
    // RGBA888 w/ Alpha Channel
    CD_RGBa8
  } cic_colorDepth;
  typedef enum _SGTR_COMBINE_MODE_ENUM {
    CMODE_INTERSECTION = RGN_AND,
    CMODE_COPY = RGN_COPY,
    CMODE_DIFFERENT = RGN_DIFF,
    CMODE_UNION = RGN_OR,
    CMODE_UNION_EXCEPT_OVERLAP = RGN_XOR,
  } cic_rCombineMode;
  typedef enum _cic_BUTTON_STYLE {
    BSTYLE_BITMAP = BS_BITMAP,
    BSTYLE_THREE_STATE = BS_AUTO3STATE,
    BSTYLE_TEXT_BOTTOM = BS_BOTTOM,
    BSTYLE_TEXT_TOP = BS_TOP,
    BSTYLE_TEXT_LEFT = BS_LEFT,
    BSTYLE_TEXT_RIGHT = BS_RIGHT,
    BSTYLE_TEXT_HCENTER = BS_CENTER,
    BSTYLE_TEXT_VCENTER = BS_VCENTER,
    BSTYLE_CHECKBOX = BS_AUTOCHECKBOX,
    BSTYLE_RIGHT_BUTTON = BS_RIGHTBUTTON,
    BSTYLE_ICON = BS_ICON,
    BSTYLE_FLAT = BS_FLAT,
    BSTYLE_TEXT_MULTILINE = BS_MULTILINE,
    BSTYLE_PUSHBUTTON = BS_PUSHBUTTON,
    BSTYLE_PUSHLIKE = BS_PUSHLIKE,
    BSTYLE_RADIOBUTTON = BS_AUTORADIOBUTTON,
    BSTYLE_SPLITBUTTON = BS_SPLITBUTTON
  } cic_buttonStyle;
  typedef struct _cic_POINT_STRUCT {
    int X;
    int Y;
  } cic_point;
  typedef struct _cic_SIZE_STRUCT {
    int WIDTH;
    int HEIGHT;
  } cic_size;
  typedef struct _cic_COLOR_STRUCT {
    float RED;
    float GREEN;
    float BLUE;
  } cic_color;
  typedef struct _cic_FONT_STRUCT {
    wchar_t* FONT_NAME;
    int FONT_SIZE;
    int ANGLE;
    cic_fontQuality QUALITY;
    cic_fontWeight WEIGHT;
    cic_charSet CHARSET;
    cic_textAlignment ALIGNMENT;
    bool IS_ITALIC;
    bool IS_UNDERLINED;
    bool HAS_STRIKEOUT;
    bool RIGHT_TO_LEFT_READ;
  } cic_font;
  typedef struct _cic_REGION_STRUCT {
    HRGN _REGION;
  } cic_region;

  typedef struct _EVENT_STRUCT cic_eventStruct;
  typedef struct _WIDGET_STRUCT cic_widget;
  typedef struct _WINDOW_STRUCT cic_window;

  typedef void (*CIC_CALLBACK)(cic_widget** _SELF);

  typedef struct _EVENT_STRUCT {
    cic_event _EVENT_TYPE;
    CIC_CALLBACK _FUNC_REF;
  } cic_eventStruct;
  typedef struct _WIDGET_STRUCT {
    void* _BASE;
    WNDPROC _PREV_PROC;

    cic_eventStruct** _EVENTS;
    size_t _nEVENTS;

    void (*DRAW)(cic_widget** _SELF, HDC _DC, PAINTSTRUCT* _PS);
    void (*HANDLE)(cic_widget** _SELF, cic_event _EVENT);

    bool _DRAGGABLE;
    bool _HAS_ENTERED;
    bool _HAS_SHAPE;
    bool _DESTROYED;

    int _WHEEL_DELTA;

    DWORD _STYLE_BEF_SHAPE;

    HWND _HANDLE;
    HFONT _HFONT;
    wchar_t* _ID;

    wchar_t* _TITLE;
    wchar_t* _TOOLTIP;
    cic_size _SIZE;
    cic_point _POSITION;
    cic_font _FONT;
    cic_textAlignment _TEXT_ALIGNMENT;
    cic_color _FG_COLOR;
    cic_color _BG_COLOR;
    cic_color _SEL_COLOR;
    cic_region* _SHAPE;

    cic_widgetType _WIDGET_TYPE;
    void* _WIDGET_UPCAST_REF;
  } cic_widget;
  typedef struct _WINDOW_STRUCT {
    cic_widget* _BASE;

    bool _FULLSCREEN_MODE;
    bool _MAXIMIZED;
    bool _MINIMIZED;

    cic_size _BFSM_SZ;
    cic_point _BFSM_PT;
    DWORD _BFSM_STYLE;
  } cic_window;
  typedef struct _BUTTON_STRUCT {
    cic_widget* _BASE;

    bool _OWNER_DRAW;

    DWORD* _SHORTCUT;
    size_t _nSHORTCUTS;
  } cic_button;

  bool cic_insertRawHandleRef(cic_widget** _WIDGET_REF);
  bool cic_removeRawHandleRef(cic_widget** _WIDGET_REF);
  cic_widget* cic_getRefByRawHandle(HWND _HANDLE);
  cic_widget* cic_getRefById(const wchar_t* _ID);

  bool cic_createEventHandler(
    cic_widget** _SELF,
    cic_event _EVENT,
    CIC_CALLBACK _FUNC_REF
  );
  CIC_CALLBACK cic_getEventHandler(
    cic_widget* _SELF,
    cic_event _EVENT
  );

  cic_region* cic_createRegion(HRGN _REGION);
  cic_region* cic_createRectRegion(
    cic_point _POSITION,
    cic_size _SIZE
  );
  cic_region* cic_createRoundRectRegion(
    cic_point _POSITION,
    cic_size _SIZE,
    int _RADIUS
  );
  cic_region* cic_createEllipticRegion(
    cic_point _POSITION,
    cic_size _RADIUSs
  );
  cic_region* cic_createPolygonRegion(
    cic_point POINTS[],
    size_t _nPOINTS,
    int _POLY_MODE
  );

  void cic_setRegion(cic_region** _REGION, HRGN _HRGN);

  HRGN cic_getHRGN(cic_region* _REGION);
  HRGN cic_getHRGNClone(cic_region* _REGION);
  bool cic_combineRegion(HRGN _DEST_REGION, HRGN _SRC_REGION, cic_rCombineMode _COMBINE_MODE);

  bool cic_destroyRegion(cic_region** _REGION);

  cic_size cic_rectToSize(RECT _RECT);
  cic_point cic_pointToPoint(POINT _POINT);

  bool cic_lpcwstrToLPWSTR(
    LPWSTR* _DEST,
    LPCWSTR _SOURCE
  );

  bool cic_setClipboard(const wchar_t* _SOURCE);
  bool cic_getClipboard(wchar_t** _DEST);

  bool cic_isKeyDown(
    int _VK_CODE,
    bool _TOGGLE_CHECK
  );

  LPCWSTR SGTWidgetTypeToLPCWSTR(cic_widgetType _TYPE);

#ifdef __cplusplus
}
#endif

#endif
