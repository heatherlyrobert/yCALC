/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"



/*====================------------------------------------====================*/
/*===----                     arithmetic operators                     ----===*/
/*====================------------------------------------====================*/
static void  o___ARITHMETIC______o () { return; }

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_add         (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, b + a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_subtract     (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, b - a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_multiply     (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, b * a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.000.22]*/ /*-[00.0000.03#.!]-*/ /*-[--.---.---.--]-*/
ycalc_divide       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   if (a != 0)  ycalc_pushval (__FUNCTION__, b / a);
   else         ycalc_pushval (__FUNCTION__, 0);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_modulus      (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   if (a != 0)  ycalc_pushval (__FUNCTION__, ((int) b) % ((int) a));
   else         ycalc_pushval (__FUNCTION__, 0);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_increment    (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, a + 1);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_decrement    (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, a - 1);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_unaryminus   (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, 0 - a);
   return;
}




/*====================------------------------------------====================*/
/*===----                     mathmatical functions                    ----===*/
/*====================------------------------------------====================*/
static void  o___MATH____________o () { return; }

void    /*-> tbd --------------------------------[ ------ [fv.320.010.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_power        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   int   i = 0;
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   c = b;
   for (i = 1; i <  a; ++i)  c *= b;
   ycalc_pushval (__FUNCTION__, c);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.010.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_power_of_2   (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, a * a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.010.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_power_of_3   (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, a * a * a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.010.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_power_of_4   (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, a * a * a * a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.010.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_power_of     (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   int         i           =    0;
   char        rc          =    0;
   char        x_label     [LEN_LABEL] = "";
   tDEP_ROOT  *x_ref       = NULL;
   /*---(get variable)-------------------*/
   r = ycalc_popstr (__FUNCTION__);
   rc = yCALC_variable (r, x_label);
   if (rc < 0) {
      ycalc_error_set (YCALC_ERROR_EXEC_VAR , "x");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   DEBUG_YCALC   yLOG_info    ("x_label"   , x_label);
   /*---(get ref)------------------------*/
   ycalc_call_who_named (x_label, YCALC_FULL, NULL, &x_ref);
   DEBUG_YCALC   yLOG_point   ("x_ref"      , x_ref);
   if (x_ref == NULL) {
      ycalc_error_set (YCALC_ERROR_EXEC_NULL, x_ref);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   ycalc_pushref (__FUNCTION__, x_ref, x_label);
   /*---(call power)---------------------*/
   ycalc_power   ();
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.010.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_power_of_x   (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc_pushstr (__FUNCTION__, "x");
   ycalc_power_of   ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.010.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_power_of_y   (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc_pushstr (__FUNCTION__, "y");
   ycalc_power_of   ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_abs           (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, fabs(a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_sign          (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   if      (a == 0) ycalc_pushval (__FUNCTION__,  0);
   else if (a >  0) ycalc_pushval (__FUNCTION__,  1);
   else             ycalc_pushval (__FUNCTION__, -1);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_trunc         (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, trunc(a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.330.010.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rtrunc        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   int i = 0;
   n = ycalc_popval (__FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   for (i = 0; i < n; ++i)  a *= 10;
   a = trunc (a);
   for (i = 0; i < n; ++i)  a /= 10;
   ycalc_pushval (__FUNCTION__, a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_round         (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, round(a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.330.010.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rround        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   int i = 0;
   n = ycalc_popval (__FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   for (i = 0; i < n; ++i)  a *= 10;
   a = round (a);
   for (i = 0; i < n; ++i)  a /= 10;
   ycalc_pushval (__FUNCTION__, a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_ceiling       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, ceil(a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_floor         (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, floor(a));
   return;
}

void    /*-> rand between 0-1 -------------------[ ------ [fv.210.000.01]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rand          (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc_pushval (__FUNCTION__, (double) rand());
   return;
}

void    /*-> rand between n-m -------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_randr         (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, ((double) rand() / (double) RAND_MAX) * (b - a) + a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_sqrt          (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, sqrt(a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_cbrt          (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, cbrt(a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_sqr           (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, a * a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_cube          (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, a * a * a);
   return;
}



/*====================------------------------------------====================*/
/*===----                     relational operators                     ----===*/
/*====================------------------------------------====================*/
static void  o___RELATIONAL______o () { return; }

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_equal        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, b == a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_notequal      (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, b != a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_greater      (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, b > a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_lesser       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, b < a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_gequal       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, b >= a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_lequal       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, b <= a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_sequal       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   r = ycalc_popstr (__FUNCTION__);
   s = ycalc_popstr (__FUNCTION__);
   if (strcmp (s, r) == 0) ycalc_pushval (__FUNCTION__, TRUE );
   else                    ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_snotequal    (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   r = ycalc_popstr (__FUNCTION__);
   s = ycalc_popstr (__FUNCTION__);
   if (strcmp (s, r) != 0) ycalc_pushval (__FUNCTION__, TRUE );
   else                    ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_slesser      (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   r = ycalc_popstr (__FUNCTION__);
   s = ycalc_popstr (__FUNCTION__);
   if (strcmp (s, r) <  0) ycalc_pushval (__FUNCTION__, TRUE );
   else                    ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_sgreater     (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   r = ycalc_popstr (__FUNCTION__);
   s = ycalc_popstr (__FUNCTION__);
   if (strcmp (s, r) >  0) ycalc_pushval (__FUNCTION__, TRUE );
   else                    ycalc_pushval (__FUNCTION__, FALSE);
   return;
}



/*====================------------------------------------====================*/
/*===----                       logical operators                      ----===*/
/*====================------------------------------------====================*/
static void  o___LOGICAL_________o () { return; }

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_not          (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, !a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_and          (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, b && a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_or           (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, b || a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_nand         (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, !(b && a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_nor          (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, !(b || a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_xor          (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, (b && !a) || (!b && a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_nxor         (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, !((b && !a) || (!b && a)));
   return;
}



/*====================------------------------------------====================*/
/*===----                         logic functions                      ----===*/
/*====================------------------------------------====================*/
static void  o___LOGIC___________o () { return; }

void    /*-> tbd --------------------------------[ ------ [fv.320.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_if            (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   c = ycalc_popval (__FUNCTION__);
   if (c) ycalc_pushval (__FUNCTION__, b);
   else   ycalc_pushval (__FUNCTION__, a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.23]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_ifs           (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   r = ycalc_popstr (__FUNCTION__);
   s = ycalc_popstr (__FUNCTION__);
   c = ycalc_popval (__FUNCTION__);
   if (c) ycalc_pushstr (__FUNCTION__, s);
   else   ycalc_pushstr (__FUNCTION__, r);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_within        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   c = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   if (c >= b && c <= a) ycalc_pushval (__FUNCTION__, TRUE);
   else                  ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_approx        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   c = ycalc_popval (__FUNCTION__);
   if (c >= (b - a) && c <= (b + a)) ycalc_pushval (__FUNCTION__, TRUE);
   else                              ycalc_pushval (__FUNCTION__, FALSE);
   return;
}


/*====================------------------------------------====================*/
/*===----                    date and time functions                   ----===*/
/*====================------------------------------------====================*/
static void  o___TIME____________o () { return; }

/* >> there are only six logic functions in excel (office.microsoft.com)...
 *     - VALUED         : NOW, TODAY, DATEVALUE, TIMEVALUE
 *     - HELPERS        : YEAR, MONTH, DAY, HOUR, MINUTE, SECOND, WEEKDAY
 *     - left off       : YEARFRAC, WORKDAY, NETWORKDAYS, EOMONTH, EDATE,
 *                        DAYS360
 */

long   time_zone  = 0;

void    /*-> tbd --------------------------------[ ------ [fv.210.000.01]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_now           (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc_pushval (__FUNCTION__, time (NULL));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.020.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_year          (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   char temp[100];
   a = ycalc_popval (__FUNCTION__);
   time_t  xtime = (time_t) a;
   strftime (temp, 100, "%Y", localtime (&xtime));
   ycalc_pushval (__FUNCTION__, atoi (temp));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.020.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_month         (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   char temp[100];
   a = ycalc_popval (__FUNCTION__);
   time_t  xtime = (time_t) a;
   strftime (temp, 100, "%m", localtime (&xtime));
   ycalc_pushval (__FUNCTION__, atoi (temp));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.020.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_day           (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   char temp[100];
   a = ycalc_popval (__FUNCTION__);
   time_t  xtime = (time_t) a;
   strftime (temp, 100, "%d", localtime (&xtime));
   ycalc_pushval (__FUNCTION__, atoi (temp));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.020.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_hour          (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   char temp[100];
   a = ycalc_popval (__FUNCTION__);
   time_t  xtime = (time_t) a;
   strftime (temp, 100, "%H", localtime (&xtime));
   ycalc_pushval (__FUNCTION__, atoi (temp));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.020.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_minute        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   char temp[100];
   a = ycalc_popval (__FUNCTION__);
   time_t  xtime = (time_t) a;
   strftime (temp, 100, "%M", localtime (&xtime));
   ycalc_pushval (__FUNCTION__, atoi (temp));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.020.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_second        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   char temp[100];
   a = ycalc_popval (__FUNCTION__);
   time_t  xtime = (time_t) a;
   strftime (temp, 100, "%S", localtime (&xtime));
   ycalc_pushval (__FUNCTION__, atoi (temp));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.020.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_weekday       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   char temp[100];
   a = ycalc_popval (__FUNCTION__);
   time_t  xtime = (time_t) a;
   strftime (temp, 100, "%w", localtime (&xtime));
   ycalc_pushval (__FUNCTION__, atoi (temp));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.020.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_weeknum       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   char temp[100];
   a = ycalc_popval (__FUNCTION__);
   time_t  xtime = (time_t) a;
   strftime (temp, 100, "%W", localtime (&xtime));
   ycalc_pushval (__FUNCTION__, atoi (temp));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.020.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_daynum        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   char temp[100];
   a = ycalc_popval (__FUNCTION__);
   time_t  xtime = (time_t) a;
   strftime (temp, 100, "%j", localtime (&xtime));
   ycalc_pushval (__FUNCTION__, atoi (temp));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.D80.0A0.A3]*/ /*-[05.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_timevalue     (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(locals)-----------+-----------+-*/
   int         se = 0;
   int         mn = 0;
   int         hr = 0;
   int         mo = 0;
   int         dy = 0;
   int         yr = 0;
   int         rc = 0;
   struct tm  *temp;
   ullong      x_now;
   /*---(set defaults)-------------------*/
   x_now = time (NULL);
   temp = localtime(&x_now);
   se = temp->tm_sec;
   mn = temp->tm_min;
   hr = temp->tm_hour;
   dy = temp->tm_mday;
   mo = temp->tm_mon + 1;
   yr = temp->tm_year + 1900;
   /*---(pop string)---------------------*/
   r   = ycalc_popstr(__FUNCTION__);
   /*---(attempt to read)----------------*/
   rc  = sscanf (r, "%d/%d/%d %d:%d:%d", &mo, &dy, &yr, &hr, &mn, &se);
   if (rc < 6) {
      rc  = sscanf (r, "%d/%d/%d %d:%d", &mo, &dy, &yr, &hr, &mn);
      if (rc < 5) {
         rc  = sscanf (r, "%d/%d/%d", &mo, &dy, &yr);
         if (rc < 4) {
            rc  = sscanf (r, "%d:%d:%d", &hr, &mn, &se);
            if (rc < 3) {
               rc  = sscanf (r, "%d/%d", &mo, &dy);
               if (rc < 2) {
                  rc  = sscanf (r, "%d:%d", &hr, &mn);
                  if (rc < 2) {
                     ycalc_error_set (YCALC_ERROR_EXEC_FMT, NULL);
                     return;
                  }
               }
            }
         }
      }
   }
   /*---(convert)------------------------*/
   temp->tm_sec  = se;
   temp->tm_min  = mn;
   temp->tm_hour = hr;
   temp->tm_mday = dy;
   temp->tm_mon  = mo - 1;
   if      (yr <  70  ) temp->tm_year = 100 + yr;
   else if (yr <  100 ) temp->tm_year = yr;
   else if (yr <  1970) {
      ycalc_error_set (YCALC_ERROR_EXEC_DATE, NULL);
      return;
   }
   else if (yr >  2037) {
      ycalc_error_set (YCALC_ERROR_EXEC_DATE, NULL);
      return;
   }
   else if (yr >= 1900) temp->tm_year = yr - 1900;
   temp->tm_isdst = -1;   /* ignore daylight savings */
   time_t  xtime = mktime(temp);
   /*---(complete)-----------------------*/
   ycalc_pushval (__FUNCTION__, (double) xtime);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.750.050.53]*/ /*-[01.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_date          (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(locals)-----------+-----------+-*/
   struct tm  *temp;
   time_t      x_time;
   ullong      x_now;
   int         yr = 0;
   /*---(set defaults)-------------------*/
   x_now = time (NULL);
   temp = localtime (&x_now);
   /*---(get numbers)--------------------*/
   temp->tm_sec  = 0;
   temp->tm_min  = 0;
   temp->tm_hour = 0;
   temp->tm_mday = ycalc_popval (__FUNCTION__);
   temp->tm_mon  = ycalc_popval (__FUNCTION__) - 1;
   yr            = ycalc_popval (__FUNCTION__);
   if      (yr <  70  ) temp->tm_year = 100 + yr;
   else if (yr <  100 ) temp->tm_year = yr;
   else if (yr <  1970) {
      ycalc_error_set (YCALC_ERROR_EXEC_DATE, NULL);
      return;
   }
   else if (yr >  2037) {
      ycalc_error_set (YCALC_ERROR_EXEC_DATE, NULL);
      return;
   }
   else if (yr >= 1900) temp->tm_year = yr - 1900;
   temp->tm_isdst = -1;  /* ignore daylight savings */
   time_t  xtime = mktime (temp);
   /*---(complete)-----------------------*/
   ycalc_pushval (__FUNCTION__, (double) xtime);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.430.040.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_time          (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(locals)-----------+-----------+-*/
   struct tm  *temp;
   time_t      x_time;
   ullong      x_now;
   /*---(set defaults)-------------------*/
   x_now = time (NULL);
   temp = localtime (&x_now);
   /*---(get numbers)--------------------*/
   temp->tm_sec  = ycalc_popval (__FUNCTION__);
   temp->tm_min  = ycalc_popval (__FUNCTION__);
   temp->tm_hour = ycalc_popval (__FUNCTION__);
   temp->tm_isdst = -1;  /* ignore daylight savings */
   time_t  xtime = mktime (temp);
   /*---(complete)-----------------------*/
   ycalc_pushval (__FUNCTION__, (double) xtime);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.430.030.02]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_datepart      (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(locals)-----------+-----------+-*/
   struct tm  *temp;
   ullong      x_time;
   /*---(set defaults)-------------------*/
   a = ycalc_popval (__FUNCTION__);
   x_time = (time_t) a;
   temp = localtime (&x_time);
   /*---(get numbers)--------------------*/
   temp->tm_sec  = 0;
   temp->tm_min  = 0;
   temp->tm_hour = 0;
   temp->tm_isdst = -1;  /* ignore daylight savings */
   time_t  xtime = mktime(temp);
   /*---(complete)-----------------------*/
   ycalc_pushval (__FUNCTION__, (double) xtime);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.020.03]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_timepart      (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(locals)-----------+-----------+-*/
   struct tm  *temp;
   ullong      x_time;
   /*---(set defaults)-------------------*/
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, b);
   ycalc_datepart ();
   a = ycalc_popval (__FUNCTION__);
   /*---(complete)-----------------------*/
   ycalc_pushval (__FUNCTION__, (b - a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.020.03]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_timezonesec   (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc_pushval (__FUNCTION__, time_zone);
   return;
}



/*====================------------------------------------====================*/
/*===----                     arithmetic operators                     ----===*/
/*====================------------------------------------====================*/
static void  o___BITWISE_________o () { return; }

static unsigned int   x, y;

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_bit_not     (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   m = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, ~ m);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_bit_left    (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   x = ycalc_popval (__FUNCTION__);
   y = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, y << x);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_bit_right   (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   x = ycalc_popval (__FUNCTION__);
   y = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, y >> x);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_bit_and     (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   x = ycalc_popval (__FUNCTION__);
   y = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, x & y);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_bit_or      (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   x = ycalc_popval (__FUNCTION__);
   y = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, x | y);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_bit_xor     (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   x = ycalc_popval (__FUNCTION__);
   n = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, x ^ n);
   return;
}



