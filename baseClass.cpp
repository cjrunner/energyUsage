//
//  MakeConnectionString.cpp
//  energyUsageWithoutThe_PQXX_Bullshit
//
//  Created by Clifford Campo on 5/14/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//
#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>
#include "setupForMultiFit.hpp"
#include "cmdLineArgs.hpp"
#include "baseClass.hpp"
#include "myPrototypes.hpp"
extern const char *motherOfAllSelectStatements[];
#define NUMBEROFAVGTEMPS 100; //number of elements to be initially allocated to the gsl_vector type named independentVariableAvgTemp
const std::string currentDateTime(BaseClass *);
// M E M B E R    F U N C T I O N       D E F I N I T I O N    F O R    C L A S S    M A K E C O N N E C T I O N S T R I N G
BaseClass::BaseClass(CmdLineArgs *cla,  const char ourCase, const char *myUserID, const char *myDatabaseName, const char *myPortID, const char *myHostID, const size_t maxNumberOfAvgTemps)  {
    //If true we are running multi-thread. Obviously, multiThread ***MUST*** be the same value for all instances of BaseClass. Default value is FALSE. Command line switch `-t` sets this to TRUE.
    //    memset((void *)this->debugFlags.startHere, 0, THISSIZE); //Experiment on initializing memory in one fell swoop. (DOESN'T WORK)
    /*
     this->end = &this->e;
     this->start = &this->s;
     memset((char *)this->len, 0, (size_t)(&this->e - &this->s) );
     */
    CLA = cla;   //Point to the class named `CmdLineArgs` which we are using for this run. \
    There is only one instance of CmdLineArgs, although many instances of BaseClass may point to it; \
    a case of one parent (CmdLineArgs, or CLA) with multiple children.
    //First let's initiate certain of BaseClass's values with the default values as found in class CmdLineArgs:
    clUserID = CLA->defaultUserID;
    clHostID = CLA->defaultHostID;
    clPortID = CLA->defaultPortID;
    clDatabaseName = CLA->defaultDatabaseName;
    results_File = CLA->defaultResultsFileName;

    //Now let's initiate the rest of BaseClass's member data that needs initiating.
    switch (ourCase) {
        case M1M2KWH: //'0'
            this->debugFlags.intMyCase = M1M2KWHCASE;
            this->debugFlags.mycase = M1M2KWH;
            this->BFS = *(motherOfAllSelectStatements + M1M2KWHCASE );  //m1m2kwh is the default value
            this->debugFlags.dependentVariableColumn = "m1m2kwh"; //Here we have the Constructor allowed, during execution time, to define what value gets assigned to the `const char *`
            break;
        case M1KWH:   //'1'
            this->debugFlags.intMyCase = M1KWHCASE;
            this->debugFlags.mycase = M1KWH;
            this->BFS = *(motherOfAllSelectStatements + M1KWHCASE );  //m1m2kwh is the default value
            this->debugFlags.dependentVariableColumn = "m1kwh"; //Here we have the Constructor allowed, during execution time, to define what value gets assigned to the `const char *`
            this->debugFlags.includeM2InAverages = false; //If M1KWH, don't include M2KWH in averages.
            break;
        case M2KWH:   //'2'
            this->debugFlags.intMyCase = M2KWHCASE;
            this->debugFlags.mycase = M2KWH;
            this->BFS = *(motherOfAllSelectStatements + M2KWHCASE );  //m1m2kwh is the default value
            this->debugFlags.dependentVariableColumn = "m2kwh"; //Here we have the Constructor allowed, during execution time, to define what value gets assigned to the `const char *`
            this->debugFlags.includeM1InAverages = false; //If M2KWH, don't include M1KWH in averages.
            break;
        case MAIN:    //'3'
            this->debugFlags.intMyCase = MAINCASE;
            this->debugFlags.mycase = M2KWH;
//            this->BFS = *(motherOfAllSelectStatements + M2KWHCASE );  //m1m2kwh is the default value  NOT NEEDED FOR main.cpp because main.cpp does not issue any postgreSQL commands.
            this->debugFlags.dependentVariableColumn = "main"; //Here we have the Constructor allowed, during execution time, to define what value gets assigned to the `const char *`
            this->debugFlags.includeM1InAverages = false; //If M2KWH, don't include M1KWH in averages.
            break;
        default:
            std::cerr << currentDateTime(this) << " File " << __FILE__ << ", Line " << __LINE__ << " Unknown ourCase value: " << ourCase << " passed to BaseClass Constructor. These are the only legal values: " << M1M2KWH << " (for m1m2kwh), " << M1KWH << " (for m1kwh), and " << M2KWH << " (for m2kwh). " << __FILE__ << " is exiting at " <<  " with exit code of -1." << std::endl;
            exit(-1);
    }

    clDatabaseName = cla->_clDatabaseName;
    results_File = cla->_results_File;
    debugFlags.useKelvin = cla->_useKelvin;
    clUserID = cla->_clUserID;

    clPW = cla->_clPW;
    clHostID = cla->_clHostID;
    clPortID = cla->_clPortID;
    clSQL = cla->_clSQL;
    debugFlags.debug1 = cla->_debug1;
    debugFlags.debug2 = cla->_debug2;
    debugFlags.debug3 = cla->_debug3;
    lookAtMyConnectionString = connectionString; //Point to our connection string
    this->debugFlags.tableCreated = false;
    this->debugFlags.tempTableCreated = false;

    this->debugFlags.includeM1InAverages = true;
    this->debugFlags.includeM2InAverages = true;
    
    this->clUserID = myUserID;
    this->clDatabaseName = myDatabaseName;
    this->clPortID = myPortID;
    this->clHostID = myHostID;
    this->clSQL = NULL;
    this->debugFlags.countOfGoodEntries = 0;
    this->debugFlags.countOfBadEntries = 0;
    //    this->debugFlags.dependentVariableColumn = defaultDependentVariableColumn;
    this->debugFlags.includeM2InAverages = 1;
    this->ptrMaxNumberOfAvgTemps =  &maxNumberOfAvgTemps;
    this->ptrExpDepVarArray = NULL;
    this->ptrObsDepVarArray = NULL;
    this->ptrExpMinusObsDepVarArray = NULL;
    this->ptrIndVariableArray = NULL;
    this->bufferOffset = 0; // Initialize this counter.
    this->grc = 0;
    this->brc = 0;
    this->trc_t = 0;
    
    memset(this->pwBuffer,0, MAXLENGTHPASSWORD); //Zero-out the password buffer so we don't get fooled by stepping in crap!
    this->clPW = &pwBuffer[0];
    this->minBucketTemp = MINTEMPERATURE;
    this->buckettemp = minBucketTemp + BUCKETSIZE;
    this->maxBucketTemp = MAXTEMPERATURE;
    thisCount=INITIALVALUE;
    worthyOfPolynomialFitProcessing = INITIALVALUE;
    for (this->k=0; this->k < NUMBEROFVALUES; this->k++) values[k] = INITIALVALUE;
    this->k=0;
    errmsg = NULL;
    ptrValues = values;
    
    
}

