//
//  message.cpp
//  c++Postgres
//
//  Created by Clifford Campo on 4/30/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//

// #include "message.hpp"
#include "myPrototypes.hpp"
extern const char *defalutResultsFileName[];
extern char *defaultDependentVariableColumn;
extern const char *readBackTable[];  //Read back the data that we previously inserted into table tbl_bucketized_by_tempf_ranges
extern const char *dropTable[];
extern const char *createTable[];

extern const char *insertIntoTable[];
extern const char *create_tbl_bucketize_by_tempf_ranges;
// extern const char *motherOfAllSelectStatements[];
extern const char *myTempFiles[];
extern const char *helpMessageArray[2];
extern const char *begin_Transaction;
extern const char *end_Transaction;
extern const char *fetchALLTransaction ;
extern const char *close_Transaction;
extern const int numberOfEntries;
extern const char *BFM;
extern const char *cases[];
extern const char *copy_file[];
extern const char *theMotherOfAllUnionSelectStatements[];
extern const char *copySQL;
extern const char *dependentVariableColumn[];
//extern const char *dropThenCreateTempTable;
extern const char *drop_tbl_temp_eu[];
extern const char *create_tbl_temp_eu[];
//
//
const char *BFM = " * + * + * + * + * + * + * + * + * + * + * + * +";

//
//

const char *help_Message_Array[] = {"\nThis program, ", \
    ",\nis used to obtain a mathematical polynomial function of energy usage wrt temperature with data obtained from the PostgreSQL database, \nLocalWeather, using table, tbl_energy_usage. \nThe command line parameters for this program are as follows:", \
    "\n\t--dbname or -D: provides the name of the database [[default `LocalWeather`]];", \
    "\n\t--help or -h: provides this help message;", \
    "\n\t--userid or -U: provides the database name's userid [[default `cjc`]];", \
    "\n\t--hostid or -H: provides the IP address of the database server [[default `localhost`]];", \
    "\n\t--port or -P: provides the database's port id [[default `5436`]]; ", \
    "\n\t--kelvin or -k: if set then we will be processing temperature data on the Kelvin scale. Default [[ NOT set, using Farenheit scale]];", \
    "\n\t--debug1 or -1: provides debugging output of intermediate results in the main.cpp module;", \
    "\n\t--debug2 or -2: provides debugging output of the intermediate results in the selectFromTable.cpp module;", \
    "\n\t--debug3 or -3: provides debugging output of the intermediate results in the insertBucketizedEnergyVariablesIntoTable.cpp module.\nThis is where the gsl magic happens;", \
    "\n\t--sql or -S: provides SQL statement [[ ***no default***, test purposes only ]];", \
    "\n\t--col or -t: the presence of the `-t` switch indicates that we should use multi-thread, rather than single thread processing techniques; ", \
    "\n\t--resultsFile or -f: provides the name of a text file where the results will be stored. There is no default value, this field is purely optional.\n"};
const int numberOfEntries = sizeof(help_Message_Array);
//
const char *myTempFiles[] = {"/tmp/temporary_temp_energy_usage_m1m2kwh.csv","/tmp/temporary_temp_energy_usage_m1kwh.csv","/tmp/temporary_temp_energy_usage_m2kwh.csv"};
//


const char *defalutResultsFileName[] = {"/Users/cjc/c++/energyUsage/energyUsage/M1M2kwh_resultsFile.txt", "/Users/cjc/c++/energyUsage/energyUsage/M1kwh_resultsFile.txt", "/Users/cjc/c++/energyUsage/energyUsage/M2kwh_resultsFile.txt"};
//


