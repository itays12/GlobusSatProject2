#include "utils.h"
#include "hal/errors.h"
#include "stdio.h"

int logError(int error ,char* msg)
{
	if(error != E_NO_SS_ERR)
	{
	    printf("ERROR %d",error);

	switch (error)
	{
		case E_NOT_INITIALIZED:
			printf("E_NOT_INITIALIZED: Attempt to use something that is not yet initialized.");
			break;
		case E_IS_INITIALIZED:
			printf("E_IS_INITIALIZED: Attempt to initialize something that has already been initialized.");
			break;
		case E_GET_SEMAPHORE_FAILED:
			printf("E_GET_SEMAPHORE_FAILED: Failed to obtain semaphore for communication.");
			break;
		case E_INDEX_ERROR:
			printf("E_INDEX_ERROR: Incorrect index specified.");
			break;
		case E_BITRATE_INPUT_ERROR:
			printf("E_BITRATE_INPUT_ERROR: Failed to set the bitrate of the TRXUV.");
			break;
		case E_CWCHAR_INPUT_ERROR:
			printf("E_CWCHAR_INPUT_ERROR: Failed to set the cwcharrate of the TRXUV.");
			break;
		case  E_IDLE_STATE_ERROR:
			printf("E_IDLE_STATE_ERROR: Failed to go into idle mode of the TRXUV.");
			break;
		case  E_OUTPUT_MODE_ERROR:
			printf("E_OUTPUT_MODE_ERROR: Failed to go into outmode of the TRXUV.");
			break;
		case  E_TRXUV_COMPONENT_ERROR:
			printf("E_TRXUV_COMPONENT_ERROR: Failed to choose a device in the TRXUV");
			break;
		case  E_WRONG_ADC_CHANNEL:
			printf("E_WRONG_ADC_CHANNEL: Failed to choose a correct ADC channel TRXUV.");
			break;
		case  E_RESET_SYSTEM:
			printf("E_RESET_SYSTEM: Failed to reset both microcontrollers in the TRXUV.");
			break;
		case  E_MEM_ALLOC:
			printf("E_MEM_ALLOC: Failed to allocate memory.");
			break;
		case   E_ATT_ERROR:
			printf("E_ATT_ERROR: Failed to set attenuation value in the TXS.");
			break;
		case  E_PARAM_OUTOFBOUNDS:
			printf("E_PARAM_OUTOFBOUNDS: A parameter is out of bounds.");
			break;
		case  E_TRXUV_FRAME_LENGTH:
			printf("E_TRXUV_FRAME_LENGTH: Input frame length was greater than the maximum frame length of TRXUV.");
			break;
		case  E_INPUT_POINTER_NULL:
			printf("E_INPUT_POINTER_NULL: A pointer input to the function was NULL.");
			break;
		case  E_COMMAND_NACKED:
			printf("E_COMMAND_NACKED: A command sent did not get acknowledged.");
			break;
		case  E_COMMS_ERROR:
			printf("E_COMMS_ERROR: Communication with component failed.");
			break;
		case  E_COMPARISON_ERROR:
			printf("E_COMPARISON_ERROR: Input data and output data were not equal.");
			break;
		default:
			break;
	};
	printf("%s\n\r",  msg);
	}
	return error;
}
