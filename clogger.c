/*
* logger.c: Minimalist thread-safe logging
*
* MIT License
*
* Copyright (c) 2025 Emmett Snyder
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the “Software”), to deal in the Software without restriction,
* including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE
*
*/

#include "clogger.h"
#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
static CRITICAL_SECTION log_mutex;
#define LOCK()   EnterCriticalSection(&log_mutex)
#define UNLOCK() LeaveCriticalSection(&log_mutex)
#else
#include <pthread.h>
static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
#define LOCK()   pthread_mutex_lock(&log_mutex)
#define UNLOCK() pthread_mutex_unlock(&log_mutex)
#endif

static FILE* log_file = NULL;
static size_t  current_level = LOG_LEVEL_DEBUG;

static int vlog_write(size_t level,
    const char* level_str,
    const char* caller,
    const char* fmt,
    va_list args)
{
    int rc = EXIT_SUCCESS;
    LOCK();

    if (log_file && level <= current_level) {
        time_t t = time(NULL);
        struct tm tmst;
        errno_t err = localtime_s(&tmst, &t);
        if (err != 0) {
            UNLOCK();
            return EXIT_FAILURE;
        }

        char timestr[20];
        strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", &tmst);

        if (fprintf(log_file, "%s [%s] (%s): ", timestr, level_str, caller) < 0)
            rc = EXIT_FAILURE;

        if (vfprintf(log_file, fmt, args) < 0)
            rc = EXIT_FAILURE;

        if (fputs("\n", log_file) == EOF || fflush(log_file) == EOF)
            rc = EXIT_FAILURE;
    }

    UNLOCK();
    return rc;
}

int log_init(const char* logfile, size_t level)
{
#if defined(_WIN32) || defined(_WIN64)
    InitializeCriticalSection(&log_mutex);
#endif

    FILE* fp = NULL;
    errno_t err = fopen_s(&fp, logfile, "a");
    if (err != 0 || fp == NULL)
        return EXIT_FAILURE;

    log_file = fp;
    current_level = level;
    return EXIT_SUCCESS;
}

/* Internal log functions with caller passed in */

int log_error_internal(const char* caller, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int rc = vlog_write(LOG_LEVEL_ERROR, "ERROR", caller, fmt, args);
    va_end(args);
    return rc;
}

int log_warning_internal(const char* caller, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int rc = vlog_write(LOG_LEVEL_WARNING, "WARNING", caller, fmt, args);
    va_end(args);
    return rc;
}

int log_info_internal(const char* caller, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int rc = vlog_write(LOG_LEVEL_INFO, "INFO", caller, fmt, args);
    va_end(args);
    return rc;
}

int log_debug_internal(const char* caller, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int rc = vlog_write(LOG_LEVEL_DEBUG, "DEBUG", caller, fmt, args);
    va_end(args);
    return rc;
}

void log_close(void)
{
    LOCK();
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
    UNLOCK();

#if defined(_WIN32) || defined(_WIN64)
    DeleteCriticalSection(&log_mutex);
#else
    pthread_mutex_destroy(&log_mutex);
#endif
}

int log_purge(void)
{
    LOCK();

    if (!log_file) {
        UNLOCK();
        return EXIT_FAILURE;
    }

    // Reopen the log file in write mode to truncate it
#if defined(_WIN32) || defined(_WIN64)
    FILE* fp = NULL;
    errno_t err = fopen_s(&fp, "log_temp.log", "w");
    if (err != 0 || fp == NULL) {
        UNLOCK();
        return EXIT_FAILURE;
    }
#else
    FILE* fp = fopen("log_temp.log", "w");
    if (!fp) {
        UNLOCK();
        return EXIT_FAILURE;
    }
#endif

    fclose(log_file);
    remove("log_temp.log");

    // Reopen original file in truncate mode
#if defined(_WIN32) || defined(_WIN64)
    err = fopen_s(&log_file, "log.txt", "w");
    if (err != 0 || log_file == NULL) {
        UNLOCK();
        return EXIT_FAILURE;
    }
#else
    log_file = fopen("log.txt", "w");
    if (!log_file) {
        UNLOCK();
        return EXIT_FAILURE;
    }
#endif

    UNLOCK();
    return EXIT_SUCCESS;
}
