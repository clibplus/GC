#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "gc.h"

GC StartGC(int d) {
	GC g = {
		.Bins 		= (Bin **)malloc(sizeof(Bin *) * 1),
		.Debug 		= d,
		.idx		= 0,

		.CreateBin	= _CreateBin,
		.AddToBin	= _AddToBin,
		.DestroyBin	= _DestroyBin
	};

	memset(g.Bins, '\0', sizeof(Bin *) * 1);

	return g;
}

int _CreateBin(GC *g) {
	Bin *b = (Bin *)malloc(sizeof(Bin));
	*b = (Bin){
		.BinID 		= (rand() & (1000000 - 0 + 1) + 0),
		.Objects 	= (Object **)malloc(sizeof(Object *) * 1),
		.idx 		= 0
	};

	memset(b->Objects, '\0', sizeof(Object *) * 1);
	g->Bins[g->idx] = b;
	g->idx++;
	g->Bins = (Bin **)realloc(g->Bins, sizeof(Bin *) * (g->idx + 1));

	return b->BinID;
}

static int _FindBin(GC *g, int bin_id) {
	for(int i = 0; i < g->idx; i++) {
		if(!g->Bins[i])
			break;

		if(g->Bins[i]->BinID == bin_id)
			return i;
	}

	return -1;
}

int _AddToBin(GC *g, int bin_id, Obj_T t, void *obj) {
	if(!g)
		return -1;

	if((int)t <= 0)
		return -1;

	if(!obj)
		return -1;

	int check = _FindBin(g, bin_id);
	if(check == -1)
		return -1;

	Object *o = (Object *)malloc(sizeof(Object));
	*o = (Object){
		.Obj = obj,
		.Type = t
	};

//	replugging my mic
	g->Bins[check]->Objects[g->Bins[check]->idx] = o;
	g->Bins[check]->idx++;
	g->Bins[check]->Objects = (Object **)realloc(g->Bins[check]->Objects, sizeof(Object *) * (g->Bins[check]->idx + 1));
	return 1;
}

static int _DestroyBin(GC *g, int bin_id) {
	if(!g)
		return -1;

	int check = _FindBin(g, bin_id);
	if(check == -1)
		return -1;

	for(int i = 0; i < g->Bins[check]->idx; i++) {
		if(!g->Bins[check]->Objects[i])
			break;

		free(g->Bins[check]->Objects[i]->Obj);
	}

	(void)(check == g->idx - 1 ? /* restruct arr */ NULL : free(g->Bins[check]));
	return 1;
}