BaseClass::~BaseClass() {
    
    if(this->ptrIndVariableArray != nullptr) delete [] this->ptrIndVariableArray;
    if(this->ptrExpMinusObsDepVarArray != nullptr) delete [] this->ptrExpMinusObsDepVarArray;
    if(this->ptrExpDepVarArray != nullptr) delete [] this->ptrExpDepVarArray;
    if(this->ptrObsDepVarArray != nullptr) delete [] this->ptrObsDepVarArray;
/*   Remove this code.
    if (this->debugFlags.debug1 || this->debugFlags.debug2 || this->debugFlags.debug3 ) {
        std::cout << "1. Within the MakeConnectionString Destructor, right before the delete, I hope we can delete the lookAtMyConnectionString Buffer. Connection String was " << strlen(this->lookAtMyConnectionString) << " bytes long. " << std::endl;
        std::cout << "2. Within the MakeConnectionString Destructor: I think we just deleted the lookAtMyConnectionString Buffer. Connection String was " << strlen(this->lookAtMyConnectionString) << " bytes long. " << std::endl;
    }
*/
    //  2018-06-04T08:17:48 WE NO LONGER DYNAMICALLY allocate  this->lookAtMyConnectionString   if(this->lookAtMyConnectionString != nullptr) delete [] this->lookAtMyConnectionString;
}

void BaseClass::setConString(const char *cs) { 
    this->lookAtMyConnectionString = cs;
}
void BaseClass::allocateArrays(size_t sizeOfEachArray) {
    this->sizeOfEachArray = sizeOfEachArray;
    this->ptrIndVariableArray = new double[sizeOfEachArray];
    this->ptrExpMinusObsDepVarArray = new double[sizeOfEachArray];
    this->ptrExpDepVarArray = new double[sizeOfEachArray];
    this->ptrObsDepVarArray = new double[sizeOfEachArray];
}
int BaseClass::setArray(int which, int k, double value) {
    switch(which) {
        case IV:
            *(this->ptrIndVariableArray + k) = value;
            break;
        case EMO:
            *(this->ptrExpMinusObsDepVarArray + k) = value;
            break;
        case EDVA:
            *(this->ptrExpDepVarArray + k) = value;
            break;
        case ODVA:
            *(this->ptrObsDepVarArray + k) = value;
            break;
        default:
            return BADWHICH; //Error
            
            
    }
    return 0;
}
const char *BaseClass::getPrivate(CmdLineArgs &cla, const char which) {
    switch (which) {
        case 'U':
            return (cla.defaultUserID);
            break;
        case 'P':
            return (cla.defaultPortID);
            break;
        case 'D':
            return (cla.defaultDatabaseName);
            break;
        case 'R':
            return (cla.defaultResultsFileName);
            break;
        case 'H':
            return (cla.defaultHostID);
            break;
        default:

            std::cerr << "Invalide `which` parameter. The only valid values are: \n`U` for defaultUserID; \n`P` for defaultPortID; \n`D` for defaultDatabaseName; \n`R` for defaultResultsFileName; and \n`H` for defaultHostID. All other values are invalid. We're returning NULL" << std::endl;
            return (NULL);
            break;
            
    }
}




