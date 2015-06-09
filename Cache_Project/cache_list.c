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
	struct Cache_List  *pcurr;
    struct Cache_List  *pnew;

    pnew = malloc(sizeof(struct Cache_List));
    pnew->pheader = pbh;

    for (pcurr = list->next; pcurr != list ;  pcurr = pcurr->next)
    { }

    // On insère après pcurr
    pnew->prev = pcurr->prev;
    pcurr->prev->next = pnew;
    pcurr->prev = pnew;
    pnew->next = pcurr;
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
/*
int main()
{
	printf("Testing cache_list\n");
	printf("Let's create a new empty cache list\n");
	struct Cache_List *cachelist = Cache_List_Create();
	printf("cachelist address : %p Prev : %p Next : %p \n", cachelist, cachelist->prev, cachelist->next);
	printf("cachelist empty ? %s \n", Cache_List_Is_Empty(cachelist) ? "true" : "false");
	printAllBlockInList(cachelist);
	
	struct Cache_Block_Header *pbh1 = malloc(sizeof(struct Cache_Block_Header));
	struct Cache_Block_Header *pbh2 = malloc(sizeof(struct Cache_Block_Header));
	struct Cache_Block_Header *pbh3 = malloc(sizeof(struct Cache_Block_Header));
	
	printf("===Append %p===\n", pbh1);
	Cache_List_Append(cachelist,pbh1);
	printf("cachelist empty ? %s \n", Cache_List_Is_Empty(cachelist) ? "true" : "false");
	printAllBlockInList(cachelist);
	
	printf("===Append %p===\n", pbh2);
	Cache_List_Append(cachelist,pbh2);
	printf("cachelist empty ? %s \n", Cache_List_Is_Empty(cachelist) ? "true" : "false");
	printAllBlockInList(cachelist);
	
	printf("===Prepend %p===\n", pbh3);
	Cache_List_Prepend(cachelist,pbh3);
	printf("cachelist empty ? %s \n", Cache_List_Is_Empty(cachelist) ? "true" : "false");
	printAllBlockInList(cachelist);
	
	printf("===Take out first header===\n");
	printf("First header : %p\n", Cache_List_Remove_First(cachelist));
	printAllBlockInList(cachelist);
	
	printf("===Take out last header===\n");
	printf("Last header : %p\n", Cache_List_Remove_Last(cachelist));
	printAllBlockInList(cachelist);
	
	printf("===Clear cachelist===\n");
	Cache_List_Clear(cachelist);
	printf("cachelist empty ? %s \n", Cache_List_Is_Empty(cachelist) ? "true" : "false");
	printAllBlockInList(cachelist);
	
	printf("===Add some elements again===\n");
	Cache_List_Append(cachelist,pbh1);
	Cache_List_Append(cachelist,pbh2);
	Cache_List_Append(cachelist,pbh3);
	printAllBlockInList(cachelist);
	printf("===Clear cachelist===\n");
	Cache_List_Clear(cachelist);
	printf("cachelist empty ? %s \n", Cache_List_Is_Empty(cachelist) ? "true" : "false");
	printAllBlockInList(cachelist);
	
	printf("===Add some elements again===\n");
	Cache_List_Append(cachelist,pbh1);
	Cache_List_Append(cachelist,pbh2);
	Cache_List_Append(cachelist,pbh3);
	printAllBlockInList(cachelist);
	
	printf("===Putting second element to last===\n");
	Cache_List_Move_To_End(cachelist, pbh2);
	printAllBlockInList(cachelist);
	
	printf("===Putting last element to first===\n");
	Cache_List_Move_To_Begin(cachelist, pbh2);
	printAllBlockInList(cachelist);
	
	printf("===Finished test. Clearing memory..===\n");
	Cache_List_Delete(cachelist);
	free(pbh1);free(pbh2);free(pbh3);
}*/
