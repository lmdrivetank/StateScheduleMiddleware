#include "ssm_rte.h"
#include "ssm_type.h"
#include "ssm_config.h"

/***********************************************/
Te_SystemMainState_u8 g_SystemMainState;
void SetSystemMainState(Te_SystemMainState_u8 state)
{
  g_SystemMainState = state;
}
Te_SystemMainState_u8 GetSystemMainState()
{
  return g_SystemMainState;
}
/***********************************************/

/******************************************************/
uint8_t ModuleDepend_Manager[Te_MainState_Manager_Count][Te_ModuleId_Count];
uint8_t ModuleDepend_B[Te_MainState_B_Count][Te_ModuleId_Count];
uint8_t ModuleDepend_C[Te_MainState_C_Count][Te_ModuleId_Count];
/***********************************************/

/***********************************************/
Te_MainStateType_u8 g_MainStateType_Manager[Te_MainState_Manager_Count];
Te_MainStateType_u8 g_MainStateType_A[Te_MainState_A_Count];
Te_MainStateType_u8 g_MainStateType_B[Te_MainState_B_Count];
Te_MainStateType_u8 g_MainStateType_C[Te_MainState_C_Count];
/***********************************************/

