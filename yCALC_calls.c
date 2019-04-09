/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"




char    (*g_consumer )   (void *a_owner, void *a_deproot, int a_seq, int a_lvl);

char    (*g_enabler  )   (void *a_owner, void *a_deproot);
char    (*g_pointer  )   (void *a_owner, char **a_source, char **a_type, double **a_value , char **a_string);
char    (*g_reaper   )   (void **a_owner);        /* pass deproot->owner, tries to kill thing        */

char    (*g_who_named)   (char *a_label, char a_force, void **a_owner, void **a_deproot);        /* pass label of thing, get back deproot of thing  */
char    (*g_who_at   )   (int b, int x, int y, int z, char a_force, void **a_owner, void **a_deproot);  /* pass coordinates, get back deproot of thing     */
char*   (*g_labeler  )   (void *a_owner);        /* pass deproot->owner, get back label of thing    */

char*   (*g_adjuster )   (char *a_label, int b, int x, int y, int z);        /* pass deproot->owner, get back label of thing    */
char    (*g_addresser)   (void *a_owner, int *b, int *x, int *y, int *z);
char    (*g_valuer   )   (void *a_owner, char *a_type, double *a_value  , char   **a_string);
char    (*g_special  )   (void *a_owner, char  a_what, double *a_value , char   **a_string);
char    (*g_printer  )   (void *a_owner);



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
   /*> DEBUG_DEPS   yLOG_senter  (__FUNCTION__);                                      <*/
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   /*> DEBUG_DEPS   yLOG_spoint  (a_owner);                                           <*/
   DEBUG_DEPS   yLOG_point   ("a_owner"   , __FUNCTION__);
   --rce;  if (a_owner == NULL) {
      /*> DEBUG_DEPS   yLOG_snote   ("no owner pointer");                             <*/
      /*> DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);                              <*/
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create)-------------------------*/
   /*> DEBUG_DEPS   yLOG_snote   ("malloc");                                          <*/
   DEBUG_DEPS   yLOG_note    ("running malloc");
   while (x_new == NULL && x_tries < 10) {
      /*> printf ("malloc () yCALC_enable\n");                                        <*/
      x_new = malloc (sizeof (tDEP_ROOT));
      /*> printf ("......... yCALC_enable done\n");                                   <*/
      ++x_tries;
   }
   /*> DEBUG_DEPS   yLOG_svalue  ("tries", x_tries);                                  <*/
   DEBUG_DEPS   yLOG_value   ("tries", x_tries);
   /*> DEBUG_DEPS   yLOG_spoint  (x_new);                                             <*/
   DEBUG_DEPS   yLOG_point   ("x_new"     , "x_new");
   --rce;  if (x_new == NULL) {
      /*> DEBUG_DEPS   yLOG_snote   ("FAILED");                                       <*/
      /*> DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);                              <*/
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_deproot = (tDEP_ROOT *) x_new;
   /*---(calc fields)--------------------*/
   /*> DEBUG_DEPS   yLOG_snote   ("owner");                                           <*/
   DEBUG_DEPS   yLOG_note    ("owner");
   x_deproot->owner   = a_owner;
   x_deproot->range   = -1;
   /*---(calc fields)--------------------*/
   /*> DEBUG_DEPS   yLOG_snote   ("calc");                                            <*/
   DEBUG_DEPS   yLOG_note    ("calc");
   x_deproot->nrpn    = 0;
   x_deproot->rpn     = NULL;
   x_deproot->ncalc   = 0;
   x_deproot->chead   = NULL;
   x_deproot->ctail   = NULL;
   x_deproot->cuse    = 0;
   /*---(dependencies)-------------------*/
   /*> DEBUG_DEPS   yLOG_snote   ("deps");                                            <*/
   DEBUG_DEPS   yLOG_note    ("deps");
   x_deproot->reqs    = NULL;
   x_deproot->nreq    = 0;
   x_deproot->pros    = NULL;
   x_deproot->npro    = 0;
   /*---(sequencing)---------------------*/
   /*> DEBUG_DEPS   yLOG_snote   ("seq");                                             <*/
   DEBUG_DEPS   yLOG_note    ("seq");
   x_deproot->slevel  = 0;
   x_deproot->snext   = NULL;
   x_deproot->sprev   = NULL;
   /*---(stamp)--------------------------*/
   /*> DEBUG_DEPS   yLOG_snote   ("stamp");                                           <*/
   DEBUG_DEPS   yLOG_note    ("stamp");
   x_deproot->u       = 0;
   /*---(root doubly-linked list)--------*/
   /*> DEBUG_DEPS   yLOG_snote   ("list");                                            <*/
   DEBUG_DEPS   yLOG_note    ("list");
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
   /*> DEBUG_DEPS   yLOG_snote   ("save");                                            <*/
   DEBUG_DEPS   yLOG_note    ("save to owner");
   /*> DEBUG_DEPS   yLOG_spoint  (g_enabler);                                         <*/
   DEBUG_DEPS   yLOG_point   ("g_enabler" , g_enabler);
   --rce;  if (g_enabler == NULL) {
      /*> DEBUG_DEPS   yLOG_snote   ("no enabler set");                               <*/
      /*> DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);                              <*/
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*> DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);                                      <*/
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
   g_enabler (a_owner, x_deproot);
   /*---(copy existing type)-------------*/
   rc = g_pointer (a_owner, NULL, &x_type, NULL, NULL);
   DEBUG_CALC   yLOG_value   ("pointer"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_char    ("*x_type"   , *x_type);
   x_deproot->btype = *x_type;
   DEBUG_CALC   yLOG_char    ("btype"     , x_deproot->btype);
   /*---(complete)-----------------------*/
   return 0;
}

char
yCALC_disable           (void **a_owner, void **a_deproot)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   tDEP_ROOT  *x_deproot   = NULL;
   void       *x_owner     = NULL;
   /*---(header)-------------------------*/
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_DEPS   yLOG_point   ("a_deproot" , a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, -rce);
      return -rce;
   }
   x_deproot = *a_deproot;
   DEBUG_DEPS   yLOG_point   ("x_deproot" , x_deproot);
   --rce;  if (x_deproot == NULL) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, -rce);
      return -rce;
   }
   DEBUG_DEPS   yLOG_info    ("label"     , ycalc_call_labeler (x_deproot));
   x_owner   = x_deproot->owner;
   DEBUG_DEPS   yLOG_point   ("x_owner"   , x_owner);
   --rce;  if (x_owner == NULL) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(wipe)---------------------------*/
   ycalc_calc_wipe      (*a_deproot);
   ycalc_deps_wipe_reqs (a_owner, a_deproot);
   ycalc__seq_del       (*a_deproot);
   --rce;  if (*a_deproot == NULL) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, -rce);
      return -rce;
   }
   /*---(check on provides)--------------*/
   DEBUG_DEPS   yLOG_value   ("npro"      , x_deproot->npro);
   if (x_deproot->npro > 0) {
      DEBUG_DEPS   yLOG_note    ("cleaned, but still required");
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, -rce);
      return -rce;
   }
   /*---(remove from dependency list)----*/
   DEBUG_DEPS   yLOG_note    ("remove from main list");
   if (x_deproot->rnext != NULL) x_deproot->rnext->rprev = x_deproot->rprev;
   else                          myCALC.rtail            = x_deproot->rprev;
   if (x_deproot->rprev != NULL) x_deproot->rprev->rnext = x_deproot->rnext;
   else                          myCALC.rhead            = x_deproot->rnext;
   --myCALC.rcount;
   /*---(free)---------------------------*/
   DEBUG_DEPS   yLOG_note    ("free the deproot and null it in owner");
   DEBUG_DEPS   yLOG_point   ("enabler"   , g_enabler);
   --rce;  if (g_enabler == NULL) {
      DEBUG_DEPS   yLOG_note    ("no enabler set");
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
   g_enabler (x_owner, NULL);
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
   return g_labeler (a_deproot->owner);
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
      rc = g_reaper (a_owner);
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
   rc = g_valuer ((*a_deproot)->owner, &x_type, NULL, NULL);
   DEBUG_APIS   yLOG_value   ("valuer"    , rc);
   DEBUG_APIS   yLOG_char    ("type"      , x_type);
   DEBUG_APIS   yLOG_info    ("valid"     , YCALC_GROUP_DEPS);
   --rce;  if (rc < 0) {
      DEBUG_APIS   yLOG_note    ("valuer blew, dont touch");
      DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (strchr (YCALC_GROUP_DEPS, x_type) != NULL) {
      DEBUG_APIS   yLOG_note    ("owner type requires dependencies");
      DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
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
   rc = g_valuer (*a_owner, &x_type, NULL, NULL);
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
   /*---(call reaper on owner)-----------*/
   rc = g_who_named  (x_label, YCALC_LOOK, &x_owner, NULL);
   DEBUG_CALC   yLOG_value   ("who_named"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_APIS   yLOG_point   ("x_owner"    , x_owner);
   --rce;  if (x_owner == NULL) {
      DEBUG_APIS   yLOG_note    ("cell no longer exists");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = g_valuer (x_owner, &x_type, NULL, NULL);
   DEBUG_APIS   yLOG_char    ("type"      , x_type);
   if (x_type == YCALC_DATA_BLANK) {
      rc = g_reaper (&x_owner);
      DEBUG_APIS   yLOG_value   ("reaper"    , rc);
      --rce;  if (rc < 0 ) {
         DEBUG_APIS   yLOG_note    ("reaper was not successful");
         DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      *a_owner = x_owner;
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
   char        x_save      [LEN_LABEL] = "";
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
   /*---(shortcut)-----------------------*/
   /*> if (strcmp (a_label, x_save) == 0 && a_force == x_sforce) {                    <* 
    *>    DEBUG_APIS   yLOG_note    ("shortcut applies");                             <* 
    *>    ++x_shorts;                                                                 <* 
    *>    if (a_owner   != NULL)   *a_owner   = x_sowner;                             <* 
    *>    if (a_deproot != NULL)   *a_deproot = x_sdeproot;                           <* 
    *>    DEBUG_APIS   yLOG_value   ("x_shorts"  , x_shorts);                         <* 
    *>    DEBUG_APIS   yLOG_exit    (__FUNCTION__);                                   <* 
    *>    return 0;                                                                   <* 
    *> }                                                                              <*/
   /*---(defense)------------------------*/
   DEBUG_APIS   yLOG_point   ("who_named" , g_who_named);
   --rce;  if (g_who_named == NULL) {
      DEBUG_APIS   yLOG_error   ("who_named without this callback, references cannot be resolved");
      DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(callback)-----------------------*/
   rc = g_who_named (a_label, a_force, &x_owner, &x_deproot);
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
         rc = g_who_named (a_label, a_force, NULL, &x_deproot);
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
   strlcpy (x_save, a_label, LEN_LABEL);
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
   DEBUG_APIS   yLOG_point   ("who_at"    , g_who_at);
   --rce;  if (g_who_at == NULL) {
      DEBUG_APIS   yLOG_error   ("who_named without this callback, references cannot be resolved");
      DEBUG_APIS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(callback)-----------------------*/
   rc = g_who_at    (b, x, y, z, a_force, &x_owner, &x_deproot);
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
         rc = g_who_at    (b, x, y, z, a_force, NULL, &x_deproot);
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



