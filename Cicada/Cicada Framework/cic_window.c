#include "include/cic_window.h"
#include "include/cic_widget.h"
#include "include/cic_leak_detector.h"

static void _WND_DRAW(
  cic_window* _SELF,
  HDC _DC,
  PAINTSTRUCT* _PS
) {
  if (_SELF != NULL) {
    cic_widget* _BASE = *cic_getWindowWidgetRef(_SELF);

    if (_BASE != NULL) {
      HBRUSH BRUSH = CreateSolidBrush(RGB(_BASE->_BG_COLOR.RED, _BASE->_BG_COLOR.GREEN, _BASE->_BG_COLOR.BLUE));

      if (!(_BASE->_HAS_SHAPE))
        FillRect(_DC, &_PS->rcPaint, BRUSH);
      else {
        HRGN REGION = CreateRectRgn(0, 0, 0, 0);
        GetWindowRgn(_BASE->_HANDLE, REGION);

        if (REGION != (HRGN)ERROR && REGION != (HRGN)NULLREGION) {
          FillRgn(_DC, REGION, BRUSH);

          DeleteObject(REGION);
        }
      }

      DeleteObject(BRUSH);
    }
  }
}

static LRESULT CALLBACK _WND_PROC(
  HWND _HANDLE,
  UINT _MESSAGE,
  WPARAM _WPARAM,
  LPARAM _LPARAM
) {
  if (_MESSAGE == WM_NCCREATE) {
    if (cic_getWidgetByRawHandle(_HANDLE) == NULL) {
      LPCREATESTRUCT _LPCS = (LPCREATESTRUCT)_LPARAM;

      if (_LPCS != NULL) {
        cic_window* _WINDOW = (cic_window*)(_LPCS->lpCreateParams);

        if (_WINDOW != NULL) {
          cic_insertRawHandleRef(
            cic_getWindowWidgetRef(_WINDOW)
          );
        }
      }
    }

    return DefWindowProc(
      _HANDLE, 
      _MESSAGE,
      _WPARAM,
      _LPARAM
    );
  }

  cic_widget* _BASE = cic_getWidgetByRawHandle(_HANDLE);
 
  if (_BASE != NULL) {
    cic_window* _this = (cic_window*)_BASE->_WIDGET_UPCAST_REF;

    if (_this != NULL) {
      if (_MESSAGE >= 306 && _MESSAGE <= 312)
        return SendMessage((HWND)_LPARAM, _MESSAGE, _WPARAM, _LPARAM);

      switch (_MESSAGE) {
        case WM_CREATE: {
          CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_CREATE);

          if (_EVENT_HANDLE != NULL)
            _EVENT_HANDLE(&_BASE);

          break;
        };

        case WM_CLOSE: {
          if (IsWindow(_HANDLE) && !(_BASE->_DESTROYED))
            cic_destroyWindow(&_this);

          break;
        }
        case WM_DESTROY: {
          PostQuitMessage(0);

          break;
        };

        case WM_ERASEBKGND: {
          return false;
        };
        case WM_PAINT: {
          RECT CLT_RECT;
          GetClientRect(_HANDLE, &CLT_RECT);

          PAINTSTRUCT _PS;
          HDC _DC = BeginPaint(_HANDLE, &_PS);

          HDC _MEM_DC = CreateCompatibleDC(_DC);
          HBITMAP _MEM_BMP = CreateCompatibleBitmap(_DC, _PS.rcPaint.right - _PS.rcPaint.left, _PS.rcPaint.bottom - _PS.rcPaint.top);
          HBITMAP _MEM_O_BMP = (HBITMAP)SelectObject(_MEM_DC, _MEM_BMP);
          POINT _O_ORGN;

          OffsetViewportOrgEx(_MEM_DC, -(CLT_RECT.left), -(CLT_RECT.top), &_O_ORGN);

          _WND_DRAW(_this, _MEM_DC, &_PS);

          SetViewportOrgEx(_MEM_DC, _O_ORGN.x, _O_ORGN.y, NULL);

          BitBlt(
            _DC,
            0, 0,
            CLT_RECT.right, CLT_RECT.bottom,
            _MEM_DC, 0, 0,
            SRCCOPY
          );

          EndPaint(_HANDLE, &_PS);

          SelectObject(_MEM_DC, _MEM_O_BMP);
          DeleteObject(_MEM_BMP);
          DeleteDC(_MEM_DC);

          return 0;
        };

        case WM_SYSCOMMAND: {
          switch (_WPARAM) {
            case SC_MAXIMIZE: {
              _this->_MAXIMIZED = true;

              CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_MAXIMIZED);

              if (_EVENT_HANDLE != NULL)
                _EVENT_HANDLE(&_BASE);
              
              break;
            };
            case SC_MINIMIZE: {
              _this->_MINIMIZED = true;

              CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_MINIMIZED);

              if (_EVENT_HANDLE != NULL)
                _EVENT_HANDLE(&_BASE);

              break;
            };
            case SC_RESTORE: {
              if (_this->_MAXIMIZED) {
                _this->_MAXIMIZED = false;

                CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_RESTORE_FROM_MAXIMIZED);

                if (_EVENT_HANDLE != NULL)
                  _EVENT_HANDLE(&_BASE);
              }
              else if (_this->_MINIMIZED) {
                _this->_MINIMIZED = false;

                CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_RESTORE_FROM_MINIMIZED);

                if (_EVENT_HANDLE != NULL)
                  _EVENT_HANDLE(&_BASE);
              }

              break;
            };
          }

          break;
        };

        case WM_KEYDOWN: {
          CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_KEYDOWN);

          if (_EVENT_HANDLE != NULL)
            _EVENT_HANDLE(&_BASE);

          break;
        };
        case WM_SYSKEYDOWN: {
          CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_KEYDOWN);

          if (_EVENT_HANDLE != NULL)
            _EVENT_HANDLE(&_BASE);

          break;
        }
        case WM_KEYUP: {
          CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_KEYUP);

          if (_EVENT_HANDLE != NULL)
            _EVENT_HANDLE(&_BASE);

          break;
        };
        case WM_SYSKEYUP: {
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
          _this->_HMONITOR = MonitorFromWindow(
            cic_getWindowHandle(_this),
            MONITOR_DEFAULTTONEAREST
          );

          RECT _RECT;
          GetWindowRect(
            cic_getWindowHandle(_this),
            &_RECT
          );
          cic_point _PT = cic_rectToPoint(_RECT);

          _BASE->_POSITION.X = _PT.X;
          _BASE->_POSITION.Y = _PT.Y;

          CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_MOVE);

          if (_EVENT_HANDLE != NULL)
            _EVENT_HANDLE(&_BASE);

          break;
        };
        case (WM_SIZE): {
          _this->_HMONITOR = MonitorFromWindow(
            cic_getWindowHandle(_this),
            MONITOR_DEFAULTTONEAREST
          );

          RECT _RECT;
          GetWindowRect(
            cic_getWindowHandle(_this),
            &_RECT
          );
          cic_size _SZ = cic_rectToSize(_RECT);

          _BASE->_SIZE.WIDTH = _SZ.WIDTH;
          _BASE->_SIZE.HEIGHT = _SZ.HEIGHT;

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
          if (_WPARAM) {
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
          if (_this->_MINIMIZED)
            _this->_MINIMIZED = false;

          if (_WPARAM) {
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

        case (CICM_FULLSCREEN): {
          if (!(_this->_MINIMIZED)) {
            _this->_BFSM_SZ = cic_getWindowSize(_this);
            _this->_BFSM_PT = (cic_point){
              .X = cic_getWindowPosition(_this).X,
              .Y = cic_getWindowPosition(_this).Y
            };
            _this->_BFSM_STYLE = (DWORD)GetWindowLongPtr(cic_getWindowHandle(_this), GWL_STYLE);

            cic_size _SSZ = cic_getScreenSizeByWindow(_this, false);
            cic_point _SPT = cic_getScreenPointByWindow(_this, false);
            HRGN _RECT_RGN = CreateRectRgn(
              _SPT.X, _SPT.Y,
              _SSZ.WIDTH, _SSZ.HEIGHT
            );

            SetWindowLongPtr(
              cic_getWindowHandle(_this),
              GWL_STYLE,
              _this->_BFSM_STYLE & ~(WS_OVERLAPPEDWINDOW)
            );

            cic_setWindowPosition(
              &_this,
              (cic_point) {
                .X = 0,
                .Y = 0
              }
            );
            cic_setWindowSize(
              &_this,
              _SSZ
            );

            SetWindowRgn(
              cic_getWindowHandle(_this),
              _RECT_RGN,
              true
            );

            _this->_FULLSCREEN_MODE = true;

            CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_FULLSCREEN);

            if (_EVENT_HANDLE != NULL)
              _EVENT_HANDLE(&_BASE);

            return _this->_FULLSCREEN_MODE;
          }
        };
        case (CICM_RESTORE_FS): {
          if (!(_this->_MINIMIZED)) {
            if (_BASE != NULL) {
              if (_BASE->_HAS_SHAPE) {
                cic_setWidgetShape(
                  &_BASE,
                  _BASE->_SHAPE
                );
              }
              else {
                SetWindowLongPtr(
                  cic_getWindowHandle(_this),
                  GWL_STYLE,
                  _this->_BFSM_STYLE
                );
                SetWindowRgn(
                  cic_getWindowHandle(_this),
                  NULL,
                  true
                );
              }
            }

            if (!(_this->_MAXIMIZED)) {
              _this->_FULLSCREEN_MODE = !SetWindowPos(
                cic_getWindowHandle(_this),
                (HWND)NULL,
                _this->_BFSM_PT.X, _this->_BFSM_PT.Y,
                _this->_BFSM_SZ.WIDTH, _this->_BFSM_SZ.HEIGHT,
                0
              );
            }
            else
              _this->_FULLSCREEN_MODE = false;

            _this->_BFSM_PT = (cic_point){ .X = 0, .Y = 0 };
            _this->_BFSM_SZ = (cic_size){ .WIDTH = 0, .HEIGHT = 0 };
            _this->_BFSM_STYLE = 0;

            CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_RESTORE_FROM_FULLSCREEN);

            if (_EVENT_HANDLE != NULL)
              _EVENT_HANDLE(&_BASE);

            return _this->_FULLSCREEN_MODE;
          }

          return 0;
        };

        case (CICM_MAXIMIZE): {
          if (!(_this->_FULLSCREEN_MODE)) {
            _this->_MAXIMIZED = ShowWindow(
              cic_getWindowHandle(_this),
              SW_MAXIMIZE
            );

            if (_BASE->_HAS_SHAPE) {
              cic_size _SSZ = cic_getScreenSizeByWindow(_this, true);
              cic_point _SPT = cic_getScreenPointByWindow(_this, true);
              HRGN _RECT_RGN = CreateRectRgn(
                _SPT.X, _SPT.Y,
                _SSZ.WIDTH, _SSZ.HEIGHT
              );

              SetWindowRgn(
                cic_getWindowHandle(_this),
                _RECT_RGN,
                true
              );
            }

            CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_MAXIMIZED);

            if (_EVENT_HANDLE != NULL)
              _EVENT_HANDLE(&_BASE);

            return _this->_MAXIMIZED;
          }

          return 0;
        };
        case (CICM_RESTORE_MXZ): {
          if (_this->_MAXIMIZED) {
            _this->_MAXIMIZED = !ShowWindow(
              cic_getWindowHandle(_this),
              SW_RESTORE
            );

            if (_BASE != NULL) {
              if (_BASE->_HAS_SHAPE) {
                cic_setWidgetShape(
                  &_BASE,
                  _BASE->_SHAPE
                );
              }
            }

            CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_RESTORE_FROM_MAXIMIZED);

            if (_EVENT_HANDLE != NULL)
              _EVENT_HANDLE(&_BASE);

            return _this->_MAXIMIZED;
          }

          return 0;
        };

        case (CICM_MINIMIZE): {
          if (!(_this->_MINIMIZED)) {
            _this->_MINIMIZED = ShowWindow(
              cic_getWindowHandle(_this),
              SW_MINIMIZE
            );

            CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_MINIMIZED);

            if (_EVENT_HANDLE != NULL)
              _EVENT_HANDLE(&_BASE);

            return _this->_MINIMIZED;
          }

          return 0;
        };
        case (CICM_RESTORE_MIZ): {
          if (_this->_MINIMIZED) {
            _this->_MINIMIZED = ShowWindow(
              cic_getWindowHandle(_this),
              SW_RESTORE
            );

            CIC_CALLBACK _EVENT_HANDLE = cic_getEventHandler(_BASE, EVENT_RESTORE_FROM_MINIMIZED);

            if (_EVENT_HANDLE != NULL)
              _EVENT_HANDLE(&_BASE);

            return _this->_MINIMIZED;
          }

          return 0;
        };
      }
    }
  }

  return DefWindowProc(
    _HANDLE,
    _MESSAGE,
    _WPARAM,
    _LPARAM
  );
}

