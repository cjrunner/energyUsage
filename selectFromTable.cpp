//
//  selectFromTable.cpp
//  c++Postgres
//
//  Created by Clifford Campo on 5/1/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//
/*
 SELECT Operation

*/
/*
int     polynomialFit(int polynomialdegree, int reccntr, double *XVALin, double *YVALin, double *FPXVAL, double *IPXVAL, double *returnJD, double *returnZD, char *returnISO8601, double *TSS, double *goodnessOfFit);   // <==== New Way of Calling what used to be a test program
 
 id  |    date    |   time   | microsofttime | m1kwh | m2kwh | avtempf | avtempk | m1m2kwh |       jd       | siteid
 ------+------------+----------+---------------+-------+-------+---------+---------+---------+----------------+--------
 2874 | 2018-05-04 | 05:01:00 |   43224.20903 | 42940 |  7734 |         |         |   11.51 |  2458243.20903 |
 2873 | 2018-05-03 | 06:05:00 |  43223.253472 | 42930 |  7733 | 62.2122 | 289.935 |    7.18 | 2458242.253472 |      9
 2872 | 2018-05-02 | 06:41:00 |  43222.278472 | 42924 |  7732 | 53.9762 | 285.359 |   12.32 | 2458241.278472 |      9
 2871 | 2018-05-01 | 07:18:00 |  43221.304167 | 42913 |  7731 | 52.4003 | 284.484 |   25.16 | 2458240.304167 |      9
 2870 | 2018-04-30 | 06:30:00 |  43220.270833 | 42889 |  7729 | 47.0272 | 281.498 |   23.81 | 2458239.270833 |      9
 2868 | 2018-04-29 | 07:19:00 |  43219.304861 | 42867 |  7728 | 57.8237 | 287.496 |   14.12 | 2458238.304861 |      9
 2867 | 2018-04-28 | 10:55:00 |  43218.454861 | 42856 |  7727 |  51.817 | 284.159 |   21.83 | 2458237.454861 |      9
 2865 | 2018-04-27 | 06:20:00 |  43217.263889 | 42833 |  7724 | 57.8552 | 287.514 |   13.57 | 2458236.263889 |      9
 2866 | 2018-04-26 | 07:20:00 |  43216.305556 | 42821 |  7723 |   58.89 |  288.09 |   17.11 | 2458235.305556 |      9
 2864 | 2018-04-25 | 06:05:00 |  43215.253472 | 42804 |  7722 | 57.0455 | 287.064 |   15.92 | 2458234.253472 |      9
 2863 | 2018-04-24 | 07:28:00 |  43214.311111 | 42790 |  7721 | 55.8987 | 286.427 |   15.42 | 2458233.311111 |      9
 2862 | 2018-04-23 | 06:34:00 |  43213.273611 | 42775 |  7720 | 55.3796 | 286.139 |   17.86 | 2458232.273611 |      9
 
 
 */

#include <iostream>
#include <fstream> 
#include <cstring>
#include <sys/select.h>
#include <sys/time.h>
//#include <unistd.h>`
#include <sstream>
#include "effectConnection.hpp"
#include "/usr/local/pgsql/pgsql101/include/libpq-fe.h"
#include "myPrototypes.hpp"
#include "AllocateWorkingVectors.hpp"
#include "baseClass.hpp"
#include "insertIBucketizedEnergyValuesIntoTable.hpp"
#include "BitFlags.hpp"
#include "setupForMultiFit.hpp"
#include "alignField.hpp"
const std::string currentDateTime(BaseClass *);
int     polynomialFit(int, int, double *, double *,  double *, double *,  double *, double *);
using namespace std;

#ifdef METER //Provided by the compiler -D switch
#define WHICHMETER METER
#else
#define WHICHMETER m1m2kwh   //Set the Default
#endif
extern const char *insertIntoTable[];
extern const char *createTable[];     //From message.cpp
extern const char *readBackTable[];  //SQL SELECT for reading back the data that we previously inserted into table tbl_bucketized_by_tempf_ranges
extern const char *begin_Transaction;
extern const char *end_Transaction;
extern const char *fetchALLTransaction;
extern const char *close_Transaction;
extern const char *cases[]; //Array of strings specifying cases
extern const char *myTempFiles[]; //Array of the names of Temp Files for the m1m2kwh case, the m1kwh case, and the m2kwh case.

extern const char *copySQL;
extern const char *theMotherOfAllUnionSelectStatements[];

