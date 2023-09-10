/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"


#define     MAX_RANGE       500
typedef   struct  cyCALC_RANGE  tyCALC_RANGE;
struct cyCALC_RANGE {
   char        name        [LEN_LABEL];
   int         b;
   int         bb, bx, by, bz;
   int         eb, ex, ey, ez;
   tDEP_ROOT  *ycalc;
} s_ranges [MAX_RANGE];
int     s_nrange          = 0;



static int     s_begb     = 0;
static int     s_begx     = 0;
static int     s_begy     = 0;
static int     s_begz     = 0;

static int     s_endb     = 0;
static int     s_endx     = 0;
static int     s_endy     = 0;
static int     s_endz     = 0;


/*---(overall)--------------*/
static int     s_every    = 0;
static int     s_used     = 0;
/*---(numeric)--------------*/
static int     s_numbers  = 0;
static int     s_nlit     = 0;
static int     s_nform    = 0;
static int     s_nmath    = 0;
static int     s_nlike    = 0;
/*---(alpha)----------------*/
static int     s_strings  = 0;
static int     s_slit     = 0;
static int     s_sform    = 0;
static int     s_slike    = 0;
/*---(pointers)-------------*/
static int     s_pointers = 0;
static int     s_nref     = 0;
static int     s_ncref    = 0;
static int     s_rlike    = 0;
static int     s_nrref    = 0;
/*---(other)----------------*/
static int     s_vars     = 0;
static int     s_merges   = 0;
static int     s_blanks   = 0;
static int     s_errors   = 0;
static int     s_unknown  = 0;
/*---(done)-----------------*/


static double  s_total      =  0.0;
static double  s_min        =  0.0;
static double  s_max        =  0.0;
static double  s_entries    [1000];
static int     s_b          =    0;
static int     s_x          =    0;
static int     s_y          =    0;
static int     s_z          =    0;

#define        S_MAX        1000000000


/*====================------------------------------------====================*/
/*===----                        range functions                       ----===*/
/*====================------------------------------------====================*/
static void  o___RANGES__________o () { return; }

