#include "ssm.h"
#include "b.h"
Ts_ThreadWorkflow b_workflow_Init[] = 
{
  {NULL, InitProcess_b},
  {NULL, NULL}
};

Ts_ThreadWorkflow b_workflow_Terminate[] = 
{
  {NULL, TerminateProcess_b},
  {NULL, NULL}
};
Ts_ThreadWorkflow b_workflow_WashRice[] = 
{
  {NULL, GetRice_b},
  {NULL, WashRice_b},
  {NULL, NULL}
};
Ts_ThreadWorkflow b_workflow_DryClothes[] = 
{
  {NULL, GetClothes_b},
  {NULL, HangClothes_b},
  {NULL, DryClothes_b},
  {NULL, NULL}
};
Ts_ThreadWorkflow b_workflow_PreSloganTwo[] =
{
  {NULL, PreSloganTwo_b},
  {NULL, NULL}
};
Ts_ThreadWorkflow b_workflow_SloganTwo[] = 
{
  {NULL, SloganTwo_b},
  {NULL, NULL}
};
Ts_threadStateMachineBranch b_sm_branch[] =
{
  {Te_MainState_B_Idle,         NULL},
  {Te_MainState_B_Init,         b_workflow_Init},
  {Te_MainState_B_Terminate,    b_workflow_Terminate},
  {Te_MainState_B_WashRice,     b_workflow_WashRice},
  {Te_MainState_B_DryClothes,   b_workflow_DryClothes},
  {Te_MainState_B_PreSloganTwo,    b_workflow_PreSloganTwo},
  {Te_MainState_B_SloganTwo,    b_workflow_SloganTwo},
  
  {Te_MainState_All_End, NULL}
};

void thread_create_b()
{
    module_thread_create(     Te_ModuleId_B,
                              NULL,                   //if NULL, will be set module_thread_runnable; else user self define (only ssm except)
                              1000,              //cycle time us
								1024,
                              b_sm_branch);
}

void thread_release_b()
{
  module_thread_release( Te_ModuleId_B );
}