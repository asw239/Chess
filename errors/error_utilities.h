#ifndef ERROR_UTITLITIES_H
#define ERROR_UTITLITIES_H

#include "ErrorCode.h"

typedef void (*ErrFncPtr)(enum ErrorCode err, const char *msg);

void call_error(ErrFncPtr err_fnc_arr[], enum ErrorCode err,
		const char *file_name, const char *func_name);

#endif
