/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"



/*> char    (*myCALC.e_consumer )   (void *a_owner, void *a_deproot, int a_seq, int a_lvl);   <*/

/*> char    (*myCALC.e_enabler  )   (void *a_owner, void *a_deproot);                                                      <* 
 *> char    (*myCALC.e_pointer  )   (void *a_owner, char **a_source, char **a_type, double **a_value , char **a_string);   <* 
 *> char    (*myCALC.e_reaper   )   (void **a_owner);        /+ pass deproot->owner, tries to kill thing        +/         <*/

/*> char    (*myCALC.who_named)   (char *a_label, char a_force, void **a_owner, void **a_deproot);        /+ pass label of thing, get back deproot of thing  +/          <* 
 *> char    (*myCALC.who_at   )   (int b, int x, int y, int z, char a_force, void **a_owner, void **a_deproot);  /+ pass coordinates, get back deproot of thing     +/   <* 
 *> char*   (*myCALC.labeler  )   (void *a_owner);        /+ pass deproot->owner, get back label of thing    +/                                                          <*/

/*> char*   (*g_adjuster )   (char *a_label, int b, int x, int y, int z);        /+ pass deproot->owner, get back label of thing    +/   <* 
 *> char    (*myCALC.e_addresser)   (void *a_owner, int *b, int *x, int *y, int *z);                                                            <* 
 *> char    (*myCALC.e_valuer   )   (void *a_owner, char *a_type, double *a_value  , char   **a_string);                                        <* 
 *> char    (*myCALC.e_special  )   (void *a_owner, char  a_what, double *a_value , char   **a_string);                                         <* 
 *> char    (*myCALC.e_printer  )   (void *a_owner);                                                                                            <*/


tVARS   *s_hvar   = NULL;
tVARS   *s_tvar   = NULL;
int      s_nvar   = 0;
int      s_nmac   = 0;
int      s_nloc   = 0;
int      s_ngre   = 0;
int      s_nspe   = 0;
int      s_nnor   = 0;


/*> char                                                                              <* 
 *> metis_shared_free       (char a_abbr, void **r_old)                               <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        rce         =  -10;                                                <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_YSORT  yLOG_senter  (__FUNCTION__);                                      <* 
 *>    DEBUG_YSORT  yLOG_sint    (a_abbr);                                            <* 
 *>    /+---(check return)-------------------+/                                       <* 
 *>    DEBUG_YSORT  yLOG_spoint  (r_old);                                             <* 
 *>    --rce;  if (r_old == NULL) {                                                   <* 
 *>       DEBUG_YSORT  yLOG_sexitr  (__FUNCTION__, rce);                              <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    DEBUG_YSORT  yLOG_spoint  (*r_old);                                            <* 
 *>    --rce;  if (*r_old == NULL) {                                                  <* 
 *>       DEBUG_YSORT  yLOG_snote   ("never set");                                    <* 
 *>       DEBUG_YSORT  yLOG_sexitr  (__FUNCTION__, rce);                              <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(clear and return)---------------+/                                       <* 
 *>    free (*r_old);                                                                 <* 
 *>    /+---(save return)--------------------+/                                       <* 
 *>    *r_old = NULL;                                                                 <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    DEBUG_YSORT  yLOG_sexit   (__FUNCTION__);                                      <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/

/*====================------------------------------------====================*/
/*===----                      variable keeping                        ----===*/
/*====================------------------------------------====================*/
static void  o___VARIABLES_______o () { return; }

char
ycalc_build_errorcheck  (void)
{
   char        rc          =    0;           /* return code for errors         */
   tDEP_ROOT  *x_cur       = NULL;
   void       *x_owner     = NULL;
   char       *x_type      = NULL;
   int         c           =    0;
   char        x_label     [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   x_cur = myCALC.rhead;
   while (x_cur != NULL) {
      DEBUG_YCALC   yLOG_complex ("deproot"   , "%-10p, %c", x_cur, x_cur->btype);
      if (x_cur->btype == YCALC_DATA_ERROR) {
         strlcpy (x_label, ycalc_call_labeler (x_cur), LEN_LABEL);
         DEBUG_YCALC   yLOG_info    ("HANDLE"    , x_label);
         rc = yCALC_handle (x_label);
         ++c;
      }
      x_cur = x_cur->rnext;
   }
   DEBUG_YCALC   yLOG_value   ("c"         , c);
   if (c > 100)  c = 100;
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return c;
}

char
ycalc_vars__ripple      (char *a_label, tDEP_ROOT *a_deproot)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tDEP_ROOT  *x_deproot   = NULL;
   /*---(calc upward)--------------------*/
   --rce;  if (a_deproot != NULL)  x_deproot = a_deproot;
   else {
      rc = myCALC.e_who_named (a_label, YCALC_LOOK, NULL, &x_deproot);
      if (rc < 0) return rce;
   }
   --rce;  if (x_deproot != NULL) {
      rc = ycalc_build_ripple (x_deproot, G_DEP_PROVIDE);
      if (rc < 0)  return rce;
   }
   return 0;
}

