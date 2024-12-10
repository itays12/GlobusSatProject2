/*
 * TM_managment.h
 *
 *  Created on: Apr 8, 2019
 *      Author: Hoopoe3n
 */

#ifndef TM_MANAGMENT_H_
#define TM_MANAGMENT_H_

#include "SubSystemModules/Communication/SatCommandHandler.h"
#include "SubSystemModules/Housekepping/TelemetryFiles.h"
#include <GlobalStandards.h>
#include <hal/Boolean.h>
#include <hal/Timing/Time.h>
#include <time.h>

#define MAX_FILE_NAME_SIZE 11
#define NUM_ELEMENTS_READ_AT_ONCE 1000

#define FS_FILE_ENDING "TLM"
#define FS_FILE_ENDING_SIZE 3

#ifndef FSFRAM
#define FSFRAM 0x20000
#endif

typedef enum {
  FS_SUCCESS,
  FS_DUPLICATED,
  FS_LOCKED,
  FS_TOO_LONG_NAME,
  FS_BUFFER_OVERFLOW,
  FS_NOT_EXIST,
  FS_ALLOCATION_ERROR,
  FS_FRAM_FAIL,
  FS_FAT_API_FAIL,
  FS_FAIL
} FileSystemResult;

#define PROPEGATE_FS_ERROR(err, msg)                                           \
  do {                                                                         \
    int rv = err;                                                              \
    if (logFsErr(rv, msg) != FS_SUCCESS) {                                     \
      return err;                                                              \
    }                                                                          \
  } while (0)

FileSystemResult logFsErr(FileSystemResult err, char *msg);

int CMD_getInfoImage(sat_packet_t *cmd);

int CMD_getDataImage(sat_packet_t *cmd);
/**
 * write telematry data to file
 */
int writeToFile(void *data, int size, tlm_type_t tlm_type);

int deleteTLMFiles(tlm_type_t tlm_type, Time date, int numOfDays);

/*
 * delete a single file from the SD
 */
int deleteTLMFile(tlm_type_t tlm_type, Time date);

int deleteOldestFile();

void calculateFileName(Time curr_date, char *file_name, char *ext);
/*
 * Read telematry file and send it to ground station over RF
 */
int readTLMFile(tlm_type_t tlm_type, Time date, int cmd_id,
                int resolution);
/*
 *
 */
int readTLMFiles(tlm_type_t tlm_type, Time date, int cmd_id,
                 int resolution);

int readTLMFileTimeRange(tlm_type_t tlm_type, time_unix from_time, time_unix to_time,
                         int cmd_id, int resolution);

void delete_allTMFilesFromSD();
/*!
 * Initializes the file system.
 * @note call once for boot and after DeInitializeFS.
 * @return FS_FAIL if Initializing the FS failed,
 * FS_ALLOCATION_ERROR on malloc error,
 * FS_SUCCSESS on success.
 */
FileSystemResult InitializeFS();

/*!
 * DeInitializes the file system.
 */
void DeInitializeFS(int sd_card);

void print_file(char *c_file_name);
#endif /* TM_MANAGMENT_H_ */
