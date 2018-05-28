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
extern const char *insertIntoTable;
int insertBucketizedEnergyValuesIntoTable(double, double, double, double, double, int, BaseClass *);
int insertBucketizedEnergyValuesIntoTable( double avgtemp, \
                                          double avgeu, \
                                          double stddeveu, \
                                          double mineu, \
                                          double maxeu, \
                                          int counteu, \
                                          BaseClass *ptrbc) {
    int rc=WONDERFUL;
    const char *sql; //Need to do const char *sql rather than just char *sql to make the C++11 compiler happy.
    char sqlbuffer[400];
    sql = sqlbuffer;
    ptrbc->BaseClass::indVarArray [ ptrbc->debugFlags.countOfGoodEntries - 1] = avgtemp;
    sprintf (sqlbuffer, insertIntoTable, avgtemp, avgeu, stddeveu, mineu, maxeu, counteu); //CREATE the insert into SQL statement
    if ( ptrbc->debugFlags.debug3  ) std::cout << "Insert looks like: " << sqlbuffer << std::endl;
    int numberOfReturnedRows;
    int numberOfReturnedColumns;
    PGconn  *conn;
    conn = PQconnectdb(ptrbc->lookAtMyConnectionString);  //Try to connect to database for the purpose of doing inserts.
    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "Connection to database, for the purpose of inserting data, failed for this reason:\n" << PQstatus(conn) << endl;
        rc = CONNECTTODATABASEFAILED;
    } else {
    PGresult *res;
    res = PQexec(conn, sql);
    
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "SQL statement\n" << sqlbuffer << "\nfailed with error: " << PQerrorMessage(conn) << endl;
        rc = INSERTINTOFAILED;
    } else {
        numberOfReturnedRows = PQntuples(res);
        numberOfReturnedColumns = PQnfields(res);
        if (ptrbc->debugFlags.debug3)  std::cout << "Record inserted successfully with return status of: " << PQstatus(conn) << ". Number of returned rows: " << numberOfReturnedRows << "numberOfReturnedColumns: " << numberOfReturnedColumns << endl;
    }

    PQclear(res);
    }
    PQfinish(conn);
    return rc;
}





