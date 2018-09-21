int main(void) asm ("entry");

#include <lib.h>

int main(){
	char **files = getFiles("/tmp/ok");

	cleanArr(&files);
}
