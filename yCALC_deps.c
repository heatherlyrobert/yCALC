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
yCALC__deps_new         (tDEP_LINK **a_dep)
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
yCALC__deps_free        (tDEP_LINK **a_dep)
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
yCALC_deps_init         (void)
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

char         /*-> destory all dependencies -----------[ ------ [fe.841.032.21]*/ /*-[01.0000.013.5]-*/ /*-[--.---.---.--]-*/
yCALC__deps_purge       (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tDEP_LINK  *x_curr      = NULL;
   tDEP_LINK  *x_next      = NULL;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(walk through list)--------------*/
   DEBUG_PROG   yLOG_point   ("head"      , myCALC.dhead);
   --rce;  if (myCALC.dhead == NULL) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_curr = myCALC.dhead;
   while (x_next != NULL) {
      x_next = x_curr->dnext;
      yCALC__deps_free (&x_curr);
      x_curr = x_next;
   }
   /*---(reset globals)------------------*/
   myCALC.dhead   = NULL;
   myCALC.dtail   = NULL;
   myCALC.dcount  = 0;
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> shut down dependency usage ---------[ shoot  [gz.421.001.02]*/ /*-[00.0000.102.1]-*/ /*-[--.---.---.--]-*/
yCALC_deps_wrap         (void)
{
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(dependencies)-------------------*/
   yCALC__deps_purge ();
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
void  o___UNIT_TEST_______o () { return; }

char*        /*-> unit testing accessor --------------[ ------ [gs.HA0.1B3.K5]*/ /*-[02.0000.00#.#]-*/ /*-[--.---.---.--]-*/
yCALC__unit_deps        (char *a_question, void *a_point)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_fore      = 0;
   int         x_back      = 0;
   char        temp        [LEN_RECD];
   tDEP_LINK  *x_dep       = NULL;
   char        rc          = 0;
   int         x_tab       = 0;
   int         x_col       = 0;
   int         x_row       = 0;
   int         i           = 0;
   /*---(preprare)-----------------------*/
   strcpy (yCALC__unit_answer, "yCALC            : question not understood");
   /*---(dependency list)----------------*/
   if      (strcmp (a_question, "count"    )      == 0) {
      x_dep  = myCALC.dhead; while (x_dep  != NULL) { ++x_fore; x_dep  = x_dep ->dnext; }
      x_dep  = myCALC.dtail; while (x_dep  != NULL) { ++x_back; x_dep  = x_dep ->dprev; }
      snprintf (yCALC__unit_answer, LEN_RECD, "yCALC deps count : num=%4d, fore=%4d, back=%4d", myCALC.dcount, x_fore, x_back);
   }
   else if (strcmp (a_question, "linked_list" )   == 0) {
      snprintf (yCALC__unit_answer, LEN_RECD, "yCALC deps list  : num=%4d, head=%9p, tail=%9p", myCALC.dcount, myCALC.dhead, myCALC.dtail);
   }
   /*---(complete)-----------------------*/
   return yCALC__unit_answer;
}



