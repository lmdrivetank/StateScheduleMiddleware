#ifndef _SSM_RTE_H_
#define _SSM_RTE_H_

#include "ssm_type.h"
/******************************************************/
extern uint8_t ModuleDepend_System[Te_MainState_System_Count][Te_ModuleId_Count];
/***********************************************/

/***********************************************/
extern Te_MainStateType_u8 g_MainStateType_System[Te_MainState_System_Count];
extern Te_MainStateType_u8 g_MainStateType_CruiseManager[Te_MainState_CM_Count];
extern Te_MainStateType_u8 g_MainStateType_HmiAdapte[Te_MainState_HmiAdapte_Count];
extern Te_MainStateType_u8 g_MainStateType_ADV2HR[Te_MainState_ADV2HR_Count];
/***********************************************/

#endif