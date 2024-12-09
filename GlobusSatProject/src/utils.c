#include "utils.h"
#include "hal/errors.h"
#include "stdio.h"
#include <string.h>

int logError(int error ,char* msg)
{
	if(error != E_NO_SS_ERR)
	{
    logData_t data;
    strncpy(data.msg, msg, 20);
    data.error = error;
    // only in testing
    printf("error: %s with value: %d", msg, error);
	}
	return error;
}
