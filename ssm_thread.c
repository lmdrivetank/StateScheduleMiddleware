#include "ssm_thread.h"
#include "ssm_config.h"

void module_thread(Te_MoudleId_u8 module_id, Ts_threadStateMachineBranch *psmBranch)
{
  Ts_ThreadWorkflow*            pWorkflow;
  int                           work_index = 0;
  int                           state_index = Te_MainState_All_Idle;  
  Te_ModuleMainState_u8         mainState;
  Te_ModuleSubState_u8          subState;  
  Te_MainStateType_u8           mainStateType;

  printf("\n\nthread work start id: %d\n", module_id);
  if (GetSystemMainState() != Te_MainState_System_Run)
  {
    printf("system main state is %d\n", GetSystemMainState());
    return;
  }
  ModuleHeartBeat(module_id);
  ModuleStateCheck_SSM(module_id);
  
  mainState     = GetMainState(module_id);
  subState      = GetSubState(module_id);
  printf("main state: %d\n", mainState);
  printf("sub  state: %d\n", subState);
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
  printf("sub  state: %d\n", subState);
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
        printf("*************************\n");
        if (pWorkflow[work_index].workPerformance() != Te_workPerformanceResult_Succ)
        {
          SetSubState(module_id, Te_SubState_StopFailed);
          printf("work performance fail module: %d, state: %d; index: %d\n", module_id, psmBranch[state_index].state, work_index);
          return;
        }
        printf("*************************\n");
      }
      work_index++;
    }
    if (mainStateType == Te_MainStateType_RunOnce)
    {
      SetSubState(module_id, Te_SubState_StopSucceed);
      printf("sub  state: %d\n", Te_SubState_StopSucceed);
    }
    break;
  }
}