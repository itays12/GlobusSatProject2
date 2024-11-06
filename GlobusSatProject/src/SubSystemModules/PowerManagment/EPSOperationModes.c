#include "EPSOperationModes.h"
#include "SubSystemModules/Communication/TRXVU.h"
#include <satellite-subsystems/GomEPS.h>

EpsState epsState;


/*!
 * @brief Executes the necessary procedure in order to initiate the system into Full mode
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int EnterFullMode(){

	if (epsState != FullMode){
		unmuteTRXVU(); // unmute TRXVU if its muted
		SetEPS_Channels(CHNNELS_ON);
	}
    epsState = FullMode;
    return 0;
}


/*!
 * @brief Executes the necessary procedure in order to initiate the system into Safe mode
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int EnterSafeMode(){
	if (epsState != SafeMode){
		unmuteTRXVU();
		SetEPS_Channels(CHNNELS_ON & ~SYSTEM_PAYLOAD);
	}
    epsState = SafeMode;
    return 0;
}

/*!
 * @brief Executes the necessary procedure in order to initiate the system into Critical mode
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int EnterCriticalMode(){
	if (epsState != CriticalMode){
		muteTRXVU();
		SetEPS_Channels(CHNNELS_ON & ~SYSTEM_PAYLOAD);
	}

	epsState = CriticalMode;
    return 0;
}

EpsState GetSystemState(){
    return epsState;
}

int SetEPS_Channels(channel_t channel){
	gom_eps_channelstates_t channelStates;
	channelStates.raw = channel;
	return logError(GomEpsSetOutput(EPS_I2C_BUS_INDEX, channelStates), "setEpsChannels");
}

channel_t GetEPS_Channels(){
	gom_eps_hkparam_t channelState;
	logError(GomEpsGetHkData_param(EPS_I2C_BUS_INDEX, &channelState), "GetEPS_Channels");
	return channelState.fields.channelStatus.raw;
}
