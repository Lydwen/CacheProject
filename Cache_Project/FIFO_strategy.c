#include <assert.h>

#include "strategy.h"
#include "low_cache.h"
#include "time.h"
#include "cache_list.h"

/*
Liste pointeur sur blocs valides
Des que bloc non valide devient valide on le met en queue
affectation a un autre bloc : on le met en queue

*/


void *Strategy_Create(struct Cache *pcache) 
{
	//initialisation liste
	struct Cache_List *cache_list = Cache_List_Create();
	pcache->pstrategy = cache_list;

	return NULL;	
}

void Strategy_Close(struct Cache *pcache)
{
	Cache_List_Delete(pcache->pstrategy);
}

void Strategy_Invalidate(struct Cache *pcache)
{

    
}

struct Cache_Block_Header *Strategy_Replace_Block(struct Cache *pcache) 
{
	struct Cache_Block_Header *pbh;
    /* On cherche d'abord un bloc invalide */
    if ((pbh = Get_Free_Block(pcache)) != NULL)
    {
		/*On le rajoute à la fin de la queue*/
		Cache_List_Append(((struct Cache_List *)(pcache->pstrategy)),pbh);	
		return pbh;
	}
	/*on enlève le premier bloc et on récupère son adresse*/
	pbh = Cache_List_Remove_First(((struct Cache_List *)(pcache->pstrategy)));
	/*rajoute le bloc à la fin de la queue*/
	Cache_List_Append(((struct Cache_List *)(pcache->pstrategy)),pbh);
	
	return pbh;
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

