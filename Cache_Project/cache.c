#include <stdlib.h>
#include "low_cache.h"
#include "cache.h"
#include "strategy.h"
#include <string.h>

//! Création du cache.
struct Cache *Cache_Create(const char *fic, unsigned nblocks, unsigned nrecords,
                           size_t recordsz, unsigned nderef) {


	struct Cache *cache = malloc(sizeof(struct Cache));
	cache->file = malloc(strlen(fic) * sizeof(char));
	strcpy(cache->file, fic);
	cache->fp = fopen(fic, "rw+");
	cache->nblocks = nblocks;
	cache->nrecords = nrecords;
	cache->recordsz = recordsz;
	cache->blocksz = nrecords * recordsz;
	cache->nderef = nderef;
	
	struct Cache_Instrument *instr = malloc(sizeof(struct Cache_Instrument));
	instr->n_reads = 0;
	instr->n_writes = 0;
	instr->n_hits = 0;
	instr->n_syncs = 0;
	instr->n_writes = 0;
	instr->n_deref = 0;
	cache->instrument = *instr;
	
    struct Cache_Block_Header *headers = malloc(nblocks * sizeof(struct Cache_Block_Header));
	int block;
	for(block = 0; block < nblocks;block++){
		cache->headers[block].data = malloc(recordsz * nrecords);
		cache->headers[block].ibcache = block;
		cache->headers[block].flags &= 0x0;
	}
    cache->headers = headers;
    
    cache->pfree= &headers[0];
    cache->pstrategy = Strategy_Create(cache);
	
	return cache;
}

//! Fermeture (destruction) du cache.
Cache_Error Cache_Close(struct Cache *pcache) {
    Strategy_Close(pcache);
    
    int block;
    for(block = 0; block < pcache->nblocks; block++){
		free(pcache->headers[block].data);
	}
	
	free(pcache->headers);
	free(&(pcache->instrument));
	if(fclose(pcache->fp) != 0) return CACHE_KO;
	free(pcache->file);
	free(pcache);
	return CACHE_OK;
}

//! Synchronisation du cache.
Cache_Error Cache_Sync(struct Cache *pcache) {
	unsigned int i;
	struct Cache_Block_Header curr = pcache->headers[0];

	for (i = 0 ; i < pcache->nblocks ; i++){
		//si le bit M est à 1
		if((curr.flags & MODIF) > 0){
			//on met le bit M à 0 sans changer les autres
			curr.flags &= ~MODIF;
			//puis on écrit sur le disque
			int daddr = DADDR(pcache, curr.ibfile);
			if(fseek(pcache->fp, daddr, SEEK_SET)!=0) return CACHE_KO;
			fwrite(curr.data, pcache->recordsz, pcache->nrecords, pcache->fp);
		}
	}
	
	return CACHE_OK;
}

//! Invalidation du cache.
Cache_Error Cache_Invalidate(struct Cache *pcache) {
	
	unsigned int i;
	

	for (i = 0 ; i < pcache->nblocks ; i++)
		//On met a 0 le bit V , on ne touche pas aux autres (M et R)
		pcache->headers[i].flags &= 0x6 ;
	//Le premier bloc libre est desormais le premier bloc du cache
	pcache->pfree = &(pcache->headers[0]);
	Strategy_Invalidate(pcache);

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
		if (b[i].ibfile == ibfile) { //On a trouve un bloc contenant le bon ibfile,
			if ((b[i].flags % 2) == 1 ) { //on regarde si le bit V est a 1 
				// onc on le copie dans le buffer pointe par precord
				void *a = ADDR(pcache,irfile,&b[i]);
				memcpy(precord, a,pcache->recordsz);
				//maj des informations 
				pcache->instrument.n_reads++;
				pcache->instrument.n_hits++;
				Strategy_Read(pcache,&b[i]);
				return CACHE_OK;
			}
			//Si bloc non valide alors on sort puisqu'il ne peut y en avoir qu'un avec le bon ibfile
			break;
		}
	}

	//On n'a pas trouve de bloc qui contienne l'enregistrement voulu, on cherche donc un bloc libre
	struct Cache_Block_Header *block = (struct Cache_Block_Header *) Strategy_Replace_Block(pcache);
	
	block->flags = VALID;
	block->ibfile = ibfile;
	
	int da = DADDR(pcache,ibfile); ///calcul de l'adresse du bloc dans le fichier
	memcpy(block->data, pcache->fp+da,pcache->blocksz);  //copie des donnees d'un bloc entier depuis le fichier vers le cache
	
	void *a = ADDR(pcache,irfile,block); //calcul de l'adresse de l'enregistrement dans le cache
	memcpy(precord,a,pcache->recordsz);  //copie de l'enregistrement depuis le cache vers le buffer
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
		if (b[i].ibfile == ibfile) { //On a trouve un bloc contenant le bon ibfile,
			if ((b[i].flags % 2) == 1 ) { //on regarde si le bit V est a 1 
				// onc on copie les donnees de precord dans le bloc
				void *a = ADDR(pcache,irfile,&b[i]);
				memcpy(a,precord,pcache->recordsz);
				//maj des informations 
				b[i].flags |= MODIF;
				pcache->instrument.n_writes++;
				pcache->instrument.n_hits++;
				Strategy_Write(pcache,&b[i]);
				return CACHE_OK;
			}
			break;
		}
	}

	//On n'a pas trouve de bloc qui contienne l'enregistrement voulu, on cherche donc un bloc libre
	struct Cache_Block_Header *block = (struct Cache_Block_Header *) Strategy_Replace_Block(pcache);
	
	block->flags = VALID | MODIF;
	block->ibfile = ibfile;
	
	int da = DADDR(pcache,ibfile);
	memcpy(block->data,pcache->fp+da ,pcache->blocksz); //copie des donnees d'un bloc entier depuis le fichier vers le cache
	
	void *a = ADDR(pcache,irfile,block);
	memcpy(a,precord,pcache->recordsz);  //copie de l'enregistrement depuis le cache vers le buffer
	pcache->instrument.n_writes++;
	Strategy_Write(pcache,block);
	return CACHE_OK;
}


//! Résultat de l'instrumentation.
struct Cache_Instrument *Cache_Get_Instrument(struct Cache *pcache) {
	struct Cache_Instrument *ins = malloc(sizeof(struct Cache_Instrument));
	//copie des donnees 
	*ins = pcache->instrument;

	//Remise a zero de tous les compteurs
	pcache->instrument.n_reads = 0;
	pcache->instrument.n_writes = 0;
	pcache->instrument.n_hits = 0;
	pcache->instrument.n_syncs = 0;
	pcache->instrument.n_deref = 0;
	

	return ins;
}

