#include "include/cic_globals.h"
#include "include/cic_window.h"

typedef struct _WIDGET_BASE_PTR_LIST {
  cic_widget** LIST;
  size_t ELEMENTS;
} cic_widgetBasePtrList;

static cic_widgetBasePtrList APP_HWND_LIST;

bool cic_insertRawHandleRef(cic_widget** _WIDGET_REF) {
  if (APP_HWND_LIST.LIST == NULL) {
    APP_HWND_LIST.LIST = (cic_widget**)calloc(1, sizeof(struct _WIDGET_STRUCT*));
    APP_HWND_LIST.LIST[0] = *_WIDGET_REF;
    APP_HWND_LIST.ELEMENTS = 1;

    return true;
  }

  size_t _FDX = -1;

  for (size_t IDX = 0; IDX < APP_HWND_LIST.ELEMENTS; ++IDX) {
    if (APP_HWND_LIST.LIST[IDX] == *_WIDGET_REF) {
      _FDX = IDX;

      break;
    }
  }

  if (_FDX != -1)
    return false;

  ++APP_HWND_LIST.ELEMENTS;
  cic_widget** _REALLOC_TEMP = (cic_widget**)realloc(APP_HWND_LIST.LIST, APP_HWND_LIST.ELEMENTS * sizeof(struct _WIDGET_STRUCT*));

  if (_REALLOC_TEMP != NULL) {
    APP_HWND_LIST.LIST = _REALLOC_TEMP;
    APP_HWND_LIST.LIST[APP_HWND_LIST.ELEMENTS - 1] = *_WIDGET_REF;
  }
  else
    return false;

  return true;
}
bool cic_removeRawHandleRef(cic_widget** _WIDGET_REF) {
  if (APP_HWND_LIST.ELEMENTS == 0)
    return false;

  size_t _FDX = -1;

  for (size_t IDX = 0; IDX < APP_HWND_LIST.ELEMENTS; ++IDX) {
    if (APP_HWND_LIST.LIST[IDX] == *_WIDGET_REF) {
      _FDX = IDX;

      break;
    }
  }

  if (_FDX == -1)
    return false;

  for (size_t IDX = _FDX; IDX < APP_HWND_LIST.ELEMENTS; ++IDX)
    APP_HWND_LIST.LIST[IDX] = APP_HWND_LIST.LIST[IDX + 1];

  --APP_HWND_LIST.ELEMENTS;

  if (APP_HWND_LIST.ELEMENTS == 0) {
    free(APP_HWND_LIST.LIST);
    APP_HWND_LIST.LIST = NULL;

    return true;
  }
  else {
    cic_widget** _REALLOC_TEMP = (cic_widget**)realloc(APP_HWND_LIST.LIST, APP_HWND_LIST.ELEMENTS * sizeof(struct _WIDGET_STRUCT*));

    if (_REALLOC_TEMP != NULL) {
      APP_HWND_LIST.LIST = _REALLOC_TEMP;

      return true;
    }
  }

  return false;
}
cic_widget* cic_getRefByRawHandle(HWND _HANDLE) {
  if (APP_HWND_LIST.ELEMENTS == 0)
    return NULL;

  cic_widget* WIDGET_REF = NULL;

  for (size_t IDX = 0; IDX < APP_HWND_LIST.ELEMENTS; ++IDX) {
    if (APP_HWND_LIST.LIST[IDX] != NULL) {
      if (APP_HWND_LIST.LIST[IDX]->_HANDLE == _HANDLE) {
        WIDGET_REF = APP_HWND_LIST.LIST[IDX];

        break;
      }
    }
  }

  return WIDGET_REF;
}
cic_widget* cic_getRefById(const wchar_t* _ID) {
  if (APP_HWND_LIST.ELEMENTS == 0)
    return NULL;

  cic_widget* WIDGET_REF = NULL;

  for (size_t IDX = 0; IDX < APP_HWND_LIST.ELEMENTS; ++IDX) {
    if (APP_HWND_LIST.LIST[IDX] != NULL) {
      if (wcscmp(APP_HWND_LIST.LIST[IDX]->_ID, _ID) == 0) {
        WIDGET_REF = APP_HWND_LIST.LIST[IDX];

        break;
      }
    }
  }

  return WIDGET_REF;
}

