#ifndef CIC_GROUP_H
#define CIC_GROUP_H

#include "cic_globals.h"

#ifdef __cplusplus
extern "C" {
#endif

  cic_group* cic_createGroup(cic_widget** _WIDGET_REF);

  bool cic_addWidgetToGroup(
    cic_group** _SELF,
    cic_widget** _WIDGET_REF
  );
  signed int cic_findWidgetIdxInGroup(
    cic_group* _SELF,
    const wchar_t* _WIDGET_ID
  );
  cic_widget* cic_getWidgetInGroup(
    cic_group* _SELF,
    const wchar_t* _WIDGET_ID
  );
  unsigned int cic_getChildCountInGroup(cic_group* _SELF);
  cic_widget* cic_getGroupWidgetRef(cic_group* _SELF);
  bool cic_removeWidgetFromGroup(
    cic_group** _SELF,
    const wchar_t* _WIDGET_ID
  );

#ifdef __cplusplus
}
#endif

#endif