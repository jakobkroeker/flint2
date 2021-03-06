/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2013 William Hart

******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include "flint.h"

int flint_sscanf(const char * s, const char * str, ...)
{
   va_list ap;
   size_t len = strlen(str);
   char * str2 = flint_malloc(len + 1);
   char * s2 = flint_malloc(strlen(s) + 1);
   int * w1 = NULL, * w2 = NULL;
   void ** w3;
   double * d;
   ulong * wu;
   slong * w;
   int args, floating;
   int ret;
   
   /* deal with first substring */
   size_t n = strcspn(str, "%");
   strncpy(str2, s, n);
   str2[n] = '\0';
   ret = 0;
   len -= n;
   str += n;
   s += n;

   va_start(ap, str);

   while (len) /* deal with fmt spec prefixed strings */
   {
      n = strcspn(str + 2, "%") + 2; /* be sure to skip a %% */
      strncpy(str2, str, n);
      str2[n] = '\0';
   
      switch (str[1])
      {
      case 'w':
         if (str[2] == 'x')
         {
            wu = (ulong *) va_arg(ap, ulong *);
            ret += sscanf(s, WORD_FMT "x", wu);
            s += sprintf(s2, WORD_FMT "x", *wu);
            s += n - 3;
         } else if (str[2] == 'u')
         {
            wu = (ulong *) va_arg(ap, ulong *);
            ret += sscanf(s, WORD_FMT "u", wu);
            s += sprintf(s2, WORD_FMT "u", *wu);
            s += n - 3;
         } else if (str[2] == 'd')
         {
            w = (slong *) va_arg(ap, slong *);
            ret += sscanf(s, WORD_FMT "d", w);
            s += sprintf(s2, WORD_FMT "d", *w);
            s += n - 3;
         } else
         {
            w = (slong *) va_arg(ap, slong *);
            ret += sscanf(s, WORD_FMT "d", w);
            s += sprintf(s2, WORD_FMT "d", *w);
            s += n - 2;
         }
         break;
      default: /* pass to printf */
         args = parse_fmt(&floating, str2);
         if (args) 
         {
            if (args == 3)
               w1 = va_arg(ap, int *);
            if (args >= 2)
               w2 = va_arg(ap, int *);
            if (floating)
            {
               d = va_arg(ap, double *);
               if (args == 2)
               {
                  ret += sscanf(s, str2, w2, d);
                  s += sprintf(s2, str2, *w2, *d);
               }
               else if (args == 3)
               {
                  ret += sscanf(s, str2, w1, w2, d);
                  s += sprintf(s2, str2, *w1, *w2, *d);
               }
               else
               {
                  ret += sscanf(s, str2, d);
                  s += sprintf(s2, str2, *d);
               }
            } else
            {
               w3 = va_arg(ap, void **);
               if (args == 2)
               {
                  ret += sscanf(s, str2, w2, w3);
                  s += sprintf(s2, str2, *w2, *w3);
               }
               else if (args == 3)
               {
                  ret += sscanf(s, str2, w1, w2, w3);
                  s += sprintf(s2, str2, *w1, *w2, *w3);
               }
               else
               {
                  ret += sscanf(s, str2, w3);
                  s += sprintf(s2, str2, *w3);
               }
            }
         } else /* zero args */
            s += n;
      }

      len -= n;
      str += n;
   }

   va_end(ap);

   flint_free(str2);
   flint_free(s2);

   return ret;
}
