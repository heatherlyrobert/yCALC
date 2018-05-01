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
      if ( i % 900 == 0)  s_trig[i].ta = 0.0;
   }
   /*---(complete)-----------------------*/
   return 0;
}

#define        TRIG_MIN        0.00001
#define        TRIG_MAX        9999999



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
   if      (a >= 0 && a <  TRIG_MIN)   ycalc_pushval (__FUNCTION__,  TRIG_MAX);
   else if (a <  0 && a > -TRIG_MIN)   ycalc_pushval (__FUNCTION__, -TRIG_MAX);
   else                                ycalc_pushval (__FUNCTION__, 1 / a);
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
   if      (a >= 0 && a <  TRIG_MIN)   ycalc_pushval (__FUNCTION__,  TRIG_MAX);
   else if (a <  0 && a > -TRIG_MIN)   ycalc_pushval (__FUNCTION__, -TRIG_MAX);
   else                                ycalc_pushval (__FUNCTION__, 1 / a);
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
   ycalc_cos     ();
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, (1 - a) / 2);
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
   ycalc_cos     ();
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, (1 + a) / 2);
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
   ycalc_sin     ();
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, (1 - a) / 2);
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
   ycalc_sin     ();
   a = ycalc_popval (__FUNCTION__);
   ycalc_pushval (__FUNCTION__, (1 + a) / 2);
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
   if      (a >= 0 && a <  TRIG_MIN)   ycalc_pushval (__FUNCTION__,  TRIG_MAX);
   else if (a <  0 && a > -TRIG_MIN)   ycalc_pushval (__FUNCTION__, -TRIG_MAX);
   else                                ycalc_pushval (__FUNCTION__, (1 / a) - 1);
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
   if      (a >= 0 && a <  TRIG_MIN)   ycalc_pushval (__FUNCTION__,  TRIG_MAX);
   else if (a <  0 && a > -TRIG_MIN)   ycalc_pushval (__FUNCTION__, -TRIG_MAX);
   else                                ycalc_pushval (__FUNCTION__, (1 / a) - 1);
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



/*====================------------------------------------====================*/
/*===----                          inverse                             ----===*/
/*====================------------------------------------====================*/
void  o___INVERSE_________o () { return; }

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


