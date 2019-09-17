#include "ssm_type.h"
#include "ssm_config.h"
#include "ssm_base.h"
/********************init*************************/
int InitProcess_SSM()
{
  int result = Te_workPerformanceResult_Succ;
  
  SSM_LOG(Te_AppLogLevel_INFO, "InitProcess_SSM\n");
  
  /*init rte by analysis user config*/
  InitModuleDepend();
  SSM_LOG(Te_AppLogLevel_INFO, "InitModuleDepend end\n");
  InitModuleStateType();
  SSM_LOG(Te_AppLogLevel_INFO, "InitModuleStateType end\n");
  InitModuleHeartBeatState();
  SSM_LOG(Te_AppLogLevel_INFO, "InitModuleHeartBeatState end\n");
  
  /**init succeed will transfer state to run immediately**/
  SetSystemMainState(Te_MainState_System_Run);
    
  return result;
}
/*********************************************/
int TerminateProcess_SSM()
{
  int result = Te_workPerformanceResult_Succ;
  SSM_LOG(Te_AppLogLevel_INFO, "TerminateProcess_SSM\n");
  return result;
}
/*********************************************/

/*********************************************/
int ModuleStateCheckAll_SSM()
{
  int result = Te_workPerformanceResult_Succ;
  int module_id;
  uint8_t buff_index;//ModuleId_DpendLevelRaise
  
  SSM_LOG(Te_AppLogLevel_INFO, "ModuleStateCheckAll_SSM\n");
  
  for (buff_index = 0; buff_index < Te_ModuleId_Count; buff_index++)
  {
    module_id = g_SsmInfo.ModuleId_DpendLevelRaise[buff_index];
    if (ModuleHeartBeatCheck_SSM(module_id) == -1 || ModuleStateCheck_SSM(module_id) == -1)
    {
      //SSM_LOG(Te_AppLogLevel_ERROR, "module %d check fail(heart beat or state depend)\n", module_id);
      result = Te_workPerformanceResult_Fail;
      break;
    }
  }
  return result;
}
/*********************************************/
int ReportModuleError_SSM(uint8_t moduleId, uint8_t errorId)
{
  //SSM_LOG(Te_AppLogLevel_INFO, "ReportModuleError_SSM module id: %d\n", moduleId);
}
