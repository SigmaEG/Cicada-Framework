#include "include/cic_button.h"
#include "include/cic_widget.h"

static void _BTN_DRAW(
  cic_button** _SELF,
  HDC _DC,
  PAINTSTRUCT* _PS
) {
  if (*_SELF != NULL) {
    cic_button* _BUTTON = *_SELF;
    cic_widget* _BASE = _BUTTON->_BASE;
    HBRUSH BRUSH = CreateSolidBrush(RGB(_BASE->_BG_COLOR.RED, _BASE->_BG_COLOR.GREEN, _BASE->_BG_COLOR.BLUE));
    SetTextColor(_DC, RGB(_BASE->_FG_COLOR.RED, _BASE->_FG_COLOR.GREEN, _BASE->_FG_COLOR.BLUE));
    SetBkMode(_DC, TRANSPARENT);

    if (!(_BASE->_HAS_SHAPE))
      FillRect(_DC, &_PS->rcPaint, BRUSH);
    else {
      HRGN REGION = CreateRectRgn(0, 0, 0, 0);
      GetWindowRgn(_BASE->_HANDLE, REGION);

      if (REGION != 0) {
        FillRgn(_DC, REGION, BRUSH);

        DeleteObject(REGION);
      }
    }

    RECT _TEXT_RECT;
    GetClientRect(_BASE->_HANDLE, &_TEXT_RECT);

    DrawText(
      _DC,
      _BASE->_TITLE,
      (signed int)wcslen(_BASE->_TITLE),
      &_TEXT_RECT,
      DT_CENTER | DT_VCENTER | DT_SINGLELINE
    );

    DeleteObject(BRUSH);
  }
}

