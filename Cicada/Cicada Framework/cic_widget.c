#include "include/cic_widget.h"

static LRESULT CALLBACK _WIDGET_PROC(
  HWND _HANDLE,
  UINT _MESSAGE,
  WPARAM _WPARAM,
  LPARAM _LPARAM
) {
  if (_MESSAGE == WM_NCCREATE) {
    if (cic_getRefByRawHandle(_HANDLE) == NULL) {
      LPCREATESTRUCT _LPCS = (LPCREATESTRUCT)_LPARAM;

      if (_LPCS != NULL) {
        cic_widget* _WIDGET = (cic_widget*)(_LPCS->lpCreateParams);

        if (_WIDGET != NULL) {
          cic_insertRawHandleRef(
            &_WIDGET
          );
        }
      }
    }
  }

  cic_widget* _this = cic_getRefByRawHandle(_HANDLE);

  if (_this != NULL) {
    switch (_MESSAGE) {
    case WM_ERASEBKGND: {
      return 0;
    };
    case WM_PAINT: {
      break;
    };

    case WM_DESTROY: {
      if (!(_this->_DESTROYED)) {
        _this->_DESTROYED = true;
        cic_destroyWidget(&_this);
      }

      return 0;
    };
    }

    return CallWindowProcW(
      _this->_PREV_PROC,
      _HANDLE,
      _MESSAGE,
      _WPARAM,
      _LPARAM
    );
  }

  return 0;
}

cic_widget* cic_createWidget(
  cic_widget* _PARENT,
  const wchar_t* _TITLE,
  const wchar_t* _ID,
  cic_point _POSITION,
  cic_size _SIZE,
  bool _APICALL
) {
  cic_widget* WIDGET = (cic_widget*)calloc(1, sizeof(struct _WIDGET_STRUCT));

  if (WIDGET != NULL) {
    WIDGET->_POSITION = _POSITION;
    WIDGET->_SIZE = _SIZE;

    WIDGET->_TEXT_ALIGNMENT = TEXTALIGN_LEFT;

    WIDGET->_FG_COLOR.RED = 0;
    WIDGET->_FG_COLOR.GREEN = 0;
    WIDGET->_FG_COLOR.BLUE = 0;

    WIDGET->_BG_COLOR.RED = 255;
    WIDGET->_BG_COLOR.GREEN = 255;
    WIDGET->_BG_COLOR.BLUE = 255;

    WIDGET->_SEL_COLOR.RED = 0;
    WIDGET->_SEL_COLOR.GREEN = 0;
    WIDGET->_SEL_COLOR.BLUE = 170;

    WIDGET->_WIDGET_TYPE = WTYPE_STATIC;

    cic_lpcwstrToLPWSTR(&WIDGET->_TITLE, _TITLE);

    /* __CREATE_WIDGET__ */

    if (_APICALL) {
      WNDCLASSW WIDG_CLASS;
      ZeroMemory(&WIDG_CLASS, sizeof(WNDCLASSW));
      WIDG_CLASS.lpszClassName = _ID;
      WIDG_CLASS.hInstance = GetModuleHandleW(0);
      WIDG_CLASS.hCursor = LoadCursor(0, IDC_ARROW);
      WIDG_CLASS.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
      WIDG_CLASS.style = CS_HREDRAW | CS_VREDRAW;
      WIDG_CLASS.lpfnWndProc = _WIDGET_PROC;

      WIDGET->_HANDLE = CreateWindowW(
        _ID,
        _TITLE,
        WS_CHILD | WS_VISIBLE,
        _POSITION.X, _POSITION.Y,
        _SIZE.WIDTH, _SIZE.HEIGHT,
        (HWND)cic_getWidgetHandle(_PARENT), (HMENU)0,
        (HINSTANCE)GetModuleHandleW(0),
        NULL
      );

      if (WIDGET->_HANDLE == (HWND)0) {
        cic_destroyWidget(&WIDGET);

        return NULL;
      }

      WIDGET->_PREV_PROC = (WNDPROC)SetWindowLongPtr(
        cic_getWidgetHandle(WIDGET),
        GWLP_WNDPROC,
        (LONG_PTR)_WIDGET_PROC
      );
      cic_insertRawHandleRef(&WIDGET);
      ShowWindow(
        WIDGET->_HANDLE,
        SW_SHOW
      );
      UpdateWindow(WIDGET->_HANDLE);
    }

    return WIDGET;
  }

  return NULL;
}

