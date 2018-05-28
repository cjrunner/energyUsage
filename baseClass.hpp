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
#include <cstring>
#include <gsl/gsl_vector.h> 
#include "myPrototypes.hpp"
//#include "/Users/cjc/c++/energyUsage/energyUsage/BitFlags.hpp"
#include "BitFlags.hpp"
#include "/usr/local/pgsql/pgsql101/include/libpq-fe.h"
class BaseClass {
#define IV 0
#define EMO IV+1
#define EDVA EMO + 1
#define ODVA EDVA + 1
#define BADWHICH 1
#define MAXLENGTHPASSWORD 32
private:
    const size_t szBaseClass = sizeof(BaseClass);
    const size_t maxNumberOfAvgTemps = 100;  //number of elements to be initially allocated to the gsl_vector type named independentVariableAvgTemp 100; //number of elements to be initially allocated to the gsl_vector type named independentVariableAvgTemp
    const char *const dbnameEqual = "dbname=";         // dbname= will always be the first parameter in the connection string, thus the absense of the leading blank.
    const char *const useridEqual = " user=";          //
    const char *const passwordEqual = " password=";   //
    const char *const hostidEqual = " hostaddr=";     //
    const char *const portidEqual = " port=";          //
    const char *const basicSELECT = "SELECT avtempf, avtempk, m1kwh, m2kwh, m1m2kwh FROM tbl_energy_usage WHERE date_part('J', date) BETWEEN ";
    size_t lengthOfStaticPartsOfConnectionString = strlen(dbnameEqual) + strlen(useridEqual) + strlen(passwordEqual) + strlen(hostidEqual) + strlen(portidEqual);
    size_t lengthOfDynamicPartsOfConnectionString;

public:


    enum bucketColNames  {_avgtemp, _avgeu, _stddeveu, _mineu, _maxeu, _countu};
    const size_t *ptrMaxNumberOfAvgTemps;  //number of elements to be initially allocated to the gsl_vector type named independentVariableAvgTemp 100; //number of elements to be initially allocated to the gsl_vector type named independentVariableAvgTemp
    PGresult *res;
    BitFlags debugFlags;

    double *ptrExpMinusObsDepVarArray;
    double *ptrObsDepVarArray;
    double *ptrExpDepVarArray;
    double *ptrIndVariableArray;
    
    double depVarArray[MAXNUMBEROFAVGTEMPS];
    double indVarArray[MAXNUMBEROFAVGTEMPS];
    size_t grc; //Good Row Count
    size_t brc; //Bad Row Count
    size_t trc; //Total Row Count
    const char *lookAtMyConnectionString;
    const char *clUserID;
    const char *clDatabaseName;
    const char *clPortID;
    const char *clHostID;
    size_t sizeOfEachArray;
    char pwBuffer[MAXLENGTHPASSWORD] = "";
    char *results_File;
    char *clPW;
    char *MCS;
    char *clSQL; //Points to the sql statement provided in the command line switch, if Any.
    void allocateArrays(size_t);
    BaseClass(const char *clUserid="cjc", const char *clDatabaseName="LocalWeather", const char *clPortID="5436", const char *clHostID="127.0.0.1", const size_t=MAXNUMBEROFAVGTEMPS ); 
    ~BaseClass();
    void setConString(const char *cs);
    int setArray(int, int, double);
}; //EndOfClass BaseClass
#endif