static LRESULT CALLBACK _BTN_PROC(
  HWND _HANDLE,
  UINT _MESSAGE,
  WPARAM _WPARAM,
  LPARAM _LPARAM
) {
  if (_MESSAGE == WM_NCCREATE) {

    if (cic_getRefByRawHandle(_HANDLE) == NULL) {
      LPCREATESTRUCT _LPCS = (LPCREATESTRUCT)_LPARAM;

      if (_LPCS != NULL) {
        cic_button* _BUTTON = (cic_button*)(_LPCS->lpCreateParams);

        if (_BUTTON != NULL) {
          cic_insertRawHandleRef(
            cic_getButtonWidgetRef(_BUTTON)
          );
        }
      }
    }
  }

  cic_widget* _BASE = cic_getRefByRawHandle(_HANDLE);

  if (_BASE != NULL) {
    cic_button* _this = (cic_button*)_BASE->_WIDGET_UPCAST_REF;

    if (_this != NULL) {
      switch (_MESSAGE) {
        case WM_ERASEBKGND: {
          if (_this->_OWNER_DRAW)
            return 0;

          break;
        };
        case WM_PAINT: {
          if (_this->_OWNER_DRAW) {
            RECT CLT_RECT;
            GetClientRect(_HANDLE, &CLT_RECT);

            PAINTSTRUCT _PS;
            HDC _DC = BeginPaint(_HANDLE, &_PS);

            HDC _MEM_DC = CreateCompatibleDC(_DC);
            HBITMAP _MEM_BMP = CreateCompatibleBitmap(_DC, _PS.rcPaint.right - _PS.rcPaint.left, _PS.rcPaint.bottom - _PS.rcPaint.top);
            HBITMAP _MEM_O_BMP = (HBITMAP)SelectObject(_MEM_DC, _MEM_BMP);
            POINT _O_ORGN;

            OffsetViewportOrgEx(_MEM_DC, -(CLT_RECT.left), -(CLT_RECT.top), &_O_ORGN);

            _BTN_DRAW(&_this, _MEM_DC, &_PS);

            SetViewportOrgEx(_MEM_DC, _O_ORGN.x, _O_ORGN.y, NULL);

            TransparentBlt(
              _DC,
              0, 0,
              CLT_RECT.right, CLT_RECT.bottom,
              _MEM_DC,
              0, 0,
              CLT_RECT.right, CLT_RECT.bottom,
              RGB(0, 0, 0)
            );

            EndPaint(_HANDLE, &_PS);

            SelectObject(_MEM_DC, _MEM_O_BMP);
            DeleteObject(_MEM_BMP);
            DeleteDC(_MEM_DC);

            return 0;
          }
          else
            break;
        };

        case WM_CTLCOLORBTN: {
          HBRUSH _BRUSH = CreateSolidBrush(RGB(_BASE->_BG_COLOR.RED, _BASE->_BG_COLOR.GREEN, _BASE->_BG_COLOR.BLUE));
          return (LONG_PTR)_BRUSH;
        };

        case WM_DESTROY: {
          if (!(_BASE->_DESTROYED)) {
            _BASE->_DESTROYED = true;
            cic_destroyButton(&_this);
          }

          return 0;
        };

        case WM_KEYDOWN: {
          CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_KEYDOWN);

          if (_EVENT_HANDLE != NULL)
            _EVENT_HANDLE(&_BASE);

          break;
        };
        case WM_KEYUP: {
          CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_KEYUP);

          if (_EVENT_HANDLE != NULL)
            _EVENT_HANDLE(&_BASE);

          break;
        };

        case (WM_LBUTTONDOWN): {
          CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_MOUSE_LMB_DOWN);

          if (_EVENT_HANDLE != NULL)
            _EVENT_HANDLE(&_BASE);

          break;
        };
        case (WM_LBUTTONUP): {
          CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_MOUSE_LMB_UP);

          if (_EVENT_HANDLE != NULL)
            _EVENT_HANDLE(&_BASE);

          break;
        }
        case (WM_LBUTTONDBLCLK): {
          CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_MOUSE_LMB_DBLCLK);

          if (_EVENT_HANDLE != NULL)
            _EVENT_HANDLE(&_BASE);

          break;
        };

        case (WM_RBUTTONDOWN): {
          CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_MOUSE_RMB_DOWN);

          if (_EVENT_HANDLE != NULL)
            _EVENT_HANDLE(&_BASE);

          break;
        };
        case (WM_RBUTTONUP): {
          CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_MOUSE_RMB_UP);

          if (_EVENT_HANDLE != NULL)
            _EVENT_HANDLE(&_BASE);

          break;
        };
        case (WM_RBUTTONDBLCLK): {
          CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_MOUSE_RMB_DBLCLK);

          if (_EVENT_HANDLE != NULL)
            _EVENT_HANDLE(&_BASE);

          break;
        }

        case (WM_MBUTTONDOWN): {
          CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_MOUSE_MMB_DOWN);

          if (_EVENT_HANDLE != NULL)
            _EVENT_HANDLE(&_BASE);

          break;
        };
        case (WM_MBUTTONUP): {
          CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_MOUSE_MMB_UP);

          if (_EVENT_HANDLE != NULL)
            _EVENT_HANDLE(&_BASE);

          break;
        };
        case (WM_MBUTTONDBLCLK): {
          CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_MOUSE_MMB_DBLCLK);

          if (_EVENT_HANDLE != NULL)
            _EVENT_HANDLE(&_BASE);

          break;
        }

        case (WM_MOVE): {
          _BASE->_POSITION.X = LOWORD(_LPARAM);
          _BASE->_POSITION.Y = HIWORD(_LPARAM);

          CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_MOVE);

          if (_EVENT_HANDLE != NULL)
            _EVENT_HANDLE(&_BASE);

          break;
        };
        case (WM_SIZE): {
          _BASE->_SIZE.WIDTH = LOWORD(_LPARAM);
          _BASE->_SIZE.HEIGHT = HIWORD(_LPARAM);

          CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_RESIZE);

          if (_EVENT_HANDLE != NULL)
            _EVENT_HANDLE(&_BASE);

          break;
        };

        case (WM_SETFOCUS): {
          CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_FOCUS);

          if (_EVENT_HANDLE != NULL)
            _EVENT_HANDLE(&_BASE);

          break;
        };
        case (WM_KILLFOCUS): {
          CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_UNFOCUS);

          if (_EVENT_HANDLE != NULL)
            _EVENT_HANDLE(&_BASE);

          break;
        };

        case (WM_ENABLE): {
          if (_WPARAM == TRUE) {
            CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_ENABLE);

            if (_EVENT_HANDLE != NULL)
              _EVENT_HANDLE(&_BASE);
          }
          else {
            CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_DISABLE);

            if (_EVENT_HANDLE != NULL)
              _EVENT_HANDLE(&_BASE);
          }

          break;
        };
        case (WM_SHOWWINDOW): {
          if (_WPARAM == TRUE) {
            CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_SHOW);

            if (_EVENT_HANDLE != NULL)
              _EVENT_HANDLE(&_BASE);
          }
          else {
            CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_HIDE);

            if (_EVENT_HANDLE != NULL)
              _EVENT_HANDLE(&_BASE);
          }

          break;
        };

        case (WM_PASTE): {
          CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_PASTE);

          if (_EVENT_HANDLE != NULL)
            _EVENT_HANDLE(&_BASE);

          break;
        };

        case (WM_MOUSEMOVE): {
          if (!(_BASE->_HAS_ENTERED)) {
            _BASE->_HAS_ENTERED = TRUE;

            CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_MOUSE_ENTER);

            if (_EVENT_HANDLE != NULL)
              _EVENT_HANDLE(&_BASE);
          }

          CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_MOUSE_MOVE);

          if (_EVENT_HANDLE != NULL)
            _EVENT_HANDLE(&_BASE);

          TRACKMOUSEEVENT TME;
          TME.cbSize = sizeof(TRACKMOUSEEVENT);
          TME.dwFlags = TME_LEAVE;
          TME.hwndTrack = _HANDLE;

          TrackMouseEvent(&TME);

          break;
        };
        case (WM_MOUSELEAVE): {
          CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_MOUSE_LEAVE);

          if (_EVENT_HANDLE != NULL)
            _EVENT_HANDLE(&_BASE);

          break;
        };

        case (WM_MOUSEWHEEL): {
          _BASE->_WHEEL_DELTA = GET_WHEEL_DELTA_WPARAM(_WPARAM);

          CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_MOUSE_WHEEL);

          if (_EVENT_HANDLE != NULL)
            _EVENT_HANDLE(&_BASE);

          break;
        };
      }
    }

    return CallWindowProcW(
      _BASE->_PREV_PROC,
      _HANDLE,
      _MESSAGE,
      _WPARAM,
      _LPARAM
    );
  }

  return 0;
}

