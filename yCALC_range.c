/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"


#define     MAX_RANGE       500
typedef   struct  cyCALC_RANGE  tyCALC_RANGE;
struct cyCALC_RANGE {
   char        name        [LEN_LABEL];
   int         bx, by, bz;
   int         ex, ey, ez;
   tDEP_ROOT  *ycalc;
} s_ranges [MAX_RANGE];
int     s_nrange          = 0;



static int     s_begx     = 0;
static int     s_begy     = 0;
static int     s_begz     = 0;

static int     s_endx     = 0;
static int     s_endy     = 0;
static int     s_endz     = 0;



static int     s_count_all  =    0;
static int     s_count_fil  =    0;
static int     s_count_num  =    0;
static int     s_count_str  =    0;
static int     s_count_oth  =    0;
static int     s_count_calc =    0;
static int     s_count_ptr  =    0;
static double  s_total      =  0.0;
static double  s_min        =  0.0;
static double  s_max        =  0.0;
static double  s_entries    [1000];
static int     s_x          =    0;
static int     s_y          =    0;
static int     s_z          =    0;

#define        S_MAX        1000000000


/*====================------------------------------------====================*/
/*===----                        range functions                       ----===*/
/*====================------------------------------------====================*/
void  o___RANGES__________o () { return; }

char
ycalc_range_wipe        (int n)
{
   DEBUG_CALC   yLOG_senter  (__FUNCTION__);
   DEBUG_CALC   yLOG_sint    (n);
   strlcpy (s_ranges [n].name, "", LEN_LABEL);
   s_ranges [n].ycalc                = NULL;
   s_ranges [n].bx = s_ranges [n].ex =  -10;
   s_ranges [n].by = s_ranges [n].ey =  -10;
   s_ranges [n].bz = s_ranges [n].ez =  -10;
   DEBUG_CALC   yLOG_sexit   (__FUNCTION__);
   return 0;
}

int
ycalc_range_init        (void)
{
   int         i           =    0;
   for (i = 0; i < MAX_RANGE; ++i) {
      ycalc_range_wipe (i);
   }
   s_nrange = 0;
}

int
ycalc_range_next        (void)
{
   int         i           =    0;
   for (i = 0; i < MAX_RANGE; ++i) {
      if (s_ranges [i].ycalc != NULL)  continue;
      return i;
   }
   return -1;
}

char*
ycalc_range_label       (int n)
{
   return s_ranges [n].name;
}

int
ycalc_range_size        (int n)
{
   int         x, y, z;
   s_x = x = s_ranges [n].ex - s_ranges [n].bx + 1;
   s_y = y = s_ranges [n].ey - s_ranges [n].by + 1;
   s_z = z = s_ranges [n].ez - s_ranges [n].bz + 1;
   return x * y * z;
}

char
ycalc_range_deproot     (char *a_name, tDEP_ROOT **a_deproot)
{
   char        rce         =  -10;
   int         i           =    0;
   int         n           =  -1;
   if (a_deproot != NULL)  *a_deproot = NULL;
   --rce;  if (a_name == NULL)  return rce;
   for (i = 0; i < s_nrange; ++i) {
      if (s_ranges [i].name [0] != a_name [0])         continue;
      if (strcmp (s_ranges [i].name, a_name) != 0)     continue;
      n = i;
      break;
   }
   if (n >= 0 && a_deproot != NULL)  *a_deproot = &s_ranges [n].ycalc;
   return n;
}

int
ycalc_range_by_name     (char *a_name)
{
   char        rce         =  -10;
   int         i           =    0;
   int         n           =   -1;
   --rce;  if (a_name == NULL)  return rce;
   for (i = 0; i < s_nrange; ++i) {
      if (s_ranges [i].name [0] != a_name [0])         continue;
      if (strcmp (s_ranges [i].name, a_name) != 0)     continue;
      n = i;
      break;
   }
   return n;
}

