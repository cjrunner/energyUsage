//
//  fetchAll.cpp
//  energyUsageWithoutThe_PQXX_Bullshit
//
//  Created by Clifford Campo on 5/15/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//
#include "/usr/local/pgsql/pgsql101/include/libpq-fe.h"
#include <iostream>
#include "baseClass.hpp"
#define FETCHALLFAILED 2
int fetchAllTransaction(PGconn *, BaseClass *, const char *);
int fetchAllTransaction(PGconn *conn, BaseClass *ptrbc, const char *sql, bool doPQclear)  {
    int rc=WONDERFUL;
    ptrbc->res = PQexec(conn,  sql);
    if (PQresultStatus(ptrbc->res) != PGRES_TUPLES_OK) {
        std::cout << "Failed to execute the SQL statement: " << sql << ". Error cause:\n" <<
        PQerrorMessage(conn) << std::endl;
        rc = FETCHALLFAILED;
    }
    if (doPQclear) PQclear(ptrbc->res); /* Per prototype definition of function fetchALLTransaction, doPQclear defaults to false.
                                   Page 74 of The PostgreDQL 9.0 Reference manual indicates no PQclear after executing the FETCH ALL … */
    return rc;
}
