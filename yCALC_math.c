/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"



static const double DEG2RAD = 3.1415927 / 180.0;
static const double RAD2DEG = 180.0 / 3.1415927;



typedef struct cTRIG  tTRIG;
struct  cTRIG {
   double      si;
   double      co;
   double      ta;
};
static tTRIG   s_trig [3600];

char
ycalc_math_init         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   double      a           =  0.0;
   /*---(load trig table)----------------*/
   for (i = 0; i < 3600; ++i) {
      a = ((double) i) / 10.0;
      s_trig [i].si = sin ((double) a * DEG2RAD);
      s_trig [i].co = cos ((double) a * DEG2RAD);
      s_trig [i].ta = tan ((double) a * DEG2RAD);
      if ( i % 900 == 0)  s_trig[i].ta = 0.0;
   }
   /*---(complete)-----------------------*/
   return 0;
}


/*====================------------------------------------====================*/
/*===----                     arithmetic operators                     ----===*/
/*====================------------------------------------====================*/
static void  o___ARITHMETIC______o () { return; }

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_add         (void)
{
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, b + a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_subtract     (void)
{
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, b - a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_multiply     (void)
{
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, b * a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.000.22]*/ /*-[00.0000.03#.!]-*/ /*-[--.---.---.--]-*/
ycalc_divide       (void)
{
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   if (a != 0)  ycalc_pushval (__FUNCTION__, b / a);
   else         ycalc_pushval (__FUNCTION__, 0);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_modulus      (void)
{
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, ((int) b) % ((int) a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_increment    (void)
{
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, a + 1);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_decrement    (void)
{
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, a - 1);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_unaryminus   (void)
{
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
   int   i = 0;
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   c = b;
   for (i = 1; i <  a; ++i)  c *= b;
   ycalc_pushval (__FUNCTION__, c);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_abs           (void)
{
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, fabs(a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_trunc         (void)
{
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, trunc(a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.330.010.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rtrunc        (void)
{
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
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, round(a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.330.010.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rround        (void)
{
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
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, ceil(a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_floor         (void)
{
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, floor(a));
   return;
}

void    /*-> rand between 0-1 -------------------[ ------ [fv.210.000.01]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rand          (void)
{
   ycalc_pushval (__FUNCTION__, (double) rand());
   return;
}

void    /*-> rand between n-m -------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_randr         (void)
{
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, ((double) rand() / (double) RAND_MAX) * (b - a) + a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_sqrt          (void)
{
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, sqrt(a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_cbrt          (void)
{
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, cbrt(a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_sqr           (void)
{
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, a * a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_cube          (void)
{
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
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, b == a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_notequal      (void)
{
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, b != a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_greater      (void)
{
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, b > a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_lesser       (void)
{
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, b < a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_gequal       (void)
{
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, b >= a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_lequal       (void)
{
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, b <= a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_sequal       (void)
{
   r = ycalc_popstr (__FUNCTION__);
   s = ycalc_popstr (__FUNCTION__);
   if (strcmp (s, r) == 0) ycalc_pushval (__FUNCTION__, TRUE );
   else                    ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_snotequal    (void)
{
   r = ycalc_popstr (__FUNCTION__);
   s = ycalc_popstr (__FUNCTION__);
   if (strcmp (s, r) != 0) ycalc_pushval (__FUNCTION__, TRUE );
   else                    ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_slesser      (void)
{
   r = ycalc_popstr (__FUNCTION__);
   s = ycalc_popstr (__FUNCTION__);
   if (strcmp (s, r) <  0) ycalc_pushval (__FUNCTION__, TRUE );
   else                    ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_sgreater     (void)
{
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
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, !a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_and          (void)
{
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, b && a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_or           (void)
{
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, b || a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_nand         (void)
{
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, !(b && a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_nor          (void)
{
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, !(b || a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_xor          (void)
{
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, (b && !a) || (!b && a));
   return;
}



/*====================------------------------------------====================*/
/*===----                         logic functions                      ----===*/
/*====================------------------------------------====================*/
void  o___LOGIC___________o () { return; }

void    /*-> tbd --------------------------------[ ------ [fv.320.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_if            (void)
{
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
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   c = ycalc_popval (__FUNCTION__);
   if (c >= (b - a) && c <= (b + a)) ycalc_pushval (__FUNCTION__, TRUE);
   else                              ycalc_pushval (__FUNCTION__, FALSE);
   return;
}



/*====================------------------------------------====================*/
/*===----                         trig functions                       ----===*/
/*====================------------------------------------====================*/

void  o___TRIG____________o () { return; }

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.07#.!]-*/ /*-[--.---.---.--]-*/
ycalc_degrees       (void)
{
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, a * RAD2DEG);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_radians       (void)
{
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, a * DEG2RAD);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.01]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_pi            (void)
{
   ycalc_pushval (__FUNCTION__, 3.1415927);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_hypot         (void)
{
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, sqrt(a * a + b * b));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_side          (void)
{
   b = ycalc_popval (__FUNCTION__);
   c = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, sqrt(c * c - b * b));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.010.12]*/ /*-[00.0000.03#.!]-*/ /*-[--.---.---.--]-*/
ycalc_sin           (void)
{
   int i;
   a = ycalc_popval (__FUNCTION__);
   i = round (a * 10.0);
   i = i % 3600;
   if (i < 0)  i = 3600 + i;
   ycalc_pushval (__FUNCTION__, s_trig[i].si);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_sinr          (void)
{
   ycalc_degrees ();
   ycalc_sin     ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.04]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_csc           (void)
{
   ycalc_sin     ();
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, 1);
   ycalc_pushval (__FUNCTION__, a);
   ycalc_divide ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_cscr          (void)
{
   ycalc_degrees ();
   ycalc_csc     ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.010.12]*/ /*-[00.0000.02#.!]-*/ /*-[--.---.---.--]-*/
ycalc_cos           (void)
{
   int i;
   a = ycalc_popval (__FUNCTION__);
   i = round (a * 10.0);
   i = i % 3600;
   if (i < 0)  i = 3600 + i;
   ycalc_pushval (__FUNCTION__, s_trig[i].co);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_cosr          (void)
{
   ycalc_degrees ();
   ycalc_cos     ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.04]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_sec           (void)
{
   ycalc_cos     ();
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, 1);
   ycalc_pushval (__FUNCTION__, a);
   ycalc_divide ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_secr          (void)
{
   ycalc_degrees ();
   ycalc_sec     ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.010.12]*/ /*-[00.0000.02#.!]-*/ /*-[--.---.---.--]-*/
ycalc_tan           (void)
{
   int i;
   a = ycalc_popval (__FUNCTION__);
   i = round (a * 10.0);
   i = i % 3600;
   if (i < 0)  i = 3600 + i;
   ycalc_pushval (__FUNCTION__, s_trig[i].ta);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_tanr          (void)
{
   ycalc_degrees ();
   ycalc_tan     ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.04]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_cot           (void)
{
   ycalc_tan     ();
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, 1);
   ycalc_pushval (__FUNCTION__, a);
   ycalc_divide ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_cotr          (void)
{
   ycalc_degrees ();
   ycalc_cot     ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.03]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_crd           (void)
{
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, a / 2.0);
   ycalc_sin     ();
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, a * 2.0);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_crdr          (void)
{
   ycalc_degrees ();
   ycalc_crd     ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_asin          (void)
{
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, asin(a) * RAD2DEG);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_asinr         (void)
{
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, asin(a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_acos          (void)
{
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, acos(a) * RAD2DEG);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_acosr         (void)
{
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, acos(a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_atan          (void)
{
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, atan(a) * RAD2DEG);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_atanr         (void)
{
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, atan(a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_atan2         (void)
{
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, atan2 (b,a) * RAD2DEG);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_atanr2        (void)
{
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, atan2 (b,a));
   return;
}





