//
//  executeSQL.cpp
//  energyUsageWithoutThe_PQXX_Bullshit
//
//  Created by Clifford Campo on 5/15/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//
#include "baseClass.hpp"
#include "myPrototypes.hpp"
#include "/usr/local/pgsql/pgsql101/include/libpq-fe.h"
#include <iostream>
#define EXECUTESQLFAILED 1
int executeSQL(PGconn *, BaseClass *, const char *);
int executeSQL(PGconn *conn, BaseClass *ptrbc, const char *sql, bool doPQclear)  {
    int rc=WONDERFUL;
    ptrbc->res = PQexec(conn,  sql);
    if (PQresultStatus(ptrbc->res) != PGRES_COMMAND_OK) {
        std::cout << "Failed to execute the SQL statement: " << sql << ". Error cause:\n" <<
        PQerrorMessage(conn) << std::endl;
        rc = EXECUTESQLFAILED;
    }
    if (doPQclear) PQclear(ptrbc->res); //Per prototype definition of function executeSQL, doPQclear defaults to true.
    return rc;
}
