#include <nanvix/const.h>
#include <nanvix/sem.h>
#include <sys/sem.h>
#include <errno.h>




// La fonction semctl () permet une s´erie d’op´erations de contrˆole sur le
// s´emaphore identifi´e par semid. Ces op´erations sont sp´ecifi´ees par cmd
// et peuvent ˆetre :
// — GETVAL, renvoie la valeur actuelle du s´emaphore ;
// — SETVAL, d´efinit la valeur du s´emaphore sur val ;
// — IPC RMID, supprime le s´emaphore et le d´etruit s’il n’est plus utilis´e.
// La valeur de retour de la fonction d´epend de cmd. Si c’est GETVAL, la
// valeur actuelle du s´emaphore est retourn´ee. Dans tous les autres cas
// d’ach`evement r´eussi, 0 doit ˆetre retourn´e. En cas d’erreur, −1 doit ˆetre
// retourné.
PUBLIC int sys_semctl(int semid, int cmd, int val) {
  //comment tester val ?
  if(cmd == GETVAL) {
    return getVal(semid);
  }else if(cmd == SETVAL) {
    return setVal(semid, val);
  }else if(cmd == IPC_RMID) {
    return destroy(semid);
  }
  return -EINVAL;
}