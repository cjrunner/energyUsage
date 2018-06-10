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
#include "baseClass.hpp"
extern const char *dropTable;
int dropTheTable(BaseClass *ptrbc, const char *sqlDropTable) {

    ptrbc->rc[DROPTHETABLE]=WONDERFUL;
    try {
        ptrbc->res = PQexec(ptrbc->conn, sqlDropTable);
        PQclear(ptrbc->res);
    } catch (const std::exception &e) {
        ptrbc->rc[DROPTHETABLE] = DROPTABLEFAILED;
    }
    return ptrbc->rc[DROPTHETABLE];
} //Used to drop a table if it exists
