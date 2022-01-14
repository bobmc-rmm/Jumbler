// -*- C++ -*-
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
/**@file  jumble.c 
 * @brief try to unscramble a Jumble puzzle word
 * 2022-jan-12
 */ 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef unsigned char  u8t;
enum {
   eBsize = 16,
};

int  checkValid(void);
void sort( char *p, int sz);
int  numArray( int num );
void on_error( int arg, char *msg );
void scan_dictionary( char *pWord, int sz );

char letters[eBsize+4];
char digits[eBsize+4];
int lines = 0;
int LenMax = 0;
int First=0;
int Last=0;
int SumSequence = 0;

#define DICT "data3.txt" // dictionary file
static FILE *glfp;
//-------------------------------------------------------------------- 
/// @brief permutate a jumble word; search for it in dictionary
/// @param ..argc count of argv[]'s
/// @param argv[1] ..jumple word to process
int main(int argc, char *argv[]){
   if( argc != 2 ) on_error(-1,"main input incorrect");
   int iv = strlen(argv[1]);
   if( iv < 3 || iv > 8 ) on_error(-2, "word length out of range");
   strcpy( letters, argv[1] );
   LenMax = strlen(letters)-1;
   //goto skip1;
    
   // setup a range of permutations
   switch( LenMax ){
   case 8: First = 12345678; Last = 876543210; break; // 9 letters
   case 7: First =  1234567; Last =  76543210; break; // 8 letters
   case 6: First =   123456; Last =   6543210; break; // 7 letters
   case 5: First =    12345; Last =    543210; break; // 6 letters
   case 4: First =     1234; Last =     43210; break; // 5 letters
   case 3: First =      123; Last =      3210; break; // 4 letters
   default: exit(1); break;
   }
   printf("%s.. ",letters);
   sort(letters,LenMax+1);
   printf("%s sorted\n",letters);
   memset(digits,0,sizeof(digits));

   SumSequence = 0; // example: 0+1+2+3+4=10
   for(int i=0; i <= LenMax; i++ ){
      SumSequence += i;  // this is the reference sum
   }
    
   glfp = fopen(DICT,"r");
   if( glfp==NULL ) on_error(-3,"Can't open input file");

   for( int i=First; i <= Last; i++ ){
      numArray(i);
   }
   fclose(glfp);
}

//-------------------------------------------------------------------- 
// Convert instance sequential counter to array of digits. For
// example, a 5byte jumble word will have (43210-1234=41976)
// possibilities. But many will contain a digit value>4 which is
// invalid. There are only 119 variants for a 5byte word.
/// \brief Use integer array to represent jumble word variations.
int numArray( int num ){
   int i = LenMax, s1 = num, s2=0, s3=0;
   while( i >= 0 ){
      s2 = s1/10;
      s3 = s1 - (s2*10);
      digits[i]=s3;
      s1 = s2;
      i--;
   }
   
   do{ // sum of indices for permutation instance validation
      int sum=0;
      for(int i=0; i <= LenMax; i++ ){
	 if( digits[i] > LenMax ) return -1;
	 sum += digits[i];
      }
      if( sum != SumSequence ) return -1;
   }while(0);
   
   if( checkValid() < 0 ) return -1;
   
   char tbuff[eBsize+4];
   do{ // convert permutation integer to instance string
      for(int i=0; i <= LenMax; i++ ){
	 int idx = digits[i];
	 tbuff[i]=letters[idx];
	 tbuff[i+1]=0;
      }
      //printf("+++ %s\n",tbuff);
   }while(0);

   scan_dictionary( tbuff, strlen(tbuff));
   return 0;
}

//-------------------------------------------------------------------- 
/// \brief test if jumble variant found in dictionary
/// @param pWord ..point to jumble word
/// @param sz ..word length
void scan_dictionary( char *pWord, int sz ){
   enum { BSIZE=64 };
   static char buff[BSIZE];
   int r1 = fseek( glfp, 0L, SEEK_SET );
   if( r1 != 0 ) on_error(-4,"file seek error");
   while( !feof(glfp) ){
      char *p2 = fgets(buff, sizeof(buff)-2, glfp);
      if( p2==NULL ) break;
      int s = strlen(buff);
      buff[s-1]=0;     // delete 0x0A linefeed
      // printf("%06d, %s\n",(int)strlen(buff),buff);
      r1 = strcmp(pWord,buff);
      if( r1==0 ){
	 printf("[%s = %s]\n", pWord,buff);
	 break;
      }
   }
}

//--------------------------------------------------------------------
/// \brief sort the array index and verify it's sequence
int checkValid(void){
   char buf[eBsize+4];
   memcpy(buf,digits,sizeof(digits));
   sort(buf,LenMax+1);
   for(int i=1; i <= LenMax; i++ ){
      if( buf[i-1] == buf[i] ) return -1;
   }
   return 0;
}

//-------------------------------------------------------------------- 
/// \brief simple sort of small byte array
void sort( char *p, int sz){
   for (int i=0; i<sz-1; i++){
      for (int j=i+1; j<sz; j++){
	 if (p[i] > p[j]){
	    char t=p[i];
	    p[i] = p[j];
	    p[j] = t;
	 }
      }
   }
}

//--------------------------------------------------------------------
void on_error( int arg, char *msg ){
   printf("%d .. %s\n", arg,msg);
   exit(1);
}

// jumble.c
