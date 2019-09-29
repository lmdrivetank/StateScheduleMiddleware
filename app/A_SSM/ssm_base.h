#ifndef SSM_BASE_H
#define SSM_BASE_H
#include "ssm_type.h"
#include "app_print.h"

void Get_UserFunctionInput(uint8_t input);


#define SSM_LOG(log_level, format, ...)                                         \
{                                                                               \
  LOG_PRINTF(log_level, "module ssm: ", format, ##__VA_ARGS__);                  \
}

#endif