char
ycalc_vars_legal        (uchar a_name [LEN_LABEL], char *a_kind)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         l           =    0;
   uchar       x_pre       =  '-';
   char        i           =    0;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YCALC   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_info    ("a_name"    , a_name);
   l = strlen (a_name);
   DEBUG_YCALC   yLOG_value   ("l"         , l);
   --rce;  if (l <= 0 || l > 10) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   x_pre = a_name [0];
   /*---(local)--------------------------*/
   --rce;  if (x_pre == G_CHAR_LOCAL) {
      DEBUG_YCALC   yLOG_note    ("local version");
      if (l != 2) {
         DEBUG_YCALC   yLOG_note    ("local variable limited to 2 chars");
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      if (a_kind != NULL)  *a_kind = 'l';
   }
   /*---(macro)--------------------------*/
   else if (x_pre == '@') {
      DEBUG_YCALC   yLOG_note    ("macro version");
      if (l != 2) {
         DEBUG_YCALC   yLOG_note    ("macro variable limited to 2 chars");
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      if (strchr (YSTR_UPLOW YSTR_NUMBER YSTR_GREEK, a_name [1]) == NULL) {
         DEBUG_YCALC   yLOG_note    ("greek 2nd char [0-9A-Za-zË-ˇ] only");
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      if (a_kind != NULL)  *a_kind = 'm';
   }
   /*---(special)------------------------*/
   else if (strchr ("ïÆ", x_pre) != NULL) {
      DEBUG_YCALC   yLOG_note    ("special version");
      if (l != 1) {
         DEBUG_YCALC   yLOG_note    ("special variable limited to 1 char");
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      if (a_kind != NULL)  *a_kind = 's';
   }
   /*---(greek)--------------------------*/
   else if (strchr (YSTR_GREEK, x_pre) != NULL) {
      DEBUG_YCALC   yLOG_note    ("greek version");
      if (l >  3) {
         DEBUG_YCALC   yLOG_note    ("greek variable limited to 1-3 chars");
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      if (l >= 2 && strchr (YSTR_UPLOW YSTR_NUMBER YSTR_SUBS, a_name [1]) == NULL) {
         DEBUG_YCALC   yLOG_note    ("greek 2nd char [0-9A-Za-z¿¡¬√ƒ≈] only");
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      if (l == 3 && strchr (YSTR_SUBS, a_name [2]) == NULL) {
         DEBUG_YCALC   yLOG_note    ("greek 3rd char [¿¡¬√ƒ≈] only");
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      if (a_kind != NULL)  *a_kind = 'g';
   }
   /*---(normal)-------------------------*/
   else if (strchr (YSTR_UPLOW, x_pre) != NULL) {
      DEBUG_YCALC   yLOG_note    ("normal version");
      for (i = 1; i < l; ++i) {
         if (strchr (YSTR_ALNUM YSTR_SUBS, a_name [i]) == NULL) {
            DEBUG_YCALC   yLOG_value   ("failed on" , i);
            DEBUG_YCALC   yLOG_note    ("normal variable [0-9A-Za-z_¿¡¬√ƒ≈] chars only");
            DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
      }
      if (a_kind != NULL)  *a_kind = 'n';
   }
   /*---(trouble)------------------------*/
   else  {
      DEBUG_YCALC   yLOG_note    ("variable must start with [A-Za-zË-ˇ] chars only");
      if (a_kind != NULL)  *a_kind = '-';
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc_vars_new          (char a_kind, char *a_name, char *a_label, tDEP_ROOT *a_deproot, char *a_type, double *a_value, char **a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_name      [LEN_LABEL] = "";
   char        t           [LEN_LABEL] = "";
   int         l           =    0;
   tVARS      *x_new       = NULL;
   int         x_tries     =    0;
   char        x_kind      =  '-';
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YCALC   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_info    ("a_name"    , a_name);
   DEBUG_YCALC   yLOG_point   ("a_deproot" , a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(parse name)---------------------*/
   l = strlen (a_name);
   DEBUG_YCALC   yLOG_value   ("l"         , l);
   --rce;  if (l < 2) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strlcpy  (x_name, a_name + 1, LEN_LABEL);
   strlddel (x_name, '∑', LEN_LABEL);
   strltrim (x_name, ySTR_BOTH, LEN_LABEL);
   DEBUG_YCALC   yLOG_info    ("x_name"    , x_name);
   /*---(check name)---------------------*/
   rc = ycalc_vars_legal (x_name, &x_kind);
   DEBUG_YCALC   yLOG_value   ("legal"     , rc);
   --rce;  if (rc < 0) {
      ycalc_error_finalize (YCALC_ERROR_BUILD_VAR , a_type, a_value, a_string, x_name);
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check functions)----------------*/
   /*> rc = ycalc_build_findfunc (x_name);                                                    <* 
    *> DEBUG_YCALC   yLOG_value   ("findfunc"  , rc);                                         <* 
    *> --rce;  if (rc >  0) {                                                                 <* 
    *>    DEBUG_YCALC   yLOG_note    ("would mask a function");                               <* 
    *>    ycalc_error_finalize (YCALC_ERROR_BUILD_FNC , a_type, a_value, a_string, x_name);   <* 
    *>    a_deproot->btype = YCALC_DATA_ERROR;                                                <* 
    *>    DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);                                     <* 
    *>    return rce;                                                                         <* 
    *> }                                                                                      <*/
   /*---(check variables)----------------*/
   rc = yCALC_variable (x_name, t, NULL);
   DEBUG_YCALC   yLOG_value   ("findvar"   , rc);
   --rce;  if (rc >= 0) {
      DEBUG_YCALC   yLOG_note    ("variable already exists");
      ycalc_error_finalize (YCALC_ERROR_BUILD_DUP , a_type, a_value, a_string, x_name);
      a_deproot->btype = YCALC_DATA_ERROR;
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(allocate)-----------------------*/
   while (x_new == NULL) {
      ++x_tries;
      x_new = malloc (sizeof (tVARS));
      if (x_tries > 3)   break;
   }
   DEBUG_YCALC   yLOG_value   ("x_tries"   , x_tries);
   DEBUG_YCALC   yLOG_point   ("x_new"     , x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(hook)---------------------------*/
   DEBUG_YCALC   yLOG_point   ("head"      , s_hvar);
   DEBUG_YCALC   yLOG_point   ("tail"      , s_tvar);
   x_new->next  = x_new->prev  = NULL;
   if (s_hvar  == NULL) {
      DEBUG_YCALC   yLOG_note    ("first");
      s_hvar  = s_tvar  = x_new;
   } else {
      DEBUG_YCALC   yLOG_note    ("append");
      x_new->prev    = s_tvar;
      s_tvar->next   = x_new;
      s_tvar         = x_new;
   }
   /*---(add)----------------------------*/
   x_new->kind  = x_kind;
   strlcpy (x_new->name , x_name , LEN_LABEL);
   strlcpy (x_new->title  , a_label, LEN_LABEL);
   strlcpy (x_new->content, ycalc_call_labeler (a_deproot), LEN_LABEL);
   /*---(cummulate)----------------------*/
   ++s_nvar;
   switch (x_kind) {
   case 'm'  : ++s_nmac;   break;
   case 'l'  : ++s_nloc;   break;
   case 'g'  : ++s_ngre;   break;
   case 's'  : ++s_nspe;   break;
   case 'n'  : ++s_nnor;   break;
   }
   DEBUG_YCALC   yLOG_value   ("s_nvar"    , s_nvar);
   /*---(propagate)----------------------*/
   ycalc_build_errorcheck  ();
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char 
yCALC_variable          (char *a_name, char r_real [LEN_LABEL], char r_label [LEN_LABEL])
{
   char        rce         =  -10;
   tVARS      *x_cur       = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_senter  (__FUNCTION__);
   DEBUG_YCALC   yLOG_spoint  (a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_YCALC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_snote   (a_name);
   DEBUG_YCALC   yLOG_spoint  (r_real);
   --rce;  if (r_real == NULL) {
      DEBUG_YCALC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_cur = s_hvar;
   while (x_cur != NULL) {
      if (strcmp (a_name, x_cur->name) != 0) {
         x_cur = x_cur->next;  
         continue;
      }
      strlcpy (r_real, x_cur->content, LEN_LABEL);
      if (r_label != NULL)  strlcpy (r_label, x_cur->title, LEN_LABEL);
      DEBUG_YCALC   yLOG_snote   (r_real);
      DEBUG_YCALC   yLOG_sexit   (__FUNCTION__);
      return 0;
   }
   DEBUG_YCALC   yLOG_sexitr  (__FUNCTION__, rce);
   return rce;
}

char
ycalc_vars__rem         (tVARS *a_cur)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_kind      =  '-';
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YCALC   yLOG_point   ("a_cur"     , a_cur);
   --rce;  if (a_cur == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save)---------------------------*/
   x_kind = a_cur->kind;
   /*---(wipe)---------------------------*/
   x_kind = a_cur->kind;
   a_cur->kind = '-';
   strlcpy (a_cur->name , "", LEN_LABEL);
   strlcpy (a_cur->title  , "", LEN_LABEL);
   ycalc_vars__ripple  (a_cur->content, NULL);
   strlcpy (a_cur->content, "", LEN_LABEL);
   /*---(out of linked list)-------------*/
   DEBUG_YCALC   yLOG_point   ("s_hvar"    , s_hvar);
   DEBUG_YCALC   yLOG_point   ("s_tvar"    , s_tvar);
   DEBUG_YCALC   yLOG_value   ("s_nvar"    , s_nvar);
   DEBUG_YCALC   yLOG_note    ("unlink");
   if (a_cur->next != NULL)   a_cur->next->prev  = a_cur->prev;
   else                       s_tvar             = a_cur->prev;
   if (a_cur->prev != NULL)   a_cur->prev->next  = a_cur->next;
   else                       s_hvar             = a_cur->next;
   a_cur->next = a_cur->prev = NULL;
   /*---(remove)-------------------------*/
   --s_nvar;
   switch (x_kind) {
   case 'm'  : --s_nmac;   break;
   case 'l'  : --s_nloc;   break;
   case 'g'  : --s_ngre;   break;
   case 's'  : --s_nspe;   break;
   case 'n'  : --s_nnor;   break;
   }
   DEBUG_YCALC   yLOG_point   ("s_hvar"    , s_hvar);
   DEBUG_YCALC   yLOG_point   ("s_tvar"    , s_tvar);
   DEBUG_YCALC   yLOG_value   ("s_nvar"    , s_nvar);
   /*---(free)---------------------------*/
   free (a_cur);
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char 
ycalc_vars_del          (char *a_loc, tDEP_ROOT **a_deproot)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =  -10;
   int         i           =    0;
   tVARS      *x_cur       = NULL;
   tDEP_ROOT  *x_deproot   = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YCALC   yLOG_point   ("a_loc"     , a_loc);
   --rce;  if (a_loc == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_info    ("a_loc"     , a_loc);
   DEBUG_YCALC   yLOG_point   ("a_deproot" , a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("*a_deproot", *a_deproot);
   --rce;  if (*a_deproot == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(find var)-----------------------*/
   x_cur = s_hvar;
   while (x_cur != NULL) {
      /*---(filter)-------------------------*/
      if (strcmp (a_loc, x_cur->title) != 0) {
         x_cur = x_cur->next;
         continue;
      }
      rc = ycalc_deps_delvar  (a_deproot);
      DEBUG_YCALC   yLOG_value   ("deps"      , rc);
      rc = ycalc_vars__rem (x_cur);
      DEBUG_YCALC   yLOG_value   ("remvar"    , rc);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(never found)--------------------*/
   DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
   return rce;
}

char 
yCALC_vars_dump          (void *f)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   char        t           [LEN_RECD]  = "";
   tVARS      *x_cur       = NULL;
   FILE       *x_file      = NULL;
   int         c           =    0;
   /*---(prepare)------------------------*/
   x_file = f;
   /*---(print)--------------------------*/
   fprintf (f, "#! parsing Âœ--∑∑œ∑∑œ-------------------∑∑œ------∑∑œ------∑∑Ê\n");
   fprintf (f, "#! titles  Âseq∑∑k∑∑name∑∑∑∑∑∑∑∑∑∑∑∑∑∑∑∑∑∑title∑∑∑∑content∑∑Ê\n");
   fprintf (f, "\n");
   fprintf (f, "# count = %d\n" , s_nvar);
   x_cur = s_hvar;
   while (x_cur != NULL) {
      if (c % 25 == 0)  fprintf (f, "\n#--  k  name----------------  title--  content  ¥\n");
      if (c %  5 == 0)  fprintf (f, "\n");
      fprintf (f, "%3d  %c  %-20.20s  %-7.7s  %-7.7s  ¥\n", c++, x_cur->kind, x_cur->name, x_cur->title, x_cur->content);
      x_cur = x_cur->next;  
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
yCALC_vars_status       (char a_size, short a_wide, char a_list [LEN_RECD])
{
   uchar       x_all       [LEN_LABEL] = "";
   uchar       x_mac       [LEN_LABEL] = "";
   uchar       x_loc       [LEN_LABEL] = "";
   uchar       x_gre       [LEN_LABEL] = "";
   uchar       x_spe       [LEN_LABEL] = "";
   uchar       x_nor       [LEN_LABEL] = "";
   strlpadn (s_nvar, x_all , '.', '>', 4);
   strlpadn (s_nnor, x_nor , '.', '>', 3);
   strlpadn (s_ngre, x_gre , '.', '>', 2);
   strlpadn (s_nmac, x_mac , '.', '>', 2);
   strlpadn (s_nloc, x_loc , '.', '>', 2);
   strlpadn (s_nspe, x_spe , '.', '>', 1);
   sprintf (a_list, "∑vars    %4sa %3sn %2sg %2sm %2sl %1ss ¥", x_all, x_nor, x_gre, x_mac, x_loc, x_spe);
   /*---(complete)-----------------------*/
   return 0;
}

char
ycalc_vars_wrap         (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tVARS      *x_cur       = NULL;
   tDEP_ROOT  *x_deproot   = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(walk variables)-----------------*/
   x_cur = s_hvar;
   while (x_cur != NULL) {
      /*---(get deproot)--------------------*/
      DEBUG_YCALC   yLOG_info    ("title"      , x_cur->title);
      DEBUG_YCALC   yLOG_info    ("content"    , x_cur->content);
      rc = ycalc_call_who_named  (s_hvar->title, YCALC_LOOK, NULL, &x_deproot);
      DEBUG_YCALC   yLOG_value   ("who_named"  , rc);
      DEBUG_YCALC   yLOG_point   ("x_deproot"  , x_deproot);
      /*---(real)---------------------------*/
      if (x_deproot != NULL) {
         rc = ycalc_deps_delvar (&x_deproot);
         DEBUG_YCALC   yLOG_value   ("deps"      , rc);
      }
      /*---(destroy)------------------------*/
      rc = ycalc_vars__rem (x_cur);
      DEBUG_YCALC   yLOG_value   ("remvar"    , rc);
      /*---(next)---------------------------*/
      x_cur = s_hvar;
      /*---(done)---------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       enable and disable                     ----===*/
/*====================------------------------------------====================*/
static void  o___ENABLE__________o () { return; }

char
yCALC_enable            (void *a_owner)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   void       *x_new       = NULL;
   tDEP_ROOT  *x_deproot   = NULL;
   int         x_tries     =    0;
   char       *x_type      = NULL;
   /*---(header)-------------------------*/
   /*> DEBUG_YCALC   yLOG_senter  (__FUNCTION__);                                      <*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   /*> DEBUG_YCALC   yLOG_spoint  (a_owner);                                           <*/
   DEBUG_YCALC   yLOG_point   ("a_owner"   , __FUNCTION__);
   --rce;  if (a_owner == NULL) {
      /*> DEBUG_YCALC   yLOG_snote   ("no owner pointer");                             <*/
      /*> DEBUG_YCALC   yLOG_sexitr  (__FUNCTION__, rce);                              <*/
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create)-------------------------*/
   /*> DEBUG_YCALC   yLOG_snote   ("malloc");                                          <*/
   DEBUG_YCALC   yLOG_note    ("running malloc");
   while (x_new == NULL && x_tries < 10) {
      /*> printf ("malloc () yCALC_enable\n");                                        <*/
      x_new = malloc (sizeof (tDEP_ROOT));
      /*> printf ("......... yCALC_enable done\n");                                   <*/
      ++x_tries;
   }
   /*> DEBUG_YCALC   yLOG_svalue  ("tries", x_tries);                                  <*/
   DEBUG_YCALC   yLOG_value   ("tries", x_tries);
   /*> DEBUG_YCALC   yLOG_spoint  (x_new);                                             <*/
   DEBUG_YCALC   yLOG_point   ("x_new"     , "x_new");
   --rce;  if (x_new == NULL) {
      /*> DEBUG_YCALC   yLOG_snote   ("FAILED");                                       <*/
      /*> DEBUG_YCALC   yLOG_sexitr  (__FUNCTION__, rce);                              <*/
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_deproot = (tDEP_ROOT *) x_new;
   /*---(calc fields)--------------------*/
   /*> DEBUG_YCALC   yLOG_snote   ("owner");                                           <*/
   DEBUG_YCALC   yLOG_note    ("owner");
   x_deproot->owner   = a_owner;
   x_deproot->range   = -1;
   /*---(calc fields)--------------------*/
   /*> DEBUG_YCALC   yLOG_snote   ("calc");                                            <*/
   DEBUG_YCALC   yLOG_note    ("calc");
   x_deproot->nrpn    = 0;
   x_deproot->rpn     = NULL;
   x_deproot->ncalc   = 0;
   x_deproot->chead   = NULL;
   x_deproot->ctail   = NULL;
   x_deproot->cuse    = 0;
   /*---(dependencies)-------------------*/
   /*> DEBUG_YCALC   yLOG_snote   ("deps");                                            <*/
   DEBUG_YCALC   yLOG_note    ("deps");
   x_deproot->reqs    = NULL;
   x_deproot->nreq    = 0;
   x_deproot->pros    = NULL;
   x_deproot->npro    = 0;
   /*---(sequencing)---------------------*/
   /*> DEBUG_YCALC   yLOG_snote   ("seq");                                             <*/
   DEBUG_YCALC   yLOG_note    ("seq");
   x_deproot->slevel  = 0;
   x_deproot->snext   = NULL;
   x_deproot->sprev   = NULL;
   /*---(stamp)--------------------------*/
   /*> DEBUG_YCALC   yLOG_snote   ("stamp");                                           <*/
   DEBUG_YCALC   yLOG_note    ("stamp");
   x_deproot->u       = 0;
   /*---(root doubly-linked list)--------*/
   /*> DEBUG_YCALC   yLOG_snote   ("list");                                            <*/
   DEBUG_YCALC   yLOG_note    ("list");
   x_deproot->rprev   = NULL;
   x_deproot->rnext   = NULL;
   if (myCALC.rtail == NULL) {
      myCALC.rhead         = x_deproot;
      myCALC.rtail         = x_deproot;
   } else {
      x_deproot->rprev     = myCALC.rtail;
      x_deproot->rnext     = NULL;
      myCALC.rtail->rnext  = x_deproot;
      myCALC.rtail         = x_deproot;
   }
   ++myCALC.rcount;
   /*---(save)---------------------------*/
   /*> DEBUG_YCALC   yLOG_snote   ("save");                                            <*/
   DEBUG_YCALC   yLOG_note    ("save to owner");
   /*> DEBUG_YCALC   yLOG_spoint  (myCALC.e_enabler);                                         <*/
   DEBUG_YCALC   yLOG_point   ("myCALC.e_enabler" , myCALC.e_enabler);
   --rce;  if (myCALC.e_enabler == NULL) {
      /*> DEBUG_YCALC   yLOG_snote   ("no enabler set");                               <*/
      /*> DEBUG_YCALC   yLOG_sexitr  (__FUNCTION__, rce);                              <*/
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*> DEBUG_YCALC   yLOG_sexit   (__FUNCTION__);                                      <*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   myCALC.e_enabler (a_owner, x_deproot);
   /*---(copy existing type)-------------*/
   rc = myCALC.e_pointer (a_owner, NULL, &x_type, NULL, NULL);
   DEBUG_YCALC   yLOG_value   ("pointer"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_char    ("*x_type"   , *x_type);
   x_deproot->btype = *x_type;
   DEBUG_YCALC   yLOG_char    ("btype"     , x_deproot->btype);
   /*---(complete)-----------------------*/
   return 0;
}

char
yCALC_disable           (void **a_owner, void **a_deproot)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tDEP_ROOT  *x_deproot   = NULL;
   void       *x_owner     = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YCALC   yLOG_point   ("a_deproot" , a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, -rce);
      return -rce;
   }
   x_deproot = *a_deproot;
   DEBUG_YCALC   yLOG_point   ("x_deproot" , x_deproot);
   --rce;  if (x_deproot == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, -rce);
      return -rce;
   }
   DEBUG_YCALC   yLOG_info    ("label"     , ycalc_call_labeler (x_deproot));
   x_owner   = x_deproot->owner;
   DEBUG_YCALC   yLOG_point   ("x_owner"   , x_owner);
   --rce;  if (x_owner == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(wipe)---------------------------*/
   ycalc_calc_wipe      (*a_deproot);
   ycalc_deps_wipe_reqs (a_owner, a_deproot);
   ycalc__seq_del       (*a_deproot);
   --rce;  if (*a_deproot == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, -rce);
      return -rce;
   }
   /*---(check on provides)--------------*/
   DEBUG_YCALC   yLOG_value   ("npro"      , x_deproot->npro);
   if (x_deproot->npro > 0) {
      DEBUG_YCALC   yLOG_note    ("cleaned, but still required");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, -rce);
      return -rce;
   }
   /*---(remove from dependency list)----*/
   DEBUG_YCALC   yLOG_note    ("remove from main list");
   if (x_deproot->rnext != NULL) x_deproot->rnext->rprev = x_deproot->rprev;
   else                          myCALC.rtail            = x_deproot->rprev;
   if (x_deproot->rprev != NULL) x_deproot->rprev->rnext = x_deproot->rnext;
   else                          myCALC.rhead            = x_deproot->rnext;
   --myCALC.rcount;
   /*---(free)---------------------------*/
   DEBUG_YCALC   yLOG_note    ("free the deproot and null it in owner");
   DEBUG_YCALC   yLOG_point   ("enabler"   , myCALC.e_enabler);
   --rce;  if (myCALC.e_enabler == NULL) {
      DEBUG_YCALC   yLOG_note    ("no enabler set");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   myCALC.e_enabler (x_owner, NULL);
   free (x_deproot);
   *a_deproot = NULL;
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     information wrappers                     ----===*/
/*====================------------------------------------====================*/
static void  o___INFO____________o () { return; }

char*
ycalc_call_labeler      (tDEP_ROOT *a_deproot)
{
   if (a_deproot        == NULL)  return g_nada;
   if (a_deproot->owner == NULL)  return g_nada;
   return myCALC.e_labeler (a_deproot->owner);
}

char
ycalc_call_reaper       (void **a_owner, tDEP_ROOT **a_deproot)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_pro_real  =    0;
   char        x_type      =  '-';
   void       *x_owner     = NULL;
   char        x_label     [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YCALC   yLOG_point   ("a_owner"    , a_owner);
   --rce;  if (a_owner != NULL)  {
      DEBUG_YCALC   yLOG_point   ("*a_owner"   , *a_owner);
   }
   DEBUG_YCALC   yLOG_point   ("a_deproot" , a_deproot);
   --rce;  if (a_deproot == NULL)  {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(if just a owner)----------------*/
   DEBUG_YCALC   yLOG_point   ("*a_deproot", *a_deproot);
   --rce;  if (*a_deproot == NULL)  {
      rc = myCALC.e_reaper (a_owner);
      DEBUG_YCALC   yLOG_value   ("reaper"    , rc);
      --rce;  if (rc < 0 ) {
         DEBUG_YCALC   yLOG_note    ("reaper was not successful");
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(defense)------------------------*/
   DEBUG_YCALC   yLOG_point   ("owner"     , (*a_deproot)->owner);
   --rce;  if ((*a_deproot)->owner == NULL)  {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strlcpy (x_label, ycalc_call_labeler (*a_deproot), LEN_LABEL);
   DEBUG_YCALC   yLOG_info    ("label"     , x_label);
   /*---(check type)---------------------*/
   rc = myCALC.e_valuer ((*a_deproot)->owner, &x_type, NULL, NULL);
   DEBUG_YCALC   yLOG_value   ("valuer"    , rc);
   DEBUG_YCALC   yLOG_char    ("type"      , x_type);
   DEBUG_YCALC   yLOG_info    ("valid"     , YCALC_GROUP_DEPS);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_note    ("valuer blew, dont touch");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check for variable)-------------*/
   /*> if (x_type == YCALC_DATA_VAR) {                                                <* 
    *>    rc = ycalc_vars_del (x_label, a_deproot);                                <* 
    *> }                                                                              <*/
   /*---(check reqs)---------------------*/
   DEBUG_YCALC   yLOG_value   ("nreq"      , (*a_deproot)->nreq);
   --rce;  if ((*a_deproot)->nreq > 0) {
      DEBUG_YCALC   yLOG_note    ("deproot contains real connections to other cells");
      if (x_type = YCALC_DATA_BLANK) {
         DEBUG_YCALC   yLOG_note    ("this is BLANK, there should be no requirements");
      }
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check pros)---------------------*/
   DEBUG_YCALC   yLOG_value   ("npro"      , (*a_deproot)->npro);
   x_pro_real = ycalc_range_nonrange (*a_deproot);
   DEBUG_YCALC   yLOG_value   ("real"      , x_pro_real);
   --rce;  if (x_pro_real > 0) {
      DEBUG_YCALC   yLOG_note    ("deproot provides real value to other cells");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check type)---------------------*/
   rc = myCALC.e_valuer ((*a_deproot)->owner, &x_type, NULL, NULL);
   DEBUG_YCALC   yLOG_value   ("valuer"    , rc);
   DEBUG_YCALC   yLOG_char    ("type"      , x_type);
   DEBUG_YCALC   yLOG_info    ("valid"     , YCALC_GROUP_DEPS);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_note    ("valuer blew, dont touch");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (x_type == YCALC_DATA_MERGED || x_type == YCALC_DATA_PMERGE) {
      DEBUG_YCALC   yLOG_note    ("real or potential merged cell");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (strchr (YCALC_GROUP_DEPS, x_type) != NULL) {
      /*> if (x_type != YCALC_DATA_MERGED) {                                          <*/
      DEBUG_YCALC   yLOG_note    ("owner type requires dependencies");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
      /*> }                                                                           <*/
      /*> DEBUG_YCALC   yLOG_note    ("now useless merge indicator, remove");          <*/
   }
   --rce;  if ((*a_deproot)->npro > 0 && x_type != YCALC_DATA_BLANK) {
      DEBUG_YCALC   yLOG_note    ("not blank, keep in range dependencies");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(clear from range)---------------*/
   rc = ycalc_range_unhook (a_owner, a_deproot);
   DEBUG_YCALC   yLOG_value   ("unhook"    , rc);
   --rce;  if (rc < 0 ) {
      DEBUG_YCALC   yLOG_note    ("can not remove from ranges");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = myCALC.e_valuer (*a_owner, &x_type, NULL, NULL);
   DEBUG_YCALC   yLOG_char    ("type"      , x_type);
   /*---(disable deproot)----------------*/
   rc = yCALC_disable (a_owner, a_deproot);
   DEBUG_YCALC   yLOG_value   ("disable"   , rc);
   --rce;  if (rc < 0 ) {
      DEBUG_YCALC   yLOG_note    ("can not disable");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   *a_deproot = NULL;
   DEBUG_YCALC   yLOG_point   ("*a_deproot" , *a_deproot);
   /*---(call reaper on owner)-----------*/
   rc = myCALC.e_who_named  (x_label, YCALC_LOOK, &x_owner, NULL);
   DEBUG_YCALC   yLOG_value   ("who_named"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("x_owner"    , x_owner);
   --rce;  if (x_owner == NULL) {
      DEBUG_YCALC   yLOG_note    ("cell no longer exists");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = myCALC.e_valuer (x_owner, &x_type, NULL, NULL);
   DEBUG_YCALC   yLOG_char    ("type"      , x_type);
   if (x_type == YCALC_DATA_BLANK || x_type == YCALC_DATA_GARBAGE) {
      rc = myCALC.e_reaper (&x_owner);
      DEBUG_YCALC   yLOG_value   ("reaper"    , rc);
      --rce;  if (rc < 0 ) {
         DEBUG_YCALC   yLOG_note    ("reaper was not successful");
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      *a_owner   = x_owner;
      DEBUG_YCALC   yLOG_point   ("*a_owner"   , *a_owner);
   } else {
      DEBUG_YCALC   yLOG_note    ("cell is now independent");
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc_call_who_named    (char *a_label, char a_force, void **a_owner, void **a_deproot)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   void       *x_owner     = NULL;
   void       *x_deproot   = NULL;
   char        x_label     [LEN_LABEL] = "";
   static void *x_sowner    = NULL;
   static void *x_sdeproot  = NULL;
   static char *x_sforce    =  '-';
   static int  x_shorts    =    0;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   DEBUG_YCALC   yLOG_char    ("a_force"   , a_force);
   DEBUG_YCALC   yLOG_point   ("a_owner"   , a_owner);
   DEBUG_YCALC   yLOG_point   ("a_deproot" , a_deproot);
   /*---(prepare)------------------------*/
   if (a_owner   != NULL)   *a_owner   = NULL;
   if (a_deproot != NULL)   *a_deproot = NULL;
   /*---(defense)------------------------*/
   DEBUG_YCALC   yLOG_point   ("a_label"   , a_label);
   --rce;  if (a_label == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_info    ("a_label"   , a_label);
   /*---(defense)------------------------*/
   DEBUG_YCALC   yLOG_point   ("who_named" , myCALC.e_who_named);
   --rce;  if (myCALC.e_who_named == NULL) {
      DEBUG_YCALC   yLOG_error   ("who_named without this callback, references cannot be resolved");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check variables)----------------*/
   rc = yCALC_variable (a_label, x_label, NULL);
   DEBUG_YCALC   yLOG_value   ("findvar"   , rc);
   if (rc < 0)  strlcpy (x_label, a_label, LEN_LABEL);
   /*---(callback)-----------------------*/
   rc = myCALC.e_who_named (x_label, a_force, &x_owner, &x_deproot);
   DEBUG_YCALC   yLOG_value   ("who named" , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("x_owner"   , x_owner);
   --rce;  if (x_owner == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("x_deproot" , x_deproot);
   /*---(setup)--------------------------*/
   if (a_force == YCALC_FULL) {
      DEBUG_YCALC   yLOG_note    ("fully force creation");
      if (x_deproot == NULL) {
         rc = yCALC_enable (x_owner);
         DEBUG_YCALC   yLOG_value   ("enable"    , rc);
         --rce;  if (rc < 0) {
            DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
         rc = myCALC.e_who_named (x_label, a_force, NULL, &x_deproot);
         DEBUG_YCALC   yLOG_value   ("rc"        , rc);
         --rce;  if (rc < 0) {
            DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
         DEBUG_YCALC   yLOG_point   ("x_deproot" , x_deproot);
      }
      --rce;  if (x_deproot == NULL) {
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(save)-----------------------------*/
   x_sowner   = x_owner;
   x_sdeproot = x_deproot;
   x_sforce   = a_force;
   /*---(return)---------------------------*/
   if (a_owner   != NULL) {
      *a_owner   = x_owner;
      DEBUG_YCALC   yLOG_point   ("*owner"    , *a_owner);
   }
   if (a_deproot   != NULL) {
      *a_deproot = x_deproot;
      DEBUG_YCALC   yLOG_point   ("*deproot"  , *a_deproot);
   }
   /*---(complete)-------------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc_call_who_at       (int b, int x, int y, int z, char a_force, void **a_owner, void **a_deproot)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   void       *x_owner     = NULL;
   void       *x_deproot   = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   DEBUG_YCALC   yLOG_complex ("position"  , "%2db, %3dx, %5dy, %3dz", b, x, y, z);
   DEBUG_YCALC   yLOG_char    ("a_force"   , a_force);
   DEBUG_YCALC   yLOG_point   ("owner"     , a_owner);
   DEBUG_YCALC   yLOG_point   ("deproot"   , a_deproot);
   /*---(prepare)------------------------*/
   if (a_owner   != NULL)   *a_owner   = NULL;
   if (a_deproot != NULL)   *a_deproot = NULL;
   /*---(defense)------------------------*/
   DEBUG_YCALC   yLOG_point   ("who_at"    , myCALC.e_who_at);
   --rce;  if (myCALC.e_who_at == NULL) {
      DEBUG_YCALC   yLOG_error   ("who_named without this callback, references cannot be resolved");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(callback)-----------------------*/
   rc = myCALC.e_who_at    (b, x, y, z, a_force, &x_owner, &x_deproot);
   DEBUG_YCALC   yLOG_value   ("rc"        , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("owner"     , x_owner);
   --rce;  if (x_owner == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("x_deproot" , x_deproot);
   /*---(setup)--------------------------*/
   if (a_force == YCALC_FULL) {
      DEBUG_YCALC   yLOG_note    ("fully force creation");
      if (x_deproot == NULL) {
         rc = yCALC_enable (x_owner);
         DEBUG_YCALC   yLOG_value   ("enable"    , rc);
         --rce;  if (rc < 0) {
            DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
         rc = myCALC.e_who_at    (b, x, y, z, a_force, NULL, &x_deproot);
         DEBUG_YCALC   yLOG_value   ("rc"        , rc);
         --rce;  if (rc < 0) {
            DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
         DEBUG_YCALC   yLOG_point   ("x_deproot" , x_deproot);
      }
      --rce;  if (x_deproot == NULL) {
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(save)-----------------------------*/
   if (a_owner   != NULL) {
      *a_owner = x_owner;
      DEBUG_YCALC   yLOG_point   ("*owner"    , *a_owner);
   }
   if (a_deproot   != NULL) {
      *a_deproot = x_deproot;
      DEBUG_YCALC   yLOG_point   ("*deproot"  , *a_deproot);
   }
   /*---(complete)-------------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}



