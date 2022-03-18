#include<map>
#include<string.h>

struct my_cmp
{
    bool operator()(const char* a, const char* b) const
    {
        return strcmp(a, b) < 0;
    }
};