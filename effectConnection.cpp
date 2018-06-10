// #include "baseClass.hpp"
#include "effectConnection.hpp"

extern "C"
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

const uint8_t separator=':';
using namespace std;
const char * EffectConnection::findDesiredRecord(char *theBuffer, \
                                                 uint8_t sepChar, \
                                                 char *theHost, \
                                                 size_t lenHost, \
                                                 char *thePort, \
                                                 size_t lenPort, \
                                                 char *theDatabase, \
                                                 size_t lenDatabase, \
                                                 char *theUserID, \
                                                 size_t lenUserID) {
    //Need to use strncmp because we don't have null-terminated strings, instead we have colon-terminated strings; strcmp only works with null-terminated strings.


    this->workbuf = theBuffer;  //Make a copy of the start of the input buffer. This will be our working pointer.
    this->startOfStringSegment = theBuffer;
//search for the first separator character that separates the hostid from the port id.
    while (theBuffer + SUBBUFSIZE - this->workbuf >0 && *this->workbuf != '\0') {
        if (*this->workbuf == separator) {  //Oh, by the way, did we encounter a separator character as we traversed this buffer ?
            this->arrayOfPointersToSeparatorCharacter[STARTOFHOSTID] = this->workbuf;  // If so, save its location in our array of pointers to string; //This marks the end of the first string segment.
            *this->workbuf = '\0'; //Set this first encountered separator character to NULL.
            if(strcmp(this->startOfStringSegment, theHost) != 0) {
                return(NULL);  //Return with indication that we failed to match the hostid.
            } else {
                this->workbuf++;
                this->startOfStringSegment = this->workbuf; //Have the buffer now point 1 character past what was the separator character, thus what is now the start of the second string segment: the database's name.
                break; //Exit while loop
            }
        } else {
            this->workbuf++; //Go to this segment's next character
        }
    }
//search for the second separator character that separates the port id from the database's name.
    while (theBuffer + SUBBUFSIZE - this->workbuf >0 && *this->workbuf != '\0') {
        if (*this->workbuf == separator) {  //Oh, by the way, did we encounter a separator character as we traversed this buffer ?
            this->arrayOfPointersToSeparatorCharacter[BETWEENHOSTANDPORT] = this->workbuf;  // If so, save its location in our array of pointers to string;
            *this->workbuf = '\0'; //Set this second-encountered separator character to NULL.
            if(strcmp(this->startOfStringSegment, thePort) != 0 ) {
                return(NULL);
            } else {
                workbuf++;
                this->startOfStringSegment = this->workbuf; //Have the buffer now point 1 character past what was the separator character, thus what is now the start of the second string segment: the port id.
                break; //Exit while loop
            }
        } else {
            this->workbuf++; //Go to this segment's next character
        }
    }
//Search for the third separator charater that separates the datebase's name from the user id.
    if (*workbuf == '\0') return (NULL);
    while (theBuffer + SUBBUFSIZE - this->workbuf >0  && *this->workbuf != '\0') {
        if (*this->workbuf == separator) {  //Oh, by the way, did we encounter a separator character as we traversed this buffer ?
            this->arrayOfPointersToSeparatorCharacter[BETWEENPORTANDDATABASE] = this->workbuf;  // If so, save its location  in our array of pointers to string;
            *this->workbuf = '\0';
            if(strcmp(this->startOfStringSegment, theDatabase) != 0) {
                return(NULL);
            } else {
                this->workbuf++;
                this->startOfStringSegment = this->workbuf; //Have the buffer now point 1 character past what was the separator character, thus what is now the start of the third string segment: the database user id.
                break; //Exit while loop
            }
        } else {
            this->workbuf++; //Go to this segment's next character
        }
    }
//search for the fourth separator character that separates the database user id from the password.
    if (*this->workbuf == '\0') return (NULL);
    while (theBuffer + SUBBUFSIZE - workbuf >0  && *this->workbuf != '\0') {
        if (*this->workbuf == separator) {  //Oh, by the way, did we come accross a separator character as we traversed this buffer ?
            this->arrayOfPointersToSeparatorCharacter[BETWEENDATABASEANDUSERID] = this->workbuf;  // If so, save its location in our array of pointers to string;
            *this->workbuf = '\0';
            if(strcmp(this->startOfStringSegment, theUserID) != 0) {
                return(NULL);
            } else {
                this->workbuf++; //Go past the separator character and we should be pointing to the password, which we don't have foreknowledge of what it is.
                break;
            }
        } else {
            this->workbuf++; //Go to this segment's next character
        }
    }
    return(this->workbuf); //Return to caller with a pointer to the password.
}

