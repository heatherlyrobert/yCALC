/*============================----beg-of-source---============================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"


char         /*-> tbd --------------------------------[ leaf   [fc.320.112.20]*/ /*-[01.0000.023.!]-*/ /*-[--.---.---.--]-*/
CALC__find_fcat      (char a_fcat)
{
   int         i           =    0;
   for (i = 0; i < MAX_FCAT; ++i) {
      if (s_fcats [i].fcat == ' ')  break;
      if (s_fcats [i].fcat == a_fcat) return i;
   }
   return 0;
}

char         /*-> tbd --------------------------------[ leaf   [fe.D90.326.#0]*/ /*-[01.0000.013.!]-*/ /*-[--.---.---.--]-*/
CALC__func_func      (char *a_func, char *a_terms, char *a_show)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         x_len       =    0;
   /*---(defense)------------------------*/
   --rce;  if (a_func  == NULL)   return rce;
   --rce;  if (a_terms == NULL)   return rce;
   --rce;  if (a_show  == NULL)   return rce;
   /*---(prepare)--------------------------*/
   sprintf (a_show, "?func?");
   x_len = strlen (a_terms);
   /*---(void)-----------------------------*/
   if (x_len == 2) {
      if (strcmp ("n:"    , a_terms) == 0)  sprintf (a_show, "val   %s ()"        , a_func);
      if (strcmp ("s:"    , a_terms) == 0)  sprintf (a_show, "str   %s ()"        , a_func);
      if (strcmp ("a:"    , a_terms) == 0)  sprintf (a_show, "adr   %s ()"        , a_func);
      return 0;
   }
   /*---(unary)----------------------------*/
   if (x_len == 3) {
      if (strcmp ("n:n"   , a_terms) == 0)  sprintf (a_show, "val   %s (x)"        , a_func);
      if (strcmp ("n:s"   , a_terms) == 0)  sprintf (a_show, "val   %s (n)"        , a_func);
      if (strcmp ("n:a"   , a_terms) == 0)  sprintf (a_show, "val   %s (a)"        , a_func);
      if (strcmp ("n:r"   , a_terms) == 0)  sprintf (a_show, "val   %s (a..o)"     , a_func);
      if (strcmp ("s:n"   , a_terms) == 0)  sprintf (a_show, "str   %s (x)"        , a_func);
      if (strcmp ("s:s"   , a_terms) == 0)  sprintf (a_show, "str   %s (n)"        , a_func);
      if (strcmp ("s:a"   , a_terms) == 0)  sprintf (a_show, "str   %s (a)"        , a_func);
      if (strcmp ("t:n"   , a_terms) == 0)  sprintf (a_show, "t/f   %s (x)"        , a_func);
      if (strcmp ("t:a"   , a_terms) == 0)  sprintf (a_show, "t/f   %s (a)"        , a_func);
      if (strcmp ("a:n"   , a_terms) == 0)  sprintf (a_show, "adr   %s (x)"        , a_func);
      if (strcmp ("?:n"   , a_terms) == 0)  sprintf (a_show, "v/s   %s (x)"        , a_func);
      if (strcmp ("?:r"   , a_terms) == 0)  sprintf (a_show, "v/s   %s (a..o)"     , a_func);
      return 0;
   }
   /*---(binary)---------------------------*/
   if (x_len == 4) {
      if (strcmp ("n:nn"  , a_terms) == 0)  sprintf (a_show, "num   %s (x, y)"      , a_func);
      if (strcmp ("s:ss"  , a_terms) == 0)  sprintf (a_show, "str   %s (n, m)"      , a_func);
      if (strcmp ("s:sn"  , a_terms) == 0)  sprintf (a_show, "str   %s (n, x)"      , a_func);
      if (strcmp ("s:an"  , a_terms) == 0)  sprintf (a_show, "str   %s (a, x)"      , a_func);
      if (strcmp ("a:nn"  , a_terms) == 0)  sprintf (a_show, "adr   %s (x, y)"      , a_func);
      if (strcmp ("?:nn"  , a_terms) == 0)  sprintf (a_show, "v/s   %s (x, y)"      , a_func);
   }
   /*---(terciary)-------------------------*/
   if (x_len == 5) {
      if (strcmp ("n:nnn" , a_terms) == 0)  sprintf (a_show, "val   %s (x, y, z)"    , a_func);
      if (strcmp ("n:tnn" , a_terms) == 0)  sprintf (a_show, "val   %s (t, y, z)"    , a_func);
      if (strcmp ("s:tss" , a_terms) == 0)  sprintf (a_show, "str   %s (t, n, m)"    , a_func);
      if (strcmp ("s:nss" , a_terms) == 0)  sprintf (a_show, "str   %s (x, n, m)"    , a_func);
      if (strcmp ("s:snn" , a_terms) == 0)  sprintf (a_show, "str   %s (n, x, y)"    , a_func);
      if (strcmp ("a:nnn" , a_terms) == 0)  sprintf (a_show, "adr   %s (x, y, z)"    , a_func);
      if (strcmp ("t:nnn" , a_terms) == 0)  sprintf (a_show, "t/f   %s (x, y, z)"    , a_func);
      if (strcmp ("?:nnn" , a_terms) == 0)  sprintf (a_show, "v/s   %s (x, y, z)"    , a_func);
      if (strcmp ("?:rsn" , a_terms) == 0)  sprintf (a_show, "v/s   %s (a..o, n, x)" , a_func);
   }
   /*---(quadinary)------------------------*/
   if (x_len == 6) {
      if (strcmp ("s:sssn", a_terms) == 0)  sprintf (a_show, "str   %s (n, m, o, x)"  , a_func);
   }
   /*---(complete)-------------------------*/
   return 0;
}

