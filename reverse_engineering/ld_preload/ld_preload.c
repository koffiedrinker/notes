#include<stdio.h>
#include<unistd.h>

int main() {
	int i = 0;
	while(i < 100) {
		if(i == 0)
			printf("Zzzz");
		else
			printf("zzzz");
		fflush(stdout);

		sleep(1);
		i++;
	}
	printf("\n");

	return 0;
}
