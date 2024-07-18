#include "InitSystem.h"
#include "MainTest.h"
#include "TestingDemos/CommandsTestingDemo.h"
#include "TestingDemos/EpsTestingDemo.h"
#include "TestingDemos/FileSystemTestingDemo.h"
#include "TestingDemos/TrxvuTestingDemo.h"
#include "hal/Utility/util.h"
#include "hal/boolean.h"
#include "stdio.h"

void taskTesting(){
	InitSubsystems();
	while(selectAndExecuteTest());
}

Boolean selectAndExecuteTest(){
	int selection;
	printf("Choose Tests: \n\r");
	printf("1: TRXVU \n\r");
	printf("2: EPS \n\r");
	printf("3: FileSystem \n\r");
	printf("4: Commands \n\r");

	while(UTIL_DbguGetIntegerMinMax(&selection, 1, 5) == 0){};
  InitSubsystems();
  
  Boolean stop = FALSE;
	switch (selection){
	  case 1:
      stop = MainTrxvuTestBench();
      break;
    case 2:
      stop = MainEpsTestBench();
      break;
    case 3:
      stop = MainFileSystemTestBench();
      break;
    case 4: 
      stop = MainCommandsTestBench();
      break;
	}
  return stop;
}
