//
//  insertIBucketizeEnergyValuesIntoTable.cpp
//  energyUsage
//
//  Created by Clifford Campo on 5/5/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//


//
//  insertIntoTable.cpp
//  c++Postgres
//
//  Created by Clifford Campo on 5/1/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//
#include <iostream>
#include "/usr/local/pgsql/pgsql101/include/libpq-fe.h"
#include "myPrototypes.hpp"
#include "baseClass.hpp"
#include "setupForMultiFit.hpp"


using namespace std;
extern const char *insertIntoTable[];
int insertBucketizedEnergyValuesIntoTable(double, double, double, double, double, int, BaseClass *);
int insertBucketizedEnergyValuesIntoTable( double avgtemp, \
                                          double avgeu, \
                                          double stddeveu, \
                                          double mineu, \
                                          double maxeu, \
                                          int counteu, \
                                          BaseClass *ptrbc) {
    ptrbc->rc[INSERTBUCKETIZEDVALUESINTOTABLE] = WONDERFUL; //Preset the return code anticipating success
    
    ptrbc->sqlI = new char [400];
    
    ptrbc->BaseClass::indVarArray [ ptrbc->debugFlags.countOfGoodEntries - 1] = avgtemp;
    sprintf (ptrbc->sqlI, insertIntoTable[ptrbc->debugFlags.intMyCase], avgtemp, avgeu, stddeveu, mineu, maxeu, counteu);  //CREATE the insert into SQL statement
    if ( ptrbc->debugFlags.debug3  ) std::cout << "Insert looks like: " << ptrbc->sqlI << std::endl;


    ptrbc->connI = PQconnectdb(ptrbc->lookAtMyConnectionString);  //Try to connect to database for the purpose of doing inserts.
    if (PQstatus(ptrbc->connI) != CONNECTION_OK) {
        cerr << "Connection to database, for the purpose of inserting data, failed for this reason:\n" << PQstatus(ptrbc->connI) << endl;
        ptrbc->rc[INSERTBUCKETIZEDVALUESINTOTABLE] = CONNECTTODATABASEFAILED;
    } else {

    ptrbc->resI = PQexec(ptrbc->connI, ptrbc->sqlI);
    
    if (PQresultStatus(ptrbc->resI) != PGRES_COMMAND_OK) {
        cerr << "Line " << __LINE__ << " of file " << __FILE__ << ": SQL statement\n" << ptrbc->sqlI << "\nfailed with error: " << PQerrorMessage(ptrbc->connI) << endl;
        ptrbc->rc[INSERTBUCKETIZEDVALUESINTOTABLE] = INSERTINTOFAILED;
    } else {
        ptrbc->numberOfReturnedRowsI = PQntuples(ptrbc->resI);
        ptrbc->numberOfReturnedColumnsI = PQnfields(ptrbc->resI);
        if (ptrbc->debugFlags.debug3)  std::cout << "Record inserted successfully with return status of: " << PQstatus(ptrbc->connI) << ". Number of returned rows: " << ptrbc->numberOfReturnedRowsI << ", and the numberOfReturnedColumns: " << ptrbc->numberOfReturnedColumnsI << endl;
    }

    PQclear(ptrbc->resI);
    }
    PQfinish(ptrbc->connI);
    delete [] ptrbc->sqlI;
    return ptrbc->rc[INSERTBUCKETIZEDVALUESINTOTABLE];
}