HWND cic_getWidgetHandle(cic_widget* _SELF) {
  if (_SELF != NULL)
    return _SELF->_HANDLE;

  return (HWND)0;
}

bool cic_toCenterXWidget(
  cic_widget** _SELF,
  cic_widget* _SRC
) {
  if (*_SELF != NULL && _SRC != NULL) {
    cic_widget* _WIDGET = *_SELF;
    _WIDGET->_POSITION.X = cic_getWidgetCenterX(_SRC);

    return SetWindowPos(
      cic_getWidgetHandle(_WIDGET),
      (HWND)0,
      cic_getWidgetX(_WIDGET), cic_getWidgetY(_WIDGET),
      0, 0,
      SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW
    );
  }

  return false;
}
bool cic_toCenterYWidget(
  cic_widget** _SELF,
  cic_widget* _SRC
) {
  if (*_SELF != NULL && _SRC != NULL) {
    return cic_setWidgetPosition(_SELF,
      (cic_point) {
      cic_getWidgetX(*_SELF),
        cic_getWidgetCenterY(_SRC)
    }
    );
  }

  return false;
}
bool cic_toBelowOfWidget(
  cic_widget** _SELF,
  cic_widget* _SRC,
  cic_point _OFFSET
) {
  if (*_SELF != NULL && _SRC != NULL) {
    return cic_setWidgetPosition(_SELF,
      (cic_point) {
      cic_getWidgetX(_SRC) + _OFFSET.X,
        (cic_getWidgetY(_SRC) + cic_getWidgetH(_SRC)) + _OFFSET.Y
    }
    );
  }

  return false;
}
bool cic_toAboveOfWidget(
  cic_widget** _SELF,
  cic_widget* _SRC,
  cic_point _OFFSET
) {
  if (*_SELF != NULL && _SRC != NULL) {
    return cic_setWidgetPosition(_SELF,
      (cic_point) {
      cic_getWidgetX(_SRC) + _OFFSET.X,
        (cic_getWidgetY(_SRC) - cic_getWidgetH(_SRC)) + _OFFSET.Y
    }
    );
  }

  return false;
}
bool cic_toRightOfWidget(
  cic_widget** _SELF,
  cic_widget* _SRC,
  cic_point _OFFSET
) {
  if (*_SELF != NULL && _SRC != NULL) {
    return cic_setWidgetPosition(_SELF,
      (cic_point) {
      (cic_getWidgetX(_SRC) + cic_getWidgetW(_SRC)) + _OFFSET.X,
        cic_getWidgetY(_SRC) + _OFFSET.Y
    }
    );
  }

  return false;
}
bool cic_toLeftOfWidget(
  cic_widget** _SELF,
  cic_widget* _SRC,
  cic_point _OFFSET
) {
  if (*_SELF != NULL && _SRC != NULL) {
    return cic_setWidgetPosition(_SELF,
      (cic_point) {
      (cic_getWidgetX(_SRC) - cic_getWidgetW(_SRC)) + _OFFSET.X,
        cic_getWidgetY(_SRC) + _OFFSET.Y
    }
    );
  }

  return false;
}

