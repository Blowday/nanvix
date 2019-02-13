#ifndef NANVIX_SEM_H_
#define NANVIX_SEM_H_

#include <nanvix/pm.h>


/* Structure du semaphore */
struct semaphore {
	int val;
  unsigned int key;
  unsigned int nb_attente;
  int used;
	struct process*	chain;  //TABLEAU DES PROCESSUS EN ATTENTE
};

EXTERN void reset_tab_sem();

EXTERN int create(int n, unsigned int key);
EXTERN int down(int semid);
EXTERN int up(int semid);
EXTERN int destroy(int semid);

EXTERN int getID(unsigned int key);

EXTERN int getVal(int semid);
EXTERN int setVal(int semid, int val);


#endif /* NANVIX_PM_H_ */