/*
 * Copyright(C) 2011-2016 Pedro H. Penna   <pedrohenriquepenna@gmail.com>
 *              2015-2016 Davidson Francis <davidsondfgl@hotmail.com>
 *
 * This file is part of Nanvix.
 *
 * Nanvix is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nanvix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nanvix. If not, see <http://www.gnu.org/licenses/>.
 */

#include <nanvix/clock.h>
#include <nanvix/const.h>
#include <nanvix/hal.h>
#include <nanvix/pm.h>
#include <signal.h>
#include <stdlib.h>
#include <nanvix/klib.h>

unsigned short lfsr = 0xACE1u;
unsigned bit;

unsigned myrand()
{
  bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
  return lfsr =  (lfsr >> 1) | (bit << 15);
}

#define MAXTICKETS 256
int TicketsDispo = 256 ;
struct process *TabTickets[MAXTICKETS];


PUBLIC void convert(struct process *p){
	if (p->nice < -30)
		p->tickets = 8;
	else if (p->nice < -20)
		p->tickets = 7;
	else if (p->nice < -10)
		p->tickets = 6;
	else if (p->nice < 0)
		p->tickets = 5;
	else if (p->nice < 10)
		p->tickets = 4;
	else if (p->nice < 20)
		p->tickets = 3;
	else if (p->nice < 30)
		p->tickets = 2;
	else
		p->tickets = 1;
}

PUBLIC void modif(struct process *p){
	if (p->state == PROC_ZOMBIE || p->state == PROC_DEAD || p == NULL){
		for (int i = 0 ; i<256 ; i++){
			if (TabTickets[i] == p){
				TabTickets[i] = NULL;
			}
		}
	}
	else{
		int compteur = 0;
		int i;
		for (i=0 ; i<256-TicketsDispo ; i++) {

			if (compteur == p->tickets){
				if (TabTickets[i] == p){
					TabTickets[i] = NULL ;
					TicketsDispo++;
				}
			}
			
			else if (TabTickets[i] == p){
				compteur++;
			}
		}

		if (compteur < p->tickets){
			while(compteur < p->tickets){
				i++;
				TabTickets[i] = p;
				TicketsDispo--;
				compteur++;
			}
		}
	}

	/* Order the tab */
	int j = 0;
	for (int i = 0 ; i<256 ; i++){
		if (TabTickets[j] == NULL){
			if (TabTickets[i] != NULL){
				TabTickets[j] = TabTickets[i];
				TabTickets[i] = NULL;
				j++;
			}
		}
		else{
			j++;
		}
	}
}



/**
 * @brief Schedules a process to execution.
 * 
 * @param proc Process to be scheduled.
 */
PUBLIC void sched(struct process *proc)
{
	proc->state = PROC_READY;
	proc->counter = 0;
	modif(proc);
}

/**
 * @brief Stops the current running process.
 */
PUBLIC void stop(void)
{
	curr_proc->state = PROC_STOPPED;
	sndsig(curr_proc->father, SIGCHLD);
	modif(curr_proc);
	yield();
}

/**
 * @brief Resumes a process.
 * 
 * @param proc Process to be resumed.
 * 
 * @note The process must stopped to be resumed.
 */
PUBLIC void resume(struct process *proc)
{	
	/* Resume only if process has stopped. */
	if (proc->state == PROC_STOPPED)
		sched(proc);
}

/**
 * @brief Yields the processor.
 */
PUBLIC void yield(void)
{
	ksrand(257);
	struct process *p;    /* Working process.     */
	struct process *next; /* Next process to run. */
	// int found ;

	/* Re-schedule process for execution. */
	if (curr_proc->state == PROC_RUNNING)
		sched(curr_proc);

	/* Remember this process. */
	last_proc = curr_proc;

	/* Check alarm. */
	for (p = FIRST_PROC; p <= LAST_PROC; p++)
	{
		/* Skip invalid processes. */
		if (!IS_VALID(p))
			continue;
		
		/* Alarm has expired. */
		if ((p->alarm) && (p->alarm < ticks))
			p->alarm = 0, sndsig(p, SIGALRM);
	}


	/* Choose a process to run next. */
	next = IDLE;

	int RandomTicket = (int)(myrand()%(MAXTICKETS-TicketsDispo+1));
	next = TabTickets[RandomTicket];
	/* Skip non-ready process. */
	while (next->state != PROC_READY){
		RandomTicket = (int)(krand()%(MAXTICKETS-TicketsDispo+1));
		next = TabTickets[RandomTicket];
	}
	
	/* Switch to next process. */
	next->priority = PRIO_USER;
	next->state = PROC_RUNNING;
	next->counter = PROC_QUANTUM;
	switch_to(next);
}