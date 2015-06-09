#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cache_list.h"
#include "low_cache.h"

/*! Création d'une liste de blocs */
struct Cache_List *Cache_List_Create()
{
	struct Cache_List *cache_list;
	cache_list = malloc(sizeof(struct Cache_List));
	cache_list->next = cache_list->prev = cache_list;
	return cache_list;
}

/*! Destruction d'une liste de blocs */
void Cache_List_Delete(struct Cache_List *list)
{
	struct Cache_List *current = list->next;
	
	while (current != list)
	{
		current = current->next;
		free(current->prev);
	}
	free(current);	
}

/*! Insertion d'un élément à la fin */
void Cache_List_Append(struct Cache_List *list, struct Cache_Block_Header *pbh)
{
	struct Cache_List  *plast = list->prev;
    struct Cache_List  *pnew;

    pnew = malloc(sizeof(struct Cache_List));
    pnew->pheader = pbh;

    // On insère après plast
    pnew->prev = plast;
    plast->next = pnew;
    pnew->next = list;
    list->prev = pnew;
}

/*! Insertion d'un élément au début*/
void Cache_List_Prepend(struct Cache_List *list, struct Cache_Block_Header *pbh)
{
	struct Cache_List  *pcurr = list->next;
    struct Cache_List  *pnew;

    pnew = malloc(sizeof(struct Cache_List));
    pnew->pheader = pbh;

    // On insère avant pcurr
    pnew->prev = list;
    pcurr->prev->next = pnew;
    pcurr->prev = pnew;
    pnew->next = pcurr;
}

/*! Retrait du premier élément */
struct Cache_Block_Header *Cache_List_Remove_First(struct Cache_List *list)
{
	return Cache_List_Remove(list,list->next->pheader);	
}

/*! Retrait du dernier élément */
struct Cache_Block_Header *Cache_List_Remove_Last(struct Cache_List *list)
{
	return Cache_List_Remove(list,list->prev->pheader);
}

/*! Retrait d'un élément quelconque */
struct Cache_Block_Header *Cache_List_Remove(struct Cache_List *list, struct Cache_Block_Header *pbh)
{
	struct Cache_Block_Header *cache_block_header = NULL;
	
	for (struct Cache_List *cache_current = list->next; cache_current != list; cache_current = cache_current->next)
	{
		if(cache_current->pheader == pbh)
		{
			cache_block_header = cache_current->pheader;
			cache_current->prev->next = cache_current->next;
			cache_current->next->prev = cache_current->prev;
			free(cache_current);
		}
	}
		
	return cache_block_header;	
}

/*! Remise en l'état de liste vide */
void Cache_List_Clear(struct Cache_List *list)
{	
	for (struct Cache_List *cache_current = list->next; cache_current != list;)
	{
		cache_current = cache_current->next;
		free(cache_current->prev);
	}
	list->next = list->prev = list;		
}

/*! Test de liste vide */
bool Cache_List_Is_Empty(struct Cache_List *list)
{
	return (list->next == list && list->prev == list);	
}

/*! Transférer un élément à la fin */
void Cache_List_Move_To_End(struct Cache_List *list, struct Cache_Block_Header *pbh)
{
	struct Cache_Block_Header *cache_block_header = Cache_List_Remove(list,pbh);
	
	if(cache_block_header != NULL)
	{
		Cache_List_Append(list, cache_block_header);
	}
}

/*! Transférer un élément  au début */
void Cache_List_Move_To_Begin(struct Cache_List *list, struct Cache_Block_Header *pbh)
{
	struct Cache_Block_Header *cache_block_header = Cache_List_Remove(list,pbh);
	
	if(cache_block_header != NULL)
	{
		Cache_List_Prepend(list, cache_block_header);
	}
}

void printAllBlockInList(const struct Cache_List *cachelist)
{
	assert(cachelist != NULL);
    printf("( ");
    for (struct Cache_List *cacheCurrent = cachelist->next; cacheCurrent != cachelist; cacheCurrent = cacheCurrent->next)
        printf("%p ", cacheCurrent->pheader);
    printf(")\n");
}