const char *motherOfAllSelectStatements[] = { \
    "DECLARE myportal CURSOR FOR SELECT today.avtempf AS \"today_avtempf\", yesterday.avtempf AS \"yesterday_avtempf\", (today.avtempf-yesterday.avtempf) AS \"∆avtempf\", today.avtempk AS \"today_avtempk\", yesterday.avtempk AS \"yesterday_avtempk\", yesterday.date AS \"yesterday_date\", today.date AS \"today_date\", (today.date - yesterday.date) AS \"∆day\", yesterday.m1kwh, today.m1kwh, (today.m1kwh - yesterday.m1kwh) AS \"m1Usage\", yesterday.m2kwh, today.m2kwh, (today.m2kwh- yesterday.m2kwh) AS \"m2Usage\", (1 * (today.m2kwh - yesterday.m2kwh)  +1 * (today.m1kwh - yesterday.m1kwh))/(today.date - yesterday.date) AS \"AvgDaily_m1m2kwh_Usage\"  \
    FROM \
        (SELECT date,  avtempf , avtempk, m1kwh, m2kwh, jd FROM tbl_energy_usage ORDER BY date asc) today \
        INNER JOIN \
        (SELECT date,  avtempf , avtempk, m1kwh, m2kwh, jd FROM tbl_energy_usage ORDER BY date asc) yesterday \
    ON today.jd::int = 1 + yesterday.jd::int ORDER BY today.date;", /*Starting with the DECLARE …, above, this SQL does the m1m2kwh case */
    "DECLARE myportal CURSOR FOR SELECT today.avtempf AS \"today_avtempf\", yesterday.avtempf AS \"yesterday_avtempf\", (today.avtempf-yesterday.avtempf) AS \"∆avtempf\", today.avtempk AS \"today_avtempk\", yesterday.avtempk AS \"yesterday_avtempk\", yesterday.date AS \"yesterday_date\", today.date AS \"today_date\", (today.date - yesterday.date) AS \"∆day\", yesterday.m1kwh, today.m1kwh, (today.m1kwh - yesterday.m1kwh) AS \"m1Usage\", yesterday.m2kwh, today.m2kwh, (today.m2kwh- yesterday.m2kwh) AS \"m2Usage\", (0 * (today.m2kwh - yesterday.m2kwh)  +1 * (today.m1kwh - yesterday.m1kwh))/(today.date - yesterday.date) AS \"AvgDaily_m1kwh_Usage\"  \
    FROM \
    (SELECT date,  avtempf , avtempk, m1kwh, m2kwh, jd FROM tbl_energy_usage ORDER BY date asc) today \
    INNER JOIN \
    (SELECT date,  avtempf , avtempk, m1kwh, m2kwh, jd FROM tbl_energy_usage ORDER BY date asc) yesterday \
    ON today.jd::int = 1 + yesterday.jd::int ORDER BY today.date;", /* Starting with the DECLARE, above, this SQL does the m1kwh case */ \
    "DECLARE myportal CURSOR FOR SELECT today.avtempf AS \"today_avtempf\", yesterday.avtempf AS \"yesterday_avtempf\", (today.avtempf-yesterday.avtempf) AS \"∆avtempf\", today.avtempk AS \"today_avtempk\", yesterday.avtempk AS \"yesterday_avtempk\", yesterday.date AS \"yesterday_date\", today.date AS \"today_date\", (today.date - yesterday.date) AS \"∆day\", yesterday.m1kwh, today.m1kwh, (today.m1kwh - yesterday.m1kwh) AS \"m1Usage\", yesterday.m2kwh, today.m2kwh, (today.m2kwh- yesterday.m2kwh) AS \"m2Usage\", (1 * (today.m2kwh - yesterday.m2kwh)  + 0 * (today.m1kwh - yesterday.m1kwh))/(today.date - yesterday.date) AS \"AvgDaily_m2kwh_Usage\"  \
    FROM \
    (SELECT date,  avtempf , avtempk, m1kwh, m2kwh, jd FROM tbl_energy_usage ORDER BY date asc) today \
    INNER JOIN \
    (SELECT date,  avtempf , avtempk, m1kwh, m2kwh, jd FROM tbl_energy_usage ORDER BY date asc) yesterday \
    ON today.jd::int = 1 + yesterday.jd::int ORDER BY today.date;", /* Starting with the DECLARE, above, this SQL does the m2kwh case. */ \
};  //Could interpret as: motherOfAllSelectStatements[3], or an array of 3rows.

