//
//  BitFlags.hpp
//  energyUsage
//
//  Created by Clifford Campo on 5/10/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//

#ifndef BitFlags_hpp
#define BitFlags_hpp
extern "C"
#include <iostream>
#include <cstddef>
#define M1M2KWH '0'
#define M1KWH '1'
#define M2KWH '2'
#define MAIN  '3'  //Used by the main.cpp
#define BADMYCASEVALUE 4
#define THISSIZE sizeof(BitFlags) - sizeof(size_t)
typedef struct BitFlags {
    size_t szBitFlags=THISSIZE;
    size_t startHere;
    size_t countOfGoodEntries;
    size_t countOfBadEntries;
    const char *dependentVariableColumn;  //Indicates which dependent case to use: m1m2kwh, m1kwh, or m2kwh.
    size_t indexIntoMotherOfAllSelectStatements;  //takes on the values of 0, 1, or 2
    bool   includeM2InAverages;
    bool   includeM1InAverages;
    bool   tableCreated;
    bool   tempTableCreated;
    unsigned short int  intMyCase; //Integer representation of the following.
    char  mycase;                  //'0'==>m1m2kwh case, 1==> m1kwh case, 2==>m2kwh case. This flag is set in main. (Note, can't use the word case, it's reserved for the compiler)
    bool  useKelvin;               //If true, use as independent variable kelvin temperatures 
    int8_t debug1; //when set, output level 1 debug messages (main.cpp)
    int8_t debug2; //when set, output level 2 debug messages (selectFromTable. cpp)
    int8_t debug3; //when set, output level 3 debug messages (InsertIBucketizedEnergyValuesIntoTable.cpp)
} BitFlags;

#endif /* BitFlags_h */