bool cic_toCenterOfParentWidget(
  cic_widget** _SELF,
  cic_point _OFFSET
) {
  if (*_SELF != NULL) {
    cic_widget* _WIDGET = *_SELF;
    cic_widget* _PARENT = cic_getWidgetParent(_WIDGET);

    if (_PARENT != NULL) {
      return cic_setWidgetPosition(_SELF,
        (cic_point) {
        cic_getWidgetCenterX(_PARENT) + _OFFSET.X,
          cic_getWidgetCenterY(_PARENT) + _OFFSET.Y
      }
      );
    }
  }

  return false;
}
bool cic_toSizeOfParentWidget(
  cic_widget** _SELF,
  cic_size _OFFSET
) {
  if (*_SELF != NULL) {
    cic_widget* _WIDGET = *_SELF;
    cic_widget* _PARENT = cic_getWidgetParent(_WIDGET);

    if (_PARENT != NULL) {
      return cic_setWidgetSize(_SELF,
        (cic_size) {
        cic_getWidgetW(_PARENT) + _OFFSET.WIDTH,
          cic_getWidgetH(_PARENT) + _OFFSET.HEIGHT
      }
      );
    }
  }

  return false;
}

bool cic_toCenterOfWidget(
  cic_widget** _SELF,
  cic_widget* _SRC,
  cic_point _OFFSET
) {
  if (*_SELF != NULL && _SRC != NULL) {
    return cic_setWidgetPosition(_SELF,
      (cic_point) {
      cic_getWidgetCenterX(_SRC) + _OFFSET.X,
        cic_getWidgetCenterY(_SRC) + _OFFSET.Y
    }
    );
  }

  return false;
}
bool cic_toSizeOfWidget(
  cic_widget** _SELF,
  cic_widget* _SRC,
  cic_size _OFFSET
) {
  if (*_SELF != NULL && _SRC != NULL) {
    return cic_setWidgetSize(_SELF,
      (cic_size) {
      cic_getWidgetW(_SRC) + _OFFSET.WIDTH,
        cic_getWidgetH(_SRC) + _OFFSET.HEIGHT
    }
    );
  }

  return false;
}

