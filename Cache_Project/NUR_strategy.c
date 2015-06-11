#include "strategy.h"
#include "low_cache.h"


static void refresh_R(struct Cache *pcache);
static int calc(int r, int m);



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
    int *nAcces = malloc(sizeof(int));  // Creation de la structur de donnée
    
    nAcces[0] = pcache->nderef;   // Initialisation du nombre d'acces au cache à 0

    return nAcces;//affectation de la struct dans la struct du cache
    
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
    
    refresh_R(pcache);
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
    struct Cache_Block_Header *block;
    struct Cache_Block_Header *block_tmp = NULL;

    /* On cherche d'abord un bloc invalide */
    if ((block = Get_Free_Block(pcache)) != NULL) 
    	return block;

    for (ib = 0 ; ib < pcache->nblocks; ib++)
    {
		block = &pcache->headers[ib];
		
		//printf("je suis dans Replace, %d\n", pbh->flags & REFER);
    
		tmp = calc(block->flags & REFER, block->flags & MODIF);
		if (tmp == 0) return block; 
		else if (tmp < min) 
		{
		    min = tmp;
		    block_tmp = block;
		}	
    }
    return block_tmp;    
    
}


/*!
 * RAND : Rien à faire ici.
 */
void Strategy_Read(struct Cache *pcache, struct Cache_Block_Header *pbh) 
{	
	int *nAcces  = (int *)(pcache)->pstrategy;

	// Si le compteur de déréférencements est égal à nderef, on réinitialise
	if((--nAcces[0]) == 0)
		refresh_R(pcache);
	pbh->flags |= 0x4;
}  

/*!
 * RAND : Rien à faire ici.
 */  
void Strategy_Write(struct Cache *pcache, struct Cache_Block_Header *pbh)
{
	int *nAcces  = (int*)(pcache)->pstrategy;

	// Si le compteur de déréférencements est égal à nderef, on réinitialise
	if((--nAcces[0]) == 0)
		refresh_R(pcache);
	pbh->flags |= 0x4;
} 

char *Strategy_Name()
{
    return "NUR";
}


// methode static
static void refresh_R(struct Cache *pcache)
{
	int *nAcces  = (int *)(pcache)->pstrategy;
	int i;

	if(pcache->nderef != 0){
		// On parcourt tous les blocs du cache pour remettre REFER à 0.
		for (i = 0 ; i < pcache->nblocks ; i++)
			pcache->headers[i].flags &= ~0x4;
		// Le compteur est réinitialisé
		nAcces[0] = pcache->nderef;
		// On augmente le nombre de déréférencements effectués
	    	pcache->instrument.n_deref++;
	}


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