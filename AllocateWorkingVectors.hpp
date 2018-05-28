//
//  AllocateWorkingMemory.hpp
//  energyUsage
//
//  Created by Clifford Campo on 5/3/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//

#ifndef AllocateWorkingVectors_hpp
#define AllocateWorkingVectors_hpp

#include <iostream>
//   #include "makeConnectionString.hpp" //Need this include to resolve the inherited MakeConnectionString class.
class AllocateWorkingVectors  { // AllocateWorkingVectors object inherits the MakeConnectionString Object
private:
    size_t numberOfRows;
public:
    double *temperatureVector;
    double *energyUsageVector;
    double *weightVector; //Occasionally we'll find a value other than one.
    AllocateWorkingVectors(size_t);
    ~AllocateWorkingVectors();
};
AllocateWorkingVectors::AllocateWorkingVectors(size_t nor)  {
    this->numberOfRows = nor;
    this->temperatureVector = new double[nor];  // << ===================================
    this->energyUsageVector = new double[nor];  // << ===================================
    this->weightVector = new double[nor];        // << ===================================

    
}
AllocateWorkingVectors::~AllocateWorkingVectors () {
    std::cout << "1. Deleting temperatureVector which is " << sizeof(this->temperatureVector) << " bytes long." << std::endl;
    if (this->temperatureVector != nullptr) delete this->temperatureVector;
    std::cout << "2. Deleting energyUsageVector which is " << sizeof(this->energyUsageVector) << " bytes long." << std::endl;
    if (this->energyUsageVector != nullptr) delete this->energyUsageVector;
    std::cout << "3. Deleting weightVector which is " << sizeof(this->weightVector) << " bytes long." << std::endl;
    if (this->weightVector != nullptr)  delete this->weightVector;
    
}


#endif /* AllocateWorkingMemory_h */