cic_window* cic_createWindow(
  wchar_t* _TITLE,
  wchar_t* _ID,
  cic_point _POSITION,
  cic_size _SIZE
) {
  cic_window* WINDOW = (cic_window*)calloc(1, sizeof(struct _WINDOW_STRUCT));

  if (WINDOW != NULL) {
    WINDOW->_BASE = cic_createWidget(NULL, _TITLE, _ID, _POSITION, _SIZE, false);

    if (WINDOW->_BASE != NULL) {
      WINDOW->_BASE->_WIDGET_UPCAST_REF = WINDOW;
      WINDOW->_BFSM_STYLE = WS_VISIBLE | WS_OVERLAPPEDWINDOW;

      WNDCLASSW WND_CLASS;
      SecureZeroMemory(&WND_CLASS, sizeof(WNDCLASSW));
      WND_CLASS.lpszClassName = _ID;
      WND_CLASS.hInstance = GetModuleHandle(NULL);
      WND_CLASS.hCursor = LoadCursor(NULL, IDC_ARROW);
      WND_CLASS.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
      WND_CLASS.style = CS_HREDRAW | CS_VREDRAW;
      WND_CLASS.lpfnWndProc = _WND_PROC;

      if (RegisterClass(&WND_CLASS) == 0) {
        cic_destroyWindow(&WINDOW);

        return NULL;
      }

      WINDOW->_BASE->_HANDLE = CreateWindowEx(
        0L,
        _ID,
        _TITLE,
        WS_VISIBLE | WS_OVERLAPPEDWINDOW,
        _POSITION.X, _POSITION.Y,
        _SIZE.WIDTH, _SIZE.HEIGHT,
        (HWND)NULL, (HMENU)NULL,
        WND_CLASS.hInstance,
        WINDOW
      );

      if (WINDOW->_BASE->_HANDLE == (HWND)0) {
        cic_destroyWindow(&WINDOW);

        return NULL;
      }

      WINDOW->_HMONITOR = MonitorFromWindow(
        cic_getWindowHandle(WINDOW),
        MONITOR_DEFAULTTONEAREST
      );

      cic_insertRawHandleRef(cic_getWindowWidgetRef(WINDOW));
      cic_showWindow(&WINDOW);
      UpdateWindow(cic_getWindowHandle(WINDOW));

      return WINDOW;
    }
    else {
      free(WINDOW);
      return NULL;
    }
  }

  return NULL;
}

