#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "include/cic_leak_detector.h"

#undef malloc
#undef calloc
#undef free

static MEM_LEAK* _PTR_BEG = NULL;
static MEM_LEAK* _PTR_NEXT = NULL;

void ADD(MEM_INFO _ALLOC_INFO) {
  MEM_LEAK* _MEM_LEAK_INFO = NULL;
  _MEM_LEAK_INFO = (MEM_LEAK*)malloc(sizeof(MEM_LEAK));

  if (_MEM_LEAK_INFO != NULL) {
    _MEM_LEAK_INFO->_MEM_INFO._ADDR = _ALLOC_INFO._ADDR;
    _MEM_LEAK_INFO->_MEM_INFO._SZ = _ALLOC_INFO._SZ;

    strcpy(_MEM_LEAK_INFO->_MEM_INFO._FNAME, _ALLOC_INFO._FNAME);

    _MEM_LEAK_INFO->_MEM_INFO._LN = _ALLOC_INFO._LN;
    _MEM_LEAK_INFO->_NEXT = NULL;

    if (_PTR_BEG == NULL) {
      _PTR_BEG = _MEM_LEAK_INFO;
      _PTR_NEXT = _PTR_BEG;
    }
    else {
      _PTR_NEXT->_NEXT = _MEM_LEAK_INFO;
      _PTR_NEXT = _PTR_NEXT->_NEXT;
    }
  }
}

void ERASE(unsigned _POS) {
  unsigned _IDX = 0;
  MEM_LEAK* _TEMP = NULL;
  MEM_LEAK* _ALLOC_INFO = NULL;

  if (_POS == 0) {
    MEM_LEAK* _TEMP = _PTR_BEG;
    _PTR_BEG = _PTR_BEG->_NEXT;
    free(_TEMP);
  }
  else {
    for (_IDX = 0, _ALLOC_INFO = _PTR_BEG; _IDX < _POS; _ALLOC_INFO = _ALLOC_INFO->_NEXT, ++_IDX) {
      if (_POS == _IDX + 1) {
        _TEMP = _ALLOC_INFO->_NEXT;
        _ALLOC_INFO->_NEXT = _TEMP->_NEXT;
        free(_TEMP);
        break;
      }
    }
  }
}

void CLEAR()
{
  MEM_LEAK* _TEMP = _PTR_BEG;
  MEM_LEAK* _ALLOC_INFO = _PTR_BEG;

  while (_ALLOC_INFO != NULL) {
    _ALLOC_INFO = _ALLOC_INFO->_NEXT;
    free(_TEMP);
    _TEMP = _ALLOC_INFO;
  }
}

void* CIC_MALLOC(unsigned int _SZ, const char* _FL, unsigned int _LN) {
  void* _PTR = malloc(_SZ);

  if (_PTR != NULL) {
    ADD_MEM_INFO(_PTR, _SZ, _FL, _LN);
  }

  return _PTR;
}

void* CIC_CALLOC(unsigned int _nELEMENTS, unsigned int _SZ, const char* _FL, unsigned int _LN) {
  unsigned TOT_SZ;
  void* _PTR = calloc(_nELEMENTS, _SZ);

  if (_PTR != NULL) {
    TOT_SZ = _nELEMENTS * _SZ;
    ADD_MEM_INFO(_PTR, TOT_SZ, _FL, _LN);
  }

  return _PTR;
}

void CIC_FREE(void* MEM_REF) {
  REM_MEM_INFO(MEM_REF);
  free(MEM_REF);
}

void ADD_MEM_INFO(void* _MEM_REF, unsigned int _SZ, const char* _FL, unsigned int _LN) {
  MEM_INFO _MEM_ALLOC_INFO;

  memset(&_MEM_ALLOC_INFO, 0, sizeof(_MEM_ALLOC_INFO));
  _MEM_ALLOC_INFO._ADDR = _MEM_REF;
  _MEM_ALLOC_INFO._SZ = _SZ;
  strncpy(_MEM_ALLOC_INFO._FNAME, _FL, FILE_NAME_LENGTH);
  _MEM_ALLOC_INFO._LN = _LN;

  ADD(_MEM_ALLOC_INFO);
}

void REM_MEM_INFO(void* _MEM_REF) {
  MEM_LEAK* _LEAK_INFO = _PTR_BEG;

  for (unsigned short _IDX = 0; _LEAK_INFO != NULL; ++_IDX, _LEAK_INFO = _LEAK_INFO->_NEXT) {
    if (_LEAK_INFO->_MEM_INFO._ADDR == _MEM_REF) {
      ERASE(_IDX);

      break;
    }
  }
}

void REP_MEM_LEAK(void) {
  MEM_LEAK* _LEAK_INFO;

  FILE* _FP = fopen(OUTPUT_FILE, "wt");
  char _INFO[1024];

  if (_FP != NULL) {
    if (_PTR_BEG != NULL) {
      sprintf(_INFO, "%s\n", "CIC Memory Leak Summary");
      fwrite(_INFO, (strlen(_INFO) + 1), 1, _FP);
      sprintf(_INFO, "%s\n", "[-----------------------------------]");
      fwrite(_INFO, (strlen(_INFO) + 1), 1, _FP);

      for (_LEAK_INFO = _PTR_BEG; _LEAK_INFO != NULL; _LEAK_INFO = _LEAK_INFO->_NEXT) {
        sprintf(_INFO, "address : %p\n", _LEAK_INFO->_MEM_INFO._ADDR);
        fwrite(_INFO, (strlen(_INFO) + 1), 1, _FP);
        sprintf(_INFO, "size    : %d bytes\n", _LEAK_INFO->_MEM_INFO._SZ);
        fwrite(_INFO, (strlen(_INFO) + 1), 1, _FP);
        sprintf(_INFO, "file    : %s\n", _LEAK_INFO->_MEM_INFO._FNAME);
        fwrite(_INFO, (strlen(_INFO) + 1), 1, _FP);
        sprintf(_INFO, "line    : %d\n", _LEAK_INFO->_MEM_INFO._LN);
        fwrite(_INFO, (strlen(_INFO) + 1), 1, _FP);
        sprintf(_INFO, "%s\n", "[-----------------------------------]");
        fwrite(_INFO, (strlen(_INFO) + 1), 1, _FP);
      }
    }
  }

  CLEAR();
}
