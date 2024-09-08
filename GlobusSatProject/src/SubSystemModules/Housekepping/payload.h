/*
 * IsisTRXVUdemo.h
 *
 *  Created on: 6 feb. 2015
 *      Author: malv
 */

#ifndef ISIS_SOREQ_PAYLOAD_H_
#define ISIS_SOREQ_PAYLOAD_H_

#include <hal/boolean.h>

/***
 * Starts demo.
 * Calls Init and Menu in sequence.
 * Returns FALSE on failure to initialize.
 */

typedef struct {
    unsigned char board;
    unsigned char radfet1;
    unsigned char radfet2;
    unsigned char ormad;
    unsigned char fpga;
} RadiationData;

Boolean MasterRadiationWrite(RadiationData* data);
/***
 * This function reads the voltages for RADFET1 and RADFET2 from the Soreq payload.
 * OPCODE sent to the payload is 0x33.
 * Returns FALSE on failure.
 */

Boolean MasterTemperatureWrite(void);
/***
 * This function sends a command to the Soreq payload to start the radiation experiment.
 * OPCODE sent to the payload is 0x77.
 * Returns FALSE on failure.
 */

Boolean MasterPIC32SELWrite(void);
/***
 * This function sends a command to the Soreq payload to start the radiation experiment.
 * OPCODE sent to the payload is 0x66.
 * Returns FALSE on failure.
 */

Boolean MasterPIC32SEUWrite(void);
/***
 * This function sends a command to the Soreq payload to start the radiation experiment.
 * OPCODE sent to the payload is 0x47.
 * Returns FALSE on failure.
 */

Boolean softReset(void);
/***
 * This function sends a command to the Soreq payload to start the radiation experiment.
 * OPCODE sent to the payload is 0xF8.
 * Returns FALSE on failure.
 */

Boolean clearWatchdog (void);
/***
 * This function sends a command to the Soreq payload to start the radiation experiment.
 * OPCODE sent to the payload is 0x3F.
 * Returns FALSE on failure.
 */

Boolean soreqDebugging(void);
/***
 * This function sends a command to the Soreq payload to start the radiation experiment.
 * OPCODE sent to the payload is 0x32.
 * Returns FALSE on failure.
 */

Boolean soreqPayloadInit(void);

/***
 * Loop producing an interactive
 * text menu for invoking subsystem functions
 * note:
 * Depends on an initialized TRXVU subsystem driver.
 */

Boolean IsisTRXVUdemoMain(void);

/***
 * Initializes the TRXVU subsystem driver.
 * Returns FALSE on failure.
 *
 * note:
 * Depends on an initialized I2C driver.
 * Initialize the I2C interface once before using
 * any of the subsystem library drivers
 */
Boolean IsisTRXVUdemoInit(void);



#endif /* ISIS_SOREQ_PAYLOAD_H_ */
