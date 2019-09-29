#include "ssm.h"
#include "ssm_type.h"
#include "ssm_config.h"
#include "ssm_thread.h"
#include "ssm_base.h"
#include "ssm_main.h"
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
  {NULL, ModuleStateCheckAll_SSM},
  {NULL, NULL}
};
Ts_ThreadWorkflow ssm_workflow_terminate[] = 
{
  {NULL, TerminateProcess_SSM},
  {NULL, NULL}
};
#define         TaskId_SSM      Te_ModuleId_Count
extern void thread_work_ssm(Te_MoudleId_u8 module_id, Ts_threadStateMachineBranch *psmBranch);
void thread_init_ssm()
{
  module_thread_init();
  
  module_thread_create(	TaskId_SSM,
                        thread_work_ssm,        //if NULL, will be set module_thread_runnable; else user self define (only ssm except)                        
                        1000*10,					//ms
						1024 * 2,
                        ssm_sm_branch);
}
void thread_release_ssm()
{
  module_thread_release( TaskId_SSM );
}
void thread_work_ssm(Te_MoudleId_u8 module_id, Ts_threadStateMachineBranch *psmBranch)
{
  Ts_ThreadWorkflow*            pWorkflow;
  int                           work_index = 0;
  int                           state_index = Te_MainState_All_Idle;  
  Te_SystemMainState_u8         mainState;
     
  mainState = GetSystemMainState();  
  
  while (psmBranch[state_index].state != Te_MainState_All_End)
  {
    if (psmBranch[state_index].state != mainState)
    {
      state_index++;
      continue;
    }
    
    pWorkflow = psmBranch[state_index].workflow;
    while(pWorkflow != NULL && pWorkflow[work_index].workPerformance != NULL)
    {
      if (pWorkflow[work_index].workCondition == NULL ||
          pWorkflow[work_index].workCondition() == Te_workConditionResult_OK)
      {
        //SSM_LOG(Te_AppLogLevel_INFO, "-------------\n");
        if (pWorkflow[work_index].workPerformance() != Te_workPerformanceResult_Succ)
        {
          SSM_LOG(Te_AppLogLevel_ERROR, "system work performance fail! state: %d; index: %d\n", psmBranch[state_index].state, work_index);
          SSM_LOG(Te_AppLogLevel_ERROR, "system thread end\n");
          break;
        }
        //SSM_LOG(Te_AppLogLevel_INFO, "-------------\n");
      }
      work_index++;
    }
    break;
  }
}