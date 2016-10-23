#include<iostream>
#include<cstdio>				//FOR sscanf
#include <algorithm>
#include "date.h"

namespace Date {
	inline date::date()			//默认为1900-1-1 8:00:00
	{
		tm_hour = 8;
		tm_sec = tm_min = 0;
		tm_mday = 1;
		tm_mon = tm_year = tm_yday = 0;
		tm_wday = 1;			//1900.1.1星期一
		tm_isdst = -1;			//不知道什么时制
	} 

	date::date(int y, int m, int md, int h, int mi, int s, int is) {
		tm_year = y - 1900;		//自1900的年数
		tm_mon = m - 1;			//当年中的 第几月　　　0-11
		tm_mday = md;			///在当月中的第几天　1-31
		tm_hour = h;			//0-23小时
		tm_min = mi;			//0-59分
		tm_sec = s;				//0-59秒
		this->process();		//计算与校验
		tm_wday = Date::weekDay(tm_year + 1900, tm_mon + 1, tm_mday);
		//当前日期是星期几　0-6　0为星期日
		tm_yday = Date::yday(tm_year + 1900, tm_mon + 1, tm_mday);
		//当前日期为一年中第多少天　0-365
		tm_isdst = is;			//正为夏时制,0为非夏时制,负为不知此信息
		//if(isError())
		//   *this=date();
	}

	inline int date::getYear() const {
		return tm_year + 1900;
	} 

	inline int date::getMonth() const {
		return tm_mon + 1;
	} 

	inline int date::getDay() const {
		return tm_mday;
	} 

	inline int date::date::getWeek() const {
		return tm_wday;
	} 

	inline int date::isleap() const {
		return (getYear()) % ((getYear()) % 100 ? 4 : 400) ? 0 : 1;
	} 

	bool date::isError() const {
		if (tm_year < 0 || tm_mon < 0 || tm_mon > 11 || tm_mday < 1
			|| tm_mday > dayFmon() || tm_hour < 0 || tm_hour > 23 || tm_min < 0
			|| tm_min > 59 || tm_sec < 0 || tm_sec > 59)
			return true;

		return false;
	} 

	inline void date::pro_addone_mon()	//月份加一　并处理
	{
		if (++this->tm_mon > 11) {
			this->tm_mon = 0;
			++this->tm_year;
		}
	}

	static int pro_add_for_mon(int &dm, date & tmp)	//dm中所含整月数<局部函数>
	{
		int m = 0; //dm中含多少个整月,tmp加上这些月后的日期处理

		for (int n = tmp.dayFmon(); dm >= n; n = tmp.dayFmon()) {
			++m;
			dm -= n;
			tmp.pro_addone_mon();
		}

		return m;
	}

	int date::dayFmon() const	//某年某月的最大天数
	{
		int d = 0;

		switch (getMonth()) {
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			d = 31;
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			d = 30;
			break;
		case 2:
			d = 28 + isleap();
			break;
		} 

		return d;
	}

	int date::allDay() const	//自1900.1.1来的第多少天(含本天)
	{
		int sum = 365 * tm_year + Date::numLeap(1900, getYear());

		for (int m = 1; m <= tm_mon; ++m)
			 sum += Date::dayFmon(getYear(), m);
		 sum += tm_mday;		///加上了本天

		 return sum;
	} 

	std::string date::datetostr() const	//将时间转为字符串
	{
		char tmp[32] = { 0 };

		strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S %A", this);
		return std::string(tmp);
	}

	date date::operator +(const int v) {
		if (v == 0)
			return *this;
		date d(*this);
		if (v > 0) {
			d.tm_mday += v;		//将当天日期加上要增加的数
			while (d.tm_mday > d.dayFmon())	//当加所加天数超过当月天数
			{
				d.tm_mday -= d.dayFmon();
				d.pro_addone_mon();
			}
			d.tm_wday = Date::weekDay(d.getYear(), d.getMonth(), d.getDay());
			d.tm_yday = Date::yday(d.getYear(), d.getMonth(), d.getDay());
			return d;
		} else
			return d - (-v);
	}

	date & date::operator+=(const int v) {
		if (v == 0)
			return *this;
		if (v > 0) {
			tm_mday += v;
			while (tm_mday > dayFmon()) {
				tm_mday -= dayFmon();
				pro_addone_mon();
			}
			tm_wday = Date::weekDay(getYear(), getMonth(), getDay());
			tm_yday = Date::yday(getYear(), getMonth(), getDay());
			return *this;
		} else
			return (*this) -= (-v);
	}

	date date::operator -(const int v) {
		if (v == 0)
			return *this;
		date d(*this);
		if (v > 0) {
			d.tm_mday -= v;
			while (d.tm_mday < 1) {
				--d.tm_mon;
				if (d.tm_mon == -1) {
					d.tm_mon = 11;
					--d.tm_year;
				}
				d.tm_mday += d.dayFmon();
			}
			d.tm_wday = Date::weekDay(d.getYear(), d.getMonth(), d.getDay());
			d.tm_yday = Date::yday(d.getYear(), d.getMonth(), d.getDay());
			return d;
		} else
			return d + (-v);
	}

	date & date::operator -=(const int v) {
		if (v == 0)
			return *this;
		if (v > 0) {
			tm_mday -= v;
			while (tm_mday < 1) {
				--tm_mon;
				if (tm_mon == -1) {
					tm_mon = 11;
					--tm_year;
				}
				tm_mday += dayFmon();
			}
			tm_wday = Date::weekDay(getYear(), getMonth(), getDay());
			tm_yday = Date::yday(getYear(), getMonth(), getDay());
			return *this;
		} else
			return *this += (-v);
	}

