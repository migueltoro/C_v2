/*
 * dates.h
 *
 *  Created on: 7 nov. 2018
 *      Author: migueltoro
 */

#ifndef DATES_H_
#define DATES_H_

#include <time.h>

#include "../types/list.h"
#include "../types/types.h"
#include "../types/iterables.h"
#include "heap.h"

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

time_t * date_parse(time_t * out, char * text);
time_t * time_parse(time_t * out, char * text);
char * time_tostring(const void * p, char * mem);
char * date_tostring(const void * p, char * mem);
bool time_equals(const void * p1, const void * p2);
int time_naturalorder(const void * t1,const  void * t2);

time_t time_now();
time_t time_parse_date_s(char * text);
time_t hour_parse(char * text);
time_t time_parse_s(char * text);

// day 1..31, month 1..12
time_t time_of_date(int day, int month, int year);
time_t time_of(int day, int month, int year, int hour, int minute, int second);

struct tm * time_calendar(time_t time);
time_t time_add_days(time_t date, int days);
time_t time_add_months(time_t date, int months);
time_t time_add_years(time_t date, int years);
time_t time_minus_days(time_t date, int days);
time_t time_minus_months(time_t date, int months);
time_t time_minus_years(time_t date, int years);

double time_diff_seconds(time_t start,time_t end);
double time_diff_minutes(time_t start,time_t end);
double time_diff_hours(time_t start,time_t end);
int time_diff_years(time_t start,time_t end);
int time_diff_months(time_t start,time_t end);
int time_diff_days(time_t start,time_t end);

char * time_day_month_tostring(const void * p, char * mem);
char * date_tostring_s(const void * p, char * mem);
char * hours_tostring(const void * p, char * mem);


extern type date_type;
extern type time_type;

void test_dates();
void test_dates_1();

#endif /* DATES_H_ */