bool cic_createEventHandler(
  cic_widget** _SELF,
  cic_event _EVENT,
  CIC_CALLBACK _FUNC_REF
) {
  if (*_SELF != NULL) {
    cic_widget* _WIDGET = *_SELF;

    if (_WIDGET != NULL) {
      cic_eventStruct* _EST = (cic_eventStruct*)calloc(1, sizeof(struct _EVENT_STRUCT));

      if (_EST != NULL) {
        _EST->_EVENT_TYPE = _EVENT;
        _EST->_FUNC_REF = _FUNC_REF;

        if (_WIDGET->_EVENTS == NULL) {
          _WIDGET->_EVENTS = (cic_eventStruct**)calloc(1, sizeof(struct _EVENT_STRUCT*));

          if (_WIDGET->_EVENTS != NULL) {
            _WIDGET->_nEVENTS = 1;
            _WIDGET->_EVENTS[0] = _EST;
          }
          else
            return false;
        }
        else {
          if (cic_getEventHandler(_WIDGET, _EVENT) == NULL && _WIDGET->_EVENTS != NULL) {
            _WIDGET->_nEVENTS += 1;
            cic_eventStruct** _REALLOC_TEMP = (cic_eventStruct**)realloc(_WIDGET->_EVENTS, _WIDGET->_nEVENTS * sizeof(struct _EVENT_STRUCT*));

            if (_REALLOC_TEMP != NULL) {
              _WIDGET->_EVENTS = _REALLOC_TEMP;
              _WIDGET->_EVENTS[_WIDGET->_nEVENTS - 1] = _EST;
            }
            else
              return false;
          }
          else {
            size_t _FDX = -1;

            for (size_t IDX = 0; IDX < _WIDGET->_nEVENTS; ++IDX) {
              if (_WIDGET->_EVENTS[IDX] != NULL) {
                if (_WIDGET->_EVENTS[IDX]->_EVENT_TYPE == _EVENT) {
                  _FDX = IDX;

                  break;
                }
              }
            }

            _WIDGET->_EVENTS[_FDX] = _EST;
          }
        }

        return true;
      }
    }
  }

  return false;
}
CIC_CALLBACK cic_getEventHandler(
  cic_widget* _SELF,
  cic_event _EVENT
) {
  if (_SELF != NULL) {
    for (size_t IDX = 0; IDX < _SELF->_nEVENTS; ++IDX) {
      if (_SELF->_EVENTS[IDX] != NULL) {
        if (_SELF->_EVENTS[IDX]->_EVENT_TYPE == _EVENT)
          return _SELF->_EVENTS[IDX]->_FUNC_REF;
      }
    }
  }

  return NULL;
}

