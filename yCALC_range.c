/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"


#define     MAX_RANGE       200
typedef   struct  cyCALC_RANGE  tyCALC_RANGE;
struct cyCALC_RANGE {
   char        name        [LEN_LABEL];
   int         z;
   int         bx, by;
   int         ex, ey;
   int         count;
   tDEP_ROOT   ycalc;
} s_ranges [MAX_RANGE];
int     s_nrange          = 0;



static int     s_begx     = 0;
static int     s_begy     = 0;
static int     s_begz     = 0;

static int     s_endx     = 0;
static int     s_endy     = 0;
static int     s_endz     = 0;



/*====================------------------------------------====================*/
/*===----                        range functions                       ----===*/
/*====================------------------------------------====================*/
void  o___RANGES__________o () { return; }

int
ycalc_range_init        (void)
{
   int         i           =    0;
   for (i = 0; i < s_nrange; ++i) {
      strlcpy (s_ranges [i].name, "", LEN_LABEL);
      s_ranges [i].bx = s_ranges [i].ex =  -10;
      s_ranges [i].by = s_ranges [i].ey =  -10;
      s_ranges [i].z                    =  -10;
      s_ranges [i].count                =    0;
   }
   s_nrange = 0;
}

int
ycalc_range_label       (int n)
{
   return s_ranges [n].name;
}

char
ycalc_range_deproot     (char *a_name, tDEP_ROOT **a_deproot)
{
   int         i           =    0;
   int         n           =  -1;
   if (a_deproot != NULL)  *a_deproot = NULL;
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
   int         i           =    0;
   int         n           =  -1;
   for (i = 0; i < s_nrange; ++i) {
      if (s_ranges [i].name [0] != a_name [0])         continue;
      if (strcmp (s_ranges [i].name, a_name) != 0)     continue;
      n = i;
      break;
   }
   return n;
}

int
ycalc_range_by_coords   (int bx, int ex, int by, int ey, int z)
{
   int         i           =    0;
   int         n           =  -1;
   DEBUG_DEPS    yLOG_complex ("range"     , "bx=%4d, ex=%4d, by=%4d, ey=%4d, z=%4d", bx, by, ex, ey, z);
   for (i = 0; i < s_nrange; ++i) {
      if (s_ranges [i].z    != z)         continue;
      if (s_ranges [i].bx   != bx)        continue;
      if (s_ranges [i].ex   != ex)        continue;
      if (s_ranges [i].by   != by)        continue;
      if (s_ranges [i].ey   != ey)        continue;
      n = i;
      break;
   }
   return n;
}

int
ycalc_range_add         (int bx, int ex, int by, int ey, int z)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        t           [LEN_LABEL];
   int         x_pos       = 0;             /* iterator -- columns            */
   int         y_pos       = 0;             /* iterator -- rows               */
   tDEP_ROOT  *x_range     = NULL;
   tDEP_ROOT  *x_dst       = NULL;
   /*---(begin)--------------------------*/
   DEBUG_DEPS    yLOG_enter   (__FUNCTION__);
   DEBUG_DEPS    yLOG_complex ("range"     , "bx=%4d, ex=%4d, by=%4d, ey=%4d, z=%4d", bx, by, ex, ey, z);
   /*---(create name)--------------------*/
   sprintf (t, "­a%d", s_nrange + 1);
   strlcpy (s_ranges [s_nrange].name, t, LEN_LABEL);
   /*---(assign beg)---------------------*/
   s_ranges [s_nrange].bx  = bx;
   s_ranges [s_nrange].by  = by;
   /*---(assign end)---------------------*/
   s_ranges [s_nrange].ex  = ex;
   s_ranges [s_nrange].ey  = ey;
   /*---(assign z)-----------------------*/
   s_ranges [s_nrange].z   = z;
   /*---(tie all cells)------------------*/
   x_range = &s_ranges [s_nrange].ycalc;
   for (y_pos = by; y_pos <= ey; ++y_pos) {
      for (x_pos = bx; x_pos <= ex; ++x_pos) {
         /*---(get existing)-------------*/
         g_who_at (x_pos, y_pos, z, NULL, &x_dst);
         DEBUG_DEPS    yLOG_complex ("target"    , "x=%4d, y=%4d, z=%4d, ptr=%9p", x_pos, y_pos, z, x_dst);
         if (x_dst    == NULL)   continue;
         /*---(create dependency)--------*/
         DEBUG_DEPS   yLOG_info    ("target"    , ycalc_call_labeler (x_dst));
         rc  = ycalc_deps_create (G_DEP_RANGE, x_range, x_dst);
         if (rc  <  0)  break;
      }
   }
   /*---(update)-------------------------*/
   ++s_nrange;
   /*---(complete)-----------------------*/
   DEBUG_DEPS    yLOG_exit    (__FUNCTION__);
   return s_nrange - 1;
}

