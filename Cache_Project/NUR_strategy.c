#include <assert.h>

#include "strategy.h"
#include "low_cache.h"
#include "random.h"
#include "time.h"

/*
	Remplacer le bloc qui a le plus petit 2*R+M 
	R a 1 des que bloc référencé (lecture/ecriture)
	remis à 0 regulièrement (nderef UT)


*/

void *Strategy_Create(struct Cache *pcache) 
{
 


 }

/void Strategy_Close(struct Cache *pcache)
{
}

void Strategy_Invalidate(struct Cache *pcache)
{

	//mettre les R a 0
}

struct Cache_Block_Header *Strategy_Replace_Block(struct Cache *pcache) 
{

// Get_Free_Block, sinon strat



}


void Strategy_Read(struct Cache *pcache, struct Cache_Block_Header *pbh) 
{

	//mettre R a 1
}  

void Strategy_Write(struct Cache *pcache, struct Cache_Block_Header *pbh)
{

	//mettre M a 1
} 

char *Strategy_Name()
{
    return "NUR";
}