cic_button* cic_createButton(
  cic_widget* _PARENT,
  wchar_t* _TEXT,
  wchar_t* _ID,
  const cic_point _POSITION,
  const cic_size _SIZE,
  bool _OWNER_DRAW
) {
  cic_button* BUTTON = (cic_button*)calloc(1, sizeof(struct _BUTTON_STRUCT));

  if (BUTTON != NULL) {
    BUTTON->_BASE = cic_createWidget(_PARENT, _TEXT, _ID, _POSITION, _SIZE, false);

    if (BUTTON->_BASE != NULL) {
      BUTTON->_BASE->_WIDGET_UPCAST_REF = BUTTON;
      BUTTON->_OWNER_DRAW = _OWNER_DRAW;

      BUTTON->_BASE->_HANDLE = CreateWindowExW(
        0L,
        L"BUTTON",
        _TEXT,
        WS_VISIBLE | WS_TABSTOP | WS_CHILD | BS_FLAT | ((_OWNER_DRAW == true) ? BS_OWNERDRAW : 0),
        _POSITION.X, _POSITION.Y,
        _SIZE.WIDTH, _SIZE.HEIGHT,
        cic_getWidgetHandle(_PARENT),
        (HMENU)0,
        ((HINSTANCE)GetWindowLongPtr(
          cic_getWidgetHandle(_PARENT),
          GWLP_HINSTANCE
        )),
        BUTTON
      );

      if (BUTTON->_BASE->_HANDLE == (HWND)0) {
        cic_destroyButton(&BUTTON);

        return NULL;
      }

      BUTTON->_BASE->_PREV_PROC = (WNDPROC)SetWindowLongPtr(
        cic_getButtonHandle(BUTTON),
        GWLP_WNDPROC,
        (LONG_PTR)_BTN_PROC
      );
      cic_insertRawHandleRef(
        cic_getButtonWidgetRef(BUTTON)
      );
      ShowWindow(
        cic_getButtonHandle(BUTTON),
        SW_SHOW
      );
      UpdateWindow(
        cic_getButtonHandle(BUTTON)
      );

      return BUTTON;
    }
    else {
      free(BUTTON);
      BUTTON = NULL;
    }
  }

  return NULL;
}

