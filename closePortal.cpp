//
//  closePortal.cpp
//  energyUsageWithoutThe_PQXX_Bullshit
//
//  Created by Clifford Campo on 5/16/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//

#include <stdio.h>
#include "/usr/local/pgsql/pgsql101/include/libpq-fe.h"
#include "myPrototypes.hpp"
#include <iostream>
#define CLOSEPORTALTRANSACTIONFAILED 5
int closePortal(PGconn *, BaseClass *, const char *);
int closePortal(PGconn *conn, BaseClass *ptrbc, const char *sql, bool doPQclear)  {
    int rc=WONDERFUL;
    ptrbc->res = PQexec(conn,  sql);
    if (PQresultStatus(ptrbc->res) != PGRES_COMMAND_OK) {
        std::cout << __FILE__ << "Line " << __LINE__ << "\tFailed to execute the SQL statement: " << sql << ". Error cause:\n" <<
        PQerrorMessage(conn) << std::endl;
        rc = CLOSEPORTALTRANSACTIONFAILED;
    }
    if (doPQclear) PQclear(ptrbc->res); //Per prototype definition of function closePortal, doPQclear defaults to true.
    return rc;
}
