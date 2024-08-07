#include "EPSOperationModes.h"
#include "SubSystemModules/Communication/TRXVU.h"



int CheckTrxvuUnmute(){
	if (is_idle && CheckExecutionTime(last_idle, mute_time)){
		SetIdleState(ISIStrvu )
	}
	return 0;
}

/*!
 * @brief Executes the necessary procedure in order to initiate the system into Full mode
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int EnterFullMode(){
	return 0;
}

/*!
 * @brief Executes the necessary procedure in order to initiate the system into Cruise mode
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int EnterCruiseMode(){
	return 0;

}

/*!
 * @brief Executes the necessary procedure in order to initiate the system into Safe mode
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int EnterSafeMode(){
	return 0;

}

/*!
 * @brief Executes the necessary procedure in order to initiate the system into Critical mode
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int EnterCriticalMode(){
	return 0;
}
