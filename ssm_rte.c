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
uint8_t ModuleDepend_HmiAdapte[Te_MainState_HmiAdapte_Count][Te_ModuleId_Count];
/***********************************************/

/***********************************************/
Te_MainStateType_u8 g_MainStateType_HmiAdapte[Te_MainState_HmiAdapte_Count];
Te_MainStateType_u8 g_MainStateType_CruiseManager[Te_MainState_CM_Count];
Te_MainStateType_u8 g_MainStateType_ADV2HR[Te_MainState_ADV2HR_Count];
/***********************************************/



