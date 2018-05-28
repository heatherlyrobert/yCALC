/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"



static const double DEG2RAD = 3.1415927 / 180.0;
static const double RAD2DEG = 180.0 / 3.1415927;

#define        TRIG_ZERO      -0.000001
#define        TRIG_MIN        0.00001
#define        TRIG_MAX        9999999



typedef struct cTRIG  tTRIG;
struct  cTRIG {
   double      si;
   double      co;
   double      ta;
};
static tTRIG   s_trig [3600];

char
ycalc_trig_init         (void)
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
      if ( i ==  900)  s_trig[i].ta =  TRIG_MAX;
      if ( i == 2700)  s_trig[i].ta =  TRIG_MAX;
   }
   /*---(complete)-----------------------*/
   return 0;
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




/*====================------------------------------------====================*/
/*===----                         basic six                            ----===*/
/*====================------------------------------------====================*/
void  o___BASIC_6_________o () { return; }

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
   if      (a >= 0 && a <  TRIG_MIN)   ycalc_pushval (__FUNCTION__,  TRIG_MAX);
   else if (a <  0 && a > -TRIG_MIN)   ycalc_pushval (__FUNCTION__, -TRIG_MAX);
   else                                ycalc_pushval (__FUNCTION__, 1 / a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_cotr          (void)
{
   ycalc_degrees ();
   ycalc_cot     ();
   return;
}



/*====================------------------------------------====================*/
/*===----                         extended                             ----===*/
/*====================------------------------------------====================*/
void  o___EXTEND__________o () { return; }

void    /*-> tbd --------------------------------[ ------ [fv.320.000.04]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_sec           (void)
{
   ycalc_cos     ();
   a = ycalc_popval (__FUNCTION__);
   if      (a >  TRIG_ZERO && a <  TRIG_MIN)   ycalc_pushval (__FUNCTION__,  TRIG_MAX);
   else if (a <= 0         && a > -TRIG_MIN)   ycalc_pushval (__FUNCTION__, -TRIG_MAX);
   else                                        ycalc_pushval (__FUNCTION__, 1 / a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_secr          (void)
{
   ycalc_degrees ();
   ycalc_sec     ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.04]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_csc           (void)
{
   ycalc_sin     ();
   a = ycalc_popval (__FUNCTION__);
   if      (a >  TRIG_ZERO && a <  TRIG_MIN)   ycalc_pushval (__FUNCTION__,  TRIG_MAX);
   else if (a <=  0        && a > -TRIG_MIN)   ycalc_pushval (__FUNCTION__, -TRIG_MAX);
   else                                        ycalc_pushval (__FUNCTION__, 1 / a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_cscr          (void)
{
   ycalc_degrees ();
   ycalc_csc     ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.04]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_vsin          (void)
{
   ycalc_cos     ();
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, 1 - a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_vsinr         (void)
{
   ycalc_degrees ();
   ycalc_vsin    ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.04]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_vcos          (void)
{
   ycalc_cos     ();
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, 1 + a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_vcosr         (void)
{
   ycalc_degrees ();
   ycalc_vcos    ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.04]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_csin          (void)
{
   ycalc_sin     ();
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, 1 - a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_csinr         (void)
{
   ycalc_degrees ();
   ycalc_csin    ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.04]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_ccos          (void)
{
   ycalc_sin     ();
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, 1 + a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_ccosr         (void)
{
   ycalc_degrees ();
   ycalc_ccos    ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.04]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_hvsin          (void)
{
   ycalc_vsin    ();
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, a / 2);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_hvsinr         (void)
{
   ycalc_degrees ();
   ycalc_hvsin    ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.04]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_hvcos          (void)
{
   ycalc_vcos    ();
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, a / 2);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_hvcosr         (void)
{
   ycalc_degrees ();
   ycalc_hvcos    ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.04]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_hcsin          (void)
{
   ycalc_csin     ();
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, a / 2);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_hcsinr         (void)
{
   ycalc_degrees ();
   ycalc_hcsin    ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.04]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_hccos          (void)
{
   ycalc_ccos     ();
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, a / 2);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_hccosr         (void)
{
   ycalc_degrees ();
   ycalc_hccos    ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.04]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_xsec          (void)
{
   ycalc_cos     ();
   a = ycalc_popval (__FUNCTION__);
   if      (a >  TRIG_ZERO && a <  TRIG_MIN)   ycalc_pushval (__FUNCTION__,  TRIG_MAX);
   else if (a <= 0         && a > -TRIG_MIN)   ycalc_pushval (__FUNCTION__, -TRIG_MAX);
   else                                        ycalc_pushval (__FUNCTION__, (1 / a) - 1);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_xsecr         (void)
{
   ycalc_degrees ();
   ycalc_xsec    ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.04]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_xcsc          (void)
{
   ycalc_sin     ();
   a = ycalc_popval (__FUNCTION__);
   if      (a > TRIG_ZERO && a <  TRIG_MIN)   ycalc_pushval (__FUNCTION__,  TRIG_MAX);
   else if (a <= 0        && a > -TRIG_MIN)   ycalc_pushval (__FUNCTION__, -TRIG_MAX);
   else                                       ycalc_pushval (__FUNCTION__, (1 / a) - 1);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_xcscr         (void)
{
   ycalc_degrees ();
   ycalc_xcsc    ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.03]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_crd           (void)
{
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, a / 2.0);
   ycalc_sin     ();
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, fabs (a * 2.0));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_crdr          (void)
{
   ycalc_degrees ();
   ycalc_crd     ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.03]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_hcrd          (void)
{
   ycalc_crd     ();
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, a / 2.0);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_hcrdr         (void)
{
   ycalc_degrees ();
   ycalc_hcrd    ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.03]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_scrd          (void)
{
   a = ycalc_popval (__FUNCTION__);
   a = fabs (a);
   while (a > 360)  a -= 360;
   ycalc_pushval (__FUNCTION__, 180 - a);
   ycalc_crd     ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_scrdr         (void)
{
   ycalc_degrees ();
   ycalc_scrd    ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.03]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_vcrd          (void)
{
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, a / 2.0);
   ycalc_cos     ();
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, fabs (a * 2.0));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_vcrdr         (void)
{
   ycalc_degrees ();
   ycalc_vcrd    ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.03]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_hvcrd          (void)
{
   ycalc_vcrd    ();
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, a / 2.0);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_hvcrdr         (void)
{
   ycalc_degrees ();
   ycalc_hvcrd   ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.03]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_sag           (void)
{
   a = ycalc_popval (__FUNCTION__);
   a = fabs (a);
   while (a > 360)  a -= 360;
   ycalc_pushval (__FUNCTION__, a / 2.0);
   ycalc_vsin    ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.03]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_sagr          (void)
{
   ycalc_degrees ();
   ycalc_sag     ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.03]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_apo           (void)
{
   ycalc_sag     ();
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, 1 - a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.03]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_apor          (void)
{
   ycalc_degrees ();
   ycalc_apo     ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.03]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_csg           (void)
{
   ycalc_sag     ();
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, 2 - a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.320.000.03]*/ /*-[00.0000.01#.!]-*/ /*-[--.---.---.--]-*/
ycalc_csgr          (void)
{
   ycalc_degrees ();
   ycalc_csg     ();
   return;
}



/*====================------------------------------------====================*/
/*===----                          inverse                             ----===*/
/*====================------------------------------------====================*/
void  o___INVERSE_________o () { return; }

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_asin          (void)
{
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, asin(a) * RAD2DEG);
   ycalc_pushval (__FUNCTION__, 3);
   ycalc_rround ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_asinr         (void)
{
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, asin(a));
   ycalc_pushval (__FUNCTION__, 3);
   ycalc_rround ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_acos          (void)
{
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, acos(a) * RAD2DEG);
   ycalc_pushval (__FUNCTION__, 3);
   ycalc_rround ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_acosr         (void)
{
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, acos(a));
   ycalc_pushval (__FUNCTION__, 3);
   ycalc_rround ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_atan          (void)
{
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, atan(a) * RAD2DEG);
   ycalc_pushval (__FUNCTION__, 3);
   ycalc_rround ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_atanr         (void)
{
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, atan(a));
   ycalc_pushval (__FUNCTION__, 3);
   ycalc_rround ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_atan2         (void)
{
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, atan2 (b,a) * RAD2DEG);
   ycalc_pushval (__FUNCTION__, 3);
   ycalc_rround ();
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_atanr2        (void)
{
   a = ycalc_popval (__FUNCTION__);
   b = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, atan2 (b,a));
   ycalc_pushval (__FUNCTION__, 3);
   ycalc_rround ();
   return;
}

