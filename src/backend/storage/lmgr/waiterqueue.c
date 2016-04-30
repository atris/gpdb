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

#include "storage/proc.h"
#include "storage/lwlock.h"
#include "storage/waiterqueue.h"

int DeadlockTimeoutWaiterQueue = 1000;

WaiterQueue*
InitWaiterQueue()
{
  bool found = false;
  WaiterQueue *wque = NULL;

  wque = (WaiterQueue *) ShmemInitStruct("WaiterQueue",
                          WaiterQueueDataSize,
                          &found);

  if (found)
  {
    SHMQueueInit(&wque->links);
  }
  
  return wque;
}

PGPROC *
WaiterQueueWakeUp(WaiterQueue *waitQueue)
{
  PGPROC    *proc;
	PGPROC		*retProc;

  proc = (PGPROC *) MAKE_PTR(waitQueue->links.next);

  /* Proc should be sleeping ... */
	if (proc->links.prev == INVALID_OFFSET ||
		proc->links.next == INVALID_OFFSET)
		return NULL;

	/* Save next process before we zap the list link */
	retProc = (PGPROC *) MAKE_PTR(proc->links.next);

	/* Remove process from wait queue */
	SHMQueueDelete(&(proc->links));

	/* And awaken it */
	PGSemaphoreUnlock(&proc->sem);

	return retProc;
}
