/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"



#define  S_ERROR_RANGE    -70



tTERMS      s_terms [MAX_TERM] = {
   { 'n' , "num"    , "numeric or cell address"        },
   { 's' , "str"    , "string or cell address"         },
   { 't' , "t/f"    , "boolean (true or false)"        },
   { '?' , "v/s"    , "string or numeric (runtime)"    },
   { 'a' , "addr"   , "cell address or pointer"        },
   { 'r' , "range"  , "range of cells or pointer"      },
   { '-' , "---"    , "-----"                          },
};


/*====================------------------------------------====================*/
/*===----                        program level                         ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char
ycalc_build_init        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   char        t           [LEN_LABEL];
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       memory management                      ----===*/
/*====================------------------------------------====================*/
static void  o___MALLOC__________o () { return; }

tCALC*       /*-> create a new calculation entry -----[ leaf   [gp.740.222.40]*/ /*-[11.0000.018.!]-*/ /*-[--.---.---.--]-*/
ycalc__build_new        (tDEP_ROOT *a_deproot)
{
   /*---(locals)-----------+-----+-----+-*/
   tCALC      *x_new       = NULL;          /* current calculation element    */
   char        x_tries     =    0;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_senter  (__FUNCTION__);
   /*---(allocate)-----------------------*/
   while (x_new == NULL && x_tries < 10)  {
      ++x_tries;
      x_new = (tCALC *) malloc (sizeof (tCALC));
   }
   DEBUG_CALC   yLOG_spoint  (x_new);
   if (x_new == NULL) {
      DEBUG_CALC   yLOG_snote   ("FAILED");
      DEBUG_CALC   yLOG_sexit   (__FUNCTION__);
      return  NULL;
   }
   /*---(set owner)----------------------*/
   DEBUG_CALC   yLOG_snote   ("deproot");
   x_new->deproot = a_deproot;
   /*---(set to defaults)----------------*/
   DEBUG_CALC   yLOG_snote   ("init");
   x_new->t       = '-';
   x_new->v       = 0.0;
   x_new->s       = NULL;
   x_new->r       = NULL;
   x_new->f       = NULL;
   /*---(tie into deproot->calc)---------*/
   DEBUG_CALC   yLOG_snote   ("link");
   x_new->next    = NULL;
   x_new->prev    = NULL;
   if (a_deproot->chead == NULL) {
      a_deproot->chead        = x_new;
   } else {
      a_deproot->ctail->next  = x_new;
      x_new->prev             = a_deproot->ctail;
   }
   a_deproot->ctail  = x_new;
   /*---(update counter)-----------------*/
   DEBUG_CALC   yLOG_snote   ("counter");
   ++a_deproot->ncalc;
   DEBUG_CALC   yLOG_sint    (a_deproot->ncalc);
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_sexit   (__FUNCTION__);
   return x_new;
}



/*====================------------------------------------====================*/
/*===----                       support functions                      ----===*/
/*====================------------------------------------====================*/
static void  o___SUPPORT_________o () { return; }

