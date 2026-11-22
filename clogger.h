/*
* logger.h: Thread-safe logging - header file.
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

#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#ifndef LOGGER_H
#define LOGGER_H


#include <stdlib.h>
#include <stdarg.h>

/* Logging levels */
enum {
    LOG_LEVEL_ERROR = 0,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG
};

/* Initialization and cleanup */
int log_init(const char* logfile, size_t level);
void log_close(void);

/* Internal logging functions */
int log_error_internal(const char* caller, const char* fmt, ...);
int log_warning_internal(const char* caller, const char* fmt, ...);
int log_info_internal(const char* caller, const char* fmt, ...);
int log_debug_internal(const char* caller, const char* fmt, ...);

/* Macro wrappers for capturing caller name */
#define log_error(...)   log_error_internal(__func__, __VA_ARGS__)
#define log_warning(...) log_warning_internal(__func__, __VA_ARGS__)
#define log_info(...)    log_info_internal(__func__, __VA_ARGS__)
#define log_debug(...)   log_debug_internal(__func__, __VA_ARGS__)

/* Purge all contents from the log file while retaining the file handle */
int log_purge(void);

#endif /* LOGGER_H */

#ifdef __cplusplus
}
#endif

