//
//  MakeHelpMessage.cpp
//  energyUsageWithoutThe_PQXX_Bullshit
//
//  Created by Clifford Campo on 5/17/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//
#include "makeHelpMessage.hpp"
#include "baseClass.hpp"
#include <iostream>
extern const char *helpMessageArray[];
extern const char *BFM;
MakeHelpMessage::MakeHelpMessage(const char *p, const char *helpMessage[0], int numberOfEntries) {
    this->numberOfEntries = numberOfEntries;
    this->helpMessageArray = &helpMessage[0];
    yourMessage = this->message;
    programName = p;
    yourProgramName = p;
    this->ptrToMessage = NULL;
}
MakeHelpMessage::~MakeHelpMessage() {
    std::cout << "In MakeHelpMessage destructor" << std::endl;
    if (this->ptrToMessage != nullptr) delete[] (this->ptrToMessage); //If we acquired storage, clean up the acquired storage
    this->ptrToMessage = NULL;
}
char *MakeHelpMessage::createMessage (const char *nameOfThisProgram) { 
    long lengthOfHelpMessage = strlen(nameOfThisProgram);
    for (int i = 0; i < this->numberOfEntries; i++) {
        lengthOfHelpMessage += strlen(this->helpMessageArray[i]);
    }
    lengthOfHelpMessage  += strlen (BFM);
    

    this->ptrToMessage = new char [lengthOfHelpMessage];
    
    std::strcat(this->ptrToMessage, helpMessageArray[0]);
    std::strcat(this->ptrToMessage, nameOfThisProgram);
    for (int i = 1; i < numberOfEntries; i++) {
       std::strcat(this->ptrToMessage, helpMessageArray[i]);
    }

    std::strcat(this->ptrToMessage, BFM);
    return this->ptrToMessage;
}
