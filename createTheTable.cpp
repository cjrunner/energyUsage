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
#include "baseClass.hpp"
extern const char *createTable;

int createTheTable( BaseClass *, const char *, bool=true) ;
int createTheTable( BaseClass *ptrbc, const char *sqlCreateTable, bool doPQclear) {
    ptrbc->rc[CREATETHETABLE]=WONDERFUL;
    try {
        ptrbc->res = PQexec(ptrbc->conn, sqlCreateTable);
        if(doPQclear) PQclear(ptrbc->res);
    } catch (const std::exception &e) {
        ptrbc->rc[CREATETHETABLE] = CREATETABLEFAILED;
    }
    return ptrbc->rc[CREATETHETABLE];
} //Used to drop a table if it exists with the second parameter pointing to the SQL
