#include "ssm.h"
#include "ssm_type.h"
#include "ssm_config.h"
#include "ssm_thread.h"

extern Ts_ThreadWorkflow ssm_workflow_init[];
extern Ts_ThreadWorkflow ssm_workflow_run[];
extern Ts_ThreadWorkflow ssm_workflow_terminate[];

Ts_threadStateMachineBranch ssm_sm_branch[] =
{
  {Te_MainState_System_Idle,            NULL},
  {Te_MainState_System_Init,            ssm_workflow_init},
  {Te_MainState_System_PreSimple,       ssm_workflow_run},
  {Te_MainState_System_RunSimple,       ssm_workflow_run},
  {Te_MainState_System_PrePCC,          ssm_workflow_run},
  {Te_MainState_System_RunPCC,          ssm_workflow_run},
  {Te_MainState_System_Terminate,       ssm_workflow_terminate},
  
  {Te_MainState_All_End,                NULL}
};
Ts_ThreadWorkflow ssm_workflow_init[] = 
{
  {NULL, InitProcess_SSM},
  {NULL, NULL}
};
Ts_ThreadWorkflow ssm_workflow_run[] = 
{
  //{NULL, RunProcess_SSM},
  {NULL, UserFunctionInputCheck_SSM},
  {NULL, ModuleStateCheckAll_SSM},
 // {NULL, ReportModuleError_SSM},
  {NULL, NULL}
};
Ts_ThreadWorkflow ssm_workflow_terminate[] = 
{
  {NULL, TerminateProcess_SSM},
  {NULL, NULL}
};

void ssm_thread()
{
  Te_MoudleId_u8        module_id = Te_ModuleId_System;
  
  module_thread(module_id, ssm_sm_branch);
}