char
ycalc_range_wipe        (int n)
{
   DEBUG_YCALC   yLOG_senter  (__FUNCTION__);
   DEBUG_YCALC   yLOG_sint    (n);
   ystrlcpy (s_ranges [n].name, "", LEN_LABEL);
   s_ranges [n].ycalc                = NULL;
   s_ranges [n].bb = s_ranges [n].eb =  -10;
   s_ranges [n].bx = s_ranges [n].ex =  -10;
   s_ranges [n].by = s_ranges [n].ey =  -10;
   s_ranges [n].bz = s_ranges [n].ez =  -10;
   DEBUG_YCALC   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ycalc_range_init        (void)
{
   int         i           =    0;
   for (i = 0; i < MAX_RANGE; ++i) {
      ycalc_range_wipe (i);
   }
   s_nrange = 0;
   return 0;
}

char
ycalc_range_wrap        (void)
{
   char        rc          =    0;
   int         i           =    0;
   int         c           =    0;
   char        t           [LEN_LABEL];
   void       *x_owner     = NULL;
   tDEP_ROOT  *x_range     = NULL;
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   for (i = 0; i < MAX_RANGE; ++i) {
      sprintf (t, "®a%d", i + 1);
      rc = ycalc_call_who_named (t, YCALC_LOOK, &x_owner, &x_range);
      if (rc >= 0 && x_owner != NULL) {
         ++c;
         DEBUG_YCALC   yLOG_info    ("found"     , t);
         yCALC_disable (&x_owner, &x_range);
      }
      ycalc_range_wipe (i);
   }
   s_nrange = 0;
   DEBUG_YCALC   yLOG_value   ("wiped"     , c);
   /*> printf ("had to wipe %d ranges\n", c);                                         <*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
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
   int         b, x, y, z, t;
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   s_b = b = s_ranges [n].eb - s_ranges [n].bb + 1;
   DEBUG_YCALC   yLOG_complex ("univers"   , "%4d, %4db, %4de", b, s_ranges [n].bb, s_ranges [n].eb);
   s_x = x = s_ranges [n].ex - s_ranges [n].bx + 1;
   DEBUG_YCALC   yLOG_complex ("x"         , "%4d, %4db, %4de", x, s_ranges [n].bx, s_ranges [n].ex);
   s_y = y = s_ranges [n].ey - s_ranges [n].by + 1;
   DEBUG_YCALC   yLOG_complex ("y"         , "%4d, %4db, %4de", y, s_ranges [n].by, s_ranges [n].ey);
   s_z = z = s_ranges [n].ez - s_ranges [n].bz + 1;
   DEBUG_YCALC   yLOG_complex ("z"         , "%4d, %4db, %4de", z, s_ranges [n].bz, s_ranges [n].ez);
   t = b * x * y * z;
   DEBUG_YCALC   yLOG_value   ("t"         , t);
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return t;
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
   tDEP_LINK  *x_save      = NULL;
   int         n           =   -1;
   void       *x_owner     = NULL;
   tDEP_ROOT  *x_deproot   = NULL;
   char      **x_source    = NULL;
   char       *x_type      = NULL;
   tDEP_ROOT  *x_src       = NULL;
   tDEP_ROOT  *x_trg       = NULL;
   /*---(prepare)------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   DEBUG_YCALC   yLOG_point   ("a_deproot"  , a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("a_deproot"  , ycalc_call_labeler (a_deproot));
   DEBUG_YCALC   yLOG_point   ("a_range"    , a_range);
   --rce;  if (a_range == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("a_range"    , ycalc_call_labeler (a_range));
   n = a_range->range;
   DEBUG_YCALC   yLOG_value   ("range_no"   , n);
   /*---(check owners)-------------------*/
   DEBUG_YCALC   yLOG_value   ("npro"      , a_range->npro);
   --rce;  if (a_range->npro > 1) {
      DEBUG_YCALC   yLOG_note    ("range required by other deproots");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(clear label)--------------------*/
   rc = myCALC.e_pointer (a_range->owner, &x_source, &x_type, NULL, NULL);
   *x_type = YCALC_DATA_BLANK;
   if (*x_source != NULL)  free (*x_source);
   *x_source = NULL;
   /*---(remove data items)--------------*/
   x_next = a_range->reqs;
   while (x_next != NULL) {
      x_save    = x_next->next;
      x_deproot = x_next->target;
      x_owner   = x_next->target->owner;
      DEBUG_YCALC   yLOG_value   ("nreq"      , a_range->nreq);
      DEBUG_YCALC   yLOG_char    ("link type" , x_next->type);
      x_src        = x_next->source;
      x_trg        = x_next->target;
      DEBUG_YCALC   yLOG_complex ("range targ"    , ycalc_call_labeler (x_trg));
      rc = ycalc_deps_delete (x_next->type, &(x_src), &(x_trg), &(x_trg->owner));
      DEBUG_YCALC   yLOG_value   ("delete"    , rc);
      x_next = x_save;
   }
   DEBUG_YCALC   yLOG_point   ("a_range"   , a_range);
   DEBUG_YCALC   yLOG_value   ("nreq"      , a_range->nreq);
   DEBUG_YCALC   yLOG_point   ("owner"     , a_range->owner);
   /*---(delete link)--------------------*/
   x_owner = a_range->owner;
   rc = ycalc_deps_delete (G_DEP_POINTER, &a_deproot, &a_range, &x_owner);
   DEBUG_YCALC   yLOG_point   ("a_range"   , a_range);
   DEBUG_YCALC   yLOG_point   ("x_owner"   , x_owner);
   rc = ycalc_range_wipe  (n);
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> unhook an entry from all ranges ----[ ------ [ge.A34.142.41]*/ /*-[02.0000.515.5]-*/ /*-[--.---.---.--]-*/
ycalc_range_unhook      (void **a_owner, tDEP_ROOT **a_deproot)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tDEP_LINK  *x_next      = NULL;
   tDEP_LINK  *x_save      = NULL;
   /*---(prepare)------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   DEBUG_YCALC   yLOG_point   ("a_owner"    , a_owner);
   if (a_owner != NULL)  DEBUG_YCALC   yLOG_point   ("*a_owner"   , *a_owner);
   DEBUG_YCALC   yLOG_point   ("a_deproot"  , a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("*a_deproot" , *a_deproot);
   --rce;  if (*a_deproot == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_info    ("label"      , ycalc_call_labeler (*a_deproot));
   /*---(check all links)----------------*/
   DEBUG_YCALC   yLOG_value   ("npro"      , (*a_deproot)->npro);
   x_next = (*a_deproot)->pros;
   while (x_next != NULL) {
      x_save = x_next->next;
      /*---(filter)----------------------*/
      DEBUG_YCALC   yLOG_char    ("link type" , x_next->type);
      /*---(check for range)-------------*/
      if (x_next->type == G_DEP_ENTRY) {
         DEBUG_YCALC   yLOG_complex ("target"    , ycalc_call_labeler (x_next->target));
         rc = ycalc_deps_delete  (G_DEP_RANGE, &(x_next->target), a_deproot, a_owner);
         DEBUG_YCALC   yLOG_value   ("delete"    , rc);
      }
      /*---(go to next)------------------*/
      x_next = x_save;
      /*---(done)------------------------*/
   }
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

int
ycalc_range_by_coords   (int bb, int eb, int bx, int ex, int by, int ey, int bz, int ez)
{
   int         i           =    0;
   int         n           =  -1;
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   DEBUG_YCALC   yLOG_complex ("coords"    , "bb=%4d, eb=%4d, bx=%4d, ex=%4d, by=%4d, ey=%4d, bz=%4d, ez=%4d", bb, eb, bx, ex, by, ey, bz, ez);
   for (i = 0; i < s_nrange; ++i) {
      DEBUG_YCALC   yLOG_value   ("entry"     , i);
      if (s_ranges [i].ycalc == NULL)     continue;
      DEBUG_YCALC   yLOG_complex ("check"     , "bb=%4d, eb=%4d, bx=%4d, ex=%4d, by=%4d, ey=%4d, bz=%4d, ez=%4d, ycalc=%p", s_ranges [i].bb, s_ranges [i].eb, s_ranges [i].bx, s_ranges [i].ex, s_ranges [i].by, s_ranges [i].ey, s_ranges [i].bz, s_ranges [i].ez, s_ranges [i].ycalc);
      if (s_ranges [i].bb   != bb)        continue;
      if (s_ranges [i].eb   != eb)        continue;
      if (s_ranges [i].bz   != bz)        continue;
      if (s_ranges [i].ez   != ez)        continue;
      if (s_ranges [i].bx   != bx)        continue;
      if (s_ranges [i].ex   != ex)        continue;
      if (s_ranges [i].by   != by)        continue;
      if (s_ranges [i].ey   != ey)        continue;
      n = i;
      break;
   }
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return n;
}

int
ycalc_range_add         (int bb, int eb, int bx, int ex, int by, int ey, int bz, int ez)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         n           =    0;
   char        t           [LEN_DESC ];
   int         b_pos       = 0;             /* iterator -- tabs               */
   int         x_pos       = 0;             /* iterator -- columns            */
   int         y_pos       = 0;             /* iterator -- rows               */
   int         z_pos       = 0;             /* iterator -- depth              */
   tDEP_ROOT  *x_range     = NULL;
   tDEP_ROOT  *x_dst       = NULL;
   void       *x_owner     = NULL;
   char       *x_type      = NULL;
   char      **x_source    = NULL;
   char        x_beg       [LEN_LABEL];
   char        x_end       [LEN_LABEL];
   /*---(begin)--------------------------*/
   DEBUG_YCALC    yLOG_enter   (__FUNCTION__);
   DEBUG_YCALC    yLOG_complex ("range"     , "bb=%4d, eb=%4d, bx=%4d, ex=%4d, by=%4d, ey=%4d, bz=%4d, ez=%4d", bb, eb, bx, ex, by, ey, bz, ez);
   /*---(create name)--------------------*/
   n = ycalc_range_next ();
   DEBUG_YCALC    yLOG_value   ("n"         , n);
   if (n < 0) {
      DEBUG_YCALC    yLOG_exitr   (__FUNCTION__, n);
      return n;
   }
   sprintf (t, "®a%d", n + 1);
   ystrlcpy (s_ranges [n].name, t, LEN_LABEL);
   DEBUG_YCALC    yLOG_info    ("range"     , t);
   /*---(assign boundaries---------------*/
   DEBUG_YCALC    yLOG_note    ("assign boundaries");
   s_ranges [n].bb  = bb;
   s_ranges [n].eb  = eb;
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
   rc = myCALC.e_pointer (x_owner, &x_source, &x_type, NULL, NULL);
   *x_type = YCALC_DATA_INTERN;
   DEBUG_YCALC   yLOG_char    ("*x_type"   , *x_type);
   x_range->btype = YCALC_DATA_INTERN;
   DEBUG_YCALC   yLOG_char    ("btype"     , x_range->btype);
   ystr4gyges (bb, bx, by, bz, NULL, x_beg, YSTR_CHECK);
   ystr4gyges (eb, ex, ey, ez, NULL, x_end, YSTR_CHECK);
   sprintf (t, "®%s..%s", x_beg, x_end);
   if (*x_source != NULL)  free (*x_source);
   *x_source = strdup (t);
   rc = myCALC.e_printer (x_owner);
   DEBUG_YCALC   yLOG_value   ("printer"   , rc);
   /*---(tie all cells)------------------*/
   DEBUG_YCALC    yLOG_note    ("assign entries");
   for (z_pos = bz; z_pos <= ez; ++z_pos) {
      for (y_pos = by; y_pos <= ey; ++y_pos) {
         for (x_pos = bx; x_pos <= ex; ++x_pos) {
            for (b_pos = bb; b_pos <= eb; ++b_pos) {
               /*---(get existing)-------------*/
               rc = ycalc_call_who_at (b_pos, x_pos, y_pos, z_pos, YCALC_LOOK, &x_owner, &x_dst);
               DEBUG_YCALC    yLOG_complex ("target"    , "b =%4d, x =%4d, y =%4d, z =%4d, owner=%9p, deproot=%9p", b_pos, x_pos, y_pos, z_pos, x_owner, x_dst);
               /*---(filter)-------------------*/
               if (x_owner == NULL)                    continue;
               rc = myCALC.e_valuer  (x_owner, &x_type, NULL, NULL);
               if (rc  <  0)                           continue;
               DEBUG_YCALC   yLOG_char    ("*type"     , *x_type);
               if (*x_type  == YCALC_DATA_BLANK)        continue;
               /*---(create dependency)--------*/
               rc = ycalc_call_who_at (b_pos, x_pos, y_pos, z_pos, YCALC_FULL, &x_owner, &x_dst);
               DEBUG_YCALC   yLOG_info    ("assign"    , ycalc_call_labeler (x_dst));
               rc  = ycalc_deps_create (G_DEP_RANGE, &x_range, &x_dst);
               if (rc  <  0)                           break;
               /*---(done)---------------------*/
            }
         }
      }
   }
   /*---(update)-------------------------*/
   if (n == s_nrange) ++s_nrange;
   /*---(complete)-----------------------*/
   DEBUG_YCALC    yLOG_exit    (__FUNCTION__);
   return n;
}

char
ycalc_range_use         (tDEP_ROOT *a_src, int bb, int eb, int bx, int ex, int by, int ey, int bz, int ez, tDEP_ROOT **a_range)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         n           =    0;
   tDEP_ROOT  *x_range     = NULL;
   /*---(begin)--------------------------*/
   DEBUG_YCALC    yLOG_enter   (__FUNCTION__);
   DEBUG_YCALC    yLOG_complex ("range"     , "bb=%4d, eb=%4d, bx=%4d, ex=%4d, by=%4d, ey=%4d, bz=%4d, ez=%4d", bb, eb, bx, ex, by, ey, bz, ez);
   /*---(find/create)--------------------*/
   n = ycalc_range_by_coords (bb, eb, bx, ex, by, ey, bz, ez);
   DEBUG_YCALC    yLOG_value   ("n"         , n);
   if (n < 0) {
      n = ycalc_range_add  (bb, eb, bx, ex, by, ey, bz, ez);
      DEBUG_YCALC    yLOG_value   ("n"         , n);
   }
   --rce;  if (n < 0) {
      DEBUG_YCALC    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(link to source)-----------------*/
   x_range = s_ranges [n].ycalc;
   rc  = ycalc_deps_create (G_DEP_POINTER, &a_src, &x_range);
   DEBUG_YCALC    yLOG_value   ("create"    , rc);
   --rce;  if (rc  <  0) {
      DEBUG_YCALC    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save)---------------------------*/
   if (a_range != NULL)  *a_range = x_range;
   /*---(complete)-----------------------*/
   DEBUG_YCALC    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc_range_include     (tDEP_ROOT **a_src, int b, int x, int y, int z)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   tDEP_ROOT  *x_range     = NULL;
   /*---(begin)--------------------------*/
   DEBUG_YCALC    yLOG_enter   (__FUNCTION__);
   DEBUG_YCALC    yLOG_complex ("source"    , "b =%4d, x =%4d, y =%4d, z =%4d, ptr=%9p", b, x, y, z, *a_src);
   for (i = 0; i < s_nrange; ++i) {
      if (s_ranges [i].ycalc == NULL)     continue;
      DEBUG_YCALC    yLOG_complex ("range"     , "%s, bb=%4d, eb=%4d, bx=%4d, ex=%4d, by=%4d, ey=%4d, bz=%4d, ez=%4d", s_ranges [i].name, s_ranges [i].bb, s_ranges [i].eb, s_ranges [i].bx, s_ranges [i].ex, s_ranges [i].by, s_ranges [i].ey, s_ranges [i].bz, s_ranges [i].ez);
      if (s_ranges [i].bb   >  b)   continue;
      if (s_ranges [i].eb   <  b)   continue;
      if (s_ranges [i].bz   >  z)   continue;
      if (s_ranges [i].ez   <  z)   continue;
      if (s_ranges [i].bx   >  x)   continue;
      if (s_ranges [i].ex   <  x)   continue;
      if (s_ranges [i].by   >  y)   continue;
      if (s_ranges [i].ey   <  y)   continue;
      x_range = s_ranges [i].ycalc;
      if (*a_src == NULL) {
         rc = ycalc_call_who_at  (b, x, y, z, YCALC_FULL, NULL, a_src);
         if (rc  <  0) {
            DEBUG_YCALC    yLOG_exitr   (__FUNCTION__, rc);
            return rc;
         }
      }
      rc  = ycalc_deps_create (G_DEP_RANGE, &x_range, a_src);
      if (rc  <  0) {
         DEBUG_YCALC    yLOG_exitr   (__FUNCTION__, rc);
         return rc;
      }
   }
   DEBUG_YCALC    yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        range functions                       ----===*/
/*====================------------------------------------====================*/
static void  o___RANGE___________o () { return; }


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
   DEBUG_YCALC   yLOG_char    ("range#"    , x_deproot->range);
   if (x_deproot->range < 0) {
      ycalc_error_set (YCALC_ERROR_EXEC_PTR , NULL);
      return;
   }
   /*---(initialize)---------------------*/
   s_every    = ycalc_range_size (x_deproot->range);
   s_used     = 0;
   s_numbers  = s_nlit     = s_nform    = s_nmath    = s_nlike    =  0;
   s_strings  = s_slit     = s_sform    = s_slike    =  0;
   s_pointers = s_nref     = s_ncref    = s_rlike    = s_nrref    = 0;
   s_vars     = s_merges   = s_blanks   = 0;
   s_errors   = s_unknown  = 0;
   s_total    = 0.0;
   s_min      =   S_MAX;
   s_max      = -(S_MAX);
   /*---(loop)---------------------------*/
   x_next = x_deproot->reqs;
   while (x_next != NULL) {
      /*---(filter)----------------------*/
      DEBUG_YCALC   yLOG_char    ("link type" , x_next->type);
      if (x_next->type != G_DEP_RANGE) {
         x_next = x_next->next;
         continue;
      }
      rc = myCALC.e_valuer  (x_next->target->owner, &x_type, &x_value, NULL);
      DEBUG_YCALC   yLOG_value   ("valuer"    , rc);
      DEBUG_YCALC   yLOG_char    ("owner type", x_type);
      /*---(pure types)------------------*/
      if (strchr (YCALC_GROUP_NUM, x_type) != NULL) {
         ++s_used;
         ++s_numbers;
         switch (x_type) {
         case YCALC_DATA_NUM   :   ++s_nlit;   break;
         case YCALC_DATA_NFORM :   ++s_nform;  break;
         case YCALC_DATA_NLIKE :   ++s_nlike;  break;
         case YCALC_DATA_MATH  :   ++s_nmath;  break;
         }
         s_total += x_value;
         if (x_value < s_min)  s_min = x_value;
         if (x_value > s_max)  s_max = x_value;
         s_entries [s_numbers - 1] = x_value;
         DEBUG_YCALC   yLOG_value   ("numeric"   , s_numbers);
         DEBUG_YCALC   yLOG_value   ("entry"     , s_entries [s_numbers - 1]);
      } else if (strchr (YCALC_GROUP_STR, x_type) != NULL) {
         ++s_used;
         ++s_strings;
         switch (x_type) {
         case YCALC_DATA_STR   :   ++s_slit;   break;
         case YCALC_DATA_SFORM :   ++s_sform;  break;
         case YCALC_DATA_SLIKE :   ++s_slike;  break;
         }
         DEBUG_YCALC   yLOG_value   ("string"    , s_strings);
      } else if (strchr (YCALC_GROUP_POINT, x_type) != NULL) {
         ++s_used;
         ++s_pointers;
         switch (x_type) {
         case YCALC_DATA_ADDR  :   ++s_nref;   break;
         case YCALC_DATA_CADDR :   ++s_ncref;  break;
         case YCALC_DATA_RLIKE :   ++s_rlike;  break;
         case YCALC_DATA_RANGE :   ++s_nrref;  break;
         }
         DEBUG_YCALC   yLOG_value   ("pointer"   , s_pointers);
      } else if (x_type == YCALC_DATA_VAR) {
         ++s_used;
         ++s_vars;
         DEBUG_YCALC   yLOG_value   ("vars"      , s_vars);
      } else if (x_type == YCALC_DATA_ERROR) {
         ++s_used;
         ++s_errors;
         DEBUG_YCALC   yLOG_value   ("error"     , s_errors);
      } else if (x_type == YCALC_DATA_MERGED) {
         ++s_used;
         ++s_merges;
         DEBUG_YCALC   yLOG_value   ("merges"    , s_merges);
      } else if (x_type == YCALC_DATA_BLANK) {
         ++s_used;
         ++s_blanks;
         DEBUG_YCALC   yLOG_value   ("blank"     , s_blanks);
      } else {
         ++s_used;
         ++s_unknown;
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   if (s_errors + s_blanks > 0) {
      DEBUG_YCALC   yLOG_note    ("errors in data range stop calculation");
      ycalc_error_set (YCALC_ERROR_EXEC_ERR , NULL);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   ycalc_pushval (__FUNCTION__, s_total);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_every       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_every);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_used        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_used);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_empties     (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_every - s_used);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_numbers     (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_numbers);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_nlit        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_nlit);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_nform       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_nform);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_nmath       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_nmath);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_nlike       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_nlike);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_strings     (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_strings);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_slit        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_slit);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_sform       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_sform);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_slike       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_slike);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_nref        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_nref);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_ncref       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_ncref);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rlikes      (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_rlike);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_nrref       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_nrref);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_pointers    (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_pointers);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_vars        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_vars);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_merges      (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_merges);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_blanks      (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_blanks);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_errors      (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_errors);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_unknowns    (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_unknown);
   return;
}