cic_region* cic_createRegion(HRGN _REGION) {
  cic_region* REGION = (cic_region*)calloc(1, sizeof(struct _CIC_REGION_STRUCT));

  if (REGION != NULL)
    REGION->_REGION = _REGION;

  return REGION;
}
cic_region* cic_createRectRegion(
  cic_point _POSITION,
  cic_size _SIZE
) {
  cic_region* REGION = (cic_region*)calloc(1, sizeof(struct _CIC_REGION_STRUCT));

  if (REGION != NULL) {
    REGION->_REGION = CreateRectRgn(
      _POSITION.X, _POSITION.Y,
      _SIZE.WIDTH, _SIZE.HEIGHT
    );
  }

  return REGION;
}
cic_region* cic_createRoundRectRegion(
  cic_point _POSITION,
  cic_size _SIZE,
  signed int _RADIUS
) {
  cic_region* REGION = (cic_region*)calloc(1, sizeof(struct _CIC_REGION_STRUCT));

  if (REGION != NULL) {
    REGION->_REGION = CreateRoundRectRgn(
      _POSITION.X, _POSITION.Y,
      _SIZE.WIDTH, _SIZE.HEIGHT,
      _RADIUS, _RADIUS
    );
  }

  return REGION;
}
cic_region* cic_createEllipticRegion(
  cic_point _POSITION,
  cic_size _RADIUSs
) {
  cic_region* REGION = (cic_region*)calloc(1, sizeof(struct _CIC_REGION_STRUCT));

  if (REGION != NULL) {
    REGION->_REGION = CreateEllipticRgn(
      _POSITION.X, _POSITION.Y,
      _RADIUSs.WIDTH, _RADIUSs.HEIGHT
    );
  }

  return REGION;
}
cic_region* cic_createPolygonRegion(
  cic_point POINTS[],
  size_t _nPOINTS,
  signed int _POLY_MODE
) {
  cic_region* REGION = (cic_region*)calloc(1, sizeof(struct _CIC_REGION_STRUCT));

  if (REGION != NULL) {
    POINT* _POINTS = (POINT*)calloc(_nPOINTS, sizeof(struct tagPOINT));

    if (_POINTS != NULL) {
      for (size_t IDX = 0; IDX < _nPOINTS; ++IDX) {
        cic_point _SPT = POINTS[IDX];
        POINT _PT;
        _PT.x = _SPT.X;
        _PT.y = _SPT.Y;

        _POINTS[IDX] = _PT;
      }

      REGION->_REGION = CreatePolygonRgn(_POINTS, (signed int)_nPOINTS, _POLY_MODE);

      free(_POINTS);
      _POINTS = NULL;
    }
  }

  return REGION;
}

void cic_setRegion(cic_region** _REGION, HRGN _HRGN) {
  if (*_REGION != NULL) {
    cic_region* REGION = *_REGION;

    if (REGION->_REGION != 0)
      DeleteObject(REGION->_REGION);

    REGION->_REGION = _HRGN;
  }
  else
    *_REGION = cic_createRegion(_HRGN);
}
HRGN cic_getHRGN(cic_region* _REGION) {
  if (_REGION != NULL)
    return _REGION->_REGION;

  return 0;
}
HRGN cic_getHRGNClone(cic_region* _REGION) {
  if (_REGION != NULL) {
    HRGN _CLONE = CreateRectRgn(0, 0, 0, 0);

    if (cic_combineRegion(_CLONE, _REGION->_REGION, CMODE_COPY))
      return _CLONE;
  }

  return 0;
}
bool cic_combineRegion(HRGN _DEST_REGION, HRGN _SRC_REGION, cic_rCombineMode _COMBINE_MODE) {
  if (_COMBINE_MODE != CMODE_COPY)
    return CombineRgn(_DEST_REGION, _DEST_REGION, _SRC_REGION, (signed int)_COMBINE_MODE) >= 2;
  else
    return CombineRgn(_DEST_REGION, _SRC_REGION, 0, (signed int)_COMBINE_MODE) >= 2;

  return false;
}

bool cic_destroyRegion(cic_region** _REGION) {
  if (*_REGION != NULL) {
    cic_region* REGION = *_REGION;

    if (REGION->_REGION != NULL)
      DeleteObject(REGION->_REGION);

    if (REGION != NULL)
      free(REGION);

    REGION = NULL;
  }

  return true;
}

cic_size cic_rectToSize(RECT _RECT) {
  return (cic_size) {
    .WIDTH = _RECT.right - _RECT.left,
    .HEIGHT = _RECT.bottom - _RECT.top
  };
}
cic_point cic_rectToPoint(RECT _RECT) {
  return (cic_point) {
    .X = _RECT.left,
      .Y = _RECT.top
  };
}
cic_point cic_pointToPoint(POINT _POINT) {
  return (cic_point) {
    .X = _POINT.x,
    .Y = _POINT.y
  };
}

