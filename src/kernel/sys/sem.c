#include <nanvix/const.h>
#include <nanvix/sem.h>

int nb_sem = 0;
struct semaphore tab_sem[PROC_MAX];  //Nombre arbitraire de semaphore max

/* Fonctions utilitaires */
PUBLIC void reset_tab_sem() {
  for(int i=0; i<PROC_MAX; i++) {
    tab_sem[i].used = 0;
  }
}
int first_free() {
  int i=0;
  while(tab_sem[i].used == 1) {
    i++;
  }
  return i;
}


/* Initialiser un semaphore
 * Entrée : Valeur de base du sémaphore
 * Sortie : Id du sémaphore (dans le tableau) */
PUBLIC int create(int n, unsigned int key) {
  /* Test si il reste de la place dans le tableau de semaphores */
  if(nb_sem == PROC_MAX) {
    return -1;
  }else {
    /* On place le semaphore dans le premier index libre */
    int i = first_free();
    tab_sem[i].val = n;
    tab_sem[i].nb_attente = 0;
    tab_sem[i].used = 1;
    tab_sem[i].key = key;
    nb_sem++;
    return i; // On renvoi cet index (semid)
  }
}

/* Opération down sur un semaphore (prendre une ressource, -1), peut être bloquante */
PUBLIC void down(int semid) {
  (void)(semid);
}

/* Opération up sur un semaphore (rendre une ressource, +1) */
PUBLIC void up(int semid) {
(void)(semid);
}

/* Detruire un semaphore */
PUBLIC void destroy(int semid) {
  tab_sem[semid].used = 0;
}