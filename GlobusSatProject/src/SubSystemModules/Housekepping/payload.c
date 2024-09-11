
#include <hal/Drivers/I2C.h>
#include <hal/boolean.h>
#include <hal/errors.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <hal/Timing/Time.h>
#include "hal/Drivers/I2C.h"
#include <hal/Utility/util.h>
#include "payload.h"
#define SOREQ_I2C_ADDRESS 0x55
#define READ_PIC32_RESETS 0x66
#define READ_Radiation 0x33
#define READ_DELAY 200
#define TIMEOUT 4000
#define READ_PIC32_RESETS 0x66
#define PAYLOAD_I2C_ADDRESS 0x55
#define SOREQ_DEBUGGING 0x32
#define OPCODE_MASTER_READ_FROM_PAYLOAD 0x45 // in tau-1



typedef enum {
    PAYLOAD_SUCCESS, PAYLOAD_I2C_Write_Error, PAYLOAD_I2C_Read_Error, PAYLOAD_TIMEOUT
} SoreqResult;

typedef enum
{
    board, radfet, ormad, fpga
} PayloadSubSystem;

int changeIntIndian(int num)
{
    return ((num>>24)&0xff) | // move byte 3 to byte 0
                        ((num<<8)&0xff0000) | // move byte 1 to byte 2
                        ((num>>8)&0xff00) | // move byte 2 to byte 1
                        ((num<<24)&0xff000000); // byte 0 to byte 3
}

SoreqResult masterRead(int size,unsigned char* buffer)
{
    unsigned char *opcode_read = OPCODE_MASTER_READ_FROM_PAYLOAD;
    int err=0;
    int i=0;

    do
    {
        err = I2C_write(PAYLOAD_I2C_ADDRESS,opcode_read,1);
        if(err!=0)
        {
            return PAYLOAD_I2C_Write_Error;
        }
        vTaskDelay(READ_DELAY);
        err = I2C_read(SOREQ_I2C_ADDRESS,buffer,size);
        if(err!=0)
        {
            return PAYLOAD_I2C_Read_Error;
        }
        if(buffer[3] == 0)
        {
            return PAYLOAD_SUCCESS;
        }
        vTaskDelay(READ_DELAY);
    }while(i++<TIMEOUT);
    return PAYLOAD_TIMEOUT;
}

SoreqResult payloadSendCommand(char opcode, int size, unsigned char* buffer,int delay)
{
	vTaskDelay(50);
	int err = I2C_write(PAYLOAD_I2C_ADDRESS, &opcode, 1);
	if(err!=0)
	{
		return PAYLOAD_I2C_Write_Error;
	}
	if(delay>0)
	{
		vTaskDelay(delay);
	}
	SoreqResult res =  masterRead(size,buffer);

	return res;
}



Boolean MasterRadiationWrite(RadiationData* rad)
{
    unsigned char buffer[12];
    SoreqResult res = payloadSendCommand(READ_Radiation, 12, buffer, 0);
    if (res != PAYLOAD_SUCCESS)
    {
        return FALSE;
    }
    rad->STATUS = (0x0000FF0000000000000000 & buffer) >> 40;
    if (rad->STATUS != 0)
    {
        printf("Radiation data is not ready\n");
        return FALSE;
    }
    rad->OPCODE = (0xFFFF00000000000000000000 & buffer) >> 48;
    rad->RADFET1 = (0x000000FFFFFFFF0000000000 & buffer) >> 24;
    rad->RADFET2 = (0x0000000000000000FFFFFFFF00 & buffer) >> 8;
    return TRUE;
}

Boolean MasterTemperatureWrite(char buffer[12])
{
    char buffer[12];

    SoreqResult res = payloadSendCommand(READ_Radiation, 12, buffer, 0);
    if (res != PAYLOAD_SUCCESS)
    {
        return FALSE;
    }
    float temperature = *(float*)buffer; // Cast buffer value to float
    printf("Temperature: %f\n", temperature); // Print temperature as float
    return TRUE;
}

Boolean soreqDebugging(char buffer[12])
{
    SoreqResult res = payloadSendCommand(SOREQ_DEBUGGING, 12, buffer, 0);
    if (res != PAYLOAD_SUCCESS)
    {
        return FALSE;
    }
    Boolean first_check = (*buffer >> 32) & 0xFFFF == 0x3800;
    Boolean second_check = (*buffer >> 16) & 0xFFFF == 0x3200;
    Boolean third_check = (*buffer) & 0xFFFF == 0x3500;
    if(first_check && second_check && third_check)
    {
        return TRUE;
    }
    return FALSE;
}
