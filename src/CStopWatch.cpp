//////////////////////////////////////////////////////////////////////////////
// CStopWatch.cpp
// =========
// High Resolution CStopWatch.
// This CStopWatch is able to measure the elapsed time with 1 micro-second accuracy
// in both Windows, Linux and Unix system
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2003-01-13
// UPDATED: 2006-01-13
//
// Copyright (c) 2003 Song Ho Ahn
//////////////////////////////////////////////////////////////////////////////

#include "CStopWatch.h"
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////
// constructor
///////////////////////////////////////////////////////////////////////////////
CStopWatch::CStopWatch()
{
#ifdef _WIN32
    QueryPerformanceFrequency(&frequency);
    startCount.QuadPart = 0;
    endCount.QuadPart = 0;
#else
    startCount.tv_sec = startCount.tv_usec = 0;
    endCount.tv_sec = endCount.tv_usec = 0;
#endif

    stopped = 0;
    startTimeInMicroSec = 0;
    endTimeInMicroSec = 0;
}



///////////////////////////////////////////////////////////////////////////////
// distructor
///////////////////////////////////////////////////////////////////////////////
CStopWatch::~CStopWatch()
{
}



///////////////////////////////////////////////////////////////////////////////
// start CStopWatch.
// startCount will be set at this point.
///////////////////////////////////////////////////////////////////////////////
void CStopWatch::startTimer()
{
    stopped = 0; // reset stop flag
#ifdef _WIN32
    QueryPerformanceCounter(&startCount);
#else
    gettimeofday(&startCount, NULL);
#endif
}



///////////////////////////////////////////////////////////////////////////////
// stop the CStopWatch.
// endCount will be set at this point.
///////////////////////////////////////////////////////////////////////////////
void CStopWatch::stopTimer()
{
    stopped = 1; // set CStopWatch stopped flag

#ifdef _WIN32
    QueryPerformanceCounter(&endCount);
#else
    gettimeofday(&endCount, NULL);
#endif
}



///////////////////////////////////////////////////////////////////////////////
// compute elapsed time in micro-second resolution.
// other getElapsedTime will call this first, then convert to correspond resolution.
///////////////////////////////////////////////////////////////////////////////
double CStopWatch::getElapsedTimeInMicroSec()
{
#ifdef _WIN32
    if(!stopped)
        QueryPerformanceCounter(&endCount);

    startTimeInMicroSec = startCount.QuadPart * (1000000.0 / frequency.QuadPart);
    endTimeInMicroSec = endCount.QuadPart * (1000000.0 / frequency.QuadPart);
#else
    if(!stopped)
        gettimeofday(&endCount, NULL);

    startTimeInMicroSec = (startCount.tv_sec * 1000000.0) + startCount.tv_usec;
    endTimeInMicroSec = (endCount.tv_sec * 1000000.0) + endCount.tv_usec;
#endif

    return endTimeInMicroSec - startTimeInMicroSec;
}



///////////////////////////////////////////////////////////////////////////////
// divide elapsedTimeInMicroSec by 1000
///////////////////////////////////////////////////////////////////////////////
double CStopWatch::getElapsedTimeInMilliSec()
{
    return this->getElapsedTimeInMicroSec() * 0.001;
}



///////////////////////////////////////////////////////////////////////////////
// divide elapsedTimeInMicroSec by 1000000
///////////////////////////////////////////////////////////////////////////////
double CStopWatch::getElapsedTimeInSec()
{
    return this->getElapsedTimeInMicroSec() * 0.000001;
}



///////////////////////////////////////////////////////////////////////////////
// same as getElapsedTimeInSec()
///////////////////////////////////////////////////////////////////////////////
double CStopWatch::getElapsedTime()
{
    return this->getElapsedTimeInSec();
}
