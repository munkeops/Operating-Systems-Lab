#include<stdio.h>
#include<stdlib.h>
#include<inttypes.h>
#include<unistd.h>
#include<time.h>

struct map
{
    uint16_t PFN;
    int Valid;
    int ProtectBits;
    int present;
    int dirty;
    int mode;
};

int search_pfn(struct map* vpns ,int n,int postn) 
{
    int j=0;
    while(j<postn)
    {
        if(vpns[j].PFN==n)
        {
            return 1;
        }
        j++;
    }
    return 0;
}
int search_vpa(uint16_t* x,int n,int postn)
{
    int j=0;
    while(j<postn)
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
    int a[size];
    int i=size-1;
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
void randomize_valid(struct map * vpns,int size)
{
    int i,v;
    for(i=0;i<size;i++)
    {
        v=rand()%2;
        vpns[i].Valid=v;
    }
}
void randomize_protect(struct map * vpns,int size)
{
    int i,k;
    for(i=0;i<size;i++)
    {
        k=rand()%10;
        vpns[i].ProtectBits=k;

    }
    
}

int randomize_pfns(struct map * vpns,int size)
{
    int i=0;
    for(i=0;i<size;i++)
    {
        uint16_t random_14=rand()%16384;
        
        if(search_pfn(vpns,random_14,i)==1)
        {
            i--;
        }
        else
        {
            vpns[i].PFN=random_14;            
        }
        
    }
}

void randomize_vpas(uint16_t * vpa,int size)
{
    int i=0;
    for(i=0;i<size;i++)
    {
        
        //uint16_t rand_num=0x0400*rand()%64+0001*rand()%1024; testing with 10;
        //int rand_10_vals[10];
        int rand_10=rand()%size;
        uint16_t rand_num=0x0400+(rand_10<<10)+rand()%1024;


        if(search_vpa(vpa,rand_num,i)==1)
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

    
    int n;
    printf("Enter number of vpas to generate\n");
    scanf("%d",&n);
    struct map vpns[n];
    uint16_t vpa[n];

    randomize_pfns(vpns,n);//randomising the page frames
    randomize_vpas(vpa,n);// randomise the virtual addresses
    randomize_valid(vpns,n);// randomise the valid bits
    randomize_protect(vpns,n);// 3 bits -RWE hence since we are only doing memory access , if R is 0 it wont be able to access
    
    int i;
    clock_t start,end;
    double cpu_time;
    double time;
    int counter=0;
    double avg_access_time=0;
    int shift=10;
    int OFFSET_MASK=0x03ff;
    int PFN_SHIFT=10;
    for(i=0;i<n;i++)
    {
        
        start =clock();
        uint16_t VirtualAddress=vpa[i];
        printf("Virtual Address : ");
        binary_printf(VirtualAddress,16);
        int VPN=VirtualAddress>>shift;
        struct map PTE =vpns[VPN];
        sleep(0.5);
        if(PTE.Valid==0)
        {
            printf("SEGMENTATION FAULT\n\n");
            continue;
            end=clock();

            //exit(0);
        }
        sleep(0.5);
        if(CanAccess(PTE.ProtectBits)==0)
        {
            printf("PROTECTION FAULT\n\n");
            end=clock();
            continue;
            //exit(0);
        }
        int offset=VirtualAddress & OFFSET_MASK;
        uint32_t PhysAddr=(PTE.PFN<<PFN_SHIFT)+offset;
        printf("PROTECTION BITS : ");
        binary_printf(PTE.ProtectBits,3);
        printf("VPN : ");        
        binary_printf(VPN,6);
        printf("OFFSET : ");        
        binary_printf(offset,10);
        printf("PFN : ");
        binary_printf(PTE.PFN,14);
        sleep(1);
        printf("Physical Address : ");
        binary_printf(PhysAddr,24);
        end=clock();
        time=end-start;
        cpu_time=((double)(end-start))/CLOCKS_PER_SEC;
        avg_access_time+=cpu_time;
        counter++;
        //printf("TIME : %e\n",time);
        printf("CPU TIME : %e\n",cpu_time);   
        sleep(1);    

        printf("\n");
       
    }
    avg_access_time=avg_access_time/counter;
    printf("average access time : %e\n",avg_access_time);
    
}