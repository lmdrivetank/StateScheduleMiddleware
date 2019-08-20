#include "datadomain.h"
#include "ssm_type.h"


/********************init*************************/
int InitProcess_SSM()
{
  int index = 0;  
  
  /*init rte by analysis user config*/
  InitModuleDepend();
  InitModuleStateType();
  /**/
  InitModuleHeartBeatState();
  /**/
  InitUserFunctionInput();
}
/*********************************************/
int TerminateProcess_SSM()
{
  return 0;
}
/*********************************************/
int UserFunctionInputCheck_SSM()
{
  uint8_t system_state;
  
  //calcture system dst state, and get state path plan, and manager by context
  
  SetModuleState(Te_ModuleId_System, system_state);
}
/*********************************************/
int ModuleStateCheckAll_SSM()
{
  int module_id;
  uint8_t buff_index;//ModuleId_DpendLevelRaise
  
  for (buff_index = 0; buff_index < Te_ModuleId_Count; buff_index++)
  {
    module_id = g_SsmInfo.ModuleId_DpendLevelRaise[buff_index];
    if (ModuleHeartBeatCheck_SSM(module_id) == -1 || ModuleStateCheck_SSM(module_id) == -1)
      return -1;
  }
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
  
}