char         /*-> tbd --------------------------------[ leaf   [fe.C60.32#.F0]*/ /*-[01.0000.013.!]-*/ /*-[--.---.---.--]-*/
CALC__func_oper      (char *a_oper, char *a_terms, char *a_show)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         x_len       =    0;
   /*---(defense)------------------------*/
   --rce;  if (a_oper  == NULL)   return rce;
   --rce;  if (a_terms == NULL)   return rce;
   --rce;  if (a_show  == NULL)   return rce;
   /*---(prepare)--------------------------*/
   sprintf (a_show, "?op?");
   x_len = strlen (a_terms);
   /*---(exceptions)-----------------------*/
   if (x_len == 3 && strcmp (a_oper, "-:") == 0) {
      strcpy (a_show, "num   -x");
      return 0;
   }
   if (x_len == 3 && strcmp (a_oper, "+:") == 0) {
      strcpy (a_show, "num   +x");
      return 0;
   }
   if (x_len == 3 && strcmp (a_oper, ":+") == 0) {
      strcpy (a_show, "num   x++");
      return 0;
   }
   if (x_len == 3 && strcmp (a_oper, ":-") == 0) {
      strcpy (a_show, "num   x--");
      return 0;
   }
   /*---(unary)----------------------------*/
   if (x_len == 3) {
      if (a_terms [0] == 'n')  sprintf (a_show, "num   %sx", a_oper);
      if (a_terms [0] == 't')  sprintf (a_show, "t/f   %sx", a_oper);
      return 0;
   }
   /*---(binary values)--------------------*/
   if (x_len == 4 && a_terms [0] == 'n') {
      sprintf (a_show, "num   x %s y", a_oper);
      return 0;
   }
   /*---(binary logic)---------------------*/
   if (x_len == 4 && a_terms [0] == 't') {
      if (a_terms [2] == 'n')  sprintf (a_show, "t/f   x %s y", a_oper);
      if (a_terms [2] == 's')  sprintf (a_show, "t/f   n %s m", a_oper);
      return 0;
   }
   /*---(binary string)--------------------*/
   if (x_len == 4 && a_terms [0] == 's') {
      sprintf (a_show, "str   n %s m", a_oper);
      return 0;
   }
   /*---(complete)-------------------------*/
   return 0;
}

char         /*-> tbd --------------------------------[ ------ [gc.750.030.51]*/ /*-[03.0000.102.!]-*/ /*-[--.---.---.--]-*/
CALC_func_list       (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   char        x_save      =  '?';
   char       *x_title     = "   ---name---   ret   ---example--------------  ---description------------------------------------";
   char        t           [LEN_DESC];
   int         c           =    0;
   int         d           =    0;
   void       *f           = NULL;
   /*---(header)-------------------------*/
   printf ("gyges_hekatonkheires - cell formula operators and functions\n");
   for (i = 0; i < MAX_FUNCS; ++i) {
      if (g_ycalc_funcs [i].name [0] == 'E')   break;
      if (g_ycalc_funcs [i].fcat != x_save) {
         printf ("\n   %c %s\n", g_ycalc_funcs [i].fcat, s_fcats [CALC__find_fcat (g_ycalc_funcs [i].fcat)].desc);
         printf ("%s\n", x_title);
      }
      if (g_ycalc_funcs [i].f == f)      ++d;
      if (g_ycalc_funcs [i].type == 'f')  CALC__func_func (g_ycalc_funcs [i].name, g_ycalc_funcs [i].terms, t);
      if (g_ycalc_funcs [i].type == 'o')  CALC__func_oper (g_ycalc_funcs [i].name, g_ycalc_funcs [i].terms, t);
      printf ("   %-10.10s   %-30.30s  %-45.50s\n",
            g_ycalc_funcs [i].name, t, g_ycalc_funcs [i].desc );
      x_save = g_ycalc_funcs [i].fcat;
      f = g_ycalc_funcs [i].f;
      ++c;
   }
   printf ("\nend-of-list  %d total operators and functions (%d dups)\n", c, d);
   /*---(sum of short ones)--------------*/
   printf ("\n\nquick access, one-char functions\n");
   for (i = 0; i < MAX_FUNCS; ++i) {
      if (g_ycalc_funcs [i].name [0] == 'E')   break;
      if (g_ycalc_funcs [i].type  != 'f')   continue;
      if (g_ycalc_funcs [i].len   >   1 )   continue;
      if (g_ycalc_funcs [i].type == 'f')  CALC__func_func (g_ycalc_funcs [i].name, g_ycalc_funcs [i].terms, t);
      if (g_ycalc_funcs [i].type == 'o')  CALC__func_oper (g_ycalc_funcs [i].name, g_ycalc_funcs [i].terms, t);
      printf ("   %-10.10s   %-30.30s  %-45.50s\n",
            g_ycalc_funcs [i].name , t, g_ycalc_funcs [i].desc );
   }
   printf ("end of quick list\n\n");
   /*---(complete)-----------------------*/
   exit (1);
}

char
main               (int argv, char **argc)
{
   CALC_func_list ();
   return 0;
}



/*============================----end-of-source---============================*/
