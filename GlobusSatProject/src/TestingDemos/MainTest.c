#include "InitSystem.h"
#include "MainTest.c"

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

	while(UTIL_DbguGetIntegerMinMax(&selection, 1, 5) == 0);

	switch (selection){
	case 1:

	}
}
