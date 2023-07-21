#include "include/cic_button.h"
#include "include/cic_widget.h"

static void _BTN_DRAW(cic_button** _SELF, HDC _DC, PAINTSTRUCT* _PS) {
  if (*_SELF != NULL) {
    cic_button* _BUTTON = *_SELF;
    cic_widget* _BASE = _BUTTON->_BASE;
    HBRUSH BRUSH = CreateSolidBrush(RGB(_BASE->_BG_COLOR.RED, _BASE->_BG_COLOR.GREEN, _BASE->_BG_COLOR.BLUE));

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
            return NULL;

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

            return NULL;
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

          return NULL;
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

  return NULL;
}

cic_button* cic_createButton(
  cic_widget* _PARENT,
  const wchar_t* _TEXT,
  const wchar_t* _ID,
  cic_point _POSITION,
  cic_size _SIZE,
  bool _OWNER_DRAW
) {
  cic_button* BUTTON = (cic_button*)calloc(1, sizeof(struct _BUTTON_STRUCT));

  if (BUTTON != NULL) {
    BUTTON->_BASE = cic_createWidget(_PARENT, _TEXT, _ID, _POSITION, _SIZE, false);

    if (BUTTON->_BASE != NULL) {
      BUTTON->_BASE->_WIDGET_UPCAST_REF = BUTTON;

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

DWORD cic_getButtonCheckState(
  cic_button* _SELF
) {
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