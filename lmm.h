#ifndef LMM_H
#define LMM_H

#define MAX_NAME_SIZE 20

struct pageFamily{
	char pageFamilyName[MAX_NAME_SIZE];
	uint32_t pageFamilySize;
};

struct pageForFamilies{
	struct pageForFamilies * next;
	struct pageFamily pageFamilyArr[0];
};



//subtracting pageForFamilies pointer because will need a portion of VM page to be reserved for address of next VM page
#define MAX_pageFamily_PerPage \
	(PAGE_SIZE-sizeof(struct pageForFamilies *))/sizeof(struct pageFamily)


#define ITERATE_PAGE_FAMILIES_BEGIN(startingVMPage, currentPageFamily) \
{	\
	uint32_t count = 0;	\
	for (currentPageFamily=(struct pageFamily *)&startingVMPage->pageFamilyArr[0];	\
		currentPageFamily->pageFamilySize && count < MAX_pageFamily_PerPage;	\
		currentPageFamily++,count++){				\

	

#define ITERATE_PAGE_FAMILIES_END(startingVMPage, currentPageFamily)}}

#endif
