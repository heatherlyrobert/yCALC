/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"




/*====================------------------------------------====================*/
/*===----                      dependency sequencing                   ----===*/
/*====================------------------------------------====================*/
static void  o___SEQUENCING______o () { return; }
/*
 *  dependency sequencing must allow...
 *  -- access in layers according to calculation/depencency requirements
 *  -- rapid linear access for driving calculations
 *  -- rapid (if not direct) access from an individual cell
 *  -- rapid creation (avoid malloc)
 *
 *  have chosen to use an array of simple lists implemented within the cells.
 *  made the lists doublly linked to speed removing random cells.
 *
 *
 */

#define   MAX_EXEC        100
/*--------- ----------- ----------- ----------- */
static      int         s_max       = -1;
static      int         s_total     =  0;
static      int         s_count     [MAX_EXEC];
static      tDEP_ROOT  *s_heads     [MAX_EXEC];
static      tDEP_ROOT  *s_tails     [MAX_EXEC];

char         /*-> prepare for new calculation --------[ leaf   [fz.742.031.10]*/ /*-[01.0000.014.2]-*/ /*-[--.---.---.--]-*/
ycalc__seq_clear        (void)
{
   /*---(locals)-----------+-----------+-*/
   tDEP_ROOT  *x_next      = NULL;
   char        i           =    0;
   int         c           =    0;
   int         x_len       =    0;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(walk through list)--------------*/
   DEBUG_CALC   yLOG_note    ("resetting cell calc exec fields");
   x_next = myCALC.rroot;
   while (x_next != NULL) {
      DEBUG_CALC   yLOG_point   ("x_next"    , x_next);
      DEBUG_CALC   yLOG_info    ("label"     , ycalc_call_labeler (x_next));
      x_len = strllen (ycalc_call_labeler (x_next), LEN_LABEL);
      DEBUG_CALC   yLOG_value   ("x_len"     , x_len);
      DEBUG_CALC   yLOG_point   ("rnext"     , x_next->rnext);
      x_next->slevel = -1;
      x_next->snext  = NULL;
      x_next->sprev  = NULL;
      if (x_next == x_next->rnext)  break;
      x_next = x_next->rnext;
      ++c;
   }
   DEBUG_CALC   yLOG_value   ("target"    , myCALC.rcount);
   DEBUG_CALC   yLOG_value   ("cleared"   , c);
   DEBUG_CALC   yLOG_note    ("resetting calc exec heads and tails");
   for (i = 0; i < MAX_EXEC; ++i) {
      s_count [i] = 0;
      s_heads [i] = NULL;
      s_tails [i] = NULL;
   }
   s_total =  0;
   s_max   = -1;
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> add a cell to calculation level ----[ leaf   [fe.D54.215.70]*/ /*-[01.0000.015.6]-*/ /*-[--.---.---.--]-*/
ycalc__seq_add          (char a_level, tDEP_ROOT *a_deproot)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_senter  (__FUNCTION__);
   /*---(defense : level)----------------*/
   DEBUG_CALC   yLOG_svalue  ("a_level"   , a_level);
   --rce;  if (a_level >= MAX_EXEC) {
      DEBUG_CALC   yLOG_snote   ("level greater than max");
      DEBUG_CALC   yLOG_sexit   (__FUNCTION__);
      return rce;
   }
   /*---(defense : cell)-----------------*/
   DEBUG_CALC   yLOG_spoint  (a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_CALC   yLOG_snote   ("NULL cell");
      DEBUG_CALC   yLOG_sexit   (__FUNCTION__);
      return rce;
   }
   DEBUG_CALC   yLOG_sinfo   ("label"     , ycalc_call_labeler (a_deproot));
   DEBUG_CALC   yLOG_spoint  (myCALC.rroot);
   --rce;  if (a_deproot == myCALC.rroot) {
      DEBUG_CALC   yLOG_snote   ("cell is root");
      DEBUG_CALC   yLOG_sexit   (__FUNCTION__);
      return -(rce);
   }
   /*---(defense : already assigned)-----*/
   DEBUG_CALC   yLOG_svalue  ("slevel"    , a_deproot->slevel);
   --rce;  if (a_deproot->slevel >= 0) {
      DEBUG_CALC   yLOG_snote   ("already assigned");
      DEBUG_CALC   yLOG_sexit   (__FUNCTION__);
      return -(rce);
   }
   /*---(first item)---------------------*/
   if (s_heads [a_level] == NULL) {
      DEBUG_CALC   yLOG_snote   ("add as first");
      s_heads [a_level] = a_deproot;
      a_deproot->slevel   = a_level;
      a_deproot->sprev    = NULL;
      a_deproot->snext    = NULL;
      s_tails [a_level] = a_deproot;
   }
   /*---(add to tail)--------------------*/
   else {
      DEBUG_CALC   yLOG_snote   ("add to tail");
      s_tails [a_level]->snext = a_deproot;
      a_deproot->slevel   = a_level;
      a_deproot->sprev    = s_tails [a_level];
      a_deproot->snext    = NULL;
      s_tails [a_level] = a_deproot;
   }
   /*---(update totals)------------------*/
   DEBUG_CALC   yLOG_snote   ("counts)");
   ++(s_count [a_level]);
   ++s_total;
   if (a_level > s_max)  s_max = a_level;
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char         /*-> delete a cell from calulation ------[ leaf   [fe.A43.123.60]*/ /*-[01.0000.015.3]-*/ /*-[--.---.---.--]-*/
ycalc__seq_del          (tDEP_ROOT *a_deproot)
{
   /*---(locals)-----------+-----------+-*/
   char        x_level     = 0;
   char        rce         = -10;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(defense : cell)-----------------*/
   DEBUG_CALC   yLOG_point   ("a_deproot" , a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_CALC   yLOG_note    ("NULL cell");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_info    ("label"     , ycalc_call_labeler (a_deproot));
   /*---(defense : unassigned)-----------*/
   x_level = a_deproot->slevel;
   DEBUG_CALC   yLOG_value   ("x_level"   , x_level);
   --rce;  if (x_level < 0) {
      DEBUG_CALC   yLOG_note    ("unassigned already");
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return -(rce);
   }
   /*---(next)---------------------------*/
   DEBUG_CALC   yLOG_note    ("forward");
   if (a_deproot->snext == NULL)  s_tails [x_level]     = a_deproot->sprev;
   else                           a_deproot->snext->sprev = a_deproot->sprev;
   /*---(prev)---------------------------*/
   DEBUG_CALC   yLOG_note    ("backward");
   if (a_deproot->sprev == NULL)  s_heads [x_level]     = a_deproot->snext;
   else                        a_deproot->sprev->snext = a_deproot->snext;
   /*---(update cell)--------------------*/
   DEBUG_CALC   yLOG_note    ("clear");
   a_deproot->slevel   = -1;
   a_deproot->sprev    = NULL;
   a_deproot->snext    = NULL;
   /*---(update totals)------------------*/
   DEBUG_CALC   yLOG_note    ("counts");
   --(s_count [x_level]);
   --s_total;
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> dependency-based calc sequencing ---[ ------ [fc.F55.135.E3]*/ /*-[02.0000.024.!]-*/ /*-[--.---.---.--]-*/
ycalc__seq_recursion    (int a_level, tDEP_LINK *a_dep, char a_dir, long a_stamp)
{
   /*---(locals)-----------+-----------+-*/
   tDEP_LINK  *x_dep       = NULL;
   tDEP_ROOT  *x_curr      = NULL;
   char        rc          = 0;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   DEBUG_CALC   yLOG_value   ("a_level"   , a_level);
   DEBUG_CALC   yLOG_value   ("a_stamp"   , a_stamp);
   DEBUG_CALC   yLOG_char    ("a_dir"     , a_dir);
   /*---(defenses)-----------------------*/
   DEBUG_CALC   yLOG_point   ("*x_dep"    , a_dep);
   if (a_dep        == NULL) {
      DEBUG_CALC   yLOG_note    ("a_dep is NULL");
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   DEBUG_CALC   yLOG_char    ("dep type"  , a_dep->type);
   x_curr     = a_dep->target;
   DEBUG_CALC   yLOG_point   ("*x_curr"   , x_curr);
   if (x_curr       == NULL) {
      DEBUG_CALC   yLOG_note    ("cell is NULL");
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   DEBUG_CALC   yLOG_info    ("curr label", ycalc_call_labeler (x_curr));
   /*---(calculate)----------------------*/
   if (x_curr->u != a_stamp) {
      DEBUG_CALC   yLOG_note    ("stamp does not match");
      rc = ycalc__seq_add  (a_level, x_curr);
   } else if (a_dir == 'u' && x_curr->slevel < a_level) {
      DEBUG_CALC   yLOG_note    ("up and must move lower");
      rc = ycalc__seq_del  (x_curr);
      if (rc == 0)  rc = ycalc__seq_add  (a_level, x_curr);
   } else if (a_dir == 'd' && x_curr->slevel < a_level) {
      DEBUG_CALC   yLOG_note    ("down and must move higher");
      rc = ycalc__seq_del  (x_curr);
      if (rc == 0)  rc = ycalc__seq_add  (a_level, x_curr);
   } else {
      DEBUG_CALC   yLOG_note    ("everything is fine");
   }
   if (rc != 0) {
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(update cell and dep)------------*/
   x_curr->u = a_stamp;
   /*---(recurse)------------------------*/
   DEBUG_CALC   yLOG_value   ("npro"  , x_curr->npro);
   DEBUG_CALC   yLOG_value   ("nreq"  , x_curr->nreq);
   if (a_dir == 'u')  x_dep = x_curr->pros;
   else               x_dep = x_curr->reqs;
   while (x_dep != NULL) {
      rc = ycalc__seq_recursion (a_level + 1, x_dep, a_dir, a_stamp);
      if (rc < 0) {
         DEBUG_CALC   yLOG_exit    (__FUNCTION__);
         return rc;
      }
      x_dep = x_dep->next;
   }
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

long
yCALC_getstamp          (void *a_deproot)
{
   tDEP_ROOT  *x_deproot   = NULL;
   if (a_deproot == NULL)  return -1;
   x_deproot = (tDEP_ROOT *) a_deproot;
   return x_deproot->u;
}

char         /*-> dependency-based calc marking ------[ ------ [fe.I85.584.97]*/ /*-[03.0000.063.!]-*/ /*-[--.---.---.--]-*/
ycalc__seq_driver       (tDEP_ROOT *a_deproot, char a_dir_rec, char a_dir_act, long a_stamp, void *a_consumer)
{
   /*---(locals)-------------------------*/
   char        rce         = -10;
   tDEP_LINK  *x_dep       = NULL;
   tDEP_ROOT  *x_deproot   = NULL;
   tDEP_ROOT  *x_next      = NULL;
   int         i           = 0;
   int         x_sub       = 0;
   int         x_seq       = 0;
   int         x_lvl       = 0;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   DEBUG_CALC   yLOG_char    ("a_dir_rec" , a_dir_rec);
   DEBUG_CALC   yLOG_char    ("a_dir_act" , a_dir_act);
   DEBUG_CALC   yLOG_value   ("a_stamp"   , a_stamp);
   DEBUG_CALC   yLOG_point   ("a_consumer", a_consumer);
   /*---(defense : cell)-----------------*/
   DEBUG_CALC   yLOG_point   ("a_deproot" , a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_CALC   yLOG_note    ("NULL deproot");
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_CALC   yLOG_info    ("label"     , ycalc_call_labeler (a_deproot));
   DEBUG_CALC   yLOG_value   ("npro"      , a_deproot->npro);
   DEBUG_CALC   yLOG_value   ("nreq"      , a_deproot->nreq);
   g_consumer = a_consumer;
   /*---(prepare)------------------------*/
   ycalc__seq_clear ();
   /*---(recurse)------------------------*/
   if (a_dir_rec == 'u') {
      x_dep = a_deproot->pros;
      x_sub = a_deproot->npro;
   } else {
      x_dep = a_deproot->reqs;
      x_sub = a_deproot->nreq;
   }
   while (x_dep != NULL) {
      ++i;
      DEBUG_CALC   yLOG_complex ("recurse"   , "%d of %d, %s", i, x_sub, ycalc_call_labeler (x_dep->target));
      ycalc__seq_recursion (0, x_dep, a_dir_rec, a_stamp);
      x_dep = x_dep->next;
   }
   DEBUG_CALC   yLOG_note    ("done recursing");
   /*---(execution : defense)------------*/
   if (s_total <= 0) {
      DEBUG_CALC   yLOG_note    ("no calculations required");
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(act on sequenced list)----------*/
   for (i = 0; i <= s_max; ++i) {
      x_lvl = i;
      if (a_dir_act == 'u') x_lvl = s_max - i;
      DEBUG_CALC   yLOG_value   ("LEVEL"     , x_lvl);
      x_deproot = s_heads [x_lvl];
      DEBUG_CALC   yLOG_point   ("x_deproot"    , x_deproot);
      x_sub = 0;
      while (x_deproot != NULL) {
         x_next = x_deproot->snext;
         if (g_consumer != NULL)   g_consumer (x_deproot->owner, x_deproot, x_seq, x_lvl);
         ++x_sub;
         ++x_seq;
         x_deproot = x_next;
      }
      DEBUG_CALC   yLOG_value   ("expected"  , s_count [x_lvl]);
      DEBUG_CALC   yLOG_value   ("subtotal"  , x_sub);
   }
   DEBUG_CALC   yLOG_value   ("expected"  , s_total);
   DEBUG_CALC   yLOG_value   ("subtotal"  , x_seq);
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        simplifiers                           ----===*/
/*====================------------------------------------====================*/
static void  o___SIMPLIFIERS_____o () { return; }

char         /*-> dependency-based calc upward -------[ leaf   [gc.950.028.3C]*/ /*-[02.0000.304.5]-*/ /*-[--.---.---.--]-*/
yCALC_seq_up       (void *a_deproot, void *a_consumer) { return ycalc__seq_driver (a_deproot   , 'u', 'd', rand() , a_consumer); }

char         /*-> dependency-based calc downward -----[ leaf   [gc.940.027.3A]*/ /*-[02.0000.00#.1]-*/ /*-[--.---.---.--]-*/
yCALC_seq_down     (void *a_deproot, void *a_consumer) { return ycalc__seq_driver (a_deproot   , 'd', 'u', rand() , a_consumer); }

char         /*-> dependency-based calculation all ---[ ------ [gc.840.026.38]*/ /*-[02.0000.402.1]-*/ /*-[--.---.---.--]-*/
yCALC_seq_full     (void *a_consumer)                  { return ycalc__seq_driver (myCALC.rroot, 'd', 'u', rand() , a_consumer); }

char         /*-> dependency-based wiping of cells ---[ ------ [gc.740.025.36]*/ /*-[02.0000.104.!]-*/ /*-[--.---.---.--]-*/
yCALC_seq_downdown (long a_stamp, void *a_consumer)    { return ycalc__seq_driver (myCALC.rroot, 'd', 'd', a_stamp, a_consumer); }

char         /*-> dependency-based wiping of cells ---[ ------ [gc.740.025.36]*/ /*-[02.0000.104.!]-*/ /*-[--.---.---.--]-*/
yCALC_seq_downup   (long a_stamp, void *a_consumer)    { return ycalc__seq_driver (myCALC.rroot, 'd', 'u', a_stamp, a_consumer); }

char         /*-> dependency-based calculation all ---[ ------ [gc.840.026.38]*/ /*-[02.0000.402.1]-*/ /*-[--.---.---.--]-*/
yCALC_calculate    (void)                              { return ycalc__seq_driver (myCALC.rroot, 'd', 'u', rand() , ycalc_execute_auto); }

char         /*-> dependency-based calculation all ---[ ------ [gc.840.026.38]*/ /*-[02.0000.402.1]-*/ /*-[--.---.---.--]-*/
yCALC_calc_from    (void *a_deproot)                   { return ycalc__seq_driver (a_deproot   , 'u', 'd', rand() , ycalc_execute_auto); }



/*====================------------------------------------====================*/
/*===----                         reporting                            ----===*/
/*====================------------------------------------====================*/
static void  o___REPORTING_______o () { return; }

char         /*-> tbd --------------------------------[ leaf   [gc.630.122.30]*/ /*-[02.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc__seq_list    (char *a_list)
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;
   tDEP_ROOT  *x_curr      = NULL;
   /*---(defenses)--------------------*/
   if (a_list  == NULL) return -1;     /* then no point                       */
   /*---(walk the list)---------------*/
   strncpy (a_list, "", LEN_RECD);
   for (i = 0; i <= s_max; ++i) {
      x_curr = s_heads [i];
      while (x_curr != NULL) {
         strncat (a_list, ycalc_call_labeler (x_curr), LEN_RECD);
         strncat (a_list, ",", LEN_RECD);
         x_curr = x_curr->snext;
      }
      strncat (a_list, ";", LEN_RECD);
   }
   if (strcmp (a_list, "") == 0) {
      strncpy (a_list, "((nada))", LEN_RECD);
   }
   /*---(complete)--------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNIT_TEST_______o () { return; }

char*        /*-> unit testing accessor --------------[ ------ [gs.HA0.1B3.K5]*/ /*-[02.0000.00#.#]-*/ /*-[--.---.---.--]-*/
ycalc__unit_seq         (char *a_question, void *a_point)
{
   /*---(locals)-----------+-----+-----+-*/
   tDEP_LINK  *x_dep       = NULL;
   tDEP_ROOT  *x_root      = NULL;
   char        x_label     [LEN_LABEL];
   char        x_temp      [LEN_RECD ];
   int         i           =    0;
   /*---(preprare)-----------------------*/
   strcpy (ycalc__unit_answer, "yCALC            : question not understood");
   x_dep  = (tDEP_LINK *) a_point;
   x_root = (tDEP_ROOT *) a_point;
   /*---(dependency list)----------------*/
   if      (strcmp (a_question, "counts"   )      == 0) {
      sprintf  (ycalc__unit_answer, "yCALC seq count  : %03d %02d", s_total, s_max);
      for (i = 0; i <= s_max; ++i) {
         if (i == 0)  strcat (ycalc__unit_answer, "  ");
         sprintf (x_temp, " %02d", s_count [i]);
         strcat  (ycalc__unit_answer, x_temp);
      }
   }
   else if (strcmp (a_question, "level"       )   == 0) {
      strlcpy (x_label, ycalc_call_labeler (x_root), LEN_RECD);
      if (x_root->slevel >= 0) snprintf (ycalc__unit_answer, LEN_RECD, "yCALC seq level  : %-5.5s  %2d", x_label, x_root->slevel);
      else                     snprintf (ycalc__unit_answer, LEN_RECD, "yCALC seq level  : %-5.5s  %2s", x_label, "--"          );
   }
   /*---(complete)-----------------------*/
   return ycalc__unit_answer;
}


