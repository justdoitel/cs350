
#include <stdio.h>
#include <stdlib.h>
void check(long x);
void perfect(long num);
void rem (const char *s);
void prime(int x);
void nearest(void);

int main(int argc, char const *argv[])
{
    for (long i = 1993;i<=2039;i++){
        check(i);
    }
    for (long i = 1;i<=100;i++){
    perfect(i);
    }
    rem("The quick brown fox jumps over the lazy dog");
    prime(100);
    nearest();
    return 0;
}
