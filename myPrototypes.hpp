//
//  insertIntoTable.hpp
//  c++Postgres
//
//  Created by Clifford Campo on 5/1/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//
#ifndef __MYPROTOTYPES__
#define __MYPROTOTYPES__
#define NOTONCOMMANDLINE 0
#define ONCOMMANDLINE 1 + NOTONCOMMANDLINE
#define POLYNOMIALDEGREE 3
#define BUCKETSIZE 2
#define MAXTEMPERATURE 100
#define MINTEMPERATURE 0
#define DODROPTABLE 1
#define DOCREATETABLE 2*DODROPTABLE
#define DOINSERTINTOTABLE 2*DOCREATETABLE
#define SIZEOFBUFFERCONTAININGMOTHEROFALLSELECTSTATEMNTS 500
#define SIZEOFONEROW 12
#define INITIALVALUE 0
#define WONDERFUL 0
//Beginning of error IDs
#define EXECUTESQLFAILED 1
#define FETCHALLFAILED 2
#define BEGINTRANSACTIONFAILED 3
#define UNKNOWNTRANSACTIONFAILED 4
#define DROPTABLEFAILED 5
#define CREATETABLEFAILED 6
#define CONNECTTODATABASEFAILED 7
#define INSERTINTOFAILED 8
#define FAILEDTOCREATETEMPTABLE 9
#define COPYFAILURE 10
#define COPYENDFAILURE 11
// End of error IDs
#define NUMBEROFHELPMESSAGRSEGMENTS 14
#define M1M2KWHCASE 0
#define M1KWHCASE 1
#define M2KWHCASE 2
#define MAXNUMBEROFAVGTEMPS 100

#define MYTEMPFILENAME "/tmp/temporary_temp_energy_usage_file.csv"
#include "baseClass.hpp"
#include "BitFlags.hpp"
#include "/usr/local/pgsql/pgsql101/include/libpq-fe.h"
int insertIntoTheTable(char *, PGconn *);
extern int makeTable(char *, PGconn *);
//  I DON"T KNOW WHY COMPILER CAN'T FIND BaseClass * ????? int selectFromTable(PGconn *, BaseClass *,  const char *, const char *, int8_t disconnectFlag=false);

enum colnames {_currentavtempf,  _previousavtempf,  _davtempf,  _currentavtempk,  _previousavtempk,   _previousdate,   _currentdate, _dday,  _previousm1kwh, _currentm1kwh, _m1Usage, _previousm2kwh,  _currentm2kwh, _m2Usage, _AvgDailyUsage};
extern int updateTable(const char *, PGconn *, const char *);
extern int dropTheTable(const char *, PGconn *); //Used to drop a table if it exists with the second parameter pointing to the SQL
extern int createTheTable(const char *, PGconn *); //Used to create a table if it doesn't exists with the second parameter pointing to the SQL

static inline void rtrim(std::string &);
const size_t ZERO = 0;
#endif /* __MYPROTOTYPES__ */