bool cic_lpcwstrToLPWSTR(
  wchar_t** _DEST,
  const wchar_t* _SOURCE
) {
  if (*_DEST != NULL)
    free(*_DEST);

  size_t LEN_SOURCE = (wcslen(_SOURCE) + 1);
  *_DEST = (LPWSTR)calloc(LEN_SOURCE, sizeof(WCHAR));

  if (*_DEST != 0) {
    wcscpy(*_DEST, _SOURCE);

    if ((wcslen(*_DEST) + 1) == LEN_SOURCE)
      return true;
  }
 
  free(*_DEST);
  *_DEST = NULL;

  return false;
};

bool cic_setClipboard(const wchar_t* _SOURCE) {
  bool _SUCC = false;

  if (OpenClipboard(NULL)) {
    if (EmptyClipboard()) {
      size_t DATA_SZ = (wcslen(_SOURCE) + 1);

      HGLOBAL _CLIP_INFO = GlobalAlloc(GMEM_MOVEABLE, DATA_SZ * sizeof(WCHAR));
      if (_CLIP_INFO != NULL) {
        LPWSTR _CLIP_DATA = (LPWSTR)GlobalLock(_CLIP_INFO);
        if (_CLIP_DATA != NULL) {
          wcscpy(_CLIP_DATA, _SOURCE);

          if (SetClipboardData(CF_UNICODETEXT, _CLIP_DATA) != NULL)
            _SUCC = true;
        }

        GlobalUnlock(_CLIP_INFO);
      }
    }

    CloseClipboard();
  }

  return _SUCC;
}
bool cic_getClipboard(wchar_t** _DEST) {
  if (*_DEST != NULL)
    free(*_DEST);

  *_DEST = NULL;

  bool _SUCC = false;

  if (OpenClipboard(NULL)) {
    HANDLE _CLIP_DATA = GetClipboardData(CF_UNICODETEXT);

    if (_CLIP_DATA != NULL) {
      LPWSTR _CLIP_GET = (LPWSTR)GlobalLock(_CLIP_DATA);

      if (_CLIP_GET != NULL) {
        size_t DATA_SZ = (wcslen(_CLIP_GET) + 1);

        *_DEST = (LPWSTR)calloc(DATA_SZ, sizeof(WCHAR));

        wcscpy(*_DEST, _CLIP_GET);

        GlobalUnlock(_CLIP_GET);

        _SUCC = true;
      }
    }

    CloseClipboard();
  }

  return _SUCC;
}

bool cic_isKeyDown(
  signed int _VK_CODE,
  bool _TOGGLE_CHECK
) {
  if (!(_TOGGLE_CHECK)) {
    if (GetKeyState(_VK_CODE) < 0)
      return true;
  }
  else {
    if ((GetKeyState(_VK_CODE) & 1) == 1)
      return true;
  }

  return false;
}

LPCWSTR cic_widgetTypeToLPCWSTR(cic_widgetType _TYPE) {
  return (
    _TYPE == WTYPE_STATIC ? L"STATIC" :
    _TYPE == WTYPE_BUTTON ? L"BUTTON" :
    _TYPE == WTYPE_WINDOW ? L"WINDOW" :
    _TYPE == WTYPE_INPUT ? L"EDIT" :
    _TYPE == WTYPE_SCROLLBAR ? L"SCROLLBAR" :
    L"NULL_CLASS"
    );
}

unsigned int cic_getNumOfDisplayMonitors() {
  return GetSystemMetrics(SM_CMONITORS);
}

typedef struct _MONITOR_ENUM_STRUCT {
  signed int _IDX;
  HMONITOR _HMONITOR;
} _cic_monitorEnum;

