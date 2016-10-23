#ifndef DATE_H_
#define DATE_H_

#include <ctime>
#include <string>				//FOR STRING

namespace Date {
	enum DIF_YMD { Y, M, D };	//FOR datedif()

	class date:public tm		///数据成员公有
	{
	  public:
		date();					//默认为1900-1-1 8:00:00
		date(int y, int m, int md, int h = 8, int mi = 0, int s = 0, int is =
			 -1);
		//参数分别为:年　月　日　时　分　秒　时制
		bool isError() const;	//错误的时间信息返真
		int getYear() const;	//获取年数
		int getMonth() const;	//月
		int getDay() const;		//日
		int getWeek() const;	//星期几(0为星期日)
		int isleap() const;		//闰年为真
		int dayFmon() const;	//当月的最大天数
		int allDay() const;		//自1970.1.1来的多少天(含本天)
		std::string datetostr() const;	//将时间转为字符串　1970-1-1 08:00:00 Monday
		date operator +(const int);
		date & operator+=(const int);
		date operator -(const int);
		date & operator-=(const int);
		void process();			//计算与校验 改变日期后用此处理
	//private:
		void process_m();		//月份大于12时的处理<可能不止大一>
		void pro_addone_mon();	//月份加一并处理
	};							//CLASS DATE

	int isleap(int);			//闰年判断
	int weekDay(int, int, int);	//星期几（星期日为0）
	int yday(int, int, int);	//一年中的第多少天 从0开始
	int dayFmon(int, int);		//某年某月的最大天数
	int numLeap(unsigned, unsigned);	//两年份间有多少个闰年(不含后一参数)

	int difyear(const date &, const date &);	//两日期相差年数
	int difmonth(const date &, const date &);	//从1900.1以来相差月数
	int difmonth_t(const date &, const date &);	//同上
	int difday(const date &, const date &);	//从1900.1.1以来相差天数
	int datedif(const date &, const date &, DIF_YMD);	//相差　年月日
	std::string difDate(const date & a, const date & b);
	//两日期间相差多少年又多少月又多少日
	bool operator>(const date &, const date &);
	bool operator==(const date &, const date &);

	void inDate(date &);		///年月日输入　时刻默认为八点正
	date *now();				//系统当前日期

}

#endif



