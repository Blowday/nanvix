#include <nanvix/const.h>



// La fonction semget permet à un processus d’utiliser un sémaphore associé
// à une clé ( texttt key). Si aucun s´emaphore n’est associé à key,
// un nouveau sémaphore doit ˆetre créé.
// En cas de réussite, la fonction retournera l’identificateur du s´emaphore
// associé à la clée. En cas d’erreur, −1 doit etre retourné.
PUBLIC int sys_semget(unsigned key) {
  
  return(0);
}