//
//  currentDateTime.cpp
//  multiThreadEnergyUsage
//
//  Created by Clifford Campo on 6/8/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//  See https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
//


#include "currentDateTime.hpp"

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime(BaseClass *bc) {
    bc->now = time(0);
    bc->tstruct = *localtime(&bc->now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    /* the tm struct looks like:
     buf    char *    0x7ffeefbff680    0x00007ffeefbff680
     *buf    char    '2'
     now    time_t    1528493750
     tstruct    tm
     tm_gmtoff    long    -14400  //offset in seconds. divide by 3600 to get hours.
     tm_hour    int    17
     tm_isdst    int    1
     tm_mday    int    8
     tm_min    int    35
     tm_mon    int    5
     tm_sec    int    50
     tm_wday    int    5
     tm_yday    int    158
     tm_year    int    118
     tm_zone    char *    "EDT"    0x0000000101011c4c
     *tm_zone    char    'E'   
     */
    //Carefully construct the time in human-readable ISO 8601 format from the data supplied in the tm structure.
    sprintf(bc->buf,  \
            "%d-%02d-%02dT%02d:%02d:%02d %s", \
            (YEAROFSTANDARDEPOCH + bc->tstruct.tm_year), \
            (1 + bc->tstruct.tm_mon),  /* Note that duirng the month of January tm_mon = 0 */  \
            bc->tstruct.tm_mday, \
            bc->tstruct.tm_hour, \
            bc->tstruct.tm_min, \
            bc->tstruct.tm_sec, \
            bc->tstruct.tm_zone);
    
    return bc->buf;
}

