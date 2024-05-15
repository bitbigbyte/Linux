#include <stdio.h>
#include <stdlib.h>

#define DEBUG
#ifdef DEBUG
#define DBG(...) fprintf(stderr, " DBG(%s, %s(), %d): ", __FILE__, __FUNCTION__, __LINE__); fprintf(stderr, __VA_ARGS__)
#else
#define DBG(...)
#endif

int main(void)
{
    while(1)
    {
        DBG("this is a debug message!\n");
        sleep(1);
    }
    return 0;
}