extern const char *motherOfAllSelectStatements[];
extern const char *copy_file[];
//extern const char *dropThenCreateTempTable;
extern const char *drop_tbl_temp_eu[];
extern const char *dropTable[];
extern const char *create_tbl_temp_eu[];
const std::string currentDateTime(BaseClass *);
void exit_nicely(BaseClass *,  int, int) ;
void exit_nicely(BaseClass *bc, int rc, int lineNumber) {
    PQfinish(bc->conn); //Close this task's connection.
    cout << currentDateTime(bc) << "\t" << __FILE__ << "Line " << lineNumber << "\nexit_nicely is terminating this program with exit code: " << rc << cases[bc->debugFlags.mycase] << endl;
    bc->selectFromTable_RC = rc; //Duplicate the exit code as selectFromTable_RC this was the mother task will know why the child exited
    exit (rc);
}
int createTheTable(BaseClass *, const char *, bool = true);
int dropTheTable(BaseClass *, const char *);
int closePortal(BaseClass *, const char *, bool = true);
int endTransaction(BaseClass *, const char *, bool = true);
int executeSQL(BaseClass *, const char *, bool = true);
int beginTransaction(BaseClass *, const char *, bool = true);
int fetchAllTransaction(BaseClass *, const char *, bool = false);
int insertBucketizedEnergyValuesIntoTable( double, double, double, double, double, int, BaseClass *);
void selectFromTable(register BaseClass *ptrbc, int myRC) {

    enum bucketColNames  {_avgtemp, _avgeu, _stddeveu, _mineu, _maxeu, _countu};  //This enum did not work when placed in BaseClass
    const char *theCases[] = {"m1m2kwh", "m1kwh", "m2kwh"};
    ptrbc->selectFromTable_RC = 0;
    ptrbc->conn = PQconnectdb(ptrbc->lookAtMyConnectionString);
    if (ptrbc->debugFlags.debug2 ) std::cout << theCases[ptrbc->debugFlags.intMyCase] << currentDateTime(ptrbc) << "\nThe mother of all select statements looks like:\n" << motherOfAllSelectStatements[ptrbc->debugFlags.intMyCase] << std::endl;
//============
//    const char * sqlStat0 = "SELECT ";
    //(8 +10)/2;
    
    ///8
//    const char *sqlStat2 = " AND avtempf <= ";
    //10;
    const char *sqlStat1 = " AS \"AvgIndependentVariable\", avg(%s) AS \"AvgDependentVariable\", stddev(%s) AS \"σOfDependentVariable\", min(%s) AS \"minDependentVariable\", max(%s) AS \"maxDependentVariable\", count(*) AS \"NumberOfDependentVariablesInThisBucket\" FROM tbl_energy_usage WHERE avtempf > ";



    
    ptrbc->len = strlen(motherOfAllSelectStatements[ptrbc->debugFlags.intMyCase]);
    ptrbc->sql = new char [ ptrbc->len ];
    ptrbc->len = sprintf(ptrbc->sql, motherOfAllSelectStatements[ptrbc->debugFlags.intMyCase], ptrbc->debugFlags.includeM2InAverages);  //At this point ptrbc->debugFlags.includeM2InAverages is either a 1 or a zero, depending upon the presences of the -c command line switch. If -c switch is NOT present then ptrbc->debugFlags.includeM2InAverages = 1, else its zero.

    ptrbc->len = strlen(sqlStat1);
    ptrbc->len += 4 * (11 + strlen ( ptrbc->debugFlags.dependentVariableColumn ) ); //m1m2kwh is the longest it can be
    ptrbc->subCharBuffer = new char[  ptrbc->len ];
    ptrbc->len = sprintf(ptrbc->subCharBuffer,sqlStat1, ptrbc->debugFlags.dependentVariableColumn, ptrbc->debugFlags.dependentVariableColumn, ptrbc->debugFlags.dependentVariableColumn, ptrbc->debugFlags.dependentVariableColumn);
//===============

 

    if (!ptrbc->debugFlags.tableCreated) {  //Have we created the table?
        try {
            ptrbc->rc[SELECTFROMTABLE] = dropTheTable(ptrbc, dropTable[ptrbc->debugFlags.intMyCase]);   //First drop the pre-existing table to get a fresh start.
        } catch (const std::exception &e) {
            std::cerr << "Failed to drop the table, tbl_bucketized_by_tempf_ranges. EXITING " << cases[ptrbc->debugFlags.intMyCase] << std::endl;
            exit(1);
        }
        try {
            if(createTheTable(ptrbc, createTable[ptrbc->debugFlags.intMyCase], true)) std::cerr << "Failed to create the table, tbl_bucketized_by_tempf_ranges. EXITING " << cases[ptrbc->debugFlags.intMyCase] << std::endl; //Go create table tbl_bucketized_by_tempf_ranges
        } catch (const std::exception &e) {
            std::cerr << "Failed to create the table, tbl_bucketized_by_tempf_ranges. EXITING " << cases[ptrbc->debugFlags.intMyCase] << std::endl;
            exit(2);
        }
        ptrbc->debugFlags.tableCreated = true;  //Indicate table created.
    }
//     MOVED TO myPrototypes   enum colnames {_currentavtempf,  _previousavtempf,  _davtempf,  _currentavtempk,  _previousavtempk,   _previousdate,   _currentdate, _dday,  _previousm1kwh, _currentm1kwh, _m1Usage, _previousm2kwh,  _currentm2kwh, _m2Usage, _AvgDailyUsage};


    ptrbc->res = PQexec(ptrbc->conn,  begin_Transaction);
    if (PQresultStatus(ptrbc->res) != PGRES_COMMAND_OK) {
        std::cerr << "cases[ptrbc->debugFlags.intMyCase]" <<"Failed to execute the SQL statement: " << begin_Transaction << ". Error cause:\n" << PQerrorMessage(ptrbc->conn) << std::endl;
        ptrbc->rc[SELECTFROMTABLE] = BEGINTRANSACTIONFAILED;
    }
    //    PQclear(ptrbc->res);

//    rc = executeSQL(ptrC, ptrbc->res, ptrBigSQL);
    ptrbc->res = PQexec(ptrbc->conn,  motherOfAllSelectStatements[ptrbc->debugFlags.intMyCase]);
    if (PQresultStatus(ptrbc->res) != PGRES_COMMAND_OK) {
        std::cerr << "Failed to execute the SQL statement: " << motherOfAllSelectStatements[ptrbc->debugFlags.intMyCase] << ". Error cause:\n" <<
        PQerrorMessage(ptrbc->conn) << cases[ptrbc->debugFlags.intMyCase] << std::endl;
        ptrbc->rc[SELECTFROMTABLE] = EXECUTESQLFAILED;
    }

    PQclear(ptrbc->res);
    if (ptrbc->rc[SELECTFROMTABLE]) exit_nicely (ptrbc, ptrbc->rc[SELECTFROMTABLE], __LINE__);
//    rc = fetchAllTransaction(ptrC,ptrbc->res, fetchALLTransaction);
    ptrbc->res = PQexec(ptrbc->conn,  "FETCH ALL in myportal" );
    if (PQresultStatus(ptrbc->res) != PGRES_TUPLES_OK) {
        std::cerr << "Failed to execute the SQL FETCH ALL: " << ptrbc->sql << ". Error cause:\n" <<
        PQerrorMessage(ptrbc->conn) << cases[ptrbc->debugFlags.intMyCase] << std::endl;
        ptrbc->rc[SELECTFROMTABLE] = FETCHALLFAILED;
        cerr << "Line " << __LINE__ << "Of file " << __FILE__ << "Failed when attempting to execute this sql:\n" << motherOfAllSelectStatements[ptrbc->debugFlags.intMyCase] << "\nWe got a return status of: " << PQresultStatus(ptrbc->res) << cases[ptrbc->debugFlags.intMyCase] << endl;
        exit_nicely(ptrbc, ptrbc->rc[SELECTFROMTABLE], __LINE__);
    }

    ptrbc->numberOfReturnedRows = PQntuples(ptrbc->res);
    ptrbc->numberOfReturnedColumns = PQnfields(ptrbc->res);
//    Align *af = new Align(ASSUMEDBUFFERSIZE);
    const char *headerRow[] ={" currentavtempf | prevavtempf |  ∆avtempf  | currentavtempk | prevavtempk |   yesterday    |   today    | ∆day | cm1kwh | pm1kwh | m1Usage | cm2kwh | pm2kwh | m2Usage | AvgDailyUsage",
                                "----------------+-------------+------------+----------------+-------------+----------------+------------+------+------- +------- +---------+------- +- ------+---------+---------------+"}; \
    /*  Example of what's returned by this mother of all select statements:
          col00           col01         col02          col03          col04          col05          col06     col07  col08   col09    col10    col11   col12   col13        col14
          16w              13w          12wide        16wide          13wide         16wide        12wide      6W      8w      7w      9w       7w     7w       9w         15w
     currentavtempf | prevavtempf |  ∆avtempf  | currentavtempk | prevavtempk |   yesterday    |   today    | ∆day | m1kwh | m1kwh | m1Usage | m2kwh | m2kwh | m2Usage | AvgDailyUsage
    ----------------+-------------+------------+----------------+-------------+----------------+------------+------+-------+-------+---------+-------+-------+---------+---------------
         64.4       |       38.86 |     -25.54 |         291.15 |      276.96 |     2014-12-03 | 2014-12-01 |    2 | 12776 | 12720 |      56 |  2670 |  2665 |       5 |          30.5
        46.15       |       38.86 |      -7.29 |         281.01 |      276.96 |     2014-12-03 | 2014-12-02 |    1 | 12776 | 12739 |      37 |  2670 |  2667 |       3 |            40
        38.86       |       39.27 |       0.41 |         276.96 |      277.19 |     2014-12-04 | 2014-12-03 |    1 | 12813 | 12776 |      37 |  2673 |  2670 |       3 |            40
        39.27       |       34.82 |      -4.45 |         277.19 |      274.71 |     2014-12-05 | 2014-12-04 |    1 | 12852 | 12813 |      39 |  2675 |  2673 |       2 |            41
        34.82       |       40.55 |       5.73 |         274.71 |      277.90 |     2014-12-06 | 2014-12-05 |    1 | 12890 | 12852 |      38 |  2678 |  2675 |       3 |            41
             .          .            .                     .          .                .          .           .       .          .      .      .           .               .
             .          .            .                     .          .                .          .           .       .          .      .      .           .               .
             .          .            .                     .          .                .          .           .       .          .      .      .           .               .

*/

    Align *af = new Align(ASSUMEDBUFFERSIZE);
    af->alignSetup(   headerRow[1]);
enum colwidths {col00 = 16, col01=13, col02=12, col03=16, col04=13, col05=16, col06=12, col07=6,col08=8, col09=8, col10=9, col11=8, col12=8, col13=9, col14=15};
#define NEITHERFIRSTORLAST 2 //This means we will not include the first elemet or the last element because they are misleading and erroneous: \
      a) The first element lacks the previous day's reading;  \
      b) Likely, the last element does not have a full-day's worth of readings.
    AllocateWorkingVectors *awv = new AllocateWorkingVectors (ptrbc->numberOfReturnedRows - NEITHERFIRSTORLAST);   /* << =================== awv object ================ */ //Strange, yet powerful, things these templates!!
    if (ptrbc->debugFlags.debug2 ) std::cout  <<headerRow[0] << "\t" << theCases[ptrbc->debugFlags.intMyCase] << std::endl;
    if (ptrbc->debugFlags.debug2 ) std::cout  <<headerRow[1] << "\t" << theCases[ptrbc->debugFlags.intMyCase] << std::endl;

    const size_t bufferSize = SIZEOFONEROW  *  ptrbc->numberOfReturnedRows;
