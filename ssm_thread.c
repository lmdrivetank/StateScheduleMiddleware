#include "ssm_thread.h"
#include "ssm_config.h"

void module_thread(Te_MoudleId_u8 module_id, Ts_threadStateMachineBranch *psmBranch)
{
  Ts_ThreadWorkflow*     pWorkflow;
  int                           work_index = 0;
  int                           state_index = Te_MainState_All_Idle;  
  Te_ModuleMainState_u8         mainState;
  Te_ModuleSubState_u8          subState;  
  Te_MainStateType_u8           mainStateType;

  ModuleHeartBeat(module_id);
  ModuleStateCheck_SSM(module_id);
  
  mainState     = GetMainState(module_id);
  subState      = GetSubState(module_id);
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
  
  while (psmBranch[state_index].state != Te_MainState_All_End)
  {
    if (psmBranch[state_index].state != GetMainState_CM())
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
          SetSubState_CM(Te_SubState_StopFailed);
          return;
        }
      }
      work_index++;
    }
    if (mainStateType == Te_MainStateType_RunOnce)
    {
      SetSubState_CM(Te_SubState_StopSucceed);
    }
  }
}