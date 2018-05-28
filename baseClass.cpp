//
//  MakeConnectionString.cpp
//  energyUsageWithoutThe_PQXX_Bullshit
//
//  Created by Clifford Campo on 5/14/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//
#include "setupForMultiFit.hpp"
#include "baseClass.hpp"
#define NUMBEROFAVGTEMPS 100; //number of elements to be initially allocated to the gsl_vector type named independentVariableAvgTemp

// M E M B E R    F U N C T I O N       D E F I N I T I O N    F O R    C L A S S    M A K E C O N N E C T I O N S T R I N G
BaseClass::BaseClass(const char *myUserID, const char *myDatabaseName, const char *myPortID, const char *myHostID, const size_t maxNumberOfAvgTemps ) {
//    memset((void *)this->debugFlags.startHere, 0, THISSIZE); //Experiment on initializing memory in one fell swoop. (DOESN'T WORK)
    this->debugFlags.tableCreated = false;
    this->debugFlags.tempTableCreated = false;
    this->debugFlags.debug1 = false;
    this->debugFlags.debug2 = false;
    this->debugFlags.debug3 = false;
    this->debugFlags.useKelvin = false;
    this->debugFlags.includeM1InAverages = true;
    this->debugFlags.includeM2InAverages = true;
    this->debugFlags.mycase = M1M2KWH;
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
    this->grc = 0;
    this->brc = 0;
    this->trc = 0;
    memset(this->pwBuffer,0, MAXLENGTHPASSWORD); //Zero-out the password buffer so we don't get fooled by crap
    this->clPW = &pwBuffer[0];

}


BaseClass::~BaseClass() {

    if(this->ptrIndVariableArray != nullptr) delete [] this->ptrIndVariableArray;
    if(this->ptrExpMinusObsDepVarArray != nullptr) delete [] this->ptrExpMinusObsDepVarArray;
    if(this->ptrExpDepVarArray != nullptr) delete [] this->ptrExpDepVarArray;
    if(this->ptrObsDepVarArray != nullptr) delete [] this->ptrObsDepVarArray;
    if (this->debugFlags.debug1 || this->debugFlags.debug2 || this->debugFlags.debug3 ) {
        std::cout << "1. Within the MakeConnectionString Destructor, right before the delete, I hope we can delete the lookAtMyConnectionString Buffer. Connection String was " << strlen(this->lookAtMyConnectionString) << " bytes long. " << std::endl;
        std::cout << "2. Within the MakeConnectionString Destructor: I think we just deleted the lookAtMyConnectionString Buffer. Connection String was " << strlen(this->lookAtMyConnectionString) << " bytes long. " << std::endl;
    }
    if(this->lookAtMyConnectionString != nullptr) delete this->lookAtMyConnectionString;
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
