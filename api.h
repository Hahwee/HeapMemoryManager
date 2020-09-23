#include <stdio.h>
#include <memory.h>
#include <unistd.h> //getpagesize() function
#include <sys/mman.h> //mmap()

static size_t PAGE_SIZE=0;

void mm_init(){

	PAGE_SIZE=getpagesize();

}

static void * mm_get_new_vm_page_from_kernel(int units){
	char *vm_page==mmap(0,units*PAGE_SIZE,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_ANON|MAP_PRIVATE,0,0);

	if(vm_page==MAP_FAILED){
		printf("Error: Failed to allocate\n");
		reutrn NULL;
	}

	memset(vm_page,0,units*PAGE_SIZE);
	return (void *)vm_page;
}

static void mm_return_vm_page_to_kernal(void *vm_page, int units){
	if(munmap(vm_page,units*PAGE_SIZE)){
		printf("Error");
	}
}

int main(int argc, char **argv){
	mm_init();
	printf("VM size = %lu\n", PAGE_SIZE);
	void *addr1 = mm_get_new_vm_page_from_kernel(1);
	void *addr2 = mm_get_New_vm_page_from_kernal(1);
	printf("page1 = %p\n, page2 = %p\n", addr1,addr2);
	return 0;
}
