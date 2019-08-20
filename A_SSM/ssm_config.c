#ifndef SSM_CONFIG_H
#define SSM_CONFIG_H

/******************************************************/
const struct Ts_MainStateConfig g_MainStateConfig_CM[] = 
{
  {Te_MainState_CM_Idle,                Te_MainStateType_Idle},
  {Te_MainState_CM_Init,                Te_MainStateType_RunOnce},
  {Te_MainState_CM_PreSimple,           Te_MainStateType_RunOnce},
  {Te_MainState_CM_RunSimple,           Te_MainStateType_RunCycle},
  {Te_MainState_CM_PrePcc,              Te_MainStateType_RunOnce},
  {Te_MainState_CM_RunPcc,              Te_MainStateType_RunCycle},
  {Te_MainState_CM_Terminate,           Te_MainStateType_RunOnce},
  {Te_MainState_All_End,                Te_MainStateType_End}
};
const struct Ts_MainStateConfig g_MainStateConfig_HmiAdapte[] = 
{
  {Te_MainState_HmiAdapte_Idle,         Te_MainStateType_Idle},
  {Te_MainState_HmiAdapte_Init,         Te_MainStateType_RunOnce},
  {Te_MainState_HmiAdapte_Run,          Te_MainStateType_RunCycle},
  {Te_MainState_HmiAdapte_Terminate,    Te_MainStateType_RunOnce},
  {Te_MainState_All_End,                Te_MainStateType_End}
};
const struct Ts_MainStateConfig g_MainStateConfig_ADV2HR[] = 
{
  {Te_MainState_ADV2HR_Idle,            Te_MainStateType_Idle},
  {Te_MainState_ADV2HR_Init,            Te_MainStateType_RunOnce},
  {Te_MainState_ADV2HR_Run,             Te_MainStateType_RunCycle},
  {Te_MainState_ADV2HR_Terminate,       Te_MainStateType_RunOnce},
  {Te_MainState_All_End,                Te_MainStateType_End}
};
const struct Ts_MainStateConfig g_MainStateConfig_System[] = 
{       
  {Te_MainState_System_Idle,          	Te_MainStateType_Idle},
  {Te_MainState_System_Init,          	Te_MainStateType_RunOnce},
  {Te_MainState_System_PreSimple,    	Te_MainStateType_RunOnce},
  {Te_MainState_System_RunSimple,      	Te_MainStateType_RunCycle},
  {Te_MainState_System_PrePCC,      	Te_MainStateType_RunOnce},
  {Te_MainState_System_RunPCC,        	Te_MainStateType_RunCycle},
  {Te_MainState_System_Terminate,    	Te_MainStateType_RunOnce},
  {Te_MainState_All_End,                Te_MainStateType_End}
};
/******************************************************/
const struct Ts_StateDependPair SystemDependCruiseManager[] = 
{
  {Te_MainState_System_Idle,            Te_MainState_CM_Idle},
  {Te_MainState_System_Init,            Te_MainState_CM_Init},
  {Te_MainState_System_PreSimple,       Te_MainState_CM_PreSimple},
  {Te_MainState_System_RunSimple,       Te_MainState_CM_RunSimple},
  {Te_MainState_System_PrePCC,          Te_MainState_CM_PrePcc},
  {Te_MainState_System_RunPCC,          Te_MainState_CM_RunPcc},
  {Te_MainState_System_Terminate,       Te_MainState_CM_Terminate},
  {Te_MainState_All_End,                Te_MainState_All_End}
};
const struct Ts_StateDependPair SystemDependHmiAdapte[] = 
{
  {Te_MainState_System_Idle,            Te_MainState_HmiAdapte_Idle},
  {Te_MainState_System_Init,            Te_MainState_HmiAdapte_Init},
  {Te_MainState_System_PreSimple,       Te_MainState_HmiAdapte_Run},
  {Te_MainState_System_RunSimple,       Te_MainState_HmiAdapte_Run},
  {Te_MainState_System_PrePCC,          Te_MainState_HmiAdapte_Run},
  {Te_MainState_System_RunPCC,          Te_MainState_HmiAdapte_Run},
  {Te_MainState_System_Terminate,       Te_MainState_HmiAdapte_Terminate},
  {Te_MainState_All_End,                Te_MainState_All_End}
};
const struct Ts_StateDependPair SystemDependADV2HR[] = 
{
  {Te_MainState_System_Idle,            Te_MainState_ADV2HR_Idle},
  {Te_MainState_System_Init,            Te_MainState_ADV2HR_Init},
  {Te_MainState_System_RunPCC,          Te_MainState_ADV2HR_Run},
  {Te_MainState_System_Terminate,       Te_MainState_ADV2HR_Terminate},
  {Te_MainState_All_End,                Te_MainState_All_End}
};
const struct Ts_ModuleInfo SystemDependModuleList[] = 
{
  {
      Te_ModuleId_CruiseManager, 
      SystemDependCruiseManager, 
      sizeof(SystemDependCruiseManager)/sizeof(struct Ts_StateDependPair)
  },
  {
      Te_ModuleId_ADV2HR, 
      SystemDependADV2HR, 
      sizeof(SystemDependADV2HR)/sizeof(struct Ts_StateDependPair)
  },
  {     
      Te_ModuleId_HmiAdapte, 
      SystemDependHmiAdapte, 
      sizeof(SystemDependHmiAdapte)/sizeof(struct Ts_StateDependPair)
  },
  {     
      Te_ModuleId_End, 
      NULL, 
      0
  }
};
/******************************************************/
struct Ts_SsmInfo g_SsmInfo = 
{
  {//struct Ts_ModuleInfo list
    {//struct Ts_ModuleInfo
        Te_ModuleId_System, //module_id
        {//struct Ts_StateManager
          Te_MainState_System_Count,//state_count
          g_MainStateConfig_System,//plist_stateConfig
          g_MainStateType_System//pMainStateType
        },
        {//struct Ts_ModuleDependManager
          sizeof(SystemDependModuleList)/sizeof(struct Ts_DependModuleConfig),
          SystemDependModuleList, 
          ModuleDependInitLevel,          
          ModuleDepend_System             /* param fisrt: self state index; second: depend module id; result: depend module state */
        },
        {//struct Ts_HeartBeatManager
          Te_HeartBeatState_OK,
          {0,0}
        }
    },
    {
        Te_ModuleId_CruiseManager,
        {
          Te_MainState_CM_Count,
          g_MainStateConfig_CM,
          g_MainStateType_CM
        },
        {
          0,
          NULL,
          ModuleDependInitLevel,
          NULL
        },
        {//struct Ts_HeartBeatManager
          Te_HeartBeatState_OK,
          {0,0}
        }
    },
    {
        Te_ModuleId_HmiAdapte, 
        {
          Te_MainState_HmiAdapte_Count,
          g_MainStateConfig_HmiAdapte,
          g_MainStateType_HmiAdapte
        },
        {
          0,
          NULL,     
          ModuleDependInitLevel, 
          NULL
        },
        {//struct Ts_HeartBeatManager
          Te_HeartBeatState_OK,
          {0,0}
        }
    },
    {
        Te_ModuleId_ADV2HR, 
        {
          Te_MainState_ADV2HR_Count,
          g_MainStateConfig_ADV2HR,
          g_MainStateType_ADV2HR
        },
        {
          0,
          NULL,    
          ModuleDependInitLevel,           
          NULL
        },
        {//struct Ts_HeartBeatManager
          Te_HeartBeatState_OK,
          {0,0}
        }
    }
  }//struct Ts_ModuleInfo list end
};

#endif