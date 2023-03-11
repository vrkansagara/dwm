#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

/**
 * Easy debug
 * @define bool is_debug_on = false;
 * @usageprint_dbg(is_debug_on,"\n DWM starting....\n");
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
//		int ret = vprintf(format, args);
		vprintf(format, args);
		va_end(args);
	}
	return;
}