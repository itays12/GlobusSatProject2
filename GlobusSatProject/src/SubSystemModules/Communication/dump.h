#ifndef DUMP_H_
#define DUMP_H_
#include "SubSystemModules/Communication/TRXVU.h"

void dumpTask();

void startDump(dump_arguments_t* args);

void abortDump();
void forceAbortDump();

#endif
