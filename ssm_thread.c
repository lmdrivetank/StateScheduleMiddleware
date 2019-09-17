#include "ssm_thread.h"
#include "ssm_config.h"
#include "ssm_base.h"
#include "os_app.h"


static Ts_TaskContext   g_buffTaskContext[Task_Count];

void module_thread_round(Ts_TaskContext* pTaskContext) 
{
  while (1)
  {
    vTaskDelayUntilMs(&(pTaskContext->prev_ticks), pTaskContext->cycle_ticks);
    pTaskContext->pxTaskRunnable(pTaskContext->task_id, pTaskContext->psmBranch);
  }
} 
void module_thread_create(	uint8_t                         task_id,
                                TaskRunnable_t                  pxTaskCode,     //if NULL, will be set module_thread_runnable; else user self define (only ssm except)
                        	const char * const              pcName,
                        	const uint16_t                  usStackDepth,
                        	UBaseType_t                     uxPriority,
                                const uint32_t                  uStartTimeUs,
                        	const uint32_t                  uCycleTimeUs,
                                Ts_threadStateMachineBranch*    psmBranch) 
{
  Ts_TaskContext* pTaskContext = &(g_buffTaskContext[task_id]);
  pTaskContext->task_id = task_id;
  pTaskContext->pxTaskRunnable = (pxTaskCode == NULL ? module_thread_runnable : pxTaskCode);
  pTaskContext->start_us = uStartTimeUs + TaskStartPublicDelay;
  pTaskContext->cycle_us = uCycleTimeUs;
  configASSERT(pTaskContext->start_us > uCycleTimeUs);
  pTaskContext->prev_ticks = MS_TO_TICK(pTaskContext->start_us - uCycleTimeUs)*ThreadTimeUnit;
  pTaskContext->cycle_ticks = MS_TO_TICK(uCycleTimeUs)*ThreadTimeUnit;
  pTaskContext->psmBranch = psmBranch;
  
  xTaskCreate((TaskFunction_t)module_thread_round, pcName, usStackDepth, pTaskContext, uxPriority, pTaskContext->task_handle );
}
void module_thread_release( uint8_t task_id )
{
  vTaskDelete(g_buffTaskContext[task_id].task_handle);
  
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

  if (GetSystemMainState() != Te_MainState_System_Run)
  {
    SSM_LOG(Te_AppLogLevel_INFO, "system main state is %d, module(%d) depend system running\n", GetSystemMainState(), module_id);
    return;
  }
  ModuleHeartBeat(module_id);
  ModuleStateCheck_SSM(module_id);
  
  mainState     = GetMainState(module_id);
  subState      = GetSubState(module_id);
  SSM_LOG(Te_AppLogLevel_INFO, "main state: %d\n", mainState);
  SSM_LOG(Te_AppLogLevel_INFO, "sub current state: %d\n", subState);
  mainStateType = GetMainStateType(module_id, mainState);
  
  if (subState == Te_SubState_Hold)
  {
    SetSubState(module_id, Te_SubState_StopSucceed);
    return;
  }
  else if (subState != Te_SubState_Ready && subState != Te_SubState_Run)
  {
    //if prepare, should wait depend module which work once stop succeed, and ssm will change state to ready
    //if stop succeed or next
    //if heart beat error, should wait ssm solve error
    return;
  }
  else if (subState == Te_SubState_Ready)
  {
    SetSubState(module_id, Te_SubState_Run);
  }  
  SSM_LOG(Te_AppLogLevel_INFO, "sub run state: %d\n", GetSubState(module_id));
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
        SSM_LOG(Te_AppLogLevel_INFO, "-------------\n");
        if (pWorkflow[work_index].workPerformance() != Te_workPerformanceResult_Succ)
        {
          result = Te_workPerformanceResult_Fail;
          SetSubState(module_id, Te_SubState_StopFailed);
          SSM_LOG(Te_AppLogLevel_ERROR, "sub end state: %d\n", GetSubState(module_id));
          SSM_LOG(Te_AppLogLevel_ERROR, "work performance fail module: %d, state: %d; index: %d\n", module_id, psmBranch[state_index].state, work_index);
          break;
        }
        SSM_LOG(Te_AppLogLevel_INFO, "-------------\n");
      }
      work_index++;
    }
    if (mainStateType == Te_MainStateType_RunOnce && result == Te_workPerformanceResult_Succ)
    {
      SetSubState(module_id, Te_SubState_StopSucceed);      
    }
    break;
  }
  SSM_LOG(Te_AppLogLevel_INFO, "sub end state: %d\n", GetSubState(module_id));
}
void module_thread_init()
{
  uint8_t       task_id = 0;
  for (task_id = 0; task_id < Task_Count; task_id ++)
  {
    g_buffTaskContext[task_id].task_id = UnValidTaskId;
  }
}