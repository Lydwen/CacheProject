/*!
 * \file RAND_strategy.c
 *
 * \brief  Stratégie de remplacement au hasard..
 * 
 * \author Jean-Paul Rigault 
 *
 * $Id: RAND_strategy.c,v 1.3 2008/03/04 16:52:49 jpr Exp $
 */

#include <assert.h>

#include "strategy.h"
#include "low_cache.h"
#include "random.h"
#include "time.h"


struct Data
    {
       	unsigned nAcces;    //le nombre d'acces au cache, au bout duquel le R sera remis a 0
        
    };

static void mise_a_zero(struct Cache *pcache)
{
	struct Data *d = (struct Data *)(pcache->pstrategy);
	int i;
	if(pcache->nderef == 0 || --d->nAcces > 0)
		return;

	for(i = 0; i < pcache->nblocks; i++)
	{
		
		pcache->headers[i].flags &= 0x4;
		
	}

	d->nAcces = pcache->nderef;
	pcache->instrument.n_deref ++;


}

static int calc (int r, int m){
	if(r >= 1)
		r = 1;
	else
		r = 0;
	if(m >= 1)
		m = 1;
	else
		m = 0;
	return r << 1 | m;
	
}
/*!
 * Initialisation de la strategie NUR : . 
 *
 * On alloue la memoir necessaire pour contenire la structure avec le 
 * bit de referencage ainsi que le nombre maximun d'access avant la remise a zero
 * 
*/
void *Strategy_Create(struct Cache *pcache) 
{	
	//printf("je suis dans Create\n");
    struct Data *d = malloc(sizeof(struct Data));  // Creation de la structur de donnée
    d->nAcces = pcache->nderef;   // Initialisation du nombre d'acces au cache à 0

    return d;//affectation de la struct dans la struct du cache
    
}

/*!
 * NUR : Liberation de l'espace allloué pour la stategie.
 */
void Strategy_Close(struct Cache *pcache)
{
    //printf("je suis dans Close\n");
    
    free(pcache->pstrategy); //liberation de la memoir alloué pour la struct de donnée

}


/*!
 * RAND : Rien à faire ici.
 */
void Strategy_Invalidate(struct Cache *pcache)
{
    //printf("je suis dans Invalidate\n");
    
    struct Data *d = (struct Data *)(pcache->pstrategy);

    if (pcache->nderef != 0) 
    {
        d->nAcces = 1;
        mise_a_zero(pcache);
    }   
}

/*! 
 * RAND : On prend le premier bloc invalide. S'il n'y en a plus, on prend un bloc au hasard.
 */
struct Cache_Block_Header *Strategy_Replace_Block(struct Cache *pcache) 
{
    //printf("je suis dans Replace\n");
    
    int ib;
    int min = 4;
    int tmp;
    struct Cache_Block_Header *pbh;
    struct Cache_Block_Header *pbh_tmp = NULL;

    /* On cherche d'abord un bloc invalide */
    if ((pbh = Get_Free_Block(pcache)) != NULL) return pbh;

    for (ib = 0 ; ib < pcache->nblocks; ib++)
    {
		pbh = &pcache->headers[ib];
		
		//printf("je suis dans Replace, %d\n", pbh->flags & REFER);
    
		tmp = calc(pbh->flags & REFER, pbh->flags & MODIF);
		if (tmp == 0) return pbh; 
		else if (tmp < min) 
		{
		    min = tmp;
		    pbh_tmp = pbh;
		}	
    }
    return pbh_tmp;    
    
}


/*!
 * RAND : Rien à faire ici.
 */
void Strategy_Read(struct Cache *pcache, struct Cache_Block_Header *pbh) 
{	
	//printf("je suis dans Read\n");
    
	mise_a_zero(pcache);
	pbh->flags |= 0x4;
}  

/*!
 * RAND : Rien à faire ici.
 */  
void Strategy_Write(struct Cache *pcache, struct Cache_Block_Header *pbh)
{
	//printf("je suis dans Write\n");
    
	mise_a_zero(pcache);
	pbh->flags |= 0x4;
} 

char *Strategy_Name()
{
    return "NUR";
}

