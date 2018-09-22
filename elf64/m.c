#include <sys/mman.h>
#include <stdio.h>
       #include <sys/types.h>
       #include <unistd.h>
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>

int main(){
	printf("%d\n", PROT_READ|PROT_WRITE);
	printf("%d\n", MAP_PRIVATE);
	printf("%d\n", O_WRONLY | O_CREAT);
	printf("%d\n", MAP_FAILED);
	printf("%d\n", SEEK_END);

}