bool cic_setWidgetPosition(
  cic_widget** _SELF,
  cic_point _POSITION
) {
  if (*_SELF != NULL) {
    cic_widget* _WIDGET = *_SELF;
    _WIDGET->_POSITION = _POSITION;

    return SetWindowPos(
      cic_getWidgetHandle(_WIDGET),
      (HWND)0,
      cic_getWidgetX(_WIDGET), cic_getWidgetY(_WIDGET),
      0, 0,
      SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW
    );
  }

  return false;
}
bool cic_setWidgetSize(
  cic_widget** _SELF,
  cic_size _SIZE
) {
  if (*_SELF != NULL) {
    cic_widget* _WIDGET = *_SELF;
    _WIDGET->_SIZE = _SIZE;

    return SetWindowPos(
      cic_getWidgetHandle(_WIDGET),
      (HWND)0,
      0, 0,
      cic_getWidgetW(_WIDGET), cic_getWidgetH(_WIDGET),
      SWP_NOMOVE | SWP_NOZORDER
    );
  }

  return false;
}
bool cic_setWidgetLabel(
  cic_widget** _SELF,
  const wchar_t* _TITLE
) {
  if (*_SELF != NULL) {
    cic_widget* _WIDGET = *_SELF;
    cic_lpcwstrToLPWSTR(&_WIDGET->_TITLE, _TITLE);

    return cic_redrawWidget(_SELF);
  }

  return false;
}
bool cic_setWidgetTooltip(
  cic_widget** _SELF,
  const wchar_t* _TOOLTIP
) {
  if (*_SELF != NULL) {
    cic_widget* _WIDGET = *_SELF;
    cic_lpcwstrToLPWSTR(&_WIDGET->_TOOLTIP, _TOOLTIP);

    return cic_redrawWidget(_SELF);
  }

  return false;
}
bool cic_setWidgetLabelAlignment(
  cic_widget** _SELF,
  cic_textAlignment _ALIGNMENT
) {
  if (*_SELF != NULL) {
    cic_widget* _WIDGET = *_SELF;
    _WIDGET->_TEXT_ALIGNMENT = _ALIGNMENT;

    return cic_redrawWidget(_SELF);
  }

  return false;
}
bool cic_setWidgetForegroundColor(
  cic_widget** _SELF,
  cic_color _COLOR
) {
  if (*_SELF != NULL) {
    cic_widget* _WIDGET = *_SELF;
    _WIDGET->_FG_COLOR = _COLOR;

    return cic_redrawWidget(_SELF);
  }

  return false;
}
bool cic_setWidgetBackgroundColor(
  cic_widget** _SELF,
  cic_color _COLOR
) {
  if (*_SELF != NULL) {
    cic_widget* _WIDGET = *_SELF;
    _WIDGET->_BG_COLOR = _COLOR;

    return cic_redrawWidget(_SELF);
  }

  return false;
}
bool cic_setWidgetSelectionColor(
  cic_widget** _SELF,
  cic_color _COLOR
) {
  if (*_SELF != NULL) {
    cic_widget* _WIDGET = *_SELF;
    _WIDGET->_SEL_COLOR = _COLOR;

    return cic_redrawWidget(_SELF);
  }

  return false;
}
bool cic_setWidgetLabelFont(
  cic_widget** _SELF,
  cic_font _FONT
) {
  if (*_SELF != NULL) {
    cic_widget* _WIDGET = *_SELF;
    _WIDGET->_FONT = _FONT;

    if (_WIDGET->_HFONT != 0)
      DeleteObject(_WIDGET->_HFONT);

    _WIDGET->_HFONT = CreateFontW(
      _WIDGET->_FONT.FONT_SIZE,
      _WIDGET->_FONT.FONT_SIZE,
      _WIDGET->_FONT.ANGLE,
      _WIDGET->_FONT.ANGLE,
      (DWORD)_WIDGET->_FONT.WEIGHT,
      _WIDGET->_FONT.IS_ITALIC,
      _WIDGET->_FONT.IS_UNDERLINED,
      _WIDGET->_FONT.HAS_STRIKEOUT,
      (DWORD)_WIDGET->_FONT.CHARSET,
      OUT_DEFAULT_PRECIS,
      CLIP_DEFAULT_PRECIS,
      (DWORD)_WIDGET->_FONT.QUALITY,
      0,
      _WIDGET->_FONT.FONT_NAME
    );

    if (_WIDGET->_HFONT != 0)
      return true;
  }

  return false;
}
bool cic_setWidgetFontSize(
  cic_widget** _SELF,
  int SIZE
) {
  if (*_SELF != NULL) {
    cic_widget* _WIDGET = *_SELF;
    _WIDGET->_FONT.FONT_SIZE = SIZE;

    return cic_setWidgetLabelFont(_SELF, _WIDGET->_FONT);
  }

  return false;
}
bool cic_setWidgetShape(
  cic_widget** _SELF,
  cic_region* _REGION
) {
  if (*_SELF != NULL) {
    cic_widget* _WIDGET = *_SELF;

    if (_REGION != NULL) {
      _WIDGET->_STYLE_BEF_SHAPE = (DWORD)GetWindowLongPtr(cic_getWidgetHandle(_WIDGET), GWL_STYLE);
      _WIDGET->_HAS_SHAPE = 1;

      cic_setRegion(&_WIDGET->_SHAPE, cic_getHRGNClone(_REGION));
      SetWindowLongPtr(cic_getWidgetHandle(_WIDGET), GWL_STYLE, _WIDGET->_STYLE_BEF_SHAPE & ~(WS_OVERLAPPEDWINDOW));
      SetWindowRgn(cic_getWidgetHandle(_WIDGET), cic_getHRGNClone(_WIDGET->_SHAPE), 1);

      return true;
    }

    if (_WIDGET->_HAS_SHAPE) {
      if (_WIDGET->_STYLE_BEF_SHAPE != 0)
        SetWindowLongPtr(cic_getWidgetHandle(_WIDGET), GWL_STYLE, _WIDGET->_STYLE_BEF_SHAPE);
    }

    _WIDGET->_STYLE_BEF_SHAPE = 0;
    _WIDGET->_HAS_SHAPE = 0;
    cic_setRegion(&_WIDGET->_SHAPE, (HRGN)0);
    SetWindowRgn(cic_getWidgetHandle(_WIDGET), (HRGN)0, 1);

    return true;
  }

  return false;
}

