#ifndef TIMEUTIL_H
#define TIMEUTIL_H

/*******************************************************************************

InetVis - Internet Visualisation

version: 0.9.5
release date: 2007/11/21

Computer Science Honours Project - 2005
Computer Science Masters Project - 2006/7

author: Jean-Pierre van Riel
supervisor: Barry Irwin

License
-------

InetVis - Internet Visaulisation for network traffic.
Copyright (C) 2006, Jean-Pierre van Riel

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
Street, Fifth Floor, Boston, MA 02110-1301, USA.

*******************************************************************************/

/* timeutil.h & timeutil.cpp
 *
 * A class of static methods to provide time conversion and arithmetic
 *
 */

#include <qstring.h>
#include <qdatetime.h>
#include <math.h>

//if windows
//TO-DO

//if linux
#include <sys/time.h>


//reference values in seconds for timevals
#define TV_MIN 60
#define TV_HOUR 3600
#define TV_DAY 86400
#define TV_WEEK 604800
#define TV_YEAR 31557600


class TimeUtil
{
private:

    //preset variables for improving itterative computation
    static struct timeval timeRange0;
    static struct timeval timeRange1;
    static struct timeval timeRangeLength;
    static double dTimeRangeLength;

public:

    //time conversion functions that convert betweem time structures and other
    //values, i.e QDateTime object, double, etc
    //- note callers responsability to provide parameter struct/object, and
    //  this is in order to improve performance by passing by reference
    static void qDateTimeToTimeval(struct timeval &tv,
                                   const QDateTime &qDateTime);
    static void timevalToQDateTime(QDateTime &qDateTime,
                                   const struct timeval &tv);
    static QString realativeTimevalToQString(const struct timeval &tv);
    static void doubleToTimeval(struct timeval &result, double d);
    //these functions are not intended to convert to values larger than
    //2147483647 - the max value of a 32bit two's compliment integer
    //If the numbers are too large, overflow will result in a negative
    //value; it is the callers responsibility to check for this
    static double timevalToDouble(const struct timeval &tv);
    static double qTimeToDouble(const QTime &qTime);
    //convert from QTime to double value of seconds, reult should always
    //be less than TV_DAY
    static void doubleToQTime(QTime &time, const double d);

    //logical and arithmetic functions for computing time values
    static bool timevalGreaterThan(const struct timeval &tv1,
                                   const struct timeval &tv2);
    //true if tv1 > tv2, else false
    static void addTimevals(struct timeval &result, const struct timeval &tv1,
                            const struct timeval &tv2);
    //result = tv1 + tv2
    static void incTimeval(struct timeval &result, const struct timeval &tv);
    //result += tv
    static void subTimevals(struct timeval &result, const struct timeval &tv1,
                            const struct timeval &tv2); //result = tv2 - tv1
    static double relTimeFrac(const struct timeval &t, const struct timeval &t0,
                              const struct timeval &t1); //returns (t - t0)/(t1 - t0), the fraction
    //of time within the range [t0,t1] mapped to range [0,1]
    static double relTimeFrac(const struct timeval &t);
    //overloaded function to use precomputed values, to improve
    //itterative performance
    static void setTimeRange(const struct timeval &tv0, const struct timeval &tv1);

};
#endif TIMEUTIL_H
