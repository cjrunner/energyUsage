//
//  baseClass.hpp
//  
//
//  Created by Clifford Campo on 5/2/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//
#ifndef __BASECLASS__
#define __BASECLASS__

#include <typeinfo>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

#include <gsl/gsl_vector.h> 
#include "currentDateTime.hpp"
#include "cmdLineArgs.hpp"
#include "myPrototypes.hpp"
#include "alignField.hpp"
//#include "/Users/cjc/c++/energyUsage/energyUsage/BitFlags.hpp"
#include "BitFlags.hpp"

#include "/usr/local/pgsql/pgsql101/include/libpq-fe.h"
#define WONDERFUL 0
#define MAXTEMPERATURE 100
#define MINTEMPERATURE 0
#define MAXNUMBEROFAVGTEMPS 100
#define IV 0
#define EMO IV+1
#define EDVA EMO + 1
#define ODVA EDVA + 1
#define BADWHICH 1
#define MAXLENGTHPASSWORD 32
#define SELECTFROMTABLE 0
#define CLOSEPORTAL 1 + SELECTFROMTABLE
#define CREATETHETABLE 1 + CLOSEPORTAL
#define DROPTHETABLE 1 + CREATETHETABLE
#define ENDTRANSACTION 1 + DROPTHETABLE
#define EXECUTESQL 1 + ENDTRANSACTION
#define FETCHALLTRANSACTION 1 + EXECUTESQL
#define INSERTBUCKETIZEDVALUESINTOTABLE 1 + FETCHALLTRANSACTION
#define BEGINTRANSACTION 1 + INSERTBUCKETIZEDVALUESINTOTABLE
#define NUMBEROFVALUES 9
#define NUMBEROFPGRESULTOBJCECTS 9
#define WORKBUFSIZE 80

class BaseClass    {
    friend CmdLineArgs;
private: //These values get set during execution time, not during compile time.

    const size_t szBaseClass = sizeof(BaseClass);

    const size_t maxNumberOfAvgTemps = MAXNUMBEROFAVGTEMPS;   //number of elements to be initially allocated to the gsl_vector type named independentVariableAvgTemp 100; //number of elements to be initially allocated to the gsl_vector type named independentVariableAvgTemp
    const char *const dbnameEqual   = "dbname=";         // dbname= will always be the first parameter in the connection string, thus the absense of the leading blank.
    const char *const useridEqual   = " user=";          //
    const char *const passwordEqual = " password=";   //
    const char *const hostidEqual   = " hostaddr=";     //
    const char *const portidEqual   = " port=";          //
    const char *const basicSELECT   = "SELECT avtempf, avtempk, m1kwh, m2kwh, m1m2kwh FROM tbl_energy_usage WHERE date_part('J', date) BETWEEN ";
    size_t lengthOfStaticPartsOfConnectionString = strlen(dbnameEqual) + strlen(useridEqual) + strlen(passwordEqual) + strlen(hostidEqual) + strlen(portidEqual);
    size_t lengthOfDynamicPartsOfConnectionString;
    CmdLineArgs *CLA;   //Points to the class named CmdLineArgs. This class is a friend of class CmdLineArgs
    const size_t szPrivateBaseClass = sizeof(szBaseClass) + sizeof(szPrivateBaseClass) + sizeof(maxNumberOfAvgTemps) + sizeof(dbnameEqual) + sizeof(useridEqual) + sizeof(passwordEqual) + sizeof(hostidEqual) + sizeof(portidEqual) + sizeof(basicSELECT) + sizeof(lengthOfStaticPartsOfConnectionString) + sizeof(lengthOfDynamicPartsOfConnectionString) +sizeof(*CLA) + sizeof(szPublicPartsOfBaseClass);
    const size_t szPublicPartsOfBaseClass = szBaseClass - szPrivateBaseClass;
public:

    const size_t *ptrMaxNumberOfAvgTemps;  //number of elements to be initially allocated to the gsl_vector type named independentVariableAvgTemp 100; //number of elements to be initially allocated to the gsl_vector type named independentVariableAvgTemp
    PGconn *conn; //PostgreSQL's Connection Object
    PGresult *res;
    BitFlags debugFlags;
    
    double *ptrExpMinusObsDepVarArray;
    double *ptrObsDepVarArray;
    double *ptrExpDepVarArray;
    double *ptrIndVariableArray;
    
    double depVarArray[MAXNUMBEROFAVGTEMPS];
    double indVarArray[MAXNUMBEROFAVGTEMPS];
    const char *errmsg;
    const char *BFS; //New to multiThread version Points to motherOfAllSelectStatements specific to this thread 
    const char *lookAtMyConnectionString;
    const char *clUserID;
    const char *clDatabaseName;
    const char *clPortID;
    const char *clHostID;
    size_t sizeOfEachArray;
    char pwBuffer[MAXLENGTHPASSWORD];
    const char *results_File;  //Name of the results file
    const char *clPW;
    char *MCS;
    const char *clSQL; //Points to the sql statement provided in the command line switch, if Any.
    //Variables specifict to selectFromTable
    char *start;
    char *end;
    std::ostringstream *ptros;
    std::ofstream myTempFile; //Create an object for writing to a file (an ofstream)
    std::ostringstream *outstring; //Create a pointer to object that replaces std::cout or std::cerr with a character string.
    size_t len;
    //    double julianDate = 2458243.209030; unused
    char s;
    size_t bufferOffset;
    int minBucketTemp;
    int buckettemp;
    int maxBucketTemp;
    int countOfGoodEntries;
    int loopCounter;
    int rowBeingProcessed;
    int numberOfReturnedRows;
    int numberOfReturnedColumns;
    int rc[8];
    int grc;
    int brc;
    int k;
    int resultsPQputCopyEnd;
    int resultsPQputCopyData;
    //    int trc=INITIALVALUE;
    size_t grc_t; //Good Row Count
    size_t brc_t; //Bad Row Count
    size_t trc_t; //Total Row Count = grc_t + brc_t
    int thisCount;
    int selectFromTable_RC; //Return code from selectFromTable
    int worthyOfPolynomialFitProcessing;
    char *temp;
    char *eu;
    char *copyBuffer;
    char *sqlbuffer;
    char *subCharBuffer;
    char *sql;
    double averageVariencePerValidReading;
    double squareRootOfSumOfTheSquaresOfTheVarience ;
    double values[NUMBEROFVALUES];
    double *ptrValues;
    char  workbuf[WORKBUFSIZE];
    char connectionString[WORKBUFSIZE];   //Will hold the actual connection string.
    //Variables specific to dropTheTable
    PGresult *resD;
    
    //Variables specific to effectConnection
    
    //Variables specific to insertBucketizedEnergyValuesIntoTable.cpp
    
    //Variables specific to currentDateTime.cpp
    time_t     now;
    struct tm  tstruct;
    char buf[WORKBUFSIZE];
    //
    PGconn  *connI;
    PGresult *resI;
    int numberOfReturnedRowsI;
    int numberOfReturnedColumnsI;
    char e;
    void allocateArrays(size_t);
    char *sqlI; //Need to do const char *sql rather than just char *sql to make the C++11 compiler happy.
    //Prototypes
    BaseClass(int);
    BaseClass(CmdLineArgs *,  const char = '0', const char * = "cjc", const char * = "LocalWeather", const char * = "5436", const char * = "127.0.0.1", const size_t = MAXNUMBEROFAVGTEMPS );
    const char *getPrivate(CmdLineArgs &,char);
    ~BaseClass();

    void setConString(const char *cs);
    int setArray(int, int, double);

}; //EndOfClass BaseClass
#endif


