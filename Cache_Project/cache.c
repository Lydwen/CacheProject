#include <stdlib.h>
#include "cache.h"
#include "low_cache.h"
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
	cache->nderef = NSYNC;
	
	struct Cache_Instrument *instr = malloc(sizeof(struct Cache_Instrument));
	instr->n_reads = 0;
	instr->n_writes = 0;
	instr->n_hits = 0;
	instr->n_syncs = 0;
	instr->n_writes = 0;
	instr->n_deref = 0;
	cache->instrument = *instr;
	
    struct Cache_Block_Header *headers = malloc(nblocks * sizeof(struct Cache_Block_Header));
    cache->headers = headers;
    
    cache->pfree= &headers[0];
    cache->pstrategy = Strategy_Create(cache);
	return cache;
}

//! Fermeture (destruction) du cache.
Cache_Error Cache_Close(struct Cache *pcache) {
	return CACHE_KO;
}

//! Synchronisation du cache.
Cache_Error Cache_Sync(struct Cache *pcache) {
	return CACHE_KO;
}

//! Invalidation du cache.
Cache_Error Cache_Invalidate(struct Cache *pcache) {
	return CACHE_KO;
}

//! Lecture  (à travers le cache).
Cache_Error Cache_Read(struct Cache *pcache, int irfile, void *precord) {
	return CACHE_KO;
}

//! Écriture (à travers le cache).
Cache_Error Cache_Write(struct Cache *pcache, int irfile, const void *precord) {
	return CACHE_KO;
}


//! Résultat de l'instrumentation.
struct Cache_Instrument *Cache_Get_Instrument(struct Cache *pcache) {
	return &(pcache->instrument);
}

