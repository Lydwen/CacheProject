#include <assert.h>

#include "strategy.h"
#include "low_cache.h"
#include "random.h"
#include "time.h"


/*
liste pointeurs blocs valides (occupés)
mise en queue de liste :
	chaque fois qu'un bloc change d'affectation
	operation lecture ecriture
*/

void *Strategy_Create(struct Cache *pcache) 
{
 //initialisation liste + free a la fin




 }

/void Strategy_Close(struct Cache *pcache)
{
}

void Strategy_Invalidate(struct Cache *pcache)
{
}

struct Cache_Block_Header *Strategy_Replace_Block(struct Cache *pcache) 
{

// Get_Free_Block, sinon strat



}


void Strategy_Read(struct Cache *pcache, struct Cache_Block_Header *pbh) 
{

	//mettre bloc en queue
}  

void Strategy_Write(struct Cache *pcache, struct Cache_Block_Header *pbh)
{
	
	//mettre bloc en queue
	
} 

char *Strategy_Name()
{
    return "LRU";
}