static signed int CALLBACK _cic_MonitorByIdx(
  HMONITOR _HMONITOR,
  HDC _MONITOR_DC,
  LPRECT _MONITOR_RECT,
  LPARAM _LPARAM
) {
  _cic_monitorEnum* _MONITOR_ENUM = (_cic_monitorEnum*)_LPARAM;

  if (--_MONITOR_ENUM->_IDX < 0) {
    _MONITOR_ENUM->_HMONITOR = _HMONITOR;

    return false;
  }

  return true;
}
HMONITOR cic_getMonitorByIdx(unsigned int _MONITOR_IDX) {
  _cic_monitorEnum _MONITOR_ENUM;
  _MONITOR_ENUM._IDX = _MONITOR_IDX;
  _MONITOR_ENUM._HMONITOR = NULL;

  EnumDisplayMonitors(
    NULL,
    NULL,
    _cic_MonitorByIdx,
    (LPARAM)&_MONITOR_ENUM
  );

  return _MONITOR_ENUM._HMONITOR;
}

HMONITOR cic_getMonitorByWindow(cic_window* _WINDOW) {
  if (_WINDOW != NULL)
    return _WINDOW->_HMONITOR;

  return NULL;
}

static unsigned int _MONITOR_LOOP_IDX = 0;
static signed int CALLBACK _cic_MonitorByHandle(
  HMONITOR _HMONITOR,
  HDC _MONITOR_DC,
  LPRECT _MONITOR_RECT,
  LPARAM _LPARAM
) {
  _cic_monitorEnum* _MONITOR_ENUM = (_cic_monitorEnum*)_LPARAM;

  if (_HMONITOR == _MONITOR_ENUM->_HMONITOR) {
    _MONITOR_ENUM->_IDX = _MONITOR_LOOP_IDX;
    _MONITOR_LOOP_IDX = 0;

    return false;
  }

  ++_MONITOR_LOOP_IDX;

  return true;
  
}
signed int cic_getMonitorIdxByHandle(HMONITOR _HMONITOR) {
  _cic_monitorEnum _MONITOR_ENUM;
  _MONITOR_ENUM._IDX = -1;
  _MONITOR_ENUM._HMONITOR = _HMONITOR;

  EnumDisplayMonitors(
    NULL,
    NULL,
    _cic_MonitorByHandle,
    (LPARAM)&_MONITOR_ENUM
  );

  _MONITOR_LOOP_IDX = 0;

  return _MONITOR_ENUM._IDX;
}

