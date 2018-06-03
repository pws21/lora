#include <stdio.h>

int main () {

   int  var = 20;   /* actual variable declaration */
   int  *ip;        /* pointer variable declaration */
   
   ip = &var;  /* store address of var in pointer variable*/
   int **uu = &ip;

   printf("Address of &var variable: %p\n", &var  );

   /* address stored in pointer variable */
   printf("Address stored in ip variable: %p\n", ip );
   printf("Address stored in &ip variable: %p\n", &ip );

   /* uu */
   printf("Address stored in uu variable: %p\n", uu );
   printf("Address stored in &uu variable: %p\n", &uu );
   printf("Address stored in *uu variable: %p\n", *uu );

   /* access the value using the pointer */
   printf("Value of *ip variable: %d\n", *ip );

   return 0;
}