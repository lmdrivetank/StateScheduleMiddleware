#include "ssm.h"
#include "manager.h"

Ts_ThreadWorkflow manager_workflow_Init[] = 
{
  {NULL, InitProcess_manager},
  {NULL, NULL}
};

Ts_ThreadWorkflow manager_workflow_Terminate[] = 
{
  {NULL, TerminateProcess_manager},
  {NULL, NULL}
};
Ts_ThreadWorkflow manager_workflow_CookRice[] = 
{
  {NULL, CookRice_manager},
  {NULL, NULL}
};
Ts_ThreadWorkflow manager_workflow_WashClothes[] = 
{
  {NULL, WashClothes_manager},
  {NULL, NULL}
};
Ts_ThreadWorkflow manager_workflow_PreSlogan[] =
{
  {NULL, PreSlogan_manager},
  {NULL, NULL}
};
Ts_ThreadWorkflow manager_workflow_Slogan[] = 
{
  {NULL, Slogan_manager},
  {NULL, NULL}
};

Ts_threadStateMachineBranch manager_sm_branch[] =
{
  {Te_MainState_Manager_Idle,                   NULL},
  {Te_MainState_Manager_Init,                   manager_workflow_Init},
  {Te_MainState_Manager_Terminate,              manager_workflow_Terminate},
  {Te_MainState_Manager_CookRice,               manager_workflow_CookRice},
  {Te_MainState_Manager_WashClothes,            manager_workflow_WashClothes},
  {Te_MainState_Manager_PreSlogan,                 manager_workflow_PreSlogan},
  {Te_MainState_Manager_Slogan,                 manager_workflow_Slogan},
  
  {Te_MainState_All_End, NULL}
};


void thread_create_manager()
{
    module_thread_create(     Te_ModuleId_Manager,
                              NULL,                   //if NULL, will be set module_thread_runnable; else user self define (only ssm except)
                              1000,              //cycle time us
								1024,
                              manager_sm_branch);
}
void thread_release_manager()
{
  module_thread_release( Te_ModuleId_Manager );
}