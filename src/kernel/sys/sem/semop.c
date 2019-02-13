#include <nanvix/const.h>
#include <nanvix/sem.h>






// La fonction semop permet d’effectuer des op´erations atomiques incr´ementant
// ou d´ecr´ementant la variable associ´ee au s´emaphore identifi´e par semid.
// Une valeur n´egative pour op sp´ecifie l’op´eration downw () et une valeur
// positive l’op´eration up ().
// La fonction retourne 0 en cas de r´eussite, ou −1 en cas d’erreur.
PUBLIC int sys_semop(int semid, int op) {
  int res;
  if(op > 0) {
    for(int i=0; i<op; i++) {
      res = up(semid);
      if(res == -1) {
        return -1;
      }
    }
    return 0;
  }else if(op < 0) {
    for(int i=0; i>op; i--) {
      res = down(semid);
      if(res == -1) {
        return -1;
      }
    }
    return 0;
  }
  return -1;
}