cic_size cic_getScreenSizeByWindow(
  cic_window* _WINDOW,
  bool _WORKAREA
) {
  HMONITOR _HMONITOR = MonitorFromWindow(
    cic_getWindowHandle(_WINDOW),
    MONITOR_DEFAULTTONEAREST
  );

  MONITORINFO _MONITOR_INFO;
  _MONITOR_INFO.cbSize = sizeof(MONITORINFO);
  GetMonitorInfo(_HMONITOR, &_MONITOR_INFO);

  if (_WORKAREA)
    return cic_rectToSize(_MONITOR_INFO.rcWork);
  else
    return cic_rectToSize(_MONITOR_INFO.rcMonitor);
}
cic_point cic_getScreenPointByWindow(
  cic_window* _WINDOW,
  bool _WORKAREA
) {
  HMONITOR _HMONITOR = MonitorFromWindow(
    cic_getWindowHandle(_WINDOW),
    MONITOR_DEFAULTTONEAREST
  );

  MONITORINFO _MONITOR_INFO;
  _MONITOR_INFO.cbSize = sizeof(MONITORINFO);
  GetMonitorInfo(_HMONITOR, &_MONITOR_INFO);

  if (_WORKAREA)
    return cic_rectToPoint(_MONITOR_INFO.rcWork);
  else
    return cic_rectToPoint(_MONITOR_INFO.rcMonitor);
}
cic_point cic_getScreenCenterByWindow(
  cic_window* _WINDOW,
  bool _WORKAREA
) {
  HMONITOR _HMONITOR = MonitorFromWindow(
    cic_getWindowHandle(_WINDOW),
    MONITOR_DEFAULTTONEAREST
  );

  MONITORINFO _MONITOR_INFO;
  _MONITOR_INFO.cbSize = sizeof(MONITORINFO);
  GetMonitorInfo(_HMONITOR, &_MONITOR_INFO);

  cic_size _MONITOR_SZ;

  if (_WORKAREA)
    _MONITOR_SZ = cic_rectToSize(_MONITOR_INFO.rcWork);
  else
    _MONITOR_SZ = cic_rectToSize(_MONITOR_INFO.rcMonitor);

  cic_point _CENTER = {
    .X = (_WORKAREA ? _MONITOR_INFO.rcWork.left : _MONITOR_INFO.rcMonitor.left) + (_MONITOR_SZ.WIDTH / 2),
    .Y = (_WORKAREA ? _MONITOR_INFO.rcWork.top : _MONITOR_INFO.rcMonitor.top) + (_MONITOR_SZ.HEIGHT / 2)
  };

  return _CENTER;
}
cic_size cic_getScreenSizeByIdx(
  unsigned int _MONITOR_IDX,
  bool _WORKAREA
) {
  HMONITOR _HMONITOR = cic_getMonitorByIdx(_MONITOR_IDX);

  if (_HMONITOR != NULL) {
    MONITORINFO _MONITOR_INFO;
    _MONITOR_INFO.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(_HMONITOR, &_MONITOR_INFO);

    if (_WORKAREA)
      return cic_rectToSize(_MONITOR_INFO.rcWork);
    else
      return cic_rectToSize(_MONITOR_INFO.rcMonitor);
  }

  return (cic_size) { .WIDTH = -1, .HEIGHT = -1 };
}
cic_point cic_getScreenPointByIdx(
  unsigned int _MONITOR_IDX,
  bool _WORKAREA
) {
  HMONITOR _HMONITOR = cic_getMonitorByIdx(_MONITOR_IDX);

  if (_HMONITOR != NULL) {
    MONITORINFO _MONITOR_INFO;
    _MONITOR_INFO.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(_HMONITOR, &_MONITOR_INFO);

    if (_WORKAREA)
      return cic_rectToPoint(_MONITOR_INFO.rcWork);
    else
      return cic_rectToPoint(_MONITOR_INFO.rcMonitor);
  }

  return (cic_point) { -1, -1 };
}
cic_point cic_getScreenCenterByIdx(
  unsigned int _MONITOR_IDX,
  bool _WORKAREA
) {
  HMONITOR _HMONITOR = cic_getMonitorByIdx(_MONITOR_IDX);
  
  if (_HMONITOR != NULL) {
    MONITORINFO _MONITOR_INFO;
    _MONITOR_INFO.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(_HMONITOR, &_MONITOR_INFO);

    cic_size _MONITOR_SZ;

    if (_WORKAREA)
      _MONITOR_SZ = cic_rectToSize(_MONITOR_INFO.rcWork);
    else
      _MONITOR_SZ = cic_rectToSize(_MONITOR_INFO.rcMonitor);

    cic_point _CENTER = {
      .X = (_WORKAREA ? _MONITOR_INFO.rcWork.left : _MONITOR_INFO.rcMonitor.left) + (_MONITOR_SZ.WIDTH / 2),
      .Y = (_WORKAREA ? _MONITOR_INFO.rcWork.top : _MONITOR_INFO.rcMonitor.top) + (_MONITOR_SZ.HEIGHT / 2)
    };

    return _CENTER;
  }

  return (cic_point) { .X = -1, .Y = -1 };
}

cic_point cic_calcCenterPoint(cic_point _CENTER_POINT, cic_size _SIZE_OF_SRC) {
  cic_point _CENTER = {
    .X = _CENTER_POINT.X - (_SIZE_OF_SRC.WIDTH / 2),
    .Y = _CENTER_POINT.Y - (_SIZE_OF_SRC.HEIGHT / 2)
  };

  return _CENTER;
}


bool cic_exit(signed int _EXIT_CODE) {
  PostQuitMessage(0);
}