#define THISROWISNULL 1
//    int ptrbc->numberOfReturnedRows;

    ptrbc->copyBuffer = new char[bufferSize]; //Get sufficient space to store, in one buffer, all temperture and energy usage (eu) data.
    memset(ptrbc->copyBuffer, 0, bufferSize); //Start with a clean copyBuffer
    for (ptrbc->loopCounter = 0; ptrbc->loopCounter < ptrbc->numberOfReturnedRows; ptrbc->loopCounter++ ) {
        if (PQgetisnull(ptrbc->res, ptrbc->loopCounter, _AvgDailyUsage) == THISROWISNULL) { //PQgetisnull Returns 1 if row IS NULL. Returns 0 if row is NOT NULL
            ++ptrbc->brc; //increment the bad row count.
            continue; //Skip NULL values of today's avgtempf and the first record and the last record
        }
        try {
            if (ptrbc->debugFlags.debug2 ) {
                cout << af->alignCenter(0,  PQgetvalue(ptrbc->res, ptrbc->loopCounter, _currentavtempf) ) << "|";
                cout << af->alignCenter(1,  PQgetvalue(ptrbc->res, ptrbc->loopCounter, _previousavtempf) ) << "|";
                cout << af->alignCenter(2,  PQgetvalue(ptrbc->res, ptrbc->loopCounter, _davtempf) ) << "|" ;
                cout << af->alignCenter(3,  PQgetvalue(ptrbc->res, ptrbc->loopCounter, _currentavtempk) ) << "|" ;
                cout << af->alignCenter(4,  PQgetvalue(ptrbc->res, ptrbc->loopCounter, _previousavtempk) ) << "|" ;
                cout << af->alignCenter(5,  PQgetvalue(ptrbc->res, ptrbc->loopCounter, _previousdate) ) << "|" ;
                cout << af->alignCenter(6,  PQgetvalue(ptrbc->res, ptrbc->loopCounter, _currentdate) ) << "|" ;
                cout << af->alignCenter(7,  PQgetvalue(ptrbc->res, ptrbc->loopCounter, _dday) ) << "|" ;
                cout << af->alignCenter(8,  PQgetvalue(ptrbc->res, ptrbc->loopCounter, _currentm1kwh) ) << "|" ;
                cout << af->alignCenter(9,  PQgetvalue(ptrbc->res, ptrbc->loopCounter, _previousm1kwh) ) << "|" ;
                cout << af->alignCenter(10, PQgetvalue(ptrbc->res, ptrbc->loopCounter, _m1Usage) ) << "|" ;
                cout << af->alignCenter(11, PQgetvalue(ptrbc->res, ptrbc->loopCounter, _currentm2kwh) ) << "|" ;
                cout << af->alignCenter(12, PQgetvalue(ptrbc->res, ptrbc->loopCounter, _previousm2kwh) ) << "|" ;
                cout << af->alignCenter(13, PQgetvalue(ptrbc->res, ptrbc->loopCounter, _m2Usage) ) << "|" ;
                cout << af->alignCenter(14, PQgetvalue(ptrbc->res, ptrbc->loopCounter, _AvgDailyUsage) ) << "|";
                cout << cases[ptrbc->debugFlags.intMyCase] << endl;
            }
            switch (ptrbc->debugFlags.mycase) {
                case M1M2KWH:
                    ptrbc->eu = PQgetvalue(ptrbc->res, ptrbc->loopCounter, _AvgDailyUsage);
                    awv->energyUsageVector[ptrbc->grc] = atof(ptrbc->eu);
                    break;
                case M1KWH:
                    ptrbc->eu = PQgetvalue(ptrbc->res, ptrbc->loopCounter, _m1Usage);
                    awv->energyUsageVector[ptrbc->grc]  = atof(ptrbc->eu);
                    break;
                case M2KWH:
                    ptrbc->eu = PQgetvalue(ptrbc->res, ptrbc->loopCounter, _m2Usage);
                    awv->energyUsageVector[ptrbc->grc]  = atof(ptrbc->eu);
                    break;
                default:
                    cerr << "Line " << __LINE__ << "File " << __FILE__ << "It seems like we picked up an undefined value for ptrbc->debugFlags.mycase: " << ptrbc->debugFlags.mycase << "\t We're terminating." << cases[ptrbc->debugFlags.intMyCase] << endl;
                    exit (BADMYCASEVALUE);
            }
            ptrbc->temp = PQgetvalue(ptrbc->res, ptrbc->loopCounter, _currentavtempf);
            if(strlen(ptrbc->temp) < 1) {
                continue; // skip this row if nothing returned because COPY doesn't like nothing there
            } else {
                awv->temperatureVector[ptrbc->grc]  = atof(ptrbc->temp);
            }
            strcpy(ptrbc->copyBuffer + ptrbc->bufferOffset, ptrbc->temp );
            ptrbc->bufferOffset += strlen(ptrbc->temp);
            *(ptrbc->copyBuffer+ptrbc->bufferOffset) = ','; //insert a comma to separate the fields.
            ptrbc->bufferOffset++;     //Accout for the comma we just added
            *(ptrbc->copyBuffer + ptrbc->bufferOffset) = ' '; //insert a comma to separate the fields.
//            strcpy(ptrbc->copyBuffer+ptrbc->bufferOffset, ", ");
            ptrbc->bufferOffset++; //strlen(","); //account for the space character  we added, above.
            strcpy(ptrbc->copyBuffer + ptrbc->bufferOffset, ptrbc->eu);  //ptrbc->eu is different for the m1m2kwh case than for the m1kwh and m2kwh cases. Process differently
            ptrbc->bufferOffset += strlen(ptrbc->eu); //Account for the eu data we just added
            *(ptrbc->copyBuffer+ptrbc->bufferOffset) = '\n'; //insert a new line character so postgres's COPY will understand that this is the end of the row.
//        strcpy(ptrbc->copyBuffer+ptrbc->bufferOffset, "\n");
            ptrbc->bufferOffset++; //strlen("\n"); account for the '\n' character
            *(ptrbc->copyBuffer + ptrbc->bufferOffset) = '\0'; //Terminate our growing string. Do we really have to do this since we did a massive memset just after allocating this dynamic instruction?
            //But don't update the ptrbc->bufferOffset to account for the above NULL character. We'll just over-write it with the temp data if it's there.
            awv->weightVector[ptrbc->grc] = atof(PQgetvalue(ptrbc->res, ptrbc->loopCounter, _dday));
            ptrbc->grc++; //increment the good row count
        } catch (const std::exception &e) {
            std::cerr << "On row: " << ptrbc->grc << ", we encountered some kind of conversion error, like trying to convert a null to an int or a float. [["<< e.what() << "]] Skipping this row, however processing continues." << cases[ptrbc->debugFlags.intMyCase] << std::endl;
            ++ptrbc->brc; //increment the bad row count.
        }
        ++ptrbc->trc_t; //Increment the total row count, includes valid and invalid rows.
        
    }  //End of for loop
    strcat(ptrbc->copyBuffer , "\\." );   //The COPY command likes to see `\.` as the final line.
    if (ptrbc->debugFlags.debug2) cout << "copyBuffer Looks Like: \n" << ptrbc->copyBuffer << endl;
    if (ptrbc->debugFlags.debug2) std::cout << "\nNumber of VALID Returned Rows: " << ptrbc->grc << ".\nNumber of INVALID Rows: " << ptrbc->brc << ".\nTotal Number of rows processed: " << ptrbc->grc + ptrbc->brc << ".\nNumber of unprocessed rows: " << ptrbc->numberOfReturnedRows-ptrbc->trc_t << "\n length of ptrbc->copyBuffer " << strlen(ptrbc->copyBuffer) << " bytes." << cases[ptrbc->debugFlags.intMyCase] << std::endl;
    
    if (ptrbc->debugFlags.debug2)  std::cout << "Successfully performed SELECT operation on all data.\nNow let's look at what we've put in the temperatureVector and the energyUsageVector." << cases[ptrbc->debugFlags.intMyCase] << std::endl;
    
    
    if (ptrbc->debugFlags.debug2 ) {
        for (ptrbc->k=0; ptrbc->k < ptrbc->numberOfReturnedRows - ptrbc->brc; ptrbc->k++) {
            
            std::cout << ptrbc->k << ". " << awv->temperatureVector[ptrbc->k] << ", " << awv->energyUsageVector[ptrbc->k] << cases[ptrbc->debugFlags.intMyCase] << std::endl;
        }
    }
    //Take a character --  as in ptrbc->debugFlags.mycase -- and turn \
    it into an integer, so arrays can use it as an index, by subtracting the character '0'
    ptrbc->k = (int)(ptrbc->debugFlags.mycase - '0');
    ptrbc->myTempFile.open(myTempFiles[ptrbc->k]);
    ptrbc->myTempFile  << ptrbc->copyBuffer  << endl;
    ptrbc->myTempFile.close();  //Close the output file
    ptrbc->allocateArrays(ptrbc->grc);
    for(ptrbc->loopCounter = 0; ptrbc->loopCounter<ptrbc->grc; ptrbc->loopCounter++ )  {
        ptrbc->setArray(IV, ptrbc->loopCounter, awv->temperatureVector[ ptrbc->loopCounter] ); //Place the independent variable, Average Daily Temperature, into its designated array.
        ptrbc->setArray(ODVA, ptrbc->loopCounter, awv->energyUsageVector[ ptrbc->loopCounter] ); //Place the observed dependent variable (EnergyUsage as Read (Observed) from the electric meters into its array.

    }

    //WE will  now close the portal and end the transaction because I don't think we can do a create table on the ptrC connection with a transaction in progress.
    ptrbc->rc[SELECTFROMTABLE] = closePortal(ptrbc , "CLOSE myportal");  //Close myportal before ending the transaction. closePortal does a ¡PQclear! ;

    ptrbc->rc[SELECTFROMTABLE] = endTransaction(ptrbc, "END");  //End the transaction. endTransaction does a ¡PQclear!

    //Create a temporary table named tbl_temp_eux; it is here where we will mirror on a temporary table the data that went into the temperature vector and the energy usage vector, above.
    if (!ptrbc->debugFlags.tempTableCreated) {  //Have we created the temporary table?
        //We will do the DROP Table and Create Table with one SQL statement
        ptrbc->res = PQexec(ptrbc->conn, drop_tbl_temp_eu[ptrbc->debugFlags.intMyCase]);
        if (PQresultStatus(ptrbc->res) != PGRES_COMMAND_OK ) {
            cerr << drop_tbl_temp_eu[ptrbc->debugFlags.intMyCase] << " command failed " << cases[ptrbc->debugFlags.intMyCase] << endl;
            exit_nicely(ptrbc, DROPTABLEFAILED, __LINE__);
        }
        PQclear(ptrbc->res);
        ptrbc->res = PQexec(ptrbc->conn, create_tbl_temp_eu[ptrbc->debugFlags.intMyCase] );
        if (PQresultStatus(ptrbc->res) != PGRES_COMMAND_OK ) {
            cerr << create_tbl_temp_eu[ptrbc->debugFlags.intMyCase] << " command failed " << cases[ptrbc->debugFlags.intMyCase] << endl;
            exit_nicely(ptrbc, CREATETABLEFAILED, __LINE__);
        }
        PQclear(ptrbc->res);
        ptrbc->debugFlags.tempTableCreated = true;  //Indicate table created.
    }
