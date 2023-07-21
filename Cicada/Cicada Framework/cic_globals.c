#include "include/cic_globals.h"

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
    if (APP_HWND_LIST.LIST[IDX] == *_WIDGET_REF)
      _FDX = IDX;
  }

  if (_FDX != -1)
    return false;

  APP_HWND_LIST.ELEMENTS += 1;
  APP_HWND_LIST.LIST = (cic_widget**)realloc(APP_HWND_LIST.LIST, APP_HWND_LIST.ELEMENTS * sizeof(struct _WIDGET_STRUCT*));

  if (APP_HWND_LIST.LIST != NULL)
    APP_HWND_LIST.LIST[APP_HWND_LIST.ELEMENTS - 1] = *_WIDGET_REF;
  else
    return false;

  return true;
}
bool cic_removeRawHandleRef(cic_widget** _WIDGET_REF) {
  if (APP_HWND_LIST.ELEMENTS == 0)
    return false;

  size_t _FDX = -1;

  for (size_t IDX = 0; IDX < APP_HWND_LIST.ELEMENTS; ++IDX) {
    cic_widget* WIDGET_REF = APP_HWND_LIST.LIST[IDX];

    if (WIDGET_REF == *_WIDGET_REF) {
      _FDX = IDX;

      break;
    }
  }

  if (_FDX == -1)
    return false;

  for (size_t IDX = _FDX; IDX < APP_HWND_LIST.ELEMENTS; ++IDX)
    APP_HWND_LIST.LIST[IDX] = APP_HWND_LIST.LIST[IDX + 1];

  APP_HWND_LIST.ELEMENTS -= 1;

  if (APP_HWND_LIST.ELEMENTS == 0) {
    free(APP_HWND_LIST.LIST);
    APP_HWND_LIST.LIST = NULL;
  }
  else {
    APP_HWND_LIST.LIST = (cic_widget**)realloc(APP_HWND_LIST.LIST, APP_HWND_LIST.ELEMENTS * sizeof(struct _WIDGET_STRUCT*));

    if (APP_HWND_LIST.LIST == NULL)
      return false;
  }

  return true;
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
            _WIDGET->_EVENTS = (cic_eventStruct**)realloc(_WIDGET->_EVENTS, _WIDGET->_nEVENTS * sizeof(struct _EVENT_STRUCT*));

            if (_WIDGET->_EVENTS != NULL)
              _WIDGET->_EVENTS[_WIDGET->_nEVENTS - 1] = _EST;
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
  cic_region* REGION = (cic_region*)calloc(1, sizeof(struct _cic_REGION_STRUCT));

  if (REGION != NULL)
    REGION->_REGION = _REGION;

  return REGION;
}
cic_region* cic_createRectRegion(
  cic_point _POSITION,
  cic_size _SIZE
) {
  cic_region* REGION = (cic_region*)calloc(1, sizeof(struct _cic_REGION_STRUCT));

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
  int _RADIUS
) {
  cic_region* REGION = (cic_region*)calloc(1, sizeof(struct _cic_REGION_STRUCT));

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
  cic_region* REGION = (cic_region*)calloc(1, sizeof(struct _cic_REGION_STRUCT));

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
  int _POLY_MODE
) {
  cic_region* REGION = (cic_region*)calloc(1, sizeof(struct _cic_REGION_STRUCT));

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

      REGION->_REGION = CreatePolygonRgn(_POINTS, (int)_nPOINTS, _POLY_MODE);

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
    return CombineRgn(_DEST_REGION, _DEST_REGION, _SRC_REGION, (int)_COMBINE_MODE) >= 2;
  else
    return CombineRgn(_DEST_REGION, _SRC_REGION, 0, (int)_COMBINE_MODE) >= 2;

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
  cic_size SIZE;
  SIZE.WIDTH = _RECT.right - _RECT.left;
  SIZE.HEIGHT = _RECT.bottom - _RECT.top;

  return SIZE;
}
cic_point cic_pointToPoint(POINT _POINT) {
  cic_point POINT;
  POINT.X = _POINT.x;
  POINT.Y = _POINT.y;

  return POINT;
}

bool cic_lpcwstrToLPWSTR(
  LPWSTR* _DEST,
  LPCWSTR _SOURCE
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
  int _VK_CODE,
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
