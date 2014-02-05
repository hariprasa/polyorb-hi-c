/*
 * This is a part of PolyORB-HI-C distribution, a minimal
 * middleware written for generated code from AADL models.
 * You should use it with the Ocarina toolsuite.
 *
 * For more informations, please visit http://assert-project.net/taste
 *
 * Copyright (C) 2007-2009 Telecom ParisTech, 2010-2012 ESA & ISAE.
 */
#ifndef __PO_HI_TIME_H__
#define __PO_HI_TIME_H__

#include <po_hi_types.h>

/* including po_hi_returns.h to be able to use constants in
 * specifications
 */
#include <po_hi_returns.h>

#ifndef HAVE_CLOCK_GETTIME
#if defined (__CYGWIN__) || defined (__MINGW32__)
#else
#include <xlocale.h>
#endif

#include <time.h>
#endif

#ifdef XENO_NATIVE
#include <native/timer.h>
#include <native/task.h>
#endif


#ifdef _WIN32
#include <tchar.h>
#include <windows.h>

/*
 * Win32 helper functions to convert __po_hi_time_t to a representation
 * that would be suitable for Windows.
 */
unsigned __po_hi_windows_tick_to_unix_seconds(long long win_ticks);
LARGE_INTEGER __po_hi_unix_seconds_to_windows_tick(unsigned sec, unsigned nsec);
#endif

/*
 * Represent the time in PolyORB-HI.
 *
 * The value stored in this type depends on the system : on POSIX, it
 * is the epoch (time since 1970), on other systems, it can be the
 * number of elapsed ticks since the beginning of the program.
 *
 * The granularity of the time is in microsecond (10^-6)
 */
typedef struct
{
   __po_hi_uint32_t     sec;     /* amount of second     */
   __po_hi_uint32_t     nsec;    /* amount of nanosecond */
}__po_hi_time_t;

/*
 * An ACSL predicate to verify invariant on _po_hi_time_t.
 */
/*@
  @ predicate is_time_struct_correct(__po_hi_time_t *mytime) =
  @   mytime->sec >= 0 &&
  @   mytime->nsec >= 0 && mytime->nsec < 1000000000;
  @*/

#define __PO_HI_TIME_TO_US(value) ((value.sec*1000000)+(value.nsec / 1000))

#define __PO_HI_TIME_TO_MS(value) ((value.sec*1000)+(value.nsec / 1000000))

/*
 * <ugly hack warning>
 * Add specification for clock_gettime ensuring that the timespec
 * structure used in clock_gettime does not overlap with a ghost field
 * representing the __po_hi_time_t * parameter of __po_hi_get_time.
 * </ugly hack warning>
 */
//@ ghost __po_hi_time_t *time_struct_to_be_initialized;

/*@ ensures \valid(time_struct_to_be_initialized);
  @ ensures \separated(time_struct_to_be_initialized, __tp);
  @ ensures time_struct_to_be_initialized == \old(time_struct_to_be_initialized);
  @*/
int clock_gettime (clockid_t __clock_id, struct timespec *__tp);

/*
 * Get the current time and store informations
 * in the structure mytime.
 * If there is an error, returns a negative value
 * (ERROR_CLOCK). Else, returns a positive value.
 *
 * For the ACSL specification, this function always returns
 * __PO_HI_SUCCESS as we do force POSIX and guarantee that the call to
 * clock_gettime is correct. The specification also precises the
 * possible values for sec and nsec fields.
 *
 * Must include po_hi_returns.h to be able to use __PO_HI_SUCCESS
 * in specification.
 */
/*@ requires \valid(mytime);
  @ //assigns mytime->sec;
  @ //assigns mytime->nsec;
  @ ensures is_time_struct_correct(mytime);
  @ ensures \result == __PO_HI_SUCCESS;
  @*/
int __po_hi_get_time (__po_hi_time_t* mytime);

/*
 * Add the two structures given in parameter. The returned
 * value is the result of the operation.
 */
int __po_hi_add_times (__po_hi_time_t* result,
                       const __po_hi_time_t* left,
                       const __po_hi_time_t* right);

/*
 * Build a __po_hi_time_t value which contains the
 * amount of time (in seconds) represented by the
 * argument seconds.
 */
int __po_hi_seconds (__po_hi_time_t* time,
                     const __po_hi_uint32_t seconds);

/*
 * Build a __po_hi_time_t value which contains the
 * amount of time (in milliseconds) represented by the
 * argument milliseconds.
 */
int __po_hi_milliseconds  (__po_hi_time_t* time,
                           const __po_hi_uint32_t milliseconds);

/*
 * Build a __po_hi_time_t value which contains the
 * amount of time (in microseconds) represented by the
 * argument microseconds.
 */
int __po_hi_microseconds  (__po_hi_time_t* time,
                           const __po_hi_uint32_t microseconds);

/*
 * sleep until the time given in argument.
 * Return SUCCESS if there is no error. Else, it returns
 * a negative value : ERROR_CLOCK or ERROR_PTHREAD_COND
 */
int __po_hi_delay_until (const __po_hi_time_t* time);

/*
 * Copy a time value from src to dst.
 * Returns __PO_HI_SUCCESS if successful.
 */
int __po_hi_time_copy (__po_hi_time_t* dst, const __po_hi_time_t* src);

/*
 * If the system doesn't support the clock_gettime function, we
 * emulate it. For example, Darwin does not support it
 */
#ifdef NEED_CLOCK_GETTIME
#define CLOCK_REALTIME 0
int clock_gettime(int clk_id, struct timespec *tp);
#endif

/*
 * Indicates if a time value is greater than an other.
 * Returns 1 if value is greater than limit.
 * Returns 0 otherwise.
 */
int __po_hi_time_is_greater (const __po_hi_time_t* value, const __po_hi_time_t* limit);

/*
 * Ensure that ETIMEDOUT is defined
 * Workaround for bug #286
 */
#include <errno.h>
#ifndef ETIMEDOUT
#define ETIMEDOUT 60
#endif

#endif /* __PO_HI_TIME_H__ */
