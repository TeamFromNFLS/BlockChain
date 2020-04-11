#include <iostream>
#include <cstdio>
#include <cstring>
#include "ripemd160.h"

#define RMDsize 160

byte *RMD(byte *message)
/*
* returns RMD(message)
* message should be a string terminated by '\0'
*/
{
     dword MDbuf[RMDsize / 32];         /* contains (A, B, C, D(, E))   */
     static byte hashcode[RMDsize / 8]; /* for final hash-value         */
     dword X[16];                       /* current 16-word chunk        */
     unsigned int i;                    /* counter                      */
     dword length;                      /* length in bytes of message   */
     dword nbytes;                      /* # of bytes not yet processed */

     /* initialize */
     MDinit(MDbuf);
     length = (dword)strlen((char *)message);

     /* process message in 16-word chunks */
     for (nbytes = length; nbytes > 63; nbytes -= 64)
     {
          for (i = 0; i < 16; i++)
          {
               X[i] = BYTES_TO_DWORD(message);
               message += 4;
          }
          compress(MDbuf, X);
     } /* length mod 64 bytes left */

     /* finish: */
     MDfinish(MDbuf, message, length, 0);

     for (i = 0; i < RMDsize / 8; i += 4)
     {
          hashcode[i] = MDbuf[i >> 2];             /* implicit cast to byte  */
          hashcode[i + 1] = (MDbuf[i >> 2] >> 8);  /*  extracts the 8 least  */
          hashcode[i + 2] = (MDbuf[i >> 2] >> 16); /*  significant bits.     */
          hashcode[i + 3] = (MDbuf[i >> 2] >> 24);
     }

     return (byte *)hashcode;
}

int main()
{
     byte *b = (byte *)"The quick brown fox jumps over the lazy dog";
     byte *hashcode = RMD(b);
     printf("hashcode: ");
     char ret[41] = {};
     for (int j = 0; j < RMDsize / 8; j++)
     {
          char c[5];
          sprintf_s(c, "%02x", hashcode[j]);
          strcat_s(ret, c);
     }
     printf("%s", ret); //hashcode: 37f332f68db77bd9d7edd4969571ad671cf9dd3b
     printf("\n");
}
