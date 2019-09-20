/*Created by J.Wong 2016/9/4 9:09:05*/

#pragma once
#include <iostream>
#include <limits>
#include <string>
#include <stdio.h>
//#include "../gui/enum.h"

namespace Gemini{
	typedef enum dateFormat{
		TextDate,
		IsoDate	
	}DateFormat;	
}

class CDate{
	
	public:
		typedef  long long int64;
		enum MonthNameType{
			DateFormat = 0,
        	StandaloneFormat	
		};
		
	private:
		explicit	CDate(int64 julianDay):m_jd(julianDay){}
	public:	
		CDate():m_jd(nullJd()){}
		CDate(int y, int m, int d);
		bool isNull()const{return !isValid();}
		bool isValid()const{return m_jd>=minJd()&&m_jd<=maxJd();}
		int year()const;
		int month()const;
		int day()const;
		int dayOfWeek()const;
		int dayOfYear()const;
		int daysInMonth()const;
		int daysInYear()const;
		int	weekNumbers(int* yearNum=0)const;
		
		static bool isValid(int y, int m, int d);
    	static bool isLeapYear(int year);
    	
		static inline CDate fromJulianDay(int64 jd)
		{ return jd >= minJd() && jd <= maxJd() ? CDate(jd) : CDate() ; }
		inline int64 toJulianDay() const { return m_jd; }
			
		static std::string shortMonthName(int month, MonthNameType type = DateFormat);
    	static std::string shortDayName(int weekday, MonthNameType type = DateFormat);
    	static std::string longMonthName(int month, MonthNameType type = DateFormat);
    	static std::string longDayName(int weekday, MonthNameType type = DateFormat);
		
		static CDate currentDate();
		//static CDate fromString(const std::string &s, Gemini::DateFormat f = Gemini::TextDate);
    	static CDate fromString(const std::string &s, const std::string &format);

		std::string toString(Gemini::DateFormat f = Gemini::TextDate) const;
    	std::string 	toString(const std::string &format) const;
    	bool   	setDate(int year, int month, int day);
    	void   	getDate(int* year, int* month, int* day);
    	CDate	addDays(int days)const;
    	CDate	addMonths(int months)const;
    	CDate	addYears(int years)const;
    	int64	daysTo(const CDate& date)const;
    	bool 	operator==(const CDate &other) const { return m_jd == other.m_jd; }
    	bool 	operator!=(const CDate &other) const { return m_jd != other.m_jd; }
    	bool 	operator< (const CDate &other) const { return m_jd <  other.m_jd; }
    	bool 	operator<=(const CDate &other) const { return m_jd <= other.m_jd; }
    	bool 	operator> (const CDate &other) const { return m_jd >  other.m_jd; }
    	bool 	operator>=(const CDate &other) const { return m_jd >= other.m_jd; }
    	
    friend std::ostream& operator<<(std::ostream& os, const CDate& date);
    friend std::istream& operator>>(std::istream& is, CDate& date);	 
	private:
		static inline int64 nullJd() { return (std::numeric_limits<int64>::min)();}
    	static inline int64 minJd() { return int64(-784350574879LL); }
    	static inline int64 maxJd() { return int64( 784354017364LL); }
		int64 m_jd;
    	friend class CDateTime;
    	friend class CDateTimePrivate;	
};


class  CTime{
	explicit	CTime(int ms):m_mds(ms){}
	public:
		CTime():m_mds(NullTime){}
		CTime(int h, int m, int s=0, int ms=0);
		bool	isNull()const{return m_mds==NullTime;}
		bool	isValid()const;
		
		int		hour()const;
		int 	minute()const;
		int		second()const;
		int		msec()const;
		std::string toString(Gemini::DateFormat f = Gemini::TextDate) const;
    	std::string toString(const std::string &format) const;
    		
    	bool 	setHMS(int h, int m, int s, int ms = 0);
    	CTime	addSecs(int s)const;
    	int		secsTo(CTime& time)const;
    	CTime	addMsecs(int ms)const;
    	int		msecsTo(CTime& time)const;
    	
    	bool operator==(const CTime &other) const { return m_mds == other.m_mds; }
   		bool operator!=(const CTime &other) const { return m_mds != other.m_mds; }
    	bool operator< (const CTime &other) const { return m_mds <  other.m_mds; }
  		bool operator<=(const CTime &other) const { return m_mds <= other.m_mds; }
    	bool operator> (const CTime &other) const { return m_mds >  other.m_mds; }
    	bool operator>=(const CTime &other) const { return m_mds >= other.m_mds; }
	
		static inline CTime fromMSecsSinceStartOfDay(int msecs) {return CTime(msecs);}
 		inline int msecsSinceStartOfDay() const { return m_mds == NullTime ? 0 : m_mds;}

