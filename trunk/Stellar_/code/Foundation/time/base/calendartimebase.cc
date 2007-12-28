//------------------------------------------------------------------------------
//  calendartimebase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "time/calendartime.h"

namespace Base
{
using namespace Timing;
using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
CalendarTimeBase::CalendarTimeBase() :
    year(1972),
    month(July),
    weekday(Saturday),
    day(1),
    hour(0),
    minute(0),
    second(0),
    milliSecond(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Formats a calendar time into a string using the following substitution
    string:

    {YEAR}          - the year member
    {MONTH}         - the month member
    {WEEKDAY}       - the weekday member
    {DAY}           - the numerical day-in-month member
    {HOUR}          - the hour member
    {MINUTE}        - the minute member
    {SECOND}        - the second member
    {MILLISECOND}   - the millisecond member
*/
String
CalendarTimeBase::Format(const String& fmtString, const CalendarTime& calTime)
{
    /*String str = fmtString;
    String yearString, monthString, weekdayString, dayString;
    String hourString, minuteString, secondString, milliSecondString;

    yearString = itoa(calTime.year);
    weekdayString = WeekdayToString(calTime.weekday);
    if (calTime.month < 10)
    {
        monthString = "0";
    }
    monthString.append(itoa(calTime.month));
    if (calTime.day < 10)
    {
        dayString = "0";
    }
    dayString.append(itoa(calTime.day));
    if (calTime.hour < 10)
    {
        hourString = "0";
    }
    hourString.append(itoa(calTime.hour));
    if (calTime.minute < 10)
    {
        minuteString = "0";
    }
    minuteString.append(itoa(calTime.minute));
    if (calTime.second < 10)
    {
        secondString = "0";
    }
    secondString.append(itoa(calTime.second));
    if (calTime.milliSecond < 10)
    {
        milliSecondString = "00";
    }
    else if (calTime.milliSecond < 100)
    {
        milliSecondString.append("0");
    }
    milliSecondString.append(itoa(calTime.milliSecond));

    str.SubstituteString("{YEAR}", yearString.c_str());
    str.SubstituteString("{MONTH}", monthString.c_str());
    str.SubstituteString("{WEEKDAY}", weekdayString.c_str());
    str.SubstituteString("{DAY}", dayString.c_str());
    str.SubstituteString("{HOUR}", hourString.c_str());
    str.SubstituteString("{MINUTE}", minuteString.c_str());
    str.SubstituteString("{SECOND}", secondString.c_str());
    str.SubstituteString("{MILLISECOND}", milliSecondString.c_str());*/

	String str = "null";
    return str;
}

//------------------------------------------------------------------------------
/**
*/
String
CalendarTimeBase::MonthToString(Month m)
{
    switch (m)
    {
        case January:   return "January";
        case February:  return "February";
        case March:     return "March";
        case April:     return "April";
        case May:       return "May";
        case June:      return "June";
        case July:      return "July";
        case August:    return "August";
        case September: return "September";
        case October:   return "October";
        case November:  return "November";
        case December:  return "December";
        default:        return "";
    }
}

//------------------------------------------------------------------------------
/**
*/
CalendarTime::Month
CalendarTimeBase::StringToMonth(const String& str)
{
    if (str == "January") return January;
    else if (str == "February") return February;
    else if (str == "March") return March;
    else if (str == "April") return April;
    else if (str == "May") return May;
    else if (str == "June") return June;
    else if (str == "July") return July;
    else if (str == "August") return August;
    else if (str == "September") return September;
    else if (str == "October") return October;
    else if (str == "November") return November;
    else if (str == "December") return December;
    else
    {
        s_error("CalendarTime::StringToMonth(): invalid month string '%s'!", str.c_str());
        return January;
    }
}

//------------------------------------------------------------------------------
/**
*/
String
CalendarTimeBase::WeekdayToString(Weekday d)
{
    switch (d)
    {
        case Sunday:    return "Sunday";
        case Monday:    return "Monday";
        case Tuesday:   return "Tuesday";
        case Wednesday: return "Wednesday";
        case Thursday:  return "Thursday";
        case Friday:    return "Friday";
        case Saturday:  return "Saturday";
        default:        return "";
    }
}

//------------------------------------------------------------------------------
/**
*/
CalendarTime::Weekday
CalendarTimeBase::StringToWeekday(const String& str)
{
    if (str == "Sunday") return Sunday;
    else if (str == "Monday") return Monday;
    else if (str == "Tuesday") return Tuesday;
    else if (str == "Wednesday") return Wednesday;
    else if (str == "Thursday") return Thursday;
    else if (str == "Friday") return Friday;
    else if (str == "Saturday") return Saturday;
    else
    {
        s_error("CalendarTime::StringToWeekday(): invalid weekday string '%s'!", str.c_str());
        return Sunday; 
    }
}

//------------------------------------------------------------------------------
/**
*/
void
CalendarTimeBase::SetYear(Year y)
{
    this->year = y;
}

//------------------------------------------------------------------------------
/**
*/
CalendarTime::Year
CalendarTimeBase::GetYear() const
{
    return this->year;
}

//------------------------------------------------------------------------------
/**
*/
void
CalendarTimeBase::SetMonth(Month m)
{
    this->month = m;
}

//------------------------------------------------------------------------------
/**
*/
CalendarTime::Month
CalendarTimeBase::GetMonth() const
{
    return this->month;
}

//------------------------------------------------------------------------------
/**
*/
void
CalendarTimeBase::SetWeekday(Weekday d)
{
    this->weekday = d;
}

//------------------------------------------------------------------------------
/**
*/
CalendarTime::Weekday
CalendarTimeBase::GetWeekday() const
{
    return this->weekday;
}

//------------------------------------------------------------------------------
/**
*/
void
CalendarTimeBase::SetDay(Day d)
{
    this->day = d;
}

//------------------------------------------------------------------------------
/**
*/
CalendarTime::Day
CalendarTimeBase::GetDay() const
{
    return this->day;
}

//------------------------------------------------------------------------------
/**
*/
void
CalendarTimeBase::SetHour(Hour h)
{
    this->hour = h;
}

//------------------------------------------------------------------------------
/**
*/
CalendarTime::Hour
CalendarTimeBase::GetHour() const
{
    return this->hour;
}

//------------------------------------------------------------------------------
/**
*/
void
CalendarTimeBase::SetMinute(Minute m)
{
    this->minute = m;
}

//------------------------------------------------------------------------------
/**
*/
CalendarTime::Minute
CalendarTimeBase::GetMinute() const
{
    return this->minute;
}

//------------------------------------------------------------------------------
/**
*/
void
CalendarTimeBase::SetSecond(Second s)
{
    this->second = s;
}

//------------------------------------------------------------------------------
/**
*/
CalendarTime::Second
CalendarTimeBase::GetSecond() const
{
    return this->second;
}

//------------------------------------------------------------------------------
/**
*/
void
CalendarTimeBase::SetMilliSecond(MilliSecond m)
{
    this->milliSecond = m;
}

//------------------------------------------------------------------------------
/**
*/
CalendarTime::MilliSecond
CalendarTimeBase::GetMilliSecond() const
{
    return this->milliSecond;
}

} // namespace Base
