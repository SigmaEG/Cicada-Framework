
#ifndef CIC_LEAK_DETECTOR_H
#define CIC_LEAK_DETECTOR_H

#define FILE_NAME_LENGTH 256
#define OUTPUT_FILE "CIC_LEAK_INFO.txt"
#define malloc(_SZ) CIC_MALLOC (_SZ, __FILE__, __LINE__)
#define calloc(_nELEMENTS, _SZ) CIC_CALLOC (_nELEMENTS, _SZ, __FILE__, __LINE__)
#define free(_MEM_REF) CIC_FREE(_MEM_REF)

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct _MEM_INFO {
    void* _ADDR;
    size_t _SZ;
    char _FNAME[FILE_NAME_LENGTH];
    unsigned int _LN;
  } MEM_INFO;

  typedef struct _MEM_LEAK {
    MEM_INFO _MEM_INFO;
    struct _MEM_LEAK* _NEXT;
  } MEM_LEAK;

  void ADD(MEM_INFO ALLOC_INFO);
  void ERASE(unsigned POS);
  void CLEAR(void);

  void* CIC_MALLOC(size_t SZ, const char* FL, unsigned int LN);
  void* CIC_CALLOC(size_t nELEMENTS, size_t SZ, const char* FL, unsigned int LN);
  void CIC_FREE(void* MEM_REF);

  void ADD_MEM_INFO(void* MEM_REF, size_t size, const char* FL, unsigned int LN);
  void REM_MEM_INFO(void* MEM_REF);
  void REP_MEM_LEAK(void);

#ifdef __cplusplus
}
#endif

#endif