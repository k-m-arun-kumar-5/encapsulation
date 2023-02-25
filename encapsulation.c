/* demo of Encapsulation and Decapsulation of datas */
#include "stdio.h"
#include "string.h"

typedef struct
{
    int mac;
    int msgtype;
    unsigned char dataptr[1];
} mainencap;

typedef struct
{
    int ip;
    int namelen;
    char name[20];
    int msgtype;
    unsigned char dataptr[1];
} subencap1;

typedef struct
{
    int port;
    unsigned char dataptr[1];
} lastencap;

unsigned char tempdata[50];
char trace_flag = 0;

int Main_Encap(void **const ptr);
int Sub_Encap1(void *const ptr);
int LastEncap(void *const ptr);


int main()
{
    mainencap *mainptr = NULL;
    subencap1 *subptr;
    lastencap *lastptr;

    Main_Encap(&mainptr);
    if(trace_flag)
       printf("\n inside main: mainptr: %#X", mainptr);
    printf("\n mainencap: mac: %d",mainptr->mac);
    if(mainptr->msgtype == 1)
    {
        printf("\n mainencap: msgtype: %d", mainptr->msgtype);
       /* subptr points to garbage address */
       /* subptr = mainptr->dataptr; */    /* warning: suspicous ptr conv */
       subptr = (subencap1 *)mainptr->dataptr;
       if(trace_flag)
            printf("\n subptr: %#X ",subptr);
       printf("\n subencap1: num: %d, name: %s, namelen: %d",subptr->ip, subptr->name, subptr->namelen);
       if(subptr->msgtype == 2)
       {
	        lastptr = (lastencap *)subptr->dataptr;
	        printf("\n lastptr: %#X",lastptr);
            printf("\n last encap: msg type: %d, port: %d, uchar: %c ",subptr->msgtype, lastptr->port, lastptr->dataptr[0]);
       }
    }
    return 1;
}

int Main_Encap(void **const ptr)
{
   mainencap *mainptr = NULL;
   *ptr = mainptr = (mainencap *)tempdata;
   if(trace_flag)
       printf("\n mainptr: %#X", mainptr);
   printf("\n In mainencap's num & msgtype = 1 for subencap 1: Enter nums - ");
   scanf("%d %d", &mainptr->mac, &mainptr->msgtype);

   if((mainptr)->msgtype == 1)
       Sub_Encap1(mainptr);
   return 1;
}

int Sub_Encap1(void *const ptr)
{
   subencap1 *subptr;

  /* subptr = ptr->dataptr; */  /* error: ptr req at left side of -> */

   /* subptr points to NULL, if assigned below */
   /* subptr = ((mainencap *)ptr)->dataptr; */        /* warning: suspicous ptr conv */
   /* we are using address allocated dataptr, */
   subptr =  (subencap1 *)((mainencap *)ptr)->dataptr;
   if(trace_flag)
     printf("\n subptr: %#X ",subptr);
   printf("Inside sub_encap1: Enter: num, name: ");
   scanf("%d %s", &subptr->ip, subptr->name);
   subptr->namelen = strlen(subptr->name);
   if(trace_flag)
        printf("\n ((mainencap *)ptr)->dataptr: %#X,&subptr->ip: %#X",((mainencap *)ptr)->dataptr, &subptr->ip);
   printf("\n msgtype = 2 for last encap: Enter msgtype : ");
   scanf("%d", &subptr->msgtype);
   if(subptr->msgtype == 2)
     LastEncap(ptr);
   return 1;
}

int LastEncap(void *const ptr)
{
   subencap1 *subptr;
   lastencap *lastptr;

   /* lastptr =  (lastencap *)((subencap1 *)ptr->dataptr)->dataptr; */
    /* error: ptr req on left side of -> */

/* subptr = ((mainencap *)ptr)->dataptr;*/     /* warning: suspicous ptr conv */
/* lastptr = subptr->dataptr; */               /* warning: suspicous ptr conv */

  /* lastptr, is garbage */
  /* lastptr =  (lastencap *)((subencap1 *)((mainencap *)ptr)->dataptr)->dataptr; */
   lastptr =  (lastencap *)((subencap1 *)&((mainencap *)ptr)->dataptr)->dataptr;
   if(trace_flag)
   {
       printf("\n lastptr: %#X", lastptr);
   }
   printf("\n Inside lastencap - Enter: port & uchar: " );
   scanf("%d %c", &lastptr->port, &lastptr->dataptr[0]);
   return 1;

}