HWND cic_getButtonHandle(cic_button* _SELF) {
  if (_SELF != NULL) {
    cic_widget* _WIDGET = *cic_getButtonWidgetRef(_SELF);

    if (_WIDGET != NULL)
      return _WIDGET->_HANDLE;
  }

  return 0;
}
cic_widget** cic_getButtonWidgetRef(cic_button* _SELF) {
  if (_SELF != NULL) {
    if (_SELF->_BASE != NULL)
      return &(_SELF->_BASE);
  }

  return NULL;
}

bool cic_setButtonPosition(
  cic_button** _SELF,
  cic_point _POINT
) {
  if (*_SELF != NULL) {
    return cic_setWidgetPosition(
      cic_getButtonWidgetRef(*_SELF),
      (cic_point) {
        .X = _POINT.X,
        .Y = _POINT.Y
      }
    );
  }

  return false;
}

bool cic_setButtonX(
  cic_button** _SELF,
  signed int _X
) {
  if (*_SELF != NULL) {
    return cic_setWidgetX(
      cic_getButtonWidgetRef(*_SELF),
      _X
    );
  }

  return false;
}
bool cic_setButtonY(
  cic_button** _SELF,
  signed int _Y
) {
  if (*_SELF != NULL) {
    return cic_setWidgetX(
      cic_getButtonWidgetRef(*_SELF),
      _Y
    );
  }

  return false;
}
bool cic_setButtonSize(
  cic_button** _SELF,
  cic_size _SIZE
) {
  if (*_SELF != NULL) {
    return cic_setWidgetSize(
      cic_getButtonWidgetRef(*_SELF),
      _SIZE
    );
  }

  return false;
}

bool cic_setButtonWidth(
  cic_button** _SELF,
  signed int _WIDTH
) {
  if (*_SELF != NULL) {
    return cic_setWidgetWidth(
      cic_getButtonWidgetRef(*_SELF),
      _WIDTH
    );
  }

  return false;
}
bool cic_setButtonHeight(
  cic_button** _SELF,
  signed int _HEIGHT
) {
  if (*_SELF != NULL) {
    return cic_setWidgetHeight(
      cic_getButtonWidgetRef(*_SELF),
      _HEIGHT
    );
  }

  return false;
}
bool cic_setButtonW(
  cic_button** _SELF,
  signed int _WIDTH
) {
  return cic_setButtonWidth(_SELF, _WIDTH);
}
bool cic_setButtonH(
  cic_button** _SELF,
  signed int _HEIGHT
) {
  return cic_setButtonHeight(_SELF, _HEIGHT);
}

