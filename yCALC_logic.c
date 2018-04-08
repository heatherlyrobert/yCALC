/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"



/*====================------------------------------------====================*/
/*===----                     relational operators                     ----===*/
/*====================------------------------------------====================*/
static void  o___RELATIONAL______o () { return; }

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_equal        (void)
{
   a = yCALC_popval (__FUNCTION__);
   b = yCALC_popval (__FUNCTION__);
   yCALC_pushval (__FUNCTION__, b == a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_notequal      (void)
{
   a = yCALC_popval (__FUNCTION__);
   b = yCALC_popval (__FUNCTION__);
   yCALC_pushval (__FUNCTION__, b != a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_greater      (void)
{
   a = yCALC_popval (__FUNCTION__);
   b = yCALC_popval (__FUNCTION__);
   yCALC_pushval (__FUNCTION__, b > a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_lesser       (void)
{
   a = yCALC_popval (__FUNCTION__);
   b = yCALC_popval (__FUNCTION__);
   yCALC_pushval (__FUNCTION__, b < a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_gequal       (void)
{
   a = yCALC_popval (__FUNCTION__);
   b = yCALC_popval (__FUNCTION__);
   yCALC_pushval (__FUNCTION__, b >= a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_lequal       (void)
{
   a = yCALC_popval (__FUNCTION__);
   b = yCALC_popval (__FUNCTION__);
   yCALC_pushval (__FUNCTION__, b <= a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_sequal       (void)
{
   r = yCALC_popstr (__FUNCTION__, ++s_narg);
   s = yCALC_popstr (__FUNCTION__, ++s_narg);
   if (strcmp (s, r) == 0) yCALC_pushval (__FUNCTION__, TRUE );
   else                    yCALC_pushval (__FUNCTION__, FALSE);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_snotequal    (void)
{
   r = yCALC_popstr (__FUNCTION__, ++s_narg);
   s = yCALC_popstr (__FUNCTION__, ++s_narg);
   if (strcmp (s, r) != 0) yCALC_pushval (__FUNCTION__, TRUE );
   else                    yCALC_pushval (__FUNCTION__, FALSE);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_slesser      (void)
{
   r = yCALC_popstr (__FUNCTION__, ++s_narg);
   s = yCALC_popstr (__FUNCTION__, ++s_narg);
   if (strcmp (s, r) <  0) yCALC_pushval (__FUNCTION__, TRUE );
   else                    yCALC_pushval (__FUNCTION__, FALSE);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_sgreater     (void)
{
   r = yCALC_popstr (__FUNCTION__, ++s_narg);
   s = yCALC_popstr (__FUNCTION__, ++s_narg);
   if (strcmp (s, r) >  0) yCALC_pushval (__FUNCTION__, TRUE );
   else                    yCALC_pushval (__FUNCTION__, FALSE);
   return;
}



/*====================------------------------------------====================*/
/*===----                       logical operators                      ----===*/
/*====================------------------------------------====================*/
static void  o___LOGICAL_________o () { return; }

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_not          (void)
{
   a = yCALC_popval (__FUNCTION__);
   yCALC_pushval (__FUNCTION__, !a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_and          (void)
{
   a = yCALC_popval (__FUNCTION__);
   b = yCALC_popval (__FUNCTION__);
   yCALC_pushval (__FUNCTION__, b && a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_or           (void)
{
   a = yCALC_popval (__FUNCTION__);
   b = yCALC_popval (__FUNCTION__);
   yCALC_pushval (__FUNCTION__, b || a);
   return;
}


