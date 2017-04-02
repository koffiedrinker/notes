# LD_PRELOAD (ELF) trick

When the "loader" is mapped into memory by the kernel and control is transferred from the kernel to the loader (something like `ld-linux-x86-64.so.2`), the loader checks the `LD_PRELOAD` environment variable and checks the DYNAMIC segment for binaries to load. If you create a library that contains functions with the same function signature as the original libc functions then you have effectively overwritten that function.

This trick is handy to bypass certain "slow" functions or computations that don't change the functionality of the binary itself.

## Example

Let's say we have this program:
```c
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
```

Source: https://www.gnu.org/software/libc/manual/html_node/Sleeping.html

```bash
[koffiedrinker@ctf bag_of_tricks]$ !gcc
gcc -o ld_preload ld_preload.c 
[koffiedrinker@ctf bag_of_tricks]$ ./ld_preload 
Zzzzzzzzzzzzzzzzzzzzzzzz^C
```

Which slowly prints out a string of 'z'. But we don't want to wait 100 seconds for the program to finish. We can preload our own sleep() function to bypass the real sleep().

```c
unsigned int sleep(unsigned int seconds) {}
```
Source of sleep() signature: https://www.gnu.org/software/libc/manual/html_node/Sleeping.html

Compiling the library:
```
$ gcc -shared -o yourLibrary.so yourLibrary.c
```

And TADA:
```
[koffiedrinker@ctf bag_of_tricks]$ ./ld_preload 
Zzzzzzzz^C
[koffiedrinker@ctf bag_of_tricks]$ # Too slow!
[koffiedrinker@ctf bag_of_tricks]$ LD_PRELOAD=./yourLibrary.so ./ld_preload 
Zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
```

## CTF Examples

* [HITB 2013 - bin100](https://cedricvb.be/post/reverse-engineering-the-hitb-binary-100-ctf-challenge/)
