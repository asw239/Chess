#include <string.h>
#include "error_utilities.h"

#define CALL_TRACE_SIZE 512

void call_error(ErrFncPtr err_fnc_arr[], enum ErrorCode err,
	const char *file_name, const char *func_name)
{
	char call_trace[CALL_TRACE_SIZE] = {};
	strcat(call_trace, "In file ");
	strcat(call_trace, file_name);
	strcat(call_trace, ", ");
	strcat(call_trace, func_name);

	if(!err_fnc_arr[err])
		err_fnc_arr[GLOBAL_ERROR](err, call_trace);
	else
		err_fnc_arr[err](err, call_trace);
}
