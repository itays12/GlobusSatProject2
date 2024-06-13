#include "utils.h"
#include "hal/errors.h"
#include "stdio.h"

int logError(int error ,char* msg)
{
	if(error != E_NO_SS_ERR)
	{
	    printf("ERROR %d encountered with message %s\n\r", error, msg);
	}
	return error;
}
