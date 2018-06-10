//
//  beginTransaction.cpp
//  energyUsageWithoutThe_PQXX_Bullshit
//
//  Created by Clifford Campo on 5/15/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//
#include "/usr/local/pgsql/pgsql101/include/libpq-fe.h"
#include <iostream>
#include "myPrototypes.hpp"
#include "baseClass.hpp"
int beginTransaction(BaseClass *, const char *);
int beginTransaction(BaseClass *ptrbc, const char *sql, bool doPQclear)  {
    int rc=WONDERFUL;
    ptrbc->res= PQexec(ptrbc->conn,  sql);
    if (PQresultStatus(ptrbc->res) != PGRES_COMMAND_OK) {
        std::cout << "Failed to execute the SQL statement: " << sql << ". Error cause:\n" <<
        PQerrorMessage(ptrbc->conn) << std::endl;
        rc = BEGINTRANSACTIONFAILED;
    }
    if (doPQclear) PQclear(ptrbc->res); //Per prototype definition of function beginTransaction, doPQclear defaults to true.
    return rc;
}
