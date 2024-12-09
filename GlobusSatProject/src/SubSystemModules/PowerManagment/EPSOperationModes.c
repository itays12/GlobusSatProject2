#include "EPSOperationModes.h"
#include "GlobalStandards.h"
#include "SubSystemModules/Communication/TRXVU.h"
#include "utils.h"
#include <satellite-subsystems/imepsv2_piu.h>

EpsState epsState;

/*!
 * @brief Executes the necessary procedure in order to initiate the system into
 * Full mode
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int EnterFullMode() {

  if (epsState != FullMode) {
    unmuteTRXVU(); // unmute TRXVU if its muted
    turnOnEpsChannel(SYSTEM_PAYLOAD);
  }
  epsState = FullMode;
  return 0;
}

/*!
 * @brief Executes the necessary procedure in order to initiate the system into
 * Safe mode
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int EnterSafeMode() {
  if (epsState != SafeMode) {
    unmuteTRXVU();
    turnOffEpsChannel(SYSTEM_PAYLOAD);
  }
  epsState = SafeMode;
  return 0;
}

/*!
 * @brief Executes the necessary procedure in order to initiate the system into
 * Critical mode
 * @return	0 on success
 * 			errors according to <hal/errors.h>
 */
int EnterCriticalMode() {
  if (epsState != CriticalMode) {
    muteTRXVU();
    turnOffEpsChannel(SYSTEM_PAYLOAD);
  }

  epsState = CriticalMode;
  return 0;
}

EpsState GetSystemState() { return epsState; }

static channel_t channelStates;

int turnOnEpsChannel(int channel) {
  imepsv2_piu__replyheader_t header;
  imepsv2_piu__outputbuschanneloff(EPS_I2C_ADDR, channel, &header);
}

int turnOffEpsChannel(int channel) {
  imepsv2_piu__replyheader_t header;
  imepsv2_piu__outputbuschannelon(EPS_I2C_ADDR, channel, &header);
}
