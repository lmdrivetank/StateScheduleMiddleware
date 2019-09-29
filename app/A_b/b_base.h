#ifndef B_BASE_H
#define B_BASE_H

#include "app_type.h"
#include "app_print.h"
#include "ssm.h"

#define B_LOG(log_level, format, ...)                                           \
{                                                                               \
  LOG_PRINTF(log_level, "employee b action: ", format, ##__VA_ARGS__);                  \
}

#endif