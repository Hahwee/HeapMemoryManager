#ifndef API_LMM_H
#define API_LMM_H

#include <stdint.h>

void mm_init();

void new_pageFamily_Instance(char * name, uint32_t size);

void print_RegisteredPageFams();

#define MM_REG_STRUCT(name) \
	(new_pageFamily_Instance(#name,sizeof(name))) 

#endif
