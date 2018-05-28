
#ifndef __ISITDAYLIGHTSAVINGSTIME__
#define __ISITDAYLIGHTSAVINGSTIME__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define THEUSUALSUSPECTS	1
#define NUMBEROFDAYSINDAYLIGHTSAVINGSTIME	238	//Number of days between second Sunday in March and first Sunday in November.
#define STANDARDTIME	0                                                /* Return Code of 0 from isItDaylightSavingsTime.c => Standard Time */
#define DAYLIGHTSAVINGSTIME  1+STANDARDTIME                             /* Return Code of 1 from isItDaylightSavingsTime.c => Daylight Savings time */
#define CHANGINGOVERTOSTANDARDTIME 1+DAYLIGHTSAVINGSTIME                /* Return Code of 2 from isItDaylightSavingsTime.c => Changing Over TO Standard Time because it's 2AM on the First Sunday In November */
#define CHANGINGOVERFROMSTANDARDTIME 1+CHANGINGOVERTOSTANDARDTIME       /* Return Code of 3 from isItDaylightsavingstime.c => Changing Over FROM Standard Time because it's 2AM on the Second Sunday In March */
#define TWOAM  0.08333333333 // Two AM is this fraction of a day (i.e., 8-and-a-third percent of a day). NB: if we did 2/24 we would get, due to floating point round-off, 0.083335 which is not as precise. Perhaps with different cpu architecture this simple division would give a more precise answer.
typedef	struct	{
double	J;	//Input is Julian Day
double	Jstart; //Holds the julian date & time of the start of daylight savings time.
double	Jend;  //Holds the julian date & time of the end of daylight savings time.
double	work;	
int	j;	//Integer portion of J.
double	Frac;	//Fractional Part of Julian Day, aka J.
int	p, q, r, s, t, u, v, w, x, y, z, Y, M, D, Y1, M1, m, d;	//Work variables for storing intermediate values
int	year;	//Output year
int	month;	//Output month
int	day;	//Output day
int	hour;	//Output hour
int	minute;	//Output minute
double	second;	//Output second
//                                                                      000000000011111111112222222
union { //                                                              012345678901234567890123456
char	        Gregorian[28];	//Array for holding a character string, YYYY-MM-DD hh:mm:ss.uuuuuu that is the  Gregorian Equivalent of the Julian Time
char	GregorianStartDST[28];	//Array for holding a character string, YYYY-MM-DD hh:mm:ss.uuuuuu that is the  Gregorian Equivalent of the Julian Time, Jstart, representing the date and time of the start of Daylight Savings Time.
};
char	GregorianEndDST[28];	//Array for holding a character string, YYYY-MM-DD hh:mm:ss.uuuuuu that is the  Gregorian Equivalent of the Julian Time, Jend, representing the date & time of the end of Daylight Savings Time.
/*   Work variables used by daylightsavingstime.c */
int     rc;
int     year_;
int     month_;
int     day_;
int	    hour_;
int	    minute_;
int	    second_;
int     dayOfWeek;
int     dststart;
int     jd;
char	workbuf[20];
char   *ptrLocalTime;
} gregorian;


/* Function Prototypes used by daylightsavingstime.c */

int	noIsTheAnswer(register gregorian *);
int	itDepends(register gregorian *);
int	yesIsTheAnswer(register gregorian *);
int     isItDaylightSavingsTime(register gregorian *);


/* Function Prototypes */

int     dow(register gregorian *);          //, int y, int m, int d);
int     GregorianToJulian(register gregorian *);                   //, int, int, int);
int	     JulianToGregorian(register gregorian	*);


//#ifndef GATHERINPUTPARAM
//#define GATHERINPUTPARAM
//int     gatherInputParam(register gregorian *, int, char **);
#endif   /* __ISITDAYLIGHTSAVINGSTIME__ */
