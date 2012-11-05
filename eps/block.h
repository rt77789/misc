
#ifndef EPS_BLOCK_XIAOE
#define EPS_BLCCK_XIAOE

#include <cstdio>
#include <cstring>
#include "consts.h"

/**
** This is the external index unit/block structure.
** The index is consist of such blocks one by one.
** @author rt77789/xiaoe.
*/



struct Block {
	
	double dims[DIM_NUM];
	int child[CH_NUM];
	
	Block() {
		memset(child, 0xff, sizeof(size_t) * CH_NUM);
	}
};

#endif