//extern const short numberOfColumnsInMotherOFAllSelectStatements = 2;
/*  Example of what's returned by this mother of all select statements for the case of *motherOfAllSelectStatements[0]:
 currentavtempf | prevavtempf |  ∆avtempf  | currentavtempk | prevavtempk |   yesterday    |   today    | ∆day | m1kwh | m1kwh | m1Usage | m2kwh | m2kwh | m2Usage | AvgDailyUsage
----------+----------+------------+----------+----------+------------+------------+------+-------+-------+---------+-------+-------+---------+---------------
     64.4 |    38.86 |     -25.54 |   291.15 |   276.96 | 2014-12-03 | 2014-12-01 |    2 | 12776 | 12720 |      56 |  2670 |  2665 |       5 |          30.5
    46.15 |    38.86 |      -7.29 |   281.01 |   276.96 | 2014-12-03 | 2014-12-02 |    1 | 12776 | 12739 |      37 |  2670 |  2667 |       3 |            40
    38.86 |    39.27 |       0.41 |   276.96 |   277.19 | 2014-12-04 | 2014-12-03 |    1 | 12813 | 12776 |      37 |  2673 |  2670 |       3 |            40
    39.27 |    34.82 |      -4.45 |   277.19 |   274.71 | 2014-12-05 | 2014-12-04 |    1 | 12852 | 12813 |      39 |  2675 |  2673 |       2 |            41
    34.82 |    40.55 |       5.73 |   274.71 |    277.9 | 2014-12-06 | 2014-12-05 |    1 | 12890 | 12852 |      38 |  2678 |  2675 |       3 |            41
      .          .            .          .           .          .           .          .     .       .           .      .      .           .               .
      .          .            .          .           .          .           .          .     .       .           .      .      .           .               .
      .          .            .          .           .          .           .          .     .       .           .      .      .           .               .
 */
const char *dependentVariableColumn[] = {"m1m2kwh", "m1kwh", "m2kwh"};
extern const short numberOfThreads = sizeof(dependentVariableColumn)/sizeof(BFM);  //Note Well: doing just sizeof(dependentVariableColumn) \
give the length of three pointer types. by divinding sizeof(dependentVariableColumn) by the sizeof one typical pointer we get the \
/number of entries comprising the array known as dependentVariableColumn.
//
const char *cases[] = {"\tCase m1m2kwh", "\tCase m1kwh", "\tCase m2kwh"};
//
/*
const char *create_tbl_bucketize_by_tempf_ranges = "CREATE TABLE IF NOT EXISTS tbl_bucketized_by_tempf_ranges  ( \
id serial PRIMARY KEY, // id primary key \
avgTemp float8, \
avgEU   float8, \
stddevEU float8, \
minEU float8, \
maxEU float8, \
countEU int);";
 */
//
//
const char *insertIntoTable[] = {"INSERT INTO tbl_bucketized_by_tempf_ranges0 (avgTemp, avgEU, stddevEU, minEU, maxEU, countEU) VALUES(%f,%f,%f,%f,%f,%d);", "INSERT INTO tbl_bucketized_by_tempf_ranges1 (avgTemp, avgEU, stddevEU, minEU, maxEU, countEU) VALUES(%f,%f,%f,%f,%f,%d);", "INSERT INTO tbl_bucketized_by_tempf_ranges2 (avgTemp, avgEU, stddevEU, minEU, maxEU, countEU) VALUES(%f,%f,%f,%f,%f,%d);"};

//
//
/*
const char *insertIntoTmpTable = "INSERT INTO tbl_temp_eu (avgTemp, avgEU) VALUES(%f,%f);";
*/

