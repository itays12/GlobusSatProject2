

/*
 /$$$$$$$$ /$$$$$$$$ /$$    /$$ /$$$$$$$$ /$$              /$$$$$$  /$$   /$$  /$$$$$$           /$$$$$$  /$$$$$$$$ /$$$$$$ /$$$$$$$
|__  $$__/| $$_____/| $$   | $$| $$_____/| $$             /$$__  $$| $$$ | $$ /$$__  $$         /$$__  $$| $$_____/|_  $$_/| $$__  $$
   | $$   | $$      | $$   | $$| $$      | $$            | $$  \__/| $$$$| $$| $$  \__/        | $$  \ $$| $$        | $$  | $$  \ $$
   | $$   | $$$$$   |  $$ / $$/| $$$$$   | $$            |  $$$$$$ | $$ $$ $$| $$ /$$$$ /$$$$$$| $$  | $$| $$$$$     | $$  | $$$$$$$/
   | $$   | $$__/    \  $$ $$/ | $$__/   | $$             \____  $$| $$  $$$$| $$|_  $$|______/| $$  | $$| $$__/     | $$  | $$__  $$
   | $$   | $$        \  $$$/  | $$      | $$             /$$  \ $$| $$\  $$$| $$  \ $$        | $$  | $$| $$        | $$  | $$  \ $$
   | $$   | $$$$$$$$   \  $/   | $$$$$$$$| $$$$$$$$      |  $$$$$$/| $$ \  $$|  $$$$$$/        |  $$$$$$/| $$       /$$$$$$| $$  | $$
   |__/   |________/    \_/    |________/|________/       \______/ |__/  \__/ \______/          \______/ |__/      |______/|__/  |__/
*/

/*


 /$$$$$$$  /$$$$$$$  /$$$$$$ /$$   /$$  /$$$$$$        /$$$$$$$$ /$$   /$$ /$$$$$$$$ /$$      /$$       /$$   /$$  /$$$$$$  /$$      /$$ /$$$$$$$$       /$$   /$$  /$$$$$$  /$$      /$$ /$$
| $$__  $$| $$__  $$|_  $$_/| $$$ | $$ /$$__  $$      |__  $$__/| $$  | $$| $$_____/| $$$    /$$$      | $$  | $$ /$$__  $$| $$$    /$$$| $$_____/      | $$$ | $$ /$$__  $$| $$  /$ | $$| $$
| $$  \ $$| $$  \ $$  | $$  | $$$$| $$| $$  \__/         | $$   | $$  | $$| $$      | $$$$  /$$$$      | $$  | $$| $$  \ $$| $$$$  /$$$$| $$            | $$$$| $$| $$  \ $$| $$ /$$$| $$| $$
| $$$$$$$ | $$$$$$$/  | $$  | $$ $$ $$| $$ /$$$$         | $$   | $$$$$$$$| $$$$$   | $$ $$/$$ $$      | $$$$$$$$| $$  | $$| $$ $$/$$ $$| $$$$$         | $$ $$ $$| $$  | $$| $$/$$ $$ $$| $$
| $$__  $$| $$__  $$  | $$  | $$  $$$$| $$|_  $$         | $$   | $$__  $$| $$__/   | $$  $$$| $$      | $$__  $$| $$  | $$| $$  $$$| $$| $$__/         | $$  $$$$| $$  | $$| $$$$_  $$$$|__/
| $$  \ $$| $$  \ $$  | $$  | $$\  $$$| $$  \ $$         | $$   | $$  | $$| $$      | $$\  $ | $$      | $$  | $$| $$  | $$| $$\  $ | $$| $$            | $$\  $$$| $$  | $$| $$$/ \  $$$
| $$$$$$$/| $$  | $$ /$$$$$$| $$ \  $$|  $$$$$$/         | $$   | $$  | $$| $$$$$$$$| $$ \/  | $$      | $$  | $$|  $$$$$$/| $$ \/  | $$| $$$$$$$$      | $$ \  $$|  $$$$$$/| $$/   \  $$ /$$
|_______/ |__/  |__/|______/|__/  \__/ \______/          |__/   |__/  |__/|________/|__/     |__/      |__/  |__/ \______/ |__/     |__/|________/      |__/  \__/ \______/ |__/     \__/|__/


 */







#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

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
#include <hcc/api_fat.h>

#ifdef TESTING
#include "TestingDemos/MainTest.h"
#else

void taskMain() {
  InitSubsystems();

  f_format(f_getdrive(), 0);
 const portTickType taskDelay = 30 / portTICK_RATE_MS;
  while (TRUE) {
    logError(EPS_Conditioning(), "Error in EPS");
    logError(TRX_Logic(), "Error in TRX");
    Maintenance();

    TelemetryCollectorLogic();

    vTaskDelay(taskDelay);
  }
}
#endif

// main operation function. will be called upon software boot.
int main() {

  xTaskHandle taskMainHandle;

  TRACE_CONFIGURE_ISP(DBGU_STANDARD, 115200, BOARD_MCK);
  // Enable the Instruction cache of the ARM9 core. Keep the MMU and Data Cache
  // disabled.
  CP15_Enable_I_Cache();

  // The actual watchdog is already started, this only initializes the
  // watchdog-kick interface.
  WDT_start();

// create the main operation task of the satellite

	WDT_startWatchdogKickTask(10 / portTICK_RATE_MS, TRUE);
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









/*


              U _____ u        _         U  ___ u  __     __   U _____ u        ____        U  ___ u   _   _     U _____ u   _   _
 __        __ \| ___"|/       |"|         \/"_ \/  \ \   /"/u  \| ___"|/     U |  _"\ u      \/"_ \/  | \ |"|    \| ___"|/  | \ |"|
 \"\      /"/  |  _|"       U | | u       | | | |   \ \ / //    |  _|"        \| |_) |/      | | | | <|  \| |>    |  _|"   <|  \| |>
 /\ \ /\ / /\  | |___        \| |/__  .-,_| |_| |   /\ V /_,-.  | |___         |  _ <    .-,_| |_| | U| |\  |u    | |___   U| |\  |u
U  \ V  V /  U |_____|        |_____|  \_)-\___/   U  \_/-(_/   |_____|        |_| \_\    \_)-\___/   |_| \_|     |_____|   |_| \_|
.-,_\ /\ /_,-. <<   >>        //  \\        \\       //         <<   >>        //   \\_        \\     ||   \\,-.  <<   >>   ||   \\,-.
 \_)-'  '-(_/ (__) (__)      (_")("_)      (__)     (__)       (__) (__)      (__)  (__)      (__)    (_")  (_/  (__) (__)  (_")  (_/


 */
