#include "TLM_management.h"
#include "SubSystemModules/Communication/SPL.h"
#include "SubSystemModules/Communication/SatCommandHandler.h"
#include "SubSystemModules/Communication/TRXVU.h"
#include "SubSystemModules/Housekepping/TelemetryFiles.h"
#include "hcc/api_fat.h"
#include <stdio.h>
#include <string.h>

#define MAX_FILE_NAME 32

int createDirectories(const char *path) {
  char temp[MAX_FILE_NAME];
  char *p = NULL;

  snprintf(temp, sizeof(temp), "%s", path);

  // Find the last slash to exclude the file name
  char *last_slash = strrchr(temp, '/');
  if (last_slash != NULL) {
    *last_slash = '\0'; // Terminate at the directory portion of the path
  }

  for (p = temp + 1; *p; p++) {
    if (*p == '/') {
      *p = '\0'; // Temporarily terminate the string
      if (f_mkdir(temp) != F_NO_ERROR) {
       return FS_FAT_API_FAIL;
      }
      *p = '/'; 
    }
  }

  if (f_mkdir(temp) != F_NO_ERROR) {
    if (f_getlasterror() != F_ERR_DUPLICATED) {
      printf("Error creating final directory: %s\n", temp);
      return FS_FAT_API_FAIL;
    }
  }

  return FS_SUCCESS;
}

int writeToFile(void *data, int size, char *ext) {
  Time time;
  Time_get(&time);

  char buffer[MAX_FILE_NAME];
  snprintf(buffer, MAX_FILE_NAME, "/log/%04d/%02d/%02d.%s", time.year + 2000,
           time.month, time.date, ext);

  printf("Writing telemetry to file: %s\n", buffer);

  if (createDirectories(buffer) != FS_SUCCESS) {
    return FS_FAT_API_FAIL;
  }

  FN_FILE *file = f_open(buffer, "a");
  if (!file) {
    return FS_NOT_EXIST;
  }

  time_unix unix_time = Time_convertTimeToEpoch(&time);

  if (f_write(&unix_time, sizeof(time_unix), 1, file) != 1) {
    f_close(file);
    return FS_FAT_API_FAIL;
  }

  if (f_write(data, size, 1, file) != 1) {
    f_close(file);
    return FS_FAT_API_FAIL;
  }

  if (f_close(file) != 0) {
    return FS_FAT_API_FAIL;
  }

  return FS_SUCCESS;
}

void calculateFileName(Time curr_date, char *file_name, char *endFileName,
                       int days2Add) {
  const int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  Time adjusted_date = curr_date;

  adjusted_date.date += days2Add;

  while (adjusted_date.date > days_in_month[adjusted_date.month - 1]) {
    int max_days = days_in_month[adjusted_date.month - 1];
    if (adjusted_date.month == 2 &&
        Time_isLeapYear(adjusted_date.year + 2000)) {
      max_days = 29;
    }

    adjusted_date.date -= max_days;
    adjusted_date.month++;

    if (adjusted_date.month > 12) {
      adjusted_date.month = 1;
      adjusted_date.year++;
    }
  }

  snprintf(file_name, MAX_FILE_NAME, "/log/%04d/%02d/%02d.%s",
           adjusted_date.year + 2000, adjusted_date.month, adjusted_date.date,
           endFileName);
}

int readTLMFile(char *ext, Time date, int numOfDays, int cmd_id,
                int data_size) {
  char file_name[MAX_FILE_NAME];
  calculateFileName(date, file_name, ext, numOfDays);

  FILE *file = fopen(file_name, "rb");
  if (!file) {
    return -1;
  }

  time_unix current_unix_time;
  Time current_time;
  Time_get(&current_time);
  current_unix_time = Time_convertTimeToEpoch(&current_time);

  sat_packet_t cmd = {0};
  cmd.ID = cmd_id;
  cmd.cmd_type = dump_type;

  int buffer_offset = 0;
  int err = 0;
  while (!feof(file)) {
    time_unix file_timestamp;

    if (fread(&file_timestamp, sizeof(time_unix), 1, file) != 1) {
      if (feof(file))
        break;
      fclose(file);
      return -1;
    }

    if (file_timestamp > current_unix_time) {
      break;
    }

    if (buffer_offset + sizeof(time_unix) + data_size >
        MAX_COMMAND_DATA_LENGTH) {
      cmd.length = buffer_offset;
      err = logError(TransmitSplPacket(&cmd, NULL), "TransmitSplPacket");
      buffer_offset = 0;
    }
    memcpy(cmd.data + buffer_offset, &file_timestamp, sizeof(time_unix));
    buffer_offset += sizeof(time_unix);

    size_t bytes_read = fread(cmd.data + buffer_offset, 1, data_size, file);
    if (bytes_read == 0) {
      if (feof(file))
        break;
      fclose(file);
      return -1;
    }
    buffer_offset += bytes_read;
  }

  fclose(file);
  return 0;
}
