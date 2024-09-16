#include <payload.h>

SoreqResult payloadSendCommand(char opcode,int size,unsigned char* buffer,int delay)
{
	vTaskDelay(50);
	int err = I2C_write(PAYLOAD_I2C_ADDRESS, &opcode,1);
	if(err!=0){
		return PAYLOAD_I2C_WRITE_Error;
	}
	if(delay>0){
		vTaskDelay(delay);
	}
	SoreqResult res = payloadRead(size,buffer);

	return res;
}


SoreqResult payloadPICread(){
	char buffer[12];
	int* count = 0;

	SoreqResult res = payloadsendcommand(READ_PIC32_RESET,12,buffer,0);
	memcpy(count,buffer+4,4);
	if(*count==0){
		memcpy(count,buffer+8,4);
	}
	*count = changeIntIndian(*count);
	printf("\r\n count number %d \r\n", *count);
}


