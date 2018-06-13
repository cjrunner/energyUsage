//
//  cmdLineArgs.hpp
//  multiThreadEnergyUsage
//
//  Created by Clifford Campo on 6/5/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//

#ifndef cmdLineArgs_hpp
#define cmdLineArgs_hpp

#include <iostream>
class CmdLineArgs {
private:
    const char *defaultDatabaseName = "LocalWeather";
    const char *defaultUserID = "cjc";
    const char *defaultHostID = "localhost";
    const char *defaultPortID = "5436";
    const char *defaultResultsFileName = "/Users/cjc/energyUsage/eu";
    bool multiThread = false;               //If true we are running as a multi-thread job. Set by constructor.
    unsigned short numberOfPossibleCommandLineArguments;
public:
    
    const char *_clDatabaseName;
    const char *_results_File;
    const char *_clUserID;
    const char *_clPW;
    const char *_clHostID;
    const char *_clPortID;
    const char *_clSQL;
    bool _debug1 = false;
    bool _debug2 = false;
    bool _debug3 = false;
    bool _useKelvin = false;
    void threadSet(bool);
    bool threadGet(void);
    CmdLineArgs(bool, const unsigned short);
    ~CmdLineArgs();
    friend class BaseClass; //Allow the class named BaseClass to access this class's private member date
    
    
};
#endif /* cmdLineArgs_hpp */
