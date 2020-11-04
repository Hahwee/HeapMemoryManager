#include <stdio.h>
#include <memory.h>
#include <unistd.h> //getpagesize() function
#include <sys/mman.h> //mmap()
#include <stdint.h>
#include <assert.h> //needed for assert() function

//static global variables
static size_t PAGE_SIZE=0;
#include "lmm.h"

static struct pageForFamilies * startingVMPage=NULL;
struct pageFamily * currentPageFamily = NULL;

//function to map virtual memory page size to memory and return pointer to beginning region of allocated memory
static void * get_VMpage_from_kernel(int units){
	void * vmpage=mmap(0,PAGE_SIZE*units,PROT_EXEC|PROT_WRITE|PROT_READ,MAP_PRIVATE|MAP_ANONYMOUS,0,0);	
	printf("%p\n",vmpage);

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

void new_pageFamily_Instance(char * name, uint32_t size){

	struct pageFamily * currentPageFamily = NULL;
	struct pageForFamilies * pageForFamilies_NewInstance = NULL;

	if (size>PAGE_SIZE){
		printf("Error, struct exceeds size of VM Page, Function name: %s\n", __FUNCTION__);
		return;
	}

	
	if (!startingVMPage){
		startingVMPage=(struct pageForFamilies *)get_VMpage_from_kernel(1);
		startingVMPage->next=NULL;
		strncpy(startingVMPage->pageFamilyArr[0].pageFamilyName, name, MAX_NAME_SIZE);
		startingVMPage->pageFamilyArr[0].pageFamilySize=size;
		return;
	}

	uint32_t count = 0;

	ITERATE_PAGE_FAMILIES_BEGIN(startingVMPage, currentPageFamily) {

		if(strncmp(currentPageFamily->pageFamilyName, name, MAX_NAME_SIZE) != 0) {
			count++;
			continue;
		}

		assert(0);

	} ITERATE_PAGE_FAMILIES_END(startingVMPage, currentPageFamily);

	if (count == MAX_pageFamily_PerPage){
		pageForFamilies_NewInstance = (struct pageForFamilies *)get_VMpage_from_kernel(1);
		pageForFamilies_NewInstance->next = startingVMPage;
		startingVMPage = pageForFamilies_NewInstance;
		currentPageFamily = &startingVMPage->pageFamilyArr[0];		
	}

	strncpy(currentPageFamily->pageFamilyName, name, MAX_NAME_SIZE);
	currentPageFamily->pageFamilySize = size;
	
}

int main(){
	//function to getpagesize of virtual memory page on Linux system and print
	PAGE_SIZE=getpagesize();
	printf("%ld\n",PAGE_SIZE);

	void * vmpage_Address=get_VMpage_from_kernel(2);
	return_VMpage_to_kernel(vmpage_Address, PAGE_SIZE*2);

	printf("%ld\n",MAX_pageFamily_PerPage);

	return 0;
}