int
ycalc_range_nonrange    (tDEP_ROOT *a_deproot)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         c           =    0;
   tDEP_LINK  *x_next      = NULL;
   /*---(defense)------------------------*/
   --rce;  if (a_deproot == NULL)   return rce;
   /*---(count)--------------------------*/
   x_next = a_deproot->pros;
   while (x_next != NULL) {
      if (x_next->type != G_DEP_ENTRY)  ++c;
      x_next = x_next->next;
   }
   /*---(complete)-----------------------*/
   return c;
}

char
ycalc_range_delete      (tDEP_ROOT *a_deproot, tDEP_ROOT *a_range)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tDEP_LINK  *x_next      = NULL;
   int         n           =   -1;
   /*---(prepare)------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   DEBUG_CALC   yLOG_point   ("a_deproot"  , a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_point   ("a_deproot"  , ycalc_call_labeler (a_deproot));
   DEBUG_CALC   yLOG_point   ("a_range"    , a_range);
   --rce;  if (a_range == NULL) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_CALC   yLOG_point   ("a_range"    , ycalc_call_labeler (a_range));
   n = a_range->range;
   DEBUG_CALC   yLOG_value   ("range_no"   , n);
   /*---(check owners)-------------------*/
   DEBUG_CALC   yLOG_value   ("npro"      , a_range->npro);
   --rce;  if (a_range->npro > 1) {
      DEBUG_CALC   yLOG_note    ("range required by other deproots");
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(remove data items)--------------*/
   x_next = a_range->reqs;
   while (x_next != NULL) {
      DEBUG_CALC   yLOG_char    ("link type" , x_next->type);
      DEBUG_DEPS   yLOG_complex ("target"    , ycalc_call_labeler (x_next->target));
      rc = ycalc_deps_delete (x_next->type, &(x_next->source), &(x_next->target));
      DEBUG_CALC   yLOG_value   ("delete"    , rc);
      rc = ycalc_call_reaper (&(x_next->target));
      DEBUG_CALC   yLOG_value   ("reaper"    , rc);
      x_next = x_next->next;
   }
   /*---(delete link)--------------------*/
   rc = ycalc_deps_delete (G_DEP_POINTER, &a_deproot, &a_range);
   rc = ycalc_range_wipe  (n);
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc_range_unhook      (tDEP_ROOT **a_deproot)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tDEP_LINK  *x_next      = NULL;
   /*---(prepare)------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   DEBUG_CALC   yLOG_point   ("a_deproot"  , a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_point   ("*a_deproot" , *a_deproot);
   --rce;  if (*a_deproot == NULL) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_info    ("label"      , ycalc_call_labeler (*a_deproot));
   /*---(check all links)----------------*/
   DEBUG_CALC   yLOG_value   ("npro"      , (*a_deproot)->npro);
   x_next = (*a_deproot)->pros;
   while (x_next != NULL) {
      /*---(filter)----------------------*/
      DEBUG_CALC   yLOG_char    ("link type" , x_next->type);
      /*---(check for range)-------------*/
      if (x_next->type == G_DEP_ENTRY) {
         DEBUG_DEPS   yLOG_complex ("target"    , ycalc_call_labeler (x_next->target));
         rc = ycalc_deps_delete  (G_DEP_RANGE, &(x_next->target), a_deproot);
         DEBUG_CALC   yLOG_value   ("delete"    , rc);
      }
      /*---(go to next)------------------*/
      x_next = x_next->next;
      /*---(done)------------------------*/
   }
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

int
ycalc_range_by_coords   (int bx, int ex, int by, int ey, int bz, int ez)
{
   int         i           =    0;
   int         n           =  -1;
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   DEBUG_DEPS   yLOG_complex ("coords"    , "bx=%4d, ex=%4d, by=%4d, ey=%4d, bz=%4d, ez=%4d", bx, ex, by, ey, bz, ez);
   for (i = 0; i < s_nrange; ++i) {
      DEBUG_DEPS   yLOG_value   ("entry"     , i);
      if (s_ranges [i].ycalc == NULL)     continue;
      DEBUG_DEPS   yLOG_complex ("check"     , "bx=%4d, ex=%4d, by=%4d, ey=%4d, bz=%4d, ez=%4d, ycalc=%p", s_ranges [i].bx, s_ranges [i].ex, s_ranges [i].by, s_ranges [i].ey, s_ranges [i].bz, s_ranges [i].ez, s_ranges [i].ycalc);
      if (s_ranges [i].bz   != bz)        continue;
      if (s_ranges [i].ez   != ez)        continue;
      if (s_ranges [i].bx   != bx)        continue;
      if (s_ranges [i].ex   != ex)        continue;
      if (s_ranges [i].by   != by)        continue;
      if (s_ranges [i].ey   != ey)        continue;
      n = i;
      break;
   }
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return n;
}

int
ycalc_range_add         (int bx, int ex, int by, int ey, int bz, int ez)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         n           =    0;
   char        t           [LEN_LABEL];
   int         x_pos       = 0;             /* iterator -- columns            */
   int         y_pos       = 0;             /* iterator -- rows               */
   int         z_pos       = 0;             /* iterator -- tabs               */
   tDEP_ROOT  *x_range     = NULL;
   tDEP_ROOT  *x_dst       = NULL;
   void       *x_owner     = NULL;
   char        x_type      =  '-';
   /*---(begin)--------------------------*/
   DEBUG_DEPS    yLOG_enter   (__FUNCTION__);
   DEBUG_DEPS    yLOG_complex ("range"     , "bx=%4d, ex=%4d, by=%4d, ey=%4d, bz=%4d, ez=%4d", bx, ex, by, ey, bz, ez);
   /*---(create name)--------------------*/
   n = ycalc_range_next ();
   DEBUG_DEPS    yLOG_value   ("n"         , n);
   if (n < 0) {
      DEBUG_DEPS    yLOG_exitr   (__FUNCTION__, n);
      return n;
   }
   sprintf (t, "�a%d", n + 1);
   strlcpy (s_ranges [n].name, t, LEN_LABEL);
   /*---(assign boundaries---------------*/
   DEBUG_DEPS    yLOG_note    ("assign boundaries");
   s_ranges [n].bx  = bx;
   s_ranges [n].by  = by;
   s_ranges [n].bz  = bz;
   s_ranges [n].ex  = ex;
   s_ranges [n].ey  = ey;
   s_ranges [n].ez  = ez;
   /*---(create the entry)---------------*/
   rc = ycalc_call_who_named (t, YCALC_FULL, &x_owner, &x_range);
   s_ranges [n].ycalc = x_range;
   x_range->range = n;
   /*---(tie all cells)------------------*/
   DEBUG_DEPS    yLOG_note    ("assign entries");
   for (y_pos = by; y_pos <= ey; ++y_pos) {
      for (x_pos = bx; x_pos <= ex; ++x_pos) {
         for (z_pos = bz; z_pos <= ez; ++z_pos) {
            /*---(get existing)-------------*/
            rc = ycalc_call_who_at (x_pos, y_pos, z_pos, YCALC_LOOK, &x_owner, &x_dst);
            DEBUG_DEPS    yLOG_complex ("target"    , "x =%4d, y =%4d, z =%4d, owner=%9p, deproot=%9p", x_pos, y_pos, z_pos, x_owner, x_dst);
            /*---(filter)-------------------*/
            if (x_owner == NULL)                    continue;
            rc = g_valuer  (x_owner, &x_type, NULL, NULL);
            if (rc  <  0)                           continue;
            DEBUG_DEPS   yLOG_char    ("type"      , x_type);
            if (x_type  == YCALC_DATA_BLANK)        continue;
            /*---(create dependency)--------*/
            rc = ycalc_call_who_at (x_pos, y_pos, z_pos, YCALC_FULL, &x_owner, &x_dst);
            DEBUG_DEPS   yLOG_info    ("target"    , ycalc_call_labeler (x_dst));
            rc  = ycalc_deps_create (G_DEP_RANGE, &x_range, &x_dst);
            if (rc  <  0)                           break;
            /*---(done)---------------------*/
         }
      }
   }
   /*---(update)-------------------------*/
   if (n == s_nrange) ++s_nrange;
   /*---(complete)-----------------------*/
   DEBUG_DEPS    yLOG_exit    (__FUNCTION__);
   return n;
}