void
ycalc_literals    (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_nlit + s_slit);
   return;
}

void
ycalc_calcs       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_nform + s_nmath + s_sform);
   return;
}

void
ycalc_likes       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_nlike + s_slike + s_rlike);
   return;
}

void
ycalc_forms       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_nform + s_nmath + s_nlike + s_sform + s_slike + s_rlike);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_dist        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, sqrt (pow (s_b, 2) + pow (s_x, 2) + pow (s_y, 2) + pow (s_z, 2)));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_tabs        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_b);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_cols        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_x);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rows        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_y);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_levels      (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, s_z);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_min         (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   if (s_errors + s_blanks > 0) {
      DEBUG_YCALC   yLOG_note    ("errors in data range stop calculation");
      ycalc_error_set (YCALC_ERROR_EXEC_ERR , NULL);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   ycalc_pushval (__FUNCTION__, s_min);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_max         (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   if (s_errors + s_blanks > 0) {
      DEBUG_YCALC   yLOG_note    ("errors in data range stop calculation");
      ycalc_error_set (YCALC_ERROR_EXEC_ERR , NULL);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   ycalc_pushval (__FUNCTION__, s_max);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_range       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   if (s_errors + s_blanks > 0) {
      DEBUG_YCALC   yLOG_note    ("errors in data range stop calculation");
      ycalc_error_set (YCALC_ERROR_EXEC_ERR , NULL);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   ycalc_pushval (__FUNCTION__, s_max - s_min);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_average     (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   if (s_errors + s_blanks > 0) {
      DEBUG_YCALC   yLOG_note    ("errors in data range stop calculation");
      ycalc_error_set (YCALC_ERROR_EXEC_ERR , NULL);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   if (s_numbers > 0)  ycalc_pushval (__FUNCTION__, s_total / s_numbers);
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
   DEBUG_YCALC   yLOG_note    ("before sorting");
   for (i = 0; i < s_numbers; ++i)  DEBUG_YCALC   yLOG_complex ("entry"     , "%3d = %10.2lf", i, s_entries [i]);
   i = 1;
   /*---(sort)---------------------------*/
   DEBUG_YCALC   yLOG_note    ("sorting");
   while (i < s_numbers) {
      ++runs;
      if (runs > 500)  break;
      /*---(load vars)-------------------*/
      a = s_entries [i - 1];
      b = s_entries [i];
      DEBUG_YCALC   yLOG_complex ("current"   , "compare i,%3d (t,%3d) =  a,%10.2lf to b,%10.2lf", i, tele, a, b);
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
      DEBUG_YCALC   yLOG_complex ("swapped"   , "now     i,%3d (t,%3d) =  a,%10.2lf to b,%10.2lf", i, tele, a, b);
      /*---(update)----------------------*/
      if (tele < 0) tele = i;
      if (i > 1) --i;
   }
   /*---(wrapup)-------------------------*/
   DEBUG_YCALC   yLOG_note    ("after sorting");
   for (i = 0; i < s_numbers; ++i)  DEBUG_YCALC   yLOG_complex ("entry"     , "%3d = %10.2lf", i, s_entries [i]);
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
   DEBUG_YCALC   yLOG_value   ("count"     , s_numbers);
   if      (s_numbers == 0)  {
      DEBUG_YCALC   yLOG_value   ("pushing"   , 0);
      ycalc_pushval (__FUNCTION__,  0);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   ycalc__range_sort ();
   /*---(figure out median)--------------*/
   switch (s_numbers % 2) {
   case 0 :  /* even means average needed   */
      x_mid1 = (s_numbers / 2.0) - 1;
      x_mid2 = x_mid1 + 1;
      break;
   case 1 :  /* odd means an exact middle   */
      x_mid1 = (s_numbers / 2);
      x_mid2 = x_mid1;
      break;
   }
   q2 = (s_entries [x_mid1] + s_entries [x_mid2]) / 2.0;
   /*---(figure out quarters)------------*/
   x_qtr = s_numbers / 4;
   switch (s_numbers % 4) {
   case 0 :
      DEBUG_YCALC   yLOG_note    ("zero");
      x_1st1 = x_qtr        - 1;
      x_1st2 = x_1st1 + 1;
      x_1stw = 2.0;
      x_3rd1 = x_qtr  * 3.0 - 1;
      x_3rd2 = x_3rd1 + 1;
      x_3rdw = 2.0;
      break;
   case 1 :
      DEBUG_YCALC   yLOG_note    ("one");
      x_1st1 = x_qtr        - 1;
      x_1st2 = x_1st1 + 1;
      x_1stw = 1.0;
      x_3rd1 = x_qtr  * 3.0;
      x_3rd2 = x_3rd1 + 1;
      x_3rdw = 3.0;
      break;
   case 2 :  /* even in each half, so quartile is exact element  */ 
      DEBUG_YCALC   yLOG_note    ("two");
      x_1st1 = x_qtr;
      x_1st2 = x_1st1;
      x_1stw = 2.0;
      x_3rd1 = x_qtr  * 3.0 + 1;
      x_3rd2 = x_3rd1;
      x_3rdw = 2.0;
      break;
   case 3 :
      DEBUG_YCALC   yLOG_note    ("three");
      x_1st1 = x_qtr;
      x_1st2 = x_1st1 + 1;
      x_1stw = 3.0;
      x_3rd1 = x_qtr  * 3.0 + 1;
      x_3rd2 = x_3rd1 + 1;
      x_3rdw = 1.0;
      break;
   }
   DEBUG_YCALC   yLOG_complex ("x_1st1"    , "i=%3d, v=%10.5lf, w=%4.2lf, f=%10.5lf", x_1st1, s_entries [x_1st1], x_1stw      , s_entries [x_1st1] * x_1stw);
   DEBUG_YCALC   yLOG_complex ("x_1st2"    , "i=%3d, v=%10.5lf, w=%4.2lf, f=%10.5lf", x_1st2, s_entries [x_1st2], 4.0 - x_1stw, s_entries [x_1st2] * (4.0 - x_1stw));
   q1 = ((s_entries [x_1st1] * x_1stw) + (s_entries [x_1st2] * (4.0 - x_1stw))) / 4.0;
   /*> printf ("q1 = %lf\n", q1);                                                     <*/
   DEBUG_YCALC   yLOG_complex ("x_1st"     , "%10.5lf", q1);
   DEBUG_YCALC   yLOG_complex ("x_3rd1"    , "i=%3d, v=%10.5lf, w=%4.2lf, f=%10.5lf", x_3rd1, s_entries [x_3rd1], x_3rdw      , s_entries [x_3rd1] * x_3rdw);
   DEBUG_YCALC   yLOG_complex ("x_3rd2"    , "i=%3d, v=%10.5lf, w=%4.2lf, f=%10.5lf", x_3rd2, s_entries [x_3rd2], 4.0 - x_3rdw, s_entries [x_3rd2] * (4.0 - x_3rdw));
   q3 = ((s_entries [x_3rd1] * x_3rdw) + (s_entries [x_3rd2] * (4.0 - x_3rdw))) / 4.0;
   /*> printf ("q3 = %lf\n", q3);                                                     <*/
   DEBUG_YCALC   yLOG_complex ("x_3rd"     , "%10.5lf", q3);
   qr = q3 - q1;
   qs = (q3 + q1 - (2.0 * q2)) / (q3 - q1);
   /*> printf ("qr = %lf\n", qr);                                                     <*/
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_quarter1    (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_quarters ();
   if (s_errors + s_blanks > 0) {
      DEBUG_YCALC   yLOG_note    ("errors in data range stop calculation");
      ycalc_error_set (YCALC_ERROR_EXEC_ERR , NULL);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   ycalc_pushval (__FUNCTION__, q1);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_median      (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_quarters ();
   if (s_errors + s_blanks > 0) {
      DEBUG_YCALC   yLOG_note    ("errors in data range stop calculation");
      ycalc_error_set (YCALC_ERROR_EXEC_ERR , NULL);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   ycalc_pushval (__FUNCTION__, q2);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_quarter3    (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_quarters ();
   if (s_errors + s_blanks > 0) {
      DEBUG_YCALC   yLOG_note    ("errors in data range stop calculation");
      ycalc_error_set (YCALC_ERROR_EXEC_ERR , NULL);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   ycalc_pushval (__FUNCTION__, q3);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rangeq      (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_quarters ();
   if (s_errors + s_blanks > 0) {
      DEBUG_YCALC   yLOG_note    ("errors in data range stop calculation");
      ycalc_error_set (YCALC_ERROR_EXEC_ERR , NULL);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   ycalc_pushval (__FUNCTION__, qr);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_skew        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc__range_quarters ();
   if (s_errors + s_blanks > 0) {
      DEBUG_YCALC   yLOG_note    ("errors in data range stop calculation");
      ycalc_error_set (YCALC_ERROR_EXEC_ERR , NULL);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   ycalc_pushval (__FUNCTION__, qs);
   return;
}

void    /*-> find the most common value ---------[ ------ [fv.952.070.43]*/ /*-[02.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_mode         (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
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
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   ycalc__range_quarters ();
   DEBUG_YCALC   yLOG_value   ("count_num" , s_numbers);
   if      (s_numbers == 0)  {
      DEBUG_YCALC   yLOG_value   ("pushing"   , 0);
      ycalc_pushval (__FUNCTION__,  0);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   x_save = x_curr;
   for (i = 0; i < s_numbers; ++i) {
      x_curr = (int) s_entries [i];
      DEBUG_YCALC   yLOG_complex ("entry"     , "i=%3d, x_curr=%3d, x_save=%3d", i, x_curr, x_save);
      if (x_curr != x_save)  x_count = 0;
      ++x_count;
      DEBUG_YCALC   yLOG_value   ("count"     , x_count);
      if (x_count == x_highest) {
         ++x_multi;
         DEBUG_YCALC   yLOG_complex ("multi"     , "i=%3d, mode=%3d, highest=%3d", i, x_mode, x_highest);
      }
      if (x_count > x_highest) {
         x_mode    = x_curr;
         x_highest = x_count;
         x_multi   = 0;
         DEBUG_YCALC   yLOG_complex ("new_mode"  , "i=%3d, mode=%3d, highest=%3d", i, x_mode, x_highest);
      }
      x_save = x_curr;
   }
   if (x_multi >= 1)  ycalc_pushval (__FUNCTION__, (double) q2);
   else               ycalc_pushval (__FUNCTION__, (double) x_mode);
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return;
}

void    /*-> find the measure of variance -------[ ------ [fv.A52.070.22]*/ /*-[01.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_stddev       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(locals)-----------+-----------+-*/
   int         i           =  0;            /* loop iterator -- entry         */
   double      x_mean      =  0;
   double      x_curr      =  0;
   double      x_diff      =  0;
   double      x_var       =  0;
   double      x_total     =  0;
   double      x_stddev    =  0;
   /*---(start)--------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   ycalc__range_gather (__FUNCTION__);
   if (s_errors + s_blanks > 0) {
      DEBUG_YCALC   yLOG_note    ("errors in data range stop calculation");
      ycalc_error_set (YCALC_ERROR_EXEC_ERR , NULL);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   DEBUG_YCALC   yLOG_value   ("count_num" , s_numbers);
   /*---(defense: too few values)--------*/
   if      (s_numbers <= 1)  {
      DEBUG_YCALC   yLOG_value   ("pushing"   , 0);
      ycalc_pushval (__FUNCTION__,  0);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   /*---(calc mean)----------------------*/
   x_mean = s_total / ((double) s_numbers);
   /*---(gather variances)---------------*/
   for (i = 0; i < s_numbers; ++i) {
      x_curr   = s_entries [i];
      x_diff   = x_curr - x_mean;
      x_var    = x_diff * x_diff;
      x_total += x_var;
      DEBUG_YCALC   yLOG_complex ("entry"     , "i=%3d, x_curr=%5.2lf, x_diff=%5.2lf, x_var=%10.2lf, x_total=%10.2lf", i, x_curr, x_diff, x_var, x_total);
   }
   /*---(final calc)---------------------*/
   DEBUG_YCALC   yLOG_llong   ("variance"  , x_total / ((double) s_numbers));
   x_stddev   = sqrt(x_total / ((double) s_numbers));
   DEBUG_YCALC   yLOG_llong   ("stddev"    , x_stddev);
   ycalc_pushval (__FUNCTION__, x_stddev);
   /*---(complete)-----------------------*/
   x_mean = s_total / ((double) s_numbers);
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return;
}



/*====================------------------------------------====================*/
/*===----                        offset functions                      ----===*/
/*====================------------------------------------====================*/
static void  o___OFFSET__________o () { return; }

void
ycalc__rel_driver    (char *a_type)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   tDEP_ROOT  *x_deproot   = NULL;
   char        t           [LEN_LABEL];
   int         uo, xo, yo, zo;
   /*---(prepare)------------------------*/
   DEBUG_YCALC   yLOG_info    ("a_type"    , a_type);
   uo = xo = yo = zo = 0;
   /*---(pop appropriate offsets)--------*/
   if (a_type [6] == 'z')   zo     = ycalc_popval      (__FUNCTION__);
   if (a_type [5] == 'y')   yo     = ycalc_popval      (__FUNCTION__);
   if (a_type [4] == 'x')   xo     = ycalc_popval      (__FUNCTION__);
   if (a_type [3] == 'b')   uo     = ycalc_popval      (__FUNCTION__);
   if (ycalc_error_true ())  return;
   DEBUG_YCALC   yLOG_complex ("offsets"   , "%2du, %3dx, %3dy, %3dz", uo, xo, yo, zo);
   /*---(pop origination)----------------*/
   if (a_type [1] == 'm')   ycalc_pushref (__FUNCTION__, myCALC.deproot, myCALC.label);
   ycalc_popval_plus (__FUNCTION__, G_SPECIAL_ALLPOS);
   if (ycalc_error_true ())  return;
   /*---(pop appropriate offsets)--------*/
   if (a_type [0] == 'r') {
      DEBUG_YCALC   yLOG_note    ("relative type");
      o     += zo;
      n     += yo;
      m     += xo;
      s_buf += uo;
   }
   if (a_type [0] == 'a') {
      DEBUG_YCALC   yLOG_note    ("absolute type");
      if (a_type [6] == 'z')   o      = zo;
      if (a_type [5] == 'y')   n      = yo;
      if (a_type [4] == 'x')   m      = xo;
      if (a_type [3] == 'b')   s_buf  = uo;
   }
   DEBUG_YCALC   yLOG_complex ("address"   , "%2du, %3dx, %3dy, %3dz", s_buf, m, n, o);
   /*---(look at destination)------------*/
   rc = ycalc_call_who_at (s_buf, m, n, o, YCALC_FULL, NULL, &x_deproot);
   if (rc < 0)  {
      ycalc_error_set (YCALC_ERROR_EXEC_REF , NULL);
      return;
   }
   /*---(create calc reference)----------*/
   DEBUG_YCALC   yLOG_point   ("CALCREF"    , ycalc_call_labeler (x_deproot));
   if (myCALC.deproot == x_deproot) {
      ycalc_error_set (YCALC_ERROR_EXEC_CIR , NULL);
      return;
   }
   ycalc_pushref     (__FUNCTION__, x_deproot, ycalc_call_labeler (x_deproot));
   rc = ycalc_deps_create (G_DEP_CALCREF, &(myCALC.deproot), &x_deproot);
   if (rc < 0) {
      ycalc_error_set (YCALC_ERROR_EXEC_CIR , NULL);
      return;
   }
   /*---(complete)-----------------------*/
   return;
}

void ycalc_rel_b         (void)  { return ycalc__rel_driver ("rm/b___"); }
void ycalc_rel_x         (void)  { return ycalc__rel_driver ("rm/_x__"); }
void ycalc_rel_y         (void)  { return ycalc__rel_driver ("rm/__y_"); }
void ycalc_rel_xy        (void)  { return ycalc__rel_driver ("rm/_xy_"); }
void ycalc_rel_bxy       (void)  { return ycalc__rel_driver ("rm/bxy_"); }

void ycalc_abs_b         (void)  { return ycalc__rel_driver ("am/b___"); }
void ycalc_abs_x         (void)  { return ycalc__rel_driver ("am/_x__"); }
void ycalc_abs_y         (void)  { return ycalc__rel_driver ("am/__y_"); }
void ycalc_abs_xy        (void)  { return ycalc__rel_driver ("am/_xy_"); }
void ycalc_abs_bxy       (void)  { return ycalc__rel_driver ("am/bxy_"); }
void ycalc_address       (void)  { return ycalc__rel_driver ("am/bxy_"); }

void ycalc_ru            (void)  { return ycalc__rel_driver ("r_/b___"); }
void ycalc_rx            (void)  { return ycalc__rel_driver ("r_/_x__"); }
void ycalc_ry            (void)  { return ycalc__rel_driver ("r_/__y_"); }
void ycalc_rxy           (void)  { return ycalc__rel_driver ("r_/_xy_"); }
void ycalc_ruxy          (void)  { return ycalc__rel_driver ("r_/bxy_"); }

void ycalc_au            (void)  { return ycalc__rel_driver ("a_/b___"); }
void ycalc_ax            (void)  { return ycalc__rel_driver ("a_/_x__"); }
void ycalc_ay            (void)  { return ycalc__rel_driver ("a_/__y_"); }
void ycalc_axy           (void)  { return ycalc__rel_driver ("a_/_xy_"); }
void ycalc_auxy          (void)  { return ycalc__rel_driver ("a_/bxy_"); }

/*> void                                                                              <* 
 *> ycalc__abs_driver    (char *a_type)                                               <* 
 *> {                                                                                 <* 
 *>    DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);                       <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        rc          =    0;                                                <* 
 *>    tDEP_ROOT  *x_deproot   = NULL;                                                <* 
 *>    /+---(parse the current locaction)----+/                                       <* 
 *>    ycalc_pushref     (__FUNCTION__, myCALC.deproot, myCALC.label);                <* 
 *>    ycalc_popval_plus (__FUNCTION__, G_SPECIAL_ALLPOS);                            <* 
 *>    if (ycalc_error_true ()) return ;                                              <* 
 *>    /+---(pop appropriate replacements)---+/                                       <* 
 *>    if (a_type [3] == 'z')   o      = ycalc_popval      (__FUNCTION__);            <* 
 *>    if (a_type [2] == 'y')   n      = ycalc_popval      (__FUNCTION__);            <* 
 *>    if (a_type [1] == 'x')   m      = ycalc_popval      (__FUNCTION__);            <* 
 *>    if (a_type [0] == 'b')   s_buf  = ycalc_popval      (__FUNCTION__);            <* 
 *>    if (ycalc_error_true ()) return ;                                              <* 
 *>    /+---(look at destination)------------+/                                       <* 
 *>    rc = ycalc_call_who_at (s_buf, m, n, o, YCALC_FULL, NULL, &x_deproot);         <* 
 *>    if (rc < 0)  {                                                                 <* 
 *>       ycalc_error_set (YCALC_ERROR_EXEC_REF , NULL);                              <* 
 *>       return;                                                                     <* 
 *>    }                                                                              <* 
 *>    /+---(create calc reference)----------+/                                       <* 
 *>    DEBUG_YCALC   yLOG_point   ("CALCREF"    , ycalc_call_labeler (x_deproot));    <* 
 *>    if (myCALC.deproot == x_deproot) {                                             <* 
 *>       ycalc_error_set (YCALC_ERROR_EXEC_CIR , NULL);                              <* 
 *>       return;                                                                     <* 
 *>    }                                                                              <* 
 *>    ycalc_pushref     (__FUNCTION__, x_deproot, ycalc_call_labeler (x_deproot));   <* 
 *>    rc = ycalc_deps_create (G_DEP_CALCREF, &(myCALC.deproot), &x_deproot);         <* 
 *>    if (rc < 0) {                                                                  <* 
 *>       ycalc_error_set (YCALC_ERROR_EXEC_PTR , NULL);                              <* 
 *>       return;                                                                     <* 
 *>    }                                                                              <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    return;                                                                        <* 
 *> }                                                                                 <*/

/*> void ycalc_abs_b         (void)  { return ycalc__abs_driver ("Am b___"); }        <* 
 *> void ycalc_abs_x         (void)  { return ycalc__abs_driver ("Am _x__"); }        <* 
 *> void ycalc_abs_y         (void)  { return ycalc__abs_driver ("Am __y_"); }        <* 
 *> void ycalc_abs_xy        (void)  { return ycalc__abs_driver ("Am _xy_"); }        <* 
 *> void ycalc_abs_bxy       (void)  { return ycalc__abs_driver ("Am bxy_"); }        <* 
 *> void ycalc_address       (void)  { return ycalc__abs_driver ("Am bxy_"); }        <*/

/*> void                                                                                        <* 
 *> ycalc__off_driver    (char *a_type)                                                         <* 
 *> {                                                                                           <* 
 *>    DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                                 <* 
 *>    char        rc          =    0;                                                          <* 
 *>    tDEP_ROOT  *x_deproot   = NULL;                                                          <* 
 *>    /+---(pop appropriate offsets)--------+/                                                 <* 
 *>    a = b = c = d = 0;                                                                       <* 
 *>    if (a_type [3] == 'z')   d      = ycalc_popval      (__FUNCTION__);                      <* 
 *>    if (a_type [2] == 'y')   c      = ycalc_popval      (__FUNCTION__);                      <* 
 *>    if (a_type [1] == 'x')   b      = ycalc_popval      (__FUNCTION__);                      <* 
 *>    if (a_type [0] == 'b')   a      = ycalc_popval      (__FUNCTION__);                      <* 
 *>    if (ycalc_error_true ()) return ;                                                        <* 
 *>    /+---(parse the base location)--------+/                                                 <* 
 *>    ycalc_popval_plus (__FUNCTION__, G_SPECIAL_ALLPOS);                                      <* 
 *>    if (ycalc_error_true ()) return ;                                                        <* 
 *>    /+---(look at destination)------------+/                                                 <* 
 *>    rc = ycalc_call_who_at (s_buf + a, m + b, n + c, o + d, YCALC_FULL, NULL, &x_deproot);   <* 
 *>    if (rc < 0)  {                                                                           <* 
 *>       ycalc_error_set (YCALC_ERROR_EXEC_REF , NULL);                                        <* 
 *>       return;                                                                               <* 
 *>    }                                                                                        <* 
 *>    /+---(create calc reference)----------+/                                                 <* 
 *>    DEBUG_YCALC   yLOG_point   ("CALCREF"    , ycalc_call_labeler (x_deproot));              <* 
 *>    if (myCALC.deproot == x_deproot) {                                                       <* 
 *>       ycalc_error_set (YCALC_ERROR_EXEC_CIR , NULL);                                        <* 
 *>       return;                                                                               <* 
 *>    }                                                                                        <* 
 *>    ycalc_pushref     (__FUNCTION__, x_deproot, ycalc_call_labeler (x_deproot));             <* 
 *>    rc = ycalc_deps_create (G_DEP_CALCREF, &(myCALC.deproot), &x_deproot);                   <* 
 *>    if (rc < 0) {                                                                            <* 
 *>       ycalc_error_set (YCALC_ERROR_EXEC_CIR , NULL);                                        <* 
 *>       return;                                                                               <* 
 *>    }                                                                                        <* 
 *>    /+---(complete)-----------------------+/                                                 <* 
 *>    return;                                                                                  <* 
 *> }                                                                                           <*/

/*> void ycalc_off_b         (void)  { return ycalc__off_driver ("b___"); }           <* 
 *> void ycalc_off_x         (void)  { return ycalc__off_driver ("_x__"); }           <* 
 *> void ycalc_off_y         (void)  { return ycalc__off_driver ("__y_"); }           <* 
 *> void ycalc_off_xy        (void)  { return ycalc__off_driver ("_xy_"); }           <* 
 *> void ycalc_off_bxy       (void)  { return ycalc__off_driver ("bxy_"); }           <*/



/*====================------------------------------------====================*/
/*===----                          spreedsheet                         ----===*/
/*====================------------------------------------====================*/
static void  o___SPREADSHEET_____o () { return; }

static tDEP_ROOT  *s_deproot_last = NULL;

void
ycalc__lookup_makedep   (char u, short x, short y, short z)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          = 0;
   tDEP_ROOT  *x_deproot   = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(get cell)-----------------------*/
   rc = ycalc_call_who_at  (u, x, y, z, YCALC_FULL, NULL, &x_deproot);
   DEBUG_YCALC   yLOG_value   ("who_at"     , rc);
   DEBUG_YCALC   yLOG_point   ("x_deproot"  , x_deproot);
   DEBUG_YCALC   yLOG_point   ("CALCREF"    , ycalc_call_labeler (x_deproot));
   /*---(make dependency)----------------*/
   ycalc_pushref     (__FUNCTION__, x_deproot, ycalc_call_labeler (x_deproot));
   ycalc_deps_create (G_DEP_CALCREF, &(myCALC.deproot), &x_deproot);
   /*---(save)---------------------------*/
   s_deproot_last = x_deproot;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return;
}

void    /*-> search left column in range --------[ ------ [fv.A71.030.E7]*/ /*-[01.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc__lookup_common  (char *a_func, char a_dir, char a_type, char a_match)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          = 0;
   int         u_cur, x_cur, y_cur, z_cur;
   int         x_beg, y_beg;
   int         x_end, y_end;
   void       *x_owner     = NULL;
   tDEP_ROOT  *x_deproot   = NULL;
   char        x_type      =  '-';
   char       *x_string    = NULL;
   double      x_value     =  0.0;
   int         x_off       =    0;
   int         y_off       =    0;
   char        t           [LEN_LABEL];
   int         x, y;
   char        x_found     = '-';
   double      x_best      = 9999999;
   int         x_len       =   0;
   int         x_ind       =   0;
   int         x_cnt       =   0;
   DEBUG_YCALC   yLOG_enter   (a_func);
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   DEBUG_YCALC   yLOG_char    ("a_dir"      , a_dir);
   s_deproot_last = NULL;
   /*---(get values)---------------------*/
   switch (a_dir) {
   case 'v' :  x_off = ycalc_popval (__FUNCTION__);  break;
   case 'h' :  y_off = ycalc_popval (__FUNCTION__);  break;
   case 'r' : break;
   case 'i' :  x_ind = ycalc_popval (__FUNCTION__);  break;
   case 'c' :  x_ind = 9999999;                      break;
   default  :
               ycalc_error_set (YCALC_ERROR_EXEC_ARG , YCALC_ERROR_INTERN);
               DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
               DEBUG_YCALC   yLOG_exit    (a_func);
               return;
   }
   if (x_ind < 0) {
      DEBUG_YCALC   yLOG_note    ("requested negative position");
      ycalc_error_set (YCALC_ERROR_EXEC_ARG , YCALC_ERROR_ARG4);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      DEBUG_YCALC   yLOG_exit    (a_func);
      return;
   }
   DEBUG_YCALC   yLOG_value   ("x_off"      , x_off);
   DEBUG_YCALC   yLOG_value   ("y_off"      , y_off);
   DEBUG_YCALC   yLOG_value   ("x_ind"      , x_ind);
   /*---(get target)---------------------*/
   switch (a_type) {
   case 's' :
      r = ycalc_popstr (__FUNCTION__);
      if (r == NULL)  r = strndup ("", LEN_RECD);
      DEBUG_YCALC   yLOG_info    ("r"          , r);
      break;
   case '9' :
      if (strchr (":+", a_match) != NULL) {
         b = ycalc_popval (__FUNCTION__);
         DEBUG_YCALC   yLOG_value   ("b"          , b);
      }
      a = ycalc_popval (__FUNCTION__);
      DEBUG_YCALC   yLOG_value   ("a"          , a);
      break;
   default  :
      DEBUG_YCALC   yLOG_note    ("a_type tot legal");
      ycalc_error_set (YCALC_ERROR_EXEC_ARG , YCALC_ERROR_INTERN);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      DEBUG_YCALC   yLOG_exit    (a_func);
      return;
   }
   if (a_match == 0 || strchr ("=>:+~[]", a_match) == NULL) {
      DEBUG_YCALC   yLOG_note    ("a_match tot legal");
      ycalc_error_set (YCALC_ERROR_EXEC_ARG , YCALC_ERROR_INTERN);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      DEBUG_YCALC   yLOG_exit    (a_func);
      return;
   }
   /*---(prepare)------------------------*/
   x_deproot = (tDEP_ROOT *) ycalc_popref (__FUNCTION__);
   DEBUG_YCALC   yLOG_point   ("x_deproot"  , x_deproot);
   if (x_deproot == NULL) {
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      DEBUG_YCALC   yLOG_exit    (a_func);
      return;
   }
   u_cur = s_ranges [x_deproot->range].bb;
   x_beg = s_ranges [x_deproot->range].bx;
   y_beg = s_ranges [x_deproot->range].by;
   z_cur = s_ranges [x_deproot->range].bz;
   x_end = s_ranges [x_deproot->range].ex;
   y_end = s_ranges [x_deproot->range].ey;
   DEBUG_YCALC   yLOG_complex ("coords"    , "b=%4d, bx=%4d, ex=%4d, by=%4d, ey=%4d, bz=%4d, ez=%4d", u_cur, x_beg, x_end, y_beg, y_end, z_cur, z_cur);
   switch (a_dir) {
   case 'v'  :  x_end = x_beg;  break;
   case 'h'  :  y_end = y_beg;  break;
   default   :  break;
   }
   DEBUG_YCALC   yLOG_complex ("loops"     , "bx=%4d, ex=%4d, by=%4d, ey=%4d"                , x_beg, x_end, y_beg, y_end);
   /*---(process)------------------------*/
   for (x_cur = x_beg; x_cur <= x_end; ++x_cur) {
      for (y_cur = y_beg; y_cur <= y_end; ++y_cur) {
         DEBUG_YCALC   yLOG_note    ("checking_____________");
         DEBUG_YCALC   yLOG_complex ("point"     , " x=%4d,  y=%4d,  z=%4d", x_cur, y_cur, z_cur);
         /*---(see what's there)---------*/
         rc = ycalc_call_who_at  (u_cur, x_cur, y_cur, z_cur, YCALC_LOOK, &x_owner, &x_deproot);
         DEBUG_YCALC   yLOG_value   ("who_at"     , rc);
         DEBUG_YCALC   yLOG_point   ("x_owner"    , x_owner);
         if (rc  <  0)                                          continue;
         if (x_owner  ==  NULL)                                 continue;
         /*---(look at the value)--------*/
         rc = myCALC.e_valuer (x_owner, &x_type, &x_value, &x_string);
         DEBUG_YCALC   yLOG_value   ("valuer"     , rc);
         if (rc  <  0)                                          continue;
         /*---(string types)-------------*/
         if (a_type == 's') {
            /*---(filter)-------------------*/
            DEBUG_YCALC   yLOG_char    ("x_type"    , x_type);
            if (strchr (YCALC_GROUP_STR, x_type) == NULL)         continue;
            DEBUG_YCALC   yLOG_point   ("x_string"   , x_string);
            if (x_string == NULL)                                 continue;
            DEBUG_YCALC   yLOG_info    ("x_string"   , x_string);
            /*---(compare)------------------*/
            if (x_string [0] != r [0])                            continue;
            switch (a_match) {
            case '=' :
               if (strcmp  (x_string, r) != 0)                    continue;
               if (x_cnt++ < x_ind)                                 continue;
               x_found = 'y';
               break;
            case '>' :
               x_len = strlen (r);
               if (x_len < 1)                                     continue;
               if (strncmp (x_string, r, x_len) != 0)             continue;
               if (x_cnt++ < x_ind)                                 continue;
               x_found = 'y';
               break;
            default :
               DEBUG_YCALC   yLOG_note    ("MATCH TYPE NOT ENABLED");
               break;
            }
            /*---(done)---------------------*/
         }
         /*---(string types)-------------*/
         else if (a_type == '9') {
            /*---(filter)-------------------*/
            DEBUG_YCALC   yLOG_char    ("x_type"    , x_type);
            if (strchr (YCALC_GROUP_NUM, x_type) == NULL)         continue;
            DEBUG_YCALC   yLOG_value   ("x_value"    , x_value);
            /*---(compare)------------------*/
            switch (a_match) {
            case '=' :  /* first match */
               if (x_value != a)                                  continue;
               if (x_cnt++ < x_ind)                                 continue;
               x_found = 'y';
               break;
            case ':' :  /* first in range */
               if (x_value <  a)                                  continue;
               if (x_value >  b)                                  continue;
               if (x_cnt++ < x_ind)                                 continue;
               x_found = 'y';
               x = x_cur + x_off;
               y = y_cur + y_off;
               break;
            case '+' :  /* first in tolerance */
               if (x_value <  a - b)                              continue;
               if (x_value >  a + b)                              continue;
               if (x_cnt++ < x_ind)                                 continue;
               x_found = 'y';
               x = x_cur + x_off;
               y = y_cur + y_off;
               break;
            case '~' :  /* closest to target */
               if (x_found != 'y') {
                  x_best  = x_value;
                  x_found = 'y';
                  x = x_cur + x_off;
                  y = y_cur + y_off;
                  continue;
               } else if (fabs (a - x_value) >= fabs (a - x_best))  {
                  continue;
               } else {
                  x_best  = x_value;
                  x_found = 'y';
                  x = x_cur + x_off;
                  y = y_cur + y_off;
                  continue;
               }
               break;
            case '[' :  /* first over value */
               if (x_value <  a)  continue;
               break;
            case ']' :  /* last under value */
               if (x_value > a)  continue;
               x_found = 'y';
               x = x_cur + x_off;
               y = y_cur + y_off;
               continue;
               break;
            }
            /*---(done)---------------------*/
         }
         /*---(return)-------------------*/
         ycalc__lookup_makedep   (u_cur, x_cur + x_off, y_cur + y_off, z_cur);
         DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
         DEBUG_YCALC   yLOG_exit    (a_func);
         return 0;
      }
   }
   /*---(check for numerics)-------------*/
   if (a_type == '9' && strchr ("=[", a_match) == NULL && x_found == 'y') {
      ycalc__lookup_makedep   (u_cur, x, y, z_cur);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      DEBUG_YCALC   yLOG_exit    (a_func);
      return 0;
   }
   if (a_dir  == 'c') {
      ycalc_pushval (__FUNCTION__, x_cnt);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      DEBUG_YCALC   yLOG_exit    (a_func);
      return 0;
   }
   /*---(nothing found)------------------*/
   ycalc_error_set (YCALC_ERROR_EXEC_MISS, NULL);
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   DEBUG_YCALC   yLOG_exit    (a_func);
   return;
}

void ycalc_vlookup       (void)  { return ycalc__lookup_common (__FUNCTION__, 'v', 's', '='); }
void ycalc_vref          (void)  { ycalc_pushval (__FUNCTION__, 0);  return ycalc_vlookup (); }
void ycalc_vprefix       (void)  { return ycalc__lookup_common (__FUNCTION__, 'v', 's', '>'); }
void ycalc_vmatch        (void)  { return ycalc__lookup_common (__FUNCTION__, 'v', '9', '='); }
void ycalc_vrange        (void)  { return ycalc__lookup_common (__FUNCTION__, 'v', '9', ':'); }
void ycalc_vabout        (void)  { return ycalc__lookup_common (__FUNCTION__, 'v', '9', '+'); }
void ycalc_vclose        (void)  { return ycalc__lookup_common (__FUNCTION__, 'v', '9', '~'); }
void ycalc_vover         (void)  { return ycalc__lookup_common (__FUNCTION__, 'v', '9', '['); }
void ycalc_vunder        (void)  { return ycalc__lookup_common (__FUNCTION__, 'v', '9', ']'); }

void ycalc_hlookup       (void)  { return ycalc__lookup_common (__FUNCTION__, 'h', 's', '='); }
void ycalc_href          (void)  { ycalc_pushval (__FUNCTION__, 0);  return ycalc_hlookup (); }
void ycalc_hprefix       (void)  { return ycalc__lookup_common (__FUNCTION__, 'h', 's', '>'); }
void ycalc_hmatch        (void)  { return ycalc__lookup_common (__FUNCTION__, 'h', '9', '='); }
void ycalc_hrange        (void)  { return ycalc__lookup_common (__FUNCTION__, 'h', '9', ':'); }
void ycalc_habout        (void)  { return ycalc__lookup_common (__FUNCTION__, 'h', '9', '+'); }
void ycalc_hclose        (void)  { return ycalc__lookup_common (__FUNCTION__, 'h', '9', '~'); }
void ycalc_hover         (void)  { return ycalc__lookup_common (__FUNCTION__, 'h', '9', '['); }
void ycalc_hunder        (void)  { return ycalc__lookup_common (__FUNCTION__, 'h', '9', ']'); }

void ycalc_rlookup       (void)  { return ycalc__lookup_common (__FUNCTION__, 'r', 's', '='); }
void ycalc_rprefix       (void)  { return ycalc__lookup_common (__FUNCTION__, 'r', 's', '>'); }
void ycalc_rmatch        (void)  { return ycalc__lookup_common (__FUNCTION__, 'r', '9', '='); }
void ycalc_rrange        (void)  { return ycalc__lookup_common (__FUNCTION__, 'r', '9', ':'); }
void ycalc_rabout        (void)  { return ycalc__lookup_common (__FUNCTION__, 'r', '9', '+'); }
void ycalc_rclose        (void)  { return ycalc__lookup_common (__FUNCTION__, 'r', '9', '~'); }
void ycalc_rover         (void)  { return ycalc__lookup_common (__FUNCTION__, 'r', '9', '['); }
void ycalc_runder        (void)  { return ycalc__lookup_common (__FUNCTION__, 'r', '9', ']'); }

void ycalc_ilookup       (void)  { return ycalc__lookup_common (__FUNCTION__, 'i', 's', '='); }
void ycalc_iprefix       (void)  { return ycalc__lookup_common (__FUNCTION__, 'i', 's', '>'); }
void ycalc_imatch        (void)  { return ycalc__lookup_common (__FUNCTION__, 'i', '9', '='); }
void ycalc_irange        (void)  { return ycalc__lookup_common (__FUNCTION__, 'i', '9', ':'); }
void ycalc_iabout        (void)  { return ycalc__lookup_common (__FUNCTION__, 'i', '9', '+'); }

void ycalc_clookup       (void)  { return ycalc__lookup_common (__FUNCTION__, 'c', 's', '='); }
void ycalc_cprefix       (void)  { return ycalc__lookup_common (__FUNCTION__, 'c', 's', '>'); }
void ycalc_cmatch        (void)  { return ycalc__lookup_common (__FUNCTION__, 'c', '9', '='); }
void ycalc_crange        (void)  { return ycalc__lookup_common (__FUNCTION__, 'c', '9', ':'); }
void ycalc_cabout        (void)  { return ycalc__lookup_common (__FUNCTION__, 'c', '9', '+'); }

void
ycalc_entry             (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   char        rc          =    0;
   int         x_buf       =    0;
   int         y_cur       =    0;
   int         y_max       =    0;
   int         y_min       =    0;
   int         x_cur       =    0;
   int         x_off       =    0;
   int         z_cur       =    0;
   void       *x_owner     = NULL;
   char        x_type      =  '-';
   tDEP_ROOT  *x_deproot   = NULL;
   x_off = ycalc_popval (__FUNCTION__);
   ycalc_pushref     (__FUNCTION__, myCALC.deproot, myCALC.label);
   ycalc_popval_plus (__FUNCTION__, G_SPECIAL_ALLPOS);
   x_buf = s_buf;
   x_cur = m;
   y_max = n;
   z_cur = o;
   y_min = 0;
   for (y_cur = y_max; y_cur >= y_min; --y_cur) {
      /*---(see what's there)---------*/
      rc = ycalc_call_who_at  (x_buf, x_cur - x_off, y_cur, z_cur, YCALC_LOOK, &x_owner, &x_deproot);
      DEBUG_YCALC   yLOG_value   ("who_at"     , rc);
      DEBUG_YCALC   yLOG_point   ("x_owner"    , x_owner);
      if (rc  <  0)                                          continue;
      if (x_owner  ==  NULL)                                 continue;
      /*---(look at the value)--------*/
      rc = myCALC.e_valuer (x_owner, &x_type, NULL, NULL);
      DEBUG_YCALC   yLOG_value   ("valuer"     , rc);
      if (rc  <  0)                                          continue;
      /*---(filter)-------------------*/
      DEBUG_YCALC   yLOG_char    ("x_type"    , x_type);
      if (x_type == YCALC_DATA_BLANK)                       continue;
      /*---(save)---------------------*/
      rc = ycalc_call_who_at  (x_buf, x_cur - x_off, y_cur, z_cur, YCALC_FULL, &x_owner, &x_deproot);
      DEBUG_YCALC   yLOG_value   ("who_at"     , rc);
      DEBUG_YCALC   yLOG_point   ("x_owner"    , x_owner);
      DEBUG_YCALC   yLOG_point   ("x_deproot"  , x_deproot);
      if (rc  <  0 || x_owner == NULL || x_deproot == NULL) {
         rc = -1;
         break;
      }
      DEBUG_YCALC   yLOG_point   ("CALCREF"    , ycalc_call_labeler (x_deproot));
      ycalc_pushref     (__FUNCTION__, x_deproot, ycalc_call_labeler (x_deproot));
      ycalc_deps_create (G_DEP_CALCREF, &(myCALC.deproot), &x_deproot);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   /*---(nothing found)------------------*/
   ycalc_error_set (YCALC_ERROR_EXEC_MISS, NULL);
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return;
}



