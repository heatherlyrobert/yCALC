/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"



#define  S_ERROR_RANGE    -70



tTERMS      s_terms [MAX_TERM] = {
   { 'v' , "val"    , "numeric or cell address"        },
   { 's' , "str"    , "string or cell address"         },
   { '?' , "v/s"    , "string or numeric (runtime)"    },
   { 'a' , "adr"    , "cell address or pointer"        },
   { 'r' , "rng"    , "range of cells or pointer"      },
   { 't' , "T/F"    , "true or false"                  },
   { '-' , "---"    , "-----"                          },
};

tyCALC_ERROR   yCALC_ERRORS     [100] = {
   { G_ERROR_THING   , 'b', "#.badref"  , "" },
   { G_ERROR_RANGE   , 'b', "#.badrng"  , "" },
   { G_ERROR_DEPEND  , 'b', "#.baddep"  , "" },
   { G_ERROR_TOKEN   , 'b', "#.badtok"  , "" },
   { G_ERROR_UNKNOWN , 'b', "#.unknown" , "" },
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
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(functions)----------------------*/
   DEBUG_PROG   yLOG_note    ("clearing function calls");
   g_who_named = NULL;
   g_who_at    = NULL;
   g_labeler   = NULL;
   g_reaper    = NULL;
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yCALC_build_config      (void *a_who_named, void *a_who_at, void *a_enabler, void *a_labeler, void *a_reaper)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   void       *x_owner     = NULL;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_PROG   yLOG_char    ("status"    , myCALC.status);
   --rce;  if (myCALC.status == NULL || strchr ("IO", myCALC.status) == NULL) {
      DEBUG_PROG   yLOG_note    ("must initialize before configuring");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update who_named)---------------*/
   DEBUG_PROG   yLOG_point   ("who_named" , a_who_named);
   --rce;  if (a_who_named   == NULL) {
      DEBUG_PROG   yLOG_error   ("who_named" , "without this callback, references cannot be resolved");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   g_who_named  = a_who_named;
   /*---(update whois)------------------*/
   DEBUG_PROG   yLOG_point   ("who_at"   , a_who_at);
   --rce;  if (a_who_at    == NULL) {
      DEBUG_PROG   yLOG_error   ("whois"    , "without this callback, range dependencies are not possible");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   g_who_at    = a_who_at;
   /*---(update enabler)-----------------*/
   DEBUG_PROG   yLOG_point   ("enabler"   , a_enabler);
   --rce;  if (a_enabler   == NULL) {
      DEBUG_PROG   yLOG_error   ("enabler"   , "without this callback, no yCALC functionality is possible");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   g_enabler    = a_enabler;
   /*---(update labeler)-----------------*/
   DEBUG_PROG   yLOG_point   ("labeler"    , a_labeler);
   --rce;  if (a_labeler    == NULL) {
      DEBUG_PROG   yLOG_error   ("labeler"    , "without this callback, human-readable verification is not possible");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   g_labeler    = a_labeler;
   /*---(update reaper)-----------------*/
   DEBUG_PROG   yLOG_point   ("reaper"    , a_reaper);
   --rce;  if (a_reaper    == NULL) {
      DEBUG_PROG   yLOG_error   ("reaper"    , "without this callback, zombie collection is not possible");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   g_reaper    = a_reaper;
   /*---(set the dep root)---------------*/
   ycalc_call_who_named ("ROOT", NULL, &myCALC.rroot);
   DEBUG_PROG   yLOG_point   ("rroot"     , myCALC.rroot);
   --rce;  if (myCALC.rroot == NULL) {
      DEBUG_PROG   yLOG_note    ("must create a node named ¸ROOT¸ for yCALC to operate");
      DEBUG_PROG   yLOG_error   ("rroot"     , "without this, all dependencies fail");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update)-------------------------*/
   DEBUG_PROG   yLOG_note    ("updating status");
   myCALC.status_detail [2] = 'b';
   if (myCALC.status_detail [3] == 'e') {
      myCALC.status = 'O';
      myCALC.status_detail [6] = 'o';
   }
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
   char        rc          =    0;
   tCALC      *x_calc      = NULL;
   void       *x_thing     = NULL;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(get previous calc)--------------*/
   x_calc = a_calc->prev;
   DEBUG_CALC   yLOG_point   ("x_calc"    , x_calc);
   if (x_calc    == NULL) {     
      DEBUG_CALC   yLOG_note    ("can not find prev calculation");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, G_ERROR_RANGE);
      return G_ERROR_RANGE;
   }
   /*---(verify type)--------------------*/
   DEBUG_CALC   yLOG_char    ("type"      , x_calc->t);
   if (x_calc->t != G_TYPE_REF) {
      DEBUG_CALC   yLOG_note    ("prev calc not a reference");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, G_ERROR_RANGE);
      return G_ERROR_RANGE;
   }
   /*---(check thing)--------------------*/
   x_thing = x_calc->r;
   DEBUG_CALC   yLOG_point   ("x_thing"   , x_thing);
   if (x_thing   == NULL) {     
      DEBUG_CALC   yLOG_note    ("beginning reference can not be null");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, G_ERROR_RANGE);
      return G_ERROR_RANGE;
   }
   /*---(get address)--------------------*/
   rc = g_addresser (x_thing, x, y, z);
   DEBUG_CALC   yLOG_point   ("rc"        , rc);
   if (rc  < 0) {     
      DEBUG_CALC   yLOG_note    ("not a valid address");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, G_ERROR_RANGE);
      return G_ERROR_RANGE;
   }
   /*---(report out)---------------------*/
   DEBUG_CALC   yLOG_complex ("address"   , "%4dx, %4dy, %4dz", x, y, z);
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
ycalc__build_prepare    (tDEP_ROOT *a_deproot, char *a_rpn)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tCALC      *x_curr      = NULL;
   tCALC      *x_next      = NULL;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CALC   yLOG_char    ("status"    , myCALC.status);
   --rce;  if (myCALC.status != 'O') {
      DEBUG_PROG   yLOG_note    ("must initialize and configure before use");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_point   ("a_deproot" , a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_CALC   yLOG_note    ("a_deproot not set, nothing to do");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_point   ("a_rpn"     , a_rpn);
   --rce;  if (a_rpn == NULL) {
      DEBUG_CALC   yLOG_note    ("rpn not available, nothing to do");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_info    ("a_rpn"     , a_rpn);
   /*---(clear the calculation)----------*/
   rc = ycalc_calc_wipe  (a_deproot);
   DEBUG_CALC   yLOG_value   ("calc"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(clear the dependencies)---------*/
   rc = ycalc_deps_wipe  (a_deproot);
   DEBUG_CALC   yLOG_value   ("cleanser"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(copy the rpn)-------------------*/
   a_deproot->rpn = strndup (a_rpn, LEN_RECD);
   /*---(complete)-------------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

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
   /*---(check for range operator)-------*/
   DEBUG_CALC   yLOG_note    ("check for range operator");
   if (strcmp (a_token, "..") != 0)    return 0;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(get starting point)-------------*/
   rc = ycalc__build_coords  (a_calc->prev, &x_beg, &y_beg, &z_beg);
   DEBUG_CALC   yLOG_value   ("rc"        , rc);
   if (rc != 0) {     
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(get ending point)---------------*/
   rc = ycalc__build_coords  (a_calc->prev->prev, &x_beg, &y_beg, &z_beg);
   DEBUG_CALC   yLOG_value   ("rc"        , rc);
   if (rc != 0) {     
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(set dependencies)---------------*/
   DEBUG_CALC   yLOG_note    ("set dedendency");
   rc = yCALC_range (a_deproot, x_beg, y_beg, z_beg, x_end, y_end, z_end);
   DEBUG_CALC   yLOG_value   ("rc"        , rc);
   if (rc <  0) {     
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(update type)--------------------*/
   DEBUG_CALC   yLOG_note    ("mark type");
   a_calc->t = G_TYPE_NOOP;
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
ycalc__build_pointer    (tDEP_ROOT *a_thing, tCALC *a_calc, char *a_token)
{
   return 0;
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
      /*> DEBUG_CALC   yLOG_complex ("check"     , "%3d, %-12.12s, %3d, %-10.10p   vs %-12.12s, %3d", i, g_funcs[i].n, g_funcs[i].l, g_funcs[i].f, a_token, len);   <*/
      /*---(check for end)---------------*/
      if (g_funcs [i].f == NULL)                      break;
      if (g_funcs [i].len == 0   )                    break;
      /*---(check for length)------------*/
      if (x_len  != g_funcs [i].len)                  continue;
      /*---(check for name)--------------*/
      if (a_token [0] != g_funcs[i].name [0])              continue;
      if (strcmp (g_funcs[i].name, a_token) != 0)        continue;
      /*---(found)-----------------------*/
      DEBUG_CALC   yLOG_complex ("found it"  , "%3d, %-12.12s, %3d, %-10.10p   vs %-12.12s, %3d", i, g_funcs[i].name, g_funcs[i].len, g_funcs[i].f, a_token, x_len);
      a_calc->f = g_funcs[i].f;
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
   void       *x_ref       = NULL;
   /*---(check for reference)------------*/
   DEBUG_CALC   yLOG_note    ("look for reference");
   ycalc_call_who_named (a_token, NULL, &x_ref);
   if (x_ref == NULL)     return 0;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(check reference)----------------*/
   DEBUG_CALC   yLOG_point   ("x_ref"     , x_ref);
   if (x_ref == NULL) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, G_ERROR_THING);
      return G_ERROR_THING;
   }
   a_calc->r = x_ref;
   /*---(set dependency)-----------------*/
   rc = ycalc_deps_create (G_DEP_REQUIRE, a_deproot, x_ref);
   DEBUG_CALC   yLOG_value   ("rc"        , rc);
   if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, G_ERROR_DEPEND);
      return G_ERROR_DEPEND;
   }
   /*---(update type)-----------------*/
   DEBUG_CALC   yLOG_note    ("mark type");
   a_calc->t = G_TYPE_REF;
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
   /*---(try al types)--------------------*/
   if (rc == 0)  rc = ycalc__build_char      (a_deproot, x_calc, a_token);
   if (rc == 0)  rc = ycalc__build_string    (a_deproot, x_calc, a_token);
   if (rc == 0)  rc = ycalc__build_range     (a_deproot, x_calc, a_token);
   if (rc == 0)  rc = ycalc__build_pointer   (a_deproot, x_calc, a_token);
   if (rc == 0)  rc = ycalc__build_function  (a_deproot, x_calc, a_token);
   if (rc == 0)  rc = ycalc__build_value     (a_deproot, x_calc, a_token);
   if (rc == 0)  rc = ycalc__build_reference (a_deproot, x_calc, a_token);
   if (rc == 0)  rc = G_ERROR_TOKEN;
   /*
    *   what about characters, should we catch that separately ?
    */
   /*---(report out)-----------------------*/
   if (rc == 1) {
      DEBUG_CALC   yLOG_complex ("element"   , "typ=%c, val=%F, str=%-9p, ref=%-9p, fnc=%-9p", x_calc->t, x_calc->v, x_calc->s, x_calc->r, x_calc->f);
   }
   /*---(complete)-------------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                          main driver                         ----===*/
/*====================------------------------------------====================*/
static void  o___DRIVER__________o () { return; }

char
yCALC_build             (char *a_label, char *a_rpn, char **a_notice)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tDEP_ROOT  *x_deproot   = NULL;
   char        x_work      [LEN_RECD];       /* working copy of source string  */
   char       *p;                           /* strtok current pointer         */
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   rc = ycalc_call_who_named (a_label, NULL, &x_deproot);
   DEBUG_CALC   yLOG_value   ("who named" , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_point   ("x_deproot" , x_deproot);
   --rce;  if (x_deproot == NULL) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = ycalc__build_prepare (x_deproot, a_rpn);
   DEBUG_CALC   yLOG_value   ("prepare"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check for cleanse only)-------------*/
   if (a_rpn [0] == NULL) {
      DEBUG_CALC   yLOG_note    ("empty rpn, cleansed and returned");
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(initialize)-------------------------*/
   strncpy (x_work, a_rpn, LEN_RECD);
   DEBUG_CALC   yLOG_info    ("x_work"    , x_work);
   p = ycalc__build_strtok (x_work);
   --rce;  if (p == NULL) {
      DEBUG_CALC   yLOG_note    ("could not tokenize rpn");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(process tokens)---------------------*/
   while (p != NULL) {
      DEBUG_CALC   yLOG_info    ("token"     , p);
      rc = ycalc__build_step (x_deproot, p);
      if (rc != 1)   break;
      p = ycalc__build_strtok (NULL);
   }
   /*---(check for failure)----------------*/
   if (a_notice != NULL) {
      strlcpy (a_notice, "", LEN_RECD);
      if (rc != 1) {
         switch (rc) {
         case G_ERROR_RANGE  :
            strlcpy (a_notice, "#.badrng" , LEN_RECD);
            break;
         case G_ERROR_THING  :
            strlcpy (a_notice, "#.badref" , LEN_RECD);
            break;
         case G_ERROR_DEPEND :
            strlcpy (a_notice, "#.baddep" , LEN_RECD);
            break;
         case G_ERROR_TOKEN  :
            strlcpy (a_notice, "#.badtok" , LEN_RECD);
            break;
         case G_ERROR_UNKNOWN:
            strlcpy (a_notice, "#.unknown", LEN_RECD);
            break;
         }
      }
   }
   --rce;  if (rc != 1) {
      DEBUG_CALC   yLOG_note    ("could not build calculation chain");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-------------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}


