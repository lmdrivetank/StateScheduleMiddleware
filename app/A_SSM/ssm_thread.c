#include "ssm_thread.h"
#include "ssm_config.h"
#include "ssm_base.h"


static Ts_TaskContext   g_buffTaskContext[Task_Count];

DWORD WINAPI module_thread_round(Ts_TaskContext* pTaskContext) 
{
  while (1)
  {
	  printf("--------%s work thread start---------\n", GetModuleName(pTaskContext->task_id));
    pTaskContext->pxTaskRunnable(pTaskContext->task_id, pTaskContext->psmBranch);
	printf("--------%s work thread end---------\n", GetModuleName(pTaskContext->task_id));
	Sleep(pTaskContext->cycle_ms);
  }
  return 0;
} 
extern void module_thread_runnable(Te_MoudleId_u8 module_id, Ts_threadStateMachineBranch* psmBranch);
void module_thread_create(	uint8_t                         task_id,
                            TaskRunnable_t                  pxTaskCode,     //if NULL, will be set module_thread_runnable; else user self define (only ssm except)
                        	uint16_t						cycle_ms,
                        	const uint16_t                  usStackDepth,
                            Ts_threadStateMachineBranch*    psmBranch) 
{
  Ts_TaskContext* pTaskContext = &(g_buffTaskContext[task_id]);
  pTaskContext->task_id = task_id;
  pTaskContext->pxTaskRunnable = (pxTaskCode == NULL ? module_thread_runnable : pxTaskCode);
  pTaskContext->psmBranch = psmBranch;
  pTaskContext->cycle_ms = cycle_ms;
  pTaskContext->task_handle = CreateThread(NULL, usStackDepth, module_thread_round, pTaskContext, 0, NULL);
}
void module_thread_release( uint8_t task_id )
{
  //vTaskDelete(g_buffTaskContext[task_id].task_handle);
  TerminateThread(g_buffTaskContext[task_id].task_handle, 0);
  
  g_buffTaskContext[task_id].task_id = UnValidTaskId;
}

void module_thread_runnable(Te_MoudleId_u8 module_id, Ts_threadStateMachineBranch *psmBranch)
{
  int                           result = Te_workPerformanceResult_Succ;
  Ts_ThreadWorkflow*            pWorkflow;
  int                           work_index = 0;
  int                           state_index = Te_MainState_All_Idle;  
  Te_ModuleMainState_u8         mainState;
  Te_ModuleSubState_u8          subState;  
  Te_MainStateType_u8           mainStateType;  
  Te_SystemMainState_u8         sysMstate = GetSystemMainState();
  static BOOL					entry_enable = TRUE;  
	  
  if (sysMstate != Te_MainState_System_Run)
  {
    SSM_LOG(Te_AppLogLevel_DEBUG, "system main state is %d, module(%d) depend system running\n", GetSystemMainState(), module_id);
    return;
  }

  while (entry_enable != TRUE)
  {
	  Sleep(10);
  }
  entry_enable = FALSE;
  
  ModuleHeartBeat(module_id);
  
  mainState     = GetMainState(module_id);
  subState      = GetSubState(module_id);
  SSM_LOG(Te_AppLogLevel_DEBUG, "main state: %d\n", mainState);
  SSM_LOG(Te_AppLogLevel_DEBUG, "sub current state: %d\n", subState);
  mainStateType = GetMainStateType(module_id, mainState);
  
  if (subState == Te_SubState_Prepare || subState == Te_SubState_Ready || subState == Te_SubState_Run)
  {
	  ModuleStateCheck_SSM(module_id, TRUE);
  }

  if (subState == Te_SubState_Hold)
  {
    SetSubState(module_id, Te_SubState_StopSucceed);
	entry_enable = TRUE;
    return;
  }
  else if (subState != Te_SubState_Ready && subState != Te_SubState_Run)
  {
    //if prepare, should wait depend module which work once stop succeed, and ssm will change state to ready
    //if stop succeed or next
    //if heart beat error, should wait ssm solve error
	  entry_enable = TRUE;
    return;
  }
  else if (subState == Te_SubState_Ready)
  {
    SetSubState(module_id, Te_SubState_Run);
  }  
  SSM_LOG(Te_AppLogLevel_DEBUG, "sub run state: %d\n", GetSubState(module_id));
  

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
        SSM_LOG(Te_AppLogLevel_DEBUG, "-------------\n");
        if (pWorkflow[work_index].workPerformance() != Te_workPerformanceResult_Succ)
        {
          result = Te_workPerformanceResult_Fail;
          SetSubState(module_id, Te_SubState_StopFailed);
          SSM_LOG(Te_AppLogLevel_ERROR, "sub end state: %d\n", GetSubState(module_id));
          SSM_LOG(Te_AppLogLevel_ERROR, "work performance fail module: %d, state: %d; index: %d\n", module_id, psmBranch[state_index].state, work_index);
          break;
        }
        SSM_LOG(Te_AppLogLevel_DEBUG, "-------------\n");
      }
      work_index++;
    }
    if (mainStateType == Te_MainStateType_RunOnce && result == Te_workPerformanceResult_Succ)
    {
      SetSubState(module_id, Te_SubState_StopSucceed);      
    }
    break;
  }
  SSM_LOG(Te_AppLogLevel_DEBUG, "sub end state: %d\n", GetSubState(module_id));
  entry_enable = TRUE;
}
void module_thread_init()
{
  uint8_t       task_id = 0;
  for (task_id = 0; task_id < Task_Count; task_id ++)
  {
    g_buffTaskContext[task_id].task_id = UnValidTaskId;
  }
}