char *readLine(char *inputBuffer, int maxBufferSize, int *rec, FILE *fp) {
    rec++;
    if (fgets(inputBuffer, maxBufferSize, fp) != NULL) {
        return(inputBuffer);
    } else {
        return(NULL);
    }
}
char *scanForSeparator(char *buf, char *sep, unsigned long bufsize) {
    while (buf < buf+bufsize) {
        if(strncmp(buf, sep, strlen(sep) ) != 0) {
            buf++;        //Point to next character location
        } else {
            return(buf);  //Return with the address of the separator character we encountered.
        }
    }
    return(NULL);
}
EffectConnection::~EffectConnection() {
    //Check to see if PGPASS was already closed before trying to cloase it.
    if(this->PGPASS != nullptr) {
        fclose(this->PGPASS);
        this->pw = NULL;
    }
}
PGconn *EffectConnection::connectToDataBase(const char *cs) {
    try {
        this->connectionObject = PQconnectdb(cs);  //Attempt to connect to database
        if ( PQstatus (this->connectionObject) != CONNECTION_OK) {
            std::cerr << "Connection to database " << this->database<< " failed because of: " <<PQerrorMessage(this->connectionObject) << std::endl;
            PQfinish(this->connectionObject);
            exit (2);
        } else {
            std::cerr << "Successfully connected to database " << this->database <<  std::endl;
        }
    } catch  (const std::exception &e) {
        std::cerr << "Failed to connect to database using connection string " << cs << std::endl;
        exit (1);
    }
    return this->connectionObject;
}


EffectConnection::EffectConnection(char *cs, const char *host, const char *user, const  char *database, const char *port, const  char *file) {
    this->recordCounter = 0;
    this->host = host;

    this->user = user;

    this->database = database;

    this->port = port;
    this->separatorCharacterFoundHere = SEPERATORCHARACTER;
    this->PGPASS = fopen(file, "r");
 //   void current_utc_time(struct timespec *);
 //   static const char *filename;
 //   filename = cs->file;
 //   static const char iotype[] = "r";

 //   current_utc_time(&this->start);   /* Start the timer */
    
    this->mainbuf = this->connstring;
    this->subbuf  = this->connstring + MAXBUFSIZE - SUBBUFSIZE;
        while (1) {
            std::fgets(this->subbuf,  SUBBUFSIZE, this->PGPASS);  //This fgets really reads in the whole file. Check out structure of type FILE (PGPASS) _p member entry.
            this->subbufPGPASS = this->PGPASS->_p; //this->subbufPGPASS point to fgets' internal input buffer.
            if (this->subbuf != NULL  ) {   //Have we processed all records?
                this->recordCounter++;
                if ( (this->pw = findDesiredRecord( (char *)this->subbuf, separator,  (char *)this->host, (size_t)strlen(this->host), (char *)this->port, (size_t )strlen(this->port), (char *)this->database, (size_t )strlen(this->database), (char *)this->user, (size_t)strlen(this->user)  ) ) != NULL) {
                    fclose(this->PGPASS); //Close this->PGPASS because we've found the desired entry.
                    this->PGPASS = NULL; //Indicate file is not opened.
                    break; //Break out while loop if we found the record we're looking for.
                } else {
                    continue;
                }
                
            } else {
                this->returnCode=-5;
                fclose(this->PGPASS); //Close this->PGPASS because we failed to find the desired record.
                this->PGPASS = NULL;  //Indicate File Pointer PGPASS is closed
                
            }
        }

//this->mbs contains the size of the connection strig we just constructed.
    this->mbs = snprintf(this->connstring, MAXBUFSIZE - SUBBUFSIZE, "dbname=%s host=%s user=%s port=%s password=%s", this->database, this->host, this->user, this->port, this->pw ); //Put all the pieces together to make a connection string.
    //At this point i->mainbuf contains what looks like the connection string. However, we will now construct the connect string piece-by-piece.
    //    delete i;  //Let's see if the destructor gets called.
    strncpy(cs, this->connstring, (strlen(this->connstring)-1)); //The '-1' is there to prevent copying over the '\n' character that immediately follows the password's rightmost character.

}
