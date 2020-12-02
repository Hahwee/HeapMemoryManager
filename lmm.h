#ifndef LMM_H
#define LMM_H

#define MAX_NAME_SIZE 20

//define boolean states
typedef enum{
	MM_FALSE,
	MM_TRUE
} vmBool;


struct pageFamily{
	char pageFamilyName[MAX_NAME_SIZE];
	uint32_t pageFamilySize;
};

struct pageForFamilies{
	struct pageForFamilies * next;
	struct pageFamily pageFamilyArr[0];
};

struct blockMetadata{
	bool isAllocated;
	uint32_t blockSize;
	struct blockMetadata * nextBlock;
	struct blockMetadata * prevBlock;
	uint32_t offset;
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



//macros to manipulate metablock structure

//retrieves offset of given field
#define metaOffset(container_structure, field_name) \
	(uint32_t)(&(((container_structure *)0)->field_name));

//gets VM page pointer from offset
#define metaGetPage(metaBlockPtr) \
	((void *)(char *)metaBlockPtr - metaBlockPtr->offset);

//points to the next metablock
#define metaNextBlock(metaBlockPtr) \
	(metaBlockPtr->nextBlock);

//retrieves pointer to next metablock from block size
#define metaNextBlockBySize(metaBlockPtr) \
	(struct blockMetadata *)((char*)(metaBlockPtr + 1) + metaBlockPtr->blockSize);

//points to previious metablock
#define metaPrevBlock(metaBlockPtr) \
	(metaBlockPtr->prevBlock); 




#endif
