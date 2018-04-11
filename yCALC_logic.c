/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"



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


