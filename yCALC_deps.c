/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"



tDEP_INFO   g_dep_info [MAX_DEPTYPE] = {
   /*----type------  ---match----- ---dir--------  ---description--------------------------------------- index count total */

   {  G_DEP_REQUIRE, G_DEP_PROVIDE, G_DEP_DIRREQ , "requires another cell for its value"                ,  0  ,  0  ,  0   },
   {  G_DEP_PROVIDE, G_DEP_REQUIRE, G_DEP_DIRPRO , "provides its value to another cell"                 ,  0  ,  0  ,  0   },

   {  G_DEP_RANGE  , G_DEP_CELL   , G_DEP_DIRREQ , "range pointer that provides dependency shortcut"    ,  0  ,  0  ,  0   },
   {  G_DEP_CELL   , G_DEP_RANGE  , G_DEP_DIRPRO , "individual cell that makes up a range pointer"      ,  0  ,  0  ,  0   },

   {  G_DEP_FORMAT , G_DEP_COPY   , G_DEP_DIRREQ , "format master cell providing format template"       ,  0  ,  0  ,  0   },
   {  G_DEP_COPY   , G_DEP_FORMAT , G_DEP_DIRPRO , "individual cell following the a format template"    ,  0  ,  0  ,  0   },

   {  G_DEP_SOURCE , G_DEP_LIKE   , G_DEP_DIRREQ , "source formula master other cell follow"            ,  0  ,  0  ,  0   },
   {  G_DEP_LIKE   , G_DEP_SOURCE , G_DEP_DIRPRO , "follows a source formula with ref adjustments"      ,  0  ,  0  ,  0   },

   {  G_DEP_MERGED , G_DEP_BLEED  , G_DEP_DIRREQ , "provides contents for set of merged cells"          ,  0  ,  0  ,  0   },
   {  G_DEP_BLEED  , G_DEP_MERGED , G_DEP_DIRPRO , "provides bleed-over space to display contents"      ,  0  ,  0  ,  0   },

   {  G_DEP_CALCREF, G_DEP_ADDRESS, G_DEP_DIRREQ , "contains a calculated/runtime reference function"   ,  0  ,  0  ,  0   },
   {  G_DEP_ADDRESS, G_DEP_CALCREF, G_DEP_DIRPRO , "provides its value to a calculated reference"       ,  0  ,  0  ,  0   },

   {  G_DEP_WATCHER, G_DEP_STALKED, G_DEP_DIRPRO , "watcher, watches another cell for properties"       ,  0  ,  0  ,  0   },
   {  G_DEP_STALKED, G_DEP_WATCHER, G_DEP_DIRREQ , "observed, provides information to another cell"     ,  0  ,  0  ,  0   },

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
   DEBUG_DEPS   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_DEPS   yLOG_spoint  ("a_dep");
   --rce;  if (a_dep == NULL) {
      DEBUG_DEPS   yLOG_snote   ("no return dep pointer");
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_spoint  ("*a_dep");
   --rce;  if (*a_dep != NULL) {
      DEBUG_DEPS   yLOG_snote   ("dep pointer already assigned");
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(create)-------------------------*/
   DEBUG_DEPS   yLOG_snote   ("malloc");
   while (x_dep == NULL && x_tries < 10) {
      x_dep = (tDEP_LINK *) malloc (sizeof (tDEP_LINK));
      ++x_tries;
   }
   DEBUG_DEPS   yLOG_svalue  ("tries", x_tries);
   --rce;  if (x_dep == NULL) {
      DEBUG_DEPS   yLOG_snote   ("FAILED");
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(dep fields)---------------------*/
   DEBUG_DEPS   yLOG_snote   ("dep pointers");
   x_dep->type    = G_DEP_BLANK;
   x_dep->prev    = NULL;
   x_dep->next    = NULL;
   x_dep->match   = NULL;
   /*---(dependency doubly-linked list)--*/
   DEBUG_DEPS   yLOG_snote   ("main list");
   x_dep->dprev   = NULL;
   x_dep->dnext   = NULL;
   if (myCALC.dtail == NULL) {
      myCALC.dhead         = x_dep;
      myCALC.dtail         = x_dep;
   } else {
      x_dep->dprev   = myCALC.dtail;
      x_dep->dnext   = NULL;
      myCALC.dtail->dnext  = x_dep;
      myCALC.dtail         = x_dep;
   }
   ++myCALC.dcount;
   /*---(cell pointers)------------------*/
   DEBUG_DEPS   yLOG_snote   ("cell pointers");
   x_dep->source  = NULL;
   x_dep->target  = NULL;
   /*---(statistics)---------------------*/
   DEBUG_DEPS   yLOG_snote   ("stats");
   x_dep->count   = 0;
   /*---(save)---------------------------*/
   DEBUG_DEPS   yLOG_snote   ("save");
   *a_dep = x_dep;
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char         /*-> destroy a single dependency --------[ leaf   [fp.B42.102.E0]*/ /*-[11.0000.034.5]-*/ /*-[--.---.---.--]-*/
ycalc__deps_free        (tDEP_LINK **a_dep)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   tDEP_LINK  *x_dep       = NULL;
   /*---(header)-------------------------*/
   DEBUG_DEPS   yLOG_senter  (__FUNCTION__);
   /*---(defense: null cell)-------------*/
   DEBUG_DEPS   yLOG_spoint  (a_dep);
   --rce;  if (a_dep       == NULL) {
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_spoint  (*a_dep);
   --rce;  if (*a_dep      == NULL) {
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_dep = *a_dep;
   /*---(remove from dependency list)----*/
   DEBUG_DEPS   yLOG_snote   ("main list");
   if (x_dep->dnext != NULL) x_dep->dnext->dprev     = x_dep->dprev;
   else                      myCALC.dtail            = x_dep->dprev;
   if (x_dep->dprev != NULL) x_dep->dprev->dnext     = x_dep->dnext;
   else                      myCALC.dhead            = x_dep->dnext;
   --myCALC.dcount;
   /*---(detatch from paired dep)--------*/
   DEBUG_DEPS   yLOG_snote   ("detach pair");
   if (x_dep->match != NULL) {
      x_dep->match->match  = NULL;
      x_dep->match         = NULL;
   }
   /*---(if require, take off cell)------*/
   if      (strchr (S_DEP_REQS, x_dep->type) != NULL) {
      DEBUG_DEPS   yLOG_snote   ("requires");
      if (x_dep->next  != NULL) x_dep->next->prev    = x_dep->prev;
      if (x_dep->prev  != NULL) x_dep->prev->next    = x_dep->next;
      else                      x_dep->source->reqs  = x_dep->next;
      --(x_dep->source->nreq);
   }
   /*---(if provide, take off cell)------*/
   else if (strchr (S_DEP_PROS, x_dep->type) != NULL) {
      DEBUG_DEPS   yLOG_snote   ("provides");
      if (x_dep->next  != NULL) x_dep->next->prev    = x_dep->prev;
      if (x_dep->prev  != NULL) x_dep->prev->next    = x_dep->next;
      else                      x_dep->source->pros  = x_dep->next;
      --(x_dep->source->npro);
   }
   /*---(free)---------------------------*/
   DEBUG_DEPS   yLOG_snote   ("free");
   free (x_dep);
   *a_dep = NULL;
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
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
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   myCALC.rroot  = NULL;
   myCALC.rhead  = NULL;
   myCALC.rtail  = NULL;
   myCALC.rcount = 0;
   myCALC.dhead  = NULL;
   myCALC.dtail  = NULL;
   myCALC.dcount = 0;
   strlcpy (S_DEP_REQS, "" , 10);
   strlcpy (S_DEP_PROS, "" , 10);
   strlcpy (S_DEP_LIKE, "l", 10);
   /*---(complete info table)------------*/
   --rce;
   for (i = 0; i < MAX_DEPTYPE; ++i) {
      DEBUG_PROG   yLOG_char    ("type"      , g_dep_info [i].type);
      /*---(check for end)---------------*/
      if (g_dep_info [i].type == G_DEP_BLANK)  break;
      /*---(add to lists)----------------*/
      sprintf (t, "%c", g_dep_info [i].type);
      DEBUG_PROG   yLOG_info    ("str type"  , t);
      DEBUG_PROG   yLOG_char    ("dir"       , g_dep_info [i].dir);
      if      (g_dep_info [i].dir  == G_DEP_DIRREQ)  strcat (S_DEP_REQS, t);
      else if (g_dep_info [i].dir  == G_DEP_DIRPRO)  strcat (S_DEP_PROS, t);
      else {
         DEBUG_PROG   yLOG_note    ("type direction not + or -");
         DEBUG_PROG   yLOG_exit    (__FUNCTION__);
         return rce;
      }
      /*---(find match)------------------*/
      ++x_count;
      for (j = 0; j < MAX_DEPTYPE; ++j) {
         if (g_dep_info [j].type != g_dep_info [i].match) continue;
         ++x_found;
         g_dep_info [i].match_index = j;
         DEBUG_PROG   yLOG_char    ("match"     , g_dep_info [j].type);
         DEBUG_PROG   yLOG_value   ("match_indx", g_dep_info [i].match_index);
      }
   }
   DEBUG_PROG   yLOG_value   ("x_count"   , x_count);
   DEBUG_PROG   yLOG_value   ("x_found"   , x_found);
   --rce;  if (x_found != x_count) {
      DEBUG_PROG   yLOG_note    ("could not match all dep types");
      DEBUG_PROG   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(report out)---------------------*/
   DEBUG_PROG   yLOG_info    ("S_DEP_REQS", S_DEP_REQS);
   DEBUG_PROG   yLOG_info    ("S_DEP_PROS", S_DEP_PROS);
   DEBUG_PROG   yLOG_info    ("S_DEP_LIKE", S_DEP_LIKE);
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> destroy a single dependency --------[ leaf   [fp.B42.102.E0]*/ /*-[11.0000.034.5]-*/ /*-[--.---.---.--]-*/
ycalc__deproot_free     (tDEP_LINK **a_deproot)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   tDEP_ROOT  *x_deproot   = NULL;
   /*---(header)-------------------------*/
   DEBUG_DEPS   yLOG_senter  (__FUNCTION__);
   /*---(defense: null cell)-------------*/
   DEBUG_DEPS   yLOG_spoint  (a_deproot);
   --rce;  if (a_deproot  == NULL) {
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_spoint  (*a_deproot);
   --rce;  if (*a_deproot == NULL) {
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_deproot = *a_deproot;
   /*---(remove from dependency list)----*/
   DEBUG_DEPS   yLOG_snote   ("main list");
   if (x_deproot->rnext != NULL) x_deproot->rnext->rprev = x_deproot->rprev;
   else                          myCALC.rtail            = x_deproot->rprev;
   if (x_deproot->rprev != NULL) x_deproot->rprev->rnext = x_deproot->rnext;
   else                          myCALC.rhead            = x_deproot->rnext;
   --myCALC.rcount;
   /*---(free)---------------------------*/
   DEBUG_DEPS   yLOG_snote   ("free");
   free (x_deproot);
   *a_deproot = NULL;
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
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
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(walk through links)-------------*/
   DEBUG_PROG   yLOG_point   ("link_head" , myCALC.dhead);
   l_curr = myCALC.dhead;
   c      = 0;
   while (l_curr != NULL) {
      l_next = l_curr->dnext;
      ycalc__deps_free (&l_curr);
      l_curr = l_next;
      ++c;
   }
   DEBUG_PROG   yLOG_value   ("purged"    , c);
   /*---(reset globals)------------------*/
   myCALC.dhead   = NULL;
   myCALC.dtail   = NULL;
   myCALC.dcount  = 0;
   /*---(walk through roots)-------------*/
   DEBUG_PROG   yLOG_point   ("root_tail" , myCALC.rtail);
   r_curr = myCALC.rtail;
   c      = 0;
   while (r_curr != NULL) {
      r_next = r_curr->rprev;
      /*> yCALC_build (r_curr, "", NULL);                                             <*/
      ycalc__deproot_free (&r_curr);
      r_curr = r_next;
      ++c;
   }
   DEBUG_PROG   yLOG_value   ("purged"    , c);
   /*---(reset globals)------------------*/
   myCALC.rroot   = NULL;
   myCALC.rhead   = NULL;
   myCALC.rtail   = NULL;
   myCALC.rcount  = 0;
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> shut down dependency usage ---------[ shoot  [gz.421.001.02]*/ /*-[00.0000.102.1]-*/ /*-[--.---.---.--]-*/
ycalc_deps_wrap         (void)
{
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(dependencies)-------------------*/
   ycalc__deps_purge ();
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
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
/*===----                      create a dependency                     ----===*/
/*====================------------------------------------====================*/
static void  o___CREATE__________o () { return; }

tDEP_LINK*   /*-> create a requires dependency -------[ ------ [fp.C43.123.61]*/ /*-[02.0000.017.!]-*/ /*-[--.---.---.--]-*/
ycalc__deps_create_req  (char a_type, char a_index, tDEP_ROOT *a_source, tDEP_ROOT *a_target)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   tDEP_LINK  *x_next      = NULL;          /* iterator -- dependency lists   */
   tDEP_LINK  *x_req       = NULL;          /* new requires entry             */
   char        x_label     [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   /*---(check for existing requires)----*/
   DEBUG_DEPS   strlcpy (x_label, g_labeler (a_source->owner), LEN_LABEL);
   DEBUG_DEPS   yLOG_info    ("src label" , x_label);
   DEBUG_DEPS   yLOG_value   ("src nreqs" , a_source->nreq);
   x_next = a_source->reqs;
   while (x_next != NULL) {
      DEBUG_DEPS   strlcpy (x_label, g_labeler (x_next->source->owner), LEN_LABEL);
      DEBUG_DEPS   yLOG_info    ("trg label" , x_label);
      if (x_next->target == a_target) {
         DEBUG_DEPS   yLOG_note    ("found existing, done");
         DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
         return x_next;
      }
      x_next = x_next->next;
   }
   /*---(create a new requires)-------*/
   DEBUG_DEPS   yLOG_note    ("existing not found, create new reqs");
   rc = ycalc__deps_new (&x_req);
   if (rc < 0 || x_req == NULL)   {
      DEBUG_DEPS yLOG_note      ("could not make a new reqs entry");
      DEBUG_DEPS yLOG_exit      (__FUNCTION__);
      return NULL; 
   }
   /*---(assign basics)---------------*/
   DEBUG_DEPS   yLOG_note    ("assign basic values");
   x_req->type     = a_type;
   x_req->source   = a_source;
   x_req->target   = a_target;
   /*---(add to dep counters)---------*/
   DEBUG_DEPS   yLOG_note    ("increment dependency table counters");
   ++(g_dep_info [a_index].count);
   ++(g_dep_info [a_index].total);
   /*---(hook it up to cell)----------*/
   DEBUG_DEPS   yLOG_note    ("hook up dependency into root linked list");
   if (a_source->reqs == NULL) {
      a_source->reqs = x_req;
   } else {
      x_next = a_source->reqs;
      while (x_next->next != NULL)  x_next = x_next->next;
      x_next->next         = x_req;
      x_req->prev          = x_next;
   }
   ++a_source->nreq;
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
   return x_req;
}

tDEP_LINK*   /*-> create a provides dependency -------[ ------ [fp.C43.123.62]*/ /*-[02.0000.017.!]-*/ /*-[--.---.---.--]-*/
ycalc__deps_create_pro  (char a_type, char a_index, tDEP_ROOT *a_source, tDEP_ROOT *a_target)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          =    0;
   tDEP_LINK  *x_next      = NULL;          /* iterator -- dependency lists   */
   tDEP_LINK  *x_pro       = NULL;          /* new requires entry             */
   char        x_label     [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   /*---(check for existing requires)----*/
   DEBUG_DEPS   strlcpy (x_label, g_labeler (a_source->owner), LEN_LABEL);
   DEBUG_DEPS   yLOG_info    ("src label" , x_label);
   DEBUG_DEPS   yLOG_value   ("src npros" , a_source->npro);
   x_next = a_source->pros;
   while (x_next != NULL) {
      DEBUG_DEPS   strlcpy (x_label, g_labeler (x_next->source->owner), LEN_LABEL);
      DEBUG_DEPS   yLOG_info    ("trg label" , x_label);
      if (x_next->target == a_target) {
         DEBUG_DEPS   yLOG_note    ("found existing, done");
         DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
         return x_next;
      }
      x_next = x_next->next;
   }
   /*---(create a new provides)-------*/
   DEBUG_DEPS   yLOG_note    ("existing not found, create new requires");
   rc = ycalc__deps_new (&x_pro);
   if (rc < 0 || x_pro == NULL)   {
      DEBUG_DEPS yLOG_note      ("could not make a new pros entry");
      DEBUG_DEPS yLOG_exit      (__FUNCTION__);
      return NULL; 
   }
   /*---(assign basics)---------------*/
   DEBUG_DEPS   yLOG_note    ("assign basic values");
   x_pro->type     = ycalc__deps_abbr (a_index);
   x_pro->source   = a_source;
   x_pro->target   = a_target;
   /*---(add to dep counters)---------*/
   DEBUG_DEPS   yLOG_note    ("increment dependency table counters");
   ++(g_dep_info [a_index].count);
   ++(g_dep_info [a_index].total);
   /*---(hook it up to cell)----------*/
   DEBUG_DEPS   yLOG_note    ("hook up dependency into cell linked list");
   if (a_source->pros == NULL) {
      a_source->pros = x_pro;
   } else {
      x_next = a_source->pros;
      while (x_next->next != NULL)  x_next = x_next->next;
      x_next->next         = x_pro;
      x_pro->prev          = x_next;
   }
   ++a_source->npro;
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
   return x_pro;
}

char         /*-> create a two-way dependency --------[ ------ [ge.M88.15#.B6]*/ /*-[01.0000.526.H]-*/ /*-[--.---.---.--]-*/
yCALC_create       (char a_type, void *a_source, void *a_target)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;          /* return code for errors         */
   char        rc          =    0;          /* generic return code            */
   char        x_index     =    0;          /* dependency type table index    */
   tDEP_LINK  *x_require   = NULL;    /* new requires entry                   */
   tDEP_LINK  *x_provide   = NULL;    /* new provides entry                   */
   char        x_label     [LEN_LABEL];
   tDEP_ROOT  *x_source    = (tDEP_ROOT *) a_source;
   tDEP_ROOT  *x_target    = (tDEP_ROOT *) a_target;
   /*---(header)-------------------------*/
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   /*---(defense: null pointers)---------*/
   DEBUG_DEPS   yLOG_point   ("x_source"  , x_source);
   --rce;  if (x_source     == NULL)   {
      DEBUG_DEPS yLOG_note      ("source is null");
      DEBUG_DEPS yLOG_exit      (__FUNCTION__);
      return rce;
   }
   DEBUG_DEPS   strlcpy (x_label, g_labeler (x_source->owner), LEN_LABEL);
   DEBUG_DEPS   yLOG_info    ("src label" , x_label);
   DEBUG_DEPS   yLOG_point   ("x_target"  , x_target);
   --rce;  if (x_target     == NULL)   {
      DEBUG_DEPS yLOG_note      ("target is null");
      DEBUG_DEPS yLOG_exit      (__FUNCTION__);
      return rce;
   }
   DEBUG_DEPS   strlcpy (x_label, g_labeler (x_target->owner), LEN_LABEL);
   DEBUG_DEPS   yLOG_info    ("trg label" , x_label);
   /*---(defense: linking back to root)--*/
   --rce;  if (x_target     == myCALC.rroot)  {
      DEBUG_DEPS yLOG_note      ("can not route back to root");
      DEBUG_DEPS yLOG_exit      (__FUNCTION__);
      return rce;
   }
   /*---(defense: circular ref)----------*/
   --rce;  if (x_source     == x_target) {
      DEBUG_DEPS   yLOG_note    ("source and target are same");
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(defense: type)------------------*/
   DEBUG_DEPS   yLOG_char    ("a_type"    , a_type);
   x_index     = ycalc__deps_index (a_type);
   DEBUG_DEPS   yLOG_value   ("dep index" , x_index);
   --rce;  if (x_index      <  0) {
      DEBUG_DEPS   yLOG_note    ("dependency type not found");
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(check for circlular ref)--------*/
   DEBUG_DEPS   yLOG_note    ("check for potential circular reference");
   rc     = ycalc__deps_circle (0, x_target, x_source, rand());
   /*> rc     = DEP_circle (0, x_target, x_source, rand());                           <*/
   DEBUG_DEPS   yLOG_value   ("circle rc" , rc);
   --rce;  if (rc  <  0) {
      DEBUG_DEPS   yLOG_note    ("request would cause a circular reference");
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(check if target needs unroot)---*/
   DEBUG_DEPS   yLOG_note    ("check target for unrooting");
   rc = ycalc__deps_rooting (x_target, G_DEP_UNROOT);
   /*> rc = DEP_rooting (x_target, G_DEP_UNROOT);                                     <*/
   if (rc <  0) {
      DEBUG_DEPS   yLOG_note    ("target could not be properly unrooted");
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(create require)-----------------*/
   x_require   = ycalc__deps_create_req (a_type, x_index, x_source, x_target);
   --rce;  if (x_require    == NULL    ) {
      DEBUG_DEPS   yLOG_note    ("requires did not get made");
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(create provide)-----------------*/
   x_index     = ycalc__deps_match (a_type);
   x_provide   = ycalc__deps_create_pro (a_type, x_index, x_target, x_source);
   --rce;  if (x_require    == NULL    ) {
      DEBUG_DEPS   yLOG_note    ("provides did not get made");
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(cross link them)----------------*/
   DEBUG_DEPS   yLOG_note    ("link require and provide together");
   x_require->match     = x_provide;
   x_provide->match     = x_require;
   /*---(check if source needs rooting)--*/
   DEBUG_DEPS   yLOG_note    ("check source for rooting");
   rc = ycalc__deps_rooting (x_source, G_DEP_ROOT);
   /*> rc = DEP_rooting (x_source, G_DEP_ROOT);                                       <*/
   --rce;  if (rc <  0) {
      DEBUG_DEPS   yLOG_note    ("source could not be properly rooted");
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(check for dup linking)----------*/
   /*> rc = DEP_check (0, x_source, 'n', rand());                                     <*/
   /*> rc = DEP_checkall ('n');                                                       <* 
    *> if (rc <  0) {                                                                 <* 
    *>    DEBUG_DEPS   yLOG_note    ("failed dependency check");                      <* 
    *>    DEBUG_DEPS   yLOG_exit    (__FUNCTION__);                                   <* 
    *>    return rce;                                                                 <* 
    *> }                                                                              <*/
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_note    ("all actions complete");
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        deleting links                        ----===*/
/*====================------------------------------------====================*/
static void  o___DELETE__________o () { return; }

char         /*-> remove a requires dependency -------[ ------ [fe.933.143.31]*/ /*-[03.0000.016.!]-*/ /*-[--.---.---.--]-*/
ycalc__deps_delete_req  (char a_type, char a_index, tDEP_ROOT *a_source, tDEP_ROOT *a_target)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   char        rc          = 0;
   tDEP_LINK  *x_next      = NULL;          /* iterator -- dependency lists   */
   tDEP_LINK  *x_req       = NULL;          /* new requires entry             */
   char        x_label     [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   /*---(check for existing requires)----*/
   DEBUG_DEPS   strlcpy (x_label, g_labeler (a_source->owner), LEN_LABEL);
   DEBUG_DEPS   yLOG_info    ("src label" , x_label);
   DEBUG_DEPS   yLOG_value   ("src nreqs" , a_source->nreq);
   x_next = a_source->reqs;
   --rce; while (x_next != NULL) {
      DEBUG_DEPS   strlcpy (x_label, g_labeler (x_next->target->owner), LEN_LABEL);
      DEBUG_DEPS   yLOG_info    ("trg label" , x_label);
      if (x_next->target == a_target) {
         DEBUG_DEPS   yLOG_note    ("found existing");
         rc = ycalc__deps_free (&x_next);
         if (rc != 0)  {
            DEBUG_DEPS   yLOG_note    ("deletion failed");
            DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
            return rce;
         }
         DEBUG_DEPS   yLOG_note    ("update dependency table totals");
         --(g_dep_info [a_index].count);
         DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
         return 0;
      }
      x_next = x_next->next;
   }
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_note    ("did not find dependency");
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
   --rce;
   return rce;
}

char         /*-> remove a provides dependency -------[ ------ [fe.933.143.31]*/ /*-[03.0000.016.!]-*/ /*-[--.---.---.--]-*/
ycalc__deps_delete_pro  (char a_type, char a_index, tDEP_ROOT *a_source, tDEP_ROOT *a_target)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   char        rc          = 0;
   tDEP_LINK  *x_next      = NULL;          /* iterator -- dependency lists   */
   tDEP_LINK  *x_req       = NULL;          /* new requires entry             */
   char        x_label     [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   /*---(check for existing requires)----*/
   DEBUG_DEPS   strlcpy (x_label, g_labeler (a_source->owner), LEN_LABEL);
   DEBUG_DEPS   yLOG_info    ("src label" , x_label);
   DEBUG_DEPS   yLOG_value   ("nprovide"  , a_source->npro);
   x_next = a_source->pros;
   --rce; while (x_next != NULL) {
      DEBUG_DEPS   strlcpy (x_label, g_labeler (x_next->target->owner), LEN_LABEL);
      DEBUG_DEPS   yLOG_info    ("trg label" , x_label);
      if (x_next->target == a_target) {
         DEBUG_DEPS   yLOG_note    ("found existing");
         rc = ycalc__deps_free (&x_next);
         if (rc != 0)  {
            DEBUG_DEPS   yLOG_note    ("deletion failed");
            DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
            return rce;
         }
         DEBUG_DEPS   yLOG_note    ("update dependency table totals");
         --(g_dep_info [a_index].count);
         DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
         return 0;
      }
      x_next = x_next->next;
   }
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_note    ("did not find dependency");
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
   --rce;
   return rce;
}

char         /*-> remove a two-way dependency --------[ ------ [ge.P79.15#.I6]*/ /*-[02.0000.345.3]-*/ /*-[--.---.---.--]-*/
yCALC_delete            (char a_type, void *a_source, void *a_target)
{  /*---(design notes)--------------------------------------------------------*/
   /* removes a two-way dependency so that the dependency tree remains        */
   /* accurate and unneeded cells are removed.  it is important that these    */
   /* entries remain in a well-formed dependency graph so all necessary cells */
   /* are calculated at the right times.                                      */
   /*---(header)-------------------------*//*---------------------------------*/
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;          /* return code for errors         */
   char        rc          =    0;          /* generic return code            */
   char        x_index     =    0;          /* dependency type table index    */
   tDEP_LINK  *x_require   = NULL;    /* new requires entry                   */
   tDEP_LINK  *x_provide   = NULL;    /* new provides entry                   */
   char        x_label     [LEN_LABEL];
   char        x_type      =  ' ';
   tDEP_ROOT  *x_source    = (tDEP_ROOT *) a_source;
   tDEP_ROOT  *x_target    = (tDEP_ROOT *) a_target;
   /*---(defense: null pointers)---------*/
   DEBUG_DEPS   yLOG_point   ("x_source"  , x_source);
   --rce;  if (x_source     == NULL)   {
      DEBUG_DEPS yLOG_note      ("source is null");
      DEBUG_DEPS yLOG_exit      (__FUNCTION__);
      return rce; 
   }
   DEBUG_DEPS   strlcpy (x_label, g_labeler (x_source->owner), LEN_LABEL);
   DEBUG_DEPS   yLOG_info    ("src label" , x_label);
   DEBUG_DEPS   yLOG_value   ("nreq"  , x_source->nreq);
   DEBUG_DEPS   yLOG_point   ("x_target"  , x_target);
   --rce;  if (x_target     == NULL)   {
      DEBUG_DEPS yLOG_note      ("target is null");
      DEBUG_DEPS yLOG_exit      (__FUNCTION__);
      return rce;
   }
   DEBUG_DEPS   strlcpy (x_label, g_labeler (x_target->owner), LEN_LABEL);
   DEBUG_DEPS   yLOG_info    ("trg label" , x_label);
   DEBUG_DEPS   yLOG_value   ("nprovide"  , x_target->npro);
   /*---(defense: linking back to root)--*/
   --rce;  if (x_target     == myCALC.rroot)  {
      DEBUG_DEPS yLOG_note      ("can not delete a route back to root");
      DEBUG_DEPS yLOG_exit      (__FUNCTION__);
      return rce;
   }
   /*---(defense: circular ref)----------*/
   --rce;  if (x_source     == x_target) {
      DEBUG_DEPS   yLOG_note    ("source and target are same");
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(defense: type)------------------*/
   DEBUG_DEPS   yLOG_char    ("a_type"    , a_type);
   x_index     = ycalc__deps_index (a_type);
   DEBUG_DEPS   yLOG_value   ("dep index" , x_index);
   --rce;  if (x_index      <  0) {
      DEBUG_DEPS   yLOG_note    ("dependency type not found");
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(delete require)-----------------*/
   rc   = ycalc__deps_delete_req (a_type, x_index, x_source, x_target);
   --rce;  if (rc <  0) {
      DEBUG_DEPS   yLOG_note    ("requires did not get removed");
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_DEPS   yLOG_value   ("nreq"  , x_source->nreq);
   /*---(delete provide)-----------------*/
   x_index     = ycalc__deps_match (a_type);
   DEBUG_DEPS   yLOG_value   ("dep index" , x_index);
   rc   = ycalc__deps_delete_pro (a_type, x_index, x_target, x_source);
   --rce;  if (rc <  0) {
      DEBUG_DEPS   yLOG_note    ("provides did not get removed");
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_DEPS   yLOG_value   ("nprovide"  , x_target->npro);
   /*---(check if source needs unroot)---*/
   --rce;
   if        (x_source           == myCALC.rroot) {
      DEBUG_DEPS   yLOG_note    ("source is root, so no unrooting, done");
   } else if (x_source->nreq >  0) {
      DEBUG_DEPS   yLOG_note    ("source has requires, so leave alone");
   } else {
      DEBUG_DEPS   yLOG_note    ("must check/unroot source");
      rc = ycalc__deps_rooting (x_source, G_DEP_UNROOT);
      if (rc <  0) {
         DEBUG_DEPS   yLOG_note    ("source could not be properly unrooted");
         DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
         return rce;
      }
   }
   /*---(check on target)----------------*/
   g_valuer (x_target, &x_type, NULL, NULL);
   if        (x_source           == myCALC.rroot) {
      DEBUG_DEPS   yLOG_note    ("source was root, so this is unrooting, done");
   } else if (x_target->npro >  0) {
      DEBUG_DEPS   yLOG_note    ("target already connected to tree");
   } else if (x_target->nreq >  0) {
      DEBUG_DEPS   yLOG_note    ("target is still needed, must root");
      rc = ycalc__deps_rooting (x_target, G_DEP_ROOT);
      if (rc <  0) {
         DEBUG_DEPS   yLOG_note    ("target could not be properly rooted");
         DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
         return rce;
      }
   } else if (x_type == '-') {
      DEBUG_DEPS   yLOG_note    ("target is a zombie and not required, delete");
      rc = g_reaper (x_target);
      /*> rc = CELL_delete (CHG_NOHIST, x_target->tab, x_target->col, x_target->row);   <*/
      if (rc != 0) {
         DEBUG_DEPS   yLOG_note    ("target could not be properly deleted");
         DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
         return rce;
      }
      x_target == NULL;
   } else {
      DEBUG_DEPS   yLOG_note    ("target is now independent, nothing to do");
   }
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_note    ("all actions complete");
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
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
   char        x_label     [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   /*---(defenses)-----------------------*/
   --rce;  if (a_curr == NULL) {
      DEBUG_DEPS   yLOG_note    ("curr is null, FAIL");
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   --rce;  if (a_curr == myCALC.rroot) {
      DEBUG_DEPS   yLOG_note    ("curr is root, nothing to do");
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return 1;
   }
   DEBUG_DEPS   yLOG_value   ("owners"    , a_curr->npro);
   /*---(quick path, already rooted)-----*/
   --rce;
   if (a_curr->npro == 1 && a_curr->pros->target == myCALC.rroot) {
      switch (a_type) {
      case G_DEP_ROOT      : 
      case G_DEP_CHECKROOT : 
         DEBUG_DEPS   yLOG_note    ("perfectly rooted");
         break;
      case G_DEP_UNROOT    : 
         DEBUG_DEPS   yLOG_note    ("perfect root found, unrooting");
         rc = yCALC_delete (G_DEP_REQUIRE, myCALC.rroot, a_curr);
         if (rc != 0) {
            DEBUG_DEPS   yLOG_note    ("could not delete root dependency");
            DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
            return rce;
         }
         DEBUG_DEPS   yLOG_value   ("owners"    , a_curr->npro);
         break;
      }
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return a_curr->npro;
   }
   /*---(indenpendent cell)--------------*/
   --rce;
   if (a_curr->npro == 0) {
      switch (a_type) {
      case G_DEP_ROOT      : 
         DEBUG_DEPS   yLOG_note    ("independent cell, rooting");
         rc = yCALC_create (G_DEP_REQUIRE, myCALC.rroot, a_curr);
         if (rc != 0) {
            DEBUG_DEPS   yLOG_note    ("could not create root dependency");
            DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
            return rce;
         }
         DEBUG_DEPS   yLOG_value   ("owners"    , a_curr->npro);
         break;
      case G_DEP_CHECKROOT : 
      case G_DEP_UNROOT    : 
         DEBUG_DEPS   yLOG_note    ("independent cell, nothing to do");
         break;
      }
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return a_curr->npro;
   }
   /*---(count link types)---------------*/
   --rce;
   x_next = a_curr->pros;
   while (x_next != NULL) {
      DEBUG_DEPS   strlcpy (x_label, g_labeler (x_next->target->owner), LEN_LABEL);
      DEBUG_DEPS   yLOG_info    ("owned by"  , x_label);
      if (x_next->target != myCALC.rroot)  ++x_norms;
      else                          ++x_roots;
      x_next = x_next->next;
   }
   DEBUG_DEPS   yLOG_value   ("roots"     , x_roots);
   DEBUG_DEPS   yLOG_value   ("norms"     , x_norms);
   /*---(normal dependent cell)----------*/
   if (x_roots == 0) {
      switch (a_type) {
      case G_DEP_CHECKROOT : 
         DEBUG_DEPS   yLOG_note    ("normal dependent cell, no root");
         break;
      case G_DEP_ROOT      : 
         DEBUG_DEPS   yLOG_note    ("normal dependent cell, can not root");
         break;
      case G_DEP_UNROOT    : 
         DEBUG_DEPS   yLOG_note    ("normal dependent cell, no root, can not unroot");
         break;
      }
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return x_roots;
   }
   /*---(deal with duplicate roots)------*/
   --rce;
   x_next  = a_curr->pros;
   if (x_norms == 0)  x_roots = 0;
   else               x_roots = 1;
   while (x_next != NULL) {
      if (x_next->target == myCALC.rroot && x_roots > 0) {
         DEBUG_DEPS   yLOG_note    ("found reduntant root, cleaning");
         rc = yCALC_delete (G_DEP_REQUIRE, myCALC.rroot, a_curr);
      }
      ++x_roots;
      x_next = x_next->next;
   }
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_value   ("owners"    , a_curr->npro);
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
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
   char        x_label     [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   DEBUG_DEPS   yLOG_value   ("a_level"   , a_level);
   /*---(defenses)-----------------------*/
   DEBUG_DEPS   yLOG_point   ("a_source"  , a_source);
   --rce;  if (a_source   == NULL)  {
      DEBUG_DEPS   yLOG_info    ("FAILED"    , "source pointer is null");
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return  rce;
   }
   DEBUG_DEPS   strlcpy (x_label, g_labeler (a_source->owner), LEN_LABEL);
   DEBUG_DEPS   yLOG_info    ("label"     , x_label);
   DEBUG_DEPS   yLOG_point   ("a_target"  , a_target);
   --rce;  if (a_target   == NULL)  {
      DEBUG_DEPS   yLOG_info    ("FAILED"    , "target pointer is null");
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return  rce;
   }
   --rce;  if (a_source == a_target) {
      DEBUG_DEPS   yLOG_info    ("FAILED"    , "match, circular link");
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_DEPS   strlcpy (x_label, g_labeler (a_target->owner), LEN_LABEL);
   DEBUG_DEPS   yLOG_info    ("label"     , x_label);
   --rce;  if (a_level  >= MAX_DEP) {
      DEBUG_DEPS   yLOG_info    ("FAILED"    , "past max depth");
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return  rce;
   }
   /*---(check for already processed)----*/
   if (a_source->u == a_stamp) {
      DEBUG_DEPS   yLOG_info    ("SKIP"      , "already processed");
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return  0;
   }
   /*---(stamp at end)-------------------*/
   a_source->u = a_stamp;
   /*---(process children)---------------*/
   DEBUG_DEPS   yLOG_value   ("nrequire"  , a_source->nreq);
   --rce;
   x_next = a_source->reqs;
   while (x_next != NULL) {
      /*---(recurse to child)------------*/
      DEBUG_DEPS   strlcpy (x_label, g_labeler (x_next->target->owner), LEN_LABEL);
      DEBUG_DEPS   yLOG_info    ("label"     , x_label);
      rc = ycalc__deps_circle (a_level + 1, x_next->target, a_target, a_stamp);
      if (rc < 0) {
         DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
         return rc;
      }
      /*---(prepare for next)------------*/
      x_next = x_next->next;
   }
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                          mass actions                        ----===*/
/*====================------------------------------------====================*/
static void  o___MASS____________o () { return; }

char         /*-> remove all requires deps -----------[ ------ [ge.A34.142.41]*/ /*-[02.0000.515.5]-*/ /*-[--.---.---.--]-*/
ycalc_deps_cleanse      (tDEP_ROOT *a_curr)
{  /*---(design notes)--------------------------------------------------------*/
   /*---(locals)-----------+-----------+-*/
   tDEP_LINK  *x_next      = NULL;
   tDEP_LINK  *x_save      = NULL;
   char        rce         =  -10;
   char        rc          = 0;
   char        x_label     [LEN_LABEL];
   /*---(begin)--------------------------*/
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   DEBUG_DEPS   yLOG_point   ("a_curr"    , a_curr);
   /*---(defenses)-----------------------*/
   --rce;  if (a_curr == NULL) {
      DEBUG_DEPS   yLOG_note    ("exiting, dep root is NULL");
      DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
      return rce;
   }
   /*---(reqs)---------------------------*/
   DEBUG_DEPS   strlcpy (x_label, g_labeler (a_curr->owner), LEN_LABEL);
   DEBUG_DEPS   yLOG_info    ("owned by"  , x_label);
   DEBUG_DEPS   yLOG_value   ("nreq"      , a_curr->nreq);
   x_next = a_curr->reqs;
   while (x_next != NULL) {
      DEBUG_DEPS    yLOG_point   ("x_next"      , x_next);
      x_save = x_next->next;
      DEBUG_DEPS   strlcpy (x_label, g_labeler (x_next->target->owner), LEN_LABEL);
      DEBUG_DEPS   yLOG_complex ("target"    , "ptr=%9p, label=%s", x_next->target, x_label);
      rc = yCALC_delete (x_next->type, a_curr, x_next->target);
      DEBUG_DEPS   yLOG_value   ("nreq"      , a_curr->nreq);
      x_next = x_save;
   }
   /*---(check if rooted)----------------*/
   DEBUG_DEPS   yLOG_value   ("nprovide"  , a_curr->npro);
   DEBUG_DEPS   yLOG_point   ("provides"  , a_curr->pros);
   if (a_curr->npro == 1) {
      DEBUG_DEPS   yLOG_point   ("source"    , a_curr->pros->source);
      DEBUG_DEPS   strlcpy (x_label, g_labeler (a_curr->pros->source->owner), LEN_LABEL);
      DEBUG_DEPS   yLOG_info    ("label"     , x_label);
      DEBUG_DEPS   yLOG_point   ("target"    , a_curr->pros->target);
      DEBUG_DEPS   strlcpy (x_label, g_labeler (a_curr->pros->target->owner), LEN_LABEL);
      DEBUG_DEPS   yLOG_point   ("rroot"     , myCALC.rroot);
      if (a_curr->pros->target == myCALC.rroot) {
         DEBUG_DEPS   yLOG_note    ("unrooting");
         rc = yCALC_delete (G_DEP_REQUIRE, myCALC.rroot, a_curr);
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> establish a range dependency -------[ ------ [ge.K94.17#.D5]*/ /*-[02.0000.109.C]-*/ /*-[--.---.---.--]-*/
yCALC_range      (void *a_source, int x1, int y1, int z1, int x2, int y2, int z2)
{  /*---(design notes)--------------------------------------------------------* 
    * creates the full set of dependencies required when a range formula is   * 
    * used in a cell.  given how this spreadsheet works and visual clarity,   *
    * all cells are created in the range if not already existing.  since      *
    * ranges are not uncommon, this is simply the shared logic to get it      *
    * done.  it is critical that callers pay attention to the return code as  *
    * uncaught errors can run amok.                                           */
   /*---(locals)-----------+-----------+-*//*---------------------------------*/
   tDEP_ROOT  *x_source    = NULL;
   tDEP_ROOT  *x_target    = NULL;          /* cell at current tab, col, row  */
   int         x_pos       = 0;             /* iterator -- columns            */
   int         y_pos       = 0;             /* iterator -- rows               */
   char        rc          = 0;             /* generic return code character  */
   char        rce         = -10;           /* return code for errors         */
   char        rce_save    = 0;             /* saved rce value for backout    */
   char        x_label     [LEN_LABEL];
   /*---(begin)--------------------------*/
   DEBUG_DEPS    yLOG_enter   (__FUNCTION__);
   DEBUG_DEPS    yLOG_point   ("a_source"    , a_source);
   DEBUG_DEPS    yLOG_complex ("range"     , "bt=%4d, bc=%4d, br=%4d, et=%4d, ec=%4d, er=%4d", z1, x1, y1, z2, x2, y2);
   /*---(prepare)------------------------*/
   x_source = (tDEP_ROOT *) a_source;
   /*---(defense: multiple tabs)---------*/
   --rce;  if (z1 != z2)  {
      DEBUG_DEPS    yLOG_note    ("aborting, tabs do not match");
      DEBUG_DEPS    yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(defense: range orientation)-----*/
   --rce;  if (x1 >  x2)  {
      DEBUG_DEPS    yLOG_note    ("aborting, beg col after end col");
      DEBUG_DEPS    yLOG_exit    (__FUNCTION__);
      return rce;
   }
   --rce;  if (y1 >  y2) {
      DEBUG_DEPS    yLOG_note    ("aborting, beg row after end row");
      DEBUG_DEPS    yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(setup dependencies)-------------*/
   DEBUG_DEPS   strlcpy (x_label, g_labeler (x_source->owner), LEN_LABEL);
   DEBUG_DEPS   yLOG_info    ("source"    , x_label);
   --rce;
   for (y_pos = y1; y_pos <= y2; ++y_pos) {
      for (x_pos = x1; x_pos <= x2; ++x_pos) {
         /*---(get existing)-------------*/
         x_target  = g_whois (x_pos, y_pos, z1);
         DEBUG_DEPS    yLOG_complex ("target"    , "col=%4d, row=%4d, ptr=%9p", x_pos, y_pos, x_target);
         if (x_target    == NULL)   continue;
         /*---(create dependency)--------*/
         DEBUG_DEPS   strlcpy (x_label, g_labeler (x_target->owner), LEN_LABEL);
         DEBUG_DEPS   yLOG_info    ("target"    , x_label);
         rc  = yCALC_create (G_DEP_RANGE, x_source, x_target);
         if (rc  <  0)               { rce_save = rce - 3; break; }
      }
      if (rce_save < 0) break;
   }
   /*---(backout)------------------------*/
   if (rce_save < 0) {
      DEBUG_DEPS    yLOG_note    ("aborting, error previously encountered");
      ycalc_deps_cleanse (x_source);
      rce = rce_save;
      DEBUG_DEPS    yLOG_exit    (__FUNCTION__);
      return  rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_DEPS    yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       enable and disable                     ----===*/
/*====================------------------------------------====================*/
static void  o___ENABLE__________o () { return; }

char
yCALC_enable            (void *a_owner, void **a_ycalc)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   tDEP_ROOT  *x_ycalc     = NULL;
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
   DEBUG_DEPS   yLOG_spoint  (a_ycalc);
   --rce;  if (a_ycalc == NULL) {
      DEBUG_DEPS   yLOG_snote   ("no return ycalc pointer");
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_spoint  (*a_ycalc);
   --rce;  if (*a_ycalc != NULL) {
      DEBUG_DEPS   yLOG_snote   ("ycalc pointer already assigned");
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(create)-------------------------*/
   DEBUG_DEPS   yLOG_snote   ("malloc");
   while (x_ycalc == NULL && x_tries < 10) {
      x_ycalc = (tDEP_ROOT *) malloc (sizeof (tDEP_ROOT));
      ++x_tries;
   }
   DEBUG_DEPS   yLOG_svalue  ("tries", x_tries);
   DEBUG_DEPS   yLOG_spoint  (x_ycalc);
   --rce;  if (x_ycalc == NULL) {
      DEBUG_DEPS   yLOG_snote   ("FAILED");
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(calc fields)--------------------*/
   DEBUG_DEPS   yLOG_snote   ("owner");
   x_ycalc->owner   = a_owner;
   /*---(calc fields)--------------------*/
   DEBUG_DEPS   yLOG_snote   ("calc");
   x_ycalc->rpn     = NULL;
   x_ycalc->ncalc   = 0;
   x_ycalc->chead   = NULL;
   x_ycalc->ctail   = NULL;
   x_ycalc->cuse    = 0;
   /*---(dependencies)-------------------*/
   DEBUG_DEPS   yLOG_snote   ("calc");
   x_ycalc->reqs    = NULL;
   x_ycalc->nreq    = 0;
   x_ycalc->pros    = NULL;
   x_ycalc->npro    = 0;
   /*---(sequencing)---------------------*/
   DEBUG_DEPS   yLOG_snote   ("seq");
   x_ycalc->slevel  = 0;
   x_ycalc->snext   = NULL;
   x_ycalc->sprev   = NULL;
   /*---(stamp)--------------------------*/
   DEBUG_DEPS   yLOG_snote   ("stamp");
   x_ycalc->u       = 0;
   /*---(root doubly-linked list)--------*/
   DEBUG_DEPS   yLOG_snote   ("list");
   x_ycalc->rprev   = NULL;
   x_ycalc->rnext   = NULL;
   if (myCALC.rtail == NULL) {
      myCALC.rhead         = x_ycalc;
      myCALC.rtail         = x_ycalc;
   } else {
      x_ycalc->rprev   = myCALC.rtail;
      x_ycalc->rnext   = NULL;
      myCALC.rtail->rnext  = x_ycalc;
      myCALC.rtail         = x_ycalc;
   }
   ++myCALC.rcount;
   /*---(save)---------------------------*/
   DEBUG_DEPS   yLOG_snote   ("save");
   *a_ycalc = x_ycalc;
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
   return 0;
}


/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNIT_TEST_______o () { return; }

char*        /*-> unit testing accessor --------------[ ------ [gs.HA0.1B3.K5]*/ /*-[02.0000.00#.#]-*/ /*-[--.---.---.--]-*/
ycalc__unit_deps        (char *a_question, void *a_point)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_fore      = 0;
   int         x_back      = 0;
   tDEP_LINK  *x_dep       = NULL;
   tDEP_ROOT  *x_root      = NULL;
   char        x_list      [LEN_RECD ];
   char        x_label     [LEN_LABEL];
   /*---(preprare)-----------------------*/
   strcpy (ycalc__unit_answer, "yCALC            : question not understood");
   x_dep  = (tDEP_LINK *) a_point;
   x_root = (tDEP_ROOT *) a_point;
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
      ycalc_audit_disp_reqs (x_root, x_list);
      strlcpy (x_label, g_labeler (x_root->owner), LEN_RECD);
      snprintf (ycalc__unit_answer, LEN_RECD, "yCALC deps reqs  : %-5s (%2d) %-.40s", x_label, x_root->nreq, x_list);
   }
   else if (strcmp (a_question, "provides"    )   == 0) {
      ycalc_audit_disp_pros (x_root, x_list);
      strlcpy (x_label, g_labeler (x_root->owner), LEN_RECD);
      snprintf (ycalc__unit_answer, LEN_RECD, "yCALC deps pros  : %-5s (%2d) %-.40s", x_label, x_root->npro, x_list);
   }
   /*---(complete)-----------------------*/
   return ycalc__unit_answer;
}



