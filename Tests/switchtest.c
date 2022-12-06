#include <stdio.h>
#include <string.h>

int main(void)
{
    int x = 1;
switch(x)
{
	case 1:
		printf("Five, ");
	case 2:
		printf("four, ");
	case 3:
		printf("three, ");
	case 4:
		printf("two, ");
	case 5:
		printf("one... ");
	default:
		printf("Blast off!\n");
}

}