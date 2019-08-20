#include "thread_work.h"
#include "ssm.h"

struct Ts_threadStateMachineBranch ssm_sm_branch[] =
{
  {Te_MainState_SSM_Idle, NULL},
  {Te_MainState_SSM_Init, ssm_work_sequ_Init},
  {Te_MainState_SSM_Run, ssm_work_sequ_Run},
  {Te_MainState_SSM_Terminate, ssm_work_sequ_Terminate},
  
  {Te_MainState_SSM_Cnt, NULL}
};
struct Ts_threadWorkSequ ssm_work_sequ_Init[] = 
{
  {NULL, InitProcess_SSM},
  {NULL, NULL}
};
struct Ts_threadWorkSequ ssm_work_sequ_Run[] = 
{
  //{NULL, RunProcess_SSM},
  {NULL, UserFunctionInputCheck_SSM},
  {NULL, ModuleStateCheckAll_SSM},
  {NULL, ReportModuleError_SSM},
  {NULL, NULL}
};
struct Ts_threadWorkSequ ssm_work_sequ_Terminate[] = 
{
  {NULL, TerminateProcess_SSM},
  {NULL, NULL}
};

void ssm_thread()
{
  int                           work_index = 0;
  int                           state_index = Te_MainState_CM_Idle;
  ModuleSubState_T              subState;
  ModuleMainState_T             mainState;
  struct Ts_threadWorkSequ*     pWorkSequ;
  Te_MainStateType_u8           mainStateType);
  uint8_t                       module_id       =        Te_ModuleId_System;    

  ModuleHeartBeat(module_id);
  ModuleStateCheck_SSM(module_id);
  
  mainState     = GetMainState(module_id);
  subState      = GetSubState(module_id);
  mainStateType = GetMainStateType(module_id, mainState);
  
  if (subState == Te_SubState_Hold)
  {
    SetSubState(module_id, Te_SubState_StopSucceed);
  }
  else if (subState != Te_SubState_Ready && subState != Te_SubState_Run)
  {
    //else if prepare, should depend module work once stop succeed, and ssm will change state to ready
    return;
  }
  else if (subState == Te_SubState_Ready)
  {
    SetSubState(module_id, Te_SubState_Run);
  }  
  
  while (cm_sm_branch[state_index].state != Te_MainState_CM_Cnt)
  {
    if (cm_sm_branch[state_index].state != GetMainState_CM())
    {
      state_index++;
      continue;
    }
    
    pWorkSequ = cm_sm_branch[state_index].workSequ;
    while(pWorkSequ[work_index].workPerformance != NULL)
    {
      if (pWorkSequ[work_index].workCondition == NULL ||
          pWorkSequ[work_index].workCondition() == Te_workConditionResult_OK)
      {
        if (pWorkSequ[work_index].workPerformance() != Te_workPerformanceResult_Succ)
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