	void date::process_m()		//月份大于12时的处理
	{
		while (this->tm_mon > 11) {
			this->tm_mon -= 12;
			++this->tm_year;
		}
	}

	void date::process() {
		if (tm_sec < 0)
			tm_sec = 0;
		else
			while (tm_sec > 59) {
				tm_sec -= 60;
				++tm_min;
			}

		if (tm_min < 0)
			tm_min = 0;
		else
			while (tm_min > 59) {
				tm_min -= 60;
				++tm_hour;
			}

		if (tm_hour < 0)
			tm_hour = 0;
		else
			while (tm_hour > 23) {
				tm_hour -= 24;
				++tm_mday;
			}

		process_m();

		if (tm_mday < 1)
			tm_mday = 1;
		else
			while (tm_mday > dayFmon()) {
				tm_mday -= dayFmon();
				pro_addone_mon();
			}

		if (tm_mon < 0)
			tm_mon = 0;
		else
			process_m();

		if (tm_year < 0)
			tm_year = 0;
	}

	int weekDay(int y, int m, int d)	//星期几（星期日为0）
	{
		if (m == 1 || m == 2) {
			m += 12;
			--y;
		}
		int iWeek =
			(d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400) % 7;

		return (iWeek + 1) % 7;
	}

	int yday(int y, int m, int d)	//一年中的第多少天 从0开始
	{
		int day = 0;

		for (int i = 1; i < m; ++i) {
			day += Date::dayFmon(y, m);
		}

		return day + d - 1;		//从0开始
	}

	inline int isleap(int y) {
		return y % (y % 100 ? 4 : 400) ? 0 : 1;
	}

	int dayFmon(int y, int m)	//某年某月的最大天数
	{
		int d = 0;

		switch (m) {
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			d = 31;
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			d = 30;
			break;
		case 2:
			d = 28 + Date::isleap(y);
			break;
		}

		return d;
	}

	int difyear(const date & a, const date & b)	//两日期相差年数
	{
		int y = a.getYear() - b.getYear();

		if (y != 0) {
			const date *max = 0;
			const date *min = 0;
			if (y < 0) {
				y = -y;
				max = &b;
				min = &a;
			} else {
				max = &a;
				min = &b;
			}
			int maxm = max->getMonth();
			int minm = min->getMonth();
			if (maxm < minm || (maxm == minm && max->getDay() < min->getDay()))
				--y;
		}

		return y;
	}

	int difmonth(const date & a, const date & b) {
		int y = difyear(a, b);
		int dm = a.allDay() - b.allDay();
		if (dm == 0)
			return 0;
		const date & max = dm > 0 ? a : b;
		const date & min = dm > 0 ? b : a;
		date tmp(min.getYear() + y, min.getMonth(), min.getDay());
		//date tmp(dm>0?b:a);
		//tmp.tm_year += y;
		dm = max.allDay() - tmp.allDay();

		return y * 12 + pro_add_for_mon(dm, tmp);
	}

	int difmonth_t(const date & a, const date & b) {
		int d = a.allDay() - b.allDay();
		date tmp(d < 0 ? a : b);
		if (d < 0)
			d = -d;
		return pro_add_for_mon(d, tmp);
	}

	int difday(const date & a, const date & b)	//相差天数
	{
		int d = a.allDay() - b.allDay();
		if (d < 0)
			d = -d;
		return d;
	}

	int datedif(const date & a, const date & b, DIF_YMD k) {
		switch (k) {
		case Y:
			return difyear(a, b);
		case M:
			return difmonth(a, b);
		default:
			return difday(a, b);
		}
	}

	date *now() {
		time_t t = time(0);
		return static_cast < date * >(localtime(&t));
	}

	bool operator>(const date & a, const date & b) {
		return mktime(const_cast < date * >(&a)) > mktime(const_cast <
														  date * >(&b));
	}

	bool operator==(const date & a, const date & b) {
		return mktime(const_cast < date * >(&a)) == mktime(const_cast <
														   date * >(&b));
	}

	void inDate(date & obj) {
		std::string tmp;
		getline(std::cin, tmp);
		if (sscanf(tmp.c_str(), "%d%*[^0-9]%d%*[^0-9]%d",
				   &obj.tm_year, &obj.tm_mon, &obj.tm_mday) != 3) {
			obj = date();
		} else {
			obj.tm_hour = 8;
			obj.tm_min = obj.tm_sec = 0;
			obj.tm_year -= 1900;
			obj.tm_mon -= 1;
			obj.process();
			obj.tm_wday =
				Date::weekDay(obj.getYear(), obj.getMonth(), obj.getDay());
			obj.tm_yday =
				Date::yday(obj.getYear(), obj.getMonth(), obj.getDay());
			obj.tm_isdst = -1;
		}
	}

	std::string difDate(const date & a, const date & b) {
		int y = difyear(a, b);
		date tmp(b > a ? a : b);

		int dm =
			difday(a, b) - 365 * y - Date::numLeap(a.getYear(), b.getYear());
		tmp.tm_year += y;
		int m = pro_add_for_mon(dm, tmp);

		char str[64] = { 0 };
		sprintf(str, "两日期相差: %d 年又 %d 个月又 %d 天", y, m,
				dm);

		return std::string(str);
	}

	int numLeap(unsigned ya, unsigned yb) {
		if (yb < ya)
			std::swap(ya, yb);
		register int i = 0;
		for (; ya < yb; ++ya) {
			if (Date::isleap(ya))
				++i;
		}

		return i;
	}
}								//namespace
