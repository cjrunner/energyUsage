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
#include "/usr/local/pgsql/pgsql101/include/libpq-fe.h"
#include "myPrototypes.hpp"
#include "AllocateWorkingVectors.hpp"
#include "baseClass.hpp"
#include "insertIBucketizedEnergyValuesIntoTable.hpp"
#include "BitFlags.hpp"
#include "setupForMultiFit.hpp"
#include "alignField.hpp"
int selectFromTable(PGconn *,  BaseClass *, const char *);
int     polynomialFit(int, int, double *, double *,  double *, double *,  double *, double *);
using namespace std;

#ifdef METER //Provided by the compiler -D switch
#define WHICHMETER METER
#else
#define WHICHMETER m1m2kwh   //Set the Default
#endif
extern const char *insertIntoTable;
extern const char *createTable;     //From message.cpp
extern const char *readBackTable;  //SQL SELECT for reading back the data that we previously inserted into table tbl_bucketized_by_tempf_ranges
extern const char *begin_Transaction;
extern const char *end_Transaction;
extern const char *fetchALLTransaction;
extern const char *close_Transaction;

extern const char *insertIntoTmpTable;
extern const char *copySQL;
extern const char *theMotherOfAllUnionSelectStatements;
extern const char *copy_STDIN_SQL;
//extern const char *dropThenCreateTempTable;
extern const char *drop_tbl_temp_eu;
extern const char *create_tbl_temp_eu;

