//
//  main.h
//  makeConnectionString
//
//  Created by Clifford Campo on 8/12/17.
//  Copyright © 2017 Clifford Campo. All rights reserved.
//
#include "baseClass.hpp"
extern "C"

#include <stdio.h>
#include "/Users/cjc/C/PG/includes/pgtypes.h"

//
//  iobuf.hpp
//  ParsePGpass
//
//  Created by Clifford Campo on 8/8/17.
//  Copyright © 2017 Clifford Campo. All rights reserved.
/*  2017-11-24T09:04 Modified by adding STARTOFHOSTID, NUMBEROFPGPASSITEMS, and the number of items in array
 arrayOfPointersToSeparatorCharacter to the value specified by NUMBEROFPGPASSITEMS rom 4. Modifications driven
 by the need to create a connection string that allows for a host id other than ‘localhost‘
 Other modifications were made to makeConnectionString, including the function prototype
 ‘findDesiredRecord‘
 */
//
#ifndef __IOBUF__
#define __IOBUF__

#define MAXBUFSIZE               256
#define SUBBUFSIZE               50
#define STARTOFHOSTID            0
#define BETWEENHOSTANDPORT       1 + STARTOFHOSTID                    //2017-11-23T09:09 added
#define BETWEENPORTANDDATABASE   1 + BETWEENHOSTANDPORT               //2017-11-23T09:09 modified
#define BETWEENDATABASEANDUSERID 1 + BETWEENPORTANDDATABASE           //2017-11-23T09:09 modified
#define BETWEENUSERIDANDPASSWORD 1 + BETWEENDATABASEANDUSERID         //2017-11-23T09:09 modified
#define NUMBEROFPGPASSITEMS      1 + BETWEENUSERIDANDPASSWORD         //2017-11-23T09:09 added
#define NUMBEROFITERATIONS       1500
#define SEPERATORCHARACTER       ':'

class EffectConnection  {
//typedef struct {
private:
    FILE *PGPASS;
    char separatorCharacter;
    const char *arrayOfPointersToSeparatorCharacter[NUMBEROFPGPASSITEMS];        //2017-11-23T09:09 modified
    char separatorCharacterFoundHere = ':';
    const char *mainbuf;
    const char *host;                                                               //2017-11-23T09:09 added
    const char *port;
    const char *database;
    const char *user;
    const char *pw;
    char *subbuf;
    char *workbuf;
    unsigned char *subbufPGPASS;  //Represents one blocksize of fgets' input buffer.

    char connstring[MAXBUFSIZE];
    int  recordCounter;
    int  mbs;    //Max Buffer Size;
    int returnCode;
    struct timespec start;
    struct timespec stop;
public:
    char *startOfStringSegment;
    const char *cs; //points to Resulting constring
    PGconn *connectionObject;
//    int makeConnectionString( void );
    PGconn *connectToDataBase(const char *);
    EffectConnection(char *usersConnectStringBuffer, const char *host="127.0.0.1", const char *user="cjc", const char *database="LocalWeather", const char *port="5436", const char *file="/Users/cjc/.pgpass");
    ~EffectConnection();
//    void current_utc_time(struct timespec *);
    long diff_micro(struct timespec *, struct timespec *);
    FILE * openFile(char *, char *);
    int    closeFile(FILE *);
    const char * readLine(char *, int, int *, FILE * );
//    char * findDesiredRecord(char *theBuffer, uint8_t sepChar, char *theHost, size_t lenHost, char *thePort, size_t lenPort, char *theDatabase, size_t lenDatabase, char *theUserID, size_t lenUserID);
    const char * findDesiredRecord(char *, uint8_t, char *, size_t, char *, size_t, char *, size_t, char *, size_t); /* 2017-11-23T09:09
                                                                                                                         The first char * points to the input buffer and teh uint8_t defines the sparator character;
                                                                                                                         the second char * points to the separator character;
                                                                                                                         the third char * points to the host's id;
                                                                                                                         the fourth char * point to the port id;
                                                                                                                         the fifth char * points to the database's name;
                                                                                                                         the sixth char * points to the userid (user's name as known by the database management system).
                                                                                                                         */
    const char * scanForSeparator(char *, char *, unsigned long);
    const char * extractString(char *);
    int makeConnectionString(makeCS *);   /* Function used for creating postgresql connection strings */
};
#endif
