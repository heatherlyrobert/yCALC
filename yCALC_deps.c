/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"



tDEP_INFO   g_dep_info [MAX_DEPTYPE] = {
   /*----type------  ---match----- ---dir--------  ---description--------------------------------------- index count total */

   {  G_DEP_REQUIRE, G_DEP_PROVIDE, G_DEP_DIRREQ , "requires another cell for its value"                ,  0  ,  0  ,  0   },
   {  G_DEP_PROVIDE, G_DEP_REQUIRE, G_DEP_DIRPRO , "provides its value to another cell"                 ,  0  ,  0  ,  0   },

   {  G_DEP_POINTER, G_DEP_TARGET , G_DEP_DIRREQ , "pointer that provides dependency indirection"       ,  0  ,  0  ,  0   },
   {  G_DEP_TARGET , G_DEP_POINTER, G_DEP_DIRPRO , "data item/cell that provides the data"              ,  0  ,  0  ,  0   },

   {  G_DEP_RANGE  , G_DEP_ENTRY  , G_DEP_DIRREQ , "range that provides dependency indirection"         ,  0  ,  0  ,  0   },
   {  G_DEP_ENTRY  , G_DEP_RANGE  , G_DEP_DIRPRO , "individual cell that makes up a range"              ,  0  ,  0  ,  0   },

   {  G_DEP_FORMAT , G_DEP_COPY   , G_DEP_DIRREQ , "format master cell providing format template"       ,  0  ,  0  ,  0   },
   {  G_DEP_COPY   , G_DEP_FORMAT , G_DEP_DIRPRO , "individual cell following the a format template"    ,  0  ,  0  ,  0   },

   {  G_DEP_SOURCE , G_DEP_LIKE   , G_DEP_DIRREQ , "source formula master other cell follow"            ,  0  ,  0  ,  0   },
   {  G_DEP_LIKE   , G_DEP_SOURCE , G_DEP_DIRPRO , "follows a source formula with ref adjustments"      ,  0  ,  0  ,  0   },

   {  G_DEP_MERGED , G_DEP_BLEED  , G_DEP_DIRREQ , "provides contents for set of merged cells"          ,  0  ,  0  ,  0   },
   {  G_DEP_BLEED  , G_DEP_MERGED , G_DEP_DIRPRO , "provides bleed-over space to display contents"      ,  0  ,  0  ,  0   },

   {  G_DEP_CALCREF, G_DEP_ADDRESS, G_DEP_DIRREQ , "contains a calculated/runtime reference function"   ,  0  ,  0  ,  0   },
   {  G_DEP_ADDRESS, G_DEP_CALCREF, G_DEP_DIRPRO , "provides its value to a calculated reference"       ,  0  ,  0  ,  0   },

   {  G_DEP_CONTENT, G_DEP_TITLE  , G_DEP_DIRREQ , "contains a variables value"                         ,  0  ,  0  ,  0   },
   {  G_DEP_TITLE  , G_DEP_CONTENT, G_DEP_DIRPRO , "provides a name to the variable"                    ,  0  ,  0  ,  0   },

   {  G_DEP_WATCHER, G_DEP_STALKED, G_DEP_DIRPRO , "watcher, watches another cell for properties"       ,  0  ,  0  ,  0   },
   {  G_DEP_STALKED, G_DEP_WATCHER, G_DEP_DIRREQ , "observed, provides information to another cell"     ,  0  ,  0  ,  0   },

   {  G_DEP_CALL   , G_DEP_DEST   , G_DEP_DIRPRO , "watcher, watches another cell for properties"       ,  0  ,  0  ,  0   },
   {  G_DEP_DEST   , G_DEP_CALL   , G_DEP_DIRREQ , "observed, provides information to another cell"     ,  0  ,  0  ,  0   },

   {  G_DEP_BLANK  , G_DEP_BLANK  , G_DEP_DIRNONE, "newly created dependency, not yet assigned"         ,  0  ,  0  ,  0   },

   /*----type------  ---match----- ---dir--------  ---description--------------------------------------- index count total */
};



char S_DEP_REQS [10] = "";
char S_DEP_PROS [10] = "";
char S_DEP_LIKE [10] = "";



/*====================------------------------------------====================*/
/*===----                       allocation/memory                      ----===*/
/*====================------------------------------------====================*/
static void  o___MALLOC__________o () { return; }

