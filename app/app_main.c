#include "ssm_main.h"
#include "manager_main.h"
#include "a_main.h"
#include "b_main.h"
#include "c_main.h"
#include "ssm.h"
#include <stdio.h>
Te_ModuleMainState_u8 state_request;
int main(void)
{
	printf("hello world!\n\n");
  
  //YC_data_init();
  thread_init_ssm();
  thread_create_manager();
  thread_create_a();
  thread_create_b();
  thread_create_c();
  SetSystemMainState(Te_MainState_System_Init);
#if 1
  state_request = 0;
  while (state_request <= Te_MainState_Manager_Terminate)
  {    
	  printf("\n******************************\n**************new task*************\n");
    SetMainState(Te_ModuleId_Manager, state_request, TRUE);
	Sleep(1000*5);
	state_request++;
  }
#endif

  SetSystemMainState(Te_MainState_System_Terminate);
}


