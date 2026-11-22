# CLOGGER

A minimalist c logging utility similar to python's logging module. Drop directly into source project. A demo is also provided.
Generates logs of the format:

2026-11-21 19:56:24 [INFO] (main):
**** Intialiized
2026-11-21 19:56:24 [ERROR] (main): This is an error
2026-11-21 19:56:24 [WARNING] (main): This is a warning
2026-11-21 19:56:24 [DEBUG] (main): This is a debug call


Tested in a multithreaded C++ program on WIN64.

Drop source and header files into your project. Should compile with both gcc and msvc.

Log files need to be initialized with a **file name** and **logging level**. The former specifies where the log goes, the latter what gets written to it.

| Level             | Value   |
| ------------------| 0       |
| LOG_LEVEL_ERROR   | 1       |
| LOG_LEVEL_WARNING | 2       |
| LOG_LEVEL_INFO    | 3       |
| LOG_LEVEL_DEBUG   | 4       |

A demo project is included.

Enjoy
