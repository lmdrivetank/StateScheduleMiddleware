#ifndef _SSM_TYPE_H_
#define _SSM_TYPE_H_

typedef uint8_t ModuleMainState_T;
typedef uint8_t ModuleSubState_T;

typedef enum
{
  Te_ModuleId_System,
  Te_ModuleId_CruiseManager,
  Te_ModuleId_HmiAdapte,
  Te_ModuleId_ADV2HR,
  
  Te_ModuleId_Count
} Te_ModuleId;
typedef enum
{
  Te_HeartBeatState_Fine        = 0xff,
  Te_HeartBeatState_Pause       = 0,
  Te_HeartBeatState_Stop        = 0x11  /*stop for a long time*/
} Te_HeartBeatState;
typedef uint8_t Te_HeartBeatState_u8;  
/**/
typedef enum
{
  Te_MainState_System_Idle,
  Te_MainState_System_Init,
  Te_MainState_System_PreSimple,
  Te_MainState_System_RunSimple,
  Te_MainState_System_PrePCC,
  Te_MainState_System_RunPCC,
  Te_MainState_System_Terminate,
    
  Te_MainState_System_Count
} Te_MainState_System;

typedef enum
{
  Te_MainState_CM_Idle,
  Te_MainState_CM_Init,
  Te_MainState_CM_PreSimple,
  Te_MainState_CM_RunSimple,
  Te_MainState_CM_PrePcc,
  Te_MainState_CM_RunPcc,
  Te_MainState_CM_Terminate,
  
  Te_MainState_CM_Count,
} Te_MainState_CM;

typedef enum
{
  Te_MainState_HmiAdapte_Idle,
  Te_MainState_HmiAdapte_Init,
  Te_MainState_HmiAdapte_Run,
  Te_MainState_HmiAdapte_Terminate,
  
  Te_MainState_HmiAdapte_Count,
} Te_MainState_HmiAdapte;

typedef enum
{
  Te_MainState_ADV2HR_Idle,
  Te_MainState_ADV2HR_Init,
  Te_MainState_ADV2HR_Run,
  Te_MainState_ADV2HR_Terminate,
  
  Te_MainState_ADV2HR_Count
} Te_MainState_ADV2HR;

typedef enum
{
  Te_MainStateType_Idle,
  Te_MainStateType_RunOnce,
  Te_MainStateType_RunCycle,
  
  Te_MainStateType_Count
} Te_MainStateType;
typedef uint8_t Te_MainStateType_u8;

typedef struct
{
  ModuleMainState_T                             module_self_state;
  ModuleMainState_T                             depend_state;
} Ts_StateDependPair;
typedef struct
{
  const uint8_t                                 depend_module_id;
  struct Ts_StateDependPair const*const         plist_pair;
  const uint8_t                                 pair_count;
} Ts_DependModuleConfig;
typedef struct
{
  const uint8_t                                 MainState_id;
  const Te_MainStateType_u8                     state_type;
} Ts_MainStateConfig;
typedef struct
{
  /* datedomain store them
  uint8_t                                       main_state;
  uint8_t                                       sub_state;
  ************************/
  const uint8_t                                 state_count;  
  struct Ts_MainStateConfig const*const         plist_stateConfig;
  
  Te_MainStateType_u8*const                     plist_MainStateType;            /*state type = buff[state id]*/
} Ts_StateManager;
typedef struct 
{ 
  const uint8_t                                 depend_module_count;
  struct Ts_DependModuleConfig const*const      plist_dependModule;
  
  uint8_t                                       module_depend_level;
  ModuleMainState_T*const*const                 pplist_ModuleStateDepend;       /*depend module state = buff[self state id][depend module id: Te_ModuleId_Count]*//*shall malloc*/
} Ts_ModuleDependManager;
typedef struct
{
  /*  Te_HeartBeatState_Fine        = 0xff,
      Te_HeartBeatState_Pause       = 0,
      Te_HeartBeatState_Stop        = 0x11  //stop for a long time*/
  Te_HeartBeatState_u8                          state;
  
  uint16_t                                      heartbeat_pauseCount;    
} Ts_HeartBeatManager;
typedef struct
{
  const uint8_t                                 module_id;  
  
  struct Ts_StateManager                        state_manager;
  struct Ts_ModuleDependManager                 moduledepend_manager;
  struct Ts_HeartBeatManager                    heartbeat_manager;
} Ts_ModuleInfo;
typedef struct
{
  /*config region*/
  struct Ts_ModuleInfo                          module_info[Te_ModuleId_Count]; 
  /*rte*/
  uint8_t                                       ModuleIndex[Te_ModuleId_Count];                 /*return index for buff module_info*/
  uint8_t                                       ModuleId_DpendLevelRaise[Te_ModuleId_Count];    /*return module id*/
} Ts_SsmInfo;

#endif