cic_window* cic_createWindowCRT(
  wchar_t* _TITLE,
  wchar_t* _ID,
  cic_point _POSITION,
  cic_size _SIZE,
  CIC_CALLBACK _CRT_FUNC_REF
) {
  cic_window* WINDOW = (cic_window*)calloc(1, sizeof(struct _WINDOW_STRUCT));

  if (WINDOW != NULL) {
    WINDOW->_BASE = cic_createWidget(NULL, _TITLE, _ID, _POSITION, _SIZE, false);

    if (WINDOW->_BASE != NULL) {
      WINDOW->_BASE->_WIDGET_UPCAST_REF = WINDOW;

      WINDOW->_BFSM_STYLE = WS_VISIBLE | WS_OVERLAPPEDWINDOW;
      cic_createEventHandler(&WINDOW->_BASE, EVENT_CREATE, _CRT_FUNC_REF);

      WNDCLASSW WND_CLASS;
      SecureZeroMemory(&WND_CLASS, sizeof(WNDCLASSW));
      WND_CLASS.lpszClassName = _ID;
      WND_CLASS.hInstance = GetModuleHandle(NULL);
      WND_CLASS.hCursor = LoadCursor(0, IDC_ARROW);
      WND_CLASS.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
      WND_CLASS.style = CS_HREDRAW | CS_VREDRAW;
      WND_CLASS.lpfnWndProc = _WND_PROC;

      if (RegisterClass(&WND_CLASS) == 0) {
        cic_destroyWindow(&WINDOW);

        return NULL;
      }

      WINDOW->_BASE->_HANDLE = CreateWindowEx(
        0L,
        _ID,
        _TITLE,
        WS_VISIBLE | WS_OVERLAPPEDWINDOW,
        _POSITION.X, _POSITION.Y,
        _SIZE.WIDTH, _SIZE.HEIGHT,
        (HWND)0, (HMENU)0,
        WND_CLASS.hInstance,
        WINDOW
      );

      if (WINDOW->_BASE->_HANDLE == NULL) {
        cic_destroyWindow(&WINDOW);

        return NULL;
      }

      WINDOW->_HMONITOR = MonitorFromWindow(
        cic_getWindowHandle(WINDOW),
        MONITOR_DEFAULTTONEAREST
      );

      cic_insertRawHandleRef(cic_getWindowWidgetRef(WINDOW));
      cic_showWindow(&WINDOW);
      UpdateWindow(cic_getWindowHandle(WINDOW));

      return WINDOW;
    }
    else {
      free(WINDOW);
      return NULL;
    }
  }

  return NULL;
}