char         /*-> create a new, blank dependency -----[ leaf   [fp.C53.022.40]*/ /*-[11.0000.028.F]-*/ /*-[--.---.---.--]-*/
ycalc__deps_new         (tDEP_LINK **a_dep)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   tDEP_LINK  *x_dep       = NULL;
   int         x_tries     =    0;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*> DEBUG_YCALC   yLOG_senter  (__FUNCTION__);                                      <*/
   /*---(defense)------------------------*/
   DEBUG_YCALC   yLOG_point   ("a_dep"     , a_dep);
   /*> DEBUG_YCALC   yLOG_spoint  ("a_dep");                                           <*/
   --rce;  if (a_dep == NULL) {
      DEBUG_YCALC   yLOG_note   ("no return dep pointer");
      DEBUG_YCALC   yLOG_exitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("*a_dep"    , *a_dep);
   /*> DEBUG_YCALC   yLOG_spoint  (*a_dep);                                          <*/
   --rce;  if (*a_dep != NULL) {
      DEBUG_YCALC   yLOG_note   ("dep pointer already assigned");
      DEBUG_YCALC   yLOG_exitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(create)-------------------------*/
   DEBUG_YCALC   yLOG_note    ("malloc");
   /*> DEBUG_YCALC   yLOG_snote   ("malloc");                                          <*/
   DEBUG_YCALC   yLOG_point   ("x_dep"     , x_dep);
   while (x_dep == NULL && x_tries < 10) {
      /*> DEBUG_YCALC   yLOG_point   ("x_dep"     , x_dep);                            <*/
      /*> printf ("malloc () ycalc__deps_new\n");                                     <*/
      x_dep = (tDEP_LINK *) malloc (sizeof (tDEP_LINK));
      DEBUG_YCALC   yLOG_value   ("sizeof"    , sizeof (tDEP_LINK));
      DEBUG_YCALC   yLOG_point   ("x_dep"     , x_dep);
      ++x_tries;
   }
   DEBUG_YCALC   yLOG_value   ("tries"     , x_tries);
   /*> DEBUG_YCALC   yLOG_sint    (x_tries);                                  <*/
   --rce;  if (x_dep == NULL) {
      DEBUG_YCALC   yLOG_note   ("FAILED");
      DEBUG_YCALC   yLOG_exitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(dep fields)---------------------*/
   DEBUG_YCALC   yLOG_note    ("dep pointers");
   /*> DEBUG_YCALC   yLOG_snote   ("dep pointers");                                    <*/
   x_dep->type    = G_DEP_BLANK;
   x_dep->prev    = NULL;
   x_dep->next    = NULL;
   x_dep->match   = NULL;
   /*---(dependency doubly-linked list)--*/
   DEBUG_YCALC   yLOG_note    ("main list");
   /*> DEBUG_YCALC   yLOG_snote   ("main list");                                       <*/
   x_dep->dprev   = NULL;
   x_dep->dnext   = NULL;
   if (myCALC.dtail == NULL) {
      myCALC.dhead         = x_dep;
   } else {
      x_dep->dprev         = myCALC.dtail;
      x_dep->dnext         = NULL;
      myCALC.dtail->dnext  = x_dep;
   }
   myCALC.dtail         = x_dep;
   ++myCALC.dcount;
   DEBUG_YCALC   yLOG_value   ("dcount"    , myCALC.dcount);
   /*> DEBUG_YCALC   yLOG_sint    (myCALC.dcount);                                     <*/
   /*---(cell pointers)------------------*/
   DEBUG_YCALC   yLOG_note    ("cell pointers");
   /*> DEBUG_YCALC   yLOG_snote   ("cell pointers");                                   <*/
   x_dep->source  = NULL;
   x_dep->target  = NULL;
   /*---(statistics)---------------------*/
   DEBUG_YCALC   yLOG_note    ("stats");
   /*> DEBUG_YCALC   yLOG_snote   ("stats");                                           <*/
   x_dep->count   = 0;
   /*---(save)---------------------------*/
   DEBUG_YCALC   yLOG_note    ("save");
   /*> DEBUG_YCALC   yLOG_snote   ("save");                                            <*/
   *a_dep = x_dep;
   DEBUG_YCALC   yLOG_point   ("a_dep"     , a_dep);
   DEBUG_YCALC   yLOG_point   ("*a_dep"    , *a_dep);
   /*---(complete)-----------------------*/
   /*> DEBUG_YCALC   yLOG_sexit   (__FUNCTION__);                                      <*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> destroy a single dependency --------[ leaf   [fp.B42.102.E0]*/ /*-[11.0000.034.5]-*/ /*-[--.---.---.--]-*/
ycalc__deps_free        (tDEP_LINK **a_dep)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   tDEP_LINK  *x_dep       = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_senter  (__FUNCTION__);
   /*---(defense: null cell)-------------*/
   DEBUG_YCALC   yLOG_spoint  (a_dep);
   --rce;  if (a_dep       == NULL) {
      DEBUG_YCALC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_spoint  (*a_dep);
   --rce;  if (*a_dep      == NULL) {
      DEBUG_YCALC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_dep = *a_dep;
   /*---(remove from dependency list)----*/
   DEBUG_YCALC   yLOG_snote   ("main list");
   if (x_dep->dnext != NULL) x_dep->dnext->dprev     = x_dep->dprev;
   else                      myCALC.dtail            = x_dep->dprev;
   if (x_dep->dprev != NULL) x_dep->dprev->dnext     = x_dep->dnext;
   else                      myCALC.dhead            = x_dep->dnext;
   --myCALC.dcount;
   /*---(detatch from paired dep)--------*/
   DEBUG_YCALC   yLOG_snote   ("detach pair");
   if (x_dep->match != NULL) {
      x_dep->match->match  = NULL;
      x_dep->match         = NULL;
   }
   /*---(if require, take off cell)------*/
   if      (strchr (S_DEP_REQS, x_dep->type) != NULL) {
      DEBUG_YCALC   yLOG_snote   ("requires");
      if (x_dep->next  != NULL) x_dep->next->prev    = x_dep->prev;
      if (x_dep->prev  != NULL) x_dep->prev->next    = x_dep->next;
      else                      x_dep->source->reqs  = x_dep->next;
      --(x_dep->source->nreq);
   }
   /*---(if provide, take off cell)------*/
   else if (strchr (S_DEP_PROS, x_dep->type) != NULL) {
      DEBUG_YCALC   yLOG_snote   ("provides");
      if (x_dep->next  != NULL) x_dep->next->prev    = x_dep->prev;
      if (x_dep->prev  != NULL) x_dep->prev->next    = x_dep->next;
      else                      x_dep->source->pros  = x_dep->next;
      --(x_dep->source->npro);
   }
   /*---(free)---------------------------*/
   DEBUG_YCALC   yLOG_spoint  (*a_dep);
   free (*a_dep);
   *a_dep = NULL;
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char         /*-> destroy a single dependency --------[ leaf   [fp.B42.102.E0]*/ /*-[11.0000.034.5]-*/ /*-[--.---.---.--]-*/
ycalc__deps_freepair    (tDEP_LINK **a_dep)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   tDEP_LINK  *x_dep       = NULL;
   tDEP_LINK  *x_match     = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_senter  (__FUNCTION__);
   /*---(defense: null cell)-------------*/
   DEBUG_YCALC   yLOG_spoint  (a_dep);
   --rce;  if (a_dep       == NULL) {
      DEBUG_YCALC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_spoint  (*a_dep);
   --rce;  if (*a_dep      == NULL) {
      DEBUG_YCALC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_dep   = *a_dep;
   x_match = (*a_dep)->match;
   ycalc__deps_free (&x_dep);
   ycalc__deps_free (&x_match);
   *a_dep = NULL;
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        program level                         ----===*/
/*====================------------------------------------====================*/
static void      o___PROGRAM_________________o (void) {;}

char         /*-> prepare dependency capability ------[ shoot  [ge.G84.095.81]*/ /*-[02.0000.102.1]-*/ /*-[--.---.---.--]-*/
ycalc_deps_init         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           = 0;
   int         j           = 0;
   int         x_count     = 0;
   int         x_found     = 0;
   int         x_reqs      = 0;
   int         x_pros      = 0;
   int         t           [5];
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   myCALC.rroot  = NULL;
   myCALC.rhead  = NULL;
   myCALC.rtail  = NULL;
   myCALC.rcount = 0;
   myCALC.dhead  = NULL;
   myCALC.dtail  = NULL;
   myCALC.dcount = 0;
   ystrlcpy (S_DEP_REQS, "" , 10);
   ystrlcpy (S_DEP_PROS, "" , 10);
   ystrlcpy (S_DEP_LIKE, "l", 10);
   /*---(complete info table)------------*/
   --rce;
   for (i = 0; i < MAX_DEPTYPE; ++i) {
      DEBUG_YCALC   yLOG_char    ("type"      , g_dep_info [i].type);
      /*---(check for end)---------------*/
      if (g_dep_info [i].type == G_DEP_BLANK)  break;
      /*---(add to lists)----------------*/
      sprintf (t, "%c", g_dep_info [i].type);
      DEBUG_YCALC   yLOG_info    ("str type"  , t);
      DEBUG_YCALC   yLOG_char    ("dir"       , g_dep_info [i].dir);
      if      (g_dep_info [i].dir  == G_DEP_DIRREQ)  strcat (S_DEP_REQS, t);
      else if (g_dep_info [i].dir  == G_DEP_DIRPRO)  strcat (S_DEP_PROS, t);
      else {
         DEBUG_YCALC   yLOG_note    ("type direction not + or -");
         DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
         return rce;
      }
      /*---(find match)------------------*/
      ++x_count;
      for (j = 0; j < MAX_DEPTYPE; ++j) {
         if (g_dep_info [j].type != g_dep_info [i].match) continue;
         ++x_found;
         g_dep_info [i].match_index = j;
         DEBUG_YCALC   yLOG_char    ("match"     , g_dep_info [j].type);
         DEBUG_YCALC   yLOG_value   ("match_indx", g_dep_info [i].match_index);
      }
   }
   DEBUG_YCALC   yLOG_value   ("x_count"   , x_count);
   DEBUG_YCALC   yLOG_value   ("x_found"   , x_found);
   --rce;  if (x_found != x_count) {
      DEBUG_YCALC   yLOG_note    ("could not match all dep types");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(report out)---------------------*/
   DEBUG_YCALC   yLOG_info    ("S_DEP_REQS", S_DEP_REQS);
   DEBUG_YCALC   yLOG_info    ("S_DEP_PROS", S_DEP_PROS);
   DEBUG_YCALC   yLOG_info    ("S_DEP_LIKE", S_DEP_LIKE);
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> tbd --------------------------------[ ------ [fz.842.041.24]*/ /*-[01.0000.013.T]-*/ /*-[--.---.---.--]-*/
ycalc__deproot_wipe     (void *a_owner, void *a_deproot, int a_seq, int a_lvl)
{
   yCALC_disable (&a_owner, &a_deproot);
   return 0;
}

char         /*-> destory all dependencies -----------[ ------ [fe.841.032.21]*/ /*-[01.0000.013.5]-*/ /*-[--.---.---.--]-*/
ycalc__deps_purge       (void)
{
   /*---(locals)-----------+-----+-----+-*/
   tDEP_LINK  *l_curr      = NULL;
   tDEP_LINK  *l_next      = NULL;
   tDEP_ROOT  *r_curr      = NULL;
   tDEP_ROOT  *r_next      = NULL;
   int         c           =    0;
   long        x_stamp     =    0;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   DEBUG_YCALC   yLOG_note    ("INITIAL CONDITIONS");
   DEBUG_YCALC   yLOG_point   ("rroot"     , myCALC.rroot);
   DEBUG_YCALC   yLOG_point   ("rhead"     , myCALC.rhead);
   DEBUG_YCALC   yLOG_point   ("rtail"     , myCALC.rtail);
   DEBUG_YCALC   yLOG_value   ("rcount"    , myCALC.rcount);
   DEBUG_YCALC   yLOG_point   ("dhead"     , myCALC.dhead);
   DEBUG_YCALC   yLOG_point   ("dtail"     , myCALC.dtail);
   DEBUG_YCALC   yLOG_value   ("dcount"    , myCALC.dcount);
   /*---(clear sequencing)---------------*/
   ycalc__seq_clear ();
   /*---(disconnect dependent cells)-----*/
   x_stamp = rand ();
   /*> rc = yCALC_seq_downdown (x_stamp, ycalc__deproot_wipe);                        <*/
   rc = yCALC_seq_downup   (x_stamp, ycalc__deproot_wipe);
   DEBUG_YCALC   yLOG_note    ("AFTER SEQUENCED DEPROOT DELETE");
   DEBUG_YCALC   yLOG_point   ("rroot"     , myCALC.rroot);
   DEBUG_YCALC   yLOG_point   ("rhead"     , myCALC.rhead);
   DEBUG_YCALC   yLOG_point   ("rtail"     , myCALC.rtail);
   DEBUG_YCALC   yLOG_value   ("rcount"    , myCALC.rcount);
   DEBUG_YCALC   yLOG_point   ("dhead"     , myCALC.dhead);
   DEBUG_YCALC   yLOG_point   ("dtail"     , myCALC.dtail);
   DEBUG_YCALC   yLOG_value   ("dcount"    , myCALC.dcount);
   /*---(walk through remaining)---------*/
   r_curr = myCALC.rtail;
   c      = 0;
   while (r_curr != NULL) {
      r_next = r_curr->rprev;
      yCALC_disable (&r_curr->owner, &r_curr);
      r_curr = r_next;
      ++c;
   }
   DEBUG_YCALC   yLOG_note    ("AFTER ALL DEPROOT DELETE");
   DEBUG_YCALC   yLOG_value   ("purged"    , c);
   DEBUG_YCALC   yLOG_point   ("rroot"     , myCALC.rroot);
   DEBUG_YCALC   yLOG_point   ("rhead"     , myCALC.rhead);
   DEBUG_YCALC   yLOG_point   ("rtail"     , myCALC.rtail);
   DEBUG_YCALC   yLOG_value   ("rcount"    , myCALC.rcount);
   DEBUG_YCALC   yLOG_point   ("dhead"     , myCALC.dhead);
   DEBUG_YCALC   yLOG_point   ("dtail"     , myCALC.dtail);
   DEBUG_YCALC   yLOG_value   ("dcount"    , myCALC.dcount);
   /*---(reset globals)------------------*/
   myCALC.rroot   = NULL;
   myCALC.rhead   = NULL;
   myCALC.rtail   = NULL;
   myCALC.rcount  = 0;
   DEBUG_YCALC   yLOG_note    ("AFTER RESET GLOBALS");
   DEBUG_YCALC   yLOG_point   ("rroot"     , myCALC.rroot);
   DEBUG_YCALC   yLOG_point   ("rhead"     , myCALC.rhead);
   DEBUG_YCALC   yLOG_point   ("rtail"     , myCALC.rtail);
   DEBUG_YCALC   yLOG_value   ("rcount"    , myCALC.rcount);
   /*---(walk through links)-------------*/
   l_curr = myCALC.dtail;
   c      = 0;
   while (l_curr != NULL) {
      l_next = l_curr->dprev;
      ycalc__deps_free (&l_curr);
      /*> ycalc__deps_freepair (&l_curr);                                             <*/
      l_curr = l_next;
      ++c;
   }
   DEBUG_YCALC   yLOG_note    ("AFTER ALL DEPLINK DELETE");
   DEBUG_YCALC   yLOG_value   ("purged"    , c);
   DEBUG_YCALC   yLOG_point   ("dhead"     , myCALC.dhead);
   DEBUG_YCALC   yLOG_point   ("dtail"     , myCALC.dtail);
   DEBUG_YCALC   yLOG_value   ("dcount"    , myCALC.dcount);
   /*---(reset globals)------------------*/
   myCALC.dhead   = NULL;
   myCALC.dtail   = NULL;
   myCALC.dcount  = 0;
   DEBUG_YCALC   yLOG_note    ("AFTER RESET GLOBALS");
   DEBUG_YCALC   yLOG_point   ("dhead"     , myCALC.dhead);
   DEBUG_YCALC   yLOG_point   ("dtail"     , myCALC.dtail);
   DEBUG_YCALC   yLOG_value   ("dcount"    , myCALC.dcount);
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> shut down dependency usage ---------[ shoot  [gz.421.001.02]*/ /*-[00.0000.102.1]-*/ /*-[--.---.---.--]-*/
ycalc_deps_wrap         (void)
{
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(dependencies)-------------------*/
   ycalc__deps_purge ();
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                    table lookup functions                    ----===*/
/*====================------------------------------------====================*/
static void  o___TABLE___________o () { return; }

char         /*-> find the index of a dep type -------[ leaf   [fc.320.112.10]*/ /*-[01.0000.026.!]-*/ /*-[--.---.---.--]-*/
ycalc__deps_index       (char a_type)
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;
   /*---(search)-------------------------*/
   for (i = 0; i < MAX_DEPTYPE; ++i) {
      if (g_dep_info [i].type != a_type) continue;
      return i;
   }
   /*---(failure)------------------------*/
   return -1;
}

char         /*-> find index of a dep match type -----[ leaf   [fc.320.112.10]*/ /*-[01.0000.026.!]-*/ /*-[--.---.---.--]-*/
ycalc__deps_match       (char a_type)
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;
   /*---(search)-------------------------*/
   for (i = 0; i < MAX_DEPTYPE; ++i) {
      if (g_dep_info [i].type != a_type) continue;
      return g_dep_info [i].match_index;
   }
   /*---(failure)------------------------*/
   return -1;
}

char         /*-> find index of a dep match type -----[ leaf   [fc.310.112.10]*/ /*-[01.0000.018.!]-*/ /*-[--.---.---.--]-*/
ycalc__deps_abbr        (char a_index)
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;
   /*---(complete)-----------------------*/
   if (a_index >= 0 && a_index < MAX_DEPTYPE) {
      return g_dep_info [a_index].type;
   }
   /*---(failure)------------------------*/
   return -1;
}



/*====================------------------------------------====================*/
/*===----                        shared functions                      ----===*/
/*====================------------------------------------====================*/
static void  o___SHARED__________o () { return; }

char
ycalc_deps_validate     (char a_type, tDEP_ROOT **a_source, tDEP_ROOT **a_target)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;          /* return code for errors         */
   char        rc          =    0;          /* generic return code            */
   char        x_index     =    0;          /* dependency type table index    */
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense source)-----------------*/
   DEBUG_YCALC   yLOG_point   ("a_source"  , a_source);
   --rce;  if (a_source     == NULL)   {
      DEBUG_YCALC yLOG_note      ("source pointer is null");
      DEBUG_YCALC yLOG_exitr     (__FUNCTION__, rce);
      return rce; 
   }
   DEBUG_YCALC   yLOG_point   ("*a_source" , *a_source);
   --rce;  if (*a_source    == NULL)   {
      DEBUG_YCALC yLOG_note      ("source is null");
      DEBUG_YCALC yLOG_exitr     (__FUNCTION__, rce);
      return rce; 
   }
   DEBUG_YCALC   yLOG_info    ("label"     , ycalc_call_labeler (*a_source));
   DEBUG_YCALC   yLOG_value   ("nreq"      , (*a_source)->nreq);
   /*---(defense target)-----------------*/
   DEBUG_YCALC   yLOG_point   ("a_target"  , a_target);
   --rce;  if (a_target     == NULL)   {
      DEBUG_YCALC yLOG_note      ("target pointer is null");
      DEBUG_YCALC yLOG_exitr     (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("*a_target" , *a_target);
   --rce;  if (*a_target     == NULL)   {
      DEBUG_YCALC yLOG_note      ("target is null");
      DEBUG_YCALC yLOG_exitr     (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_note    ("checking");
   /*> DEBUG_YCALC   yLOG_info    ("label"     , ycalc_call_labeler (*a_target));     <*/
   DEBUG_YCALC   yLOG_value   ("npro"      , (*a_target)->npro);
   /*---(check link back to root)--------*/
   --rce;  if (*a_target == myCALC.rroot)  {
      DEBUG_YCALC yLOG_note      ("can not delete a route back to root");
      DEBUG_YCALC yLOG_exitr     (__FUNCTION__, rce);
      return rce;
   }
   /*---(check circlular link)-----------*/
   --rce;  if (*a_source == *a_target) {
      DEBUG_YCALC   yLOG_note    ("source and target are same");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check type)---------------------*/
   DEBUG_YCALC   yLOG_char    ("a_type"    , a_type);
   x_index     = ycalc__deps_index (a_type);
   DEBUG_YCALC   yLOG_value   ("dep index" , x_index);
   --rce;  if (x_index      <  0) {
      DEBUG_YCALC   yLOG_note    ("dependency type not found");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return x_index;
}



/*====================------------------------------------====================*/
/*===----                      create a dependency                     ----===*/
/*====================------------------------------------====================*/
static void  o___CREATE__________o () { return; }

tDEP_LINK*   /*-> create a requires dependency -------[ ------ [fp.C43.123.61]*/ /*-[02.0000.017.!]-*/ /*-[--.---.---.--]-*/
ycalc__deps_create_req  (char a_type, char a_index, tDEP_ROOT **a_source, tDEP_ROOT **a_target)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   tDEP_LINK  *x_next      = NULL;          /* iterator -- dependency lists   */
   tDEP_LINK  *x_req       = NULL;          /* new requires entry             */
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(check for existing requires)----*/
   DEBUG_YCALC   yLOG_info    ("source"    , ycalc_call_labeler (*a_source));
   DEBUG_YCALC   yLOG_info    ("target"    , ycalc_call_labeler (*a_target));
   DEBUG_YCALC   yLOG_value   ("src nreqs" , (*a_source)->nreq);
   x_next = (*a_source)->reqs;
   DEBUG_YCALC   yLOG_point   ("src head"  , (*a_source)->reqs);
   while (x_next != NULL) {
      DEBUG_YCALC   yLOG_info    ("check"     , ycalc_call_labeler (x_next->target));
      if (x_next->target == *a_target) {
         DEBUG_YCALC   yLOG_note    ("found existsing updating count");
         DEBUG_YCALC   yLOG_value   ("found"     , ++x_next->count);
         DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
         return x_next;
      }
      x_next = x_next->next;
   }
   /*---(create a new requires)-------*/
   DEBUG_YCALC   yLOG_note    ("existing not found, create new reqs");
   rc = ycalc__deps_new (&x_req);
   DEBUG_YCALC   yLOG_value   ("rc"        , rc);
   if (rc < 0 || x_req == NULL)   {
      DEBUG_YCALC yLOG_note      ("could not make a new reqs entry");
      DEBUG_YCALC yLOG_exit      (__FUNCTION__);
      return NULL; 
   }
   /*---(assign basics)---------------*/
   DEBUG_YCALC   yLOG_note    ("assign basic values");
   x_req->type     = a_type;
   x_req->source   = *a_source;
   x_req->target   = *a_target;
   x_req->count    = 1;
   /*---(add to dep counters)---------*/
   DEBUG_YCALC   yLOG_note    ("increment dependency table counters");
   ++(g_dep_info [a_index].count);
   ++(g_dep_info [a_index].total);
   /*---(hook it up to cell)----------*/
   DEBUG_YCALC   yLOG_note    ("hook up dependency into root linked list");
   if ((*a_source)->reqs == NULL) {
      (*a_source)->reqs = x_req;
   } else {
      x_next = (*a_source)->reqs;
      while (x_next->next != NULL)  x_next = x_next->next;
      x_next->next         = x_req;
      x_req->prev          = x_next;
   }
   ++(*a_source)->nreq;
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_point   ("x_req"     , x_req);
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return x_req;
}

tDEP_LINK*   /*-> create a provides dependency -------[ ------ [fp.C43.123.62]*/ /*-[02.0000.017.!]-*/ /*-[--.---.---.--]-*/
ycalc__deps_create_pro  (char a_type, char a_index, tDEP_ROOT **a_source, tDEP_ROOT **a_target)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          =    0;
   tDEP_LINK  *x_next      = NULL;          /* iterator -- dependency lists   */
   tDEP_LINK  *x_pro       = NULL;          /* new requires entry             */
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(check for existing requires)----*/
   DEBUG_YCALC   yLOG_info    ("source"    , ycalc_call_labeler (*a_source));
   DEBUG_YCALC   yLOG_info    ("target"    , ycalc_call_labeler (*a_target));
   DEBUG_YCALC   yLOG_value   ("src npros" , (*a_source)->npro);
   x_next = (*a_source)->pros;
   while (x_next != NULL) {
      DEBUG_YCALC   yLOG_info    ("check"     , ycalc_call_labeler (x_next->target));
      if (x_next->target == *a_target) {
         DEBUG_YCALC   yLOG_note    ("found existsing updating count");
         DEBUG_YCALC   yLOG_value   ("found"     , ++x_next->count);
         DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
         return x_next;
      }
      x_next = x_next->next;
   }
   /*---(create a new provides)-------*/
   DEBUG_YCALC   yLOG_note    ("existing not found, create new requires");
   rc = ycalc__deps_new (&x_pro);
   DEBUG_YCALC   yLOG_value   ("rc"        , rc);
   if (rc < 0 || x_pro == NULL)   {
      DEBUG_YCALC yLOG_note      ("could not make a new pros entry");
      DEBUG_YCALC yLOG_exit      (__FUNCTION__);
      return NULL; 
   }
   /*---(assign basics)---------------*/
   DEBUG_YCALC   yLOG_note    ("assign basic values");
   x_pro->type     = ycalc__deps_abbr (a_index);
   x_pro->source   = *a_source;
   x_pro->target   = *a_target;
   x_pro->count    = 1;
   /*---(add to dep counters)---------*/
   DEBUG_YCALC   yLOG_note    ("increment dependency table counters");
   ++(g_dep_info [a_index].count);
   ++(g_dep_info [a_index].total);
   /*---(hook it up to cell)----------*/
   DEBUG_YCALC   yLOG_note    ("hook up dependency into cell linked list");
   if ((*a_source)->pros == NULL) {
      (*a_source)->pros = x_pro;
   } else {
      x_next = (*a_source)->pros;
      while (x_next->next != NULL)  x_next = x_next->next;
      x_next->next         = x_pro;
      x_pro->prev          = x_next;
   }
   ++(*a_source)->npro;
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_point   ("x_pro"     , x_pro);
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return x_pro;
}

char         /*-> create a two-way dependency --------[ ------ [ge.M88.15#.B6]*/ /*-[01.0000.526.H]-*/ /*-[--.---.---.--]-*/
ycalc_deps_create       (char a_type, tDEP_ROOT **a_source, tDEP_ROOT **a_target)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;          /* return code for errors         */
   char        rc          =    0;          /* generic return code            */
   char        x_index     =    0;          /* dependency type table index    */
   tDEP_LINK  *x_require   = NULL;    /* new requires entry                   */
   tDEP_LINK  *x_provide   = NULL;    /* new provides entry                   */
   char        x_dir       =  '-';
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   rc = ycalc_deps_validate (a_type, a_source, a_target);
   DEBUG_YCALC   yLOG_value   ("validate"  , rc);
   --rce;  if (rc <  0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_index = rc;
   x_dir = g_dep_info [x_index].dir;
   DEBUG_YCALC   yLOG_char    ("dir"       , x_dir);
   --rce;  if (x_dir != G_DEP_DIRREQ) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check for circlular ref)--------*/
   DEBUG_YCALC   yLOG_note    ("check for potential circular reference");
   rc     = ycalc__deps_circle (0, *a_target, *a_source, rand());
   DEBUG_YCALC   yLOG_value   ("circle rc" , rc);
   --rce;  if (rc  <  0) {
      DEBUG_YCALC   yLOG_note    ("request would cause a circular reference");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, -100);
      return -100;
   }
   /*---(check if target needs unroot)---*/
   DEBUG_YCALC   yLOG_note    ("check target for unrooting");
   rc = ycalc__deps_rooting (*a_target, G_DEP_UNROOT);
   if (rc <  0) {
      DEBUG_YCALC   yLOG_note    ("target could not be properly unrooted");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(create require)-----------------*/
   x_require   = ycalc__deps_create_req (a_type, x_index, a_source, a_target);
   --rce;  if (x_require    == NULL    ) {
      DEBUG_YCALC   yLOG_note    ("requires did not get made");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(create provide)-----------------*/
   x_index     = ycalc__deps_match (a_type);
   x_provide   = ycalc__deps_create_pro (a_type, x_index, a_target, a_source);
   --rce;  if (x_require    == NULL    ) {
      DEBUG_YCALC   yLOG_note    ("provides did not get made");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(cross link them)----------------*/
   DEBUG_YCALC   yLOG_note    ("link require and provide together");
   x_require->match     = x_provide;
   x_provide->match     = x_require;
   /*---(check if source needs rooting)--*/
   DEBUG_YCALC   yLOG_note    ("check source for rooting");
   rc = ycalc__deps_rooting (*a_source, G_DEP_ROOT);
   --rce;  if (rc <  0) {
      DEBUG_YCALC   yLOG_note    ("source could not be properly rooted");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_note    ("all actions complete");
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> create a two-way dependency --------[ ------ [ge.M88.15#.B6]*/ /*-[01.0000.526.H]-*/ /*-[--.---.---.--]-*/
yCALC_create       (char a_type, char *a_source, char *a_target)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   char        rc          = 0;
   tDEP_ROOT  *x_source    = NULL;
   tDEP_ROOT  *x_target    = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(get ends)-----------------------*/
   rc = ycalc_call_who_named (a_source, YCALC_FULL, NULL, &x_source);
   --rce;  if (rc <  0) {
      DEBUG_YCALC   yLOG_note    ("source could not be identified");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   rc = ycalc_call_who_named (a_target, YCALC_FULL, NULL, &x_target);
   --rce;  if (rc <  0) {
      DEBUG_YCALC   yLOG_note    ("target could not be identified");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(call real function)-------------*/
   rc = ycalc_deps_create (a_type, &x_source, &x_target);
   --rce;  if (rc <  0) {
      DEBUG_YCALC   yLOG_note    ("could not create dependency");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        deleting links                        ----===*/
/*====================------------------------------------====================*/
static void  o___DELETE__________o () { return; }

char         /*-> remove a requires dependency -------[ ------ [fe.933.143.31]*/ /*-[03.0000.016.!]-*/ /*-[--.---.---.--]-*/
ycalc__deps_delete_req  (char a_type, char a_index, tDEP_ROOT **a_source, tDEP_ROOT **a_target)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   char        rc          = 0;
   tDEP_LINK  *x_next      = NULL;          /* iterator -- dependency lists   */
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(check for existing requires)----*/
   DEBUG_YCALC   yLOG_point   ("a_source"  , *a_source);
   DEBUG_YCALC   yLOG_info    ("...label"  , ycalc_call_labeler (*a_source));
   DEBUG_YCALC   yLOG_point   ("a_target"  , *a_target);
   DEBUG_YCALC   yLOG_info    ("...label"  , ycalc_call_labeler (*a_target));
   DEBUG_YCALC   yLOG_value   ("src nreqs" , (*a_source)->nreq);
   x_next = (*a_source)->reqs;
   --rce; while (x_next != NULL) {
      DEBUG_YCALC   yLOG_info    ("review"    , ycalc_call_labeler (x_next->target));
      if (x_next->target == *a_target) {
         DEBUG_YCALC   yLOG_note    ("found existing");
         rc = ycalc__deps_free (&x_next);
         /*> rc = ycalc__deps_freepair (&x_next);                                     <*/
         if (rc != 0)  {
            DEBUG_YCALC   yLOG_note    ("deletion failed");
            DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
            return rce;
         }
         DEBUG_YCALC   yLOG_note    ("update dependency table totals");
         --(g_dep_info [a_index].count);
         DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
         return 0;
      }
      x_next = x_next->next;
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_note    ("did not find dependency");
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   --rce;
   return rce;
}

char         /*-> remove a provides dependency -------[ ------ [fe.933.143.31]*/ /*-[03.0000.016.!]-*/ /*-[--.---.---.--]-*/
ycalc__deps_delete_pro  (char a_type, char a_index, tDEP_ROOT **a_source, tDEP_ROOT **a_target)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   char        rc          = 0;
   tDEP_LINK  *x_next      = NULL;          /* iterator -- dependency lists   */
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(check for existing requires)----*/
   DEBUG_YCALC   yLOG_point   ("a_source"  , *a_source);
   DEBUG_YCALC   yLOG_info    ("...label"  , ycalc_call_labeler (*a_source));
   DEBUG_YCALC   yLOG_point   ("a_target"  , *a_target);
   DEBUG_YCALC   yLOG_info    ("...label"  , ycalc_call_labeler (*a_target));
   DEBUG_YCALC   yLOG_value   ("nprovide"  , (*a_source)->npro);
   x_next = (*a_source)->pros;
   --rce; while (x_next != NULL) {
      DEBUG_YCALC   yLOG_info    ("review"    , ycalc_call_labeler (x_next->target));
      if (x_next->target == *a_target) {
         DEBUG_YCALC   yLOG_note    ("found existing");
         rc = ycalc__deps_free (&x_next);
         /*> rc = ycalc__deps_freepair (&x_next);                                     <*/
         if (rc != 0)  {
            DEBUG_YCALC   yLOG_note    ("deletion failed");
            DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
            return rce;
         }
         DEBUG_YCALC   yLOG_note    ("update dependency table totals");
         --(g_dep_info [a_index].count);
         DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
         return 0;
      }
      x_next = x_next->next;
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_note    ("did not find dependency");
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   --rce;
   return rce;
}

char         /*-> remove a two-way dependency --------[ ------ [ge.P79.15#.I6]*/ /*-[02.0000.345.3]-*/ /*-[--.---.---.--]-*/
ycalc_deps_delete       (char a_type, tDEP_ROOT **a_source, tDEP_ROOT **a_target, void **a_finish)
{
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;          /* return code for errors         */
   char        rc          =    0;          /* generic return code            */
   char        x_index     =    0;          /* dependency type table index    */
   char        x_type      =  ' ';
   tDEP_LINK  *x_next      = NULL;    /* new provides entry                   */
   int         x_proreal   =    0;
   char        x_dir       =  '-';
   char       *x_realtype  = NULL;
   char        x_label     [LEN_LABEL] = "";
   char      **x_source    = NULL;
   /*---(defense)------------------------*/
   rc = ycalc_deps_validate (a_type, a_source, a_target);
   DEBUG_YCALC   yLOG_value   ("validate"  , rc);
   --rce;  if (rc <  0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_index = rc;
   x_dir = g_dep_info [x_index].dir;
   DEBUG_YCALC   yLOG_char    ("dir"       , x_dir);
   --rce;  if (x_dir != G_DEP_DIRREQ) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("a_finish"  , a_finish);
   if (a_finish != NULL) {
      DEBUG_YCALC   yLOG_point   ("*a_finish" , *a_finish);
   }
   /*---(delete require)-----------------*/
   rc   = ycalc__deps_delete_req (a_type, x_index, a_source, a_target);
   --rce;  if (rc <  0) {
      DEBUG_YCALC   yLOG_note    ("requires did not get removed");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YCALC   yLOG_value   ("nrequire"  , (*a_source)->nreq);
   /*---(delete provide)-----------------*/
   x_index     = ycalc__deps_match (a_type);
   DEBUG_YCALC   yLOG_value   ("dep index" , x_index);
   rc   = ycalc__deps_delete_pro (a_type, x_index, a_target, a_source);
   --rce;  if (rc <  0) {
      DEBUG_YCALC   yLOG_note    ("provides did not get removed");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YCALC   yLOG_value   ("nprovide"  , (*a_target)->npro);
   /*---(check on source)----------------*/
   DEBUG_YCALC   yLOG_note    ("REVIEW THE SOURCE POINT");
   myCALC.e_valuer ((*a_source)->owner, &x_type, NULL, NULL);
   DEBUG_YCALC   yLOG_complex ("source"    , "%-10.10p, %-8.8s, %c", (*a_source)->owner, ycalc_call_labeler (*a_source), x_type);
   --rce;
   if        (*a_source  == myCALC.rroot) {
      DEBUG_YCALC   yLOG_note    ("source is root (root), don't mess with it");
   } else if ((*a_source)->nreq >  0) {
      DEBUG_YCALC   yLOG_note    ("source has requires (branch), so leave alone");
   } else {
      DEBUG_YCALC   yLOG_note    ("source is independent (leaf), must check unroot");
      rc = ycalc__deps_rooting (*a_source, G_DEP_UNROOT);
      if (rc <  0) {
         DEBUG_YCALC   yLOG_note    ("source could not be properly unrooted");
         DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
         return rce;
      }
   }
   /*---(check on target)----------------*/
   DEBUG_YCALC   yLOG_note    ("review the target");
   ystrlcpy (x_label, ycalc_call_labeler (*a_target), LEN_LABEL);
   DEBUG_YCALC   yLOG_info    ("target"    , x_label);
   x_next = (*a_target)->pros;
   while (x_next != NULL) {
      if (x_next->type != G_DEP_ENTRY)  ++x_proreal;
      x_next = x_next->next;
   }
   DEBUG_YCALC   yLOG_value   ("x_proreal" , x_proreal);
   myCALC.e_valuer ((*a_target)->owner, &x_type, NULL, NULL);
   /*---(already well connected)---------*/
   if        (*a_source  == myCALC.rroot) {
      DEBUG_YCALC   yLOG_note    ("source was root, so now independent");
   }
   else if (x_proreal >  0) {
      DEBUG_YCALC   yLOG_note    ("target already hard-connected to tree");
   }
   /*---(not connected at all)-----------*/
   else if ((*a_target)->npro == 0 && (*a_target)->nreq >  0) {
      DEBUG_YCALC   yLOG_note    ("target is still needed, must root");
      rc = ycalc__deps_rooting (*a_target, G_DEP_ROOT);
      if (rc <  0) {
         DEBUG_YCALC   yLOG_note    ("target could not be properly rooted");
         DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
         return rce;
      }
   }
   /*---(range-connected only)-----------*/
   else if ((*a_target)->nreq >  0) {
      DEBUG_YCALC   yLOG_note    ("leave attached as is");
   }
   else if (x_type == '-') {
      DEBUG_YCALC   yLOG_note    ("target is a zombie and not required, delete");
      rc = ycalc_call_reaper (a_finish, a_target);
      if (rc != 0) {
         DEBUG_YCALC   yLOG_note    ("target could not be properly deleted");
         DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
         return rce;
      }
      *a_target == NULL;
   }
   else {
      DEBUG_YCALC   yLOG_note    ("target is now independent");
      if (x_type == YCALC_DATA_MERGED) {
         DEBUG_YCALC   yLOG_note    ("floating merge indicator, evaluate");
         rc = myCALC.e_pointer ((*a_source)->owner, &x_source, NULL, NULL, NULL);
         DEBUG_YCALC   yLOG_info    ("*x_source" , *x_source);
         rc = myCALC.e_pointer ((*a_target)->owner, NULL, &x_realtype, NULL, NULL);
         DEBUG_YCALC   yLOG_char    ("x_realtype", *x_realtype);
         if (*x_source == NULL || strcmp (*x_source, "") == 0) {
            *x_realtype = YCALC_DATA_GARBAGE;
         }
      }
      rc = ycalc_call_reaper (a_finish, a_target);
      if (rc != 0) {
         DEBUG_YCALC   yLOG_note    ("target could not be properly deleted");
         DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
         return rce;
      }
      /*> if (rc != 0) {                                                                       <* 
       *>    if ((*a_target)->npro == 0) {                                                     <* 
       *>       rc = ycalc__deps_rooting (*a_target, G_DEP_ROOT);                              <* 
       *>       if (rc <  0) {                                                                 <* 
       *>          DEBUG_YCALC   yLOG_note    ("target could not be properly rooted");          <* 
       *>          DEBUG_YCALC   yLOG_exit    (__FUNCTION__);                                   <* 
       *>          return rce;                                                                 <* 
       *>       }                                                                              <* 
       *>       if (x_type == YCALC_DATA_MERGED) {                                             <* 
       *>          DEBUG_YCALC   yLOG_note    ("floating merge indicator, make into string");   <* 
       *>          rc = myCALC.e_pointer ((*a_target)->owner, NULL, &x_realtype, NULL, NULL);         <* 
       *>          *x_realtype = YCALC_DATA_STR;                                               <* 
       *>          yCALC_handle (x_label);                                                     <* 
       *>       }                                                                              <* 
       *>    } else {                                                                          <* 
       *>       DEBUG_YCALC   yLOG_note    ("target could not be properly deleted");            <* 
       *>       DEBUG_YCALC   yLOG_exit    (__FUNCTION__);                                      <* 
       *>       return rce;                                                                    <* 
       *>    }                                                                                 <* 
       *> }                                                                                    <*/
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_note    ("all actions complete");
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> delete a two-way dependency --------[ ------ [ge.M88.15#.B6]*/ /*-[01.0000.526.H]-*/ /*-[--.---.---.--]-*/
yCALC_delete       (char a_type, char *a_source, char *a_target)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   char        rc          = 0;
   tDEP_ROOT  *x_source    = NULL;
   tDEP_ROOT  *x_target    = NULL;
   void       *x_finish    = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(get ends)-----------------------*/
   rc = ycalc_call_who_named (a_source, YCALC_OWNR, NULL, &x_source);
   --rce;  if (rc <  0) {
      DEBUG_YCALC   yLOG_note    ("source could not be identified");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   rc = ycalc_call_who_named (a_target, YCALC_OWNR, &x_finish, &x_target);
   --rce;  if (rc <  0) {
      DEBUG_YCALC   yLOG_note    ("target could not be identified");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(call real function)-------------*/
   rc = ycalc_deps_delete (a_type, &x_source, &x_target, &x_finish);
   --rce;  if (rc <  0) {
      DEBUG_YCALC   yLOG_note    ("could not delete dependency");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> remove a two-way dependency --------[ ------ [ge.833.132.31]*/ /*-[02.0000.10#.!]-*/ /*-[--.---.---.--]-*/
ycalc_deps_delcalcref   (tDEP_ROOT *a_deproot)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   tDEP_LINK  *x_next      = NULL;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense: null pointers)---------*/
   DEBUG_YCALC   yLOG_info    ("DEFENSES"  , "make sure this is processable");
   DEBUG_YCALC   yLOG_point   ("a_deproot" , a_deproot);
   --rce;  if (a_deproot     == NULL)   {
      DEBUG_YCALC   yLOG_value   ("FAILED"    , rce);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YCALC   yLOG_info    ("label"     , ycalc_call_labeler (a_deproot));
   /*---(review calculated deps)---------*/
   --rce;
   x_next = a_deproot->reqs;
   while (x_next != NULL) {
      DEBUG_YCALC   yLOG_info    ("label"     , ycalc_call_labeler (x_next->target));
      DEBUG_YCALC   yLOG_char    ("type"      , x_next->type);
      if (x_next->type != G_DEP_CALCREF) {
         DEBUG_YCALC   yLOG_note    ("wrong type, skipping");
      } else {
         rc = ycalc_deps_delete (G_DEP_CALCREF, &(a_deproot), &(x_next->target), &(x_next->target->owner));
      }
      x_next = x_next->next;
   }
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> remove a two-way dependency --------[ ------ [ge.833.132.31]*/ /*-[02.0000.10#.!]-*/ /*-[--.---.---.--]-*/
ycalc_deps_delvar       (tDEP_ROOT **a_deproot)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   tDEP_LINK  *x_next      = NULL;
   char        rc          =    0;
   tDEP_LINK  *x_deproot   = NULL;
   void       *x_other     = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense: null pointers)---------*/
   DEBUG_YCALC   yLOG_info    ("DEFENSES"  , "make sure this is processable");
   DEBUG_YCALC   yLOG_point   ("a_deproot" , a_deproot);
   --rce;  if (a_deproot     == NULL)   {
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("*a_deproot", *a_deproot);
   --rce;  if (*a_deproot    == NULL)   {
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YCALC   yLOG_info    ("label"     , ycalc_call_labeler (*a_deproot));
   /*---(review calculated deps)---------*/
   DEBUG_YCALC   yLOG_value   ("nreq"      , (*a_deproot)->nreq);
   DEBUG_YCALC   yLOG_value   ("npro"      , (*a_deproot)->npro);
   --rce;
   x_next = (*a_deproot)->pros;
   while (x_next != NULL) {
      DEBUG_YCALC   yLOG_info    ("label"     , ycalc_call_labeler (x_next->target));
      DEBUG_YCALC   yLOG_char    ("type"      , x_next->type);
      if (x_next->type != G_DEP_TITLE) {
         DEBUG_YCALC   yLOG_note    ("wrong type, skipping");
      } else {
         DEBUG_YCALC   yLOG_note    ("going for the delete");
         x_deproot =  x_next->target;
         x_other   =  x_next->target->owner;
         DEBUG_YCALC   yLOG_point   ("x_deproot" , x_deproot);
         DEBUG_YCALC   yLOG_point   ("x_other"   , x_other);
         rc = ycalc_deps_delete (G_DEP_CONTENT, &(x_deproot), a_deproot, a_deproot);
         DEBUG_YCALC   yLOG_note    ("then, cleanup as necessary");
         rc = ycalc_call_reaper (&x_other, &x_deproot);
         DEBUG_YCALC   yLOG_value   ("reaper"    , rc);
      }
      x_next = x_next->next;
   }
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     rooting and unrooting                    ----===*/
/*====================------------------------------------====================*/
static void  o___ROOTING_________o () { return; }

char         /*-> root and unroot cells --------------[ ------ [ge.P86.258.H2]*/ /*-[03.0000.125.!]-*/ /*-[--.---.---.--]-*/
ycalc__deps_rooting     (tDEP_ROOT *a_curr, char a_type)
{  /*---(design notes)-------------------*/
   /*
    *   since root can carry a huge number of dependencies, this search checks
    *   upward from the target for speed.
    *      a = audit/check root
    *      u = unroot
    *      r = root
    *
    */
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   char        rc          = 0;
   tDEP_LINK  *x_next      = NULL;
   int         x_norms     = 0;
   int         x_roots     = 0;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defenses)-----------------------*/
   --rce;  if (a_curr == NULL) {
      DEBUG_YCALC   yLOG_note    ("curr is null, FAIL");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   --rce;  if (a_curr == myCALC.rroot) {
      DEBUG_YCALC   yLOG_note    ("curr is root, nothing to do");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return 1;
   }
   DEBUG_YCALC   yLOG_value   ("owners"    , a_curr->npro);
   /*---(quick path, already rooted)-----*/
   --rce;
   if (a_curr->npro == 1 && a_curr->pros->target == myCALC.rroot) {
      switch (a_type) {
      case G_DEP_ROOT      : 
      case G_DEP_CHECKROOT : 
         DEBUG_YCALC   yLOG_note    ("perfectly rooted");
         break;
      case G_DEP_UNROOT    : 
         DEBUG_YCALC   yLOG_note    ("perfect root found, unrooting");
         rc = ycalc_deps_delete (G_DEP_REQUIRE, &(myCALC.rroot), &a_curr, NULL);
         if (rc != 0) {
            DEBUG_YCALC   yLOG_note    ("could not delete root dependency");
            DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
            return rce;
         }
         DEBUG_YCALC   yLOG_value   ("owners"    , a_curr->npro);
         break;
      }
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return a_curr->npro;
   }
   /*---(indenpendent cell)--------------*/
   --rce;
   if (a_curr->npro == 0) {
      switch (a_type) {
      case G_DEP_ROOT      : 
         DEBUG_YCALC   yLOG_note    ("independent cell, rooting");
         rc = ycalc_deps_create (G_DEP_REQUIRE, &(myCALC.rroot), &a_curr);
         if (rc != 0) {
            DEBUG_YCALC   yLOG_note    ("could not create root dependency");
            DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
            return rce;
         }
         DEBUG_YCALC   yLOG_value   ("owners"    , a_curr->npro);
         break;
      case G_DEP_CHECKROOT : 
      case G_DEP_UNROOT    : 
         DEBUG_YCALC   yLOG_note    ("independent cell, nothing to do");
         break;
      }
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return a_curr->npro;
   }
   /*---(count link types)---------------*/
   --rce;
   x_next = a_curr->pros;
   while (x_next != NULL) {
      DEBUG_YCALC   yLOG_info    ("owned by"  , ycalc_call_labeler (x_next->target));
      if (x_next->target != myCALC.rroot)  ++x_norms;
      else                          ++x_roots;
      x_next = x_next->next;
   }
   DEBUG_YCALC   yLOG_value   ("roots"     , x_roots);
   DEBUG_YCALC   yLOG_value   ("norms"     , x_norms);
   /*---(normal dependent cell)----------*/
   if (x_roots == 0) {
      switch (a_type) {
      case G_DEP_CHECKROOT : 
         DEBUG_YCALC   yLOG_note    ("normal dependent cell, no root");
         break;
      case G_DEP_ROOT      : 
         DEBUG_YCALC   yLOG_note    ("normal dependent cell, can not root");
         break;
      case G_DEP_UNROOT    : 
         DEBUG_YCALC   yLOG_note    ("normal dependent cell, no root, can not unroot");
         break;
      }
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return x_roots;
   }
   /*---(deal with duplicate roots)------*/
   --rce;
   x_next  = a_curr->pros;
   if (x_norms == 0)  x_roots = 0;
   else               x_roots = 1;
   while (x_next != NULL) {
      if (x_next->target == myCALC.rroot && x_roots > 0) {
         DEBUG_YCALC   yLOG_note    ("found reduntant root, cleaning");
         rc = ycalc_deps_delete (G_DEP_REQUIRE, &(myCALC.rroot), &a_curr, NULL);
      }
      ++x_roots;
      x_next = x_next->next;
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_value   ("owners"    , a_curr->npro);
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

#define     MAX_DEP    100

char         /*-> tbd --------------------------------[ ------ [ge.E45.437.71]*/ /*-[02.0000.027.!]-*/ /*-[--.---.---.--]-*/
ycalc__deps_circle (int a_level, tDEP_ROOT *a_source, tDEP_ROOT *a_target, long a_stamp)
{  /*---(design notes)-------------------*/
   /*
    *  burrows down into the a_source "requires" dependencies to see if the
    *  a_target cell is found.  if so, adding a dependency from a_target to
    *  a_source would create a circular reference.
    */
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   tDEP_LINK  *x_next      = NULL;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   DEBUG_YCALC   yLOG_value   ("a_level"   , a_level);
   /*---(defenses)-----------------------*/
   DEBUG_YCALC   yLOG_point   ("a_source"  , a_source);
   --rce;  if (a_source   == NULL)  {
      DEBUG_YCALC   yLOG_info    ("FAILED"    , "source pointer is null");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return  rce;
   }
   DEBUG_YCALC   yLOG_info    ("label"     , ycalc_call_labeler (a_source));
   DEBUG_YCALC   yLOG_point   ("a_target"  , a_target);
   --rce;  if (a_target   == NULL)  {
      DEBUG_YCALC   yLOG_info    ("FAILED"    , "target pointer is null");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return  rce;
   }
   --rce;  if (a_source == a_target) {
      DEBUG_YCALC   yLOG_info    ("FAILED"    , "match, circular link");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YCALC   yLOG_info    ("label"     , ycalc_call_labeler (a_target));
   --rce;  if (a_level  >= MAX_DEP) {
      DEBUG_YCALC   yLOG_info    ("FAILED"    , "past max depth");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return  rce;
   }
   /*---(check for already processed)----*/
   if (a_source->u == a_stamp) {
      DEBUG_YCALC   yLOG_info    ("SKIP"      , "already processed");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return  0;
   }
   /*---(stamp at end)-------------------*/
   a_source->u = a_stamp;
   /*---(process children)---------------*/
   DEBUG_YCALC   yLOG_value   ("nrequire"  , a_source->nreq);
   --rce;
   x_next = a_source->reqs;
   while (x_next != NULL) {
      /*---(recurse to child)------------*/
      DEBUG_YCALC   yLOG_info    ("label"     , ycalc_call_labeler (x_next->target));
      rc = ycalc__deps_circle (a_level + 1, x_next->target, a_target, a_stamp);
      if (rc < 0) {
         DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
         return rc;
      }
      /*---(prepare for next)------------*/
      x_next = x_next->next;
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                          mass actions                        ----===*/
/*====================------------------------------------====================*/
static void  o___MASS____________o () { return; }

char         /*-> remove all requires deps -----------[ ------ [ge.A34.142.41]*/ /*-[02.0000.515.5]-*/ /*-[--.---.---.--]-*/
ycalc_deps_wipe_reqs    (void **a_owner, tDEP_ROOT **a_deproot)
{
   /*---(locals)-----------+-----------+-*/
   tDEP_LINK  *x_next      = NULL;
   tDEP_LINK  *x_save      = NULL;
   tDEP_ROOT  *x_target    = NULL;
   void       *x_owner     = NULL;
   char        rce         =  -10;
   char        rc          = 0;
   char        t           [LEN_RECD] = "";
   /*---(filter)-------------------------*/
   if (a_deproot  == NULL)  return 0;
   if (*a_deproot == NULL)  return 0;
   /*---(begin)--------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   DEBUG_YCALC   yLOG_point   ("a_owner"   , a_owner);
   if (a_owner != NULL)  DEBUG_YCALC   yLOG_point   ("*a_owner"  , *a_owner);
   DEBUG_YCALC   yLOG_point   ("a_deproot" , a_deproot);
   DEBUG_YCALC   yLOG_point   ("*a_deproot", *a_deproot);
   /*---(reqs)---------------------------*/
   DEBUG_YCALC   yLOG_info    ("owned by"  , ycalc_call_labeler (*a_deproot));
   DEBUG_YCALC   yLOG_value   ("nreq"      , (*a_deproot)->nreq);
   /*---(BEG DEBUGGING)------------------*/
   yCALC_show_reqs  (*a_deproot, NULL, t);
   DEBUG_YCALC   yLOG_info    ("reqs"      , t);
   /*---(END DEBUGGING)------------------*/
   x_next = (*a_deproot)->reqs;
   while (x_next != NULL) {
      DEBUG_YCALC    yLOG_point   ("x_next"      , x_next);
      x_save   = x_next->next;
      x_target = x_next->target;
      x_owner  = x_next->target->owner;
      DEBUG_YCALC   yLOG_complex ("target"    , "type=%c, ptr=%9p, label=%s", x_next->type, x_target, ycalc_call_labeler (x_target));
      switch (x_next->type) {
      case G_DEP_POINTER :
         DEBUG_YCALC    yLOG_note    ("pointer type is special");
         rc = ycalc_range_delete (*a_deproot, x_target);
         if (rc < 0)  rc = ycalc_deps_delete  (x_next->type, a_deproot, &(x_target), &(x_owner));
         break;
      case G_DEP_CONTENT :
         DEBUG_YCALC    yLOG_note    ("content type should not delete");
         break;
      default :
         DEBUG_YCALC    yLOG_note    ("normal pointer, wipe");
         rc = ycalc_deps_delete  (x_next->type, a_deproot, &(x_target), &(x_owner));
         break;
      }
      DEBUG_YCALC   yLOG_value   ("nreq"      , (*a_deproot)->nreq);
      x_next = x_save;
   }
   /*---(unhook from ranges)-------------*/
   DEBUG_YCALC   yLOG_value   ("npro"      , (*a_deproot)->npro);
   ycalc_range_unhook (a_owner, a_deproot);
   /*---(BEG DEBUGGING)------------------*/
   /*> yCALC_show_pros  (*a_deproot, NULL, t);                                        <* 
    *> DEBUG_YCALC   yLOG_info    ("pros"      , t);                                  <*/
   /*---(END DEBUGGING)------------------*/
   /*---(check if rooted)----------------*/
   DEBUG_YCALC   yLOG_point   ("*a_deproot", *a_deproot);
   if (*a_deproot != NULL) {
      DEBUG_YCALC   yLOG_value   ("npro"      , (*a_deproot)->npro);
      DEBUG_YCALC   yLOG_point   ("pros"      , (*a_deproot)->pros);
      if ((*a_deproot)->npro == 1) {
         DEBUG_YCALC   yLOG_point   ("source"    , (*a_deproot)->pros->source);
         DEBUG_YCALC   yLOG_info    ("label"     , ycalc_call_labeler ((*a_deproot)->pros->source));
         DEBUG_YCALC   yLOG_point   ("target"    , (*a_deproot)->pros->target);
         DEBUG_YCALC   yLOG_info    ("label"     , ycalc_call_labeler ((*a_deproot)->pros->target));
         DEBUG_YCALC   yLOG_point   ("rroot"     , myCALC.rroot);
         if ((*a_deproot)->pros->target == myCALC.rroot) {
            DEBUG_YCALC   yLOG_note    ("unrooting");
            rc = ycalc_deps_delete (G_DEP_REQUIRE, &(myCALC.rroot), a_deproot, a_owner);
         }
      }
   }
   DEBUG_YCALC   yLOG_point   ("a_owner"   , a_owner);
   if (a_owner != NULL)  DEBUG_YCALC   yLOG_point   ("*a_owner"  , *a_owner);
   DEBUG_YCALC   yLOG_point   ("a_deproot" , a_deproot);
   DEBUG_YCALC   yLOG_point   ("*a_deproot", *a_deproot);
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       merging locations                      ----===*/
/*====================------------------------------------====================*/
static void  o___MERGING_________o () { return; }

char         /*-> find the merge source --------------[ leaf   [gp.420.113.30]*/ /*-[01.0000.306.#]-*/ /*-[--.---.---.--]-*/
yCALC_merge_source      (void *a_deproot, void **a_owner)
{  /*---(design notes)--------------------------------------------------------*/
   /* identify the source of the current cells merge                          */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tDEP_ROOT  *x_deproot   = NULL;
   tDEP_LINK  *x_next      = NULL;
   /*---(begin)--------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defenses)-----------------------*/
   DEBUG_YCALC   yLOG_point   ("a_deproot" , a_deproot);
   --rce;  if (a_deproot  == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_deproot = (tDEP_ROOT *) a_deproot;
   DEBUG_YCALC   yLOG_point   ("a_owner"   , a_owner);
   if (a_owner   != NULL)  *a_owner   = NULL;
   /*---(find head)----------------------*/
   x_next = x_deproot->pros;
   while (x_next != NULL) {
      DEBUG_YCALC   yLOG_point   ("x_next"    , x_next);
      DEBUG_YCALC   yLOG_char    ("type"      , x_next->type);
      if (G_DEP_BLEED == x_next->type) {
         DEBUG_YCALC   yLOG_note    ("FOUND");
         break;
      }
      x_next = x_next->next;
   }
   /*---(save)---------------------------*/
   --rce;  if (x_next == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (a_owner   != NULL)  *a_owner   = x_next->target->owner;
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       external information                   ----===*/
/*====================------------------------------------====================*/
static void  o___EXTERNAL________o () { return; }

int
yCALC_nreq              (void *a_deproot)
{
   tDEP_ROOT  *x_deproot   = NULL;
   if (a_deproot == NULL)   return 0;
   x_deproot = (tDEP_ROOT *) a_deproot;
   return x_deproot->nreq;
}

int
yCALC_npro              (void *a_deproot)
{
   tDEP_ROOT  *x_deproot   = NULL;
   if (a_deproot == NULL)   return 0;
   x_deproot = (tDEP_ROOT *) a_deproot;
   return x_deproot->npro;
}

int
yCALC_ncalc             (void *a_deproot)
{
   tDEP_ROOT  *x_deproot   = NULL;
   if (a_deproot == NULL)   return 0;
   x_deproot = (tDEP_ROOT *) a_deproot;
   return x_deproot->ncalc;
}

char
yCALC_stamp_cmp         (void *a_deproot, long a_stamp)
{
   tDEP_ROOT  *x_deproot   = NULL;
   if (a_deproot == NULL)        return 0;
   x_deproot = (tDEP_ROOT *) a_deproot;
   if (x_deproot->u == a_stamp)  return 1;
   return 0;
}

char
yCALC_stamp_set         (void *a_deproot, long a_stamp)
{
   tDEP_ROOT  *x_deproot   = NULL;
   if (a_deproot == NULL)        return 0;
   x_deproot = (tDEP_ROOT *) a_deproot;
   x_deproot->u = a_stamp;
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       helpful accessors                      ----===*/
/*====================------------------------------------====================*/
static void  o___ACCESSORS_______o () { return; }

char
yCALC_show_rpn          (void *a_deproot, int *a_nrpn, char *a_rpn)
{
   tDEP_ROOT  *x_deproot   = NULL;
   if (a_deproot == NULL)  return -1;
   x_deproot = (tDEP_ROOT *) a_deproot;
   if (x_deproot->rpn == NULL)  return -2;
   if (a_nrpn != NULL)  *a_nrpn = x_deproot->nrpn;
   if (a_rpn  != NULL)  ystrlcpy (a_rpn, x_deproot->rpn, LEN_RECD);
   return 0;
}

char
yCALC_show_reqs         (void *a_deproot, int *a_nreq, char *a_reqs)
{
   tDEP_ROOT  *x_deproot   = NULL;
   if (a_deproot == NULL)  return -1;
   x_deproot = (tDEP_ROOT *) a_deproot;
   if (a_nreq != NULL)  *a_nreq = x_deproot->nreq;
   if (a_reqs != NULL)  yCALC_disp_reqs (x_deproot, a_reqs);
   return 0;
}

char
yCALC_show_pros         (void *a_deproot, int *a_npro, char *a_pros)
{
   tDEP_ROOT  *x_deproot   = NULL;
   char        x_list      [LEN_RECD ];
   if (a_deproot == NULL)  return -1;
   x_deproot = (tDEP_ROOT *) a_deproot;
   if (a_npro != NULL)  *a_npro = x_deproot->npro;
   if (a_pros != NULL)  yCALC_disp_pros (x_deproot, a_pros);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                          reporting                           ----===*/
/*====================------------------------------------====================*/
static void  o___REPORTS_________o () { return; }

char 
ycalc_deps_dump         (void *f, char a_scope)
{
   /*---(locals)-----------+-----+-----+-*/
   char        t           [LEN_RECD]  = "";
   tDEP_LINK  *x_cur       = NULL;
   FILE       *x_file      = NULL;
   int         a           =    0;
   int         c           =    0;
   int         u           =    0;
   int         s           =    0;
   void       *x_owner     = NULL;
   char      **x_source    = NULL;
   char       *x_type      = NULL;
   char        x_save      =  '�';
   char        x_pre       [LEN_TERSE] = "";
   char        x_suf       [LEN_TERSE] = "";
   char        x_label     [LEN_LABEL] = "";
   char        x_sys       =  '-';
   /*---(prepare)------------------------*/
   x_file = f;
   /*---(print)--------------------------*/
   fprintf (f, "#! parsing ��--��Ϸ��-------------���---------��Ϸ��------------------------���-------------���---------��Ϸ��------------------------���\n");
   fprintf (f, "#! titles  �seq��t��source����������owner�������t��content��������������������target����������owner�������t��content���������������������\n");
   fprintf (f, "\n");
   fprintf (f, "# count = %d (user and system dependencies)\n" , myCALC.dcount);
   fprintf (f, "# requested scope '%c' = %s\n" , a_scope, (a_scope == '-') ? "user deps only" : "all deps");
   x_cur = myCALC.dhead;
   while (x_cur != NULL) {
      /*---(filter)----------------------*/
      ++a;
      x_sys = '-';
      strcpy (x_label, "");
      if (x_cur->source != NULL)  strcpy (x_label, ycalc_call_labeler (x_cur->source));
      if (x_label [0] == '�')  x_sys = 'y';
      strcpy (x_label, "");
      if (x_cur->target != NULL)  strcpy (x_label, ycalc_call_labeler (x_cur->target));
      if (x_label [0] == '�')  x_sys = 'y';
      if (x_sys == 'y') {
         ++s;
         if (a_scope != 'A') {
            x_cur = x_cur->dnext;  
            continue;
         }
      } else {
         ++u;
      }
      /*---(headers)---------------------*/
      if (c % 26 == 0)  fprintf (f, "\n#--  t  source--------  owner-----  t  content------------------  target--------  owner-----  t  content------------------  �\n");
      /*---(prepare)---------------------*/
      if (strchr (S_DEP_REQS, x_cur->type) != NULL) {
         strcpy (x_pre , "");
         strcpy (x_suf , "  ");
         fprintf (f, "\n");
      } else {
         strcpy (x_pre , "  ");
         strcpy (x_suf , "");
      }
      /*---(prefix)----------------------*/
      fprintf (f, "%3d  %c", c++, x_cur->type);
      /*---(source)----------------------*/
      if (x_cur->source == NULL) {
         fprintf (f, "  %-10.10p  %-10.10s  %c  %-25.25s", NULL, "����", '�', "����");
      } else {
         x_owner = x_cur->source->owner;
         if (x_owner == NULL) {
            fprintf (f, "  %-8.8p  %-10.10s  %c  %-25.25s", x_cur->source, "����", '�', "����");
         } else {
            myCALC.e_pointer (x_owner, &x_source, &x_type, NULL, NULL);
            fprintf (f, "  %-8.8p  %s%-8.8s%s  %c  %-25.25s", x_cur->source, x_pre, ycalc_call_labeler (x_cur->source), x_suf, *x_type, *x_source);
         }
      }
      /*---(target)----------------------*/
      if (x_cur->target == NULL) {
         fprintf (f, "  %-8.8p  %-10.10s  %c  %-25.25s", NULL, "����", '�', "����");
      } else {
         x_owner = x_cur->target->owner;
         if (x_owner == NULL) {
            fprintf (f, "  %-8.8p  %-10.10s  %c  %-25.25s", x_cur->target, "����", '�', "����");
         } else {
            myCALC.e_pointer (x_owner, &x_source, &x_type, NULL, NULL);
            fprintf (f, "  %-8.8p  %s%-8.8s%s  %c  %-25.25s", x_cur->target, x_pre, ycalc_call_labeler (x_cur->target), x_suf, *x_type, *x_source);
         }
      }
      /*---(suffix)----------------------*/
      fprintf (f, "  �\n");
      /*---(next)------------------------*/
      x_cur = x_cur->dnext;  
      /*---(done)------------------------*/
   }
   /*---(footer)-------------------------*/
   fprintf (f, "\n\n# shown = %d (%d all > %d user, %d sys)\n\n" , c, a, u, s);
   /*---(complete)-----------------------*/
   return 0;
}

char  yCALC_deps_dump_user    (void *f) { return ycalc_deps_dump (f, '-'); }
char  yCALC_deps_dump_all     (void *f) { return ycalc_deps_dump (f, 'A'); }


/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNIT_TEST_______o () { return; }

char*        /*-> unit testing accessor --------------[ ------ [gs.HA0.1B3.K5]*/ /*-[02.0000.00#.#]-*/ /*-[--.---.---.--]-*/
ycalc__unit_deps        (char *a_question, char *a_label)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_fore      = 0;
   int         x_back      = 0;
   void       *x_owner     = NULL;
   tDEP_ROOT  *x_deproot   = NULL;
   tDEP_LINK  *x_dep       = NULL;
   char        x_list      [LEN_RECD ];
   char        x_label     [LEN_LABEL];
   char        rc          =    0;
   int         c           =    0;
   /*---(preprare)-----------------------*/
   strcpy (ycalc__unit_answer, "yCALC            : question not understood");
   if (a_label == NULL)   ystrlcpy (x_label, "---"  , LEN_LABEL);
   else                   ystrlcpy (x_label, a_label, LEN_LABEL);
   rc = ycalc_call_who_named (x_label, YCALC_LOOK, &x_owner, &x_deproot);
   /*---(dependency list)----------------*/
   if      (strcmp (a_question, "count"    )      == 0) {
      x_dep  = myCALC.dhead; while (x_dep  != NULL) { ++x_fore; x_dep  = x_dep ->dnext; }
      x_dep  = myCALC.dtail; while (x_dep  != NULL) { ++x_back; x_dep  = x_dep ->dprev; }
      snprintf (ycalc__unit_answer, LEN_RECD, "yCALC deps count : num=%4d, fore=%4d, back=%4d", myCALC.dcount, x_fore, x_back);
   }
   else if (strcmp (a_question, "linked_list" )   == 0) {
      snprintf (ycalc__unit_answer, LEN_RECD, "yCALC deps list  : num=%4d, head=%9p, tail=%9p", myCALC.dcount, myCALC.dhead, myCALC.dtail);
   }
   else if (strcmp (a_question, "requires"    )   == 0) {
      if (x_deproot == NULL) {
         snprintf (ycalc__unit_answer, LEN_RECD, "yCALC deps reqs  : %-5s (--) -", x_label);
      } else {
         yCALC_disp_reqs (x_deproot, x_list);
         DEBUG_YCALC    yLOG_info    ("x_list"    , x_list);
         snprintf (ycalc__unit_answer, LEN_RECD, "yCALC deps reqs  : %-5s (%2d) %s", x_label, x_deproot->nreq, x_list);
         DEBUG_YCALC    yLOG_info    ("answer"    , ycalc__unit_answer);
      }
   }
   else if (strcmp (a_question, "provides"    )   == 0) {
      if (x_deproot == NULL) {
         snprintf (ycalc__unit_answer, LEN_RECD, "yCALC deps pros  : %-5s (--) -", x_label);
      } else {
         yCALC_disp_pros (x_deproot, x_list);
         snprintf (ycalc__unit_answer, LEN_RECD, "yCALC deps pros  : %-5s (%2d) %s", x_label, x_deproot->npro, x_list);
      }
   }
   else if (strcmp (a_question, "overview"    )   == 0) {
      if (x_deproot == NULL) {
         snprintf (ycalc__unit_answer, LEN_RECD, "yCALC deps over  : %-5s nreq --, npro -- (range --, real --)", x_label);
      } else {
         c = ycalc_range_nonrange (x_deproot);
         snprintf (ycalc__unit_answer, LEN_RECD, "yCALC deps over  : %-5s nreq %2d, npro %2d (range %2d, real %2d)", x_label, x_deproot->nreq, x_deproot->npro, x_deproot->npro - c, c);
      }
   }
   /*---(complete)-----------------------*/
   return ycalc__unit_answer;
}



