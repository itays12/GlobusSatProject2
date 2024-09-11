#include <payload.h>

SoreqResult payloadSendCommand(char opcode,int size,unsigned char* buffer,int delay)
{
	vTaskDelay(50);
	int err = I2C_write(PAYLOAD_I2C_ADDRESS, &opcode,1);
	if(err!=0){
		return PAYLOAD_I2C_Write_Error;
	}
	if(delay>0){
		vTaskDelay(delay);
	}
	SoreqResult res = payloadRead(size,buffer);

	return res;
}
SoreqResult payloadRead(int size, unsigned char* buffer){
	unsigned char wtd_and_read[] = {CLEAR_WDT_AND_ALLOW_READ};
	int err=0;

	int i=0;
	do
	{
		err = I2C_write(PAYLOAD_I2C_ADDRESS,WTD_AND_READ,1);
		if(err!=0){
			return PAYLOAD_I2C_WROTE_Error;
		}
		vTaskDelay(READ_DELAY);
		err = I2C_read(SOREQ_I2C_ADDRESS,buffer,size);
		if(err!=0)
		{
			return PAYLOAD_I2C_Read_Error;
		}
		if(vuffer[3] == 0){
			return PAYLOAD_SUCCESS;
		}
	}
	while(TIMEOUT/READ_DELAY>i++){
		return PAYLOAD_TIMEOUT;
	}
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