cic_widget** cic_getWindowWidgetRef(cic_window* _SELF) {
  if (_SELF != NULL) {
    if (_SELF->_BASE != NULL)
      return &_SELF->_BASE;
  }

  return NULL;
}
HWND cic_getWindowHandle(cic_window* _SELF) {
  if (_SELF != NULL) {
    cic_widget** _WIDGET_REF = cic_getWindowWidgetRef(_SELF);

    if (_WIDGET_REF != NULL) {
      cic_widget* _WIDGET = *_WIDGET_REF;

      if (_WIDGET != NULL)
        return _WIDGET->_HANDLE;
    }
  }

  return (HWND)0;
}

signed int cic_startWindowMessageLoop(cic_window** _SELF) {
  if (*_SELF != NULL) {
    cic_window* _WINDOW = *_SELF;
    HWND _HANDLE = cic_getWindowHandle(_WINDOW);

    MSG MESSAGE_STRUCT;

    for (;;) {
      while (PeekMessage(
        &MESSAGE_STRUCT,
        (HWND)NULL,
        0, 0,
        PM_REMOVE
      )) {
        TranslateMessage(&MESSAGE_STRUCT);
        DispatchMessage(&MESSAGE_STRUCT);

        if (MESSAGE_STRUCT.message == WM_QUIT) {
          if (_WINDOW != NULL) {
            if (IsWindow(_HANDLE) && !((*_SELF)->_BASE->_DESTROYED))
              cic_destroyWindow(&*_SELF);
          }

          return (signed int)MESSAGE_STRUCT.wParam;
        }
      }

      WaitMessage();
    }

    return (signed int)MESSAGE_STRUCT.wParam;
  }

  return -1;
}

