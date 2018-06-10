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
#include "baseClass.hpp"
#define EXECUTESQLFAILED 1
int executeSQL(BaseClass *, const char *, bool = true);
int executeSQL( BaseClass *ptrbc, const char *sql, bool doPQclear)  {
    ptrbc->rc[EXECUTESQL]=WONDERFUL;
    ptrbc->res = PQexec(ptrbc->conn,  sql);
    if (PQresultStatus(ptrbc->res) != PGRES_COMMAND_OK) {
        std::cout << "Failed to execute the SQL statement: " << sql << ". Error cause:\n" <<
        PQerrorMessage(ptrbc->conn) << std::endl;
        ptrbc->rc[EXECUTESQL] = EXECUTESQLFAILED;
    }
    if (doPQclear) PQclear(ptrbc->res); //Per prototype definition of function executeSQL, doPQclear defaults to true.
    return ptrbc->rc[EXECUTESQL];
}
