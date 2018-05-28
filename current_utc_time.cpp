extern "C"
#include <time.h
#include <mach/mach_time.h> /* mach_absolute_time */
#include <mach/mach.h>      /* host_get_clock_service, mach_... */
#include <mach/clock.h>     /* clock_get_time */
#include <mach/mach_types.h>
#include <mach/mach_init.h>
void current_utc_time(struct timespec *);   //Function Prototype
long diff_micro(struct timespec *, struct timespec *);
long diff_time(struct timespec *, struct timespec *);
extern double deltaTime(long, long, long, long);
extern kern_return_t host_get_clock_service(host_t host, clock_id_t clock_id, clock_serv_t *clock_serv);
extern kern_return_t mach_port_deallocate(ipc_space_t task, mach_port_name_t name);
void current_utc_time(struct timespec *ts) {  //Function definition is heer.
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock,&mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts->tv_sec = mts.tv_sec;
    ts->tv_nsec= mts.tv_nsec;
}
long diff_micro(struct timespec *start, struct timespec *stop)  //D E P R E C A T E D
{
    /* Compute time difference, between start time and stop time, in microseconds */
    return ((stop->tv_sec * (1000000)) + (stop->tv_nsec / 1000)) - ((start->tv_sec * 1000000) + (start->tv_nsec / 1000));
}
long delta_time(struct timespec start, struct timespec stop)
{
    if(stop.tv_nsec < start.tv_nsec) {
        stop.tv_nsec = stop.tv_nsec + WHATASECONDSWORTHOFNANOSECONDSLOOKSLIKE;
        stop.tv_sec--; //subtract a second off of tv_sec to simulate borrowing to make stop->tv_nsec > start->tv_nsec
    }
    if ((stop.tv_sec - start.tv_sec) < 0 ) {
        return -1;
    } else {
        return ( (stop.tv_sec - start.tv_sec)*WHATASECONDSWORTHOFNANOSECONDSLOOKSLIKE + (stop.tv_nsec - start.tv_nsec) );
    }
    /* Compute time difference, between start time and stop time, in microseconds */
}
double  deltaTime(long startSec, long startNsec, long stopSec, long stopNsec) {
    if (startNsec> stopNsec) {
        stopSec--;
        stopNsec += WHATASECONDSWORTHOFNANOSECONDSLOOKSLIKE ;
        
    }
    return (1.0 * (stopSec-startSec) + 1.0 * (stopNsec - startNsec)/WHATASECONDSWORTHOFNANOSECONDSLOOKSLIKE);
}