//
//
const char *dropTable[] = {"DROP TABLE IF EXISTS tbl_bucketized_by_tempf_ranges0;","DROP TABLE IF EXISTS tbl_bucketized_by_tempf_ranges1;","DROP TABLE IF EXISTS tbl_bucketized_by_tempf_ranges2;"}  ;
//
//
/* NOT USED
 const char *createTempTable = "CREATE TEMPORARY TABLE IF NOT EXISTS tbl_bucketized_by_tempf_ranges  (ID serial PRIMARY KEY,avgTemp float8,avgEU float8, stddevEU float8, minEU float8, maxEU float8, countEU int);";
*/
//
//
const char *createTable[] = {"CREATE TABLE IF NOT EXISTS tbl_bucketized_by_tempf_ranges0  (ID serial PRIMARY KEY,avgTemp float8,avgEU float8, stddevEU float8, minEU float8, maxEU float8, countEU int);",
    "CREATE TABLE IF NOT EXISTS tbl_bucketized_by_tempf_ranges1  (ID serial PRIMARY KEY,avgTemp float8,avgEU float8, stddevEU float8, minEU float8, maxEU float8, countEU int);",
    "CREATE TABLE IF NOT EXISTS tbl_bucketized_by_tempf_ranges2  (ID serial PRIMARY KEY,avgTemp float8,avgEU float8, stddevEU float8, minEU float8, maxEU float8, countEU int);"};
