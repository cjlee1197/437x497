/*Created by J.Wong 2016/9/4 9:40:58*/

#include "datetime.h"
#include <locale.h>
#include <cmath>
#include <time.h>
#include <assert.h>
#include <cstdlib>
#include <sys/time.h>
#include <stdio.h>


typedef long long int64;
typedef unsigned int uint;

enum {
    SECS_PER_DAY = 86400,
    MSECS_PER_DAY = 86400000,
    SECS_PER_HOUR = 3600,
    MSECS_PER_HOUR = 3600000,
    SECS_PER_MIN = 60,
    MSECS_PER_MIN = 60000,
    TIME_T_MAX = 2145916799,  		// int maximum 2037-12-31T23:59:59 UTC
    JULIAN_DAY_FOR_EPOCH = 2440588 // result of julianDayFromDate(1970, 1, 1)
};

static const char monthDays[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

static inline  int min(int m,int n)
{
	return m<n? m:n;	
}
static inline int max(int m, int n)
{
	return m<n? n:m;	
}

/*date static helper function*/

static inline CDate fixedDate(int y, int m, int d)
{
	CDate result(y,m,1);
	result.setDate(y, m, min(d, result.daysInMonth()));	
	return result;
}

static inline int64 floordiv(int64 a, int b)
{
    return (a - (a < 0 ? b-1 : 0)) / b;
}

static inline int floordiv(int a, int b)
{
    return (a - (a < 0 ? b-1 : 0)) / b;
}

static inline int64 julianDayFromDate(int year, int month, int day)
{
    // Adjust for no year 0
    if (year < 0)
        ++year;

/*
 * Math from The Calendar FAQ at http://www.tondering.dk/claus/cal/julperiod.php
 * This formula is correct for all julian days, when using mathematical integer
 * division (round to negative infinity), not c++11 integer division (round to zero)
 */
    int    a = floordiv(14 - month, 12);
    int64 y = (int64)year + 4800 - a;
    int    m = month + 12 * a - 3;
    return day + floordiv(153 * m + 2, 5) + 365 * y + floordiv(y, 4) - floordiv(y, 100) + floordiv(y, 400) - 32045;
}

struct ParsedDate
{
    int year, month, day;
};

static ParsedDate getDateFromJulianDay(int64 julianDay)
{
/*
 * Math from The Calendar FAQ at http://www.tondering.dk/claus/cal/julperiod.php
 * This formula is correct for all julian days, when using mathematical integer
 * division (round to negative infinity), not c++11 integer division (round to zero)
 */
    int64 a = julianDay + 32044;
    int64 b = floordiv(4 * a + 3, 146097);
    int    c = a - floordiv(146097 * b, 4);

    int    d = floordiv(4 * c + 3, 1461);
    int    e = c - floordiv(1461 * d, 4);
    int    m = floordiv(5 * e + 2, 153);

    int    day = e - floordiv(153 * m + 2, 5) + 1;
    int    month = m + 3 - 12 * floordiv(m, 10);
    int    year = 100 * b + d - 4800 + floordiv(m, 10);

    // Adjust for no year 0
    if (year <= 0)
        --year ;

    const ParsedDate result = { year, month, day };
    return result;
}


static std::string toStringIsoDate(int64 jd)
{
    const ParsedDate pd = getDateFromJulianDay(jd);
    if (pd.year >= 0 && pd.year <= 9999)
    {
    	char str[12];
    	sprintf(str,"%04d-%02d-%02d", pd.year, pd.month, pd.day);
        return std::string(str);
     }
    else
        return std::string();
}

static	std::string toStringIsoTime(const CTime& time)
{
	if(!time.isValid())	return std::string();
	char str[12];
    sprintf(str,"%02d:%02d:%02d", time.hour(), time.minute(), time.second());
    return std::string(str);  	
}


// Converts an msecs value into a date and time
static void msecsToTime(int64 msecs, CDate *date, CTime *time)
{
    int64 jd = JULIAN_DAY_FOR_EPOCH;
    int64 ds = 0;

    if (llabs(msecs) >= MSECS_PER_DAY) {
        jd += (msecs / MSECS_PER_DAY);
        msecs %= MSECS_PER_DAY;
    }

    if (msecs < 0) {
        ds = MSECS_PER_DAY - msecs - 1;
        jd -= ds / MSECS_PER_DAY;
        ds = ds % MSECS_PER_DAY;
        ds = MSECS_PER_DAY - ds - 1;
    } else {
        ds = msecs;
    }

    if (date)
        *date = CDate::fromJulianDay(jd);
    if (time)
        *time = CTime::fromMSecsSinceStartOfDay(ds);
}

static void gm_tzset()
{
    tzset();
}

static int gm_timezone()
{
     return timezone;
}

static std::string gm_tzname(CDateTimePrivate::DaylightStatus daylightStatus)
{

    int isDst = (daylightStatus == CDateTimePrivate::DaylightTime) ? 1 : 0;
    return std::string();
}

// Calls the platform variant of mktime for the given date, time and daylightStatus,
// and updates the date, time, daylightStatus and abbreviation with the returned values
// If the date falls outside the 1970 to 2037 range supported by mktime / time_t
// then null date/time will be returned, you should adjust the date first if
// you need a guaranteed result.
static int64 gm_mktime(CDate *date, CTime *time, CDateTimePrivate::DaylightStatus *daylightStatus,
                        std::string *abbreviation, bool *ok = 0)
{
    const int64 msec = time->msec();
    int yy, mm, dd;
    date->getDate(&yy, &mm, &dd);
    tm local;
    local.tm_sec = time->second();
    local.tm_min = time->minute();
    local.tm_hour = time->hour();
    local.tm_mday = dd;
    local.tm_mon = mm - 1;
    local.tm_year = yy - 1900;
    local.tm_wday = 0;
    local.tm_yday = 0;
    if (daylightStatus)
        local.tm_isdst = int(*daylightStatus);
    else
        local.tm_isdst = -1;
    time_t secsSinceEpoch = mktime(&local);
    if (secsSinceEpoch != time_t(-1)) {
        *date = CDate(local.tm_year + 1900, local.tm_mon + 1, local.tm_mday);
        *time = CTime(local.tm_hour, local.tm_min, local.tm_sec, msec);
        if (local.tm_isdst >= 1) {
            if (daylightStatus)
                *daylightStatus = CDateTimePrivate::DaylightTime;
            if (abbreviation)
                *abbreviation = gm_tzname(CDateTimePrivate::DaylightTime);
        } else if (local.tm_isdst == 0) {
            if (daylightStatus)
                *daylightStatus = CDateTimePrivate::StandardTime;
            if (abbreviation)
                *abbreviation = gm_tzname(CDateTimePrivate::StandardTime);
        } else {
            if (daylightStatus)
                *daylightStatus = CDateTimePrivate::UnknownDaylightTime;
            if (abbreviation)
                *abbreviation = gm_tzname(CDateTimePrivate::StandardTime);
        }
        if (ok)
            *ok = true;
    } else {
        *date = CDate();
        *time = CTime();
        if (daylightStatus)
            *daylightStatus = CDateTimePrivate::UnknownDaylightTime;
        if (abbreviation)
            *abbreviation =std::string();
        if (ok)
            *ok = false;
    }
	struct timeval tv; 
    tv.tv_sec=(int)(secsSinceEpoch);
    tv.tv_usec=msec*1000;
    settimeofday(&tv,NULL);
    return ((int64)secsSinceEpoch * 1000) + msec;
}

// Calls the platform variant of localtime for the given msecs, and updates
// the date, time, and daylight status with the returned values.
static bool gm_localtime(int64 msecsSinceEpoch, CDate *localDate, CTime *localTime,
                         CDateTimePrivate::DaylightStatus *daylightStatus)
{
    const time_t secsSinceEpoch = msecsSinceEpoch / 1000;
    const int msec = msecsSinceEpoch % 1000;

    tm local;
    bool valid = false;
    tm *res = 0;
    res = localtime(&secsSinceEpoch);
    if (res) {
        local = *res;
        valid = true;
    }
    if (valid) {
        *localDate = CDate(local.tm_year + 1900, local.tm_mon + 1, local.tm_mday);
        *localTime = CTime(local.tm_hour, local.tm_min, local.tm_sec, msec);
        if (daylightStatus) {
            if (local.tm_isdst > 0)
                *daylightStatus = CDateTimePrivate::DaylightTime; 
            else if (local.tm_isdst < 0)
                *daylightStatus = CDateTimePrivate::UnknownDaylightTime;
            else
                *daylightStatus = CDateTimePrivate::StandardTime;
        }
        return true;
    } else {
        *localDate = CDate();
        *localTime = CTime();
        if (daylightStatus)
            *daylightStatus = CDateTimePrivate::UnknownDaylightTime;
        return false;
    }
}

// Converts a date/time value into msecs
static int64 timeToMSecs(const CDate &date, const CTime &time)
{
    return ((date.toJulianDay() - JULIAN_DAY_FOR_EPOCH) * MSECS_PER_DAY)
           + time.msecsSinceStartOfDay();
}

// Convert an MSecs Since Epoch into Local Time
static bool epochMSecsToLocalTime(int64 msecs, CDate *localDate, CTime *localTime,
                                  CDateTimePrivate::DaylightStatus *daylightStatus = 0)
{
    if (msecs < 0) {
        // Docs state any LocalTime before 1970-01-01 will *not* have any Daylight Time applied
        // Instead just use the standard offset from UTC to convert to UTC time
        gm_tzset();
        msecsToTime(msecs - gm_timezone() * 1000, localDate, localTime);
        if (daylightStatus)
            *daylightStatus = CDateTimePrivate::StandardTime;
        return true;
    } else if (msecs > (int64(TIME_T_MAX) * 1000)) {
        // Docs state any LocalTime after 2037-12-31 *will* have any Daylight Time applied
        // but this may fall outside the supported time_t range, so need to fake it.
        // Use existing method to fake the conversion, but this is deeply flawed as it may
        // apply the conversion from the wrong day number
        CDate utcDate;
        CTime utcTime;
        msecsToTime(msecs, &utcDate, &utcTime);
        int year, month, day;
        utcDate.getDate(&year, &month, &day);
        // 2037 is not a leap year, so make sure date isn't Feb 29
        if (month == 2 && day == 29)
            --day;
        CDate fakeDate(2037, month, day);
        int64 fakeMsecs = CDateTime(fakeDate, utcTime).toMSecsSinceEpoch();
        bool res = gm_localtime(fakeMsecs, localDate, localTime, daylightStatus);
        *localDate = localDate->addDays(fakeDate.daysTo(utcDate));
        return res;
    } else {
        // Falls inside time_t suported range so can use localtime
        return gm_localtime(msecs, localDate, localTime, daylightStatus);
    }
}

// Convert a LocalTime expressed in local msecs encoding and the corresponding
// daylight status into a UTC epoch msecs. Optionally populate the returned
// values from mktime for the adjusted local date and time.

static int64 localMSecsToEpochMSecs(int64 localMsecs,
                                     CDateTimePrivate::DaylightStatus *daylightStatus,
                                     CDate *localDate = 0, CTime *localTime = 0,
                                     std::string *abbreviation = 0)
{
    CDate dt;
    CTime tm;
    msecsToTime(localMsecs, &dt, &tm);

    int64 msecsMax = int64(TIME_T_MAX) * 1000;

    if (localMsecs <= int64(MSECS_PER_DAY)) {

        // Docs state any LocalTime before 1970-01-01 will *not* have any Daylight Time applied

        // First, if localMsecs is within +/- 1 day of minimum time_t try mktime in case it does
        // fall after minimum and needs proper daylight conversion
        if (localMsecs >= -int64(MSECS_PER_DAY)) {
            bool valid;
            int64 utcMsecs = gm_mktime(&dt, &tm, daylightStatus, abbreviation, &valid);
            if (valid && utcMsecs >= 0) {
                // mktime worked and falls in valid range, so use it
                if (localDate)
                    *localDate = dt;
                if (localTime)
                    *localTime = tm;
                return utcMsecs;
            }
        } else {
            // If we don't call mktime then need to call tzset to get offset
            gm_tzset();
        }
        // Time is clearly before 1970-01-01 so just use standard offset to convert
        int64 utcMsecs = localMsecs + gm_timezone() * 1000;
        if (localDate || localTime)
            msecsToTime(localMsecs, localDate, localTime);
        if (daylightStatus)
            *daylightStatus = CDateTimePrivate::StandardTime;
        if (abbreviation)
            *abbreviation = gm_tzname(CDateTimePrivate::StandardTime);
        return utcMsecs;

    } else if (localMsecs >= msecsMax - MSECS_PER_DAY) {

        // Docs state any LocalTime after 2037-12-31 *will* have any Daylight Time applied
        // but this may fall outside the supported time_t range, so need to fake it.

        // First, if localMsecs is within +/- 1 day of maximum time_t try mktime in case it does
        // fall before maximum and can use proper daylight conversion
        if (localMsecs <= msecsMax + MSECS_PER_DAY) {
            bool valid;
            int64 utcMsecs = gm_mktime(&dt, &tm, daylightStatus, abbreviation, &valid);
            if (valid && utcMsecs <= msecsMax) {
                // mktime worked and falls in valid range, so use it
                if (localDate)
                    *localDate = dt;
                if (localTime)
                    *localTime = tm;
                return utcMsecs;
            }
        }
        // Use existing method to fake the conversion, but this is deeply flawed as it may
        // apply the conversion from the wrong day number, e.g. if rule is last Sunday of month
        // TODO Use QTimeZone when available to apply the future rule correctly
        int year, month, day;
        dt.getDate(&year, &month, &day);
        // 2037 is not a leap year, so make sure date isn't Feb 29
        if (month == 2 && day == 29)
            --day;
        CDate fakeDate(2037, month, day);
        int64 fakeDiff = fakeDate.daysTo(dt);
        int64 utcMsecs = gm_mktime(&fakeDate, &tm, daylightStatus, abbreviation);
        if (localDate)
            *localDate = fakeDate.addDays(fakeDiff);
        if (localTime)
            *localTime = tm;
        CDate utcDate;
        CTime utcTime;
        msecsToTime(utcMsecs, &utcDate, &utcTime);
        utcDate = utcDate.addDays(fakeDiff);
        utcMsecs = timeToMSecs(utcDate, utcTime);
        return utcMsecs;

    } else {

        // Clearly falls inside 1970-2037 suported range so can use mktime
        int64 utcMsecs = gm_mktime(&dt, &tm, daylightStatus, abbreviation);
        if (localDate)
            *localDate = dt;
        if (localTime)
            *localTime = tm;
        return utcMsecs;

    }
}

/*CDate member	functions */

CDate::CDate(int y, int m, int d)
{
    setDate(y, m, d);
}

int 	CDate::year()const
{
	if(isNull())	return 0;
	return getDateFromJulianDay(m_jd).year;
}

int 	CDate::month()const
{
	if(isNull())	return 0;
	return getDateFromJulianDay(m_jd).month;
}

int 	CDate::day()const
{
	if(isNull())	return 0;
	return getDateFromJulianDay(m_jd).day;
}

int 	CDate::dayOfWeek()const
{
	if(isNull())	return 0;
	if(m_jd>=0)
		return (m_jd%7)+1;
	else
		return ((m_jd+1)%7)+7;
}

int 	CDate::dayOfYear()const
{
	if(isNull())	return 0;
	return m_jd-julianDayFromDate(year(),1,1)+1;
}

int 	CDate::daysInMonth()const
{
	if(isNull())	return 0;
	ParsedDate pd=getDateFromJulianDay(m_jd);
	if(pd.month==2&&isLeapYear(pd.year))
	{
		return 29;	
	}
	return monthDays[pd.month];
}

int 	CDate::daysInYear()const
{
	if(isNull())	return 0;
	return isLeapYear(getDateFromJulianDay(m_jd).year) ? 366 : 365;
}

int		CDate::weekNumbers(int* yearNum)const
{
	if(isNull())	return 0;
	
	int year=CDate::year();
	int yday=dayOfYear();
	int wday=dayOfWeek();
	
	int week = (yday - wday + 10) / 7;

    if (week == 0) {
        // last week of previous year
        --year;
        week = (yday + 365 + (isLeapYear(year) ? 1 : 0) - wday + 10) / 7;
        assert(week == 52 || week == 53);
    } else if (week == 53) {
        // maybe first week of next year
        int w = (yday - 365 - (isLeapYear(year + 1) ? 1 : 0) - wday + 10) / 7;
        if (w > 0) {
            ++year;
            week = w;
        }
        assert(week == 53 || week == 1);
    }

    if (yearNum != 0)
        *yearNum = year;
    return week;
}

std::string 	CDate::shortMonthName(int month, MonthNameType type)
{
	
}

std::string 	CDate::shortDayName(int weekday, MonthNameType type)
{
	
}

std::string 	CDate::longMonthName(int month, MonthNameType type)
{
	
}

std::string 	CDate::longDayName(int weekday, MonthNameType type)
{
	
}

CDate CDate::currentDate()
{
	return CDateTime::currentDateTime().date();
}

CDate fromString(const std::string &s, Gemini::DateFormat f)
{
	
}

CDate CDate::fromString(const std::string &s, const std::string &format)
{
	
}

std::string CDate::toString(Gemini::DateFormat f) const
{
	std::string str;
	switch(f)
	{
		case Gemini::TextDate:
			str=std::string();
			break;
		case Gemini::IsoDate:
		 str=toStringIsoDate(m_jd);
		 	break;
		 default:
		 	str=std::string();
		 	break;	
	}
	return str;	
}
std::string 	CDate::toString(const std::string &format) const
{
	
}

bool   	CDate::setDate(int year, int month, int day)
{
	 if (isValid(year, month, day))
        m_jd = julianDayFromDate(year, month, day);
    else
        m_jd = nullJd();

    return isValid();
}

void   	CDate::getDate(int* year, int* month, int* day)
{
	ParsedDate pd = { 0, 0, 0 };
    if (isValid())
        pd = getDateFromJulianDay(m_jd);

    if (year)
        *year = pd.year;
    if (month)
        *month = pd.month;
    if (day)
        *day = pd.day;
}

CDate	CDate::addDays(int days)const
{
	if (isNull())
        return CDate();

    return fromJulianDay(m_jd + days);
}

CDate	CDate::addMonths(int months)const
{
	 if (isNull())
        return CDate();
    if (!months)
        return *this;

    int old_y, y, m, d;
    {
        const ParsedDate pd = getDateFromJulianDay(m_jd);
        y = pd.year;
        m = pd.month;
        d = pd.day;
    }
    old_y = y;

    bool increasing = months > 0;

    while (months != 0) {
        if (months < 0 && months + 12 <= 0) {
            y--;
            months+=12;
        } else if (months < 0) {
            m+= months;
            months = 0;
            if (m <= 0) {
                --y;
                m += 12;
            }
        } else if (months - 12 >= 0) {
            y++;
            months -= 12;
        } else if (m == 12) {
            y++;
            m = 0;
        } else {
            m += months;
            months = 0;
            if (m > 12) {
                ++y;
                m -= 12;
            }
        }
    }

    // was there a sign change?
    if ((old_y > 0 && y <= 0) ||
        (old_y < 0 && y >= 0))
        // yes, adjust the date by +1 or -1 years
        y += increasing ? +1 : -1;

    return fixedDate(y, m, d);
}

CDate	CDate::addYears(int years)const
{
	if (!isValid())
        return CDate();

    ParsedDate pd = getDateFromJulianDay(m_jd);

    int old_y = pd.year;
    pd.year += years;

    // was there a sign change?
    if ((old_y > 0 && pd.year <= 0) ||
        (old_y < 0 && pd.year >= 0))
        // yes, adjust the date by +1 or -1 years
        pd.year += years > 0 ? +1 : -1;

    return fixedDate(pd.year, pd.month, pd.day);
}

int64	CDate::daysTo(const CDate& date)const
{
	if (isNull() || date.isNull())
        return 0;
    return date.m_jd - m_jd;
}

bool CDate::isLeapYear(int y)
{
    // No year 0 in Gregorian calendar, so -1, -5, -9 etc are leap years
    if ( y < 1)
        ++y;

    return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0;
}

bool CDate::isValid(int year, int month, int day)
{
    // there is no year 0 in the Gregorian calendar
    if (year == 0)
        return false;

    return (day > 0 && month > 0 && month <= 12) &&
           (day <= monthDays[month] || (day == 29 && month == 2 && isLeapYear(year)));
}

std::ostream& operator<<(std::ostream& os, const CDate& date)
{
	os<<"date( "<<date.toString(Gemini::IsoDate)<<" )";
	return os;
}

std::istream& operator>>(std::istream& is, CDate& date)
{
	int year, month, day;
	is>>year>>month>>day;
	date.setDate(year,month,day);
	return is;
}


/*CTime	member functions*/

CTime::CTime(int h, int m, int s, int ms)
{
	setHMS(h,m,s,ms);
}

bool	CTime::isValid()const
{
	 return m_mds > NullTime && m_mds < MSECS_PER_DAY;
}
		
int		CTime::hour()const
{
	if(!isValid())	return -1;
	return ds()/MSECS_PER_HOUR;
}

int 	CTime::minute()const
{
	if(!isValid())	return -1;
	return (ds()%MSECS_PER_HOUR)/MSECS_PER_MIN;
}

int		CTime::second()const
{
	if(!isValid())	return -1;
	return ds()/1000%SECS_PER_MIN;
}

int		CTime::msec()const
{
	if(!isValid())	return -1;
	return ds()%1000;
}

std::string CTime::toString(Gemini::DateFormat f ) const
{
	return toStringIsoTime(*this);
}

std::string CTime::toString(const std::string &format) const
{
	
}    		

bool 	CTime::setHMS(int h, int m, int s, int ms)
{
	if(!isValid(h,m,s,ms))
	{
		m_mds=NullTime;
		return false;
	}
	m_mds=h*MSECS_PER_HOUR+m*MSECS_PER_MIN+s*1000+ms;
	return true;
}

CTime	CTime::addSecs(int s)const
{
	s%=SECS_PER_DAY;
	addMsecs(s*1000);
}

int		CTime::secsTo(CTime& time)const
{
	if(!isValid()||!time.isValid())	return 0;
	int ourSeconds=ds()/1000;
	int	theirSeconds=ds()/1000;
	return theirSeconds-ourSeconds;
}

CTime	CTime::addMsecs(int ms)const
{
	CTime t;
	if(isValid())
	{
		if (ms < 0) {
            int negdays = (MSECS_PER_DAY - ms) / MSECS_PER_DAY;
            t.m_mds = (ds() + ms + negdays * MSECS_PER_DAY) % MSECS_PER_DAY;
        } else {
            t.m_mds = (ds() + ms) % MSECS_PER_DAY;
        }	
	}
	return t;
}

int		CTime::msecsTo(CTime& time)const
{
	if (!isValid() || !time.isValid())
        return 0;
    return time.ds() - ds();
}


CTime 	CTime::currentTime()
{
	return CDateTime::currentDateTime().time();
}

CTime 	CTime::fromString(const std::string &s, Gemini::DateFormat f)
{
	
}

CTime 	CTime::fromString(const std::string &s, const std::string &format)
{
	
}

bool 	CTime::isValid(int h, int m, int s, int ms)
{
	return (uint)h < 24 && (uint)m < 60 && (uint)s < 60 && (uint)ms < 1000;
}

void 	CTime::start()
{
	 *this = currentTime();
}

int 	CTime::restart()
{
	CTime t = currentTime();
    int n = msecsTo(t);
    if (n < 0)                                // passed midnight
        n += SECS_PER_DAY*1000;
    *this = t;
    return n;
}
int 	CTime::elapsed() const
{
	CTime t=currentTime();
	int n = msecsTo(t);
    if (n < 0)                                // passed midnight
        n += SECS_PER_DAY * 1000;
    return n;
}
    	
std::ostream &operator<<(std::ostream& os, const CTime& time)
{
	os<<"time( "<<toStringIsoTime(time)<<" )";
	return os;
}

std::istream &operator>>(std::istream& is, CTime& time)
{
	int hour, minute, secs, msecs;
	is>>hour>>minute>>secs>>msecs;
	time.setHMS(hour,minute,secs,msecs);
	return is;
}


CDateTimePrivate::CDateTimePrivate(const CDate &toDate, const CTime &toTime,int offsetSeconds):
m_msecs(0),
m_status(StatusFlag(0)),
m_offsetFromUtc(offsetSeconds)
{
		clearValidDateTime();
		clearSetToDaylightStatus();
		setDateTime(toDate,toTime);
}	
void 	CDateTimePrivate::setDateTime(const CDate &date, const CTime &time)
{
	// If the date is valid and the time is not we set time to 00:00:00
    CTime useTime = time;
    if (!useTime.isValid() && date.isValid())
        useTime = CTime::fromMSecsSinceStartOfDay(0);

   StatusFlag newStatus;

    // Set date value and status
    int64 days = 0;
    if (date.isValid()) {
        days = date.toJulianDay() - JULIAN_DAY_FOR_EPOCH;
        newStatus = ValidDate;
    } else if (date.isNull()) {
        newStatus = NullDate;
    }

    // Set time value and status
    int ds = 0;
    if (useTime.isValid()) {
        ds = useTime.msecsSinceStartOfDay();
        newStatus = StatusFlag(newStatus|ValidTime);
    } else if (time.isNull()) {
        newStatus= StatusFlag(newStatus|NullTime);
    }

    // Set msecs serial value
    m_msecs = (days * MSECS_PER_DAY) + ds;
    m_status = newStatus;
    
    // Set if date and time are valid
    checkValidDateTime();
}

std::pair<CDate, CTime> CDateTimePrivate::getDateTime() const
{
	std::pair<CDate, CTime> result;
    msecsToTime(m_msecs, &result.first, &result.second);

    if (isNullDate())
        result.first = CDate();

    if (isNullTime())
        result.second = CTime();

    return result;
}

void 	CDateTimePrivate::setDaylightStatus(DaylightStatus status)
{
	 if (status == DaylightTime) {
        m_status = StatusFlag(m_status & ~SetToStandardTime);
        m_status = StatusFlag(m_status | SetToDaylightTime);
    } else if (status == StandardTime) {
        m_status = StatusFlag(m_status & ~SetToDaylightTime);
        m_status = StatusFlag(m_status | SetToStandardTime);
    } else {
        clearSetToDaylightStatus();
    }
}

CDateTimePrivate::DaylightStatus 	CDateTimePrivate::daylightStatus() const
{
	if ((m_status & SetToDaylightTime) == SetToDaylightTime)
        return DaylightTime;
    if ((m_status & SetToStandardTime) == SetToStandardTime)
        return StandardTime;
    return UnknownDaylightTime;
}

inline int64 	CDateTimePrivate::toMSecsSinceEpoch() const
{
	 DaylightStatus status = daylightStatus();
     return localMSecsToEpochMSecs(m_msecs, &status);
}

void 	CDateTimePrivate::checkValidDateTime()
{
	 if (isValidDate() && isValidTime())
	 {
            setValidDateTime();
     		refreshDateTime();
     }
     else
     {
            clearValidDateTime();
            m_offsetFromUtc=0;
     }
     
}

void 	CDateTimePrivate::refreshDateTime()
{
	CDate testDate;
    CTime testTime;
    int64 epochMSecs = 0;
    DaylightStatus status = daylightStatus();
    epochMSecs = localMSecsToEpochMSecs(m_msecs, &status, &testDate, &testTime);
    if (timeToMSecs(testDate, testTime) == m_msecs) {
        setValidDateTime();
        m_offsetFromUtc = (m_msecs - epochMSecs) / 1000;
    } else {
        clearValidDateTime();
        m_offsetFromUtc = 0;
    }
}




/* datetime member functions*/

CDateTime::CDateTime()
{
	
}

CDateTime::CDateTime(const CDate& dt)
{
	d.setDateTime(dt,CTime(0,0,0));
}

CDateTime::CDateTime(const CDate& dt, const CTime& t)
{
	d.setDateTime(dt,t);
}

CDateTime::CDateTime(const CDateTime &other)
{
	d=other.d;
}

CDateTime::~CDateTime()
{
	
}

CDateTime&	CDateTime::operator=(const CDateTime& other)
{
	d=other.d;
	return *this;
}
   		
   		
bool	CDateTime::isNull()const
{
	return d.isNullDate() && d.isNullTime();
}

bool	CDateTime::isValid()const
{
	return d.isValidDateTime();
}

CDate	CDateTime::date()const
{
	if (d.isNullDate())
        return CDate();
    CDate dt;
    msecsToTime(d.m_msecs, &dt, 0);
    return dt;
}

CTime	CDateTime::time()const
{
	if (d.isNullTime())
        return CTime();
    CTime tm;
    msecsToTime(d.m_msecs, 0, &tm);
    return tm;
}
  		
bool 	CDateTime::isDaylightTime() const
{
	return false;
}

int64 	CDateTime::toMSecsSinceEpoch() const
{
	 return d.toMSecsSinceEpoch();
}
    	
void 	CDateTime::setDate(const CDate &date)
{
	 d.setDateTime(date, time());
}

void 	CDateTime::setTime(const CTime &time)
{
	d.setDateTime(date(),time);
}

void 	CDateTime::setMSecsSinceEpoch(int64 msecs)
{
	 //localtime
	 d.m_status= CDateTimePrivate::StatusFlag(0);
	 CDate dt;
     CTime tm;
     CDateTimePrivate::DaylightStatus status;
     epochMSecsToLocalTime(msecs, &dt, &tm, &status);
     d.setDateTime(dt, tm);
     d.setDaylightStatus(status);
     d.refreshDateTime();
}
    	
std::string 	CDateTime::toString(Gemini::DateFormat f) const
{
	std::string str;
	switch(f)
	{
		case Gemini::TextDate:
			str=std::string();
			break;
		case Gemini::IsoDate:
		 str=date().toString(Gemini::IsoDate)+" "+time().toString(Gemini::IsoDate);
		 	break;
		 default:
		 	str=std::string();
		 	break;	
	}
	return str;	
}

std::string 	CDateTime::toString(const std::string &format) const
{
	
}
    		
CDateTime 		CDateTime::addDays(int64 days) const
{
	CDateTime dt(*this);
    std::pair<CDate, CTime> p = d.getDateTime();
    CDate &date = p.first;
    CTime &time = p.second;
    date = date.addDays(days);
    // Result might fall into "missing" DaylightTime transition hour,
    // so call conversion and use the adjusted returned time
    CDateTimePrivate::DaylightStatus status = d.daylightStatus();
    localMSecsToEpochMSecs(timeToMSecs(date, time), &status, &date, &time);
    dt.d.setDateTime(date, time);
    return dt;
}

CDateTime 		CDateTime::addMonths(int months) const
{
	CDateTime dt(*this);
    std::pair<CDate, CTime> p = d.getDateTime();
    CDate &date = p.first;
    CTime &time = p.second;
    date = date.addMonths(months);
    // Result might fall into "missing" DaylightTime transition hour,
    // so call conversion and use the adjusted returned time
    CDateTimePrivate::DaylightStatus status = d.daylightStatus();
    localMSecsToEpochMSecs(timeToMSecs(date, time), &status, &date, &time);
    dt.d.setDateTime(date, time);
    return dt;
}

CDateTime 		CDateTime::addYears(int years) const
{
	CDateTime dt(*this);
    std::pair<CDate, CTime> p = d.getDateTime();
    CDate &date = p.first;
    CTime &time = p.second;
    date = date.addYears(years);
    // Result might fall into "missing" DaylightTime transition hour,
    // so call conversion and use the adjusted returned time
    CDateTimePrivate::DaylightStatus status = d.daylightStatus();
    localMSecsToEpochMSecs(timeToMSecs(date, time), &status, &date, &time);
    dt.d.setDateTime(date, time);
    return dt;
}

CDateTime 		CDateTime::addSecs(int64 secs) const
{
	return addMSecs(secs * 1000);
}

CDateTime 		CDateTime::addMSecs(int64 msecs) const
{
	if (!isValid())
        return CDateTime();

    CDateTime dt(*this);
    dt.setMSecsSinceEpoch(d.toMSecsSinceEpoch() + msecs);
    return dt;
}
    	
int64 	CDateTime::daysTo(const CDateTime & other) const
{
	return date().daysTo(other.date());
}

int64 	CDateTime::secsTo(const CDateTime &other) const
{
	return (msecsTo(other) / 1000);
}

int64 	CDateTime::msecsTo(const CDateTime &other) const
{
	if (!isValid() || !other.isValid())
        return 0;
    return other.d.toMSecsSinceEpoch() - d.toMSecsSinceEpoch();
}
    	
bool 	CDateTime::operator==(const CDateTime &other) const
{
      return (d.m_msecs == other.d.m_msecs);
}

bool CDateTime::operator<(const CDateTime &other) const
{
	return (d.m_msecs < other.d.m_msecs);
}
    	
CDateTime 	CDateTime::currentDateTime()
{
	return fromMSecsSinceEpoch(currentMSecsSinceEpoch()); 
}

CDateTime 	CDateTime::fromString(const std::string &s, Gemini::DateFormat f)
{
	
}

CDateTime 	CDateTime::fromString(const std::string &s, const std::string &format)
{
	
}

CDateTime 	CDateTime::fromMSecsSinceEpoch(int64 msecs)
{
	CDateTime dt;
    dt.setMSecsSinceEpoch(msecs);
    return dt;
}

int64 		CDateTime::currentMSecsSinceEpoch()
{
	struct timeval tv;
    gettimeofday(&tv, 0);
    return int64(tv.tv_sec) * int64(1000LL) + tv.tv_usec / 1000;
}
    	
std::ostream &operator<<(std::ostream& os, const CDateTime& dt)
{
	os<<"datetime( "<<dt.date()<<" "<<dt.time()<<" )";
	return os;
}

std::istream &operator>>(std::istream& is, CDateTime& dt)
{
	CDate d;
	CTime t;
	is>>d>>t;
	dt=CDateTime(d,t);
	return is;
}