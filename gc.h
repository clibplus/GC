#pragma once

typedef enum Obj_T {
	GARBAGE_PTR		= 0x4000001,
	GARBAGE_DPTR 	= 0x4000002,
} Obj_T;

typedef struct Object {
	void 		*Obj;
	Obj_T 		Type;
} Object;

typedef struct Bin {
	int 		BinID;
	Object		**Objects;
	long		idx;
} Bin;

typedef struct GC {
	Bin			**Bins;
	long		idx;

	int 		Debug;

	int			(*CreateBin)		(struct GC *g);
	int			(*AddToBin)			(struct GC *g, int bid, Obj_T type, void *obj);
	int 		(*AddToBinArgs)		(struct GC *g, int bid, Obj_T type, void **obj);
	int 		(*DestroyBin)		(struct GC *g, int bid);
	void 		(*DestroyGC)		(struct GC *g);
} GC;

GC StartGC(int d);
static int _FindBin(GC *g, int bin_id);
int _CreateBin(GC *g);
int _AddToBin(GC *g, int bin_id, Obj_T t, void *obj);
static int _DestroyBin(GC *g, int bin_id);
