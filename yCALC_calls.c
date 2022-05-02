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


/*> char                                                                              <* 
 *> metis_shared_free       (char a_abbr, void **r_old)                               <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        rce         =  -10;                                                <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_DATA   yLOG_senter  (__FUNCTION__);                                      <* 
 *>    DEBUG_DATA   yLOG_sint    (a_abbr);                                            <* 
 *>    /+---(check return)-------------------+/                                       <* 
 *>    DEBUG_DATA   yLOG_spoint  (r_old);                                             <* 
 *>    --rce;  if (r_old == NULL) {                                                   <* 
 *>       DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);                              <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    DEBUG_DATA   yLOG_spoint  (*r_old);                                            <* 
 *>    --rce;  if (*r_old == NULL) {                                                  <* 
 *>       DEBUG_DATA   yLOG_snote   ("never set");                                    <* 
 *>       DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);                              <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(clear and return)---------------+/                                       <* 
 *>    free (*r_old);                                                                 <* 
 *>    /+---(save return)--------------------+/                                       <* 
 *>    *r_old = NULL;                                                                 <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    DEBUG_DATA   yLOG_sexit   (__FUNCTION__);                                      <* 
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
ycalc_call_propvar      (char *a_label, tDEP_ROOT *a_deproot)
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
ycalc_call_newvar       (char a_kind, char *a_name, char *a_label, tDEP_ROOT *a_deproot, char *a_type, double *a_value, char **a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_name      [LEN_LABEL] = "";
   char        t           [LEN_LABEL] = "";
   int         l           =    0;
   tVARS      *x_new       = NULL;
   int         x_tries     =    0;
   /*---(header)-------------------------*/
   DEBUG_APIS   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_APIS   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_APIS   yLOG_info    ("a_name"    , a_name);
   DEBUG_APIS   yLOG_point   ("a_deproot" , a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(parse name)---------------------*/
   l = strlen (a_name);
   DEBUG_APIS   yLOG_value   ("l"         , l);
   --rce;  if (l < 2) {
      DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strlcpy (x_name, a_name + 1, LEN_LABEL);
   strlddel (x_name, '·', LEN_LABEL);
   strltrim (x_name, ySTR_BOTH, LEN_LABEL);
   DEBUG_APIS   yLOG_info    ("x_name"    , x_name);
   /*---(check functions)----------------*/
   /*> rc = ycalc_build_findfunc (x_name);                                                   <* 
    *> DEBUG_APIS   yLOG_value   ("findfunc"  , rc);                                         <* 
    *> --rce;  if (rc >  0) {                                                                <* 
    *>    DEBUG_APIS   yLOG_note    ("would mask a function");                               <* 
    *>    ycalc_error_finalize (YCALC_ERROR_BUILD_FNC , a_type, a_value, a_string, "dup");   <* 
    *>    DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);                                     <* 
    *>    return rce;                                                                        <* 
    *> }                                                                                     <*/
   /*---(check variables)----------------*/
   rc = ycalc_call_findvar (x_name, t);
   DEBUG_APIS   yLOG_value   ("findvar"   , rc);
   --rce;  if (rc >= 0) {
      DEBUG_APIS   yLOG_note    ("variable already exists");
      ycalc_error_finalize (YCALC_ERROR_BUILD_DUP , a_type, a_value, a_string, "dup");
      DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(allocate)-----------------------*/
   while (x_new == NULL) {
      ++x_tries;
      x_new = malloc (sizeof (tVARS));
      if (x_tries > 3)   break;
   }
   DEBUG_APIS   yLOG_value   ("x_tries"   , x_tries);
   DEBUG_APIS   yLOG_point   ("x_new"     , x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(hook)---------------------------*/
   DEBUG_APIS   yLOG_point   ("head"      , s_hvar);
   DEBUG_APIS   yLOG_point   ("tail"      , s_tvar);
   x_new->next  = x_new->prev  = NULL;
   if (s_hvar  == NULL) {
      DEBUG_APIS   yLOG_note    ("first");
      s_hvar  = s_tvar  = x_new;
   } else {
      DEBUG_APIS   yLOG_note    ("append");
      x_new->prev    = s_tvar;
      s_tvar->next   = x_new;
      s_tvar         = x_new;
   }
   /*---(add)----------------------------*/
   x_new->kind  = a_kind;
   strlcpy (x_new->name , x_name , LEN_LABEL);
   strlcpy (x_new->title  , a_label, LEN_LABEL);
   strlcpy (x_new->content, ycalc_call_labeler (a_deproot), LEN_LABEL);
   ++s_nvar;
   DEBUG_APIS   yLOG_value   ("s_nvar"    , s_nvar);
   /*---(propagate)----------------------*/
   ycalc_build_errorcheck  ();
   /*---(complete)-----------------------*/
   DEBUG_APIS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char 
ycalc_call_findvar      (char *a_name, char *a_real)
{
   char        rce         =  -10;
   tVARS      *x_cur       = NULL;
   /*---(header)-------------------------*/
   DEBUG_APIS   yLOG_senter  (__FUNCTION__);
   DEBUG_APIS   yLOG_spoint  (a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_APIS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_APIS   yLOG_snote   (a_name);
   DEBUG_APIS   yLOG_spoint  (a_real);
   --rce;  if (a_real == NULL) {
      DEBUG_APIS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_cur = s_hvar;
   while (x_cur != NULL) {
      if (strcmp (a_name, x_cur->name) != 0) {
         x_cur = x_cur->next;  
         continue;
      }
      strlcpy (a_real, x_cur->content, LEN_LABEL);
      DEBUG_APIS   yLOG_snote   (a_real);
      DEBUG_APIS   yLOG_sexit   (__FUNCTION__);
      return 0;
   }
   DEBUG_APIS   yLOG_sexitr  (__FUNCTION__, rce);
   return rce;
}

char 
ycalc_call_delvar       (char *a_loc, tDEP_ROOT **a_deproot)
{
   char        rce         =  -10;
   int         i           =    0;
   tVARS      *x_cur       = NULL;
   tDEP_ROOT  *x_deproot   = NULL;
   /*---(header)-------------------------*/
   DEBUG_APIS   yLOG_senter  (__FUNCTION__);
   DEBUG_APIS   yLOG_spoint  (a_loc);
   --rce;  if (a_loc == NULL) {
      DEBUG_APIS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_APIS   yLOG_snote   (a_loc);
   DEBUG_APIS   yLOG_spoint  (a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_APIS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_APIS   yLOG_spoint  (*a_deproot);
   --rce;  if (*a_deproot == NULL) {
      DEBUG_APIS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_cur = s_hvar;
   while (x_cur != NULL) {
      /*---(filter)-------------------------*/
      if (strcmp (a_loc, x_cur->title) != 0) {
         x_cur = x_cur->next;
         continue;
      }
      /*> ycalc_call_who_named  (x_cur->title, YCALC_LOOK, NULL, &x_deproot);         <*/
      ycalc_deps_deltitle   (a_deproot);
      /*---(propagate)----------------------*/
      x_cur->kind = '-';
      strlcpy (x_cur->name , "", LEN_LABEL);
      strlcpy (x_cur->title  , "", LEN_LABEL);
      ycalc_call_propvar      (x_cur->content, NULL);
      strlcpy (x_cur->content, "", LEN_LABEL);
      /*---(out of linked list)-------------*/
      DEBUG_APIS   yLOG_spoint  (s_hvar);
      DEBUG_APIS   yLOG_spoint  (s_tvar);
      DEBUG_APIS   yLOG_snote   ("unlink");
      if (x_cur->next != NULL)   x_cur->next->prev  = x_cur->prev;
      else                       s_tvar             = x_cur->prev;
      if (x_cur->prev != NULL)   x_cur->prev->next  = x_cur->next;
      else                       s_hvar             = x_cur->next;
      --s_nvar;
      DEBUG_APIS   yLOG_sexit   (__FUNCTION__);
      return 0;
   }
   DEBUG_APIS   yLOG_sexitr  (__FUNCTION__, rce);
   return rce;
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
   DEBUG_APIS   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_APIS   yLOG_point   ("a_owner"    , a_owner);
   --rce;  if (a_owner != NULL)  {
      DEBUG_APIS   yLOG_point   ("*a_owner"   , *a_owner);
   }
   DEBUG_APIS   yLOG_point   ("a_deproot" , a_deproot);
   --rce;  if (a_deproot == NULL)  {
      DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(if just a owner)----------------*/
   DEBUG_APIS   yLOG_point   ("*a_deproot", *a_deproot);
   --rce;  if (*a_deproot == NULL)  {
      rc = myCALC.e_reaper (a_owner);
      DEBUG_APIS   yLOG_value   ("reaper"    , rc);
      --rce;  if (rc < 0 ) {
         DEBUG_APIS   yLOG_note    ("reaper was not successful");
         DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_APIS   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(defense)------------------------*/
   DEBUG_APIS   yLOG_point   ("owner"     , (*a_deproot)->owner);
   --rce;  if ((*a_deproot)->owner == NULL)  {
      DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strlcpy (x_label, ycalc_call_labeler (*a_deproot), LEN_LABEL);
   DEBUG_APIS   yLOG_info    ("label"     , x_label);
   /*---(check reqs)---------------------*/
   DEBUG_APIS   yLOG_value   ("nreq"      , (*a_deproot)->nreq);
   --rce;  if ((*a_deproot)->nreq > 0) {
      DEBUG_APIS   yLOG_note    ("deproot contains real connections to other cells");
      DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check pros)---------------------*/
   DEBUG_APIS   yLOG_value   ("npro"      , (*a_deproot)->npro);
   x_pro_real = ycalc_range_nonrange (*a_deproot);
   DEBUG_APIS   yLOG_value   ("real"      , x_pro_real);
   --rce;  if (x_pro_real > 0) {
      DEBUG_APIS   yLOG_note    ("deproot provides real value to other cells");
      DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check type)---------------------*/
   rc = myCALC.e_valuer ((*a_deproot)->owner, &x_type, NULL, NULL);
   DEBUG_APIS   yLOG_value   ("valuer"    , rc);
   DEBUG_APIS   yLOG_char    ("type"      , x_type);
   DEBUG_APIS   yLOG_info    ("valid"     , YCALC_GROUP_DEPS);
   --rce;  if (rc < 0) {
      DEBUG_APIS   yLOG_note    ("valuer blew, dont touch");
      DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (x_type == YCALC_DATA_MERGED || x_type == YCALC_DATA_PMERGE) {
      DEBUG_APIS   yLOG_note    ("real or potential merged cell");
      DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (strchr (YCALC_GROUP_DEPS, x_type) != NULL) {
      /*> if (x_type != YCALC_DATA_MERGED) {                                          <*/
      DEBUG_APIS   yLOG_note    ("owner type requires dependencies");
      DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
      /*> }                                                                           <*/
      /*> DEBUG_APIS   yLOG_note    ("now useless merge indicator, remove");          <*/
   }
   --rce;  if ((*a_deproot)->npro > 0 && x_type != YCALC_DATA_BLANK) {
      DEBUG_APIS   yLOG_note    ("not blank, keep in range dependencies");
      DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(clear from range)---------------*/
   rc = ycalc_range_unhook (a_owner, a_deproot);
   DEBUG_APIS   yLOG_value   ("unhook"    , rc);
   --rce;  if (rc < 0 ) {
      DEBUG_APIS   yLOG_note    ("can not remove from ranges");
      DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = myCALC.e_valuer (*a_owner, &x_type, NULL, NULL);
   DEBUG_APIS   yLOG_char    ("type"      , x_type);
   /*---(disable deproot)----------------*/
   rc = yCALC_disable (a_owner, a_deproot);
   DEBUG_APIS   yLOG_value   ("disable"   , rc);
   --rce;  if (rc < 0 ) {
      DEBUG_APIS   yLOG_note    ("can not disable");
      DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   *a_deproot = NULL;
   DEBUG_APIS   yLOG_point   ("*a_deproot" , *a_deproot);
   /*---(call reaper on owner)-----------*/
   rc = myCALC.e_who_named  (x_label, YCALC_LOOK, &x_owner, NULL);
   DEBUG_YCALC   yLOG_value   ("who_named"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_APIS   yLOG_point   ("x_owner"    , x_owner);
   --rce;  if (x_owner == NULL) {
      DEBUG_APIS   yLOG_note    ("cell no longer exists");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = myCALC.e_valuer (x_owner, &x_type, NULL, NULL);
   DEBUG_APIS   yLOG_char    ("type"      , x_type);
   if (x_type == YCALC_DATA_BLANK || x_type == YCALC_DATA_GARBAGE) {
      rc = myCALC.e_reaper (&x_owner);
      DEBUG_APIS   yLOG_value   ("reaper"    , rc);
      --rce;  if (rc < 0 ) {
         DEBUG_APIS   yLOG_note    ("reaper was not successful");
         DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      *a_owner   = x_owner;
      DEBUG_APIS   yLOG_point   ("*a_owner"   , *a_owner);
   } else {
      DEBUG_APIS   yLOG_note    ("cell is now independent");
   }
   /*---(complete)-----------------------*/
   DEBUG_APIS   yLOG_exit    (__FUNCTION__);
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
   DEBUG_APIS   yLOG_enter   (__FUNCTION__);
   DEBUG_APIS   yLOG_char    ("a_force"   , a_force);
   DEBUG_APIS   yLOG_point   ("a_owner"   , a_owner);
   DEBUG_APIS   yLOG_point   ("a_deproot" , a_deproot);
   /*---(prepare)------------------------*/
   if (a_owner   != NULL)   *a_owner   = NULL;
   if (a_deproot != NULL)   *a_deproot = NULL;
   /*---(defense)------------------------*/
   DEBUG_APIS   yLOG_point   ("a_label"   , a_label);
   --rce;  if (a_label == NULL) {
      DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_APIS   yLOG_info    ("a_label"   , a_label);
   /*---(defense)------------------------*/
   DEBUG_APIS   yLOG_point   ("who_named" , myCALC.e_who_named);
   --rce;  if (myCALC.e_who_named == NULL) {
      DEBUG_APIS   yLOG_error   ("who_named without this callback, references cannot be resolved");
      DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check variables)----------------*/
   rc = ycalc_call_findvar (a_label, x_label);
   DEBUG_APIS   yLOG_value   ("findvar"   , rc);
   if (rc < 0)  strlcpy (x_label, a_label, LEN_LABEL);
   /*---(callback)-----------------------*/
   rc = myCALC.e_who_named (x_label, a_force, &x_owner, &x_deproot);
   DEBUG_APIS   yLOG_value   ("who named" , rc);
   --rce;  if (rc < 0) {
      DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_APIS   yLOG_point   ("x_owner"   , x_owner);
   --rce;  if (x_owner == NULL) {
      DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_APIS   yLOG_point   ("x_deproot" , x_deproot);
   /*---(setup)--------------------------*/
   if (a_force == YCALC_FULL) {
      DEBUG_APIS   yLOG_note    ("fully force creation");
      if (x_deproot == NULL) {
         rc = yCALC_enable (x_owner);
         DEBUG_APIS   yLOG_value   ("enable"    , rc);
         --rce;  if (rc < 0) {
            DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
         rc = myCALC.e_who_named (x_label, a_force, NULL, &x_deproot);
         DEBUG_APIS   yLOG_value   ("rc"        , rc);
         --rce;  if (rc < 0) {
            DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
         DEBUG_APIS   yLOG_point   ("x_deproot" , x_deproot);
      }
      --rce;  if (x_deproot == NULL) {
         DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
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
      DEBUG_APIS   yLOG_point   ("*owner"    , *a_owner);
   }
   if (a_deproot   != NULL) {
      *a_deproot = x_deproot;
      DEBUG_APIS   yLOG_point   ("*deproot"  , *a_deproot);
   }
   /*---(complete)-------------------------*/
   DEBUG_APIS   yLOG_exit    (__FUNCTION__);
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
   DEBUG_APIS   yLOG_enter   (__FUNCTION__);
   DEBUG_APIS   yLOG_complex ("position"  , "%2db, %3dx, %5dy, %3dz", b, x, y, z);
   DEBUG_APIS   yLOG_char    ("a_force"   , a_force);
   DEBUG_APIS   yLOG_point   ("owner"     , a_owner);
   DEBUG_APIS   yLOG_point   ("deproot"   , a_deproot);
   /*---(prepare)------------------------*/
   if (a_owner   != NULL)   *a_owner   = NULL;
   if (a_deproot != NULL)   *a_deproot = NULL;
   /*---(defense)------------------------*/
   DEBUG_APIS   yLOG_point   ("who_at"    , myCALC.e_who_at);
   --rce;  if (myCALC.e_who_at == NULL) {
      DEBUG_APIS   yLOG_error   ("who_named without this callback, references cannot be resolved");
      DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(callback)-----------------------*/
   rc = myCALC.e_who_at    (b, x, y, z, a_force, &x_owner, &x_deproot);
   DEBUG_APIS   yLOG_value   ("rc"        , rc);
   --rce;  if (rc < 0) {
      DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_APIS   yLOG_point   ("owner"     , x_owner);
   --rce;  if (x_owner == NULL) {
      DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_APIS   yLOG_point   ("x_deproot" , x_deproot);
   /*---(setup)--------------------------*/
   if (a_force == YCALC_FULL) {
      DEBUG_APIS   yLOG_note    ("fully force creation");
      if (x_deproot == NULL) {
         rc = yCALC_enable (x_owner);
         DEBUG_APIS   yLOG_value   ("enable"    , rc);
         --rce;  if (rc < 0) {
            DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
         rc = myCALC.e_who_at    (b, x, y, z, a_force, NULL, &x_deproot);
         DEBUG_APIS   yLOG_value   ("rc"        , rc);
         --rce;  if (rc < 0) {
            DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
         DEBUG_APIS   yLOG_point   ("x_deproot" , x_deproot);
      }
      --rce;  if (x_deproot == NULL) {
         DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(save)-----------------------------*/
   if (a_owner   != NULL) {
      *a_owner = x_owner;
      DEBUG_APIS   yLOG_point   ("*owner"    , *a_owner);
   }
   if (a_deproot   != NULL) {
      *a_deproot = x_deproot;
      DEBUG_APIS   yLOG_point   ("*deproot"  , *a_deproot);
   }
   /*---(complete)-------------------------*/
   DEBUG_APIS   yLOG_exit    (__FUNCTION__);
   return 0;
}



