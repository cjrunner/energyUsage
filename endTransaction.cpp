//
//  endTransaction.cpp
//  energyUsageWithoutThe_PQXX_Bullshit
//
//  Created by Clifford Campo on 5/15/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//

#include "/usr/local/pgsql/pgsql101/include/libpq-fe.h"
#include "myPrototypes.hpp"
#include <iostream>

int endTransaction(PGconn *, BaseClass *, const char *, bool doPQclear=true);
int endTransaction(PGconn *conn, BaseClass *ptrbc, const char *sql, bool doPQclear)  {
    int rc=WONDERFUL;
    ptrbc->res = PQexec(conn,  sql);
    if (PQresultStatus(ptrbc->res) != PGRES_COMMAND_OK) {
        std::cout << __FILE__ << "Line " << __LINE__ << "\tFailed to execute the SQL statement: " << sql << ". Error cause:\n" <<
        PQerrorMessage(conn) << std::endl;
        rc = BEGINTRANSACTIONFAILED;
    }
    if (doPQclear) PQclear(ptrbc->res);
    return rc;
}