char*        /*-> parse a calculation string ---------[ leaf   [gs.D80.166.A0]*/ /*-[03.0000.018.I]-*/ /*-[--.---.---.--]-*/
ycalc__build_strtok     (char *a_str)
{
   /*---(locals)-----------+-----------+-*/
   static char *x_str       = NULL;
   static int  x_pos       = 0;
   static int  x_next      = 0;
   static int  x_len       = 0;
   static char x_lit       = '-';
   int         i           = 0;
   /*---(defense)------------------------*/
   if (a_str == NULL && x_str == NULL) return NULL;
   /*---(start new string)---------------*/
   if (a_str != NULL) {
      /*> printf ("CALC_strtok : new string <<%s>>\n", a_str);                        <*/
      x_str   = a_str;
      x_pos   = 0;
      x_next  = 0;
      x_len   = strllen (a_str, LEN_RECD);
      x_lit   = '-';
      /*> printf ("   -- a_str = %p\n", a_str);                                       <*/
      /*> printf ("   -- x_str = %p <<%s>>\n", x_str, x_str);                         <*/
   }
   /*---(or, continue)-------------------*/
   if (a_str == NULL) {
      x_pos  = x_next;
      /*> printf ("CALC_strtok : existing string <<%s>>\n", x_str + x_pos);           <*/
   }
   /*---(defense at end)-----------------*/
   if (x_pos >= x_len)                 return NULL;
   if (x_str + x_pos == '\0')          return NULL;
   /*---(search for delimiter)-----------*/
   for (i = x_pos; i < x_len; ++i) {
      /*> printf ("   -- looking at %3d %c : ", i, x_str[i]);                         <*/
      /*---(quoted strings)--------------*/
      if (x_str [i] == '"' )  {
         if (x_lit == 'y') {
            /*> printf ("exiting quote,  ");                                          <*/
            x_lit = '-';
         }
         else {
            /*> printf ("entering quote, ");                                          <*/
            x_lit = 'y';
         }
      }
      if (x_lit == 'y') {
         /*> printf ("next\n");                                                       <*/
         continue;
      }
      if (x_str [i] == ',') {
         x_str [i] = '\0';
         x_next = i + 1;
         /*> printf ("COMMA, x_next = %3d, return %p\n", x_str + x_pos);              <*/
         return x_str + x_pos;
      }
      /*> printf ("next\n");                                                          <*/
   }
   /*---(complete)-----------------------*/
   x_next = x_len;
   return x_str + x_pos;
}

char
ycalc__build_coords     (tCALC *a_calc, int *x, int *y, int *z)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tCALC      *x_calc      = NULL;
   tDEP_ROOT  *x_deproot   = NULL;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(get previous calc)--------------*/
   x_calc = a_calc;
   DEBUG_CALC   yLOG_point   ("x_calc"    , x_calc);
   --rce; if (x_calc    == NULL) {     
      DEBUG_CALC   yLOG_note    ("can not find prev calculation");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(verify type)--------------------*/
   DEBUG_CALC   yLOG_char    ("type"      , x_calc->t);
   if (x_calc->t != G_TYPE_REF) {
      DEBUG_CALC   yLOG_note    ("prev calc not a reference");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, G_ERROR_RANGE);
      return rce;
      return G_ERROR_RANGE;
   }
   /*---(check thing)--------------------*/
   x_deproot = x_calc->r;
   DEBUG_CALC   yLOG_point   ("x_deproot"   , x_deproot);
   if (x_deproot   == NULL) {     
      DEBUG_CALC   yLOG_note    ("beginning reference can not be null");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, G_ERROR_RANGE);
      return G_ERROR_RANGE;
   }
   /*---(get address)--------------------*/
   rc = g_addresser (x_deproot->owner, x, y, z);
   DEBUG_CALC   yLOG_value   ("addresser" , rc);
   if (rc  < 0) {     
      DEBUG_CALC   yLOG_note    ("not a valid address");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, G_ERROR_RANGE);
      return G_ERROR_RANGE;
   }
   /*---(report out)---------------------*/
   DEBUG_CALC   yLOG_complex ("address"   , "%4dx, %4dy, %4dz", *x, *y, *z);
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc_calc_wipe         (tDEP_ROOT *a_deproot)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tCALC      *x_curr      = NULL;
   tCALC      *x_next      = NULL;
   /*---(filter)-------------------------*/
   if (a_deproot == NULL)  return 0;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_senter  (__FUNCTION__);
   /*---(clear the calculation)----------*/
   DEBUG_CALC   yLOG_spoint  (a_deproot->chead);
   x_curr = a_deproot->chead;
   while (x_curr != NULL) {
      x_next = x_curr->next;
      if (x_curr->s  != NULL)  free (x_curr->s);
      free (x_curr);
      x_curr = x_next;
      DEBUG_CALC   yLOG_spoint  (x_curr);
   }
   /*---(clear deproot vars)-------------*/
   DEBUG_CALC   yLOG_snote   ("deproot vars");
   a_deproot->chead = NULL;
   a_deproot->ctail = NULL;
   a_deproot->ncalc = 0;
   a_deproot->cuse  = 0;
   /*---(rpn)----------------------------*/
   DEBUG_CALC   yLOG_spoint  (a_deproot->rpn);
   if (a_deproot->rpn != NULL)  free (a_deproot->rpn);
   a_deproot->rpn = NULL;
   DEBUG_CALC   yLOG_spoint  (a_deproot->rpn);
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     build process steps                      ----===*/
/*====================------------------------------------====================*/
static void  o___STEPS___________o () { return; }

