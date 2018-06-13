//
//  main.cpp
//  energyUsage (based on c++Postgres)
//
//  Created by Clifford Campo on 4/24/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//

#include <AvailabilityMacros.h>
#include <iostream>
#include <thread>
#include "/usr/local/pgsql/pgsql101/include/libpq-fe.h"
#include <cstring>
#include <sstream>
#include <getopt.h>
#include <chrono> //For high resolution timing.
#include <thread>
#include <stdio.h>
#include <string.h>
#include "cmdLIneArgs.hpp"
#include "makeHelpMessage.hpp"
#include "baseClass.hpp" 
#include "effectConnection.hpp"
#include "myPrototypes.hpp"
#include "alignField.hpp"
#include "StopWatch.hpp"
//#include "getMCS.hpp"
using namespace std;
extern const short numberOfThreads;
extern const char *BFM;
extern char *defalutResultsFileName[];
extern char *defaultDependentVariableColumn;
extern const char *const message;
extern const char *motherOfAllSelectStatements[];
extern const char *help_Message_Array[];
extern const int numberOfEntries;
extern const char *cases[];
extern const char *dependentVariableColumn[];
void initializeBaseClass (BaseClass *, BaseClass *);
int selectFromTable(register BaseClass *, int);
const std::string currentDateTime(BaseClass *);
const struct option longopts[] =
{
    {"dbname",    required_argument, 0, 'D'},
    {"help",   no_argument, 0, 'h'},
    {"userid",    required_argument, 0, 'U'},
    {"hostid",   required_argument, 0, 'H'},
    {"port",      required_argument, 0, 'P'},
    {"sql",       required_argument, 0, 'S'},
    {"kelvin", no_argument, 0, 'k'},
    {"resultsFile", required_argument, 0, 'f'},
    {"thread", no_argument, 0, 't'},
    {"debug1", no_argument, 0, '1'},
    {"debug2", no_argument, 0, '2'},
    {"debug3", no_argument, 0, '3'},
    {0,0,0,0}, //End of array
};
int main(int argc, char* argv[])  {
    bool thread=false;
    std::ofstream outfile;
    std::thread threads[MAINCASE];  //will create three threads
    auto startTest = std::chrono::high_resolution_clock::now();
    int (*ptf[3])(BaseClass *, int); //although not needed in this case, and just for intellectual stimulation of the exercise, let's do an array of pointers-to-function (ptf).
//NB: sizeof(longopts) gives the size of longopts in bytes. \
    Since we know that one entry of longopts is comprised of two pointers, each 12 bytes long, and two ints, each 4 bytes long, we know that one \
    entry of longopts is 12+4+12+4 = 32 bytes long. So we have to divide the size of longopts, in bytes, by the length of one entry in longopts (32bytes). \
    Shifting an unsighed int right by 5 bit places is the same as dividing by 32. thus the `>> 5`. \
    Then subtracting 1 from the result accounts for the {0,0,0,0} end of array indicator, thus giving us the number of possible command line arguments.
    CmdLineArgs *ptrCLA = new CmdLineArgs(thread, (-1 + ( (sizeof(longopts)) >> 5) )); //Instantiate a class named CmdLineArgs. \
    It is here where we define the default values and the real-time values of the command line arguments.
    BaseClass *ptrbc = new BaseClass( ptrCLA, MAIN); //Allocate main.cpp's BaseClass
 
    StopWatch *ptrSWmain[numberOfThreads];
    ptrSWmain[M1KWHCASE] = new StopWatch();
    ptrSWmain[M2KWHCASE] = new StopWatch();
    ptrSWmain[M1M2KWHCASE] = new StopWatch();
  
    const char *const commandLineSwitches = "123hktD:U:P:H:S:f:";
    int index;

    int numberOfMessageSegments = 0;
    while ( *(help_Message_Array + numberOfMessageSegments)  )  {numberOfMessageSegments++;} //Loop until we hit the pointer that points to location 0x'00 00 00 00 00 00 00 00', marking the end of the array.
    numberOfMessageSegments--; //Back-off 1 becaue the very last entry in the array, that gets counted, is an all zero terminator, but we want our count to indicate the number of valid addresses that point to strings.
    
    char  iarg= NULL; //Jumping though hoops just to make iarg to appear as a character in the debugger.
    //NB: Each thread should get its own copy of BaseClass
//    BaseClass *bc = new BaseClass( );  /* << =============== bc ================= */


    //turn off getopt error message
    opterr=1;
    MakeHelpMessage *MSG = new MakeHelpMessage( (*(argv +0) ),  &help_Message_Array[0], numberOfMessageSegments  );     /* << =============== MSG ================ */
    while( (iarg = (char)getopt_long(argc, argv, commandLineSwitches, longopts, &index)) != -1) {  //Jumping though hoops just to make iarg to appear as a character in the debugger.
        if (iarg < ZERO) break;
        switch (iarg)
        {
            case 'D': //Database Name
                ptrCLA->_clDatabaseName = optarg;
                std::cout << "Database Name: " << ptrCLA->_clDatabaseName << std::endl;
                break;
            case 'f': //resultsFile
                std::cout << "resultsFile: " << optarg << std::endl;
                ptrCLA->_results_File = optarg;
                break;
            case 'k': //Use kelvin temperatures as independent variable
                ptrCLA->_useKelvin = true;
                break;
            case 'U': //Userid
                ptrCLA->_clUserID = optarg;
                std::cout << "User ID: " << ptrCLA->_clUserID << std::endl;
                break;
            case 'H': //Hostid
                ptrCLA->_clHostID = optarg;
               std::cout << "HostID: " << ptrCLA->_clHostID << std::endl;
                break;
                
            case 'P':   //Port
                ptrCLA->_clPortID = optarg;
                std::cout << "PortID: " << ptrCLA->_clPortID << std::endl;
                break;
            case 'S':  //SQL
                ptrCLA->_clSQL = optarg;
                std::cout << "SELECT SQL STATEMENT looks like: " << ptrCLA->_clSQL << std::endl;
                break;
            case '1':  //Debug stage 1
                ptrCLA->_debug1=true;
                break;
            case '2': //Debug stage 2
                ptrCLA->_debug2=true;
                break;
            case '3': //Debug stage 3
                ptrCLA->_debug3=true;
                break;
            case 't': //If set then doing multithread processing
                thread=true;
                ptrCLA->threadSet(thread);
                std::cout << BFM <<"\nRunning in multi-thread mode\n" << BFM << endl;
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
    outfile.open(ptrCLA->_results_File, std::ios::out | std::ios::app) ; //open results file for appending
    if (outfile.fail()) {
        throw std::ios_base::failure(std::strerror(errno)) ;
    } else {
        //make sure write fails with exception if something is wrong
        outfile.exceptions(outfile.exceptions() | std::ios::failbit | std::ifstream::badbit);
    }
    char connectionString[MAXBUFSIZE];
    char *ptrConnectionStringBuffer = connectionString;
    cout << currentDateTime(ptrbc) << "\tLine " <<  __LINE__ <<" of File " << __FILE__ << ", compiled on " << __TIMESTAMP__ <<"\nBeginning execution of program:\n" << *(argv+0) << std::endl;
    outfile << currentDateTime(ptrbc) << "\tLine " <<  __LINE__ <<" of File " << __FILE__ << ", compiled on " << __TIMESTAMP__ <<"\nBeginning execution of program:\n" << *(argv+0) << std::endl;
//EffectConnection(char *usersConnectStringBuffer, const char *host="127.0.0.1", const char *user="cjc", const char *database="LocalWeather", const char *port="5436", const char *file="/Users/cjc/.pgpass");
    EffectConnection *ecs = new EffectConnection(ptrConnectionStringBuffer, ptrCLA->_clHostID, ptrCLA->_clUserID, ptrCLA->_clDatabaseName, ptrCLA->_clPortID  );
//    if (ptrCLA->_debug1) std::cout << "Connection String Looks Like: " << connectionString << std::endl;
    int rc=WONDERFUL;
    std::stringstream outstring = stringstream();
    
    //    PGconn *conn = ecs->connectToDataBase(ptrConnectionStringBuffer);  /* << =================================== */
    BaseClass *ptrbc1 = new BaseClass( ptrCLA, M1KWH);
    for (int i=0; i < WORKBUFSIZE; i++) *(ptrbc1->connectionString + i) = *(connectionString + i);
    ptrbc1->debugFlags.mycase = M1KWH;
//    ptrbc1->setConString(ptrConnectionStringBuffer);
//    ptrbc1->lookAtMyConnectionString = ptrConnectionStringBuffer;
    ptrbc1->BFS = *(motherOfAllSelectStatements + M1KWHCASE );
    ptrbc1->results_File = defalutResultsFileName[M1KWHCASE];
    ptrbc1->debugFlags.intMyCase = M1KWHCASE;
    ptrbc1->debugFlags.debug2 = ptrCLA->_debug2;
    ptrbc1->debugFlags.debug3 = ptrCLA->_debug3;
    std::ostringstream os1;
    ptrbc1->outstring = &os1;
    
//Let's make another clone of the original base class, bc, calling this instance bc2
    BaseClass *ptrbc2 = new BaseClass( ptrCLA, M2KWH);
    for (int i=0; i < WORKBUFSIZE; i++) *(ptrbc2->connectionString + i) = *(connectionString + i);
//    ptrbc2->setConString(ptrConnectionStringBuffer);
//    ptrbc2->lookAtMyConnectionString = ptrConnectionStringBuffer;
    ptrbc2->debugFlags.mycase = M2KWH;
    ptrbc2->debugFlags.intMyCase = M2KWHCASE;
    ptrbc2->debugFlags.debug2 = ptrCLA->_debug2;
    ptrbc2->debugFlags.debug3 = ptrCLA->_debug3;
    ptrbc2->BFS = *(motherOfAllSelectStatements + M2KWHCASE );
    ptrbc2->results_File = defalutResultsFileName[M2KWHCASE];

    std::ostringstream os2;
    ptrbc2->outstring = &os2;
    
//Let's make still another clone of the original base class, bc, calling this instance bc2
    
    BaseClass *ptrbc0 = new BaseClass( ptrCLA, M1M2KWH);
    for (int i=0; i < WORKBUFSIZE; i++) *(ptrbc1->connectionString + i) = *(connectionString + i);
//    ptrbc0->setConString(ptrConnectionStringBuffer);
//    std::memcpy((void *)ptrbc0->connectionString, (void *)connectionString, strlen(ptrbc0->connectionString) );
    ptrbc0->lookAtMyConnectionString = ptrConnectionStringBuffer;
    ptrbc0->BFS = *(motherOfAllSelectStatements + M1M2KWHCASE );
    ptrbc0->results_File = defalutResultsFileName[M1M2KWHCASE];
    ptrbc0->debugFlags.intMyCase = M1M2KWHCASE;
    ptrbc0->debugFlags.debug2 = ptrCLA->_debug2;
    ptrbc0->debugFlags.debug3 = ptrCLA->_debug3;
    std::ostringstream os0;
    ptrbc0->outstring = &os0;
    /*
    int yogisAge =3;
    os0 << "My string " << " says " << "The quick " << "brown fox " << "told me that Yogi is " << yogisAge << " years  old today, " << currentDateTime(workbuf) ;    std::cout << ptrbc0->outstring->str() << std::endl;
    ptrbc0->outstring->clear();
    */
//Now, let's try to spawn-off our thread and then we'll wait for it to complete
//    std::thread t1(selectFromTable, bc1, motherOfAllSelectStatements[M1KWHCASE]); //See myPrototypes.hpp for the definition of M1M2KWHCASE, M1KWHCASE, and M2KWHCASE.
//    t1.join(); //Wait for thread, t1, to end
//Using the pointer to function technique. First get the address of the function selectFromTable.
    StopWatch *sw = new StopWatch();    // Just testing this StopWatch object I got from github.
    ptf[M1M2KWHCASE] = selectFromTable;
    ptf[M1KWHCASE] = selectFromTable;
    ptf[M2KWHCASE] = selectFromTable;
    uint64_t time = sw->ElapsedNs();
    cout << "Time to initialize three pointers to functions: " << time << " ns." <<endl;
    delete sw;
    uint64_t elapsedTime[numberOfThreads];
    if(!thread) {

        ptrSWmain[M2KWHCASE]->Restart();
        rc = (*ptf[M2KWHCASE])(ptrbc2, -1);   //Go execute the M2KWHCASE
        elapsedTime[M2KWHCASE] = ptrSWmain[M2KWHCASE]->ElapsedUs();
        std::cout << ptrbc2->outstring->str() << std::endl;  //Output the M2KWHCASE results, right here.
        outfile << ptrbc2->outstring->str() << std::endl;     //Append the rensults onto the results file.
        std::cout << "Execution Time of selectFromTable for the M2KWHCASE is " << elapsedTime[M2KWHCASE] << " µsec."  <<std::endl;
        delete ptrSWmain[M2KWHCASE];
        if (ptrbc->selectFromTable_RC) cerr << "The M2KWHCASE ended with a rc of " << ptrbc->selectFromTable_RC << endl;
    } else {
        ptrSWmain[M2KWHCASE]->Restart();  //Start the clock to time the M2KWHCASE under multi-threaded conditions
        threads[M2KWHCASE] = std::thread((*ptf[M2KWHCASE]), ptrbc2, -2);
    }

    if(!thread) {

    ptrSWmain[M1KWHCASE]->Restart();
    (*ptf[M1KWHCASE])(ptrbc1, -1);
    elapsedTime[M1KWHCASE] = ptrSWmain[M1KWHCASE]->ElapsedUs();
    std::cout << ptrbc1->outstring->str() << std::endl;  //Output the M1KWHCASE results, right here.
    outfile << ptrbc1->outstring->str() << std::endl;     //Append the rensults onto the results file.
    std::cout << "Execution Time of selectFromTable for the M1KWHCASE is " << elapsedTime[M1KWHCASE] << " µsec."  <<std::endl;
    if (ptrbc->selectFromTable_RC) cerr << "The M1KWHCASE ended with a rc of " << ptrbc->selectFromTable_RC << endl;
    } else {
        ptrSWmain[M1KWHCASE]->Restart(); //ReSet the clock to begin timing the M2KWHCASE under multi-threaded conditions
        threads[M1KWHCASE] = std::thread((*ptf[M1KWHCASE]), ptrbc1, -1);
    }
    
    if(!thread) {
    ptrSWmain[M1M2KWHCASE]->Restart(); //ReSet the clock to time execution of the M1M2Case under sequential, single thread conditions.
    (*ptf[M1M2KWHCASE])(ptrbc0, -1);
    elapsedTime[M1M2KWHCASE] = ptrSWmain[M1M2KWHCASE]->ElapsedUs();
    std::cout << ptrbc0->outstring->str() << std::endl;  //Output the M1M2KWHCASE results, right here.
    outfile << ptrbc0->outstring->str() << std::endl;     //Append the rensults onto the results file.
    std::cout << "Execution Time of selectFromTable for the M1M2KWHCASE is " << elapsedTime[M1M2KWHCASE] << " µsec."  <<std::endl;
    if (ptrbc->selectFromTable_RC) cerr << "The M1M2KWHCASE ended with a rc of " << ptrbc->selectFromTable_RC << endl;
    } else {
        ptrSWmain[M1M2KWHCASE]->Restart(); //ReSet the clock to time execution of the M1M2Case under multi-thread thread conditions.
        threads[M1M2KWHCASE] = std::thread((*ptf[M1M2KWHCASE]), ptrbc0, 0);
        
    }




    

//Delete the objects we created.
    if (ecs != nullptr)  {
        //        ecs ->~EffectConnection();
        delete ecs ;  //2018-06-04T09:20:48 We die here, probably becaue we already deleted ecs. This delete seems to call BaseClass Destructor. So, let's remove this instruction.
        ecs = NULL;
    }
    if (thread ) {
        threads[M1M2KWHCASE].join(); //Wait for the M1M2KWHCASE thread to terminate before we pull the rug (ptrbc0) out from under it.
        cout << "First to Join: Time to execute the M1M2KWHCASE " << ptrSWmain[M1M2KWHCASE]->ElapsedUs() << " µsec. " << endl;
        outfile << "First to Join: Time to execute the M1M2KWHCASE " << ptrSWmain[M1M2KWHCASE]->ElapsedUs() << " µsec. " << endl;
//        auto endT0 = std::chrono::high_resolution_clock::now();
        std::cout << ptrbc0->outstring->str() << std::endl;  //Output the M1M2KWHCASE results on this terminal
        outfile << ptrbc0->outstring->str() << std::endl;     //Append the rensults onto the results file.
    }
    if (ptrbc0 != nullptr)  {
 //       ptrbc0 ->~BaseClass();
        delete ptrbc0 ;  //2018-06-04T09:20:48 We die here, probably becaue we already deleted ptrbc0. This delete seems to call BaseClass Destructor. So, let's remove this instruction.
        ptrbc0=NULL;
    }
    if (thread) {
        threads[M2KWHCASE].join(); //Wait for the M2KWHCASE thread to terminate before we pull the rug (ptrbc2) out from under it.
        cout << "Second to Join: Time to execute the M2KWHCASE " << ptrSWmain[M2KWHCASE]->ElapsedUs() << " µsec. " << endl;
        outfile << "Second to Join: Time to execute the M2KWHCASE " << ptrSWmain[M2KWHCASE]->ElapsedUs() << " µsec. " << endl;

        std::cout << ptrbc2->outstring->str() << std::endl;  //Output the M2KWHCASE results on this terminal
        outfile << ptrbc2->outstring->str() << std::endl;     //Append the rensults onto the results file.
    }
    if (ptrbc2 != nullptr)  {
//        ptrbc2 ->~BaseClass();
        delete ptrbc2 ;  //2018-06-04T09:20:48 We die here, probably becaue we already deleted ptrbc2. This delete seems to call BaseClass Destructor. So, let's remove this instruction.
        ptrbc2 = NULL;
    }
    if (thread) {
        threads[M1KWHCASE].join(); //Wait for the M1KWHCASE thread to terminate before we pull the rug (ptrbc1) out from under it.
        cout << "Last to Join: Time to execute the M1KWHCASE " << ptrSWmain[M1KWHCASE]->ElapsedUs() << " µsec. " << endl;
        outfile << "Last to Join: Time to execute the M1KWHCASE " << ptrSWmain[M1KWHCASE]->ElapsedUs() << " µsec. " << endl;
        std::cout << ptrbc1->outstring->str() << std::endl;  //Output the M1KWHCASE result on this terminal.
        outfile << ptrbc1->outstring->str() << std::endl;     //Append the rensults onto the results file.
//        auto duration0 = std::chrono::duration_cast<std::chrono::microseconds>(endT0 - startT0);
//        std::cout << "Execution Time of selectFromTable for the M1KWHCASE is " << duration0.count() << " µsec." << std::endl;
    }
    if (ptrbc1 != nullptr)  {
//        ptrbc1 ->~BaseClass();
        delete ptrbc1 ;  //2018-06-04T09:20:48 We die here, probably becaue we already deleted ptrbc1. This delete seems to call BaseClass Destructor. So, let's remove this instruction.
        ptrbc1 = NULL;
    }
    delete ptrbc;  //Delete main.cpp's BaseClass.
    delete ptrCLA;  //Delete main.cpp's CmdLineArg
    for(unsigned short j=0 ; j < numberOfThreads; j++) delete ptrSWmain[j];  //Delete all StopWatch objects
    auto endTest = std::chrono::high_resolution_clock::now() ;
    auto testDuration = std::chrono::duration_cast<std::chrono::microseconds>(endTest - startTest);
    std::cout << "Line " << __LINE__ << " of file " << __FILE__ << ": Total Execution Time of " << *(argv + 0) << ", for all cases, is " << testDuration.count() << " µsec." << std::endl;
    outfile    <<  "Line " << __LINE__ << " of file " << __FILE__ << ": Total Execution Time of program " << *(argv + 0) << ", for all cases, is " << testDuration.count() << " µsec.\n" << BFM << BFM << "\n" << std::endl;
    outfile.close(); //Close the results file.
//    return rc;

}
/*  Sample Results
 Thu Jun  7 09:04:30 2018    Line 44    File /Users/cjc/c++/c++Postgres/c++Postgres/main.cpp
 Beginning execution of program:
 /Users/cjc/Library/Developer/Xcode/DerivedData/Build/Products/Debug/multiThreadEnergyUsage
 Connection String Looks Like: dbname=LocalWeather host=localhost user=cjc port=5436 password=x…x
 WARNING:  there is no transaction in progress
 Wed Jun  6 18:30:03 2018    EnergyUsage(T) = 1.31496 + 0.00689808*T^1 + -0.001831*T^2 + 3.80354e-05*T^3    Case m2kwh
 Wed Jun  6 18:30:03 2018    cov = [1.33861, -0.13023, 0.00344566, -2.61324e-05]         Case m2kwh
 Wed Jun  6 18:30:03 2018    cov = [-0.13023, 0.0134487, -0.000370111, 2.89019e-06]         Case m2kwh
 Wed Jun  6 18:30:03 2018    cov = [0.00344566, -0.000370111, 1.06067e-05, -8.58998e-08]         Case m2kwh
 Wed Jun  6 18:30:03 2018    cov = [-2.61324e-05, 2.89019e-06, -8.58998e-08, 7.24336e-10]         Case m2kwh
 3.79781☜ χ-squared    Case m2kwh
 1.35207☜ Trace of Covariance Matrix    Case m2kwh
 0.864914☜ correlation coefficient    Case m2kwh
 36.9329☜ Square Root Of Sum Of TheSquares Of The Varience;
 0.0301002☜ Average Varience Per Valid Reading
 3.01002☜ % Average Varience Per Valid Reading
 1227☜ Number of valid meter readings (observed dependent variable)    Case m2kwh
 Execution Time of selectFromTable for the M2KWHCASE is 566313 µsec.
 WARNING:  there is no transaction in progress
 Wed Jun  6 18:30:03 2018    EnergyUsage(T) = 51.0471 + 1.39592*T^1 + -0.0654892*T^2 + 0.000524311*T^3    Case m1kwh
 Wed Jun  6 18:30:03 2018    cov = [34.5979, -2.96079, 0.0641666, -0.000408103]         Case m1kwh
 Wed Jun  6 18:30:03 2018    cov = [-2.96079, 0.270692, -0.0060336, 3.90408e-05]         Case m1kwh
 Wed Jun  6 18:30:03 2018    cov = [0.0641666, -0.0060336, 0.000137425, -9.04907e-07]         Case m1kwh
 Wed Jun  6 18:30:03 2018    cov = [-0.000408103, 3.90408e-05, -9.04907e-07, 6.05038e-09]         Case m1kwh
 4.90432☜ χ-squared    Case m1kwh
 34.8687☜ Trace of Covariance Matrix    Case m1kwh
 -0.945749☜ correlation coefficient    Case m1kwh
 10.1743☜ Square Root Of Sum Of TheSquares Of The Varience;
 0.00829199☜ Average Varience Per Valid Reading
 0.829199☜ % Average Varience Per Valid Reading
 1227☜ Number of valid meter readings (observed dependent variable)    Case m1kwh
 Execution Time of selectFromTable for the M1KWHCASE is 571053 µsec.
 WARNING:  there is no transaction in progress
 Wed Jun  6 18:30:03 2018    EnergyUsage(T) = 53.1018 + 1.34065*T^1 + -0.0662775*T^2 + 0.000557582*T^3    Case m1m2kwh
 Wed Jun  6 18:30:03 2018    cov = [38.59, -3.42338, 0.0790911, -0.000532065]         Case m1m2kwh
 Wed Jun  6 18:30:03 2018    cov = [-3.42338, 0.323207, -0.00770207, 5.2851e-05]         Case m1m2kwh
 Wed Jun  6 18:30:03 2018    cov = [0.0790911, -0.00770207, 0.000188592, -1.32271e-06]         Case m1m2kwh
 Wed Jun  6 18:30:03 2018    cov = [-0.000532065, 5.2851e-05, -1.32271e-06, 9.46174e-09]         Case m1m2kwh
 4.17123☜ χ-squared    Case m1m2kwh
 38.9134☜ Trace of Covariance Matrix    Case m1m2kwh
 -0.892979☜ correlation coefficient    Case m1m2kwh
 11.584☜ Square Root Of Sum Of TheSquares Of The Varience;
 0.00944092☜ Average Varience Per Valid Reading
 0.944092☜ % Average Varience Per Valid Reading
 1227☜ Number of valid meter readings (observed dependent variable)    Case m1m2kwh
 Execution Time of selectFromTable for the M1KWHCASE is 497068 µsec.
 Program ended with exit code: 0
 
*/

