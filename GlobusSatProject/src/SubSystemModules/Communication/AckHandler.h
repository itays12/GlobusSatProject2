
#ifndef ACKHANDLER_H_
#define ACKHANDLER_H_
#include <hal/errors.h>
#include "SatCommandHandler.h"
#include "SPL.h"


#define SendAnonymosAck(acksubtype) SendAckPacket(acksubtype,NULL,NULL,0) //<! sends an ACK without command ID nor data


/*!
 * @brief transmits an ack packet as an SPL packet(using the SPL protocol)
 * @param[in] acksubtype type of ack to be sent according to ack_subtype_t enumeration
 * @param[in] cmd the command for which the ACK is a response to. CAN BE NULL COMMAND FOR SYSTEM ACK!
 * @note the ACK is sent with the corresponding ID of the ACK inside the SPL packet
 * @return errors according t <hal/errors.h>
 */
int SendAckPacket(ack_subtype_t acksubtype, unsigned int cmd_id, void *data, unsigned short length);


#endif /* ACKHANDLER_H_ */
