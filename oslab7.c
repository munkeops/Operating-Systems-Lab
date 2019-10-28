#include<stdio.h>

struct entry
{
    unsigned int vpn:3;
    unsigned int pfn:14;
    unsigned int offset:10;
    unsigned int valid:1;
    unsigned int present:1;
    unsigned int rwe:1;
    unsigned int protection:1;
    unsigned int dirty:1;
    unsigned int x:1;

}s;


void main()
{
    s.vpn=5;
    printf("%d\n",s.vpn);

    
}