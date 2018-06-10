//
//  fetchAll.cpp
//  energyUsageWithoutThe_PQXX_Bullshit
//
//  Created by Clifford Campo on 5/15/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//

#include <iostream>
#include "/usr/local/pgsql/pgsql101/include/libpq-fe.h"
#include "baseClass.hpp"
#define FETCHALLFAILED 2
#include "baseClass.hpp"
int fetchAllTransaction(BaseClass *, const char *);
int fetchAllTransaction(BaseClass *ptrbc, const char *sql, bool doPQclear)  {
    ptrbc->rc[FETCHALLTRANSACTION] = WONDERFUL;
    ptrbc->res = PQexec(ptrbc->conn,  sql);
    if (PQresultStatus(ptrbc->res) != PGRES_TUPLES_OK) {
        std::cout << "Failed to execute the SQL statement: " << sql << ". Error cause:\n" <<
        PQerrorMessage(ptrbc->conn) << std::endl;
        ptrbc->rc[FETCHALLTRANSACTION] = FETCHALLFAILED;
    }
    if (doPQclear) PQclear(ptrbc->res); /* Per prototype definition of function fetchALLTransaction, doPQclear defaults to false.
                                   Page 74 of The PostgreDQL 9.0 Reference manual indicates no PQclear after executing the FETCH ALL … */
    return ptrbc->rc[FETCHALLTRANSACTION];
}
