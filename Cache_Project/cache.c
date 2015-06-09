#include <stdlib.h>
#include "low_cache.h"
#include "strategy.h"
#include "cache.h"
#include <string.h>




//! Création du cache.
struct Cache *Cache_Create(const char *fic, unsigned nblocks, unsigned nrecords,
                           size_t recordsz, unsigned nderef) {

	return NULL;
}

//! Fermeture (destruction) du cache.
Cache_Error Cache_Close(struct Cache *pcache) {
	return CACHE_OK;
}

//! Synchronisation du cache.
Cache_Error Cache_Sync(struct Cache *pcache) {
	return CACHE_OK;
}

//! Invalidation du cache.
Cache_Error Cache_Invalidate(struct Cache *pcache) {
	unsigned int i;
	struct Cache_Block_Header *curr = pcache->headers;

	for (i = 0 ; i < pcache->nblocks ; i++){
		//On met a 0 le bit V , on ne touche pas aux autres (M et R)
		curr->flags = curr->flags & 0x6 ;
		//On passe au header suivant
		curr++;
	}

	return CACHE_OK;
}

//! Lecture  (à travers le cache).
Cache_Error Cache_Read(struct Cache *pcache, int irfile, void *precord) {
	if (precord == NULL )
		return CACHE_KO;

	
	//On calcule la position du bloc dans le fichier
	 int ibfile = irfile/pcache->blocksz;

	struct Cache_Block_Header *b = pcache->headers;
	unsigned int i;
	for (i=0; i<pcache->nblocks; i++){
		if ((b[i].flags % 2) == 1 && b[i].ibfile == ibfile) { //on regarde si le bit V est a 1 et si les ibfiles correspondent
			//On a trouve un bloc valide contenant le bon ibfile, onc on le copie dans le buffer pointe par precord
			int a = ADDR(pcache,irfile,&b[i]);
			memcpy(precord, (void*)b[i].data+a,pcache->recordsz);
			//maj des informations 
			pcache->instrument.n_reads++;
			pcache->instrument.n_hits++;
			Strategy_Read(pcache,&b[i]);
			return CACHE_OK;
		}
	}

	//On n'a pas trouve de bloc qui contienne l'enregistrement voulu, on cherche donc un bloc libre
	struct Cache_Block_Header *block = (struct Cache_Block_Header *) Strategy_Replace_Block(pcache);
	
	block->flags = VALID;
	block->ibfile = ibfile;
	//copie des donnees d'un bloc entier depuis le fichier vers le cache
	memcpy(block->data, DADRR(pcache,ibfile),pcache->blocksz);
	//copie de l'enregistrement depuis le cache vers le buffer
	int a = ADDR(pcache,irfile,block);
	memcpy(precord,(void*)block->data+a,pcache->recordsz);
	pcache->instrument.n_reads++;
	Strategy_Read(pcache,block);
	return CACHE_OK;
	

	
}

//! Écriture (à travers le cache).
Cache_Error Cache_Write(struct Cache *pcache, int irfile, const void *precord) {
	if (precord == NULL )
		return CACHE_KO;

	
	//On calcule la position du bloc dans le fichier
	 int ibfile = irfile/pcache->blocksz;

	struct Cache_Block_Header *b = pcache->headers;
	unsigned int i;
	for (i=0; i<pcache->nblocks; i++){
		if ((b[i].flags % 2) == 1 && b[i].ibfile == ibfile) { //on regarde si le bit V est a 1 et si les ibfiles correspondent
			//On a trouve un bloc valide contenant le bon ibfile, onc on va copier precord dans le bloc
			int a = ADDR(pcache,irfile,&b[i]);
			memcpy((void*)b[i].data+a,precord,pcache->recordsz);
			//maj des informations 
			b[i].flags |= MODIF;
			pcache->instrument.n_writes++;
			pcache->instrument.n_hits++;
			Strategy_Write(pcache,&b[i]);
			return CACHE_OK;
		}
	}

	//On n'a pas trouve de bloc qui contienne l'enregistrement voulu, on cherche donc un bloc libre
	struct Cache_Block_Header *block = (struct Cache_Block_Header *) Strategy_Replace_Block(pcache);
	
	block->flags = VALID | MODIF;
	block->ibfile = ibfile;
	//copie des donnees d'un bloc entier depuis le fichier vers le cache
	memcpy(block->data, DADRR(pcache,ibfile),pcache->blocksz);
	//copie de l'enregistrement depuis le cache vers le buffer
	int a = ADDR(pcache,irfile,block);
	memcpy((void*)block->data+a,precord,pcache->recordsz);
	pcache->instrument.n_writes++;
	Strategy_Write(pcache,block);
	return CACHE_OK;
	

	


//! Résultat de l'instrumentation.
struct Cache_Instrument *Cache_Get_Instrument(struct Cache *pcache) {
	return NULL;
}

