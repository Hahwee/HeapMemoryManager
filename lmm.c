#include <stdio.h>
#include <memory.h>
#include <unistd.h> //getpagesize() function
#include <sys/mman.h> //mmap()

//static variables
static size_t PAGE_SIZE=0;

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

int main(){
	//function to getpagesize of virtual memory page on Linux system and print
	PAGE_SIZE=getpagesize();
	printf("%ld\n",PAGE_SIZE);

	void * vmpage_Address=get_VMpage_from_kernel(2);
	return_VMpage_to_kernel(vmpage_Address, PAGE_SIZE*0);

	return 0;
}