bool cic_setButtonShortcut(
  cic_button** _SELF,
  DWORD* _VK_KEYCODES,
  size_t _nVK_KEYCODES
) {
  if (*_SELF != NULL && _VK_KEYCODES != NULL) {
    cic_button* _BUTTON = *_SELF;
    _BUTTON->_SHORTCUT = (DWORD*)calloc(_nVK_KEYCODES, sizeof(DWORD));

    if (_BUTTON->_SHORTCUT != NULL) {
      _BUTTON->_nSHORTCUTS = _nVK_KEYCODES;
      memcpy_s(_BUTTON->_SHORTCUT, _nVK_KEYCODES, _VK_KEYCODES, _nVK_KEYCODES);

      if (_BUTTON->_SHORTCUT != NULL) {
        if (memcmp(_BUTTON->_SHORTCUT, _VK_KEYCODES, _nVK_KEYCODES) == 0)
          return true;
      }
    }
   }

  return false;
}

bool cic_setButtonStyle(
  cic_button** _SELF,
  DWORD _STYLE
) {
  if (*_SELF != NULL) {
    cic_button* _BUTTON = *_SELF;

    return SetWindowLongPtrW(
      cic_getButtonHandle(_BUTTON),
      GWL_STYLE,
      (LONG_PTR)WS_VISIBLE | WS_TABSTOP | WS_CHILD | ((_BUTTON->_OWNER_DRAW == true) ? BS_OWNERDRAW : 0) | _STYLE
    );
  }

  return false;
}

cic_size cic_getButtonSize(cic_button* _SELF) {
  if (_SELF != NULL)
    return cic_getWidgetSize(*cic_getButtonWidgetRef(_SELF));

  return (cic_size) { .WIDTH = -1, .HEIGHT = -1 };
}
signed int cic_getButtonWidth(cic_button* _SELF) {
  if (_SELF != NULL)
    return cic_getWidgetW(*cic_getButtonWidgetRef(_SELF));

  return -1;
}
signed int cic_getButtonHeight(cic_button* _SELF) {
  if (_SELF != NULL)
    return cic_getWidgetH(*cic_getButtonWidgetRef(_SELF));

  return -1;
}
signed int cic_getButtonW(cic_button* _SELF) { return cic_getButtonWidth(_SELF); }
signed int cic_getButtonH(cic_button* _SELF) { return cic_getButtonHeight(_SELF); }

cic_point cic_getButtonPosition(cic_button* _SELF) {
  if (_SELF != NULL)
    return cic_getWidgetPosition(*cic_getButtonWidgetRef(_SELF));

  return (cic_point) { .X = -1, .X = -1 };
}
signed int cic_getButtonX(cic_button* _SELF) {
  if (_SELF != NULL)
    return cic_getWidgetX(*cic_getButtonWidgetRef(_SELF));

  return -1;
}
signed int cic_getButtonY(cic_button* _SELF) {
  if (_SELF != NULL)
    return cic_getWidgetY(*cic_getButtonWidgetRef(_SELF));

  return -1;
}

cic_point cic_getButtonCenter(cic_button* _SELF) {
  if (_SELF != NULL)
    return cic_getWidgetCenter(*cic_getButtonWidgetRef(_SELF));

  return (cic_point) { .X = -1, .X = -1 };
}
signed int cic_getButtonCenterX(cic_button* _SELF) {
  if (_SELF != NULL)
    return cic_getWidgetCenterX(*cic_getButtonWidgetRef(_SELF));

  return -1;
}
signed int cic_getButtonCenterY(cic_button* _SELF) {
  if (_SELF != NULL)
    return cic_getWidgetCenterY(*cic_getButtonWidgetRef(_SELF));

  return -1;
}

DWORD cic_getButtonCheckState(cic_button* _SELF) {
  if (_SELF != NULL) {

  }

  return 0;
}

bool cic_destroyButton(cic_button** _SELF) {
  if (*_SELF != NULL) {
    cic_destroyWidget(
      cic_getButtonWidgetRef(*_SELF)
    );

    free(*_SELF);
    *_SELF = NULL;
    return true;
  }

  return false;
}