char
ycalc_range_use         (tDEP_ROOT *a_src, int bx, int ex, int by, int ey, int bz, int ez, tDEP_ROOT **a_range)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         n           =    0;
   tDEP_ROOT  *x_range     = NULL;
   /*---(begin)--------------------------*/
   DEBUG_DEPS    yLOG_enter   (__FUNCTION__);
   DEBUG_DEPS    yLOG_complex ("range"     , "bx=%4d, ex=%4d, by=%4d, ey=%4d, bz=%4d, ez=%4d", bx, ex, by, ey, bz, ez);
   /*---(find/create)--------------------*/
   n = ycalc_range_by_coords (bx, ex, by, ey, bz, ez);
   DEBUG_DEPS    yLOG_value   ("n"         , n);
   if (n < 0) {
      n = ycalc_range_add  (bx, ex, by, ey, bz, ez);
      DEBUG_DEPS    yLOG_value   ("n"         , n);
   }
   --rce;  if (n < 0) {
      DEBUG_DEPS    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(link to source)-----------------*/
   x_range = s_ranges [n].ycalc;
   rc  = ycalc_deps_create (G_DEP_POINTER, &a_src, &x_range);
   DEBUG_DEPS    yLOG_value   ("create"    , rc);
   --rce;  if (rc  <  0) {
      DEBUG_DEPS    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save)---------------------------*/
   if (a_range != NULL)  *a_range = x_range;
   /*---(complete)-----------------------*/
   DEBUG_DEPS    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc_range_include     (tDEP_ROOT *a_src, int x, int y, int z)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   tDEP_ROOT  *x_range     = NULL;
   /*---(begin)--------------------------*/
   DEBUG_DEPS    yLOG_enter   (__FUNCTION__);
   DEBUG_DEPS    yLOG_complex ("source"    , "x =%4d, y =%4d, z =%4d, ptr=%9p", x, y, z, a_src);
   for (i = 0; i < s_nrange; ++i) {
      if (s_ranges [i].ycalc == NULL)     continue;
      DEBUG_DEPS    yLOG_complex ("range"     , "%s, bx=%4d, ex=%4d, by=%4d, ey=%4d, bz=%4d, ez=%4d", s_ranges [i].name, s_ranges [i].bx, s_ranges [i].ex, s_ranges [i].by, s_ranges [i].ey, s_ranges [i].bz, s_ranges [i].ez);
      if (s_ranges [i].bz   >  z)   continue;
      if (s_ranges [i].ez   <  z)   continue;
      if (s_ranges [i].bx   >  x)   continue;
      if (s_ranges [i].ex   <  x)   continue;
      if (s_ranges [i].by   >  y)   continue;
      if (s_ranges [i].ey   <  y)   continue;
      x_range = s_ranges [i].ycalc;
      rc  = ycalc_deps_create (G_DEP_RANGE, &x_range, &a_src);
      if (rc  <  0)  return rc;
   }
   DEBUG_DEPS    yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        range functions                       ----===*/
/*====================------------------------------------====================*/
void  o___RANGE___________o () { return; }


/*
 *  as part of building this spreadsheet, i developed separate functions for
 *  each of the range applications.  this was done to increase speed, but
 *  along the way i had to debug and update causing a fair amount of duplicate
 *  updates to each function and potential for mistakes.
 *
 *  i have now seen how fast the spreadsheet is so i have decided to trade the
 *  speed of separate functions for increased testability and maintainability.
 *  i could either have passed a function pointer to perform the operation or
 *  perform all the calculations and then return the appropriate value.  since
 *  the calculations are simple and fast, i opted for the clarity of wrapper
 *  function.
 *
 */


void    /*-> total the numeric cells in range ---[ ------ [fv.B73.140.92]*/ /*-[03.0000.0D#.!]-*/ /*-[--.---.---.--]-*/
ycalc__range_gather     (char *a_func)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          = 0;
   tDEP_ROOT  *x_deproot   = NULL;
   tDEP_LINK  *x_next      = NULL;
   char        x_type      =  '-';
   double      x_value     =  0.0;
   static tDEP_ROOT *x_saved  = NULL;
   /*---(prepare)------------------------*/
   x_deproot = (tDEP_ROOT *) ycalc_popref (__FUNCTION__);
   /*> if (x_deproot == x_saved)  return;                                             <*/
   /*---(initialize)---------------------*/
   s_count_all  = ycalc_range_size (x_deproot->range);
   s_count_fil  = 0;
   s_count_num  = s_count_str  = 0;
   s_count_calc = 0;
   s_total      = 0.0;
   s_min        =   S_MAX;
   s_max        = -(S_MAX);
   /*---(loop)---------------------------*/
   x_next = x_deproot->reqs;
   while (x_next != NULL) {
      /*---(filter)----------------------*/
      DEBUG_CALC   yLOG_char    ("link type" , x_next->type);
      if (x_next->type != G_DEP_RANGE) {
         x_next = x_next->next;
         continue;
      }
      rc = g_valuer  (x_next->target->owner, &x_type, &x_value, NULL);
      DEBUG_CALC   yLOG_value   ("valuer"    , rc);
      DEBUG_CALC   yLOG_char    ("owner type", x_type);
      if (strchr (YCALC_GROUP_NUM, x_type) != NULL) {
         ++s_count_fil;
         ++s_count_num;
         s_total += x_value;
         if (x_value < s_min)  s_min = x_value;
         if (x_value > s_max)  s_max = x_value;
         s_entries [s_count_num - 1] = x_value;
         DEBUG_CALC   yLOG_value   ("numeric"   , s_count_num);
         DEBUG_CALC   yLOG_value   ("entry"     , s_entries [s_count_num - 1]);
      } else if (strchr (YCALC_GROUP_STR, x_type) != NULL) {
         ++s_count_str;
         ++s_count_fil;
         DEBUG_CALC   yLOG_value   ("string"    , s_count_str);
      } else if (strchr (YCALC_GROUP_POINT, x_type) != NULL) {
         ++s_count_fil;
         ++s_count_ptr;
         DEBUG_CALC   yLOG_value   ("pointer"   , s_count_ptr);
      }
      if (strchr (YCALC_GROUP_CALC, x_type) != NULL) {
         ++s_count_calc;
         DEBUG_CALC   yLOG_value   ("calc"      , s_count_calc);
      }
      /*---(next)------------------------*/
      x_next = x_next->next;
   }
   if (s_min ==   S_MAX )  s_min = 0.0;
   if (s_max == -(S_MAX))  s_max = 0.0;
   x_saved = x_deproot;
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_sum         (void)
{
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_total);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_every       (void)
{
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_count_all);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_filled      (void)
{
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_count_fil);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_numbers     (void)
{
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_count_num);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_strings     (void)
{
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_count_str);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_pointers    (void)
{
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_count_ptr);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_empty       (void)
{
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_count_all - s_count_fil);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_calcs       (void)
{
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_count_calc);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_dist        (void)
{
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, sqrt (pow (s_z, 2) + pow (s_x, 2) + pow (s_y, 2)));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_tabs        (void)
{
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_z);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_cols        (void)
{
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_x);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rows        (void)
{
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_y);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_min         (void)
{
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_min);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_max         (void)
{
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_max);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_range       (void)
{
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_max - s_min);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_average     (void)
{
   ycalc__range_gather (__FUNCTION__);
   if (s_count_num > 0)  ycalc_pushval (__FUNCTION__, s_total / s_count_num);
   else                  ycalc_pushval (__FUNCTION__, 0);
   return;
}

static double    q1, q2, q3, qr, qs;

void    /*-> find max of all numeric cells ------[ leaf   [fv.A52.040.70]*/ /*-[03.0000.02#.!]-*/ /*-[--.---.---.--]-*/
ycalc__range_sort    (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =  0;            /* loop iterator -- entry         */
   int         tele        = -1;            /* teleport point to speed sort   */
   int         mid         =  0;            /* midpoint of entries            */
   int         runs        =  0;
   /*---(prepare)------------------------*/
   DEBUG_CALC   yLOG_note    ("before sorting");
   for (i = 0; i < s_count_num; ++i)  DEBUG_CALC   yLOG_complex ("entry"     , "%3d = %10.2lf", i, s_entries [i]);
   i = 1;
   /*---(sort)---------------------------*/
   DEBUG_CALC   yLOG_note    ("sorting");
   while (i < s_count_num) {
      ++runs;
      if (runs > 500)  break;
      /*---(load vars)-------------------*/
      a = s_entries [i - 1];
      b = s_entries [i];
      DEBUG_CALC   yLOG_complex ("current"   , "compare i,%3d (t,%3d) =  a,%10.2lf to b,%10.2lf", i, tele, a, b);
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
      s_entries [i - 1] = b;
      s_entries [i]     = a;
      a = s_entries [i - 1];
      b = s_entries [i];
      DEBUG_CALC   yLOG_complex ("swapped"   , "now     i,%3d (t,%3d) =  a,%10.2lf to b,%10.2lf", i, tele, a, b);
      /*---(update)----------------------*/
      if (tele < 0) tele = i;
      if (i > 1) --i;
   }
   /*---(wrapup)-------------------------*/
   DEBUG_CALC   yLOG_note    ("after sorting");
   for (i = 0; i < s_count_num; ++i)  DEBUG_CALC   yLOG_complex ("entry"     , "%3d = %10.2lf", i, s_entries [i]);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> find the quartile values -----------[ ------ [fv.JC3.090.43]*/ /*-[01.0000.04#.!]-*/ /*-[--.---.---.--]-*/
ycalc__range_quarters     (void)
{
   /*---(locals)-----------+-----------+-*/
   int         x_qtr       =  0;
   int         x_1st1      =  0;            /* first quartile point           */
   int         x_1st2      =  0;            /* first quartile point           */
   double      x_1stw      =  0;            /* first quartile point           */
   int         x_mid1      =  0;            /* midpoint of entries            */
   int         x_mid2      =  0;            /* midpoint of entries            */
   int         x_3rd1      =  0;            /* third quartile point           */
   int         x_3rd2      =  0;            /* third quartile point           */
   double      x_3rdw      =  0;            /* third quartile point           */
   /*---(start)--------------------------*/
   ycalc__range_gather (__FUNCTION__);
   DEBUG_CALC   yLOG_value   ("count"     , s_count_num);
   if      (s_count_num == 0)  {
      DEBUG_CALC   yLOG_value   ("pushing"   , 0);
      ycalc_pushval (__FUNCTION__,  0);
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   ycalc__range_sort ();
   /*---(figure out median)--------------*/
   switch (s_count_num % 2) {
   case 0 :  /* even means average needed   */
      x_mid1 = (s_count_num / 2.0) - 1;
      x_mid2 = x_mid1 + 1;
      break;
   case 1 :  /* odd means an exact middle   */
      x_mid1 = (s_count_num / 2);
      x_mid2 = x_mid1;
      break;
   }
   q2 = (s_entries [x_mid1] + s_entries [x_mid2]) / 2.0;
   /*---(figure out quarters)------------*/
   x_qtr = s_count_num / 4;
   switch (s_count_num % 4) {
   case 0 :
      DEBUG_CALC   yLOG_note    ("zero");
      x_1st1 = x_qtr        - 1;
      x_1st2 = x_1st1 + 1;
      x_1stw = 2.0;
      x_3rd1 = x_qtr  * 3.0 - 1;
      x_3rd2 = x_3rd1 + 1;
      x_3rdw = 2.0;
      break;
   case 1 :
      DEBUG_CALC   yLOG_note    ("one");
      x_1st1 = x_qtr        - 1;
      x_1st2 = x_1st1 + 1;
      x_1stw = 1.0;
      x_3rd1 = x_qtr  * 3.0;
      x_3rd2 = x_3rd1 + 1;
      x_3rdw = 3.0;
      break;
   case 2 :  /* even in each half, so quartile is exact element  */ 
      DEBUG_CALC   yLOG_note    ("two");
      x_1st1 = x_qtr;
      x_1st2 = x_1st1;
      x_1stw = 2.0;
      x_3rd1 = x_qtr  * 3.0 + 1;
      x_3rd2 = x_3rd1;
      x_3rdw = 2.0;
      break;
   case 3 :
      DEBUG_CALC   yLOG_note    ("three");
      x_1st1 = x_qtr;
      x_1st2 = x_1st1 + 1;
      x_1stw = 3.0;
      x_3rd1 = x_qtr  * 3.0 + 1;
      x_3rd2 = x_3rd1 + 1;
      x_3rdw = 1.0;
      break;
   }
   DEBUG_CALC   yLOG_complex ("x_1st1"    , "i=%3d, v=%10.5lf, w=%4.2lf, f=%10.5lf", x_1st1, s_entries [x_1st1], x_1stw      , s_entries [x_1st1] * x_1stw);
   DEBUG_CALC   yLOG_complex ("x_1st2"    , "i=%3d, v=%10.5lf, w=%4.2lf, f=%10.5lf", x_1st2, s_entries [x_1st2], 4.0 - x_1stw, s_entries [x_1st2] * (4.0 - x_1stw));
   q1 = ((s_entries [x_1st1] * x_1stw) + (s_entries [x_1st2] * (4.0 - x_1stw))) / 4.0;
   /*> printf ("q1 = %lf\n", q1);                                                     <*/
   DEBUG_CALC   yLOG_complex ("x_1st"     , "%10.5lf", q1);
   DEBUG_CALC   yLOG_complex ("x_3rd1"    , "i=%3d, v=%10.5lf, w=%4.2lf, f=%10.5lf", x_3rd1, s_entries [x_3rd1], x_3rdw      , s_entries [x_3rd1] * x_3rdw);
   DEBUG_CALC   yLOG_complex ("x_3rd2"    , "i=%3d, v=%10.5lf, w=%4.2lf, f=%10.5lf", x_3rd2, s_entries [x_3rd2], 4.0 - x_3rdw, s_entries [x_3rd2] * (4.0 - x_3rdw));
   q3 = ((s_entries [x_3rd1] * x_3rdw) + (s_entries [x_3rd2] * (4.0 - x_3rdw))) / 4.0;
   /*> printf ("q3 = %lf\n", q3);                                                     <*/
   DEBUG_CALC   yLOG_complex ("x_3rd"     , "%10.5lf", q3);
   qr = q3 - q1;
   qs = (q3 + q1 - (2.0 * q2)) / (q3 - q1);
   /*> printf ("qr = %lf\n", qr);                                                     <*/
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_quarter1    (void)
{
   ycalc__range_quarters ();
   ycalc_pushval (__FUNCTION__, q1);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_median      (void)
{
   ycalc__range_quarters ();
   ycalc_pushval (__FUNCTION__, q2);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_quarter3    (void)
{
   ycalc__range_quarters ();
   ycalc_pushval (__FUNCTION__, q3);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rangeq      (void)
{
   ycalc__range_quarters ();
   ycalc_pushval (__FUNCTION__, qr);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_skew        (void)
{
   ycalc__range_quarters ();
   ycalc_pushval (__FUNCTION__, qs);
   return;
}

void    /*-> find the most common value ---------[ ------ [fv.952.070.43]*/ /*-[02.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_mode         (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =  0;            /* loop iterator -- entry         */
   int         mid         =  0;            /* midpoint of entries            */
   int         x_mode      =  0;
   int         x_highest   =  0;
   int         x_multi     =  0;
   int         x_count     =  0;
   int         x_curr      =  0;
   int         x_save      =  0;
   /*---(start)--------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   ycalc__range_quarters ();
   DEBUG_CALC   yLOG_value   ("count_num" , s_count_num);
   if      (s_count_num == 0)  {
      DEBUG_CALC   yLOG_value   ("pushing"   , 0);
      ycalc_pushval (__FUNCTION__,  0);
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   x_save = x_curr;
   for (i = 0; i < s_count_num; ++i) {
      x_curr = (int) s_entries [i];
      DEBUG_CALC   yLOG_complex ("entry"     , "i=%3d, x_curr=%3d, x_save=%3d", i, x_curr, x_save);
      if (x_curr != x_save)  x_count = 0;
      ++x_count;
      DEBUG_CALC   yLOG_value   ("count"     , x_count);
      if (x_count == x_highest) {
         ++x_multi;
         DEBUG_CALC   yLOG_complex ("multi"     , "i=%3d, mode=%3d, highest=%3d", i, x_mode, x_highest);
      }
      if (x_count > x_highest) {
         x_mode    = x_curr;
         x_highest = x_count;
         x_multi   = 0;
         DEBUG_CALC   yLOG_complex ("new_mode"  , "i=%3d, mode=%3d, highest=%3d", i, x_mode, x_highest);
      }
      x_save = x_curr;
   }
   if (x_multi >= 1)  ycalc_pushval (__FUNCTION__, (double) q2);
   else               ycalc_pushval (__FUNCTION__, (double) x_mode);
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return;
}

void    /*-> find the measure of variance -------[ ------ [fv.A52.070.22]*/ /*-[01.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_stddev       (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =  0;            /* loop iterator -- entry         */
   double      x_mean      =  0;
   double      x_curr      =  0;
   double      x_diff      =  0;
   double      x_var       =  0;
   double      x_total     =  0;
   double      x_stddev    =  0;
   /*---(start)--------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   DEBUG_CALC   yLOG_value   ("count_num" , s_count_num);
   /*---(defense: too few values)--------*/
   if      (s_count_num <= 1)  {
      DEBUG_CALC   yLOG_value   ("pushing"   , 0);
      ycalc_pushval (__FUNCTION__,  0);
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   /*---(calc mean)----------------------*/
   x_mean = s_total / ((double) s_count_num);
   /*---(gather variances)---------------*/
   for (i = 0; i < s_count_num; ++i) {
      x_curr   = s_entries [i];
      x_diff   = x_curr - x_mean;
      x_var    = x_diff * x_diff;
      x_total += x_var;
      DEBUG_CALC   yLOG_complex ("entry"     , "i=%3d, x_curr=%5.2lf, x_diff=%5.2lf, x_var=%10.2lf, x_total=%10.2lf", i, x_curr, x_diff, x_var, x_total);
   }
   /*---(final calc)---------------------*/
   DEBUG_CALC   yLOG_llong   ("variance"  , x_total / ((double) s_count_num));
   x_stddev   = sqrt(x_total / ((double) s_count_num));
   DEBUG_CALC   yLOG_llong   ("stddev"    , x_stddev);
   ycalc_pushval (__FUNCTION__, x_stddev);
   /*---(complete)-----------------------*/
   x_mean = s_total / ((double) s_count_num);
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return;
}


