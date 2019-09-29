#ifndef THREAD_WORK_H
#define THREAD_WORK_H

#include "ssm_type.h"

#define         TaskStartPublicDelay                    1000*1000   //us
#define         Task_Count                              (Te_ModuleId_Count + 1)
#define         UnValidTaskId                           0xff
#define         ThreadTimeUnit                          1/1000 //unit: ms
typedef         void                                    (*TaskRunnable_t)(Te_MoudleId_u8 module_id, Ts_threadStateMachineBranch *psmBranch);
typedef struct
{
  uint8_t                       task_id;        //equal timer_id & module_id
  HANDLE						task_handle;
  TaskRunnable_t                pxTaskRunnable;
  uint16_t						cycle_ms;
  Ts_threadStateMachineBranch*  psmBranch;
} Ts_TaskContext;

//void module_thread_round(Ts_TaskContext* pTaskContext) ;
void module_thread_create(	uint8_t                         task_id,
                            TaskRunnable_t                  pxTaskCode,     //if NULL, will be set module_thread_runnable; else user self define (only ssm except)
							uint16_t						cycle_ms,
                        	const uint16_t                  usStackDepth,
                            Ts_threadStateMachineBranch*    psmBranch);
void module_thread_release( uint8_t task_id );
//void module_thread_runnable(Te_MoudleId_u8 module_id, Ts_threadStateMachineBranch *psmBranch);
void module_thread_init();

#endif