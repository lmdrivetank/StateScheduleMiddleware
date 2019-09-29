#ifndef A_BASE_H_
#define A_BASE_H_

#include "app_type.h"
#include "app_print.h"
#include "ssm.h"

#define A_LOG(log_level, format, ...)                                           \
{                                                                               \
  LOG_PRINTF(log_level, "employee a action: ", format, ##__VA_ARGS__);                  \
}

#endif