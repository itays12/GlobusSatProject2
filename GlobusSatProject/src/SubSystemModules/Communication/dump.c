#include "SubSystemModules/Communication/TRXVU.h"
#include "SubSystemModules/Housekepping/TelemetryCollector.h"
#include "TLM_management.h"
#include "dump.h"
#include <hcc/api_fat.h>
dump_arguments_t args;
xTaskHandle dumpTaskHandle = NULL;


void dumpTask() {
	f_enterFS();
  logFsErr(readTLMFileTimeRange(args.dump_type, args.t_start,args.t_end, args.id,getTlmDataSize(args.dump_type)), "DumpFail");
  dumpTaskHandle = NULL;
  vTaskDelete(NULL);
}

void startDump(dump_arguments_t* dump_args) { 
  if (dumpTaskHandle != NULL){
    return;
  }
  args = *dump_args;
  xTaskGenericCreate(dumpTask, (const signed char *)"dumpTask", 4096, NULL,
                     configMAX_PRIORITIES - 3, &dumpTaskHandle, NULL, NULL);

}

void abortDump(){
  setAbortFlag();
}

void forceAbortDump(){
  if (dumpTaskHandle == NULL) {
    return; 
  }

  vTaskDelete(dumpTaskHandle);
}
