#include "TLM_management.h"
#include "SubSystemModules/Communication/SPL.h"
#include "SubSystemModules/Communication/SatCommandHandler.h"
#include "SubSystemModules/Communication/TRXVU.h"
#include "SubSystemModules/Housekepping/TelemetryFiles.h"
#include "hcc/api_fat.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FILE_NAME 32

FileSystemResult logFsErr(FileSystemResult err, char *msg) {
  if (err != FS_SUCCESS) {
    FileSystemError fsError;
    strncpy(fsError.msg, msg, 20);
    fsError.msg[20] = 0;
    FRAM_WRITE_FIELD(&fsError, fsError);
  }

  return err;
}

char *tlmTypeToExt(tlm_type_t type) {
  switch (type) {
  case tlm_eps:
    return END_FILENAME_EPS_TLM;
  case tlm_tx:
    return END_FILE_NAME_TX;
  case tlm_antenna:
    return END_FILE_NAME_ANTENNA;
  case tlm_solar:
    return END_FILENAME_SOLAR_PANELS_TLM;
  case tlm_wod:
    return END_FILENAME_WOD_TLM;

  case tlm_rx:
    return END_FILE_NAME_RX;
  case tlm_rx_frame:
    return END_FILE_NAME_RX_FRAME;
  case tlm_log:
    return END_FILENAME_LOGS;
  default:
    return "def";
  }

  return "def";
}

FileSystemResult InitializeFS() {
  PROPEGATE_FS_ERROR(fs_init(), "fs_init");
  PROPEGATE_FS_ERROR(fs_start(), "fs_start");

  return FS_SUCCESS;
}

void DeInitializeFS(int sd_card) {
  logFsErr(fs_stop(), "fs_stop");
  logFsErr(fs_delete(), "fs_delete");
}

int createDirectories(const char *path) {
  char temp[MAX_FILE_NAME];
  char *p = NULL;

  snprintf(temp, sizeof(temp), "%s", path);

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
      return FS_FAT_API_FAIL;
    }
  }

  return FS_SUCCESS;
}