// BEGINNING OF COPY OPERATION
// No Longer Used    const char *errmsg=NULL;
    ptrbc->res = PQexec(ptrbc->conn,  copy_file[ptrbc->debugFlags.intMyCase] ); //Execute the SQL statement: COPY tbl_temp_eu (temp, eu) STDIN;
    ptrbc->resultsPQputCopyData = PQputCopyData(ptrbc->conn, ptrbc->copyBuffer, (int)strlen(ptrbc->copyBuffer));
    ptrbc->resultsPQputCopyEnd = PQputCopyEnd(ptrbc->conn, ptrbc->errmsg);
//    int whichIsIt = PQresultStatus(ptrbc->res);
    if (ptrbc->debugFlags.debug2 )  cout << " resultsPQputCopyData: " << ptrbc->resultsPQputCopyData << "; resultsPQputCopyEnd: " << ptrbc->resultsPQputCopyEnd << cases[ptrbc->debugFlags.intMyCase] << endl;
    
    if (ptrbc->errmsg) {
        cerr << "COPY operation failed with error message of: " << ptrbc->errmsg << cases[ptrbc->debugFlags.intMyCase] << endl;
    } else {
        if (ptrbc->debugFlags.debug2 ) cout << "COPY operation seems to have worked!" << cases[ptrbc->debugFlags.intMyCase] << endl;

    PQclear(ptrbc->res);
    }
