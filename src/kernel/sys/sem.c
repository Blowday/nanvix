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
PUBLIC int down(int semid) {
  if(tab_sem[semid].used == 1) {
    disable_interrupts();
    tab_sem[semid].val--;
    while(tab_sem[semid].val < 0) {
      //tab_sem[semid].wait_list[tab_sem[semid].nb_attente] = curr_proc;
      tab_sem[semid].nb_attente++;
      enable_interrupts();
      sleep(tab_sem[semid].chain, curr_proc->priority);
      disable_interrupts();
      tab_sem[semid].nb_attente--;
    }
    enable_interrupts();
    return 0;
  }
  return -1;
}

/* Opération up sur un semaphore (rendre une ressource, +1) */
PUBLIC int up(int semid) {
  if(tab_sem[semid].used == 1) {
    disable_interrupts();
    tab_sem[semid].val++;
    if(tab_sem[semid].nb_attente>0 && tab_sem[semid].val>0) {
      //réveiller des process en attente
      wakeup(tab_sem[semid].chain);
    }
    enable_interrupts();
    return 0;
  }
  return -1;
}

/* Detruire un semaphore */
PUBLIC int destroy(int semid) {
  if(tab_sem[semid].used == 1) {
    tab_sem[semid].used = 0;
    return 0;
  }
  return -1;
  
}
/* Renvoie le semid associé a une key, le crée si non existant, -1 si erreur */
PUBLIC int getID(unsigned int key) {
  for(int i=0; i<PROC_MAX; i++) {
    if(tab_sem[i].key == key) {
      return i;
    }
    return create(0,key);
  }
  return -1;
}

PUBLIC int getVal(int semid) {
  if(tab_sem[semid].used == 1) {
    return tab_sem[semid].val;
  }
  return -1;
}

PUBLIC int setVal(int semid, int val) {
  if(tab_sem[semid].used == 1) {
    tab_sem[semid].val = val;
    return 0;
  }
  return -1;
}