bool cic_setWindowPosition(
  cic_window** _SELF, 
  cic_point _POINT
) {
  if (*_SELF != NULL) {
    cic_point _SPOINT = cic_getScreenPointByWindow(*_SELF, true);

    return cic_setWidgetPosition(
      cic_getWindowWidgetRef(*_SELF),
      (cic_point) {
        .X = _SPOINT.X + _POINT.X,
        .Y = _SPOINT.Y + _POINT.Y
      }
    );
  }

  return false;
}

bool cic_setWindowX(
  cic_window** _SELF,
  signed int _X
) {
  if (*_SELF != NULL) {
    cic_point _SPOINT = cic_getScreenPointByWindow(*_SELF, true);

    return cic_setWidgetX(
      cic_getWindowWidgetRef(*_SELF),
      _SPOINT.X + _X
    );
  }

  return false;
}
bool cic_setWindowY(
  cic_window** _SELF,
  signed int _Y
) {
  if (*_SELF != NULL) {
    cic_point _SPOINT = cic_getScreenPointByWindow(*_SELF, true);

    return cic_setWidgetY(
      cic_getWindowWidgetRef(*_SELF),
      _SPOINT.Y + _Y
    );
  }

  return false;
}
bool cic_setWindowSize(
  cic_window** _SELF,
  cic_size _SIZE
) {
  if (*_SELF != NULL) {
    return cic_setWidgetSize(
      cic_getWindowWidgetRef(*_SELF),
      _SIZE
    );
  }

  return false;
}

