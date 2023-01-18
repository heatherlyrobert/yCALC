/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"



#define  S_ERROR_RANGE    -70




/*====================------------------------------------====================*/
/*===----                        program level                         ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char
ycalc_build_init        (void)
{
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
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
   DEBUG_YCALC   yLOG_senter  (__FUNCTION__);
   /*---(allocate)-----------------------*/
   while (x_new == NULL && x_tries < 10)  {
      ++x_tries;
      /*> printf ("malloc () ycalc__build_new\n");                                    <*/
      x_new = (tCALC *) malloc (sizeof (tCALC));
   }
   DEBUG_YCALC   yLOG_spoint  (x_new);
   if (x_new == NULL) {
      DEBUG_YCALC   yLOG_snote   ("FAILED");
      DEBUG_YCALC   yLOG_sexit   (__FUNCTION__);
      return  NULL;
   }
   /*---(set owner)----------------------*/
   DEBUG_YCALC   yLOG_snote   ("deproot");
   x_new->deproot = a_deproot;
   /*---(set to defaults)----------------*/
   DEBUG_YCALC   yLOG_snote   ("init");
   x_new->t       = '-';
   x_new->v       = 0.0;
   x_new->s       = NULL;
   x_new->r       = NULL;
   x_new->f       = NULL;
   /*---(tie into deproot->calc)---------*/
   DEBUG_YCALC   yLOG_snote   ("link");
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
   DEBUG_YCALC   yLOG_snote   ("counter");
   ++a_deproot->ncalc;
   DEBUG_YCALC   yLOG_sint    (a_deproot->ncalc);
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_sexit   (__FUNCTION__);
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
ycalc__build_coords     (tCALC *a_calc, int *b, int *x, int *y, int *z)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tCALC      *x_calc      = NULL;
   tDEP_ROOT  *x_deproot   = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(get previous calc)--------------*/
   x_calc = a_calc;
   DEBUG_YCALC   yLOG_point   ("x_calc"    , x_calc);
   --rce; if (x_calc    == NULL) {     
      DEBUG_YCALC   yLOG_note    ("can not find prev calculation");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(verify type)--------------------*/
   DEBUG_YCALC   yLOG_char    ("type"      , x_calc->t);
   if (x_calc->t != G_TYPE_REF) {
      DEBUG_YCALC   yLOG_note    ("prev calc not a reference");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, YCALC_ERROR_BUILD_RNG);
      return rce;
      return YCALC_ERROR_BUILD_RNG;
   }
   /*---(check thing)--------------------*/
   x_deproot = x_calc->r;
   DEBUG_YCALC   yLOG_point   ("x_deproot"   , x_deproot);
   if (x_deproot   == NULL) {     
      DEBUG_YCALC   yLOG_note    ("beginning reference can not be null");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, YCALC_ERROR_BUILD_RNG);
      return YCALC_ERROR_BUILD_RNG;
   }
   /*---(get address)--------------------*/
   rc = myCALC.e_addresser (x_deproot->owner, b, x, y, z);
   DEBUG_YCALC   yLOG_value   ("addresser" , rc);
   if (rc  < 0) {     
      DEBUG_YCALC   yLOG_note    ("not a valid address");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, YCALC_ERROR_BUILD_RNG);
      return YCALC_ERROR_BUILD_RNG;
   }
   /*---(report out)---------------------*/
   DEBUG_YCALC   yLOG_complex ("address"   , "%4db, %4dx, %4dy, %4dz", *b, *x, *y, *z);
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
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
   DEBUG_YCALC   yLOG_senter  (__FUNCTION__);
   /*---(clear the calculation)----------*/
   DEBUG_YCALC   yLOG_spoint  (a_deproot->chead);
   x_curr = a_deproot->chead;
   while (x_curr != NULL) {
      x_next = x_curr->next;
      if (x_curr->s  != NULL)  free (x_curr->s);
      x_curr->s  == NULL;
      free (x_curr);
      x_curr = x_next;
      DEBUG_YCALC   yLOG_spoint  (x_curr);
   }
   /*---(clear deproot vars)-------------*/
   DEBUG_YCALC   yLOG_snote   ("deproot vars");
   a_deproot->chead = NULL;
   a_deproot->ctail = NULL;
   a_deproot->ncalc = 0;
   a_deproot->cuse  = 0;
   /*---(rpn)----------------------------*/
   DEBUG_YCALC   yLOG_spoint  (a_deproot->rpn);
   if (a_deproot->rpn != NULL)  free (a_deproot->rpn);
   a_deproot->rpn  = NULL;
   a_deproot->nrpn = 0;
   DEBUG_YCALC   yLOG_spoint  (a_deproot->rpn);
   DEBUG_YCALC   yLOG_sint    (a_deproot->nrpn);
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_sexit   (__FUNCTION__);
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
   DEBUG_YCALC   yLOG_note    ("check for character");
   x_len = strllen (a_token, LEN_RECD);
   if (a_token [0]         != '\'')  return  0;
   if (a_token [2]         != '\'')  return  0;
   if (x_len !=  3)                  return  0;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(parse string)--------------------*/
   a_calc->v = a_token [1];
   DEBUG_YCALC   yLOG_double  ("value"     , a_calc->v);
   /*---(update type)--------------------*/
   DEBUG_YCALC   yLOG_note    ("mark type");
   a_calc->t = G_TYPE_VAL;
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
ycalc__build_string     (tDEP_ROOT *a_deproot, tCALC *a_calc, char *a_token)
{
   /*---(locals)-----------+-----+-----+-*/
   char        x_len       =    0;
   /*---(check for string)---------------*/
   DEBUG_YCALC   yLOG_note    ("check for literal string");
   x_len = strllen (a_token, LEN_RECD);
   if (a_token [0]         != '"')  return  0;
   if (a_token [x_len - 1] != '"')  return  0;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(parse string)--------------------*/
   a_token [x_len - 1] = NULL;
   a_calc->s = strndup (a_token + 1, LEN_RECD);
   DEBUG_YCALC   yLOG_info    ("string"    , a_calc->s);
   /*---(update type)--------------------*/
   DEBUG_YCALC   yLOG_note    ("mark type");
   a_calc->t = G_TYPE_STR;
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
ycalc__build_range      (tDEP_ROOT *a_deproot, tCALC *a_calc, char *a_token)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   char        x_good      =  '-';
   int         b_beg, x_beg, y_beg, z_beg;
   int         b_end, x_end, y_end, z_end;
   int         a;
   tDEP_ROOT  *x_range     = NULL;
   tDEP_ROOT  *x_temp      = NULL;
   void       *x_owner     = NULL;
   /*---(check for range operator)-------*/
   DEBUG_YCALC   yLOG_note    ("check for range operator");
   if (strcmp (a_token, "..") != 0)    return 0;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(get starting point)-------------*/
   rc = ycalc__build_coords  (a_calc->prev->prev, &b_beg, &x_beg, &y_beg, &z_beg);
   DEBUG_YCALC   yLOG_value   ("rc"        , rc);
   if (rc != 0) {     
      rc = YCALC_ERROR_BUILD_RNG;
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(get ending point)---------------*/
   rc = ycalc__build_coords  (a_calc->prev      , &b_end, &x_end, &y_end, &z_end);
   DEBUG_YCALC   yLOG_value   ("rc"        , rc);
   if (rc != 0) {     
      rc = YCALC_ERROR_BUILD_RNG;
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(delete first ref)---------------*/
   x_owner = a_calc->prev->prev->r->owner;
   rc = ycalc_deps_delete (G_DEP_REQUIRE, &a_deproot, &(a_calc->prev->prev->r), &x_owner);
   /*> if (rc <  0) {                                                                 <* 
    *>    rc = YCALC_ERROR_BUILD_DEP;                                                 <* 
    *>    DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rc);                               <* 
    *>    return rc;                                                                  <* 
    *> }                                                                              <*/
   a_calc->prev->prev->t = G_TYPE_NOOP;
   /*> ycalc_call_who_at (x_beg, y_beg, z_beg, YCALC_LOOK, NULL, &x_temp);            <*/
   /*> ycalc_call_reaper (NULL, &x_temp);                                             <*/
   /*---(delete second ref)--------------*/
   x_owner = a_calc->prev->r->owner;
   rc = ycalc_deps_delete (G_DEP_REQUIRE, &a_deproot, &(a_calc->prev->r), &x_owner);
   /*> if (rc <  0) {                                                                 <* 
    *>    rc = YCALC_ERROR_BUILD_DEP;                                                 <* 
    *>    DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rc);                               <* 
    *>    return rc;                                                                  <* 
    *> }                                                                              <*/
   a_calc->prev->t = G_TYPE_NOOP;
   /*> ycalc_call_who_at (x_end, y_end, z_end, YCALC_LOOK, NULL, &x_temp);            <*/
   /*> ycalc_call_reaper (NULL, &x_temp);                                             <*/
   /*---(set dependencies)---------------*/
   DEBUG_YCALC   yLOG_note    ("set dedendencies");
   DEBUG_YCALC   yLOG_complex ("range"     , "bb=%4d, eb=%4d, bx=%4d, ex=%4d, by=%4d, ey=%4d, z=%4d", b_beg, b_end, x_beg, x_end, y_beg, y_end, z_beg);
   rc = ycalc_range_use (a_deproot, b_beg, b_end, x_beg, x_end, y_beg, y_end, z_beg, z_end, &x_range);
   DEBUG_YCALC   yLOG_value   ("rc"        , rc);
   if (rc <  0) {     
      rc = YCALC_ERROR_BUILD_DEP;
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(update types)-------------------*/
   DEBUG_YCALC   yLOG_note    ("mark type");
   a_calc->t = G_TYPE_REF;
   a_calc->r = x_range;
   a_calc->s = strdup (ycalc_call_labeler (x_range));
   DEBUG_YCALC   yLOG_info    ("label"    , a_calc->s);
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
ycalc__build_function   (tDEP_ROOT *a_deproot, tCALC *a_calc, char *a_token)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         x_len       =    0;
   int         i           =    0;
   char        x_var       [LEN_TERSE] = "";
   tDEP_ROOT  *x_ref       = NULL;
   /*---(check for function)-------------*/
   DEBUG_YCALC   yLOG_note    ("check for function");
   x_len = strlen (a_token);
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(locate function)----------------*/
   for (i = 0; i < MAX_FUNCS; ++i) {
      DEBUG_YCALC   yLOG_complex ("check"     , "%3d, %-12.12s, %3d, %-10.10p   vs %-12.12s, %3d", i, g_ycalc_funcs[i].name, g_ycalc_funcs[i].len, g_ycalc_funcs[i].f, a_token, len);
      /*---(check for end)---------------*/
      if (g_ycalc_funcs [i].f == NULL)                      break;
      if (g_ycalc_funcs [i].len == 0   )                    break;
      /*---(check for length)------------*/
      if (x_len  != g_ycalc_funcs [i].len)                  continue;
      /*---(check for name)--------------*/
      if (a_token [0] != g_ycalc_funcs[i].name [0])         continue;
      if (strcmp (g_ycalc_funcs[i].name, a_token) != 0)     continue;
      /*---(found)-----------------------*/
      DEBUG_YCALC   yLOG_complex ("found it"  , "%3d, %-12.12s, %3d, %-10.10p   vs %-12.12s, %3d", i, g_ycalc_funcs[i].name, g_ycalc_funcs[i].len, g_ycalc_funcs[i].f, a_token, x_len);
      a_calc->f = g_ycalc_funcs[i].f;
      /*---(update type)-----------------*/
      DEBUG_YCALC   yLOG_note    ("mark type");
      a_calc->t = G_TYPE_FUNC;
      /*---(check special)---------------*/
      if (strchr ("ий", a_token [0]) != NULL) {
         DEBUG_YCALC   yLOG_note    ("special var/func overlap");
         switch (a_token [0]) {
         case 'и' : strcpy (x_var, "x");  break;
         case 'й' : strcpy (x_var, "y");  break;
         }
         /*---(find token)---------------------*/
         rc = ycalc_call_who_named (x_var, YCALC_FULL, NULL, &x_ref);
         DEBUG_YCALC   yLOG_value   ("who"       , rc);
         DEBUG_YCALC   yLOG_point   ("x_ref"     , x_ref);
         if (rc < 0 && x_ref == NULL) {
            rc = YCALC_ERROR_BUILD_REF;
            DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rc);
            return 0;
         }
         /*---(set dependency)-----------------*/
         rc = ycalc_deps_create (G_DEP_REQUIRE, &a_deproot, &x_ref);
         DEBUG_YCALC   yLOG_value   ("rc"        , rc);
         if (rc < 0) {
            rc = YCALC_ERROR_BUILD_CIR;
            DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rc);
            return 0;
         }
      }
      /*---(complete)--------------------*/
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(fall-through)-------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc_build_findfunc    (char *a_token)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         x_len       =    0;
   int         i           =    0;
   /*---(check for function)-------------*/
   DEBUG_YCALC   yLOG_note    ("check for function");
   x_len = strlen (a_token);
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(locate function)----------------*/
   for (i = 0; i < MAX_FUNCS; ++i) {
      DEBUG_YCALC   yLOG_complex ("check"     , "%3d, %-12.12s, %3d, %-10.10p   vs %-12.12s, %3d", i, g_ycalc_funcs[i].name, g_ycalc_funcs[i].len, g_ycalc_funcs[i].f, a_token, len);
      /*---(check for end)---------------*/
      if (g_ycalc_funcs [i].f == NULL)                      break;
      if (g_ycalc_funcs [i].len == 0   )                    break;
      /*---(check for length)------------*/
      if (x_len  != g_ycalc_funcs [i].len)                  continue;
      /*---(check for name)--------------*/
      if (a_token [0] != g_ycalc_funcs[i].name [0])         continue;
      if (strcmp (g_ycalc_funcs[i].name, a_token) != 0)     continue;
      /*---(found)-----------------------*/
      DEBUG_YCALC   yLOG_complex ("found it"  , "%3d, %-12.12s, %3d, %-10.10p   vs %-12.12s, %3d", i, g_ycalc_funcs[i].name, g_ycalc_funcs[i].len, g_ycalc_funcs[i].f, a_token, x_len);
      /*---(complete)--------------------*/
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(fall-through)-------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc__build_reference  (tDEP_ROOT *a_deproot, tCALC *a_calc, char *a_token)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   tDEP_ROOT  *x_ref       = NULL;
   int         b, x, y, z;
   /*---(check for reference)------------*/
   DEBUG_YCALC   yLOG_info    ("a_token"  , a_token);
   DEBUG_YCALC   yLOG_note    ("look for reference");
   rc = ycalc_call_who_named (a_token, YCALC_FULL, NULL, &x_ref);
   if (rc < 0)  return 0;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(check reference)----------------*/
   DEBUG_YCALC   yLOG_point   ("x_ref"     , x_ref);
   if (x_ref == NULL) {
      rc = YCALC_ERROR_BUILD_REF;
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   a_calc->r = x_ref;
   /*---(set dependency)-----------------*/
   rc = ycalc_deps_create (G_DEP_REQUIRE, &a_deproot, &x_ref);
   DEBUG_YCALC   yLOG_value   ("rc"        , rc);
   if (rc < 0) {
      rc = YCALC_ERROR_BUILD_CIR;
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(update type)-----------------*/
   DEBUG_YCALC   yLOG_note    ("mark type");
   a_calc->t = G_TYPE_REF;
   a_calc->s = strndup (a_token, LEN_RECD);
   DEBUG_YCALC   yLOG_info    ("label"    , a_calc->s);
   /*---(ranges)-------------------------*/
   rc = myCALC.e_addresser (x_ref->owner, &b, &x, &y, &z);
   DEBUG_YCALC   yLOG_value   ("addresser" , rc);
   if (rc < 0) {
      rc = YCALC_ERROR_BUILD_DEP;
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rc);
   }
   rc = ycalc_range_include (&x_ref, b, x, y, z);
   DEBUG_YCALC   yLOG_value   ("ranges"    , rc);
   /*---(complete)--------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
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
   double      v           =  0.0;
   /*---(check for function)-------------*/
   DEBUG_YCALC   yLOG_note    ("check for value");
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   rc = strl2num (a_token, &v, LEN_LABEL);
   DEBUG_YCALC   yLOG_value   ("strl2num"  , rc);
   if (rc < 0)  {
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   a_calc->v = v;
   DEBUG_YCALC   yLOG_double  ("value"     , a_calc->v);
   /*---(update type)-----------------*/
   DEBUG_YCALC   yLOG_note    ("mark type");
   a_calc->t = G_TYPE_VAL;
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
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
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(create and null)-----------------*/
   DEBUG_YCALC   yLOG_note    ("allocate calc entry");
   x_calc   = ycalc__build_new (a_deproot);
   DEBUG_YCALC   yLOG_value   ("calc step" , a_deproot->ncalc);
   /*---(try al types)--------------------*/
   if (rc == 0)  rc = ycalc__build_char      (a_deproot, x_calc, a_token);
   if (rc == 0)  rc = ycalc__build_string    (a_deproot, x_calc, a_token);
   if (rc == 0)  rc = ycalc__build_range     (a_deproot, x_calc, a_token);
   if (rc == 0)  rc = ycalc__build_function  (a_deproot, x_calc, a_token);
   if (rc == 0)  rc = ycalc__build_value     (a_deproot, x_calc, a_token);
   if (rc == 0)  rc = ycalc__build_reference (a_deproot, x_calc, a_token);
   if (rc == 0)  rc = YCALC_ERROR_BUILD_TOK;
   /*---(report out)-----------------------*/
   if (rc == 1)  DEBUG_YCALC   yLOG_complex ("element"   , "typ=%c, val=%F, str=%-9p, ref=%-9p, fnc=%-9p", x_calc->t, x_calc->v, x_calc->s, x_calc->r, x_calc->f);
   /*---(complete)-------------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ycalc__build_like       (tDEP_ROOT *a_deproot, char **a_source, char *a_type, char *a_rpn)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   void       *x_owner     = NULL;
   tDEP_ROOT  *x_ref       = NULL;
   int         b, x , y , z;
   int         bo, xo, yo, zo;
   char      **x_source    = NULL;
   char       *x_type      = NULL;
   char        x_work      [LEN_RECD];
   char        x_rpn       [LEN_RECD];
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(check for reference)------------*/
   DEBUG_YCALC   yLOG_note    ("look for like reference");
   rc = ycalc_call_who_named (*a_source + 1, YCALC_FULL, &x_owner, &x_ref);
   DEBUG_YCALC   yLOG_value   ("who_named" , rc);
   if (rc < 0) {
      rc = YCALC_ERROR_BUILD_LIK;
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(check reference)----------------*/
   DEBUG_YCALC   yLOG_point   ("x_owner"   , x_owner);
   DEBUG_YCALC   yLOG_point   ("x_ref"     , x_ref);
   if (x_owner == NULL || x_ref == NULL) {
      rc = YCALC_ERROR_BUILD_LIK;
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(set dependency)-----------------*/
   rc = ycalc_deps_create (G_DEP_SOURCE, &a_deproot, &x_ref);
   DEBUG_YCALC   yLOG_value   ("create"    , rc);
   if (rc < 0) {
      rc = YCALC_ERROR_BUILD_DEP;
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(get original source)------------*/
   rc = myCALC.e_pointer (x_owner, &x_source, &x_type, NULL, NULL);
   DEBUG_YCALC   yLOG_value   ("pointers"  , rc);
   if (rc < 0) {
      rc = YCALC_ERROR_BUILD_LIK;
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(check source)-------------------*/
   DEBUG_YCALC   yLOG_point   ("*x_source" , *x_source);
   if (*x_source == NULL) {
      rc = YCALC_ERROR_BUILD_LIK;
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   DEBUG_YCALC   yLOG_info    ("*x_source" , *x_source);
   /*---(check source type)--------------*/
   DEBUG_YCALC   yLOG_char    ("*x_type"   , *x_type);
   if (strchr ("=#", *x_type) == NULL) {
      rc = YCALC_ERROR_BUILD_LIK;
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(get offset)---------------------*/
   rc = myCALC.e_addresser (a_deproot->owner, &bo, &xo, &yo, &zo);
   DEBUG_YCALC   yLOG_value   ("cur_addr"  , rc);
   if (rc < 0) {
      rc = YCALC_ERROR_BUILD_LIK;
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   DEBUG_YCALC   yLOG_complex ("cur_loc"   , "%3db, %3dx, %3dy, %3dz", bo, xo, yo, zo);
   rc = myCALC.e_addresser (x_ref->owner, &b, &x, &y, &z);
   DEBUG_YCALC   yLOG_value   ("ref_addr"  , rc);
   if (rc < 0) {
      rc = YCALC_ERROR_BUILD_LIK;
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   DEBUG_YCALC   yLOG_complex ("ref_loc"   , "%3db, %3dx, %3dy, %3dz",  b, x,  y,  z);
   bo -= b;
   xo -= x;
   yo -= y;
   zo -= z;
   DEBUG_YCALC   yLOG_complex ("offset"    , "%3db, %3dx, %3dy, %3dz", bo, xo, yo, zo);
   /*---(adjust rpn)---------------------*/
   strlcpy (x_work, *x_source, LEN_RECD);
   DEBUG_YCALC   yLOG_info    ("x_work"    , x_work);
   rc = yRPN_addr_normal (x_work, bo, xo, yo, zo, LEN_RECD, x_rpn);
   DEBUG_YCALC   yLOG_value   ("adj_rpn"   , rc);
   if (rc < 0) {
      rc = YCALC_ERROR_BUILD_LIK;
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   DEBUG_YCALC   yLOG_info    ("x_rpn"    , x_rpn);
   strlcpy (a_rpn, x_rpn, LEN_RECD);
   /*---(update type)--------------------*/
   DEBUG_YCALC   yLOG_char    ("*x_type"   , *x_type);
   if      (*x_type == YCALC_DATA_NFORM) {
      DEBUG_YCALC   yLOG_note    ("source is numeric, classify as num-like");
      *a_type = YCALC_DATA_NLIKE;
      a_deproot->btype = *a_type;
      DEBUG_YCALC   yLOG_char    ("btype"     , a_deproot->btype);
   }
   else if (*x_type == YCALC_DATA_SFORM) {
      DEBUG_YCALC   yLOG_note    ("source is string, classify as str-like");
      *a_type = YCALC_DATA_SLIKE;
      a_deproot->btype = *a_type;
      DEBUG_YCALC   yLOG_char    ("btype"     , a_deproot->btype);
   }
   else {
      rc = YCALC_ERROR_BUILD_LIK;
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(complete)-------------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc_build_ripple      (tDEP_ROOT *a_deproot, char a_kind)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          =    0;
   int         x_count     =    0;
   int         i           =    0;
   int         n           =    0;
   tDEP_LINK  *x_next      = NULL;
   tDEP_LINK  *x_saved     [LEN_RECD];
   char        x_label     [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(capture dependencies)-----------*/
   x_count = a_deproot->npro;
   DEBUG_YCALC   yLOG_value   ("src npros" , x_count);
   x_next = a_deproot->pros;
   while (x_next != NULL) {
      strlcpy (x_label, ycalc_call_labeler (x_next->target), LEN_LABEL);
      DEBUG_YCALC   yLOG_complex ("check"     , "%c, %s", x_next->type, x_label);
      if (strchr (YCALC_GROUP_CALC, x_next->target->btype) != NULL) {
         if (x_next->type == a_kind) {
            DEBUG_YCALC   yLOG_note    ("found match");
            x_saved [n] = x_next;
            ++n;
         }
      }
      x_next = x_next->next;
   }
   /*---(handle empty)-------------------*/
   if (n == 0) {
      DEBUG_YCALC   yLOG_note    ("no matching dependencies found");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(walk through dependencies)------*/
   for (i = 0; i < n; ++i) {
      x_next = x_saved [i];
      strlcpy (x_label, ycalc_call_labeler (x_next->target), LEN_LABEL);
      rc = yCALC_handle (x_label);
   }
   /*---(complete)-------------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc_build_variable    (tDEP_ROOT *a_deproot, char *a_label, short b, short x, short y, short z, char **a_source, char *a_type, double *a_value, char **a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   void       *x_other     = NULL;
   tDEP_ROOT  *x_ref       = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YCALC   yLOG_point   ("a_deproot" , a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(transform location)-------------*/
   DEBUG_YCALC   yLOG_complex ("source"    , "%2du, %3dx, %3dy, %3dz", b, x, y, z);
   switch (*a_source [0]) {
   case 'т' :  --y;  break;
   case 'у' :  ++y;  break;
   case 'в' :  --x;  break;
   case 'ж' :  ++x;  break;
   }
   DEBUG_YCALC   yLOG_complex ("dest"      , "%2du, %3dx, %3dy, %3dz", b, x, y, z);
   /*---(get destination)----------------*/
   rc = ycalc_call_who_at (b, x, y, z, YCALC_FULL, &x_other, &x_ref);
   DEBUG_YCALC   yLOG_value   ("who_at"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("x_ref"     , x_ref);
   --rce;  if (x_ref == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save variable)------------------*/
   rc = ycalc_vars_new ('v', *a_source, a_label, x_ref, a_type, a_value, a_string);
   DEBUG_YCALC   yLOG_value   ("newvar"    , rc);
   if (rc < 0) {
      /*---(call reaper)--------------------*/
      DEBUG_YCALC   yLOG_note    ("then, cleanup as necessary");
      rc = ycalc_call_reaper (&x_other, &x_ref);
      DEBUG_YCALC   yLOG_value   ("reaper"    , rc);
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(set dependency)-----------------*/
   rc = ycalc_deps_create (G_DEP_CONTENT, &x_ref, &a_deproot);
   DEBUG_YCALC   yLOG_value   ("rc"        , rc);
   if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(complete)-------------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
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
   char        x_source    [LEN_RECD]  = "";;
   char        x_work      [LEN_RECD]  = "";;
   char        x_rpn       [LEN_RECD]  = "";;
   int         x_nrpn      =    0;
   char       *p           = NULL;           /* strtok current pointer         */
   int         x_len       =    0;
   char        t           [LEN_LABEL] = "";;
   tDEP_ROOT  *x_ref       = NULL;
   tCALC      *x_calc      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   DEBUG_YCALC   yLOG_point   ("a_deproot" , a_deproot);
   /*---(defense)------------------------*/
   DEBUG_YCALC   yLOG_char    ("*a_type"   , *a_type);
   DEBUG_YCALC   yLOG_info    ("valid"     , YCALC_GROUP_RPN);
   --rce;  if (strchr (YCALC_GROUP_RPN, *a_type) == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(length)-------------------------*/
   x_len = strllen (*a_source, LEN_RECD);
   --rce;  if (x_len <= 1) {
      ycalc_error_finalize (YCALC_ERROR_BUILD_RPN , a_type, a_value, a_string, "empty");
      a_deproot->btype = YCALC_DATA_ERROR;
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check for like formulas)--------*/
   --rce;  if (*a_type == YCALC_DATA_SLIKE || *a_type == YCALC_DATA_NLIKE) {
      rc = ycalc__build_like (a_deproot, a_source, a_type, x_work);
      DEBUG_YCALC   yLOG_value   ("like"      , rc);
      DEBUG_YCALC   yLOG_char    ("*a_type"   , *a_type);
      if (rc != 0) {
         ycalc_error_finalize (rc , a_type, a_value, a_string, "like");
         a_deproot->btype = YCALC_DATA_ERROR;
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      rc = yRPN_interpret (&x_work, &x_rpn, &x_nrpn, LEN_RECD, 0);
      DEBUG_YCALC   yLOG_value   ("interpret" , rc);
   } else {
      strlcpy (x_source, *a_source, LEN_RECD);
      rc = yRPN_interpret (x_source, &x_rpn, &x_nrpn, LEN_RECD, 0);
      DEBUG_YCALC   yLOG_value   ("interpret" , rc);
   }
   /*---(generate rpn)-------------------*/
   DEBUG_YCALC   yLOG_value   ("x_nrpn"    , x_nrpn);
   DEBUG_YCALC   yLOG_point   ("x_rpn"     , x_rpn);
   --rce;  if (x_nrpn <= 0 || x_rpn == NULL) {
      if (rc != 0) {
         sprintf (t, "pos %d", yRPN_errorpos ());
         ycalc_error_finalize (YCALC_ERROR_BUILD_RPN , a_type, a_value, a_string, t);
         a_deproot->btype = YCALC_DATA_ERROR;
      } else {
         ycalc_error_finalize (YCALC_ERROR_BUILD_RPN , a_type, a_value, a_string, "failed");
         a_deproot->btype = YCALC_DATA_ERROR;
      }
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save the rpn)-------------------*/
   a_deproot->rpn  = strndup (x_rpn, LEN_RECD);
   a_deproot->nrpn = x_nrpn;
   DEBUG_YCALC   yLOG_info    ("rpn"       , a_deproot->rpn);
   /*---(initialize)-------------------------*/
   p = ycalc__build_strtok (x_rpn);
   --rce;  if (p == NULL) {
      ycalc_error_finalize (YCALC_ERROR_BUILD_RPN , a_type, a_value, a_string, "empty");
      a_deproot->btype = YCALC_DATA_ERROR;
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(process tokens)---------------------*/
   while (p != NULL) {
      DEBUG_YCALC   yLOG_info    ("token"     , p);
      rc = ycalc__build_step (a_deproot, p);
      if (rc != 1) {
         ycalc_error_finalize (rc, a_type, a_value, a_string, p);
         a_deproot->btype = YCALC_DATA_ERROR;
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      p = ycalc__build_strtok (NULL);
   }
   /*---(complete)-------------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
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
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (ycalc_not_ready ()) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(setup)--------------------------*/
   DEBUG_YCALC   yLOG_point   ("a_owner"   , a_owner);
   DEBUG_YCALC   yLOG_point   ("a_deproot" , a_deproot);
   /*---(prepare)------------------------*/
   rc = ycalc_shared_verify (a_source, a_type, a_value, a_string);
   DEBUG_YCALC   yLOG_value   ("verify"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ycalc_build_trusted (a_deproot, a_source, a_type, a_value, a_string);
   DEBUG_YCALC   yLOG_value   ("trusted"   , rc);
   DEBUG_YCALC   yLOG_char    ("*a_type"   , *a_type);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
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
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (ycalc_not_ready ()) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(defense)------------------------*/
   DEBUG_YCALC   yLOG_point   ("a_owner"   , a_owner);
   --rce;  if (a_owner == NULL) {
      DEBUG_YCALC   yLOG_note    ("a_owner not set, nothing to do");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("a_deproot" , a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_YCALC   yLOG_note    ("a_deproot not set, nothing to do");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(fill pointers)------------------*/
   rc = myCALC.e_pointer (a_owner, &x_source, &x_type, &x_value, &x_string);
   DEBUG_YCALC   yLOG_value   ("pointer"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ycalc_build_detail (a_owner, a_deproot, x_source, x_type, x_value, x_string);
   DEBUG_YCALC   yLOG_value   ("detail"    , rc);
   DEBUG_YCALC   yLOG_char    ("*x_type"   , *x_type);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
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
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (ycalc_not_ready ()) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get owner/deproot)--------------*/
   rc = myCALC.e_who_named  (a_label, YCALC_OWNR, &x_owner, &x_deproot);
   DEBUG_YCALC   yLOG_value   ("who_named"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(call classify)------------------*/
   rc = ycalc_build_owner (x_owner, x_deproot);
   DEBUG_YCALC   yLOG_value   ("classify"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void      o___UNIT_TEST_______________o (void) {;}

char*        /*-> unit testing accessor --------------[ light  [us.IA0.2A5.X3]*/ /*-[02.0000.00#.#]-*/ /*-[--.---.---.--]-*/
ycalc__unit_build       (char *a_question, char *a_label)
{
   /*---(locals)-----------+-----+-----+-*/
   char        x_type      =  '-';
   tDEP_ROOT  *x_deproot   = NULL;
   char        x_label     [LEN_LABEL];
   /*---(initialize)---------------------*/
   strlcpy (ycalc__unit_answer, "yCALC_unit, unknown request", 100);
   /*> /+---(string testing)-----------------+/                                                                                                                                                                                                                                  <* 
    *> if      (strncmp (a_question, "top"       , 20)  == 0) {                                                                                                                                                                                                                  <* 
    *>    if (s_nstack <= 0)   snprintf (ycalc__unit_answer, LEN_FULL, "STACK top   (%2d) : %c %8.2lf %-10p %-.30s", s_nstack, S_TYPE_EMPTY, 0.0, NULL, "---");                                                                                                                   <* 
    *>    else {                                                                                                                                                                                                                                                                 <* 
    *>       switch (s_stack [s_nstack - 1].typ) {                                                                                                                                                                                                                               <* 
    *>       case S_TYPE_NUM :                                                                                                                                                                                                                                                   <* 
    *>       case S_TYPE_STR :                                                                                                                                                                                                                                                   <* 
    *>          snprintf (ycalc__unit_answer, LEN_FULL, "STACK top   (%2d) : %c %8.2lf %-10p %-.30s", s_nstack, s_stack [s_nstack - 1].typ, s_stack [s_nstack - 1].num, s_stack [s_nstack - 1].ref, (s_stack [s_nstack - 1].str == NULL) ? "---" : s_stack [s_nstack - 1].str);   <* 
    *>          break;                                                                                                                                                                                                                                                           <* 
    *>       case S_TYPE_REF :                                                                                                                                                                                                                                                   <* 
    *>          x_deproot = s_stack [s_nstack - 1].ref;                                                                                                                                                                                                                          <* 
    *>          strlcpy (x_label, ycalc__mock_labeler (x_deproot->owner), LEN_LABEL);                                                                                                                                                                                            <* 
    *>          snprintf (ycalc__unit_answer, LEN_FULL, "STACK top   (%2d) : %c %8.2lf %-10.10s %-.30s", s_nstack, s_stack [s_nstack - 1].typ, s_stack [s_nstack - 1].num, x_label, (s_stack [s_nstack - 1].str == NULL) ? "---" : s_stack [s_nstack - 1].str);                   <* 
    *>          break;                                                                                                                                                                                                                                                           <* 
    *>       }                                                                                                                                                                                                                                                                   <* 
    *>    }                                                                                                                                                                                                                                                                      <* 
    *> }                                                                                                                                                                                                                                                                         <*/
   /*---(complete)-----------------------*/
   return ycalc__unit_answer;
}



