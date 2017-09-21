/*******************************************************************
InetVis - Internet Visualisation
Version: 2.1.0
release date: 2017/09/21

Original Authors: Jean-Pierre van Riel, Barry Irwin
Initvis 2.x Authors: Yestin Johnson, Barry Irwin
Rhodes University
Computer Science Honours Project - 2005
Computer Science Masters Project - 2006/7
Computer Science Masters Project - 2017
author: Jean-Pierre van Riel
supervisor: Barry Irwin

InetVis - Internet Visualisation for network traffic.
Copyright (C) 2006-2017, Jean-Pierre van Riel, Barry Irwin, Yestin Johnson

*******************************************************************/

#include "timeutil.h"

struct timeval TimeUtil::timeRange0;
struct timeval TimeUtil::timeRange1;
struct timeval TimeUtil::timeRangeLength;
double TimeUtil::dTimeRangeLength = 0.0;


void TimeUtil::qDateTimeToTimeval(struct timeval &tv,
                                  const QDateTime &qDateTime)
{
    tv.tv_sec = qDateTime.toTime_t();
    //note 2^31 allows for aproximatly 68 years from 1970, so this should be a
    //valid operation till 2038
    tv.tv_usec = qDateTime.time().msec() * 1000;

}


void TimeUtil::timevalToQDateTime(QDateTime &qDateTime,
                                  const struct timeval &tv)
{
    qDateTime.setTime_t(tv.tv_sec);
    qDateTime.setTime(qDateTime.time().addMSecs((int)(tv.tv_usec / 1000.0)));

}


QString TimeUtil::realativeTimevalToQString(const struct timeval &tv)
{
    int d = 0;
    int h = 0;
    int m = 0;
    int s = 0;
    int ms = 0;
    QString retString = QString();

    //calculate values HOUR
    d = (int)(tv.tv_sec / TV_DAY); //truncates
    h = (int)((tv.tv_sec % TV_DAY) / TV_HOUR); //truncates
    m = (int)((tv.tv_sec % TV_HOUR) / TV_MIN); //truncates
    s = tv.tv_sec % 60;
    ms = (int)(tv.tv_usec / 1000.0); //millisec - simple truncation

    //now construct and report values as a string
    if (d > 0)
    {  //dont output day if days are 0
        retString.append(QString::number(d) + " days, ");
    }
    if (h < 10)
    {  retString.append('0');
    }
    retString.append(QString::number(h) + ':');
    if (m < 10)
    {  retString.append('0');
    }
    retString.append(QString::number(m) + ':');
    if (s < 10)
    {  retString.append('0');
    }
    retString.append(QString::number(s) + ':');
    if (ms < 100)
    {  retString.append('0');
    }
    if (ms < 10)
    {  retString.append('0');
    }
    retString.append(QString::number(ms));

    return retString;

}


void TimeUtil::doubleToTimeval(struct timeval &result, double d)
{
    //this should be a pretty fast conversion, but watch out for overflow -
    //getting negative numbers out is a sign the value is too big - keeping these
    //values small enough is the responsibility of the caller function
    result.tv_sec = (int)(d); //truncate
    result.tv_usec = (int)((d - result.tv_sec) * 1000000.0); //get remainder factor

}


double TimeUtil::timevalToDouble(const struct timeval &tv)
{
    return (tv.tv_sec + (tv.tv_usec / 1000000.0));

}


double TimeUtil::qTimeToDouble(const QTime &qTime)
{
    double time = 0.0;
    time += qTime.hour() * TV_HOUR;
    time += qTime.minute() * TV_MIN;
    time += qTime.second();
    time += qTime.msec() / 1000.0;
    return time;

}


void TimeUtil::doubleToQTime(QTime &time, const double d)
{
    //TODO: make more efficient...
    //if longer than a day, truncate

    double integerPart; //just a placeholder for decomosition by modf
    int millisec = (int)((modf(d, &integerPart)) * 1000.0);
    if (integerPart >= TV_DAY)
    {  integerPart = fmod(integerPart, TV_DAY);
    }
    int hours = (int)(integerPart / TV_HOUR);
    integerPart -= hours * TV_HOUR;
    int minutes = (int)(integerPart / TV_MIN);
    integerPart -= minutes * TV_MIN;
    int seconds = (int)(integerPart);
    time.setHMS(hours, minutes, seconds, millisec);

}


bool TimeUtil::timevalGreaterThan(const struct timeval &tv1,
                                  const struct timeval &tv2)
{
    //tv1 > tv2 ?
    if (tv1.tv_sec > tv2.tv_sec)
    {  return true; //no need to check usec
    }
    else
    {  if (tv1.tv_sec < tv2.tv_sec)
        {  return false; //obviously false, no need to check usec
        }
        else
        {  //sec must be equal, check usec
            if (tv1.tv_usec > tv2.tv_usec)
            {  return true;
            }
            else
            {  return false;
            }
        }
    }

}


void TimeUtil::addTimevals(struct timeval &result,
                           const struct timeval &tv1, const struct timeval &tv2)
{
    result.tv_sec = tv1.tv_sec + tv2.tv_sec;
    int useconds = tv1.tv_usec + tv2.tv_usec;
    if (useconds >= 1000000) //carry over
    {  result.tv_sec++;
        result.tv_usec = useconds - 1000000;
    }
    else
        result.tv_usec = useconds;

}


void TimeUtil::incTimeval(struct timeval &result, const struct timeval &tv)
{
    result.tv_sec += tv.tv_sec;
    int useconds = result.tv_usec + tv.tv_usec;
    if (useconds >= 1000000) //carry over
    {  result.tv_sec++;
        result.tv_usec = (useconds - 1000000);
    }
    else
        result.tv_usec = useconds;

}


void TimeUtil::subTimevals(struct timeval &result,
                           const struct timeval &tv1, const struct timeval &tv2)
{
    result.tv_sec = tv2.tv_sec - tv1.tv_sec;
    int useconds = tv2.tv_usec - tv1.tv_usec;
    if (useconds < 0) //carry over
    {  result.tv_sec--;
        result.tv_usec = useconds + 1000000;
    }
    else
        result.tv_usec = useconds;

}


double TimeUtil::relTimeFrac(const struct timeval &t, const struct timeval &t0,
                             const struct timeval &t1)
{
    //returns (t - t0)/(t1 - t0), the fraction of time within the range [t0,t1]
    //mapped to range [0,1]
    struct timeval divedend, divisor;
    subTimevals(divedend, t, t0); //(t - t0)
    subTimevals(divisor, t1, t0); //(t1 - t0)
    return timevalToDouble(divedend)/timevalToDouble(divisor); //(t - t0)/(t1 - t0)

}


double TimeUtil::relTimeFrac(const struct timeval &t)
{
    //returns (t - t0)/(t1 - t0), the fraction of time within the range [t0,t1]
    //mapped to range [0,1]
    struct timeval divedend;
    subTimevals(divedend, t, timeRange0); //(t - timeRange0)
    return (timevalToDouble(divedend)/dTimeRangeLength);
    //(t - timeRange0)/timeRangeLength

}


void TimeUtil::setTimeRange(const struct timeval &t0, const struct timeval &t1)
{
    //copy range data
    timeRange0.tv_sec = t0.tv_sec;
    timeRange0.tv_usec = t0.tv_usec;
    timeRange1.tv_sec = t1.tv_sec;
    timeRange1.tv_usec = t1.tv_usec;

    //calculate rangelength
    subTimevals(timeRangeLength, timeRange0, timeRange1); //(t1 - t0)
    dTimeRangeLength = timevalToDouble(timeRangeLength);

}
