#include <cic_window.h>
#include <cic_button.h>
#include <cic_widget.h>
#include <cic_group.h>
#include <cic_leak_detector.h>
#include "resource.h"

void _EXIT_CALLBACK(cic_widget** _SELF) {
  cic_exit(0);
}
void _FSCRN_CALLBACK(cic_widget** _SELF) {
  if (*_SELF != NULL) {
    cic_widget* _TOP_WINDOW = *_SELF;

    if (_TOP_WINDOW != NULL) {
      if ((cic_isKeyDown(VK_MENU, false) && cic_isKeyDown(VK_RETURN, false)) || cic_isKeyDown(VK_F11, false))
        cic_windowFullscreenMode(&cic_upcastWidgetRef(_TOP_WINDOW, cic_window*));
      else if (cic_isKeyDown(VK_ESCAPE, false))
        cic_restoreWindow(&cic_upcastWidgetRef(_TOP_WINDOW, cic_window*));
    }
  }
}
void _MAX_CALLBACK(cic_widget** _SELF) {
  if (*_SELF != NULL) {
    cic_widget* _TOP_WINDOW = cic_getWidgetParent(*_SELF);

    if (_TOP_WINDOW != NULL)
      cic_maximizeWindow(&cic_upcastWidgetRef(_TOP_WINDOW, cic_window*));
  }
}
void _MIN_CALLBACK(cic_widget** _SELF) {
  if (*_SELF != NULL) {
    cic_widget* _TOP_WINDOW = cic_getWidgetParent(*_SELF);

    if (_TOP_WINDOW != NULL)
      cic_minimizeWindow(&cic_upcastWidgetRef(_TOP_WINDOW, cic_window*));
  }
}
void _SIZE_CALLBACK(cic_widget** _SELF) {
  if (*_SELF != NULL) {
    if (cic_isWindowMaximized(cic_upcastWidgetRef((*_SELF), cic_window*))) {
      cic_widget* _MAX_BTN_WIDGET = cic_getWidgetById(L"MAX_BUTTON");
      cic_setWidgetLabel(&_MAX_BTN_WIDGET, L"2");
    }
    else {
      cic_widget* _MAX_BTN_WIDGET = cic_getWidgetById(L"MAX_BUTTON");
      cic_setWidgetLabel(&_MAX_BTN_WIDGET, L"1");
    }
  }
}

