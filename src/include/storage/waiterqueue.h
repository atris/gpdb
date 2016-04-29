/*-------------------------------------------------------------------------
 *
 * waiterqueue.h
 *	  Waiter queue for Resource Queue slots
 *
 *
 * Portions Copyright (c) 1996-2009, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/include/storage/waiterqueue.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef WAITERQUEUE_H
#define WAITERQUEUE_H

#include "storage/pg_shmem.h"
#include "storage/shmem.h"
#include "storage/pg_sema.h"

typedef struct WaiterQueue
{
  SHM_QUEUE links;
  PGSemaphoreData sem;		/* ONE semaphore to sleep on */
  int size;
} WaiterQueue;

#define WaiterQueueDataSize \
        ((Size)MAXALIGN(sizeof(WaiterQueue)))

extern bool isInitialized = false;

extern void InitWaiterQueue();
#endif   /* WAITERQUEUE_H */
