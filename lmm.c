#include <stdio.h>
#include <memory.h>
#include <unistd.h> //getpagesize() function
#include <sys/mman.h> //mmap()
#include <stdint.h>
#include <assert.h> //needed for assert() function
#include <string.h>

//static global variables
static size_t PAGE_SIZE=0;
#include "lmm.h"

static struct pageForFamilies * startingVMPage=NULL;

//function to map virtual memory page size to memory and return pointer to beginning region of allocated memory
static void * get_VMpage_from_kernel(int units){
	void * vmpage=mmap(0,PAGE_SIZE*units,PROT_EXEC|PROT_WRITE|PROT_READ,MAP_PRIVATE|MAP_ANONYMOUS,0,0);	
	printf("VM page memory address at: %p\n",vmpage);

	if (vmpage==MAP_FAILED){
		printf("Unable to get page\n");
		return NULL;
	}

	return vmpage;
}

//function to unmap virual memory page/s from memory and returns success/fail value
static void return_VMpage_to_kernel(void * vmPage, int units){
	int flagValue=munmap(vmPage,units);
	printf("%d\n",flagValue);
	if (flagValue==-1)
		printf("Failed to unmap memory\n");
	else if (flagValue==0)
		printf("Successfully unmapped memory\n");
}

//function to add a new page family to VM Page with available memory
void new_pageFamily_Instance(char * name, uint32_t size){

	//initialize variables
	struct pageFamily * currentPageFamily = NULL;
	struct pageForFamilies * pageForFamilies_NewInstance = NULL;

	//checks to see if size of added page family is larger than the maximum size of a VM Page
	if (size>PAGE_SIZE){
		printf("Error, struct exceeds size of VM Page, Function name: %s\n", __FUNCTION__);
		return;
	}

	//If there is no initialized VM page, then allocate memory for it 
	if (!startingVMPage){
		startingVMPage=(struct pageForFamilies *)get_VMpage_from_kernel(1);
		startingVMPage->next=NULL;
		strncpy(startingVMPage->pageFamilyArr[0].pageFamilyName, name, MAX_NAME_SIZE);
		startingVMPage->pageFamilyArr[0].pageFamilySize=size;
		return;
	}

	//count var to track number of page families on VM Page
	uint32_t count = 0;

	//Iteration macro to loop over page families 
	ITERATE_PAGE_FAMILIES_BEGIN(startingVMPage, currentPageFamily) {

		if(strncmp(currentPageFamily->pageFamilyName, name, MAX_NAME_SIZE) != 0) {
			count++;
			continue;
		}

		assert(0);

	} ITERATE_PAGE_FAMILIES_END(startingVMPage, currentPageFamily);

	//creates new VM page if current VM pagefamily limit reached
	if (count == MAX_pageFamily_PerPage){
		pageForFamilies_NewInstance = (struct pageForFamilies *)get_VMpage_from_kernel(1);
		pageForFamilies_NewInstance->next = startingVMPage;
		startingVMPage = pageForFamilies_NewInstance;
		currentPageFamily = &startingVMPage->pageFamilyArr[0];		
	}

	//copies data to newly added pageFamily
	strncpy(currentPageFamily->pageFamilyName, name, MAX_NAME_SIZE);
	currentPageFamily->pageFamilySize = size;
	
}

//prints all page families in VM Pages
void print_RegisteredPageFams(){
	
	//variables initialized
	struct pageForFamilies * tempVMPage = startingVMPage;
	struct pageFamily * currentPageFamily = NULL;
	uint32_t count = 0;
	
	//Iterates through all VM pages and prints each page family 
	while(tempVMPage != NULL){
		ITERATE_PAGE_FAMILIES_BEGIN(tempVMPage, currentPageFamily) {
			printf("Page Family: %s, Size = %d\n", currentPageFamily->pageFamilyName, currentPageFamily->pageFamilySize);
		} ITERATE_PAGE_FAMILIES_END(tempVMPage, currentPageFamily);
		tempVMPage = tempVMPage->next;
	}
}

//looks up specific page family
struct pageFamily * pageFamilyLookup(char * name){

	struct pageForFamilies * tempVMPage = startingVMPage;
	struct pageFamily * currentPageFamily = NULL;
	uint32_t count = 0;

	//debug print: printf("Current starting VM Page: %p\n", tempVMPage);	

	//loops through all VM pages and compares page family names with given input, returns ptr if true, NULL if false
	while(tempVMPage != NULL){
		ITERATE_PAGE_FAMILIES_BEGIN(tempVMPage, currentPageFamily) {
			if (strncmp(currentPageFamily->pageFamilyName, name, MAX_NAME_SIZE ) == 0){
				printf("TestingName: %s\n", name);
				return currentPageFamily;
			}
			else
				continue;
		}ITERATE_PAGE_FAMILIES_END(tempVMPage, currentPageFamily);
		tempVMPage = tempVMPage->next;
	}
	return NULL;
}

//initialize program, retrieves VM page size of system
void mm_init(){
	printf("Running\n");
	PAGE_SIZE=getpagesize();
}
