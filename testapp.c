#include "api_lmm.h"
#include <stdio.h>

typedef struct gundam {
	char name[20];
	uint32_t attack;
	uint32_t defense;
	uint32_t mobility;
	struct gundam * next;
}gundam;

typedef struct ship {
	char name[20];
	uint32_t capacity;
	uint32_t attack;
	uint32_t defense;
}ship;




int main(int argc, char **argv){


	mm_init();
	MM_REG_STRUCT(gundam);
	MM_REG_STRUCT(ship);

	print_RegisteredPageFams();

	struct pageFamily * tempVar = pageFamilyLookup("ship");
	
	printf("Pointer to pageFamily lookup: %p\n", tempVar);

	return 0;
}

