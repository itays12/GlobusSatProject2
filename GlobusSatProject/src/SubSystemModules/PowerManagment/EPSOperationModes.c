#include "EPSOperationModes.h"
#include "SubSystemModules/Communication/TRXVU.h"

/*!
 * @brief Executes the necessary procedure in order to initiate the system into Full mode
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int EnterFullMode(){

    UnMuteTRXVU(); // unmute TRXVU if its muted

    EpsState = 1;
    return 0;
}

/*!
 * @brief Executes the necessary procedure in order to initiate the system into Cruise mode
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int EnterCruiseMode(){
    muteTRXVU(600); // mute for 10 min

    EpsState = 2;
	return 0;
}

/*!
 * @brief Executes the necessary procedure in order to initiate the system into Safe mode
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int EnterSafeMode(){
    muteTRXVU(600); // mute for 10 min

    EpsState = 3;
    return 0;
}

/*!
 * @brief Executes the necessary procedure in order to initiate the system into Critical mode
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int EnterCriticalMode(){
    muteTRXVU(600); // mute for 10 min

    EpsState = 4;
    return 0;
}

int GetSystemState(){
    return EpsState;
}

int SetEPS_Channels(channel_t channel);

channel_t GetSystemChannelState();

Boolean EpsGetLowVoltageFlag();

void EpsSetLowVoltageFlag(Boolean low_volt_flag);
