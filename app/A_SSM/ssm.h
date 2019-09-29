#ifndef SSM_H
#define SSM_H
#include "ssm_type.h"
#include "ssm_config.h"
#include "ssm_sm.h"
#include  "ssm_rte.h"
int InitProcess_SSM();
int TerminateProcess_SSM();
int ModuleStateCheckAll_SSM();
int ReportModuleError_SSM(uint8_t moduleId, uint8_t errorId);

#endif