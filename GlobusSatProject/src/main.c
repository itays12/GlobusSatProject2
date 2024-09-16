#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include "hal/errors.h"
#include <hal/Drivers/I2C.h>
#include <hal/Drivers/SPI.h>
#include <hal/Timing/Time.h>
#include <hal/Timing/WatchDogTimer.h>
#include <hal/Utility/util.h>
#include <hal/boolean.h>

#include <at91/commons.h>
#include <at91/peripherals/cp15/cp15.h>
#include <at91/utility/exithandler.h>
#include <at91/utility/trace.h>

#include <hcc/api_fat.h>

#include "GlobalStandards.h"
#include "InitSystem.h"
#include "SubSystemModules/Communication/TRXVU.h"
#include "SubSystemModules/Housekepping/TelemetryCollector.h"
#include "SubSystemModules/Maintenance/Maintenance.h"
#include "SubSystemModules/PowerManagment/EPS.h"
#include "main.h"
#include <stdlib.h>

#ifdef TESTING
#include "TestingDemos/MainTest.h"
#else

void taskMain() {
  InitSubsystems();

  while (TRUE) {
    logError(EPS_Conditioning(), "Error in TRX");
    logError(TRX_Logic(), "Error in TRX");

    // logError(Maintenance(),"Error in Maintenance");
    // logError(Telementry(),"Error in Telementry";
  }
}
#endif

// main operation function. will be called upon software boot.
int main() {

  xTaskHandle taskMainHandle;

  TRACE_CONFIGURE_ISP(DBGU_STANDARD, 2000000, BOARD_MCK);
  // Enable the Instruction cache of the ARM9 core. Keep the MMU and Data Cache
  // disabled.
  CP15_Enable_I_Cache();

  // The actual watchdog is already started, this only initializes the
  // watchdog-kick interface.
  WDT_start();

// create the main operation task of the satellite
#ifdef TESTING
  xTaskGenericCreate(taskTesting, (const signed char *)"taskTesting", 4096,
                     NULL, configMAX_PRIORITIES - 2, &taskMainHandle, NULL,
                     NULL);
#else
  xTaskGenericCreate(taskMain, (const signed char *)"taskMain", 4096, NULL,
                     configMAX_PRIORITIES - 2, &taskMainHandle, NULL, NULL);
#endif

  vTaskStartScheduler();
  exit(0);
}
