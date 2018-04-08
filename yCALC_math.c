/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"





/*====================------------------------------------====================*/
/*===----                     arithmetic operators                     ----===*/
/*====================------------------------------------====================*/
static void  o___ARITHMETIC______o () { return; }

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_add         (void)
{
   a = yCALC_popval (__FUNCTION__);
   b = yCALC_popval (__FUNCTION__);
   yCALC_pushval (__FUNCTION__, b + a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_subtract     (void)
{
   a = yCALC_popval (__FUNCTION__);
   b = yCALC_popval (__FUNCTION__);
   yCALC_pushval (__FUNCTION__, b - a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_multiply     (void)
{
   a = yCALC_popval (__FUNCTION__);
   b = yCALC_popval (__FUNCTION__);
   yCALC_pushval (__FUNCTION__, b * a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.000.22]*/ /*-[00.0000.03#.!]-*/ /*-[--.---.---.--]-*/
yCALC_divide       (void)
{
   a = yCALC_popval (__FUNCTION__);
   b = yCALC_popval (__FUNCTION__);
   if (a != 0)  yCALC_pushval (__FUNCTION__, b / a);
   else         yCALC_pushval (__FUNCTION__, 0);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_modulus      (void)
{
   a = yCALC_popval (__FUNCTION__);
   b = yCALC_popval (__FUNCTION__);
   yCALC_pushval (__FUNCTION__, ((int) b) % ((int) a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_increment    (void)
{
   a = yCALC_popval (__FUNCTION__);
   yCALC_pushval (__FUNCTION__, a + 1);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_decrement    (void)
{
   a = yCALC_popval (__FUNCTION__);
   yCALC_pushval (__FUNCTION__, a - 1);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_unaryminus   (void)
{
   a = yCALC_popval (__FUNCTION__);
   yCALC_pushval (__FUNCTION__, 0 - a);
   return;
}




/*====================------------------------------------====================*/
/*===----                     mathmatical functions                    ----===*/
/*====================------------------------------------====================*/
static void  o___MATH____________o () { return; }

void    /*-> tbd --------------------------------[ ------ [fv.320.010.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_power        (void)
{
   int   i = 0;
   a = yCALC_popval (__FUNCTION__);
   b = yCALC_popval (__FUNCTION__);
   c = b;
   for (i = 1; i <  a; ++i)  c *= b;
   yCALC_pushval (__FUNCTION__, c);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_abs           (void)
{
   a = yCALC_popval (__FUNCTION__);
   yCALC_pushval (__FUNCTION__, fabs(a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_trunc         (void)
{
   a = yCALC_popval (__FUNCTION__);
   yCALC_pushval (__FUNCTION__, trunc(a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.330.010.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_rtrunc        (void)
{
   int i = 0;
   n = yCALC_popval (__FUNCTION__);
   a = yCALC_popval (__FUNCTION__);
   for (i = 0; i < n; ++i)  a *= 10;
   a = trunc (a);
   for (i = 0; i < n; ++i)  a /= 10;
   yCALC_pushval (__FUNCTION__, a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_round         (void)
{
   a = yCALC_popval (__FUNCTION__);
   yCALC_pushval (__FUNCTION__, round(a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.330.010.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_rround        (void)
{
   int i = 0;
   n = yCALC_popval (__FUNCTION__);
   a = yCALC_popval (__FUNCTION__);
   for (i = 0; i < n; ++i)  a *= 10;
   a = round (a);
   for (i = 0; i < n; ++i)  a /= 10;
   yCALC_pushval (__FUNCTION__, a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_ceiling       (void)
{
   a = yCALC_popval (__FUNCTION__);
   yCALC_pushval (__FUNCTION__, ceil(a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_floor         (void)
{
   a = yCALC_popval (__FUNCTION__);
   yCALC_pushval (__FUNCTION__, floor(a));
   return;
}

void    /*-> rand between 0-1 -------------------[ ------ [fv.210.000.01]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_rand          (void)
{
   yCALC_pushval (__FUNCTION__, (double) rand());
   return;
}

void    /*-> rand between n-m -------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_randr         (void)
{
   a = yCALC_popval (__FUNCTION__);
   b = yCALC_popval (__FUNCTION__);
   yCALC_pushval (__FUNCTION__, ((double) rand() / (double) RAND_MAX) * (b - a) + a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_sqrt          (void)
{
   a = yCALC_popval (__FUNCTION__);
   yCALC_pushval (__FUNCTION__, sqrt(a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_cbrt          (void)
{
   a = yCALC_popval (__FUNCTION__);
   yCALC_pushval (__FUNCTION__, cbrt(a));
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_sqr           (void)
{
   a = yCALC_popval (__FUNCTION__);
   yCALC_pushval (__FUNCTION__, a * a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_cube          (void)
{
   a = yCALC_popval (__FUNCTION__);
   yCALC_pushval (__FUNCTION__, a * a * a);
   return;
}
