#ifndef EPSOPERATIONMODES_H_
#define EPSOPERATIONMODES_H_


#include "GlobalStandards.h"


#define CHANNELS_OFF 0x00 	//!< channel state when all systems are off
#define CHNNELS_ON	 0XFF	//!< channel

#define SYSTEM_MCU         0	//!< channel state when MCU is on
#define SYSTEM_PAYLOAD		 2	//!< channel state when PAYLOAD is on


typedef enum EpsState{
	FullMode,
	SafeMode,
	CriticalMode,
}EpsState;

/*!
 * @brief Executes the necessary procedure in order to initiate the system into Full mode
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int EnterFullMode();

/*!
 * @brief Executes the necessary procedure in order to initiate the system into Cruise mode
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int EnterCruiseMode();

/*!
 * @brief Executes the necessary procedure in order to initiate the system into Safe mode
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int EnterSafeMode();

/*!
 * @brief Executes the necessary procedure in order to initiate the system into Critical mode
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int EnterCriticalMode();

/*!
 * @brief Sets the channel state according to the bitwise 'logic on'
 * if the 2'nd bit is '1' the second channel will turn on (channel = 0b01000000)
 * @note please use the defines defined in this header to turn on/off channels
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int turnOnEpsChannel(int channel);

/*!
 * @brief Gets the channel state according to the bitwise 'logic on'
 * if the 2'nd bit is '1' the second channel will turn on (channel = 0b01000000)
 * @note please use the defines defined in this header to turn on/off channels
 * @return	channel_t
 */
int turnOffEpsChannel(int channel);

/*!
 * returns the current system state according to EpsState
 */
EpsState GetSystemState();



#endif /* EPSOPERATIONMODES_H_ */
