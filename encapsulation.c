/* demo of Encapsulation and Decapsulation of datas */
#include "stdio.h"
#include "string.h"
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;

typedef char int8_t;
typedef short int int16_t;
typedef int int32_t;
typedef long int64_t;

#define TRACE_ERROR                              (0)
#define NULL_DATA_PTR                            ((void *)0)
#define NULL_CHAR                               ('\0')

#define STR_MAX_CHARS                            (7)

#define IP_MIN_VAL                               (1)
#define IP_MAX_VAL                               (1000)

typedef enum { SUCCESS, FAILURE } system_status_t;

uint16_t Get_Validate_Input_Number(void *const int32_input_num_ptr, char *const input_str_ptr, const unsigned int input_str_max_chars, const int32_t valid_min_value, const int32_t valid_max_value);
uint16_t Get_Input_Str(char *const input_str_ptr, const unsigned int input_str_max_chars);
uint16_t Str_to_Num_Conv(void *const num_conv_from_str, const char *const num_in_str);
uint32_t Power_Of(const uint8_t base, const uint8_t power);

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

char input_str[STR_MAX_CHARS];

int main()
{
    mainencap *mainptr = NULL;
    subencap1 *subptr;
    lastencap *lastptr;

    Main_Encap((void **)&mainptr);
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
       printf("\n sub_encap1: num: %d, name: %s, namelen: %d",subptr->ip, subptr->name, subptr->namelen);
       if(subptr->msgtype == 2)
       {
	        lastptr = (lastencap *)subptr->dataptr;
			if(trace_flag)
	            printf("\n lastptr: %#X",lastptr);
            printf("\n last_encap: msg type: %d, port: %d, uchar: %c ",subptr->msgtype, lastptr->port, lastptr->dataptr[0]);
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
   printf("\n In mainencap's msgtype = 1 for sub_encap1");
  // scanf("%d %d", &mainptr->mac, &mainptr->msgtype);
   printf("\n Enter: mac - ");
   if((Get_Validate_Input_Number(&mainptr->mac, input_str, STR_MAX_CHARS, IP_MIN_VAL, IP_MAX_VAL)) != SUCCESS)
   {
	   mainptr->mac = 50;
	   printf("\n ERR: invalid MAC  - default MAC = %d", mainptr->mac);
   }
   mainptr->msgtype = 1;
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
  // printf("Inside sub_encap1: Enter: num, name: ");
  // scanf("%d %s", &subptr->ip, subptr->name);
   printf("\n For message - %u, inside sub_encap1", ((mainencap *)ptr)->msgtype);
   printf("\n Enter: num - ");
   if((Get_Validate_Input_Number(&subptr->ip, input_str, STR_MAX_CHARS, IP_MIN_VAL, IP_MAX_VAL)) != SUCCESS)
   {
	   subptr->ip = 100;
	   printf("\n ERR: invalid IP  - default IP = %d", subptr->ip);
   }
   printf("Enter: name - ");
   if((Get_Input_Str(subptr->name, 20)) != SUCCESS)
   {
	   strcpy(subptr->name, "TEMP");
	   printf("\n ERR: invalid name  - default IP = %d", subptr->ip);
   }
   subptr->namelen = strlen(subptr->name) + 1;
   if(trace_flag)
        printf("\n ((mainencap *)ptr)->dataptr: %#X,&subptr->ip: %#X",((mainencap *)ptr)->dataptr, &subptr->ip);
   //printf("\n msgtype = 2 for last encap: Enter msgtype : ");
   //scanf("%d", &subptr->msgtype);
   subptr->msgtype = 2;
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

   subptr = (subencap1 *)((mainencap *)ptr)->dataptr;
/* lastptr = subptr->dataptr; */               /* warning: suspicous ptr conv */

  /* lastptr, is garbage */
  /* lastptr =  (lastencap *)((subencap1 *)((mainencap *)ptr)->dataptr)->dataptr; */
   lastptr =  (lastencap *)((subencap1 *)&((mainencap *)ptr)->dataptr)->dataptr;
   if(trace_flag)
   {
       printf("\n lastptr: %#X", lastptr);
   }
   printf("\n For sub_encap1 message - %u, inside last_encap ", subptr->msgtype);
   printf("\n Enter: port - " );
   if((Get_Validate_Input_Number(&lastptr->port, input_str, STR_MAX_CHARS, IP_MIN_VAL, IP_MAX_VAL)) != SUCCESS)
   {
	   lastptr->port = 80;
	   printf("\n ERR: invalid PORT - default port = %d", lastptr->port);
   }
   printf("\n Enter: a char - " );
   if((Get_Input_Str(input_str, 2)) != SUCCESS)
   {
	   lastptr->dataptr[0] = 'X';
	   printf("\n ERR: invalid CHAR - default char = %d", lastptr->dataptr[0]);
   }
   else
   {
	   lastptr->dataptr[0] = input_str[0];
   }
   return 1;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Str_to_Num_Conv

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           : digits are extracted from left to right format from digit in num_in_str

Func ID        : 02.04

BUGS           :
-*------------------------------------------------------------*/
uint16_t Str_to_Num_Conv( void *const num_conv_from_str_ptr, const char *const num_in_str)
{
	 int32_t num = 0, *num_conv_from_str;
	 uint32_t place;
	 int16_t cur_unit;
	 uint8_t num_chars = 0, base = 10, pos = 0, start_num_pos = 0 ;

	 if(num_conv_from_str_ptr == NULL_DATA_PTR || num_in_str == NULL_DATA_PTR )
	 {
		 #ifdef TRACE_ERROR
		   printf("ERR: data are null ptr \n");
		#endif
		return FAILURE;
	 }
	 num_conv_from_str = (int32_t *)num_conv_from_str_ptr;
	 *num_conv_from_str = 0;
	 if(num_in_str[0] >= '0' && num_in_str[0] <= '9')
	 {
		  start_num_pos = 0;
	 }
	 else if(num_in_str[0] == '-')
	 {
		 start_num_pos = 1;
	 }
	 else
	 {
    	 #ifdef TRACE_ERROR
		    printf("ERR: invalid char: %c \n", num_in_str[0]);
		 #endif
         return FAILURE;
	 }
	 num_chars = strlen(num_in_str + start_num_pos);
	 if(num_chars == 0)
	 {
		 #ifdef TRACE_ERROR
		    printf("ERR: data empty \n");
		 #endif
         return FAILURE;
	 }
	 pos = start_num_pos;
     for( place = Power_Of(base, num_chars - 1); place >= 1; place /= base, ++pos )
     {
     	 cur_unit = num_in_str[pos] - '0';
    	 if(cur_unit < 0 ||  cur_unit > 9 )
    	 {
	    	 #ifdef TRACE_ERROR
		       printf("ERR: invalid char at data[%d] = %c \n", pos, num_in_str[pos]);
		     #endif
             return FAILURE;
	     }
         num += (cur_unit * place);
     }
	 if(num_in_str[0] == '-')
	 {
		 *num_conv_from_str = -num;
	 }
	 else
	 {
	     *num_conv_from_str = num;
	 }
	 return SUCCESS;
}
/*------------------------------------------------------------*
FUNCTION NAME  : Power_Of

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 02.10

Bugs           :
-*------------------------------------------------------------*/
uint32_t Power_Of(const uint8_t base, const uint8_t power )
{
    uint32_t power_val = 1;
    uint8_t i = 0;

    if(power == 0)
    {
       return power_val;
    }
    for(i = 1; i <= power; ++i)
    {
      power_val *= base;
    }
    return power_val;
}
/*------------------------------------------------------------*
FUNCTION NAME  : Validate_Number_Input

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 01.02

BUGS           :
-*------------------------------------------------------------*/
uint16_t Get_Validate_Input_Number(void *const input_num_ptr, char *const input_str_ptr, const unsigned int input_str_max_chars, const int32_t valid_min_value, const int32_t valid_max_value)
{
	int32_t temp_int, *int32_input_num_ptr;

	if(int32_input_num_ptr == NULL_DATA_PTR)
	{
		return FAILURE;
	}
	if(valid_min_value > valid_max_value)
	{
	   return FAILURE;
	}
	int32_input_num_ptr = (int32_t *)input_num_ptr;
	*int32_input_num_ptr = 0;
	if((Get_Input_Str(input_str_ptr, input_str_max_chars)) != SUCCESS)
		return FAILURE;
	if((Str_to_Num_Conv(&temp_int, input_str_ptr)) != SUCCESS)
	{
		memset(input_str_ptr, NULL_CHAR, input_str_max_chars);
		return FAILURE;
	}
	memset(input_str_ptr, NULL_CHAR, input_str_max_chars);
	if(temp_int < valid_min_value || temp_int > valid_max_value)
	{
		 #ifdef TRACE_ERROR
		      printf("ERR: input data - %d, out of range [%d,%d] \n", temp_int, valid_min_value, valid_max_value);
		 #endif
		 return FAILURE;
	}
	*int32_input_num_ptr = temp_int;
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Get_Input_Str

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 01.02

BUGS           :
-*------------------------------------------------------------*/
uint16_t Get_Input_Str(char *const input_str_ptr, const unsigned int input_str_max_chars)
{
    unsigned int input_str_num_chars = 0;
	char rcvd_char;

    if(input_str_ptr == NULL_DATA_PTR || input_str_max_chars <= 1)
	{
		return FAILURE;
	}
    memset(input_str_ptr, NULL_CHAR, input_str_max_chars);
	while (1)
	{
		rcvd_char = (char) getchar();
		//scanf("%c", &rcvd_char);
		switch(rcvd_char)
        {
			case '\b':
              if(input_str_num_chars > 0)
			  {
			      input_str_ptr[input_str_num_chars] = NULL_CHAR;
				  --input_str_num_chars;
			  }
			break;
            case '\n':
			   if(input_str_num_chars != 0)
			   {
			      input_str_ptr[input_str_num_chars] = NULL_CHAR;
			      return SUCCESS;
			   }
            break;
            default:
     			if(input_str_num_chars + 1 < input_str_max_chars )
    		    {
		    	   input_str_ptr[input_str_num_chars] = rcvd_char;
                   ++input_str_num_chars;
	    		}
		    	else
			    {
			       printf("ERR: Input data num chars exceeds max chars : %u \n", input_str_max_chars - 1);
			       memset(input_str_ptr, NULL_CHAR, input_str_max_chars);
				   fflush(stdin);
				   return FAILURE;
			    }
		}
	}
	return SUCCESS;
}

