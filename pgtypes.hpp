#define MAXSITENAMELENGTH 16  /* Max length of a site name */
#define MAXLENSITEID    3           /* Site IDs can be no more than three characters long */


#ifndef __STDBOOL__
#define __STDBOOL__ 1
#include <stdlib.h>
#endif

/* The following typedef shows the structures used by locate_sun.c. */
#ifndef __VARLOCATESUN__
#define __VARLOCATESUN__
typedef struct {  //This struct get instantiated in the master object getoptStuff (see header file FromGetOpt.h).
    double jd_beg;  //Julian Date of the first of the known astro values.
    double jd_end;  //Julian Date of the end of the last of the known astro values.
    double jd_utc;
    double jd_tt;
    double jd_ut1;
    double jd_tdb;
    double delta_t;
    double ra;
    double dec;
    double dis;
    double rat;
    double dect;
    double dist;
    double zd;
    double az;
    double rar;
    double decr;
    double gast;
    double last;
    double theta;
    double jd[2];
    double pos[3];
    double vel[3];
    double pose[3];
    double elon;
    double elat;
    double r;
    double lon_rad;
    double lat_rad;
    double sin_lon;
    double cos_lon;
    double sin_lat;
    double cos_lat;
    double vter[3];
    double vcel[3];
    short int error;
    short int de_num;
    
} varLocateSun;
#endif
//================================================================
#ifndef __SQLBUF__
#define __SQLBUF__
typedef struct {
#define MAXLENSQLCREATIONBUFFER 550 /*2018-03-30T17:35:45 Increased to 550 bytes.  Hopefully, 550 bytes is enough space! */
#define NUMBEROFSQLCREATIONBUFFERS 4 /* Number of sql creation buffers, each of length MAXLENSQLCREATIONBUFFER bytes */
    char *ptrSQLbuf1;
    char *ptrSQLbuf2;
    char *ptrSQLbuf3;
    char *ptrSQLbuf4;
    unsigned long long  struct_size;
    char SQLbuf1[MAXLENSQLCREATIONBUFFER];
    char crlf1[4];   /* Contains carriage return character, then the linefeed chracter then the zero character + a spare zero character */
    char SQLbuf2[MAXLENSQLCREATIONBUFFER];
    char crlf2[4];   /* Contains carriage return character, then the linefeed chracter then the zero character + a spare zero character */
    char SQLbuf3[MAXLENSQLCREATIONBUFFER];
    char crlf3[4];   /* Contains carriage return character, then the linefeed chracter then the zero character + a spare zero character */
    char SQLbuf4[MAXLENSQLCREATIONBUFFER];
    char crlf4[4];   /* Contains carriage return character, then the linefeed chracter then the zero character + a spare zero character */
    wchar_t last4characters[9]; /* Last four W I D E characters ***plus*** a string-terminating null character */
} sqlbuf;
#endif
//================================================================
#ifndef __MAKECS__
#define __MAKECS__
typedef  struct {
#define MAXLENCONNSTRING 150
    int returnCode;
    char *NotUsed;
    const char *file;
    char *port;
    char *database;
    char *userid;
    char *host; //Points to the host's IP address.
    char connString[MAXLENCONNSTRING];
    void *piob;      //Pointer to iobuf
} makeCS;
#endif
