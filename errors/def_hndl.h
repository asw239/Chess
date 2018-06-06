#ifndef DEF_HNDL_H
#define DEF_HDNL_H

#include "ErrorCode.h"
#include <stdbool.h>

extern bool errors_terminate_on_def_err;

void errors_def_hndl(enum ErrorCode err, const char *msg);

#endif
