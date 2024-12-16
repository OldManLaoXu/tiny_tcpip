#ifndef NLOCKER_H
#define NLOCKER_H

#include "sys_plat.h"
#include "net_err.h"

typedef enum _nlocker_type_t{
    NLCOKER_NONE,
    NLCOKER_THREAD,
}nlocker_type_t;

typedef struct _nlocker_t{
    nlocker_type_t type;

    union{
        sys_mutex_t mutex;
        //...
    };
}nlocker_t;

net_err_t nlocker_init(nlocker_t *locker, nlocker_type_t type);
void nlocker_destroy(nlocker_t *locker);
void nlocker_lock(nlocker_t *locker);
void nlocker_unlock(nlocker_t *locker);

#endif