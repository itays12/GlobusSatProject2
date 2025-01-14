#include "utils.h"
#include "hal/errors.h"
#include "stdio.h"
#include <string.h>
#include <SubSystemModules/Housekepping/TelemetryFiles.h>
#include "TLM_management.h"

int logError(int error, char* msg) {
	if (error != E_NO_SS_ERR) {
		printf("error: %s with code: %d\n", msg, error);
		logData_t data;
		strncpy(data.msg, msg, 19);
		data.error = error;

		writeToFile(&data, sizeof(data), tlm_log);
	}
	return error;
}
