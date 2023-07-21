
#ifndef  CIC_LEAK_DETECTOR_H
#define  CIC_LEAK_DETECTOR_H

#define  FILE_NAME_LENGTH 256
#define  OUTPUT_FILE "CIC_LEAK_INFO.txt"
#define  malloc(size) CIC_MALLOC (size, __FILE__, __LINE__)
#define  calloc(elements, size)	CIC_CALLOC (elements, size, __FILE__, __LINE__)
#define  free(mem_ref) CIC_FREE(mem_ref)

typedef struct _MEM_INFO {
  void* _ADDR;
  unsigned int _SZ;
  char _FNAME[FILE_NAME_LENGTH];
  unsigned int _LN;
} MEM_INFO;

typedef struct _MEM_LEAK {
  MEM_INFO _MEM_INFO;
  struct MEM_LEAK* _NEXT;
} MEM_LEAK;

void ADD(MEM_INFO ALLOC_INFO);
void ERASE(unsigned POS);
void CLEAR(void);

void* CIC_MALLOC(unsigned int SZ, const char* FL, unsigned int LN);
void* CIC_CALLOC(unsigned int nELEMENTS, unsigned int SZ, const char* FL, unsigned int LN);
void CIC_FREE(void* MEM_REF);

void ADD_MEM_INFO(void* MEM_REF, unsigned int size, const char* FL, unsigned int LN);
void REM_MEM_INFO(void* MEM_REF);
void REP_MEM_LEAK(void);

#endif
