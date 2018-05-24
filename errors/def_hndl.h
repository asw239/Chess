#ifndef DEF_HNDL_H
#define DEF_HDNL_H

#include "ErrorCode.h"
#include <stdbool.h>

extern bool terminate_on_def_err;

void def_hndl(enum ErrorCode err, const char *msg);

#endif
