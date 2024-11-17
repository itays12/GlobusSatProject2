#include "TLM_management.h"
#include "hcc/api_fat.h"
#include <stdio.h>

#define MAX_LOG_PATH 16

int writeToFile(void *data, int size, char *ext) {
    Time time;
    Time_get(&time);

    char buffer[MAX_LOG_PATH];
    snprintf(buffer, MAX_LOG_PATH, "/log/%02d/%02d/%02d.%s", 
             time.year + 2000, time.month, time.date, ext);

    printf("Writing telemetry to file: %s\n", buffer);

    FN_FILE* file = f_open(buffer, "a");
    if (!file) {
        printf("Error opening file: %s\n", buffer);
        return FS_NOT_EXIST;  // Error code for failure to open file
    }

    time_unix unix_time = Time_convertTimeToEpoch(&time);

    // Write the timestamp
    if (f_write(&unix_time, sizeof(time_unix), 1, file) != 1) {
        printf("Error writing timestamp to file\n");
        f_close(file);  // Ensure file is closed
        return FS_FAT_API_FAIL;  // Error code for write failure
    }

    // Write the data
    if (f_write(data, size, 1, file) != 1) {
        printf("Error writing data to file\n");
        f_close(file);  // Ensure file is closed
        return FS_FAT_API_FAIL;  // Error code for write failure
    }

    // Close the file after writing
    if (f_close(file) != 0) {
        printf("Error closing file\n");
        return FS_FAT_API_FAIL;  // Error code for close failure
    }

    return FS_SUCCESS;  // Success
}
