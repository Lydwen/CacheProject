#include <assert.h>

#include "strategy.h"
#include "low_cache.h"
#include "cache_list.h"

/*
Liste pointeur sur blocs valides
Des que bloc non valide devient valide on le met en queue
affectation a un autre bloc : on le met en queue
*/

/*
 * On doit créer la liste qui servira de queue pour le FIFO
 */
void *Strategy_Create(struct Cache *pcache) 
{
	return Cache_List_Create();	
}

/* On détruit la liste pour libérer de la mémoire*/
void Strategy_Close(struct Cache *pcache)
{
	Cache_List_Delete((struct Cache_List *) pcache->pstrategy);
}

/* On vide la liste de tous les éléments qui la compose*/
void Strategy_Invalidate(struct Cache *pcache)
{
	Cache_List_Clear((struct Cache_List *)pcache->pstrategy);    
}

/*
 * La stratégie du FIFO : 
 * 		chercher un bloc libre. 
 *		si pas de bloc libre : enlever le bloc le plus ancien
 *  en terme de création et renvoyer l'adresse de ce bloc nouvellement vide 
 */
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

/* Ne fait rien pour un FIFO*/
void Strategy_Read(struct Cache *pcache, struct Cache_Block_Header *pbh) 
{
}  

/* Ne fait rien pour un FIFO*/
void Strategy_Write(struct Cache *pcache, struct Cache_Block_Header *pbh)
{
} 

/* Renvoie "FIFO" pour décrire la stratégie*/
char *Strategy_Name()
{
    return "FIFO";
}

