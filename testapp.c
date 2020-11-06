#include "api_lmm.h"

struct gundam {
	char name[20];
	uint32_t attack;
	uint32_t defense;
	uint32_t mobility;
	struct gundam * next;
};

struct ship {
	char name[20];
	uint32_t capacity;
	uint32_t attack;
	uint32_t defense;
};




int main(int argc, char **argv){

	mm_init();
	MM_REG_STRUCT(struct gundam);
	MM_REG_STRUCT(struct ship);
	
	print_RegisteredPageFams();

	return 0;
}

