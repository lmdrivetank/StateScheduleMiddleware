#ifndef _SSM_RTE_H_
#define _SSM_RTE_H_

#include "ssm_type.h"

void SetSystemMainState(Te_SystemMainState_u8 state);
Te_SystemMainState_u8 GetSystemMainState();

/******************************************************/
extern uint8_t ModuleDepend_Manager[Te_MainState_Manager_Count][Te_ModuleId_Count];
extern uint8_t ModuleDepend_B[Te_MainState_B_Count][Te_ModuleId_Count];
extern uint8_t ModuleDepend_C[Te_MainState_C_Count][Te_ModuleId_Count];
/***********************************************/

/***********************************************/
extern Te_MainStateType_u8 g_MainStateType_Manager[Te_MainState_Manager_Count];
extern Te_MainStateType_u8 g_MainStateType_A[Te_MainState_A_Count];
extern Te_MainStateType_u8 g_MainStateType_B[Te_MainState_B_Count];
extern Te_MainStateType_u8 g_MainStateType_C[Te_MainState_C_Count];
/***********************************************/

#endif