//END OF COPY OPERATION

    ptrbc->rc[SELECTFROMTABLE] = beginTransaction(ptrbc, begin_Transaction);  //Start up a new transaction where we will
    if (ptrbc->rc[SELECTFROMTABLE]) exit_nicely(ptrbc, ptrbc->rc[SELECTFROMTABLE], __LINE__);

    ptrbc->rc[SELECTFROMTABLE] = executeSQL(ptrbc, theMotherOfAllUnionSelectStatements[ptrbc->debugFlags.intMyCase] );   //
    if (ptrbc->rc[SELECTFROMTABLE]) exit_nicely(ptrbc, ptrbc->rc[SELECTFROMTABLE], __LINE__);
    ptrbc->rc[SELECTFROMTABLE] = fetchAllTransaction(ptrbc, fetchALLTransaction);
    if (ptrbc->rc[SELECTFROMTABLE]) { //Note that PGRES_TUPLES_OK happens when PQresultStatus returns a 2.
        cerr << "Failed when attempting to do a FETCH ALL execute this sql:\n" << theMotherOfAllUnionSelectStatements[ptrbc->debugFlags.intMyCase] << "\nWe got a return status of: " << PQresultStatus(ptrbc->res) <<  cases[ptrbc->debugFlags.intMyCase] <<  endl;
        exit_nicely(ptrbc, ptrbc->rc[SELECTFROMTABLE], __LINE__);
    } else {
        ptrbc->numberOfReturnedRows = PQntuples(ptrbc->res);
        ptrbc->numberOfReturnedColumns = PQnfields(ptrbc->res);
        if (ptrbc->debugFlags.debug2 ) cout << "TheMotherOfAllUnionSelectStatements returned " << ptrbc->numberOfReturnedRows << " rows and " <<    ptrbc->numberOfReturnedColumns << " columns." << cases[ptrbc->debugFlags.intMyCase] <<  endl;
    }

    if (ptrbc->debugFlags.debug2 ) cout << "Now bucketize the data. Each bucket has a temperature range of " << BUCKETSIZE << "ºF " << " and we will get the average energy usage, in kwh, for that temperature range buffer along with the energy usage standard deviation. " << cases[ptrbc->debugFlags.intMyCase] << endl;
    ptrbc->sqlbuffer= new char[SIZEOFBUFFERCONTAININGMOTHEROFALLSELECTSTATEMNTS];   /* << =================================== Dynamically acquire sufficient storage for constructing, piece-by-piece, the Mother Of All Select Statements. */

    
    const char *headerEU[2] = {"  avgtemp | avg eu |      stddev       |  min  |  max  | count\n", \
                                 "----------+--------+-------------------+-------+-------+-------\n"};
    if (ptrbc->debugFlags.debug2 ) cout << *(headerEU +0) << *(headerEU + 1) << cases[ptrbc->debugFlags.intMyCase] << std::endl; //Output the headers.//
    
    Align *a_ff = new Align(ASSUMEDBUFFERSIZE);
    a_ff->alignSetup( headerEU[1]);
    
    ptrbc->loopCounter = 0; //Reset this.
    while ( ptrbc->buckettemp < ptrbc->maxBucketTemp && ptrbc->loopCounter < ptrbc->numberOfReturnedRows ) {  //Beginning of while statement.
        // See http://libpqxx.readthedocs.io/en/latest/a00004.html for explanation of how the next two lines work

        if ( ( PQgetisnull(ptrbc->res, ptrbc->loopCounter, _countu) ) ) { //Weed-out any bad records that contain poisonous fields.
            
            ++ptrbc->loopCounter;
            continue; //Skip this record if any critical values come back as NULL. Go back to top of while loop.
        }
        //            for (result::const_iterator column = r.rbegin(); column != r.rend(); ++column) { //Beginning of for loop

        try {
            if (ptrbc->debugFlags.debug2 ) {

                std::cout << a_ff->alignCenter(0,  PQgetvalue(ptrbc->res, ptrbc->loopCounter, _avgtemp) ) << "|";
                std::cout << a_ff->alignCenter(1,  PQgetvalue(ptrbc->res, ptrbc->loopCounter, _avgeu) ) << "|";
                std::cout << a_ff->alignCenter(2,  PQgetvalue(ptrbc->res, ptrbc->loopCounter, _stddeveu) ) << "|";
                std::cout << a_ff->alignCenter(3,  PQgetvalue(ptrbc->res, ptrbc->loopCounter, _mineu) ) << "|";
                std::cout << a_ff->alignCenter(4,  PQgetvalue(ptrbc->res, ptrbc->loopCounter, _maxeu) ) << "|";
                std::cout << a_ff->alignCenter(5,  PQgetvalue(ptrbc->res, ptrbc->loopCounter, _countu) ) << "|" <<  cases[ptrbc->debugFlags.intMyCase] << endl;
            }
            if( (ptrbc->thisCount =atoi( PQgetvalue(ptrbc->res, ptrbc->loopCounter, _countu) ) ) > 1 ) {
                ptrbc->values[_avgtemp] = atof(PQgetvalue(ptrbc->res, ptrbc->loopCounter, _avgtemp) );
                ptrbc->values[_avgeu]= atof(PQgetvalue(ptrbc->res, ptrbc->loopCounter, _avgeu));
                ptrbc->values[_stddeveu] =  atof( PQgetvalue(ptrbc->res, ptrbc->loopCounter, _stddeveu) );
                ptrbc->values[_mineu] = atof(PQgetvalue(ptrbc->res, ptrbc->loopCounter, _mineu));
                ptrbc->values[_maxeu] = atof(PQgetvalue(ptrbc->res, ptrbc->loopCounter, _maxeu) );
                ptrbc->thisCount = atoi(PQgetvalue(ptrbc->res, ptrbc->loopCounter, _countu)); 
                ++ptrbc->worthyOfPolynomialFitProcessing;
                ++ptrbc->debugFlags.countOfGoodEntries;
                //INSERT the values we extracted into table tbl_bucketized_by_tempf_ranges. NOW THAT WE HAVE COPY WORKIN, PERHAPS WE CAN USE COPY RATHER THAN INSERT INTO … BLAH, BLAH …
                ptrbc->rc[SELECTFROMTABLE] = insertBucketizedEnergyValuesIntoTable( ptrbc->values[_avgtemp], ptrbc->values[_avgeu], ptrbc->values[_stddeveu],  ptrbc->values[_mineu], ptrbc->values[_maxeu], ptrbc->thisCount, ptrbc);
                if (ptrbc->rc[SELECTFROMTABLE]) {
                    cerr << "Operation to INSERT INTO tbl_bucketized_by_tempf_ranges  F A I L E D!" << cases[ptrbc->debugFlags.intMyCase] << std::endl;
                    exit_nicely(ptrbc, ptrbc->rc[SELECTFROMTABLE], __LINE__);
                }
            } else { //Come here if fewer than 2 counts
                ++ptrbc->loopCounter;
                continue; //skip this record because it's no good, not enough data for a standard deviation. Go on to do the next record.
            }
        } catch (const std::exception &e) {
            ++ptrbc->debugFlags.countOfBadEntries;
            std::cerr << __FILE__ "2. bad values try/catch detected errors for entry number " << ptrbc->debugFlags.countOfBadEntries + ptrbc->countOfGoodEntries << cases[ptrbc->debugFlags.intMyCase] << std::endl;
        }  //end of try/catch block
        
        ptrbc->buckettemp += BUCKETSIZE;
        ++ptrbc->loopCounter;
    }  //End of while loop
    ptrbc->rc[SELECTFROMTABLE] = closePortal(ptrbc , close_Transaction);  //Close myportal before ending the transaction
    ptrbc->rc[SELECTFROMTABLE] = endTransaction(ptrbc , end_Transaction);  //Close myportal before ending the transaction
