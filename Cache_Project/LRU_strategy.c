#include <assert.h>

#include "strategy.h"
#include "low_cache.h"
#include "cache_list.h"



/*
	Initialisation de liste des blocs pour la strategie LRU 
*/
void *Strategy_Create(struct Cache *pcache) 
{
	return Cache_List_Create();
}


void Strategy_Close(struct Cache *pcache)
{
	Cache_List_Delete(pcache->pstrategy);

}

/*
	on vide la liste de la strategie lru
	
*/
void Strategy_Invalidate(struct Cache *pcache)
{
	Cache_List_Clear(pcache->pstrategy);
}


/*
	Strategie LRU : si pas de bloc disponible, on renvoie le bloc LRU qui est le bloc en tete de liste

*/
struct Cache_Block_Header *Strategy_Replace_Block(struct Cache *pcache) 
{
	
    struct Cache_Block_Header *pbh;

    //On cherche d'abord un bloc invalide 
    if ((pbh= Get_Free_Block(pcache)) != NULL) {
    	//Bloc invalide trouve, on le renvoie et on le met en fin de liste puisqu'il vient d'etre utilise
    	Cache_List_Append(pcache->pstrategy,pbh);
    	return pbh;
    }
    //Pas de bloc invalide trouve, on remplace celui en tete de file car c'est le LRU 
    pbh = Cache_List_Remove_First(pcache->pstrategy);
    //Ce block vient d'etre utilise, on le met en fin de liste
    Cache_List_Append(pcache->pstrategy,pbh);

    return pbh;

}

/*
	On lit un bloc donc on le met en fin de liste puisqu'il vient d etre utilise
*/
void Strategy_Read(struct Cache *pcache, struct Cache_Block_Header *pbh) 
{	
	Cache_List_Move_To_End(pcache->pstrategy,pbh);
}  


/*
	On ecrit dans un bloc donc on le met en fin de liste puisqu'il vient d etre utilise
*/
void Strategy_Write(struct Cache *pcache, struct Cache_Block_Header *pbh)
{	
	Cache_List_Move_To_End(pcache->pstrategy,pbh);
} 

char *Strategy_Name()
{
    return "LRU";
}