char
ycalc__build_char       (tDEP_ROOT *a_deproot, tCALC *a_calc, char *a_token)
{
   /*---(locals)-----------+-----+-----+-*/
   char        x_len       =    0;
   /*---(check for string)---------------*/
   DEBUG_CALC   yLOG_note    ("check for character");
   x_len = strllen (a_token, LEN_RECD);
   if (a_token [0]         != '\'')  return  0;
   if (a_token [2]         != '\'')  return  0;
   if (x_len !=  3)                  return  0;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(parse string)--------------------*/
   a_calc->v = a_token [1];
   DEBUG_CALC   yLOG_double  ("value"     , a_calc->v);
   /*---(update type)--------------------*/
   DEBUG_CALC   yLOG_note    ("mark type");
   a_calc->t = G_TYPE_VAL;
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
ycalc__build_string     (tDEP_ROOT *a_deproot, tCALC *a_calc, char *a_token)
{
   /*---(locals)-----------+-----+-----+-*/
   char        x_len       =    0;
   /*---(check for string)---------------*/
   DEBUG_CALC   yLOG_note    ("check for literal string");
   x_len = strllen (a_token, LEN_RECD);
   if (a_token [0]         != '"')  return  0;
   if (a_token [x_len - 1] != '"')  return  0;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(parse string)--------------------*/
   a_token [x_len - 1] = NULL;
   a_calc->s = strndup (a_token + 1, LEN_RECD);
   DEBUG_CALC   yLOG_info    ("string"    , a_calc->s);
   /*---(update type)--------------------*/
   DEBUG_CALC   yLOG_note    ("mark type");
   a_calc->t = G_TYPE_STR;
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
ycalc__build_range      (tDEP_ROOT *a_deproot, tCALC *a_calc, char *a_token)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   char        x_good      =  '-';
   int         x_beg, y_beg, z_beg;
   int         x_end, y_end, z_end;
   int         a;
   tDEP_ROOT  *x_range     = NULL;
   tDEP_ROOT  *x_temp      = NULL;
   void       *x_owner     = NULL;
   /*---(check for range operator)-------*/
   DEBUG_CALC   yLOG_note    ("check for range operator");
   if (strcmp (a_token, "..") != 0)    return 0;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(get starting point)-------------*/
   rc = ycalc__build_coords  (a_calc->prev->prev, &x_beg, &y_beg, &z_beg);
   DEBUG_CALC   yLOG_value   ("rc"        , rc);
   if (rc != 0) {     
      rc = YCALC_ERROR_BUILD_RNG;
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(get ending point)---------------*/
   rc = ycalc__build_coords  (a_calc->prev      , &x_end, &y_end, &z_end);
   DEBUG_CALC   yLOG_value   ("rc"        , rc);
   if (rc != 0) {     
      rc = YCALC_ERROR_BUILD_RNG;
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(delete first ref)---------------*/
   x_owner = a_calc->prev->prev->r->owner;
   rc = ycalc_deps_delete (G_DEP_REQUIRE, &a_deproot, &(a_calc->prev->prev->r), &x_owner);
   if (rc <  0) {     
      rc = YCALC_ERROR_BUILD_DEP;
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   a_calc->prev->prev->t = G_TYPE_NOOP;
   /*> ycalc_call_who_at (x_beg, y_beg, z_beg, YCALC_LOOK, NULL, &x_temp);            <*/
   /*> ycalc_call_reaper (NULL, &x_temp);                                             <*/
   /*---(delete second ref)--------------*/
   x_owner = a_calc->prev->r->owner;
   rc = ycalc_deps_delete (G_DEP_REQUIRE, &a_deproot, &(a_calc->prev->r), &x_owner);
   if (rc <  0) {     
      rc = YCALC_ERROR_BUILD_DEP;
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   a_calc->prev->t = G_TYPE_NOOP;
   /*> ycalc_call_who_at (x_end, y_end, z_end, YCALC_LOOK, NULL, &x_temp);            <*/
   /*> ycalc_call_reaper (NULL, &x_temp);                                             <*/
   /*---(set dependencies)---------------*/
   DEBUG_CALC   yLOG_note    ("set dedendencies");
   DEBUG_DEPS   yLOG_complex ("range"     , "bx=%4d, ex=%4d, by=%4d, ey=%4d, z=%4d", x_beg, x_end, y_beg, y_end, z_beg);
   rc = ycalc_range_use (a_deproot, x_beg, x_end, y_beg, y_end, z_beg, z_end, &x_range);
   DEBUG_CALC   yLOG_value   ("rc"        , rc);
   if (rc <  0) {     
      rc = YCALC_ERROR_BUILD_DEP;
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(update types)-------------------*/
   DEBUG_CALC   yLOG_note    ("mark type");
   a_calc->t = G_TYPE_REF;
   a_calc->r = x_range;
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
ycalc__build_function   (tDEP_ROOT *a_deproot, tCALC *a_calc, char *a_token)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         x_len       =    0;
   int         i           =    0;
   /*> char       *x_valid     = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";   <*/
   /*---(check for function)-------------*/
   DEBUG_CALC   yLOG_note    ("check for function");
   x_len = strlen (a_token);
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(locate function)----------------*/
   for (i = 0; i < MAX_FUNCS; ++i) {
      /*> DEBUG_CALC   yLOG_complex ("check"     , "%3d, %-12.12s, %3d, %-10.10p   vs %-12.12s, %3d", i, g_ycalc_funcs[i].n, g_ycalc_funcs[i].l, g_ycalc_funcs[i].f, a_token, len);   <*/
      /*---(check for end)---------------*/
      if (g_ycalc_funcs [i].f == NULL)                      break;
      if (g_ycalc_funcs [i].len == 0   )                    break;
      /*---(check for length)------------*/
      if (x_len  != g_ycalc_funcs [i].len)                  continue;
      /*---(check for name)--------------*/
      if (a_token [0] != g_ycalc_funcs[i].name [0])              continue;
      if (strcmp (g_ycalc_funcs[i].name, a_token) != 0)        continue;
      /*---(found)-----------------------*/
      DEBUG_CALC   yLOG_complex ("found it"  , "%3d, %-12.12s, %3d, %-10.10p   vs %-12.12s, %3d", i, g_ycalc_funcs[i].name, g_ycalc_funcs[i].len, g_ycalc_funcs[i].f, a_token, x_len);
      a_calc->f = g_ycalc_funcs[i].f;
      /*---(update type)-----------------*/
      DEBUG_CALC   yLOG_note    ("mark type");
      a_calc->t = G_TYPE_FUNC;
      /*---(complete)--------------------*/
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(fall-through)-------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc__build_reference  (tDEP_ROOT *a_deproot, tCALC *a_calc, char *a_token)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   tDEP_ROOT  *x_ref       = NULL;
   int         x, y, z;
   /*---(check for reference)------------*/
   DEBUG_CALC   yLOG_note    ("look for reference");
   rc = ycalc_call_who_named (a_token, YCALC_FULL, NULL, &x_ref);
   if (rc < 0)  return 0;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(check reference)----------------*/
   DEBUG_CALC   yLOG_point   ("x_ref"     , x_ref);
   if (x_ref == NULL) {
      rc = YCALC_ERROR_BUILD_REF;
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   a_calc->r = x_ref;
   /*---(set dependency)-----------------*/
   rc = ycalc_deps_create (G_DEP_REQUIRE, &a_deproot, &x_ref);
   DEBUG_CALC   yLOG_value   ("rc"        , rc);
   if (rc < 0) {
      rc = YCALC_ERROR_BUILD_DEP;
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(update type)-----------------*/
   DEBUG_CALC   yLOG_note    ("mark type");
   a_calc->t = G_TYPE_REF;
   /*---(ranges)-------------------------*/
   rc = g_addresser (x_ref->owner, &x, &y, &z);
   DEBUG_CALC   yLOG_value   ("addresser" , rc);
   if (rc < 0) {
      rc = YCALC_ERROR_BUILD_DEP;
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rc);
   }
   rc = ycalc_range_include (x_ref, x, y, z);
   DEBUG_CALC   yLOG_value   ("ranges"    , rc);
   /*---(complete)--------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
ycalc__build_value      (tDEP_ROOT *a_thing, tCALC *a_calc, char *a_token)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         x_len       =    0;
   int         i           =    0;
   char       *x_valid     = "0123456789.-+";    /* only digits               */
   /*---(check for function)-------------*/
   DEBUG_CALC   yLOG_note    ("check for value");
   /*---(check for name quality)---------*/
   x_len = strlen (a_token);
   for (i = 0; i < x_len; ++i) {
      if (strchr (x_valid, a_token [i]) != NULL)  continue;
      return 0;
   }
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   a_calc->v = atof (a_token);
   DEBUG_CALC   yLOG_double  ("value"     , a_calc->v);
   /*---(update type)-----------------*/
   DEBUG_CALC   yLOG_note    ("mark type");
   a_calc->t = G_TYPE_VAL;
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
ycalc__build_step       (tDEP_ROOT *a_deproot, char *a_token)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   tCALC      *x_calc      = NULL;          /* current calculation element    */
   tCALC      *x_tail      = NULL;          /* last calculation element       */
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(create and null)-----------------*/
   DEBUG_CALC   yLOG_note    ("allocate calc entry");
   x_calc   = ycalc__build_new (a_deproot);
   DEBUG_CALC   yLOG_value   ("calc step" , a_deproot->ncalc);
   /*---(try al types)--------------------*/
   if (rc == 0)  rc = ycalc__build_char      (a_deproot, x_calc, a_token);
   if (rc == 0)  rc = ycalc__build_string    (a_deproot, x_calc, a_token);
   if (rc == 0)  rc = ycalc__build_range     (a_deproot, x_calc, a_token);
   if (rc == 0)  rc = ycalc__build_function  (a_deproot, x_calc, a_token);
   if (rc == 0)  rc = ycalc__build_value     (a_deproot, x_calc, a_token);
   if (rc == 0)  rc = ycalc__build_reference (a_deproot, x_calc, a_token);
   if (rc == 0)  rc = YCALC_ERROR_BUILD_TOK;
   /*---(report out)-----------------------*/
   if (rc == 1)  DEBUG_CALC   yLOG_complex ("element"   , "typ=%c, val=%F, str=%-9p, ref=%-9p, fnc=%-9p", x_calc->t, x_calc->v, x_calc->s, x_calc->r, x_calc->f);
   /*---(complete)-------------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                          main driver                         ----===*/
/*====================------------------------------------====================*/
static void  o___DRIVER__________o () { return; }

char
ycalc_build_trusted     (tDEP_ROOT *a_deproot, char **a_source, char *a_type, double *a_value, char **a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_rpn       [LEN_RECD];
   int         x_nrpn      =    0;
   char        x_work      [LEN_RECD];       /* working copy of source string  */
   char       *p           = NULL;           /* strtok current pointer         */
   int         x_len       =    0;
   char        t           [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   DEBUG_CALC   yLOG_point   ("a_deproot" , a_deproot);
   /*---(defense)------------------------*/
   DEBUG_CALC   yLOG_char    ("*a_type"   , *a_type);
   DEBUG_CALC   yLOG_info    ("valid"     , YCALC_GROUP_RPN);
   --rce;  if (strchr (YCALC_GROUP_RPN, *a_type) == NULL) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(length)-------------------------*/
   x_len = strllen (*a_source, LEN_RECD);
   --rce;  if (x_len <= 1) {
      ycalc_handle_error (YCALC_ERROR_BUILD_RPN , a_type, a_value, a_string, "empty");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(generate rpn)-------------------*/
   rc = yRPN_interpret (*a_source, &x_rpn, &x_nrpn, LEN_RECD, 0);
   DEBUG_CALC   yLOG_value   ("rc"        , rc);
   DEBUG_CALC   yLOG_value   ("x_nrpn"    , x_nrpn);
   DEBUG_CALC   yLOG_point   ("x_rpn"     , x_rpn);
   --rce;  if (x_nrpn <= 0 || x_rpn == NULL) {
      if (rc < 0) {
         sprintf (t, "pos %d", yRPN_errorpos ());
         ycalc_handle_error (YCALC_ERROR_BUILD_RPN , a_type, a_value, a_string, t);
      } else {
         ycalc_handle_error (YCALC_ERROR_BUILD_RPN , a_type, a_value, a_string, "failed");
      }
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save the rpn)-------------------*/
   a_deproot->rpn = strndup (x_rpn, LEN_RECD);
   DEBUG_CALC   yLOG_info    ("rpn"       , a_deproot->rpn);
   /*---(initialize)-------------------------*/
   p = ycalc__build_strtok (x_rpn);
   --rce;  if (p == NULL) {
      ycalc_handle_error (YCALC_ERROR_BUILD_RPN , a_type, a_value, a_string, "empty");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(process tokens)---------------------*/
   while (p != NULL) {
      DEBUG_CALC   yLOG_info    ("token"     , p);
      rc = ycalc__build_step (a_deproot, p);
      if (rc != 1) {
         ycalc_handle_error (rc, a_type, a_value, a_string, p);
         DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      p = ycalc__build_strtok (NULL);
   }
   /*---(complete)-------------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc_build_detail      (void *a_owner, tDEP_ROOT *a_deproot, char **a_source, char *a_type, double *a_value, char **a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;           /* return code for errors         */
   char        rc          =    0;           /* return code for errors         */
   char        x_label     [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (ycalc_not_ready ()) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(setup)--------------------------*/
   DEBUG_CALC   yLOG_point   ("a_owner"   , a_owner);
   DEBUG_CALC   yLOG_point   ("a_deproot" , a_deproot);
   /*---(prepare)------------------------*/
   rc = ycalc_shared_verify (a_source, a_type, a_value, a_string);
   DEBUG_CALC   yLOG_value   ("verify"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ycalc_build_trusted (a_deproot, a_source, a_type, a_value, a_string);
   DEBUG_CALC   yLOG_value   ("trusted"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc_build_owner       (void *a_owner, tDEP_ROOT *a_deproot)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;           /* return code for errors         */
   char        rc          =    0;           /* return code for errors         */
   char      **x_source    = NULL;
   char       *x_type      = NULL;
   double     *x_value     = NULL;
   char      **x_string    = NULL;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (ycalc_not_ready ()) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(defense)------------------------*/
   DEBUG_CALC   yLOG_point   ("a_owner"   , a_owner);
   --rce;  if (a_owner == NULL) {
      DEBUG_CALC   yLOG_note    ("a_owner not set, nothing to do");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_point   ("a_deproot" , a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_CALC   yLOG_note    ("a_deproot not set, nothing to do");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(fill pointers)------------------*/
   rc = g_pointer (a_owner, &x_source, &x_type, &x_value, &x_string);
   DEBUG_CALC   yLOG_value   ("pointer"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ycalc_build_detail (a_owner, a_deproot, x_source, x_type, x_value, x_string);
   DEBUG_CALC   yLOG_value   ("detail"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc_build_label       (char *a_label)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;           /* return code for errors         */
   char        rc          =    0;           /* return code for errors         */
   void       *x_owner     = NULL;
   tDEP_ROOT  *x_deproot   = NULL;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (ycalc_not_ready ()) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get owner/deproot)--------------*/
   rc = g_who_named  (a_label, YCALC_OWNR, &x_owner, &x_deproot);
   DEBUG_CALC   yLOG_value   ("who_named"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(call classify)------------------*/
   rc = ycalc_build_owner (x_owner, x_deproot);
   DEBUG_CALC   yLOG_value   ("classify"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}



