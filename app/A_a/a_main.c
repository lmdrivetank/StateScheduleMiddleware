#include "ssm.h"
#include "a.h"

Ts_ThreadWorkflow a_workflow_Init[] = 
{
  {NULL, InitProcess_a},
  {NULL, NULL}
};

Ts_ThreadWorkflow a_workflow_Terminate[] = 
{
  {NULL, TerminateProcess_a},
  {NULL, NULL}
};
Ts_ThreadWorkflow a_workflow_BoilWater[] = 
{
  {NULL, GetWater_a},
  {NULL, BoilWater_a},
  {NULL, NULL}
};
Ts_ThreadWorkflow a_workflow_WashClothes[] = 
{
  {NULL, GetClothes_a},
  {NULL, WashClothes_a},
  {NULL, NULL}
};
Ts_ThreadWorkflow a_workflow_PreSloganOne[] =
{
  {NULL, PreSloganOne_a},
  {NULL, NULL}
};
Ts_ThreadWorkflow a_workflow_SloganOne[] = 
{
  {NULL, SloganOne_a},
  {NULL, NULL}
};
Ts_threadStateMachineBranch a_sm_branch[] =
{
  {Te_MainState_A_Idle,                 NULL},
  {Te_MainState_A_Init,                 a_workflow_Init},  
  {Te_MainState_A_Terminate,            a_workflow_Terminate},
  {Te_MainState_A_BoilWater,            a_workflow_BoilWater},
  {Te_MainState_A_WashClothes,          a_workflow_WashClothes},
  {Te_MainState_A_PreSloganOne,         a_workflow_PreSloganOne},
  {Te_MainState_A_SloganOne,            a_workflow_SloganOne},
  
  {Te_MainState_All_End, NULL}
};

void thread_create_a()
{
    module_thread_create(     Te_ModuleId_A,
                              NULL,                   //if NULL, will be set module_thread_runnable; else user self define (only ssm except)
                              1000,             //cycle time us
								1024,
                              a_sm_branch       );
}
void thread_release_a()
{
  module_thread_release( Te_ModuleId_A );
}