char
ycalc_range_use         (tDEP_ROOT *a_src, int bx, int ex, int by, int ey, int z)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         n           =    0;
   tDEP_ROOT  *x_range     = NULL;
   /*---(begin)--------------------------*/
   DEBUG_DEPS    yLOG_enter   (__FUNCTION__);
   DEBUG_DEPS    yLOG_complex ("range"     , "bx=%4d, ex=%4d, by=%4d, ey=%4d, z=%4d", bx, by, ex, ey, z);
   /*---(find/create)--------------------*/
   n = ycalc_range_by_coords (bx, ex, by, ey, z);
   DEBUG_DEPS    yLOG_value   ("n"         , n);
   if (n < 0) {
      n = ycalc_range_add  (bx, ex, by, ey, z);
      DEBUG_DEPS    yLOG_value   ("n"         , n);
   }
   --rce;  if (n < 0) {
      DEBUG_DEPS    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(link to source)-----------------*/
   x_range = &s_ranges [n].ycalc;
   rc  = ycalc_deps_create (G_DEP_POINTER, a_src, x_range);
   DEBUG_DEPS    yLOG_value   ("create"    , rc);
   --rce;  if (rc  <  0) {
      DEBUG_DEPS    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
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
   DEBUG_DEPS    yLOG_complex ("source"    , "x=%4d, y=%4d, z=%4d, ptr=%9p", x, y, z, a_src);
   for (i = 0; i < s_nrange; ++i) {
      DEBUG_DEPS    yLOG_complex ("range"     , "bx=%4d, ex=%4d, by=%4d, ey=%4d, z=%4d", s_ranges [i].bx, s_ranges [i].by, s_ranges [i].ex, s_ranges [i].ey, s_ranges [i].z);
      if (s_ranges [i].z    != z)   continue;
      if (s_ranges [i].bx   >  x)   continue;
      if (s_ranges [i].ex   <  x)   continue;
      if (s_ranges [i].by   >  y)   continue;
      if (s_ranges [i].ey   <  y)   continue;
      x_range = &s_ranges [i].ycalc;
      rc  = ycalc_deps_create (G_DEP_RANGE, x_range, a_src);
      if (rc  <  0)  return rc;
   }
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        range functions                       ----===*/
/*====================------------------------------------====================*/
void  o___RANGE___________o () { return; }


static char  /*-> tbd --------------------------------[ leaf   [fe.843.145.40]*/ /*-[01.0000.00#.!]-*/ /*-[--.---.---.--]-*/
SHARED__rangeparse (char *a_func)
{
   /*---(locals)-----------+-----------+-*/
   void       *x_beg       = NULL;
   void       *x_end       = NULL;
   char        rce         = -10;
   char        rc          = 0;
   /*---(get range addresses)------------*/
   x_end = ycalc_popref (a_func);
   DEBUG_CALC   yLOG_point   ("x_end"     , x_end);
   --rce;  if (x_end == NULL) {
      return YCALC_ERROR_EXEC_BRNG;
   }
   x_beg = ycalc_popref (a_func);
   DEBUG_CALC   yLOG_point   ("x_beg"     , x_beg);
   --rce;  if (x_beg == NULL)   return rce;
   /*---(parse beginning)----------------*/
   rc = g_addresser (x_beg, &s_begx, &s_begy, &s_begz);
   DEBUG_CALC   yLOG_value   ("s_begx"    , s_begx);
   DEBUG_CALC   yLOG_value   ("s_begy"    , s_begy);
   DEBUG_CALC   yLOG_value   ("s_begz"    , s_begz);
   DEBUG_CALC   yLOG_value   ("rc"        , rc);
   --rce;  if (rc <  0 ) {
      return YCALC_ERROR_EXEC_BRNG;
   }
   /*---(parse ending)-------------------*/
   rc = g_addresser (x_end, &s_endx, &s_endy, &s_endz);
   DEBUG_CALC   yLOG_value   ("s_endx"    , s_endx);
   DEBUG_CALC   yLOG_value   ("s_endy"    , s_endy);
   DEBUG_CALC   yLOG_value   ("s_endz"    , s_endz);
   DEBUG_CALC   yLOG_value   ("rc"        , rc);
   --rce;  if (rc < 0) {
      return YCALC_ERROR_EXEC_BRNG;
   }
   /*---(complete)-----------------------*/
   return 0;
}

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





void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_sum         (void)
{
   a = ycalc_popref (__FUNCTION__);
   b = ycalc_popref (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, 0);
   return;
}


