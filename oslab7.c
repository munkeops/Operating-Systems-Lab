#include<stdio.h>
#include<stdlib.h>
#include<inttypes.h>

struct map
{
    uint16_t PFN;
    int Valid;
    int ProtectBits;
    int present;
    int dirty;
    int mode;
};

int search_pfn(struct map* vpns ,int n) 
{
    int j=0;
    while(j<10)
    {
        if(vpns[j].PFN==n)
        {
            return 1;
        }
        j++;
    }
    return 0;
}
int search_vpa(uint16_t* x,int n)
{
     int j=0;
    while(j<10)
    {
        if(x[j]==n)
        {
            return 1;
        }
        j++;
    }
    return 0;

}

void binary_printf(uint32_t n,int size)
{
    ;
    uint16_t num=0;
    int a[size];
    int i=size-1;
    //int i=15;
    while (n) 
    {
       
        if (n & 1)
        {
            a[i]=1;
            i--;
           
        }
        else
        {
            a[i]=0;
            i--;
            
        }

        n >>= 1;
    }
    while(i>=0)
    {
        a[i]=0;
        i--;
    }
    for(i=0;i<size;i++)
    {
        printf("%d",a[i]);
    }
    printf("\n");
    
}
void randomize_valid(struct map * vpns)
{
    int i,v;
    for(i=0;i<10;i++)
    {
        v=rand()%2;
        vpns[i].Valid=v;
    }
}
void randomize_protect(struct map * vpns)
{
    int i,k;
    for(i=0;i<10;i++)
    {
        k=rand()%10;
        vpns[i].ProtectBits=k;

    }
    
}

int randomize_pfns(struct map * vpns)
{
    int i=0;
    for(i=0;i<10;i++)
    {
        vpns[i].PFN=0;
    }
    for(i=0;i<10;i++)
    {
        uint16_t random_14=rand()%16384;
        uint16_t vpa[10];
        if(search_pfn(vpns,random_14)==1)
        {
            i--;
        }
        else
        {
            vpns[i].PFN=random_14;            
        }
        
    }
}

void randomize_vpas(uint16_t * vpa)
{
    int i=0;
    for(i=0;i<10;i++)
    {
        
        //uint16_t rand_num=0x0400*rand()%64+0001*rand()%1024; testing with 10;
        //int rand_10_vals[10];
        int rand_10=rand()%10;
        uint16_t rand_num=0x0400+(rand_10<<10)+rand()%1024;


        if(search_vpa(vpa,rand_num)==1)
        {
            i--;
        }
        else
        {
            vpa[i]=rand_num;            
        }
        
    }
}


int CanAccess(int n)
{
    if(n>=4)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
}


void main()
{

    struct map vpns[10];
    uint16_t vpa[10];


    randomize_pfns(vpns);
    randomize_vpas(vpa);
    randomize_valid(vpns);
    randomize_protect(vpns);
    
    int i;
    
    int a=4;
    //binary_printf(a);

    //uint16_t x=0x0c44;
    int shift=10;
    int OFFSET_MASK=0x03ff;
    int PFN_SHIFT=10;
    for(i=0;i<10;i++)
    {
        uint16_t VirtualAddress=vpa[i];
        printf("Virtual Address : ");
        binary_printf(VirtualAddress,16);
        int VPN=VirtualAddress>>shift;
        struct map PTE =vpns[VPN];
        if(PTE.Valid==0)
        {
            printf("SEGMENTATION FAULT\n\n");
            continue;
            //exit(0);
        }
        if(CanAccess(PTE.ProtectBits)==0)
        {
            printf("PROTECTION FAULT\n\n");
            continue;
            //exit(0);
        }
        int offset=VirtualAddress & OFFSET_MASK;
        uint32_t PhysAddr=(PTE.PFN<<PFN_SHIFT)+offset;
        printf("VPN : ");        
        binary_printf(VPN,6);
        printf("OFFSET : ");        
        binary_printf(offset,10);
        printf("PFN : ");
        binary_printf(PTE.PFN,14);
        printf("PROTECTION BITS : ");
        binary_printf(PTE.ProtectBits,3);
        printf("Physical Address : ");
        binary_printf(PhysAddr,24);
        
        printf("\n");
       
    }
    
}