/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"



/*====================------------------------------------====================*/
/*===----                          sorting list                        ----===*/
/*====================------------------------------------====================*/
static void  o___SORT____________o () { return; }

static int  s_array     [1000];          /* working list as integers       */
static int  s_narray    =  0;            /* working count of entries       */

char         /*-> sort cell label list ---------------[ ------ [ge.RE5.1H6.A2]*/ /*-[03.0000.11#.!]-*/ /*-[--.---.---.--]-*/
ycalc__sort_prep   (char *a_list)
{
   /*---(locals)-----------+-----------+-*//*---------------------------------*/
   char        rce         = -10;           /* return code for errors         */
   char        x_list      [1000] = "";     /* copy of the list for parsing   */
   char       *p           = NULL;          /* strtok parsing pointer         */
   char       *q           = ",;";          /* strtok delimeter string        */
   char       *r           = NULL;
   int         i           =  0;            /* loop iterator -- entry         */
   int         x           =  0;            /* col of current entry           */
   int         y           =  0;            /* row of current entry           */
   int         z           =  0;            /* tab of current entry           */
   char        rc          =  0;            /* return code                    */
   tDEP_ROOT  *x_deproot   = NULL;
   /*---(header)-------------------------*/
   DEBUG_SORT    yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   DEBUG_SORT    yLOG_note    ("initialize");
   for (i = 0; i < 1000; ++i)  s_array [i] = 0;
   s_narray = 0;
   /*---(parse/load)---------------------*/
   DEBUG_SORT    yLOG_note    ("load the array");
   strcpy (x_list, a_list);
   p = strtok_r (x_list, q, &r);
   s_narray = 0;
   --rce;  while (p != NULL) {
      DEBUG_SORT    yLOG_info    ("parse"     , p);
      ycalc_call_who_named (p, NULL, &x_deproot);
      DEBUG_SORT    yLOG_point   ("x_deproot" , x_deproot);
      if (x_deproot == NULL)  {
         DEBUG_SORT    yLOG_note    ("could not find deproot, EXITING");
         DEBUG_SORT    yLOG_exit    (__FUNCTION__);
         return rce;
      }
      rc = g_addresser (x_deproot->owner, &x, &y, &z);
      DEBUG_SORT    yLOG_value   ("rc"        , rc);
      if (rc < 0)  {
         DEBUG_SORT    yLOG_note    ("could not parse, EXITING");
         DEBUG_SORT    yLOG_exit    (__FUNCTION__);
         return rce;
      }
      DEBUG_SORT    yLOG_complex ("parts"     , "z=%04d, x=%04d, y=%04d", z, x, y);
      ++z;
      ++x;
      ++y;
      DEBUG_SORT    yLOG_complex ("inserted"  , "z=%04d, x=%04d, y=%04d", z, x, y);
      s_array [s_narray] = (z * 100000000) + (x * 100000) + (y * 10);
      DEBUG_SORT    yLOG_pair    (s_narray         , s_array [s_narray]);
      p = strtok_r (NULL  , q, &r);
      ++s_narray;
   }
   DEBUG_SORT    yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> sort cell label list ---------------[ ------ [ge.RE5.1H6.A2]*/ /*-[03.0000.11#.!]-*/ /*-[--.---.---.--]-*/
ycalc__sort_itself (void)
{
   /*---(locals)-----------+-----------+-*//*---------------------------------*/
   int         i           =  0;            /* loop iterator -- entry         */
   int         a           =  0;            /* comparison entry one           */
   int         b           =  0;            /* comparison entry two           */
   int         tele        = -1;            /* teleport point to speed sort   */
   /*---(header)-------------------------*/
   DEBUG_SORT    yLOG_enter   (__FUNCTION__);
   /*---(sort)---------------------------*/
   i = 1;
   while (i < s_narray) {
      /*---(load vars)-------------------*/
      a = s_array [i - 1];
      b = s_array [i];
      DEBUG_SORT    yLOG_complex ("current"   , "compare i,%3d (t,%3d) =  a,%10d to b,%10d", i, tele, a, b);
      /*---(compare)---------------------*/
      if (i == 0 || a <= b) {
         if (tele >= 0) {
            i    = tele;
            tele = -1;
         } else {
            ++i;
         }
         continue;
      }
      /*---(swap)------------------------*/
      s_array [i - 1] = b;
      s_array [i]     = a;
      a = s_array [i - 1];
      b = s_array [i];
      DEBUG_SORT    yLOG_complex ("swapped"   , "now     i,%3d (t,%3d) =  a,%10d to b,%10d", i, tele, a, b);
      /*---(update)----------------------*/
      if (tele < 0) tele = i;
      if (i > 1) --i;
   }
   DEBUG_SORT    yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> sort cell label list ---------------[ ------ [ge.RE5.1H6.A2]*/ /*-[03.0000.11#.!]-*/ /*-[--.---.---.--]-*/
ycalc__sort_wrap   (char *a_list)
{
   /*---(locals)-----------+-----------+-*//*---------------------------------*/
   char        rce         =  -10;
   int         i           =  0;            /* loop iterator -- entry         */
   int         x           =  0;            /* col of current entry           */
   int         y           =  0;            /* row of current entry           */
   int         z           =  0;            /* tab of current entry           */
   tDEP_ROOT  *x_deproot   = NULL;
   char        x_label     [15];            /* label for sorted entry         */
   /*---(header)-------------------------*/
   DEBUG_SORT    yLOG_enter   (__FUNCTION__);
   strcpy (a_list, ",");
   for (i = 0; i < s_narray; ++i) {
      DEBUG_SORT    yLOG_value   ("value"   , s_array[i]);
      z     = s_array[i] / 100000000;
      x     = (s_array[i] - (z * 100000000))  / 100000;
      y     = (s_array[i] - (z * 100000000) - (x * 100000))         / 10;
      DEBUG_SORT    yLOG_complex ("removed"   , "z=%04d, x=%04d, y=%04d", z, x, y);
      --z;
      --x;
      --y;
      DEBUG_SORT    yLOG_complex ("parts"     , "z=%04d, x=%04d, y=%04d", z, x, y);
      ycalc_call_who_at (x, y, z, NULL, &x_deproot);
      if (x_deproot == NULL)  {
         DEBUG_SORT    yLOG_note    ("could not find deproot, EXITING");
         DEBUG_SORT    yLOG_exit    (__FUNCTION__);
         return rce;
      }
      strlcpy (x_label, g_labeler (x_deproot->owner), LEN_LABEL);
      DEBUG_SORT    yLOG_info    ("label"   , x_label);
      strcat (a_list, x_label);
      strcat (a_list, ",");
   }
   DEBUG_SORT    yLOG_info    ("final"     , a_list);
   /*---(complete)-----------------------*/
   DEBUG_SORT    yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> sort cell label list ---------------[ ------ [ge.RE5.1H6.A2]*/ /*-[03.0000.11#.!]-*/ /*-[--.---.---.--]-*/
ycalc__audit_sort  (char *a_list)
{
   /*---(purpose)-------------------------------------------------------------* 
    * for unit testing to be reliable, cosmetic changes must not result in    *
    * failures or the need to update the unit test script.  label lists from  *
    * the dependency linked lists are a prime example.  they are added in the *
    * order they are encountered, but simple reordering of operations can     *
    * make them appear different even if the same cells are listed.  this     *
    * routine simply sorts a list of dependencies so they are more stable.    */
   /*---(design notes)--------------------------------------------------------* 
    * there are many sort algorithms which can handle vast amounts of complex *
    * data, but they are simply overkill and problematic for small, simple    *
    * lists.  the teleporting gnome sort is my personal favorite due to its   *
    * simplicity which in turn leads to maintainability and confidence.       */
   /*---(oddities)------------------------------------------------------------* 
    * one is added to tab, col, and row before storage in the array and       *
    * sorting as the all three are consolidated mathmatically into a single   *
    * fieild and zeros are just a little harder to debug ;)                   */
   /*---(locals)-----------+-----------+-*//*---------------------------------*/
   char        rce         = -10;           /* return code for errors         */
   char        rc          =  0;            /* return code                    */
   /*---(header)-------------------------*/
   DEBUG_SORT    yLOG_enter   (__FUNCTION__);
   /*---(defense: null)------------------*/
   --rce;  if (a_list  == NULL)  {
      DEBUG_SORT    yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(sort)---------------------------*/
   if (rc == 0)   rc = ycalc__sort_prep   (a_list);
   if (rc == 0)   rc = ycalc__sort_itself ();
   if (rc == 0)   rc = ycalc__sort_wrap   (a_list);
   /*---(complete)-----------------------*/
   DEBUG_SORT    yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                         information                          ----===*/
/*====================------------------------------------====================*/
static void  o___INFO____________o () { return; }

char         /*-> list dependencies ------------------[ leaf   [fe.740.424.60]*/ /*-[02.0000.034.!]-*/ /*-[--.---.---.--]-*/
ycalc__audit_disp_master   (tDEP_ROOT *a_me, char *a_list, char a_start, char *a_types)
{  /*---(design notes)-------------------*/
   /* combined logic from the three accessors below as they were 90% the same */
   /*---(locals)-----------+-----------+-*/
   tDEP_LINK  *n           = NULL;
   char        rce         = -10;
   /*---(defenses)-----------------------*/
   --rce;  if (a_list  == NULL)  return rce;
   strncpy (a_list, "-", LEN_RECD);   /* special for a null list */
   --rce;  if (a_me    == NULL)  return rce;
   /*---(setup)--------------------------*/
   strncpy (a_list, ",", LEN_RECD);
   --rce;  switch (a_start) {
   case 'R' :  n = a_me->reqs; break;
   case 'P' :  n = a_me->pros; break;
   default         :  return rce;
   }
   /*---(walk the list)------------------*/
   while (n != NULL) {
      if (strchr (a_types, n->type) != 0) {
         strlcat (a_list, g_labeler (n->target->owner), LEN_RECD);
         strlcat (a_list, ","                         , LEN_RECD);
      }
      n = n->next;
   }
   /*---(catch empty)--------------------*/
   if (strcmp (a_list, ",") == 0)   strcpy (a_list, ".");
   else  ycalc__audit_sort (a_list);
   /*---(complete)-----------------------*/
   return 0;
}

char       ycalc_audit_disp_reqs      (tDEP_ROOT *a_me, char *a_list) { return ycalc__audit_disp_master (a_me, a_list, 'R', S_DEP_REQS); }
char       ycalc_audit_disp_pros      (tDEP_ROOT *a_me, char *a_list) { return ycalc__audit_disp_master (a_me, a_list, 'P', S_DEP_PROS); }
char       ycalc_audit_disp_like      (tDEP_ROOT *a_me, char *a_list) { return ycalc__audit_disp_master (a_me, a_list, 'P', S_DEP_LIKE); }




