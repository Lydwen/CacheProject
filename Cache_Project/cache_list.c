#include <stdio.h>
#include <stdlib.h>
#include "cache_list.h"

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
	free(list);
}

/*! Insertion d'un élément à la fin */
void Cache_List_Append(struct Cache_List *list, struct Cache_Block_Header *pbh)
{
	
}

/*! Insertion d'un élément au début*/
void Cache_List_Prepend(struct Cache_List *list, struct Cache_Block_Header *pbh)
{
	
}

/*! Retrait du premier élément */
struct Cache_Block_Header *Cache_List_Remove_First(struct Cache_List *list)
{
	
}

/*! Retrait du dernier élément */
struct Cache_Block_Header *Cache_List_Remove_Last(struct Cache_List *list)
{
	
}

/*! Retrait d'un élément quelconque */
struct Cache_Block_Header *Cache_List_Remove(struct Cache_List *list, struct Cache_Block_Header *pbh)
{
	
}

/*! Remise en l'état de liste vide */
void Cache_List_Clear(struct Cache_List *list)
{
	
}

/*! Test de liste vide */
bool Cache_List_Is_Empty(struct Cache_List *list)
{
	return (list->next == list && list->prev == list);	
}

/*! Transférer un élément à la fin */
void Cache_List_Move_To_End(struct Cache_List *list, struct Cache_Block_Header *pbh)
{
	
}

/*! Transférer un élément  au début */
void Cache_List_Move_To_Begin(struct Cache_List *list, struct Cache_Block_Header *pbh)
{
	
}

int main()
{
	printf("Testing cache_list\n");
	printf("Let's create a new empty cache list\n");
	struct Cache_List *cachelist = Cache_List_Create();
	printf("cachelist address : %p Prev : %p Next : %p \n", cachelist, cachelist->prev, cachelist->next);
	printf("cachelist empty ? %s \n", Cache_List_Is_Empty(cachelist) ? "true" : "false");
	
}