//
//
//
const char *drop_tbl_temp_eu[] = {"DROP TABLE IF EXISTS tbl_temp_eu0;","DROP TABLE IF EXISTS tbl_temp_eu1;", "DROP TABLE IF EXISTS tbl_temp_eu2;"} ;
const char *create_tbl_temp_eu[] = {"CREATE TABLE IF NOT EXISTS tbl_temp_eu0  (id serial PRIMARY KEY, temp float8, eu float8);", "CREATE TABLE IF NOT EXISTS tbl_temp_eu1  (id serial PRIMARY KEY, temp float8, eu float8);", "CREATE TABLE IF NOT EXISTS tbl_temp_eu2  (id serial PRIMARY KEY, temp float8, eu float8);"};
const char *fetchALLTransaction = "FETCH ALL in myportal";
//
const char *readBackTable[] = {"DECLARE myportal CURSOR FOR SELECT avgTemp, avgEU, stddevEU, minEU, maxEU, countEU FROM tbl_bucketized_by_tempf_ranges0 ORDER BY ID;", "DECLARE myportal CURSOR FOR SELECT avgTemp, avgEU, stddevEU, minEU, maxEU, countEU FROM tbl_bucketized_by_tempf_ranges1 ORDER BY ID;", "DECLARE myportal CURSOR FOR SELECT avgTemp, avgEU, stddevEU, minEU, maxEU, countEU FROM tbl_bucketized_by_tempf_ranges2 ORDER BY ID;"};
//
const char *begin_Transaction = "BEGIN;";
//
const char *end_Transaction = "END;";
//
const  char *close_Transaction = "CLOSE myportal";
//   As they said in Apocalypse Now, the scene where Colonol Kilgore calls in a napalm strike: "Hold on this is going to be a big one!"
const char *theMotherOfAllUnionSelectStatements[]={"DECLARE myportal CURSOR FOR SELECT 01 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp > 00 AND temp<=02  UNION SELECT 03 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp > 02 and temp <=04  UNION SELECT 05 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >04 and temp <=06  UNION SELECT 07 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >06 and temp <=08  UNION SELECT 09, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >08 and temp <=10  UNION \
    SELECT 11 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp > 10 AND temp<=12  UNION SELECT 13 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp > 12 and temp <=14  UNION SELECT 15 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >14 and temp <=16  UNION SELECT 17 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >16 and temp <=18  UNION SELECT 19, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >18 and temp <=20  UNION \
    SELECT 21 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp > 20 AND temp<=22  UNION SELECT 23 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp > 22 and temp <=24  UNION SELECT 25 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >24 and temp <=26  UNION SELECT 27 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >26 and temp <=28  UNION SELECT 29, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >28 and temp <=30  UNION \
    SELECT 31 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp > 30 AND temp<=32  UNION SELECT 33 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp > 32 and temp <=34  UNION SELECT 35 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >34 and temp <=36  UNION SELECT 37 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >36 and temp <=38  UNION SELECT 39, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >38 and temp <=40  UNION \
    SELECT 41 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp > 40 AND temp<=42  UNION SELECT 43 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp > 42 and temp <=44  UNION SELECT 45 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >44 and temp <=46  UNION SELECT 47 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >46 and temp <=48  UNION SELECT 49, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >48 and temp <=50  UNION  \
    SELECT 51 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp > 50 AND temp<=52  UNION SELECT 53 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp > 52 and temp <=54  UNION SELECT 55 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >54 and temp <=56  UNION SELECT 57 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >56 and temp <=58  UNION SELECT 59, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >58 and temp <=60  UNION \
    SELECT 61 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp > 60 AND temp<=62  UNION SELECT 63 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp > 62 and temp <=64  UNION SELECT 65 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >64 and temp <=66  UNION SELECT 67 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >66 and temp <=68  UNION SELECT 69, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >68 and temp <=70  UNION \
    SELECT 71 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp > 70 AND temp<=72  UNION SELECT 73 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp > 72 and temp <=74  UNION SELECT 75 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >74 and temp <=76  UNION SELECT 77 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >76 and temp <=78  UNION SELECT 79, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >78 and temp <=80  UNION \
    SELECT 81 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp > 80 AND temp<=82  UNION SELECT 83 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp > 82 and temp <=84  UNION SELECT 85 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >84 and temp <=86  UNION SELECT 87 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >86 and temp <=88  UNION SELECT 89, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu0 WHERE temp >88 and temp <=90", \
    "DECLARE myportal CURSOR FOR SELECT 01 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp > 00 AND temp<=02  UNION SELECT 03 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp > 02 and temp <=04  UNION SELECT 05 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >04 and temp <=06  UNION SELECT 07 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >06 and temp <=08  UNION SELECT 09, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >08 and temp <=10  UNION \
    SELECT 11 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp > 10 AND temp<=12  UNION SELECT 13 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp > 12 and temp <=14  UNION SELECT 15 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >14 and temp <=16  UNION SELECT 17 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >16 and temp <=18  UNION SELECT 19, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >18 and temp <=20  UNION \
    SELECT 21 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp > 20 AND temp<=22  UNION SELECT 23 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp > 22 and temp <=24  UNION SELECT 25 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >24 and temp <=26  UNION SELECT 27 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >26 and temp <=28  UNION SELECT 29, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >28 and temp <=30  UNION \
    SELECT 31 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp > 30 AND temp<=32  UNION SELECT 33 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp > 32 and temp <=34  UNION SELECT 35 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >34 and temp <=36  UNION SELECT 37 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >36 and temp <=38  UNION SELECT 39, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >38 and temp <=40  UNION \
    SELECT 41 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp > 40 AND temp<=42  UNION SELECT 43 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp > 42 and temp <=44  UNION SELECT 45 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >44 and temp <=46  UNION SELECT 47 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >46 and temp <=48  UNION SELECT 49, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >48 and temp <=50  UNION  \
    SELECT 51 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp > 50 AND temp<=52  UNION SELECT 53 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp > 52 and temp <=54  UNION SELECT 55 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >54 and temp <=56  UNION SELECT 57 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >56 and temp <=58  UNION SELECT 59, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >58 and temp <=60  UNION \
    SELECT 61 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp > 60 AND temp<=62  UNION SELECT 63 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp > 62 and temp <=64  UNION SELECT 65 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >64 and temp <=66  UNION SELECT 67 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >66 and temp <=68  UNION SELECT 69, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >68 and temp <=70  UNION \
    SELECT 71 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp > 70 AND temp<=72  UNION SELECT 73 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp > 72 and temp <=74  UNION SELECT 75 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >74 and temp <=76  UNION SELECT 77 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >76 and temp <=78  UNION SELECT 79, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >78 and temp <=80  UNION \
    SELECT 81 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp > 80 AND temp<=82  UNION SELECT 83 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp > 82 and temp <=84  UNION SELECT 85 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >84 and temp <=86  UNION SELECT 87 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >86 and temp <=88  UNION SELECT 89, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu1 WHERE temp >88 and temp <=90", \
    "DECLARE myportal CURSOR FOR SELECT 01 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp > 00 AND temp<=02  UNION SELECT 03 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp > 02 and temp <=04  UNION SELECT 05 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >04 and temp <=06  UNION SELECT 07 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >06 and temp <=08  UNION SELECT 09, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >08 and temp <=10  UNION \
    SELECT 11 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp > 10 AND temp<=12  UNION SELECT 13 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp > 12 and temp <=14  UNION SELECT 15 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >14 and temp <=16  UNION SELECT 17 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >16 and temp <=18  UNION SELECT 19, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >18 and temp <=20  UNION \
    SELECT 21 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp > 20 AND temp<=22  UNION SELECT 23 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp > 22 and temp <=24  UNION SELECT 25 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >24 and temp <=26  UNION SELECT 27 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >26 and temp <=28  UNION SELECT 29, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >28 and temp <=30  UNION \
    SELECT 31 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp > 30 AND temp<=32  UNION SELECT 33 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp > 32 and temp <=34  UNION SELECT 35 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >34 and temp <=36  UNION SELECT 37 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >36 and temp <=38  UNION SELECT 39, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >38 and temp <=40  UNION \
    SELECT 41 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp > 40 AND temp<=42  UNION SELECT 43 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp > 42 and temp <=44  UNION SELECT 45 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >44 and temp <=46  UNION SELECT 47 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >46 and temp <=48  UNION SELECT 49, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >48 and temp <=50  UNION  \
    SELECT 51 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp > 50 AND temp<=52  UNION SELECT 53 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp > 52 and temp <=54  UNION SELECT 55 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >54 and temp <=56  UNION SELECT 57 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >56 and temp <=58  UNION SELECT 59, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >58 and temp <=60  UNION \
    SELECT 61 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp > 60 AND temp<=62  UNION SELECT 63 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp > 62 and temp <=64  UNION SELECT 65 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >64 and temp <=66  UNION SELECT 67 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >66 and temp <=68  UNION SELECT 69, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >68 and temp <=70  UNION \
    SELECT 71 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp > 70 AND temp<=72  UNION SELECT 73 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp > 72 and temp <=74  UNION SELECT 75 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >74 and temp <=76  UNION SELECT 77 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >76 and temp <=78  UNION SELECT 79, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >78 and temp <=80  UNION \
    SELECT 81 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp > 80 AND temp<=82  UNION SELECT 83 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp > 82 and temp <=84  UNION SELECT 85 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >84 and temp <=86  UNION SELECT 87 AS \"AVGTEMP\",avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >86 and temp <=88  UNION SELECT 89, avg(eu), stddev(eu),  min(eu), max(eu), count(*) FROM tbl_temp_eu2 WHERE temp >88 and temp <=90"};

//
const char *copySQL = "COPY tbl_temp_eu (temp, eu) FROM '/tmp/temporary_temp_energy_usage_file.csv' WITH (FORMAT csv, DELIMITER ',', NULL '\\N');";
/* NOT USED
const char *copy_STDIN_SQL = "COPY tbl_temp_eu (temp, eu) FROM STDIN WITH(FORMAT csv, DELIMITER ',', NULL '\\N');";
 */
const char *copy_file[] = {"COPY tbl_temp_eu0 (temp, eu) FROM '/tmp/temporary_temp_energy_usage_m1m2kwh.csv' WITH (FORMAT csv, DELIMITER ',', NULL '\\N');", "COPY tbl_temp_eu1 (temp, eu) FROM '/tmp/temporary_temp_energy_usage_m1kwh.csv' WITH (FORMAT csv, DELIMITER ',', NULL '\\N');", "COPY tbl_temp_eu2 (temp, eu) FROM '/tmp/temporary_temp_energy_usage_m2kwh.csv' WITH (FORMAT csv, DELIMITER ',', NULL '\\N');"};
//
