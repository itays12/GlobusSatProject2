#include <hal/boolean.h>
#include "utils.h"
#include "hal/Timing/Time.h"

//****Approved by Uri****
Boolean CheckExecutionTime(time_unix prev_time, time_unix period){
	unsigned int cur_time = 0;
	int err = Time_getUnixEpoch(&cur_time);

	logError(err,"error in CheckExecutionTime Time_g");

	if(cur_time-prev_time>=period)
		return TRUE;
	else return FALSE;
}
