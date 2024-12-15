#include "SubSystemModules/Communication/TRXVU.h"
#include "SubSystemModules/Housekepping/TelemetryCollector.h"
#include "TLM_management.h"
#include "dump.h"
#include "stdatomic.h"
dump_arguments_t args;
xTaskHandle dumpTaskHandle = NULL;
volatile atomic_int taskRunning = 0;

void dumpTask() {
  readTLMFileTimeRange(args.dump_type, args.t_start,args.t_end, args.id,getTlmDataSize(args.dump_type));
  taskRunning = 0;
  vTaskDelete(NULL);
}

void startDump(dump_arguments_t* dump_args) { 
  if (taskRunning){
    return;
  }
  args = *dump_args;
  taskRunning = 1;
  xTaskGenericCreate(dumpTask, (const signed char *)"dumpTask", 4096, NULL,
                     configMAX_PRIORITIES - 3, &dumpTaskHandle, NULL, NULL);

}

void abortDump(){
  setAbortFlag();
}

void forceAbortDump(){
  if (dumpTaskHandle == NULL || !taskRunning) {
    return; 
  }

  vTaskDelete(dumpTaskHandle);

  taskRunning = 0;
}
