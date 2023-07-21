#include <cic_window.h>
#include <cic_button.h>
#include <cic_widget.h>
#include <cic_leak_detector.h>

int main(int argc, char* argv[]) {
  atexit(REP_MEM_LEAK);

  cic_point _POINT = { 400, 400 };
  cic_size _SIZE = { 800, 600 };
  cic_point _PTS[] = { { 0, 0 }, { 800, 0 }, { 800, 600 }, { 50, 600 }, { 0, 550 }, { 0, 0 } };
  cic_region* _REGION = cic_createPolygonRegion(
    _PTS,
    6,
    WINDING
  );
  cic_region* _ROUND_RGN = cic_createRoundRectRegion(
    (cic_point) {
      0, 0
    },
    _SIZE, 12
  );
  cic_combineRegion(cic_getHRGN(_ROUND_RGN), cic_getHRGN(_REGION), CMODE_INTERSECTION);

  cic_window* WINDOW = cic_createWindow(
    L"TEST",
    L"cicwin",
    _POINT,
    _SIZE
  );
  cic_setWidgetBackgroundColor(
    cic_getWindowWidgetRef(WINDOW),
    (cic_color) {
      100, 25, 145
    }
  );

  cic_setWindowTitle(
    &WINDOW,
    L"REWRITE"
  );
  cic_windowToCenterScreen(&WINDOW);
  cic_setWidgetShape(
    cic_getWindowWidgetRef(WINDOW),
    _ROUND_RGN
  );

  cic_region* _BTN_REGION = cic_createRoundRectRegion(
    (cic_point) {
      0, 0
    },
    (cic_size) {
      100, 50
    },
    12
  );
  cic_button* BUTTON = cic_createButton(
    *cic_getWindowWidgetRef(WINDOW),
    L"Button",
    L"cicbtn",
    (cic_point) {
      200, 200
    },
    (cic_size) {
      100, 50
    },
    false
  );
  cic_setWidgetBackgroundColor(
    cic_getButtonWidgetRef(BUTTON),
    (cic_color) {
      120, 155, 170
    }
  );
  cic_setWidgetShape(
    cic_getButtonWidgetRef(BUTTON),
    _BTN_REGION
  );

  int _RET = cic_startWindowMessageLoop(&WINDOW);
  cic_destroyRegion(&_REGION);
  cic_destroyRegion(&_BTN_REGION);

  return _RET;
}
