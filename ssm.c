#include "datadomain.h"
#include "ssm_type.h"
#include "ssm_config.h"

/********************init*************************/
extern void InitUserFunctionInput();
int InitProcess_SSM()
{
  int result = Te_workPerformanceResult_Succ;
  int index = 0;  
  
  printf("InitProcess_SSM\n");
  /*init rte by analysis user config*/
  InitModuleDepend();
  InitModuleStateType();
  /**/
  InitModuleHeartBeatState();
  /**/
  InitUserFunctionInput();
  /**init succeed will transfer state to run immediately**/
  SetSystemMainState(Te_MainState_System_Run);
    
  return result;
}
/*********************************************/
int TerminateProcess_SSM()
{
  int result = Te_workPerformanceResult_Succ;
  printf("TerminateProcess_SSM\n");
  return result;
}
/*********************************************/
void InitUserFunctionInput()
{
  Te_ModuleMainState_u8 state = Te_MainState_HmiAdapte_Idle;
  printf("InitUserFunctionInput\n");
  SetMainState(Te_ModuleId_HmiAdapte, state);
}
#ifdef __DEBUG_MODE__
Te_ModuleMainState_u8 state_request = Te_MainState_HmiAdapte_Idle;
#endif
int UserFunctionInputCheck_SSM()
{
  int result = Te_workPerformanceResult_Succ;
  Te_ModuleMainState_u8 state = Te_MainState_HmiAdapte_Idle;
  printf("UserFunctionInputCheck_SSM\n");
  //calcture system dst state, and get state path plan, and manager by context
  state = state_request;
  SetMainState(Te_ModuleId_HmiAdapte, state);
  return result;
}
/*********************************************/
int ModuleStateCheckAll_SSM()
{
  int result = Te_workPerformanceResult_Succ;
  int module_id;
  uint8_t buff_index;//ModuleId_DpendLevelRaise
  printf("ModuleStateCheckAll_SSM\n");
  for (buff_index = 0; buff_index < Te_ModuleId_Count; buff_index++)
  {
    module_id = g_SsmInfo.ModuleId_DpendLevelRaise[buff_index];
    if (ModuleHeartBeatCheck_SSM(module_id) == -1 || ModuleStateCheck_SSM(module_id) == -1)
    {
      result = Te_workPerformanceResult_Fail;
      break;
    }
  }
  return result;
}
#if 0
int SystemFunctionStateCheck_SSM()
{
  uint8_t run_level = GetSystemRunLevel();
  uint8_t depend_module_main_state;
  uint8_t depend_module_id;
  uint8_t index;
  for (index = 0; index < sizeof(SystemDependModule)/sizeof(uint8_t); index++)
  {
    depend_module_id = SystemDependModule[index];
    GetModuleState(depend_module_id, &depend_module_main_state, NULL);
    if (GetSystemRunDepend(run_level, depend_moduleId) != depend_module_main_state)
      return -1;
  }
  return 0;
}
#endif
/*********************************************/
int ReportModuleError_SSM(uint8_t moduleId, uint8_t errorId)
{
  printf("ReportModuleError_SSM module id: %d\n", moduleId);
}
