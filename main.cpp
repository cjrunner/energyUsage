//
//  main.cpp
//  energyUsage (based on c++Postgres)
//
//  Created by Clifford Campo on 4/24/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//

#include <AvailabilityMacros.h>
#include <iostream>
#include "/usr/local/pgsql/pgsql101/include/libpq-fe.h"
#include <cstring>
#include <getopt.h>
#include "makeHelpMessage.hpp"
#include "baseClass.hpp" 
#include "effectConnection.hpp"
#include "myPrototypes.hpp"
//#include "getMCS.hpp"
using namespace std;
extern const short numberOfRowsInMotherOFAllSelectStatements;
extern const short numberOfColumnsInMotherOFAllSelectStatements;
extern char *defalutResultsFileName;
extern char *defaultDependentVariableColumn;
extern const char *const message;
extern const char *motherOfAllSelectStatements[];
extern const char *help_Message_Array[];
extern const int numberOfEntries;
int selectFromTable(PGconn *,  BaseClass *, const char *);
int main(int argc, char* argv[])  {
    char connectionString[MAXBUFSIZE];
    char *ptrConnectionStringBuffer = connectionString;
    cout << __TIMESTAMP__ << "\tLine " <<  __LINE__ <<"\tFile " << __FILE__ << "\nBeginning execution of program:\n" << *(argv+0) << std::endl;
    const char *BFS;
    const struct option longopts[] =
    {
        {"dbname",    required_argument, 0, 'D'},
        {"help",       no_argument,        0, 'h'},
        {"userid",    required_argument, 0, 'U'},
        {"hostid",    required_argument, 0, 'H'},
        {"port",       required_argument, 0, 'P'},
        {"sql",         required_argument, 0, 'S'},
        {"col",         required_argument, 0, 'c'},
        {"kelvin",      no_argument,        0, 'k'},
        {"resultsFile", required_argument, 0, 'f'},
        {"debug1",     no_argument,         0, '1'},
        {"debug2",     no_argument,         0, '2'},
        {"debug3",     no_argument,         0, '3'},
        {0,0,0,0}, //End of array
    };
    const char *const commandLineSwitches = "123hkD:U:P:H:S:c:f:";
    int index;

    
    int numberOfMessageSegments = 0;
    while ( *(help_Message_Array + numberOfMessageSegments)  )  {numberOfMessageSegments++;} //Loop until we hit the pointer that points to location 0x'00 00 00 00 00 00 00 00', marking the end of the array.
    numberOfMessageSegments--; //Back-off 1 becaue the very last entry in the array, that gets counted, is an all zero terminator, but we want our count to indicate the number of valid addresses that point to strings.
    
    char  iarg= NULL; //Jumping though hoops just to make iarg to appear as a character in the debugger.
    BaseClass *bc = new BaseClass( );  /* << =============== bc ================= */
    bc->results_File = defalutResultsFileName;

    //turn off getopt error message
    opterr=1;
    MakeHelpMessage *MSG = new MakeHelpMessage( (*(argv +0) ),  &help_Message_Array[0], numberOfMessageSegments  );     /* << =============== MSG ================ */
    while( (iarg = (char)getopt_long(argc, argv, commandLineSwitches, longopts, &index)) != -1) {  //Jumping though hoops just to make iarg to appear as a character in the debugger.
        if (iarg < ZERO) break;
        switch (iarg)
        {
            case 'D': //Database Name
                bc->clDatabaseName = optarg;
                if (bc->debugFlags.debug1  )  std::cout << "Database Name: " << bc->clDatabaseName << std::endl;
                break;
            case 'f': //resultsFile
                if (bc->debugFlags.debug1  ) std::cout << "resultsFile: " << optarg << std::endl;
                bc->results_File = optarg;
                break;
            case 'k': //Use kelvin temperatures as independent variable
                bc->debugFlags.useKelvin = true;
                break;
            case 'U': //Userid
                bc->clUserID = optarg;
                if (bc->debugFlags.debug1  ) std::cout << "User ID: " << bc->clUserID << std::endl;
                break;
            case 'c': //case of which meter reading to use
                if (strcmp(optarg, "m1m2kwh") == 0) {
                    BFS = *(motherOfAllSelectStatements + M1M2KWHCASE );  //m1m2kwh is the default value
                    bc->debugFlags.mycase = M1M2KWH;
                    bc->debugFlags.dependentVariableColumn = optarg;
                } else if (strcmp(optarg, "m1kwh") == 0 ) {
                    bc->debugFlags.mycase = M1KWH;
                    BFS = *(motherOfAllSelectStatements + M1KWHCASE );  
                    bc->debugFlags.dependentVariableColumn = optarg;
                    bc->debugFlags.includeM2InAverages = false;
                } else if (strcmp(optarg, "m2kwh") == 0) {
                    bc->debugFlags.mycase = M2KWH;
                    BFS = *(motherOfAllSelectStatements + M2KWHCASE);
                    bc->debugFlags.dependentVariableColumn = optarg;
                    bc->debugFlags.includeM1InAverages = false;
                } else {
                    std::cerr << "The only valid values for the -c command line switch are: m1m2kwh, m1kwh, or m2kwh. The value entered \"" << optarg << "\" is invalid! We're exiting because of this faux pas. Try again with a valid value of the -c command line switch: m1m2kwh, m1kwh, or m2kwh" << std::endl;
                    exit (2);
                }

                if (bc->debugFlags.debug1  ) std::cout << "Using column: " << bc->debugFlags.dependentVariableColumn<< " for the dependent variable" << std::endl;
                break;
            case 'p': //Password
                bc->clPW = optarg;
                std::cout << "Enter the password for the database (32 character, max): " << endl;
                std::cout << "Password: " << bc->clPW << std::endl;

                break;
                
            case 'H': //Hostid
                bc->clHostID = optarg;
                if (bc->debugFlags.debug1  ) std::cout << "HostID: " << bc->clHostID << std::endl;

                break;
                
            case 'P':   //Port
                bc->clPortID = optarg;
                if (bc->debugFlags.debug1  ) std::cout << "PortID: " << bc->clPortID << std::endl;
                break;
            case 'S':  //SQL
                bc->clSQL = optarg;
                if (bc->debugFlags.debug1  ) std::cout << "SELECT SQL STATEMENT looks like: " << bc->clSQL << std::endl;
                break;
            case '1':  //Debug stage 1
                bc->debugFlags.debug1=true;
                break;
            case '2': //Debug stage 2
                bc->debugFlags.debug2=true;
                break;
            case '3': //Debug stage 3
                bc->debugFlags.debug3=true;
                break;
            case 'h': //Help
                
                std::cout << MSG->createMessage(*(argv +0) )  << std::endl;
                MSG->~MakeHelpMessage(); //Call destructor to delete dynamically acquired storage.
                exit (WONDERFUL);

            default:
                std::cerr << "Unknown command line parameter. Skipping." << std::endl;
                break;
        } //End of switch
    }  //End of While

//EffectConnection(char *usersConnectStringBuffer, const char *host="127.0.0.1", const char *user="cjc", const char *database="LocalWeather", const char *port="5436", const char *file="/Users/cjc/.pgpass");
    EffectConnection *ecs = new EffectConnection(ptrConnectionStringBuffer, bc->clHostID, bc->clUserID, bc->clDatabaseName, bc->clPortID  );

    if (bc->debugFlags.debug1) std::cout << "Connection String Looks Like: " << connectionString << std::endl;
    int rc=WONDERFUL;
    bc->setConString(ptrConnectionStringBuffer);
    PGconn *conn = ecs->connectToDataBase(ptrConnectionStringBuffer);  /* << =================================== */
    
    rc = selectFromTable(conn,  bc, BFS);
    PQfinish(conn);  //Since this main routine did the connection, this main routine must do the PQfinish to disconnec from the database.
    return WONDERFUL;

}

