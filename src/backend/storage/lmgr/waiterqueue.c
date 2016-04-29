/*-------------------------------------------------------------------------
 *
 * waiterqueue.c
 *	  Waiter Queue management routines
 *
 * Portions Copyright (c) 1996-2008, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $PostgreSQL: pgsql/src/backend/optimizer/util/waiterqueue.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "storage/lwlock.h"
#include "storage/waiterqueue.h"

void
InitWaiterQueue(WaiterQueue *wque)
{
  bool found = false;

  LWLockAcquire(WaiterQueueLock, LW_EXCLUSIVE);

  if (isInitialized == false)
  {
    wque = ShmemInitStruct("WaiterQueue",
                              WaiterQueueDataSize,
                              &found);

    if (!found)
    {
      SHMQueueInit(&wque->waiterQueue);
    }

    isInitialized = true;
  }

  LWLockRelease(WaiterQueueLock);
}
