// ***************************************************************************************
//    Project: Understanding Producer-Consumer and Publish-Subscribe Patterns in C
//    File: common.h
//    Date: 2024-08-08
//    Author: Navid Dezashibi
//    Contact: navid@dezashibi.com
//    Website: https://www.dezashibi.com | https://github.com/dezashibi
//    License:
//     Please refer to the LICENSE file, repository or website for more information about
//     the licensing of this work. If you have any questions or concerns,
//     please feel free to contact me at the email address provided above.
// ***************************************************************************************
// *  Description: Read the readme file for more information
// ***************************************************************************************

#ifndef NAVID_COMMON_H_
#define NAVID_COMMON_H_

#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#define xsleep(x) Sleep((x))
#else
void xsleep(unsigned int milliseconds)
{
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}
#endif

#endif // NAVID_COMMON_H_
