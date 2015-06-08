#include <assert.h>

#include "strategy.h"
#include "low_cache.h"
#include "random.h"
#include "time.h"


/*
liste pointeurs blocs libres
mise en queue de liste :
	chaque fois qu'un bloc change d'affectation
	operation lecture ecriture
*/

void *Strategy_Create(struct Cache *pcache) 
{
 
 }

/void Strategy_Close(struct Cache *pcache)
{
}

void Strategy_Invalidate(struct Cache *pcache)
{
}

struct Cache_Block_Header *Strategy_Replace_Block(struct Cache *pcache) 
{
}


void Strategy_Read(struct Cache *pcache, struct Cache_Block_Header *pbh) 
{
}  

void Strategy_Write(struct Cache *pcache, struct Cache_Block_Header *pbh)
{
} 

char *Strategy_Name()
{
    return "LRU";
}
