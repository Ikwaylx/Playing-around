#include <stdio.h>
#include <math.h>

int main(void)
{
    // Testing in a program if integers round up or down cause why not basics
    int firstnum = 5;
    printf("The first number is %i.\nNow we'll divide by 2\n", firstnum);
    int secondnum = (firstnum/2);
    printf("The answer is %i. Notice that it rounds down an integer if the answer is 2.5.\nNow we'll round it.\n", secondnum);
    printf("The answer is %i.\n", (int) round(firstnum/2));
    return 0;
}