int writeToFile(void *data, int size, tlm_type_t tlm_type) {
  char *ext = tlmTypeToExt(tlm_type);

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

void normalizeDate(Time *date) {
  const int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  while (date->date > days_in_month[date->month - 1]) {
    int max_days = days_in_month[date->month - 1];
    if (date->month == 2 && Time_isLeapYear(date->year + 2000)) {
      max_days = 29;
    }

    date->date -= max_days;
    date->month++;

    if (date->month > 12) {
      date->month = 1;
      date->year++;
    }
  }
}

void calculateFileName(Time curr_date, char *file_name, char *endFileName) {

  snprintf(file_name, MAX_FILE_NAME, "/log/%04d/%02d/%02d.%s",
           curr_date.year + 2000, curr_date.month, curr_date.date, endFileName);
}

int readTLMFileTimeRange(tlm_type_t tlm_type, time_unix from_time,
                         time_unix to_time, int cmd_id, int data_size) {

  time_unix current_unix_time = from_time;
  Time current_time;
  PROPEGATE_FS_ERROR(Time_convertEpochToTime(current_unix_time, &current_time),
                     "epochToTime");

  sat_packet_t cmd = {0};
  cmd.ID = cmd_id;
  cmd.cmd_type = dump_type;
  cmd.cmd_subtype = tlm_type;

  char *ext = tlmTypeToExt(tlm_type);
  char file_name[MAX_FILE_NAME];
  FILE *file = NULL;

  int err = 0;
  while (1) {
    calculateFileName(current_time, file_name, ext);

    FILE *file = fopen(file_name, "rb");
    if (!file) {
      return FS_FAT_API_FAIL;
    }

    int buffer_offset = 0;
    while (!feof(file)) {
      time_unix file_timestamp;

      if (fread(&file_timestamp, sizeof(time_unix), 1, file) != 1) {
        if (feof(file))
          break;
        err = FS_FAT_API_FAIL;
        goto cleanup;
      }

      if (file_timestamp > to_time) {
        goto cleanup;
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
        err = FS_FAT_API_FAIL;
        goto cleanup;
      }
      buffer_offset += bytes_read;
    }

    fclose(file);
    file = NULL;

    current_time.date += 1;
    normalizeDate(&current_time);
  }
cleanup:
  if (file) {
    fclose(file);
  }

  return err;
}

int deleteOldestFile() {
  FN_FIND find;
  int res;

  int oldest_year = 9999;
  int oldest_month = 99;
  int oldest_day = 99;
  char oldest_ext[8] = {0}; // extension buffer
  char oldest_filepath[MAX_FILE_NAME] = {0};

  // Step 1: Find the earliest year
  {
    char search_path[] = "/log";
    res = f_findfirst(search_path, &find);
    if (res != F_NO_ERROR) {
      return FS_NOT_EXIST;
    }

    do {
      if ((find.attr & F_ATTR_DIR) && find.name[0] != '.') {
        int year = atoi(find.name);
        if (year > 0 && year < oldest_year) {
          oldest_year = year;
        }
      }
      res = f_findnext(&find);
    } while (res == F_NO_ERROR);
  }

  if (oldest_year == 9999) {
    return FS_NOT_EXIST;
  }

  // Step 2: Find earliest month in that year
  {
    char year_path[MAX_FILE_NAME];
    snprintf(year_path, sizeof(year_path), "/log/%04d", oldest_year);

    res = f_findfirst(year_path, &find);
    if (res != F_NO_ERROR) {
      return FS_NOT_EXIST;
    }

    do {
      if ((find.attr & F_ATTR_DIR) && find.name[0] != '.') {
        int month = atoi(find.name);
        if (month > 0 && month < oldest_month) {
          oldest_month = month;
        }
      }
      res = f_findnext(&find);
    } while (res == F_NO_ERROR);
  }

  if (oldest_month == 99) {
    return FS_NOT_EXIST;
  }

  // Step 3: Find earliest day file in that month
  {
    char month_path[MAX_FILE_NAME];
    snprintf(month_path, sizeof(month_path), "/log/%04d/%02d", oldest_year,
             oldest_month);

    res = f_findfirst(month_path, &find);
    if (res != F_NO_ERROR) {
      return FS_NOT_EXIST;
    }

    do {
      if (!(find.attr & F_ATTR_DIR) && find.name[0] != '.') {
        int day = atoi(find.name);
        if (day > 0) {
          if (day < oldest_day ||
              (day == oldest_day && strcmp(find.ext, oldest_ext) < 0)) {
            oldest_day = day;
            strncpy(oldest_ext, find.ext, sizeof(oldest_ext));
          }
        }
      }
      res = f_findnext(&find);
    } while (res == F_NO_ERROR);
  }

  if (oldest_day == 99) {
    return FS_NOT_EXIST;
  }

  // Construct the full path of the oldest file and delete it
  snprintf(oldest_filepath, sizeof(oldest_filepath), "/log/%04d/%02d/%02d.%s",
           oldest_year, oldest_month, oldest_day, oldest_ext);

  if (f_delete(oldest_filepath) != F_NO_ERROR) {
    return FS_FAT_API_FAIL;
  }

  // Attempt to remove the month directory
  {
    char month_path[MAX_FILE_NAME];
    snprintf(month_path, sizeof(month_path), "/log/%04d/%02d", oldest_year,
             oldest_month);

    int rmdir_res = f_rmdir(month_path);
    if (rmdir_res != F_NO_ERROR) {
      if (rmdir_res == F_ERR_NOTEMPTY) {
        return FS_FAT_API_FAIL;
      }
    }
  }

  // Attempt to remove the year directory
  {
    char year_path[MAX_FILE_NAME];
    snprintf(year_path, sizeof(year_path), "/log/%04d", oldest_year);

    int rmdir_res = f_rmdir(year_path);
    if (rmdir_res != F_NO_ERROR) {
      if (rmdir_res == F_ERR_NOTEMPTY) {
      } else {
        return FS_FAT_API_FAIL;
      }
    }
  }

  return FS_SUCCESS;
}
