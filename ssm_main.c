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
  {Te_MainState_System_Run,             ssm_workflow_run},
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
  Ts_threadStateMachineBranch*  psmBranch = ssm_sm_branch;
  Ts_ThreadWorkflow*            pWorkflow;
  int                           work_index = 0;
  int                           state_index = Te_MainState_All_Idle;  
  Te_SystemMainState_u8         mainState;
  
  mainState     = GetSystemMainState();  
  
  while (psmBranch[state_index].state != Te_MainState_All_End)
  {
    if (psmBranch[state_index].state != mainState)
    {
      state_index++;
      continue;
    }
    
    pWorkflow = psmBranch[state_index].workflow;
    while(pWorkflow[work_index].workPerformance != NULL)
    {
      if (pWorkflow[work_index].workCondition == NULL ||
          pWorkflow[work_index].workCondition() == Te_workConditionResult_OK)
      {
        if (pWorkflow[work_index].workPerformance() != Te_workPerformanceResult_Succ)
        {
          printf("system work performance fail state: %d; index: %d\n", psmBranch[state_index].state, work_index);
          return;
        }
      }
      work_index++;
    }
    break;
  }
}