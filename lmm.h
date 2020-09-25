
#define MAX_NAME_SIZE 20

struct pageFamily{
	char pageFamilyName[MAX_NAME_SIZE];
	uint32_t pageFamilySize;
}

struct pageForFamilies{
	struct pageForFamilies * next=NULL;
	struct pageFamily pageFamilyArr[0];
}

#define MAX_pageFamily_PerPage \
	(PAGE_SIZE-sizeof(struct pageForFamilies *))/sizeof(pageFamily)
