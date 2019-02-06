#ifndef NANVIX_SEM_H_
#define NANVIX_SEM_H_

#include <nanvix/pm.h>


/* Structure du semaphore */
struct semaphore {
	unsigned int val;
  unsigned int key;
  unsigned int nb_attente;
  int used;
	struct process*	wait_list[PROC_MAX];  //TABLEAU DES PROCESSUS EN ATTENTE
};

EXTERN void reset_tab_sem();

EXTERN int create(int n, unsigned int key);
EXTERN void down(struct process* sem);
EXTERN void up(struct semaphore* sem);
EXTERN void destroy(struct semaphore* sem);


#endif /* NANVIX_PM_H_ */