bool cic_setWindowWidth(
  cic_window** _SELF,
  signed int _WIDTH
) {
  if (*_SELF != NULL) {
    return cic_setWidgetWidth(
      cic_getWindowWidgetRef(*_SELF),
      _WIDTH
    );
  }

  return false;
}
bool cic_setWindowHeight(
  cic_window** _SELF,
  signed int _HEIGHT
) {
  if (*_SELF != NULL) {
    return cic_setWidgetHeight(
      cic_getWindowWidgetRef(*_SELF),
      _HEIGHT
    );
  }

  return false;
}
bool cic_setWindowW(
  cic_window** _SELF,
  signed int _WIDTH
) {
  return cic_setWindowWidth(_SELF, _WIDTH);
}
bool cic_setWindowH(
  cic_window** _SELF, 
  signed int _HEIGHT
) {
  return cic_setWindowHeight(_SELF, _HEIGHT);
}

bool cic_windowToCenterScreen(cic_window** _SELF) {
  if (*_SELF != NULL) {
    cic_window* _WINDOW = *_SELF;

    MONITORINFO _MI;
    _MI.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(
      _WINDOW->_HMONITOR,
      &_MI
    );

    RECT _WORK_AREA = _MI.rcWork;
    signed int X = _WORK_AREA.left;
    signed int Y = _WORK_AREA.top;
    signed int WIDTH = _WORK_AREA.right - _WORK_AREA.left;
    signed int HEIGHT = _WORK_AREA.bottom - _WORK_AREA.top;

    signed int CENTER_X = (X + (WIDTH / 2)) - (cic_getWindowW(_WINDOW) / 2);
    signed int CENTER_Y = Y + (HEIGHT / 2) - (cic_getWindowH(_WINDOW) / 2);

    return cic_setWidgetPosition(
      cic_getWindowWidgetRef(_WINDOW),
      (cic_point) {
        CENTER_X,
        CENTER_Y
      }
    );
  }

  return false;
}
bool cic_windowFullscreenMode(cic_window** _SELF) {
  if (*_SELF != NULL) {
    cic_window* _WINDOW = *_SELF;

    if (_WINDOW->_BASE != NULL) {
      if (!(_WINDOW->_FULLSCREEN_MODE)) {
        PostMessage(
          cic_getWindowHandle(_WINDOW),
          CICM_FULLSCREEN,
          0, 0
        );
      }
      else {
        PostMessage(
          cic_getWindowHandle(_WINDOW),
          CICM_RESTORE_FS,
          0, 0
        );
      }
    }
  }

  return false;
}
bool cic_maximizeWindow(cic_window** _SELF) {
  if (*_SELF != NULL) {
    cic_window* _WINDOW = *_SELF;

    if (_WINDOW->_BASE != NULL) {
      if (!(_WINDOW->_MAXIMIZED)) {
        PostMessage(
          cic_getWindowHandle(_WINDOW),
          CICM_MAXIMIZE,
          0, 0
        );
      }
      else {
        PostMessage(
          cic_getWindowHandle(_WINDOW),
          CICM_RESTORE_MXZ,
          0, 0
        );
      }
    }
  }

  return false;
}
bool cic_minimizeWindow(cic_window** _SELF) {
  if (*_SELF != NULL) {
    cic_window* _WINDOW = *_SELF;

    if (_WINDOW->_BASE != NULL) {
      if (!(_WINDOW->_MINIMIZED)) {
        PostMessage(
          cic_getWindowHandle(_WINDOW),
          CICM_MINIMIZE,
          0, 0
        );
      }
      else {
        PostMessage(
          cic_getWindowHandle(_WINDOW),
          CICM_RESTORE_MIZ,
          0, 0
        );
      }
    }
  }

  return false;
}
bool cic_iconizeWindow(cic_window** _SELF) {
  return cic_minimizeWindow(_SELF);
}
bool cic_setWindowTitle(cic_window** _SELF, const wchar_t* _TITLE) {
  if (*_SELF != NULL) {
    cic_window* _WINDOW = *_SELF;

    return SetWindowText(
      cic_getWindowHandle(_WINDOW),
      _TITLE
    );
  }

  return false;
}