bool cic_redrawWidget(cic_widget** _SELF) {
  if (*_SELF != NULL) {
    cic_widget* _WIDGET = *_SELF;

    RECT CLT_RECT;
    GetClientRect(
      cic_getWidgetHandle(_WIDGET),
      &CLT_RECT
    );

    return InvalidateRect(
      cic_getWidgetHandle(_WIDGET),
      &CLT_RECT, true
    );
  }

  return false;
}

cic_point cic_getWidgetPosition(cic_widget* _SELF) {
  if (_SELF != NULL)
    return _SELF->_POSITION;

  return (cic_point) { -1, -1 };
}
int cic_getWidgetX(cic_widget* _SELF) {
  if (_SELF != NULL)
    return _SELF->_POSITION.X;

  return -1;
}
int cic_getWidgetY(cic_widget* _SELF) {
  if (_SELF != NULL)
    return _SELF->_POSITION.Y;

  return -1;
}

cic_size cic_getWidgetSize(cic_widget* _SELF) {
  if (_SELF != NULL)
    return _SELF->_SIZE;

  return (cic_size) { -1, -1 };
}
int cic_getWidgetWidth(cic_widget* _SELF) {
  if (_SELF != NULL)
    return _SELF->_SIZE.WIDTH;

  return -1;
}
int cic_getWidgetHeight(cic_widget* _SELF) {
  if (_SELF != NULL)
    return _SELF->_SIZE.HEIGHT;

  return -1;
}

int cic_getWidgetW(cic_widget* _SELF) { return cic_getWidgetWidth(_SELF); };
int cic_getWidgetH(cic_widget* _SELF) { return cic_getWidgetHeight(_SELF); };

int cic_getWidgetMouseWheelDelta(cic_widget* _SELF) {
  if (_SELF != NULL)
    return _SELF->_WHEEL_DELTA;

  return -1;
}

const wchar_t* cic_getWidgetLabel(cic_widget* _SELF) {
  if (_SELF != NULL)
    return (LPCWSTR)_SELF->_TITLE;

  return NULL;
}
const wchar_t* cic_getWidgetTooltip(cic_widget* _SELF) {
  if (_SELF != NULL)
    return (LPCWSTR)_SELF->_TOOLTIP;

  return NULL;
}
cic_textAlignment cic_getWidgetLabelAlignment(cic_widget* _SELF) {
  if (_SELF != NULL)
    return _SELF->_TEXT_ALIGNMENT;

  return TEXTALIGN_LEFT;
}
cic_color cic_getWidgetForegroundColor(cic_widget* _SELF) {
  if (_SELF != NULL)
    return _SELF->_FG_COLOR;

  return (cic_color) { -1, -1, -1 };
}
cic_color cic_getWidgetBackgroundColor(cic_widget* _SELF) {
  if (_SELF != NULL)
    return _SELF->_BG_COLOR;

  return (cic_color) { -1, -1, -1 };
}
cic_color cic_getWidgetSelectionColor(cic_widget* _SELF) {
  if (_SELF != NULL)
    return _SELF->_SEL_COLOR;

  return (cic_color) { -1, -1, -1 };
}
cic_font cic_getWidgetLabelFont(cic_widget* _SELF) {
  if (_SELF != NULL)
    return _SELF->_FONT;

  return (cic_font) { 0 };
}
int cic_getWidgetFontSize(cic_widget* _SELF) {
  if (_SELF != NULL)
    return _SELF->_FONT.FONT_SIZE;

  return -1;
}

cic_point cic_getWidgetCenter(cic_widget* _SELF) {
  if (_SELF != NULL) {
    cic_point _CENTER;
    _CENTER.X = cic_getWidgetX(_SELF) + (cic_getWidgetW(_SELF) / 2);
    _CENTER.Y = cic_getWidgetY(_SELF) + (cic_getWidgetH(_SELF) / 2);

    return _CENTER;
  }

  return (cic_point) { -1, -1 };
}

