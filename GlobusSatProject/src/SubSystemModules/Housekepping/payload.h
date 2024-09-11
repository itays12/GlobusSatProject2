#define SOREQ-I2C_ADDRESS 0x55
#define READ_PIC32_RESET 0x66
#define READ_DELAY 200
#define TIMEOUT 4000
#define READ_PIC32_RESETS 0x66
#define PAYLOAD_I2C_ADDRESS
#define CLEAR_WDT_AND_ALLOW_READ 0x45
typedef enum {
	PAYLOAD_SUCCESS, PAYLOAD_I2C_WRITE_Error, PAYLOAD_I2C_READ_Error, PAYLOAD_TIMEOUT
}SoreqResult;
typedef enum
{
	board, radfet,ormad,fpga
}PayloadSubSystem;

SoreqResult payloadSendCommand(char opcode,int size,unsigned char* buffer,int delay);
SoreqResult payloadRead(int size, unsigned char* buffer);
SoreqResult payloadPICread();