    	static CTime currentTime();
    	static CTime fromString(const std::string &s, Gemini::DateFormat f = Gemini::TextDate);
    	static CTime fromString(const std::string &s, const std::string &format);
    	static bool isValid(int h, int m, int s, int ms = 0);

    	void start();
    	int restart();
    	int elapsed() const;
    	
    	friend std::ostream &operator<<(std::ostream& os, const CTime& time);
    	friend std::istream &operator>>(std::istream& is, CTime& time);
	private:
		friend class CDateTimePrivate;	
		enum TimeFlag{NullTime=-1};
		inline int ds() const { return m_mds == -1 ? 0 : m_mds; }
		int m_mds;
	
};

class CDateTimePrivate{
	public:
		typedef long long int64;
		enum DaylightStatus {
        NoDaylightTime = -2,
        UnknownDaylightTime = -1,
        StandardTime = 0,
        DaylightTime = 1
    };
    
    enum StatusFlag {
        NullDate            = 0x01,
        NullTime            = 0x02,
        ValidDate           = 0x04, // just the date field
        ValidTime           = 0x08, // just the time field
        ValidDateTime       = 0x10, // the whole object (including timezone)
        SetToStandardTime   = 0x40,
        SetToDaylightTime   = 0x80
    };
    
    int64 m_msecs;
    int m_offsetFromUtc;
    StatusFlag m_status;
    
    
    CDateTimePrivate() : m_msecs(0),
                         m_offsetFromUtc(0),
                         m_status(StatusFlag(NullDate | NullTime))
    {}

    CDateTimePrivate(const CDate &toDate, const CTime &toTime,int offsetSeconds);	
	void setDateTime(const CDate &date, const CTime &time);
	std::pair<CDate, CTime> getDateTime() const;
	void setDaylightStatus(DaylightStatus status);
    DaylightStatus daylightStatus() const;
    inline int64 toMSecsSinceEpoch() const;

	void checkValidDateTime();
    void refreshDateTime();
    
    inline bool isNullDate() const { return m_status & NullDate; }
    inline bool isNullTime() const { return m_status & NullTime; }
    inline bool isValidDate() const { return m_status & ValidDate; }
    inline bool isValidTime() const { return m_status & ValidTime; }
    inline bool isValidDateTime() const {return m_status & ValidDateTime; }
    inline void setValidDateTime() { m_status= StatusFlag(m_status|ValidDateTime); }
    inline void clearValidDateTime() { m_status =StatusFlag(m_status&~ValidDateTime); }
    inline void clearSetToDaylightStatus() { m_status = StatusFlag(m_status&~(SetToStandardTime | SetToDaylightTime)); }
	static inline int64 minJd() { return CDate::minJd(); }
    static inline int64 maxJd() { return CDate::maxJd(); }
};
class CDateTime{
	public:
		typedef  long long int64;
		CDateTime();
		explicit CDateTime(const CDate& dt);
		CDateTime(const CDate& d, const CTime& t);
		CDateTime(const CDateTime &other);
   		~CDateTime();
   		CDateTime &operator=(const CDateTime &other);
   		
   		
   		bool	isNull()const;
   		bool	isValid()const;
   		CDate	date()const;
   		CTime	time()const;
   		
   		bool isDaylightTime() const;
    	int64 toMSecsSinceEpoch() const;
    	
    	void setDate(const CDate &date);
    	void setTime(const CTime &time);
    	void setMSecsSinceEpoch(int64 msecs);
    	
    	std::string toString(Gemini::DateFormat f = Gemini::TextDate) const;
    	std::string toString(const std::string &format) const;
    		
    	CDateTime addDays(int64 days) const ;
    	CDateTime addMonths(int months) const ;
    	CDateTime addYears(int years) const ;
    	CDateTime addSecs(int64 secs) const ;
    	CDateTime addMSecs(int64 msecs) const ;
    	
    	int64 daysTo(const CDateTime &) const;
    	int64 secsTo(const CDateTime &) const;
    	int64 msecsTo(const CDateTime &) const;
    	
    	bool operator==(const CDateTime &other) const;
    	inline bool operator!=(const CDateTime &other) const { return !(*this == other); }
    	bool operator<(const CDateTime &other) const;
    	inline bool operator<=(const CDateTime &other) const { return !(other < *this); }
    	inline bool operator>(const CDateTime &other) const { return other < *this; }
    	inline bool operator>=(const CDateTime &other) const { return !(*this < other); }
    	
    	static CDateTime currentDateTime();
    	static CDateTime fromString(const std::string &s, Gemini::DateFormat f = Gemini::TextDate);
    	static CDateTime fromString(const std::string &s, const std::string &format);
    	static CDateTime fromMSecsSinceEpoch(int64 msecs);
    	static int64 currentMSecsSinceEpoch();
    	
    	friend std::ostream &operator<<(std::ostream& os, const CDateTime& dt);
    	friend std::istream &operator>>(std::istream& is, CDateTime& dt);
	
	private:
		CDateTimePrivate	d;

};