signed int main(signed int argc, char* argv[]) {
  atexit(REP_MEM_LEAK);

  signed int _RET = 0;

  cic_size _SIZE = { .WIDTH = 800, .HEIGHT = 600 };
  cic_point _POINT = cic_calcCenterPoint(
    cic_getScreenCenterByIdx(0, true),
    _SIZE
  );
  cic_point _PTS[] = {
    { .X = 0, .Y = 0 }, 
    { .X = 800, .Y = 0 },
    { .X = 800, .Y = 600 },
    { .X = 50, .Y = 600 },
    { .X = 0, .Y = 550 },
    { .X = 0, .Y = 0 }
  };
  cic_region* _REGION = cic_createPolygonRegion(
    _PTS,
    6,
    WINDING
  );
  cic_region* _ROUND_RGN = cic_createRoundRectRegion(
    (cic_point) {
      .X = 0,
      .Y = 0
    },
    _SIZE, 12
  );
  cic_combineRegion(cic_getHRGN(_ROUND_RGN), cic_getHRGN(_REGION), CMODE_INTERSECTION);

  cic_window* WINDOW = cic_createWindow(
    L"Cicada Framework - Init Window",
    L"cicwin",
    _POINT,
    _SIZE
  );
  if (WINDOW != NULL) {
    HINSTANCE _HINSTANCE = (HINSTANCE)GetWindowLongPtr(cic_getWindowHandle(WINDOW), GWLP_HINSTANCE);
    HICON ICON = LoadIcon(_HINSTANCE, MAKEINTRESOURCE(IDI_ICON1));

    PostMessage(cic_getWindowHandle(WINDOW), WM_SETICON, ICON_BIG, (LPARAM)ICON);
    PostMessage(cic_getWindowHandle(WINDOW), WM_SETICON, ICON_SMALL, (LPARAM)ICON);

    cic_setWidgetBackgroundColor(
      cic_getWindowWidgetRef(WINDOW),
      (cic_color) {
        .RED = 170,
        .GREEN = 0,
        .BLUE = 0
      }
    );

    cic_setWindowTitle(
      &WINDOW,
      L"Cicada Framework"
    );
    cic_setWidgetShape(
      cic_getWindowWidgetRef(WINDOW),
      _ROUND_RGN
    );

    cic_createEventHandler(
      cic_getWindowWidgetRef(WINDOW),
      EVENT_MAXIMIZED,
      &_SIZE_CALLBACK
    );
    cic_createEventHandler(
      cic_getWindowWidgetRef(WINDOW),
      EVENT_RESTORE_FROM_MAXIMIZED,
      &_SIZE_CALLBACK
    );
    cic_createEventHandler(
      cic_getWindowWidgetRef(WINDOW),
      EVENT_KEYDOWN,
      &_FSCRN_CALLBACK
    );

    /* TOP BUTTONS HANDLER */

    cic_region* TOP_BUTTON_RND_RGN = cic_createRoundRectRegion(
      (cic_point) {
        .X = 0,
        .Y = 0
      },
      (cic_size) {
        .WIDTH = 25,
        .HEIGHT = 25
      },
      12
    );
    cic_font TOP_BUTTON_FONT = cic_createFont(
      L"Marlett",
      13,
      0,
      WEIGHT_DEFAULT,
      CHARSET_SYMBOL,
      QUALITY_DEFAULT,
      false, false, false
    );

    cic_button* EXIT_BUTTON = cic_createButton(
      *cic_getWindowWidgetRef(WINDOW),
      L"r",
      L"EXIT_BUTTON",
      (cic_point) {
        cic_getWindowW(WINDOW) - 28,
        3
      },
      (cic_size) {
        .WIDTH = 25,
        .HEIGHT = 25
      },
      true
    );

    if (EXIT_BUTTON != NULL) {
      cic_setWidgetForegroundColor(
        cic_getButtonWidgetRef(EXIT_BUTTON),
        (cic_color) {
          .RED = 255,
          .GREEN = 255,
          .BLUE = 255
        }
      );
      cic_setWidgetBackgroundColor(
        cic_getButtonWidgetRef(EXIT_BUTTON),
        (cic_color) {
          .RED = 100,
          .GREEN = 0,
          .BLUE = 0
        }
      );
      cic_setWidgetShape(
        cic_getButtonWidgetRef(EXIT_BUTTON),
        TOP_BUTTON_RND_RGN
      );
      cic_setWidgetLabelFont(
        cic_getButtonWidgetRef(EXIT_BUTTON), 
        TOP_BUTTON_FONT
      );

      cic_createEventHandler(
        cic_getButtonWidgetRef(EXIT_BUTTON),
        EVENT_MOUSE_LMB_UP,
        &_EXIT_CALLBACK
      );
    }
    else
      printf("EXIT_BUTTON NULL\n");

    cic_button* MAX_BUTTON = cic_createButton(
      *cic_getWindowWidgetRef(WINDOW),
      L"1",
      L"MAX_BUTTON",
      (cic_point) {
        cic_getButtonX(EXIT_BUTTON) - 27,
        3
      },
      (cic_size) {
        .WIDTH = 25,
        .HEIGHT = 25
      },
      true
    );

    if (MAX_BUTTON != NULL) {
      cic_setWidgetForegroundColor(
        cic_getButtonWidgetRef(MAX_BUTTON),
        (cic_color) {
          .RED = 255,
          .GREEN = 255,
          .BLUE = 255
        }
      );
      cic_setWidgetBackgroundColor(
        cic_getButtonWidgetRef(MAX_BUTTON),
        (cic_color) {
          .RED = 100,
          .GREEN = 0,
          .BLUE = 0
        }
      );
      cic_setWidgetShape(
        cic_getButtonWidgetRef(MAX_BUTTON),
        TOP_BUTTON_RND_RGN
      );
      cic_setWidgetLabelFont(
        cic_getButtonWidgetRef(MAX_BUTTON),
        TOP_BUTTON_FONT
      );

      cic_createEventHandler(
        cic_getButtonWidgetRef(MAX_BUTTON),
        EVENT_MOUSE_LMB_UP,
        &_MAX_CALLBACK
      );
    }
    else
      printf("MAX_BUTTON NULL\n");

    cic_button* MIN_BUTTON = cic_createButton(
      *cic_getWindowWidgetRef(WINDOW),
      L"-",
      L"MIN_BUTTON",
      (cic_point) {
        cic_getButtonX(MAX_BUTTON) - 27,
        3
      },
      (cic_size) {
        .WIDTH = 25,
        .HEIGHT = 25
      },
      true
    );

    if (MIN_BUTTON != NULL) {
      cic_setWidgetForegroundColor(
        cic_getButtonWidgetRef(MIN_BUTTON),
        (cic_color) {
          .RED = 255,
          .GREEN = 255,
          .BLUE = 255
        }
      );
      cic_setWidgetBackgroundColor(
        cic_getButtonWidgetRef(MIN_BUTTON),
        (cic_color) {
          .RED = 100,
          .GREEN = 0,
          .BLUE = 0
        }
      );
      cic_setWidgetShape(
        cic_getButtonWidgetRef(MIN_BUTTON),
        TOP_BUTTON_RND_RGN
      );

      cic_createEventHandler(
        cic_getButtonWidgetRef(MIN_BUTTON),
        EVENT_MOUSE_LMB_UP,
        &_MIN_CALLBACK
      );
    }
    else
      printf("MIN_BUTTON NULL\n");

    /* TOP BUTTONS HANDLER */

    _RET = cic_startWindowMessageLoop(&WINDOW);

    cic_destroyRegion(&TOP_BUTTON_RND_RGN);
  } else
    printf("WINDOW NULL\n");

  cic_destroyRegion(&_REGION);

  return _RET;
}
