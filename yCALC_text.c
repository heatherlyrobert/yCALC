/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"




/*====================------------------------------------====================*/
/*===----                       string functions                       ----===*/
/*====================------------------------------------====================*/
static void  o___STRING__________o () { return; }

void    /*-> tbd --------------------------------[ ------ [fv.530.000.22]*/ /*-[20.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_concat       (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr(__FUNCTION__);
   s = ycalc_popstr(__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   if (s == NULL)  s = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   strncpy (t, s,   LEN_RECD);
   strncat (t, r,   LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, t);
   /*---(clean up)-----------------------*/
   free (r);
   free (s);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.530.000.32]*/ /*-[21.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_concatplus   (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr(__FUNCTION__);
   s = ycalc_popstr(__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   if (s == NULL)  s = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   strncpy (t, s,   LEN_RECD);
   if (r [0] != '\0') {
      strncat (t, " ", LEN_RECD);
      strncat (t, r,   LEN_RECD);
   }
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, t);
   /*---(clean up)-----------------------*/
   free (r);
   free (s);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.430.010.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_lower        (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr(__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   strncpy (t, r, LEN_RECD);
   len = strlen(t);
   int i;
   for (i = 0; i < len; ++i) t[i] = tolower(t[i]);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, t);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.430.010.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_upper        (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr(__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   len = strlen(r);
   strncpy (t, r, LEN_RECD);
   int i;
   for (i = 0; i < len; ++i) t[i] = toupper(t[i]);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, t);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_char         (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   n = ycalc_popval (__FUNCTION__);
   /*---(defense)------------------------*/
   if (n     <  G_KEY_SPACE)  n = '#';
   if (n     >= G_KEY_BS   )  n = '#';
   /*---(process)------------------------*/
   sprintf (t, "%c", n);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, t);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.310.000.12]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_code         (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushval (__FUNCTION__, r[0]);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_len          (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   len = strlen (r);
   /*---(return result)------------------*/
   ycalc_pushval (__FUNCTION__, (double) len);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.530.000.33]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_left         (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   n = ycalc_popval (__FUNCTION__);
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   if (n     <  0  )  n = 0;
   /*---(process)------------------------*/
   len = strlen (r);
   strncpy (t, r, LEN_RECD);
   if (n >= len)  n = len;
   t[n] = '\0';
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, t);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.540.010.33]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_right        (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   n = ycalc_popval (__FUNCTION__);
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   if (n     <  0  )  n = 0;
   /*---(process)------------------------*/
   len = strlen (r);
   if (n >  len)  n = len;
   m = len - n;
   int i;
   for (i = m; i < len; ++i) t[i - m] = r[i];
   t [n] = '\0';
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, t);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.750.010.73]*/ /*-[12.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_mid          (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   m = ycalc_popval (__FUNCTION__);
   n = ycalc_popval (__FUNCTION__);
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   if (m     <  0  )  m = 0;
   len = strlen (r);
   if (n     <  0  ) {
      if (n + m <  0) {
         n = 0;
         m = 0;
      } else {
         m = n + m;
         n = 0;
      }
   }
   /*---(process)------------------------*/
   if (n     >= len)  n = len;
   if (n + m >= len)  m = len - n;
   int i;
   for (i = n; i < n + m; ++i) t[i - n] = r[i];
   t [m] = '\0';
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, t);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

