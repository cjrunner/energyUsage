//
//  createTheTable.cpp
//  energyUsage
//
//  Created by Clifford Campo on 5/8/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//


#include <iostream>
#include <cstring>
#include "/usr/local/pgsql/pgsql101/include/libpq-fe.h"
#include "myPrototypes.hpp"
extern const char *createTable;
extern const char *createTempTable;
int createTheTable(const char *sqlCreateTable, PGconn *conn) {
    int rc=WONDERFUL;
    PGresult *res;
    try {
        res = PQexec(conn, sqlCreateTable);
        PQclear(res);
    } catch (const std::exception &e) {
        rc = CREATETABLEFAILED;
    }
    return rc;
} //Used to drop a table if it exists with the second parameter pointing to the SQL
