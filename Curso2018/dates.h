/*
 * dates.h
 *
 *  Created on: 7 nov. 2018
 *      Author: migueltoro
 */

#ifndef DATES_H_
#define DATES_H_

#include <time.h>

//struct tm {
//   int tm_sec;         /* seconds,  range 0 to 59          */
//   int tm_min;         /* minutes, range 0 to 59           */
//   int tm_hour;        /* hours, range 0 to 23             */
//   int tm_mday;        /* day of the month, range 1 to 31  */
//   int tm_mon;         /* month, range 0 to 11             */
//   int tm_year;        /* The number of years since 1900   */
//   int tm_wday;        /* day of the week, range 0 to 6    */
//   int tm_yday;        /* day in the year, range 0 to 365  */
//   int tm_isdst;       /* daylight saving time             */
//};

time_t now_time();
time_t create_time(int day, int month, int year);
struct tm * calendar(time_t time);
time_t time_add_days(time_t date, int days);
time_t time_add_months(time_t date, int months);
time_t time_add_years(time_t date, int years);
time_t time_minus_days(time_t date, int days);
time_t time_minus_months(time_t date, int months);
time_t time_minus_years(time_t date, int years);

#endif /* DATES_H_ */