/*> #define    ySTR_HEAD      'h'                                                     <* 
 *> #define    ySTR_TAIL      't'                                                     <* 
 *> #define    ySTR_BOTH      'b'                                                     <* 
 *> #define    ySTR_EVERY     'e'                                                     <* 
 *> #define    ySTR_COMPRESS  'c'                                                     <*/

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_trim         (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   strltrim (r, ySTR_BOTH, LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, r);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_ltrim        (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   strltrim (r, ySTR_HEAD, LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, r);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rtrim        (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   strltrim (r, ySTR_TAIL, LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, r);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_strim        (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   strltrim (r, ySTR_SINGLE, LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, r);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_etrim        (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   strltrim (r, ySTR_EVERY, LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, r);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_mtrim        (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   strltrim (r, ySTR_MAX, LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, r);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_printstr     (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_PRINT);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   strltrim (r, ySTR_BOTH, LEN_RECD);
   ycalc_pushstr (__FUNCTION__, r);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.010.22]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_printnum     (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   int         x_len       = 0;
   /*---(get arguments)------------------*/
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_PRINT);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   strltrim (r, ySTR_BOTH, LEN_RECD);
   x_len = strlen (r);
   if (r [x_len - 1] == '>')  r [x_len - 1] = '\0';
   ycalc_pushval (__FUNCTION__, atof (r));
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.530.000.43]*/ /*-[11.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_lpad         (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   n = ycalc_popval (__FUNCTION__);
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   if (n     <  0  )  n = 0;
   /*---(process)------------------------*/
   strlpad (r, t, ' ', '>', n);
   ycalc_pushstr (__FUNCTION__, t);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.530.000.43]*/ /*-[11.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rpad         (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   n = ycalc_popval (__FUNCTION__);
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   if (n     <  0  )  n = 0;
   /*---(process)------------------------*/
   strlpad (r, t, ' ', '<', n);
   ycalc_pushstr (__FUNCTION__, t);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.630.000.43]*/ /*-[11.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_lppad        (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   n = ycalc_popval (__FUNCTION__);
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_PRINT);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   if (n     <  0  )  n = 0;
   /*---(process)------------------------*/
   strltrim (r, ySTR_BOTH, LEN_RECD);
   strlpad (r, t, ' ', '>', n);
   ycalc_pushstr (__FUNCTION__, t);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.630.000.43]*/ /*-[11.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rppad        (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   n = ycalc_popval (__FUNCTION__);
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_PRINT);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   if (n     <  0  )  n = 0;
   /*---(process)------------------------*/
   strltrim (r, ySTR_BOTH, LEN_RECD);
   strlpad (r, t, ' ', '<', n);
   ycalc_pushstr (__FUNCTION__, t);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_value        (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   a = atof (r);
   /*---(return result)------------------*/
   ycalc_pushval (__FUNCTION__, a);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_salpha       (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   strlclean (r, ySTR_ALPHA, '-', LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, r);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_salphac      (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   strlclean (r, ySTR_ALPHA, 'y', LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, r);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_salnum       (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   strlclean (r, ySTR_ALNUM, '-', LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, r);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_salnumc      (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   strlclean (r, ySTR_ALNUM, 'y', LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, r);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_sbasic       (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   strlclean (r, ySTR_BASIC, '-', LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, r);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_sbasicc      (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   strlclean (r, ySTR_BASIC, 'y', LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, r);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_swrite       (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   strlclean (r, ySTR_WRITE, '-', LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, r);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_swritec      (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   strlclean (r, ySTR_WRITE, 'y', LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, r);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_sexten       (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   strlclean (r, ySTR_EXTEN, '-', LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, r);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_sextenc      (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   strlclean (r, ySTR_EXTEN, 'y', LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, r);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_sprint       (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   strlclean (r, ySTR_PRINT, '-', LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, r);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_sprintc      (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   strlclean (r, ySTR_PRINT, 'y', LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, r);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_sseven       (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   strlclean (r, ySTR_SEVEN, '-', LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, r);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_ssevenc      (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   strlclean (r, ySTR_SEVEN, 'y', LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, r);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.530.010.42]*/ /*-[20.0000.003.!]-*/ /*-[--.---.---.--]-*/
ycalc_find         (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   char       *rcp         = NULL;
   /*---(get arguments)------------------*/
   q = ycalc_popstr (__FUNCTION__);
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   if (q == NULL)  q = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   rcp = strstr (r, q);
   /*---(return result)------------------*/
   if (rcp == NULL)   ycalc_pushval (__FUNCTION__, -1);
   else               ycalc_pushval (__FUNCTION__, (int) (rcp - r));
   /*---(clean up)-----------------------*/
   free (q);
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.530.000.43]*/ /*-[30.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_replace      (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   n = ycalc_popval (__FUNCTION__);
   s = ycalc_popstr (__FUNCTION__);
   q = ycalc_popstr (__FUNCTION__);
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   if (q == NULL)  q = strndup (g_nada, LEN_RECD);
   if (s == NULL)  s = strndup (g_nada, LEN_RECD);
   if (n     <  0  )  n = 0;
   /*---(process)------------------------*/
   strlrepl (r, q, s, n, LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, r);
   /*---(clean up)-----------------------*/
   free (q);
   free (r);
   free (s);
   /*---(complete)-----------------------*/
   return;
}