int cic_getWidgetCenterX(cic_widget* _SELF) {
  if (_SELF != NULL)
    return cic_getWidgetX(_SELF) + (cic_getWidgetW(_SELF) / 2);

  return -1;
}
int cic_getWidgetCenterY(cic_widget* _SELF) {
  if (_SELF != NULL)
    return cic_getWidgetY(_SELF) + (cic_getWidgetH(_SELF) / 2);

  return -1;
}

bool cic_isWidgetFocused(cic_widget* _SELF) {
  if (_SELF != NULL)
    return GetFocus() == cic_getWidgetHandle(_SELF);

  return false;
}
bool cic_isWidgetVisible(cic_widget* _SELF) {
  if (_SELF != NULL)
    return IsWindowVisible(cic_getWidgetHandle(_SELF));

  return false;
}
bool cic_isWidgetEnabled(cic_widget* _SELF) {
  if (_SELF != NULL)
    return IsWindowEnabled(cic_getWidgetHandle(_SELF));

  return false;
}
bool cic_isWidgetDraggable(cic_widget* _SELF) {
  if (_SELF != NULL)
    return _SELF->_DRAGGABLE;

  return false;
}

cic_widgetType cic_getWidgetType(cic_widget* _SELF) {
  if (_SELF != NULL)
    return _SELF->_WIDGET_TYPE;

  return WTYPE_STATIC;
}

cic_widget* cic_getWidgetParent(cic_widget* _SELF) {
  if (_SELF != NULL) {
    return cic_getRefByRawHandle(
      GetAncestor(
        cic_getWidgetHandle(_SELF),
        GA_PARENT
      )
    );
  }

  return NULL;
}
cic_widget* cic_getWidgetWindow(cic_widget* _SELF) {
  if (_SELF != NULL) {
    return cic_getRefByRawHandle(
      GetAncestor(
        cic_getWidgetHandle(_SELF),
        GA_ROOT
      )
    );
  }

  return NULL;
}
cic_widget* cic_getWidgetTopWindow(cic_widget* _SELF) {
  if (_SELF != NULL) {
    return cic_getRefByRawHandle(
      GetAncestor(
        cic_getWidgetHandle(_SELF),
        GA_ROOTOWNER
      )
    );
  }

  return NULL;
}

bool cic_destroyWidget(cic_widget** _SELF) {
  if (*_SELF != NULL) {
    if (!((*_SELF)->_DESTROYED)) {
      (*_SELF)->_DESTROYED = true;
      DestroyWindow(cic_getWidgetHandle(*_SELF));
      cic_widget* _TOP_WINDOW = cic_getWidgetTopWindow(*_SELF);

      if (_TOP_WINDOW != NULL)
        cic_redrawWidget(&_TOP_WINDOW);
    }

    cic_removeRawHandleRef(_SELF);

    if ((*_SELF)->_TITLE != NULL) {
      free((*_SELF)->_TITLE);
      (*_SELF)->_TITLE = NULL;
    }

    if ((*_SELF)->_TOOLTIP != NULL) {
      free((*_SELF)->_TOOLTIP);
      (*_SELF)->_TOOLTIP = NULL;
    }

    if ((*_SELF)->_SHAPE != NULL) {
      cic_destroyRegion(&(*_SELF)->_SHAPE);
      (*_SELF)->_SHAPE = NULL;
    }

    if ((*_SELF)->_HFONT != 0) {
      DeleteObject((*_SELF)->_HFONT);
      (*_SELF)->_HFONT = 0;
    }

    if ((*_SELF)->_EVENTS != NULL) {
      if ((*_SELF)->_nEVENTS > 0) {
        for (size_t IDX = 0; IDX < (*_SELF)->_nEVENTS; ++IDX) {
          if ((*_SELF)->_EVENTS[IDX] != NULL) {
            free((*_SELF)->_EVENTS[IDX]);
            (*_SELF)->_EVENTS[IDX] = NULL;
          }
        }
      }

      if ((*_SELF)->_EVENTS != NULL) {
        free((*_SELF)->_EVENTS);
        (*_SELF)->_EVENTS = NULL;
      }
    }

    if (*_SELF != NULL) {
      free(*_SELF);
      *_SELF = NULL;
    }

    return true;
  }

  return false;
}
