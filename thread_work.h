#ifndef THREAD_WORK_H
#define THREAD_WORK_H

typedef enum 
{
  Te_workConditionResult_OK,
  Te_workConditionResult_Not
}Te_workConditionResult;

typedef enum 
{
  Te_workPerformanceResult_Succ,
  Te_workPerformanceResult_Fail
}Te_workPerformanceResult;

/*if exist state machine, different state have sepcial condition-performance list table*/
typedef struct Ts_threadStateMachineBranch
{
  uint8_t state;
  struct Ts_threadWorkSequ *workSequ;
};
typedef struct Ts_threadWorkSequ
{
  int (*workCondition)();  /*check work condition, if null mean do this task, if condition function fail mean work sequence kreak and return*/
  int (*workPerformance)();
};
typedef struct Ts_threadWorkRing
{
};

typedef enum
{
  Te_SubState_Prepare,                  //run_once should wait depend module stop succeed of run once before ready
  Te_SubState_Ready,                    //everything pre-condition is ok, next is run
  Te_SubState_Run,                      
  Te_SubState_Hold,
  Te_SubState_StopSucceed,
  Te_SubState_StopFailed,
  Te_SubState_HeartBeatError,
} Te_GeneralSubState;
#endif