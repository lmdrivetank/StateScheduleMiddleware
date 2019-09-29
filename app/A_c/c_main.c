#include "ssm.h"
#include "c.h"

Ts_ThreadWorkflow c_workflow_Init[] = 
{
  {NULL, InitProcess_c},
  {NULL, NULL}
};

Ts_ThreadWorkflow c_workflow_Terminate[] = 
{
  {NULL, TerminateProcess_c},
  {NULL, NULL}
};
Ts_ThreadWorkflow c_workflow_CookRice[] = 
{
  {NULL, GetRice_c},
  {NULL, GetWater_c},
  {NULL, CookRice_c},
  {NULL, NULL}
};
Ts_ThreadWorkflow c_workflow_CollectClothes[] = 
{
  {NULL, CollectClothes_c},
  {NULL, StoreClothes_c},
  {NULL, NULL}
};
Ts_ThreadWorkflow c_workflow_PreSloganThree[] =
{
  {NULL, PreSloganThree_c},
  {NULL, NULL}
};
Ts_ThreadWorkflow c_workflow_SloganThree[] = 
{
  {NULL, SloganThree_c},
  {NULL, NULL}
};

Ts_threadStateMachineBranch c_sm_branch[] =
{
  {Te_MainState_C_Idle,                 NULL},
  {Te_MainState_C_Init,                 c_workflow_Init},
  {Te_MainState_C_Terminate,            c_workflow_Terminate},
  {Te_MainState_C_CookRice,             c_workflow_CookRice},
  {Te_MainState_C_CollectClothes,       c_workflow_CollectClothes},
  {Te_MainState_C_PreSloganThree,          c_workflow_PreSloganThree},
  {Te_MainState_C_SloganThree,          c_workflow_SloganThree},
  
  {Te_MainState_All_End, NULL}
};


void thread_create_c()
{
    module_thread_create(     Te_ModuleId_C,
                              NULL,                   //if NULL, will be set module_thread_runnable; else user self define (only ssm except)
                              1000,              //cycle time us
								1024,
                              c_sm_branch);
}
void thread_release_c()
{
  module_thread_release( Te_ModuleId_C );
}