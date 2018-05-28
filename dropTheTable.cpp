//
//  dropTable.cpp
//  energyUsage
//
//  Created by Clifford Campo on 5/8/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//
#include <iostream>
#include <cstring>
#include "myPrototypes.hpp"
#include "/usr/local/pgsql/pgsql101/include/libpq-fe.h"
extern const char *dropTable;
int dropTheTable(const char *sqlDropTable, PGconn *conn) {
    PGresult *res;
    int rc=WONDERFUL;
    try {
        res = PQexec(conn, sqlDropTable);
        PQclear(res);
    } catch (const std::exception &e) {
        rc = DROPTABLEFAILED;
    }
    return rc;
} //Used to drop a table if it exists
