#include "utils.h"
#include "hal/errors.h"
#include "stdio.h"
#include <string.h>

int logError(int error ,char* msg)
{
	if(error != E_NO_SS_ERR)
	{
	    printf("error: %s with value: %d", msg, error);
    logData_t data;
    strncpy(data.msg, msg, 19);
    data.error = error;
    // only in testing
	}
	return error;
}