void exit_nicely(PGconn *, int, int) ;
void exit_nicely(PGconn *conn, int rc, int lineNumber) {
    PQfinish(conn);
    cout << __TIMESTAMP__ << "\t" << __FILE__ << "Line " << lineNumber << "\nexit_nicely is terminating this program with exit code: " << rc << endl;
    exit (rc);
}
int closePortal(PGconn *, BaseClass *, const char *, bool = true);
int endTransaction(PGconn *, BaseClass *, const char *, bool = true);
int executeSQL(PGconn *, BaseClass *, const char *, bool = true);
int beginTransaction(PGconn *, BaseClass *, const char *, bool = true);
int fetchAllTransaction(PGconn *, BaseClass *, const char *, bool = false);
int insertBucketizedEnergyValuesIntoTable( double, double, double, double, double, int, BaseClass *);
int selectFromTable(PGconn *ptrC, BaseClass *ptrbc, const char *ptrBigSQL) {

//    double julianDate = 2458243.209030; unused
    int rc=WONDERFUL;
    int grc=INITIALVALUE;
    int brc=INITIALVALUE;
//    int trc=INITIALVALUE;
    int thisCount=INITIALVALUE;
    int worthyOfPolynomialFitProcessing = INITIALVALUE;
 
    double values[8]={INITIALVALUE};
    double *ptrValues;
    ptrValues = &values[0];
    /*
    JulianToGregorian *JTG;
    { //START  define a new scope so we can see the destructor do its thing when we go out of scope and the pointer named `jtg` will no longer be valid
        JulianToGregorian *jtg =new JulianToGregorian(julianDate);

        strcpy(result, jtg->ptrResultingGregorian);
        strcpy(result2, jtg->ptrResultingGregorian);
        JTG = jtg;
        ;   // Just a no-op
    } //END past this point the pointer `jtg` is no longer valid
     */
    if(ptrbc->debugFlags.debug2) std::cout << __TIMESTAMP__ << "\nThe mother of all select statements looks like:\n" << ptrBigSQL << std::endl;
//============
//    const char * sqlStat0 = "SELECT ";
    //(8 +10)/2;
    
    ///8
//    const char *sqlStat2 = " AND avtempf <= ";
    //10;
    const char *sqlStat1 = " AS \"AvgIndependentVariable\", avg(%s) AS \"AvgDependentVariable\", stddev(%s) AS \"σOfDependentVariable\", min(%s) AS \"minDependentVariable\", max(%s) AS \"maxDependentVariable\", count(*) AS \"NumberOfDependentVariablesInThisBucket\" FROM tbl_energy_usage WHERE avtempf > ";
    size_t len;


    len = strlen(ptrBigSQL);
    char *sql = new char [ len ];
    len = sprintf(sql, ptrBigSQL, ptrbc->debugFlags.includeM2InAverages);  //At this point ptrbc->debugFlags.includeM2InAverages is either a 1 or a zero, depending upon the presences of the -c command line switch. If -c switch is NOT present then ptrbc->debugFlags.includeM2InAverages = 1, else its zero.

    len = strlen(sqlStat1) + 4 * (11 + strlen(ptrbc->debugFlags.dependentVariableColumn ) );
    char *subCharBuffer = new char[  len ];
    len = sprintf(subCharBuffer,sqlStat1, ptrbc->debugFlags.dependentVariableColumn, ptrbc->debugFlags.dependentVariableColumn, ptrbc->debugFlags.dependentVariableColumn, ptrbc->debugFlags.dependentVariableColumn);
//===============



    if (!ptrbc->debugFlags.tableCreated) {  //Have we created the table?
        try {
            rc = dropTheTable("DROP TABLE IF EXISTS tbl_bucketized_by_tempf_ranges", ptrC);   //First drop the pre-existing table to get a fresh start.
        } catch (const std::exception &e) {
            std::cerr << "Failed to drop the table, tbl_bucketized_by_tempf_ranges. EXITING " << std::endl;
            exit(1);
        }
        try {
            rc = createTheTable(createTable, ptrC); //Go create table tbl_bucketized_by_tempf_ranges
        } catch (const std::exception &e) {
            std::cerr << "Failed to create the table, tbl_bucketized_by_tempf_ranges. EXITING " << std::endl;
            exit(2);
        }
        ptrbc->debugFlags.tableCreated = true;  //Indicate table created.
    }
//     MOVED TO myPrototypes   enum colnames {_currentavtempf,  _previousavtempf,  _davtempf,  _currentavtempk,  _previousavtempk,   _previousdate,   _currentdate, _dday,  _previousm1kwh, _currentm1kwh, _m1Usage, _previousm2kwh,  _currentm2kwh, _m2Usage, _AvgDailyUsage};
    int numberOfReturnedRows = 0;
    int numberOfReturnedColumns =  0;


    ptrbc->res = PQexec(ptrC,  begin_Transaction);
    if (PQresultStatus(ptrbc->res) != PGRES_COMMAND_OK) {
        std::cerr << "Failed to execute the SQL statement: " << sql << ". Error cause:\n" <<
        PQerrorMessage(ptrC) << std::endl;
        rc = BEGINTRANSACTIONFAILED;
    }
    //    PQclear(ptrbc->res);

//    rc = executeSQL(ptrC, ptrbc->res, ptrBigSQL);
    ptrbc->res = PQexec(ptrC,  ptrBigSQL);
    if (PQresultStatus(ptrbc->res) != PGRES_COMMAND_OK) {
        std::cerr << "Failed to execute the SQL statement: " << ptrBigSQL << ". Error cause:\n" <<
        PQerrorMessage(ptrC) << std::endl;
        rc = EXECUTESQLFAILED; 
    }
    //The next two items only get populted after we've done a FETCH ALL
    numberOfReturnedRows = PQntuples(ptrbc->res);
    numberOfReturnedColumns = PQnfields(ptrbc->res);
    PQclear(ptrbc->res);
    if (rc) exit_nicely (ptrC, rc, __LINE__);
//    rc = fetchAllTransaction(ptrC,ptrbc->res, fetchALLTransaction);
    ptrbc->res = PQexec(ptrC,  "FETCH ALL in myportal" );
    if (PQresultStatus(ptrbc->res) != PGRES_TUPLES_OK) {
        std::cerr << "Failed to execute the SQL FETCH ALL: " << sql << ". Error cause:\n" <<
        PQerrorMessage(ptrC) << std::endl;
        rc = FETCHALLFAILED;
        cerr << "Line " << __LINE__ << "Of file " << __FILE__ << "Failed when attempting to execute this sql:\n" << ptrBigSQL << "\nWe got a return status of: " << PQresultStatus(ptrbc->res) <<endl;
        exit_nicely(ptrC, rc, __LINE__);
    }

    numberOfReturnedRows = PQntuples(ptrbc->res);
    numberOfReturnedColumns = PQnfields(ptrbc->res);
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
    const char *hr = headerRow[1];
    Align *af = new Align(ASSUMEDBUFFERSIZE, hr);
enum colwidths {col00 = 16, col01=13, col02=12, col03=16, col04=13, col05=16, col06=12, col07=6,col08=8, col09=8, col10=9, col11=8, col12=8, col13=9, col14=15};
#define NEITHERFIRSTORLAST 2 //This means we will not include the first elemet or the last element because they are erroneous: The first elemet does lacks the previous day's reading; the last element likely does not have a full-day's worth of readings.
    AllocateWorkingVectors *awv = new AllocateWorkingVectors (numberOfReturnedRows-NEITHERFIRSTORLAST);   /* << =================== awv object ================ */ //Strange, yet powerful, things these templates!!
    if (ptrbc->debugFlags.debug2 ) std::cout << headerRow[0] << std::endl;
    if (ptrbc->debugFlags.debug2 ) std::cout << headerRow[1] << std::endl;
    int loopCounter;
 //   char *aligned;
    size_t bufferOffset = 0;
    char *temp;
    char *eu;
    const size_t bufferSize = SIZEOFONEROW * numberOfReturnedRows;
    char *copyBuffer = new char[bufferSize]; //Get sufficient space to store all temperture and eu data in one buffer.
    memset(copyBuffer, 0, bufferSize); //Start with a clean copyBuffer
    for (loopCounter = 0; loopCounter < numberOfReturnedRows; loopCounter++ ) {
        if (PQgetisnull(ptrbc->res, loopCounter, _currentavtempf) || loopCounter == 0 || loopCounter == numberOfReturnedRows-1 || ( ( atof(PQgetvalue(ptrbc->res, loopCounter, _currentavtempf) )== 0) && ( (atof(PQgetvalue(ptrbc->res, loopCounter, _AvgDailyUsage)) == 0 ) ) ) ){
            ++ptrbc->brc; //increment the bad row count.
            continue; //Skip NULL values of today's avgtempf and the first record and the last record
        }
        try {
            if (ptrbc->debugFlags.debug2 ) {
                cout << af->alignCenter(0,  PQgetvalue(ptrbc->res, loopCounter, _currentavtempf) ) << "|";
                cout << af->alignCenter(1,  PQgetvalue(ptrbc->res, loopCounter, _previousavtempf) ) << "|";
                cout << af->alignCenter(2,  PQgetvalue(ptrbc->res, loopCounter, _davtempf) ) << "|" ;
                cout << af->alignCenter(3,  PQgetvalue(ptrbc->res, loopCounter, _currentavtempk) ) << "|" ;
                cout << af->alignCenter(4,  PQgetvalue(ptrbc->res, loopCounter, _previousavtempk) ) << "|" ;
                cout << af->alignCenter(5,  PQgetvalue(ptrbc->res, loopCounter, _previousdate) ) << "|" ;
                cout << af->alignCenter(6,  PQgetvalue(ptrbc->res, loopCounter, _currentdate) ) << "|" ;
                cout << af->alignCenter(7,  PQgetvalue(ptrbc->res, loopCounter, _dday) ) << "|" ;
                cout << af->alignCenter(8,  PQgetvalue(ptrbc->res, loopCounter, _currentm1kwh) ) << "|" ;
                cout << af->alignCenter(9,  PQgetvalue(ptrbc->res, loopCounter, _previousm1kwh) ) << "|" ;
                cout << af->alignCenter(10,  PQgetvalue(ptrbc->res, loopCounter, _m1Usage) ) << "|" ;
                cout << af->alignCenter(11,  PQgetvalue(ptrbc->res, loopCounter, _currentm2kwh) ) << "|" ;
                cout << af->alignCenter(12, PQgetvalue(ptrbc->res, loopCounter, _previousm2kwh) ) << "|" ;
                cout << af->alignCenter(13,  PQgetvalue(ptrbc->res, loopCounter, _m2Usage) ) << "|" ;
                cout << af->alignCenter(14,  PQgetvalue(ptrbc->res, loopCounter, _AvgDailyUsage) ) << "|" << endl;
            }
            switch (ptrbc->debugFlags.mycase) {
                case M1M2KWH:
                    eu = PQgetvalue(ptrbc->res, loopCounter, _AvgDailyUsage);
                    awv->energyUsageVector[ptrbc->grc] = atof(eu);
                    break;
                case M1KWH:
                    eu = PQgetvalue(ptrbc->res, loopCounter, _m1Usage);
                    awv->energyUsageVector[ptrbc->grc]  = atof(eu);
                    break;
                case M2KWH:
                    eu = PQgetvalue(ptrbc->res, loopCounter, _m2Usage);
                    awv->energyUsageVector[ptrbc->grc]  = atof(eu);
                    break;
                default:
                    cerr << "Line " << __LINE__ << "File " << __FILE__ << "It seems like we picked up an undefined value for ptrbc->debugFlags.mycase: " << ptrbc->debugFlags.mycase << "\t We're terminating." << endl;
                    exit (BADMYCASEVALUE);
            }
            temp = PQgetvalue(ptrbc->res, loopCounter, _currentavtempf);
            awv->temperatureVector[ptrbc->grc]  = atof(temp);
            strcpy(copyBuffer+bufferOffset, temp );
            bufferOffset += strlen(temp);
            *(copyBuffer+bufferOffset) = ','; //insert a comma to separate the fields.
            bufferOffset++;     //Accout for the comma we just added
            *(copyBuffer+bufferOffset) = ' '; //insert a comma to separate the fields.
//            strcpy(copyBuffer+bufferOffset, ", ");
            bufferOffset++; //strlen(","); //account for the cspace character  we added, above.
            strcpy(copyBuffer+bufferOffset, eu);
            bufferOffset += strlen(eu);
            *(copyBuffer+bufferOffset) = '\n'; //insert a new line character so postgres's COPY will understand that this is the end of the row.
//        strcpy(copyBuffer+bufferOffset, "\n");
            bufferOffset++; //strlen("\n"); account for the '\n' character
            *(copyBuffer+bufferOffset) = '\0'; //Terminate our growing string. Do we really have to do this since we did a massive memset just after allocating this dynamic instruction?
            //But don't update the bufferOffset to account for the above NULL character.
            awv->weightVector[ptrbc->grc] = atof(PQgetvalue(ptrbc->res, loopCounter, _dday));
            ptrbc->grc++; //increment the good row count
        } catch (const std::exception &e) {
            std::cerr << "On row: " << grc << ", we encountered some kind of conversion error, like trying to convert a null to an int or a float. [["<< e.what() << "]] Skipping this row, however processing continues." << std::endl;
            ++ptrbc->brc; //increment the bad row count.
        }
        ++ptrbc->trc; //Increment the total row count, includes valid and invalid rows.
        
    }  //End of for loop
    if (ptrbc->debugFlags.debug2) std::cout << "\nNumber of VALID Returned Rows: " << ptrbc->grc << ".\nNumber of INVALID Rows: " << ptrbc->brc << ".\nTotal Number of rows processed: " << ptrbc->grc + ptrbc->brc << ".\nNumber of unprocessed rows: " << numberOfReturnedRows-ptrbc->trc << "\n length of copyBuffer " << strlen(copyBuffer) << " bytes." << std::endl;
    
    if (ptrbc->debugFlags.debug2)  std::cout << "Successfully performed SELECT operation on all data.\nNow let's look at what we've put in the temperatureVector and the energyUsageVector." << std::endl;
    
    
    if (ptrbc->debugFlags.debug2 ) {
        for (int k=0; k<numberOfReturnedRows-brc; k++) {
            
            std::cout << k << ". " << awv->temperatureVector[k] << ", " << awv->energyUsageVector[k] << std::endl;
        }
    }
    ofstream myTempFile; //Create an object for writing to a file (an ofstream)
    myTempFile.open(MYTEMPFILENAME);
    myTempFile  << copyBuffer  << endl;
    myTempFile.close();  //Close the output file
    ptrbc->allocateArrays(ptrbc->grc);
    for(loopCounter = 0; loopCounter<ptrbc->grc; loopCounter++ )  {
        ptrbc->setArray(IV, loopCounter, awv->temperatureVector[ loopCounter] ); //Place the independent variable, Average Daily Temperature, into its designated array.
        ptrbc->setArray(ODVA, loopCounter, awv->energyUsageVector[ loopCounter] ); //Place the observed dependent variable (EnergyUsage as Read (Observed) from the electric meters into its array.

    }

    //WE will  now close the portal and end the transaction because I don't think we can do a create table on the ptrC connection with a transaction in progress.
    rc = closePortal(ptrC, ptrbc , "CLOSE myportal");  //Close myportal before ending the transaction. closePortal does a ¡PQclear! ;

    rc = endTransaction(ptrC,ptrbc, "END");  //End the transaction. endTransaction does a ¡PQclear!

    //Create a temporary table named tbl_temp_eu; it is here where we will mirror on a temporary table the data that went into the temperature vector and the energy usage vector, above.
    if (!ptrbc->debugFlags.tempTableCreated) {  //Have we created the temporary table?
        //We will do the DROP Table and Create Table with one SQL statement
        ptrbc->res = PQexec(ptrC, drop_tbl_temp_eu);
        if (PQresultStatus(ptrbc->res) != PGRES_COMMAND_OK ) {
            cerr << drop_tbl_temp_eu << " command failed " << endl;
            exit_nicely(ptrC, DROPTABLEFAILED, __LINE__);
        }
        PQclear(ptrbc->res);
        ptrbc->res = PQexec(ptrC, create_tbl_temp_eu);
        if (PQresultStatus(ptrbc->res) != PGRES_COMMAND_OK ) {
            cerr << create_tbl_temp_eu<< " command failed " << endl;
            exit_nicely(ptrC, CREATETABLEFAILED, __LINE__);
        }
        PQclear(ptrbc->res);
        ptrbc->debugFlags.tempTableCreated = true;  //Indicate table created.
    }
// BEGINNING OF COPY OPERATION
    const char *errmsg = NULL;
    ptrbc->res = PQexec(ptrC,  copy_STDIN_SQL); //Execute the SQL statement: COPY tbl_temp_eu (temp, eu) STDIN; 
    int resultsPQputCopyData = PQputCopyData(ptrC, copyBuffer, (int)strlen(copyBuffer));
    int resultsPQputCopyEnd = PQputCopyEnd(ptrC, errmsg);
//    int whichIsIt = PQresultStatus(ptrbc->res);
    if (ptrbc->debugFlags.debug2 )  cout << " resultsPQputCopyData: " << resultsPQputCopyData << "; resultsPQputCopyEnd: " << resultsPQputCopyEnd << endl;
    
    if (errmsg) {
        cerr << "COPY operation failed with error message of: " << errmsg << endl;
    } else {
        if (ptrbc->debugFlags.debug2 ) cout << "COPY operation seems to have worked!" << endl;
    ptrbc->res = PQexec(ptrC, "COMMIT;");  //This seems to be UNnecessary, although I don's see where we do a start transaction.
    PQclear(ptrbc->res);
    }
//END OF COPY OPERATION

    rc = beginTransaction(ptrC, ptrbc, begin_Transaction);  //Start up a new transaction where we will
    if (rc) exit_nicely(ptrC, rc, __LINE__);

    rc = executeSQL(ptrC, ptrbc, theMotherOfAllUnionSelectStatements);   //
    if (rc) exit_nicely (ptrC, rc, __LINE__);
    rc = fetchAllTransaction(ptrC, ptrbc, fetchALLTransaction);
    if (rc) { //Note that PGRES_TUPLES_OK happens when PQresultStatus returns a 2.
        cerr << "Failed when attempting to do a FETCH ALL execute this sql:\n" << theMotherOfAllUnionSelectStatements << "\nWe got a return status of: " << PQresultStatus(ptrbc->res) <<endl;
        exit_nicely(ptrC, rc, __LINE__);
    } else {
        numberOfReturnedRows = PQntuples(ptrbc->res);
        numberOfReturnedColumns = PQnfields(ptrbc->res);
        if (ptrbc->debugFlags.debug2 ) cout << "TheMotherOfAllUnionSelectStatements returned " << numberOfReturnedRows << " rows and " << numberOfReturnedColumns << " columns." << endl;
    }

    if (ptrbc->debugFlags.debug2 ) cout << "Now bucketize the data. Each bucket has a temperature range of " << BUCKETSIZE << "ºF " << " and we will get the average energy usage, in kwh, for that temperature range buffer along with the energy usage standard deviation. " << endl;
    char *sqlbuffer = new char[SIZEOFBUFFERCONTAININGMOTHEROFALLSELECTSTATEMNTS];   /* << =================================== Dynamically acquire sufficient storage for constructing, piece-by-piece, the Mother Of All Select Statements. */
    int minBucketTemp = MINTEMPERATURE;
    int buckettemp = minBucketTemp + BUCKETSIZE;
    int maxBucketTemp = MAXTEMPERATURE;
    int countOfGoodEntries = 0;
    
    const char *headerEU[2] = {"  avgtemp | avg eu |      stddev       |  min  |  max  | count\n", \
                                 "----------+--------+-------------------+-------+-------+-------\n"};
    if (ptrbc->debugFlags.debug2 ) cout << *(headerEU +0) << *(headerEU + 1) << std::endl; //Output the headers.//
    
    Align *aff = new Align(ASSUMEDBUFFERSIZE, headerEU[1]) ;

    
    loopCounter = 0; //Reset this.
    while ( buckettemp < maxBucketTemp && loopCounter < numberOfReturnedRows ) {  //Beginning of while statement.
        // See http://libpqxx.readthedocs.io/en/latest/a00004.html for explanation of how the next two lines work
        
        if ( ( PQgetisnull(ptrbc->res, loopCounter, _avgeu) || PQgetisnull(ptrbc->res, loopCounter, _stddeveu)  || PQgetisnull(ptrbc->res, loopCounter, _countu) ) ) { //Weed-out any bad records that contain poisonous fields.
            
            ++loopCounter;
            continue; //Skip this record if any critical values come back as NULL. Go back to top of while loop.
        }
        //            for (result::const_iterator column = r.rbegin(); column != r.rend(); ++column) { //Beginning of for loop
        
        try {
            if (ptrbc->debugFlags.debug2 ) {
                cout << aff->alignCenter(0,  PQgetvalue(ptrbc->res, loopCounter, _avgtemp) ) << "|";
                cout << aff->alignCenter(1,  PQgetvalue(ptrbc->res, loopCounter, _avgeu) ) << "|";
                cout << aff->alignCenter(2,  PQgetvalue(ptrbc->res, loopCounter, _stddeveu) ) << "|";
                cout << aff->alignCenter(3,  PQgetvalue(ptrbc->res, loopCounter, _mineu) ) << "|";
                cout << aff->alignCenter(4,  PQgetvalue(ptrbc->res, loopCounter, _maxeu) ) << "|";
                cout << aff->alignCenter(5,  PQgetvalue(ptrbc->res, loopCounter, _countu) ) << "|" << endl;
            }
            if( (thisCount =atoi( PQgetvalue(ptrbc->res, loopCounter, _countu) ) ) > 1 ) {
                values[_avgtemp] = atof(PQgetvalue(ptrbc->res, loopCounter, _avgtemp) );
                values[_avgeu]= atof(PQgetvalue(ptrbc->res, loopCounter, _avgeu));
                values[_stddeveu] =  atof( PQgetvalue(ptrbc->res, loopCounter, _stddeveu) );
                values[_mineu] = atof(PQgetvalue(ptrbc->res, loopCounter, _mineu));
                values[_maxeu] = atof(PQgetvalue(ptrbc->res, loopCounter, _maxeu) );
                thisCount = atoi(PQgetvalue(ptrbc->res, loopCounter, _countu));
                ++worthyOfPolynomialFitProcessing;
                ++ptrbc->debugFlags.countOfGoodEntries;
                //INSERT the values we extracted into table tbl_bucketized_by_tempf_ranges. NOW THAT WE HAVE COPY WORKIN, PERHAPS WE CAN USE COPY RATHER THAN INSERT INTO … BLAH, BLAH …
                rc = insertBucketizedEnergyValuesIntoTable( values[_avgtemp], values[_avgeu], values[_stddeveu],  values[_mineu], values[_maxeu], thisCount, ptrbc);
                if (rc) {
                    cerr << "Operation to INSERT INTO tbl_bucketized_by_tempf_ranges  F A I L E D!" << std::endl;
                    exit_nicely(ptrC, rc, __LINE__);
                }
            } else { //Come here if fewer than 2 counts
                ++loopCounter;
                continue; //skip this record because it's no good, not enough data for a standard deviation. Go on to do the next record.
            }
        } catch (const std::exception &e) {
            ++ptrbc->debugFlags.countOfBadEntries;
            std::cerr << __FILE__ "2. bad values try/catch detected errors for entry number " << ptrbc->debugFlags.countOfBadEntries + countOfGoodEntries << std::endl;
        }  //end of try/catch block
        
        buckettemp += BUCKETSIZE;
        ++loopCounter;
    }  //End of while loop
    rc = closePortal(ptrC, ptrbc , close_Transaction);  //Close myportal before ending the transaction
    rc = endTransaction(ptrC, ptrbc , end_Transaction);  //Close myportal before ending the transaction
// =================================================================
    
    if (worthyOfPolynomialFitProcessing > POLYNOMIALDEGREE) {


        try {  //Attempt to read back the data we just inserted into table tbl_bucketized_by_tempf_ranges
            rc = beginTransaction(ptrC, ptrbc, begin_Transaction);
            if (rc) exit_nicely(ptrC, rc, __LINE__);
            rc = executeSQL(ptrC, ptrbc, readBackTable);  //We might want to order by temp.
            if (rc) exit_nicely (ptrC, rc, __LINE__);
            rc = fetchAllTransaction(ptrC, ptrbc, fetchALLTransaction);

            if (rc) {
                if (ptrbc->debugFlags.debug2 ) cout << "FETCH ALL for SELECT statement\n" << readBackTable << "\nfailed with error: " << PQerrorMessage(ptrC) << endl;
            } else {
                numberOfReturnedRows = PQntuples(ptrbc->res);
                numberOfReturnedColumns = PQnfields(ptrbc->res);
            }

            if (numberOfReturnedRows) {
                if (numberOfReturnedRows != worthyOfPolynomialFitProcessing) {
                    std::cerr << "For some unknown reason, the numberOfReturnedRows: " << numberOfReturnedRows << " AND values worthyOfPolynomialFitProcessing: " << worthyOfPolynomialFitProcessing << " have different values. " <<std::endl;
                }
                SetupForMultiFit  *smf = new SetupForMultiFit(POLYNOMIALDEGREE, worthyOfPolynomialFitProcessing, ptrValues);   /* << =================================== */ //First, establish an instance of the SetupForMultiFit object.
                int rowBeingProcessed = 0;
                for  (loopCounter = 0; loopCounter < numberOfReturnedRows; ++loopCounter, ++rowBeingProcessed)
                { //Begin loading the gsl matrices and vectors with values.
                    values[_avgtemp] =atof(PQgetvalue(ptrbc->res, rowBeingProcessed, _avgtemp) ) ;   //Considered the independent Variable
                    values[_avgeu]= atof(PQgetvalue(ptrbc->res, rowBeingProcessed, _avgeu));        //Considered the dependent Variable
                    values[_stddeveu] = atof( PQgetvalue(ptrbc->res, rowBeingProcessed, _stddeveu) ); //Considered the weight.
                    values[_mineu] = atof(PQgetvalue(ptrbc->res, rowBeingProcessed, _mineu));
                    values[_maxeu] =atof(PQgetvalue(ptrbc->res, rowBeingProcessed, _maxeu) );
                    thisCount = atoi(PQgetvalue(ptrbc->res, rowBeingProcessed, _countu));
                    smf->captureIndependentVariableValues( rowBeingProcessed, values );    //Now, put into the object the data that will go into the independent variable matrix (_avgtemp), the dependent variable vector (_avgeu), and the weighting
                    smf->setIntoVector(smf->dependentVariable, rowBeingProcessed, *(values + _avgeu) ); //Put the depenedent variable into its vector.
                    smf->setIntoVector(smf->weights, rowBeingProcessed, 1/( *(values + _stddeveu) * *(values + _stddeveu)  ) ); //Put the weight (stddev) into it's vector. See top 0f page 451 of version 1.12 of GNU Scientific Library Reference Manual (3rd Edition) for the `1/( *(values + _stddeveu) * *(values + _stddeveu)  )` piece of the processing

// NO LONGER NEEDED                    ++rowBeingProcessed;
                } //End of for loop
                PQclear(ptrbc->res);  //Get this messag: "WARNING:  there is no transaction in progress" because we closed the myportal cursor and ended the transaction
                rc = closePortal(ptrC, ptrbc , end_Transaction);  //Close myportal
                rc = endTransaction(ptrC, ptrbc, end_Transaction);
                
                rc = smf->doMultiFit( ); //Actually go out and do the gsl_multifit
                rc = smf->outputPolynomial("EnergyUsage(T) = ");
                std::cout << smf->outputCovarianceMatrix("cov = [") << "☜ χ-squared" << std::endl;
                std::cout << smf->computeTrace(smf->covarienceMatrix->data, (int)smf->onePlusPolynomialDegree) << "☜ Trace of Covariance Matrix" << std::endl;
                std::cout << smf->computeCorrelationBetweenIndependentAndDependentVariables(ptrbc) << "☜ correlation coefficient" << std::endl;
                double squareRootOfSumOfTheSquaresOfTheVarience = smf->computeGoodnessOfResults(ptrbc);
                double averageVariencePerValidReading = squareRootOfSumOfTheSquaresOfTheVarience/ptrbc->grc;
                std::cout << squareRootOfSumOfTheSquaresOfTheVarience << "☜ Square Root Of Sum Of TheSquares Of The Varience; \n" << averageVariencePerValidReading << "☜ Average Varience Per Valid Reading\n" << averageVariencePerValidReading*100 << "\%☜ Average Varience Per Valid Reading\n"<< ptrbc->grc << "☜ Number of valid meter readings (observed dependent variable)" <<  std::endl;
            } else {
                std::cerr << "Although the SELECT seems to have worked, we got " << numberOfReturnedRows << " rows back from the table which we thought we just inserted data into!" << std::endl;
            }
        } catch (const std::exception &e) {
            std::cerr << "Some how we failed to get any data back from the table which we thought we just inserted data into!" << std::endl;
        }
 
        
        //        rc = polynomialFit(POLYNOMIALDEGREE, grc, awv->temperatureVector, awv->energyUsageVector,  NULL, NULL,  &TSS, &goodnessOfFit);
    } else {
        std::cerr << "Insufficient number of VALID datapoints to do MultiFit processing for desired polynomial of degree " << POLYNOMIALDEGREE << std::endl;
    }
    delete [] copyBuffer;
    if (subCharBuffer != nullptr) delete[](subCharBuffer);   //c++'s Equivalent to C's free when we allocate storage with a calloc/malloc. Note the use of [] prior to the (subCharacterBuffer);
    if (sqlbuffer != nullptr) delete[](sqlbuffer);       //Delete another dynamically acquired character buffer.
    if (aff != nullptr)  {
        aff->~Align();
        delete aff;
    }
    if (af != nullptr) {
        af->~Align();
        delete af;
    }
//    UPDATE tbl_energy_usage set avtempf = (SELECT avg(temperature) FROM tbl_l WHERE siteid=5 AND lt BETWEEN '2018-05-06 08:17:00' AND '2018-05-07 07:00:00' ), avtempk=SELECT (5*(avg(temperature)-32)/9+273.15 ) FROM tbl_l WHERE  siteid=5 AND lt BETWEEN '2018-05-06 08:17:00' AND '2018-05-07 07:00:00' ), siteid=5 WHERE id=2877;
    return 0;
}