bool cic_showWindow(cic_window** _SELF) {
  if (*_SELF != NULL) {
    cic_window* _WINDOW = *_SELF;

    if (_WINDOW != NULL) {
      return ShowWindow(
        cic_getWindowHandle(_WINDOW),
        SW_SHOW
      );
    }
  }

  return false;
}
bool cic_hideWindow(cic_window** _SELF) {
  if (*_SELF != NULL) {
    cic_window* _WINDOW = *_SELF;

    if (_WINDOW != NULL) {
      return ShowWindow(
        cic_getWindowHandle(_WINDOW),
        SW_HIDE
      );
    }
  }

  return false;
}

cic_size cic_getWindowSize(cic_window* _SELF) {
  if (_SELF != NULL)
    return cic_getWidgetSize(*cic_getWindowWidgetRef(_SELF));

  return (cic_size) { .WIDTH = -1, .HEIGHT = -1 };
}
signed int cic_getWindowWidth(cic_window* _SELF) {
  if (_SELF != NULL)
    return cic_getWidgetW(*cic_getWindowWidgetRef(_SELF));

  return -1;
}
signed int cic_getWindowHeight(cic_window* _SELF) {
  if (_SELF != NULL)
    return cic_getWidgetH(*cic_getWindowWidgetRef(_SELF));

  return -1;
}
signed int cic_getWindowW(cic_window* _SELF) {
  return cic_getWindowWidth(_SELF);
}
signed int cic_getWindowH(cic_window* _SELF) {
  return cic_getWindowHeight(_SELF);
}

