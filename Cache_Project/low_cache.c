#include "low_cache.h"


// Recherche d'un bloc libre.
struct Cache_Block_Header *Get_Free_Block(struct Cache *pcache){
    int tmp ;
    int i ;
    for(i = 0; i<pcache->nblocks; i++){
        tmp =  (pcache->headers[i]).flags;
        if((tmp & VALID) == 0)
            return &pcache->headers[i];
    }

    return NULL;
}