// =================================================================
    
    if (ptrbc->worthyOfPolynomialFitProcessing > POLYNOMIALDEGREE) {


        try {  //Attempt to read back the data we just inserted into table tbl_bucketized_by_tempf_ranges
            ptrbc->rc[SELECTFROMTABLE] = beginTransaction(ptrbc, begin_Transaction);
            if (ptrbc->rc[SELECTFROMTABLE]) exit_nicely(ptrbc, ptrbc->rc[SELECTFROMTABLE], __LINE__);
            ptrbc->rc[SELECTFROMTABLE] = executeSQL(ptrbc, readBackTable[ptrbc->debugFlags.intMyCase]);  //We might want to order by temp.
            if (ptrbc->rc[SELECTFROMTABLE]) exit_nicely(ptrbc, ptrbc->rc[SELECTFROMTABLE], __LINE__);
            ptrbc->rc[SELECTFROMTABLE] = fetchAllTransaction(ptrbc, fetchALLTransaction);

            if (ptrbc->rc[SELECTFROMTABLE]) {
                if (ptrbc->debugFlags.debug2 ) cout << "FETCH ALL for SELECT statement\n" << readBackTable[ptrbc->debugFlags.intMyCase] << "\nfailed with error: " << PQerrorMessage(ptrbc->conn) << cases[ptrbc->debugFlags.intMyCase] << endl;
            } else {
                ptrbc->numberOfReturnedRows = PQntuples(ptrbc->res);
                ptrbc->numberOfReturnedColumns = PQnfields(ptrbc->res);
            }

            if (ptrbc->numberOfReturnedRows) {
                if (ptrbc->numberOfReturnedRows != ptrbc->worthyOfPolynomialFitProcessing) {
                    std::cerr << "For some unknown reason, the numberOfReturnedRows: " << ptrbc->numberOfReturnedRows << " AND values worthyOfPolynomialFitProcessing: " << ptrbc->worthyOfPolynomialFitProcessing << " have different values. " << cases[ptrbc->debugFlags.intMyCase] << std::endl;
                }
                SetupForMultiFit  *smf = new SetupForMultiFit(ptrbc, POLYNOMIALDEGREE, ptrbc->worthyOfPolynomialFitProcessing, ptrbc->ptrValues);   /* << =================================== */ //First, establish an instance of the SetupForMultiFit object.

                for  (ptrbc->loopCounter = 0, ptrbc->rowBeingProcessed = 0; ptrbc->loopCounter < ptrbc->worthyOfPolynomialFitProcessing; ++ptrbc->loopCounter, ++ptrbc->rowBeingProcessed)
                { //Begin loading the gsl matrices and vectors with values.
                    ptrbc->values[_avgtemp] =atof(PQgetvalue(ptrbc->res, ptrbc->rowBeingProcessed, _avgtemp) ) ;   //Considered the independent Variable
                    ptrbc->values[_avgeu]= atof(PQgetvalue(ptrbc->res, ptrbc->rowBeingProcessed, _avgeu));        //Considered the dependent Variable
                    ptrbc->values[_stddeveu] = atof( PQgetvalue(ptrbc->res, ptrbc->rowBeingProcessed, _stddeveu) ); //Considered the weight.
                    ptrbc->values[_mineu] = atof(PQgetvalue(ptrbc->res, ptrbc->rowBeingProcessed, _mineu));
                    ptrbc->values[_maxeu] =atof(PQgetvalue(ptrbc->res, ptrbc->rowBeingProcessed, _maxeu) );
                    ptrbc->thisCount = atoi(PQgetvalue(ptrbc->res, ptrbc->rowBeingProcessed, _countu));
                    smf->captureIndependentVariableValues( ptrbc->rowBeingProcessed, ptrbc->values );    //Now, put into the object the data that will go into the independent variable matrix (_avgtemp), the dependent variable vector (_avgeu), and the weighting
                    smf->setIntoVector(smf->dependentVariable, ptrbc->rowBeingProcessed, *(ptrbc->values + _avgeu) ); //Put the depenedent variable into its vector.
                    smf->setIntoVector(smf->weights, ptrbc->rowBeingProcessed, 1/( *(ptrbc->values + _stddeveu) * *(ptrbc->values + _stddeveu)  ) ); //Put the weight (stddev) into it's vector. See top 0f page 451 of version 1.12 of GNU Scientific Library Reference Manual (3rd Edition) for the `1/( *(values + _stddeveu) * *(values + _stddeveu)  )` piece of the processing

// NO LONGER NEEDED                    ++rowBeingProcessed;
                } //End of for loop
   //             PQclear(ptrbc->res);  //Get this messag: "WARNING:  there is no transaction in progress" because we closed the myportal cursor and ended the transaction
                ptrbc->rc[SELECTFROMTABLE] = closePortal(ptrbc , end_Transaction);  //Close myportal
//  KILL THE WARNING MESSAGE                ptrbc->rc[SELECTFROMTABLE] = endTransaction(ptrbc, end_Transaction);  //Get another `WARNING:  there is no transaction in progress`
                
                ptrbc->rc[SELECTFROMTABLE] = smf->doMultiFit( ); //Actually go out and do the gsl_multifit
                ptrbc->rc[SELECTFROMTABLE] = smf->outputPolynomial("EnergyUsage(T) = ", cases[ptrbc->debugFlags.intMyCase]);
                *ptrbc->outstring << smf->outputCovarianceMatrix("cov = [", cases[ptrbc->debugFlags.intMyCase]) << "☜ χ-squared" << cases[ptrbc->debugFlags.intMyCase] << "\n";
                *ptrbc->outstring << smf->computeTrace(smf->covarienceMatrix->data, (int)smf->onePlusPolynomialDegree) << "☜ Trace of Covariance Matrix" << cases[ptrbc->debugFlags.intMyCase]  << "\n";
                *ptrbc->outstring << smf->computeCorrelationBetweenIndependentAndDependentVariables(ptrbc) << "☜ correlation coefficient" << cases[ptrbc->debugFlags.intMyCase] << "\n";
                ptrbc->squareRootOfSumOfTheSquaresOfTheVarience = smf->computeGoodnessOfResults(ptrbc);
                ptrbc->averageVariencePerValidReading = ptrbc->squareRootOfSumOfTheSquaresOfTheVarience/ptrbc->grc;
                
                *ptrbc->outstring << ptrbc->squareRootOfSumOfTheSquaresOfTheVarience << "☜ Square Root Of Sum Of TheSquares Of The Varience; \n" << ptrbc->averageVariencePerValidReading << "☜ Average Varience Per Valid Reading\n" << ptrbc->averageVariencePerValidReading*100 << "☜ \% Average Varience Per Valid Reading\n"<< ptrbc->grc << "☜ Number of valid meter readings (# observed dependent variables)" <<  cases[ptrbc->debugFlags.intMyCase] << "\n";

            } else {
                std::cerr << "Although the SELECT seems to have worked, we got " << ptrbc->numberOfReturnedRows << " rows back from the table which we thought we just inserted data into!" << cases[ptrbc->debugFlags.intMyCase] << std::endl;
            }
        } catch (const std::exception &e) {
            std::cerr << "Some how we failed to get any data back from the table which we thought we just inserted data into!" << cases[ptrbc->debugFlags.intMyCase] << std::endl;
        }
 
        
        //        rc = polynomialFit(POLYNOMIALDEGREE, grc, awv->temperatureVector, awv->energyUsageVector,  NULL, NULL,  &TSS, &goodnessOfFit);
    } else {
        std::cerr << "Insufficient number of VALID datapoints to do MultiFit processing for desired polynomial of degree " << POLYNOMIALDEGREE << cases[ptrbc->debugFlags.intMyCase] << std::endl;
    }
    if (ptrbc->sql != nullptr) delete [] ptrbc->sql;
    if (ptrbc->copyBuffer != nullptr) delete [] ptrbc->copyBuffer;
    if (ptrbc->subCharBuffer != nullptr) delete [] (ptrbc->subCharBuffer);   //c++'s Equivalent to C's free when we allocate storage with a calloc/malloc. Note the use of [] prior to the (subCharacterBuffer);
    if (ptrbc->sqlbuffer != nullptr) delete[](ptrbc->sqlbuffer);       //Delete another dynamically acquired character buffer.
    if (a_ff != nullptr)  {
        a_ff->~Align();
//        delete a_ff;
    }
    if ( af != nullptr) {
         af->~Align();
//        delete af;
    }
//    UPDATE tbl_energy_usage set avtempf = (SELECT avg(temperature) FROM tbl_l WHERE siteid=5 AND lt BETWEEN '2018-05-06 08:17:00' AND '2018-05-07 07:00:00' ), avtempk=SELECT (5*(avg(temperature)-32)/9+273.15 ) FROM tbl_l WHERE  siteid=5 AND lt BETWEEN '2018-05-06 08:17:00' AND '2018-05-07 07:00:00' ), siteid=5 WHERE id=2877;
    PQfinish(ptrbc->conn);  //Since this routine did the connection, this  routine must do the PQfinish to disconnect from the database.
}