cic_point cic_getWindowPosition(cic_window* _SELF) {
  if (_SELF != NULL) {
    cic_widget* _BASE = *cic_getWindowWidgetRef(_SELF);

    return cic_getWidgetPosition(_BASE);
  }

  return (cic_point) { .X = -1, .Y = -1 };
}
signed int cic_getWindowX(cic_window* _SELF) {
  if (_SELF != NULL)
    return cic_getWindowPosition(_SELF).X;

  return -1;
}
signed int cic_getWindowY(cic_window* _SELF) {
  if (_SELF != NULL)
    return cic_getWindowPosition(_SELF).Y;

  return -1;
}

cic_point cic_getWindowCenter(cic_window* _SELF) {
  if (_SELF != NULL) {
    cic_point _CENTER = { 
      .X = cic_getWindowW(_SELF) / 2,
      .Y = cic_getWindowH(_SELF) / 2
    };

    return _CENTER;
  }

  return (cic_point) { .X = -1, .Y = -1 };
}
signed int cic_getWindowCenterX(cic_window* _SELF) {
  if (_SELF != NULL)
    return cic_getWindowCenter(_SELF).X;

  return -1;
}
signed int cic_getWindowCenterY(cic_window* _SELF) {
  if (_SELF != NULL)
    return cic_getWindowCenter(_SELF).Y;

  return -1;
}

bool cic_isWindowFullscreenMode(cic_window* _SELF) {
  if (_SELF != NULL)
    return _SELF->_FULLSCREEN_MODE;

  return false;
}
bool cic_isWindowMaximized(cic_window* _SELF) {
  if (_SELF != NULL)
    return _SELF->_MAXIMIZED;

  return false;
}

bool cic_isWindowMinimized(cic_window* _SELF) {
  if (_SELF != NULL)
    return _SELF->_MINIMIZED;

  return false;
}
bool cic_isWindowIconized(cic_window* _SELF) {
  return cic_isWindowMinimized(_SELF);
}

void cic_exitWindow(cic_window** _SELF) {
  if (*_SELF != NULL) {
    cic_window* _WINDOW = *_SELF;
    HWND _HANDLE = cic_getWindowHandle(_WINDOW);

    if (_HANDLE != (HWND)NULL) {
      SendMessage(
        _HANDLE,
        WM_CLOSE,
        (WPARAM)0,
        (LPARAM)0
      );
    }
  }
}

bool cic_restoreWindow(cic_window** _SELF) {
  if (*_SELF != NULL) {
    cic_window* _WINDOW = *_SELF;

    if (_WINDOW->_FULLSCREEN_MODE) {
      return SendMessage(
        cic_getWindowHandle(_WINDOW),
        CICM_RESTORE_FS,
        0, 0
      );
    }
    else if (_WINDOW->_MAXIMIZED) {
      return SendMessage(
        cic_getWindowHandle(_WINDOW),
        CICM_RESTORE_MXZ,
        0, 0
      );
    }
    else if (_WINDOW->_MINIMIZED) {
      return SendMessage(
        cic_getWindowHandle(_WINDOW),
        CICM_RESTORE_MIZ,
        0, 0
      );
    }
  }

  return false;
}
bool cic_redrawWindow(cic_window** _SELF) {
  if (*_SELF != NULL) {
    cic_window* _WINDOW = *_SELF;
    cic_widget* _WIDGET = *cic_getWindowWidgetRef(_WINDOW);

    if (_WIDGET != NULL)
      return cic_redrawWidget(&_WIDGET);
  }

  return false;
}
bool cic_destroyWindow(cic_window** _SELF) {
  if (*_SELF != NULL) {
    cic_destroyWidget(
      cic_getWindowWidgetRef(*_SELF)
    );

    free(*_SELF);
    *_SELF = NULL;

    return true;
  }

  return false;
}
