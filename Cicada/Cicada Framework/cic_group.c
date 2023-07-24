#include "include/cic_group.h"
#include "include/cic_leak_detector.h"

cic_group* cic_createGroup(cic_widget** _WIDGET_REF) {
  if (_WIDGET_REF != NULL) {
    if (*_WIDGET_REF != NULL) {
      cic_group* GROUP = (cic_group*)calloc(1, sizeof(struct _GROUP_STRUCT));

      if (GROUP != NULL) {
        GROUP->_BASE = *_WIDGET_REF;
        GROUP->_nCHILDREN = 0;
        GROUP->_CLIP_CHILDREN = true;
        GROUP->_RESIZEABLE = false;
      }

      return GROUP;
    }
  }

  return NULL;
}

bool cic_addWidgetToGroup(
  cic_group** _SELF,
  cic_widget** _WIDGET_REF
) {
  if (_SELF != NULL && _WIDGET_REF != NULL) {
    if (*_SELF != NULL && *_WIDGET_REF != NULL && (*_SELF)->_BASE != NULL) {
      if ((*_WIDGET_REF)->_PART_GROUP != NULL) {
        cic_removeWidgetFromGroup(
          &((*_WIDGET_REF)->_PART_GROUP),
          (*_WIDGET_REF)->_ID
        );
      }

      if ((*_SELF)->_nCHILDREN == 0 || (*_SELF)->_CHILDREN == NULL)
        (*_SELF)->_CHILDREN = (cic_widget**)calloc(1, sizeof(struct _GROUP_STRUCT));
      else {
        unsigned int _nSZ = (*_SELF)->_nCHILDREN;
        ++_nSZ;

        cic_widget** _REALLOC_TEMP = (cic_widget**)realloc(
          (*_SELF)->_CHILDREN,
          _nSZ * sizeof(struct _GROUP_STRUCT)
        );

        if (_REALLOC_TEMP != NULL)
          (*_SELF)->_CHILDREN = _REALLOC_TEMP;
      }

      if ((*_SELF)->_CHILDREN != NULL) {
        ++(*_SELF)->_nCHILDREN;
        (*_SELF)->_CHILDREN[(*_SELF)->_nCHILDREN - 1] = *_WIDGET_REF;
        (*_WIDGET_REF)->_PART_GROUP = *_SELF;

        return SetParent(
          (*_WIDGET_REF)->_HANDLE,
          (*_SELF)->_BASE->_HANDLE
        ) == (*_WIDGET_REF)->_HANDLE;
      }
    }
  }

  return false;
}

signed int cic_findWidgetIdxInGroup(
  cic_group* _SELF,
  const wchar_t* _WIDGET_ID
) {
  if (_SELF != NULL && _SELF->_nCHILDREN >= 1 && _WIDGET_ID != NULL) {
    for (unsigned int _IDX = 0; _IDX < _SELF->_nCHILDREN; ++_IDX) {
      if (_SELF->_CHILDREN[_IDX] != NULL) {
        if (_SELF->_CHILDREN[_IDX]->_ID == _WIDGET_ID)
          return (signed int)_IDX;
      }
    }
  }

  return -1;
}

cic_widget* cic_getWidgetInGroup(
  cic_group* _SELF,
  const wchar_t* _WIDGET_ID
) {
  if (_SELF != NULL && _SELF->_nCHILDREN >= 1 && _WIDGET_ID != NULL) {
    signed int _FDX = cic_findWidgetIdxInGroup(_SELF, _WIDGET_ID);

    if (_FDX != -1)
      return _SELF->_CHILDREN[_FDX];
  }

  return NULL;
}

unsigned int cic_getChildCountInGroup(cic_group* _SELF) {
  if (_SELF != NULL)
    return _SELF->_nCHILDREN;

  return 0;
}

cic_widget* cic_getGroupWidgetRef(cic_group* _SELF) {
  if (_SELF != NULL)
    return _SELF->_BASE;

  return NULL;
}

bool cic_removeWidgetFromGroup(
  cic_group** _SELF,
  const wchar_t* _WIDGET_ID
) {
  if (_SELF != NULL && _WIDGET_ID != NULL) {
    if (*_SELF != NULL) {
      if ((*_SELF)->_nCHILDREN == 0 || (*_SELF)->_CHILDREN == NULL)
        return false;

      signed int _FDX = cic_findWidgetIdxInGroup(*_SELF, _WIDGET_ID);

      if (_FDX != -1) {
        for (unsigned int _IDX = _FDX + 1; _IDX < (*_SELF)->_nCHILDREN; ++_IDX)
          (*_SELF)->_CHILDREN[_IDX - 1] = (*_SELF)->_CHILDREN[_IDX];

        --(*_SELF)->_nCHILDREN;

        cic_widget** _REALLOC_TEMP = (cic_widget**)realloc((*_SELF)->_CHILDREN, (*_SELF)->_nCHILDREN * sizeof(struct _WIDGET_STRUCT*));

        if (_REALLOC_TEMP != NULL) {
          (*_SELF)->_CHILDREN = _REALLOC_TEMP;

          return true;
        }
      }
    }
  }

  return false;
}