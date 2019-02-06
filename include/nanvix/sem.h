#ifndef NANVIX_SEM_H_
#define NANVIX_SEM_H_

#include <nanvix/pm.h>


/* Structure du semaphore */
struct semaphore {
	unsigned int val;
  unsigned int nb_attente;
	struct process*	wait_list[PROC_MAX];  //TABLEAU DES PROCESSUS EN ATTENTE
};

EXTERN void create(int n);
EXTERN void down(struct process* sem);
EXTERN void up(struct semaphore* sem);
EXTERN void destroy(struct semaphore* sem);




#endif /* NANVIX_PM_H_ */