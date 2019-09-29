#include "ssm_type.h"
#include "ssm_config.h"
#include "ssm_rte.h"
/******************************************************/
const Ts_MainStateConfig g_MainStateConfig_Manager[] = 
{
  {Te_MainState_Manager_Idle,           Te_MainStateType_Idle},
  {Te_MainState_Manager_Init,           Te_MainStateType_RunOnce},
  {Te_MainState_Manager_Terminate,      Te_MainStateType_RunOnce},
  {Te_MainState_Manager_CookRice,       Te_MainStateType_RunOnce},
  {Te_MainState_Manager_WashClothes,    Te_MainStateType_RunOnce},
  {Te_MainState_Manager_PreSlogan,		Te_MainStateType_RunOnce},
  {Te_MainState_Manager_Slogan,         Te_MainStateType_RunCycle},
  {Te_MainState_All_End,                Te_MainStateType_End}
};
const Ts_MainStateConfig g_MainStateConfig_A[] = 
{
  {Te_MainState_A_Idle,                 Te_MainStateType_Idle},
  {Te_MainState_A_Init,                 Te_MainStateType_RunOnce},
  {Te_MainState_A_Terminate,            Te_MainStateType_RunOnce},
  {Te_MainState_A_BoilWater,            Te_MainStateType_RunOnce},
  {Te_MainState_A_WashClothes,          Te_MainStateType_RunOnce},
  {Te_MainState_A_PreSloganOne,			Te_MainStateType_RunOnce},
  {Te_MainState_A_SloganOne,            Te_MainStateType_RunCycle},
  {Te_MainState_All_End,                Te_MainStateType_End}
};
const Ts_MainStateConfig g_MainStateConfig_B[] = 
{
  {Te_MainState_B_Idle,                 Te_MainStateType_Idle},
  {Te_MainState_B_Init,                 Te_MainStateType_RunOnce},
  {Te_MainState_B_Terminate,            Te_MainStateType_RunOnce},
  {Te_MainState_B_WashRice,             Te_MainStateType_RunOnce},
  {Te_MainState_B_DryClothes,           Te_MainStateType_RunOnce},
  {Te_MainState_B_PreSloganTwo,			Te_MainStateType_RunOnce},
  {Te_MainState_B_SloganTwo,            Te_MainStateType_RunCycle},
  {Te_MainState_All_End,                Te_MainStateType_End}
};
const Ts_MainStateConfig g_MainStateConfig_C[] = 
{
  {Te_MainState_C_Idle,                 Te_MainStateType_Idle},
  {Te_MainState_C_Init,                 Te_MainStateType_RunOnce},
  {Te_MainState_C_Terminate,            Te_MainStateType_RunOnce},
  {Te_MainState_C_CookRice,             Te_MainStateType_RunOnce},
  {Te_MainState_C_CollectClothes,       Te_MainStateType_RunOnce},
  {Te_MainState_C_PreSloganThree,		Te_MainStateType_RunOnce},
  {Te_MainState_C_SloganThree,          Te_MainStateType_RunCycle},
  {Te_MainState_All_End,                Te_MainStateType_End}
};
/******************************************************/
const Ts_StateDependPair ManagerDependA[] = 
{
  {Te_MainState_Manager_Idle,           Te_MainState_A_Idle},
  {Te_MainState_Manager_Init,           Te_MainState_A_Init},
  {Te_MainState_Manager_Terminate,      Te_MainState_A_Terminate},
  {Te_MainState_Manager_PreSlogan,      Te_MainState_A_PreSloganOne},
  {Te_MainState_Manager_Slogan,         Te_MainState_A_SloganOne},
  {Te_MainState_All_End,                Te_MainState_All_End}
};
const Ts_StateDependPair ManagerDependB[] = 
{
  {Te_MainState_Manager_Idle,           Te_MainState_B_Idle},
  {Te_MainState_Manager_Init,           Te_MainState_B_Init},
  {Te_MainState_Manager_Terminate,      Te_MainState_B_Terminate},
  {Te_MainState_Manager_PreSlogan,      Te_MainState_B_PreSloganTwo},
  {Te_MainState_Manager_Slogan,         Te_MainState_B_SloganTwo},
  {Te_MainState_All_End,                Te_MainState_All_End}
};
const Ts_StateDependPair ManagerDependC[] = 
{
  {Te_MainState_Manager_Idle,           Te_MainState_C_Idle},
  {Te_MainState_Manager_Init,           Te_MainState_C_Init},
  {Te_MainState_Manager_Terminate,      Te_MainState_C_Terminate},
  {Te_MainState_Manager_CookRice,       Te_MainState_C_CookRice},
  {Te_MainState_Manager_WashClothes,    Te_MainState_C_CollectClothes},
  {Te_MainState_Manager_PreSlogan,      Te_MainState_C_PreSloganThree},
  {Te_MainState_Manager_Slogan,         Te_MainState_C_SloganThree},
  {Te_MainState_All_End,                Te_MainState_All_End}
};
const Ts_DependModuleConfig ManagerDependModuleList[] = 
{
  {
      Te_ModuleId_A, 
      ManagerDependA, 
      sizeof(ManagerDependA)/sizeof(Ts_StateDependPair)
  },
  {
      Te_ModuleId_B, 
      ManagerDependB, 
      sizeof(ManagerDependB)/sizeof(Ts_StateDependPair)
  },  
  {
      Te_ModuleId_C, 
      ManagerDependC, 
      sizeof(ManagerDependC)/sizeof(Ts_StateDependPair)
  }, 
  {     
      Te_ModuleId_End, 
      NULL, 
      0
  }
};
/******************************************************/
/******************************************************/
const Ts_StateDependPair cDependA[] = 
{
  {Te_MainState_C_CookRice,             Te_MainState_A_BoilWater},
  {Te_MainState_All_End,                Te_MainState_All_End}
};
const Ts_StateDependPair cDependB[] = 
{
  {Te_MainState_C_CookRice,             Te_MainState_B_WashRice},
  {Te_MainState_C_CollectClothes,       Te_MainState_B_DryClothes},
  {Te_MainState_All_End,                Te_MainState_All_End}
};
const Ts_DependModuleConfig cDependModuleList[] = 
{
  {
      Te_ModuleId_A, 
      cDependA, 
      sizeof(cDependA)/sizeof(Ts_StateDependPair)
  },
  {
      Te_ModuleId_B, 
      cDependB, 
      sizeof(cDependB)/sizeof(Ts_StateDependPair)
  },  
  {     
      Te_ModuleId_End, 
      NULL, 
      0
  }
};
/******************************************************/
const Ts_StateDependPair bDependA[] = 
{
  {Te_MainState_B_DryClothes,           Te_MainState_A_WashClothes},
  {Te_MainState_All_End,                Te_MainState_All_End}
};
const Ts_DependModuleConfig bDependModuleList[] = 
{
  {
      Te_ModuleId_A, 
      bDependA, 
      sizeof(bDependA)/sizeof(Ts_StateDependPair)
  },
  {     
      Te_ModuleId_End, 
      NULL, 
      0
  }
};
/******************************************************/
/******************************************************/
Ts_SsmInfo g_SsmInfo = 
{
  {//struct Ts_ModuleInfo list
    {//struct Ts_ModuleInfo
        Te_ModuleId_Manager, 
		"经理 ",
        {//struct Ts_StateManager
          Te_MainState_All_Idle,
          Te_SubState_StopSucceed,
          Te_MainState_Manager_Count,
		  {"未收任务","初始化","做饭","洗衣服","准备喊","喊口号","退出","","",""},
          g_MainStateConfig_Manager,
          g_MainStateType_Manager
        },
        {//struct Ts_ModuleDependManager
          sizeof(ManagerDependModuleList)/sizeof(Ts_DependModuleConfig),
          ManagerDependModuleList,     
          ModuleDependInitLevel, 
          ModuleDepend_Manager
        },
        {//struct Ts_HeartBeatManager
          Te_HeartBeatState_Fine,
          0
        }
    },
    {
        Te_ModuleId_A,
		"员工A ",
        {
          Te_MainState_All_Idle,
          Te_SubState_StopSucceed,
          Te_MainState_A_Count,
		  {"未收任务","初始化","退出","烧水","洗衣服","准备喊一","喊一","","",""},
          g_MainStateConfig_A,
          g_MainStateType_A
        },
        {
          0,
          NULL,
          ModuleDependInitLevel,
          NULL
        },
        {
          Te_HeartBeatState_Fine,
          0
        }
    },
    {
        Te_ModuleId_B, 
		"员工B ",
        {
          Te_MainState_All_Idle,
          Te_SubState_StopSucceed,
          Te_MainState_B_Count,
		  {"未收任务","初始化","退出","淘米","晒衣服","准备喊二","喊二","","",""},
          g_MainStateConfig_B,
          g_MainStateType_B
        },
        {
          sizeof(bDependModuleList)/sizeof(Ts_DependModuleConfig),
          bDependModuleList,     
          ModuleDependInitLevel, 
          ModuleDepend_B
        },
        {
          Te_HeartBeatState_Fine,
          0
        }
    },
    {
        Te_ModuleId_C, 
		"员工C ",
        {
          Te_MainState_All_Idle,
          Te_SubState_StopSucceed,
          Te_MainState_C_Count,
		  {"未收任务","初始化","退出","烧饭","收衣服","准备喊三","喊三","","",""},
          g_MainStateConfig_C,
          g_MainStateType_C
        },
        {
          sizeof(cDependModuleList)/sizeof(Ts_DependModuleConfig),
          cDependModuleList,     
          ModuleDependInitLevel, 
          ModuleDepend_C
        },
        {
          Te_HeartBeatState_Fine,
          0
        }
    }
  }//struct Ts_ModuleInfo list end
};

char* GetModuleName(uint8_t module_id)
{
	return g_SsmInfo.module_info[g_SsmInfo.ModuleIndex[module_id]].module_name;
}

char* GetStateName(uint8_t module_id, uint8_t state_id)
{
	return g_SsmInfo.module_info[g_SsmInfo.ModuleIndex[module_id]].state_manager.state_name[state_id];
}