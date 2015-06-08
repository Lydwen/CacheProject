#include <assert.h>

#include "strategy.h"
#include "low_cache.h"
#include "random.h"
#include "time.h"

/*
Liste pointeur sur blocs valides
Des que bloc non valide devient valide on le met en queue
affectation a un autre bloc : on le met en queue

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
}  

void Strategy_Write(struct Cache *pcache, struct Cache_Block_Header *pbh)
{
} 

char *Strategy_Name()
{
    return "FIFO";
}

