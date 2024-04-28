#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>


#define IS_DEBUG_ON 1

/**
 * Easy debug
 * @define bool is_debug_on = false;
 * @usage print_dbg(is_debug_on,"\n DWM starting....\n");
 * @param is_debug_on
 * @param restrict
 * @param ...
 */

void print_dbg(bool is_debug_on, const char * restrict format, ...) {

    char date_time[40] = {0};
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // Date time format like Y-m-d H:i:s relevant to mysql
    // https://dev.mysql.com/doc/refman/5.7/en/date-and-time-functions.html
    sprintf(date_time, "%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    if(is_debug_on != false) {
        va_list args;
        printf("\r\nDWM_DBG [%s] : ",date_time);
        va_start(args, format);
        //      int ret = vprintf(format, args);
        vprintf(format, args);
        va_end(args);
    }
}


void fprint_dbg(bool is_debug_on, const char * restrict format, ...) {

    char date_time[40] = {0};
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // Date time format like Y-m-d H:i:s relevant to mysql
    // https://dev.mysql.com/doc/refman/5.7/en/date-and-time-functions.html
    sprintf(date_time, "%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    if(is_debug_on) {
        va_list args;
        va_start(args, format);

        FILE *logFile = fopen("/tmp/dwm.log", "a+");
        if (logFile == NULL)
        {
            perror("Error opening file :: /tmp/dwm.log");
            exit(1);
        }

        //        fprintf(logFile,"\r\n=====");
        /* print timestamp first */
        fprintf(logFile,"\r\nDWM_DBG [%s] : ",date_time);
        /* print everything else */
        vfprintf (logFile, format, args);
        //        fprintf(logFile,"\r\nFile [%s] Line [ %d ] Function [%s] : ",__FILE__, __LINE__, __func__);
        //        fprintf(logFile,"\r\n=====\n\n");

        fclose(logFile);
        // +++++++++++++++++++++
        va_end(args);
    }
}