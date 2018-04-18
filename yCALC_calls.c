/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"



char    (*g_who_named)   (char *a_label      , void **a_owner, void **a_deproot);        /* pass label of thing, get back deproot of thing  */
char    (*g_who_at   )   (int x, int y, int z, void **a_owner, void **a_deproot);  /* pass coordinates, get back deproot of thing     */

char    (*g_consumer )   (void *a_owner, void *a_deproot, int a_seq, int a_lvl);
char    (*g_enabler  )   (void *a_owner, void *a_deproot);
char    (*g_reaper   )   (void *a_owner);        /* pass deproot->owner, tries to kill thing        */

char*   (*g_labeler  )   (void *a_owner);        /* pass deproot->owner, get back label of thing    */
char    (*g_addresser)   (void *a_owner, int   *x        , int    *y        , int     *z);
char    (*g_valuer   )   (void *a_owner, char *a_type, double *a_value  , char   **a_string);
char    (*g_special  )   (void *a_owner, char  a_what, double *a_value , char   **a_string);



/*====================------------------------------------====================*/
/*===----                       enable and disable                     ----===*/
/*====================------------------------------------====================*/
static void  o___ENABLE__________o () { return; }

char
yCALC_enable            (void *a_owner)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   tDEP_ROOT  *x_deproot   = NULL;
   int         x_tries     =    0;
   /*---(header)-------------------------*/
   DEBUG_DEPS   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_DEPS   yLOG_spoint  (a_owner);
   --rce;  if (a_owner == NULL) {
      DEBUG_DEPS   yLOG_snote   ("no owner pointer");
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_info    ("label"     , g_labeler (a_owner));
   /*---(create)-------------------------*/
   DEBUG_DEPS   yLOG_snote   ("malloc");
   while (x_deproot == NULL && x_tries < 10) {
      x_deproot = (tDEP_ROOT *) malloc (sizeof (tDEP_ROOT));
      ++x_tries;
   }
   DEBUG_DEPS   yLOG_svalue  ("tries", x_tries);
   DEBUG_DEPS   yLOG_spoint  (x_deproot);
   --rce;  if (x_deproot == NULL) {
      DEBUG_DEPS   yLOG_snote   ("FAILED");
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(calc fields)--------------------*/
   DEBUG_DEPS   yLOG_snote   ("owner");
   x_deproot->owner   = a_owner;
   /*---(calc fields)--------------------*/
   DEBUG_DEPS   yLOG_snote   ("calc");
   x_deproot->rpn     = NULL;
   x_deproot->ncalc   = 0;
   x_deproot->chead   = NULL;
   x_deproot->ctail   = NULL;
   x_deproot->cuse    = 0;
   /*---(dependencies)-------------------*/
   DEBUG_DEPS   yLOG_snote   ("calc");
   x_deproot->reqs    = NULL;
   x_deproot->nreq    = 0;
   x_deproot->pros    = NULL;
   x_deproot->npro    = 0;
   /*---(sequencing)---------------------*/
   DEBUG_DEPS   yLOG_snote   ("seq");
   x_deproot->slevel  = 0;
   x_deproot->snext   = NULL;
   x_deproot->sprev   = NULL;
   /*---(stamp)--------------------------*/
   DEBUG_DEPS   yLOG_snote   ("stamp");
   x_deproot->u       = 0;
   /*---(root doubly-linked list)--------*/
   DEBUG_DEPS   yLOG_snote   ("list");
   x_deproot->rprev   = NULL;
   x_deproot->rnext   = NULL;
   if (myCALC.rtail == NULL) {
      myCALC.rhead         = x_deproot;
      myCALC.rtail         = x_deproot;
   } else {
      x_deproot->rprev   = myCALC.rtail;
      x_deproot->rnext   = NULL;
      myCALC.rtail->rnext  = x_deproot;
      myCALC.rtail         = x_deproot;
   }
   ++myCALC.rcount;
   /*---(save)---------------------------*/
   DEBUG_DEPS   yLOG_snote   ("save");
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
   g_enabler (a_owner, x_deproot);
   /*---(complete)-----------------------*/
   return 0;
}

char
yCALC_disable           (void *a_deproot)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   tDEP_ROOT*  x_deproot   = NULL;
   /*---(header)-------------------------*/
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_DEPS   yLOG_point   ("a_deproot" , a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_DEPS   yLOG_note    ("nothing to do");
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(prepare)------------------------*/
   x_deproot = (tDEP_ROOT *) a_deproot;
   DEBUG_DEPS   yLOG_info    ("label"     , g_labeler (x_deproot->owner));
   /*---(wipe)---------------------------*/
   ycalc_calc_wipe     (x_deproot);
   ycalc_deps_wipe     (x_deproot);
   ycalc__seq_del      (x_deproot);
   /*---(check on provides)--------------*/
   DEBUG_DEPS   yLOG_value   ("npro"      , x_deproot->npro);
   --rce;  if (x_deproot->npro > 0) {
      DEBUG_DEPS   yLOG_note    ("cleaned, but still required");
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
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
   g_enabler (x_deproot->owner, NULL);
   free (x_deproot);
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     information wrappers                     ----===*/
/*====================------------------------------------====================*/
static void  o___INFO____________o () { return; }

char
ycalc_call_who_named    (char *a_label, void **a_owner, void **a_deproot)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   void       *x_owner     = NULL;
   void       *x_deproot   = NULL;
   char        x_save      [LEN_LABEL] = "";
   void       *x_sowner    = NULL;
   void       *x_sdeproot  = NULL;
   static int  x_shorts    =    0;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   DEBUG_CALC   yLOG_point   ("a_owner"   , a_owner);
   DEBUG_CALC   yLOG_point   ("a_deproot" , a_deproot);
   /*---(prepare)------------------------*/
   if (a_owner   != NULL)   *a_owner   = NULL;
   if (a_deproot != NULL)   *a_deproot = NULL;
   /*---(defense)------------------------*/
   DEBUG_CALC   yLOG_point   ("a_label"   , a_label);
   --rce;  if (a_label == NULL) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_info    ("a_label"   , a_label);
   /*---(shortcut)-----------------------*/
   if (strcmp (a_label, x_save) == 0) {
      DEBUG_CALC   yLOG_note    ("shortcut applies");
      ++x_shorts;
      if (a_owner   != NULL)   *a_owner   = x_sowner;
      if (a_deproot != NULL)   *a_deproot = x_sdeproot;
      DEBUG_CALC   yLOG_value   ("x_shorts"  , x_shorts);
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(defense)------------------------*/
   DEBUG_CALC   yLOG_point   ("who_named" , g_who_named);
   --rce;  if (g_who_named == NULL) {
      DEBUG_PROG   yLOG_error   ("who_named" , "without this callback, references cannot be resolved");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(callback)-----------------------*/
   rc = g_who_named (a_label, &x_owner, &x_deproot);
   DEBUG_CALC   yLOG_value   ("rc"        , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_point   ("x_owner"   , x_owner);
   --rce;  if (x_owner == NULL) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(setup)--------------------------*/
   DEBUG_CALC   yLOG_point   ("x_deproot" , x_deproot);
   if (x_deproot == NULL) {
      rc = yCALC_enable (x_owner);
      DEBUG_CALC   yLOG_value   ("enable"    , rc);
      --rce;  if (rc < 0) {
         DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      rc = g_who_named (a_label, NULL, &x_deproot);
      DEBUG_CALC   yLOG_value   ("rc"        , rc);
      --rce;  if (rc < 0) {
         DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_CALC   yLOG_point   ("x_deproot" , x_deproot);
   }
   --rce;  if (x_deproot == NULL) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save)-----------------------------*/
   strlcpy (x_save, a_label, LEN_LABEL);
   x_sowner   = x_owner;
   x_sdeproot = x_deproot;
   /*---(return)---------------------------*/
   if (a_owner   != NULL) {
      *a_owner = x_owner;
      DEBUG_CALC   yLOG_point   ("*owner"    , *a_owner);
   }
   if (a_deproot   != NULL) {
      *a_deproot = x_deproot;
      DEBUG_CALC   yLOG_point   ("*deproot"  , *a_deproot);
   }
   /*---(complete)-------------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc_call_who_at       (int x, int y, int z, void **a_owner, void **a_deproot)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   void       *x_owner     = NULL;
   void       *x_deproot   = NULL;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   DEBUG_CALC   yLOG_point   ("owner"     , a_owner);
   DEBUG_CALC   yLOG_point   ("deproot"   , a_deproot);
   /*---(prepare)------------------------*/
   if (a_owner   != NULL)   *a_owner   = NULL;
   if (a_deproot != NULL)   *a_deproot = NULL;
   /*---(defense)------------------------*/
   DEBUG_CALC   yLOG_point   ("who_at"    , g_who_at);
   --rce;  if (g_who_at == NULL) {
      DEBUG_PROG   yLOG_error   ("who_named" , "without this callback, references cannot be resolved");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(callback)-----------------------*/
   rc = g_who_at    (x, y, z, &x_owner, &x_deproot);
   DEBUG_CALC   yLOG_value   ("rc"        , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_point   ("owner"     , x_owner);
   --rce;  if (x_owner == NULL) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(setup)--------------------------*/
   DEBUG_CALC   yLOG_point   ("x_deproot" , x_deproot);
   if (x_deproot == NULL) {
      rc = yCALC_enable (x_owner);
      DEBUG_CALC   yLOG_value   ("enable"    , rc);
      --rce;  if (rc < 0) {
         DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      rc = g_who_at    (x, y, z, NULL, &x_deproot);
      DEBUG_CALC   yLOG_value   ("rc"        , rc);
      --rce;  if (rc < 0) {
         DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_CALC   yLOG_point   ("x_deproot" , x_deproot);
   }
   --rce;  if (x_deproot == NULL) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save)-----------------------------*/
   if (a_owner   != NULL) {
      *a_owner = x_owner;
      DEBUG_CALC   yLOG_point   ("*owner"    , *a_owner);
   }
   if (a_deproot   != NULL) {
      *a_deproot = x_deproot;
      DEBUG_CALC   yLOG_point   ("*deproot"  , *a_deproot);
   }
   /*---(complete)-------------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}



