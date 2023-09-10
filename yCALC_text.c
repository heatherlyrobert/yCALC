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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   s = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   if (s == NULL)  s = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   ystrlcpy (t, s,   LEN_RECD);
   ystrlcat (t, r,   LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, t);
   /*---(clean up)-----------------------*/
   free (r);
   free (s);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.530.000.22]*/ /*-[20.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_concatspace  (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   s = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   if (s == NULL)  s = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   ystrlcpy (t, s,   LEN_RECD);
   ystrlcat (t, " ", LEN_RECD);
   ystrlcat (t, r,   LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, t);
   /*---(clean up)-----------------------*/
   free (r);
   free (s);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.530.000.32]*/ /*-[21.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_concatcomma  (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   s = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   if (s == NULL)  s = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   ystrlcpy (t, s,   LEN_RECD);
   if (r [0] != '\0') {
      ystrlcat (t, ",", LEN_RECD);
      ystrlcat (t, r,   LEN_RECD);
   }
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, t);
   /*---(clean up)-----------------------*/
   free (r);
   free (s);
   /*---(complete)-----------------------*/
   return;
}

void
ycalc_remove       (void)
{
   int         l1          =    0;
   int         l2          =    0;
   char       *p           = NULL;
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   s = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   if (s == NULL)  s = strndup (g_nada, LEN_RECD);
   /*---(find substr)--------------------*/
   p = strstr (s, r);
   if (p == NULL) strcpy (t, s);
   else {
      l1 = p - s;
      l2 = strlen (r);
      sprintf (t, "%*.*s%s", l1, l1, s, p + l2);
   }
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, t);
   /*---(clean up)-----------------------*/
   free (r);
   free (s);
   /*---(complete)-----------------------*/
   return;
}

void
ycalc_list         (void)
{
   int         l           =    0;
   char        p           =    0;
   char        q           =    0;
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   a = ycalc_popval (__FUNCTION__);
   DEBUG_YCALC   yLOG_value   ("a"         , a);
   s = ycalc_popstr (__FUNCTION__);
   DEBUG_YCALC   yLOG_info    ("s"         , s);
   /*---(defense)------------------------*/
   if (s == NULL)  s = strndup (g_nada, LEN_RECD);
   /*---(find)---------------------------*/
   l = strlen (s);
   p = ystrldpos (s + 1, ',', a    , LEN_RECD);
   DEBUG_YCALC   yLOG_value   ("p"         , p);
   if (p < 0) {
      DEBUG_YCALC   yLOG_note    ("delimiter never found");
      if (a == 0 && l > 0)  ystrlcpy (t, s, LEN_RECD);
      else                  strcpy (t, "");
   } else {
      DEBUG_YCALC   yLOG_note    ("found delimiter");
      ++p;
      ++p;
      ystrlcpy  (t, s + p, LEN_RECD);
      DEBUG_YCALC   yLOG_info    ("t"         , t);
      q = ystrldpos (t, ',', 0, LEN_RECD);
      DEBUG_YCALC   yLOG_value   ("q"         , q);
      if (q >= 0)  t [q] = '\0';
   }
   ystrltrim (t, ySTR_BOTH, LEN_RECD);
   DEBUG_YCALC   yLOG_info    ("t"         , t);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, t);
   /*---(clean up)-----------------------*/
   free (s);
   /*---(complete)-----------------------*/
   return;
}

void
ycalc__parse        (void)
{
   int         l           =    0;
   char        d           =  ' ';
   short       p           =    0;
   char        q           =    0;
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   DEBUG_YCALC   yLOG_info    ("r"         , r);
   a = ycalc_popval (__FUNCTION__);
   DEBUG_YCALC   yLOG_value   ("a"         , a);
   s = ycalc_popstr (__FUNCTION__);
   DEBUG_YCALC   yLOG_info    ("s"         , s);
   /*---(defense)------------------------*/
   if (s == NULL)  s = strndup (g_nada, LEN_RECD);
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(find)---------------------------*/
   if (r [0] != '\0')  d = r [0];
   DEBUG_YCALC   yLOG_char    ("d"         , d);
   l = strlen (s);
   p = ystrldpos (s, d, a    , LEN_RECD);
   DEBUG_YCALC   yLOG_value   ("p"         , p);
   if (p < 0) {
      DEBUG_YCALC   yLOG_note    ("delimiter never found");
      if (a == 0 && l > 0)  ystrlcpy (t, s, LEN_RECD);
      else                  strcpy (t, "");
   } else {
      DEBUG_YCALC   yLOG_note    ("found delimiter");
      while (s [++p] == d) ;
      ystrlcpy  (t, s + p, LEN_RECD);
      DEBUG_YCALC   yLOG_info    ("t"         , t);
      q = ystrldpos (t, d, 0, LEN_RECD);
      DEBUG_YCALC   yLOG_value   ("q"         , q);
      if (q >= 0)  t [q] = '\0';
   }
   ystrltrim (t, ySTR_BOTH, LEN_RECD);
   DEBUG_YCALC   yLOG_info    ("t"         , t);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, t);
   /*---(clean up)-----------------------*/
   free (s);
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void
ycalc_parse        (void)
{
   ycalc_pushstr (__FUNCTION__, "§");
   ycalc__parse  ();
   return 0;
}

void
ycalc_word         (void)
{
   ycalc_pushstr (__FUNCTION__, " ");
   ycalc__parse  ();
   return 0;
}

void
ycalc_mask         (void)
{
   int         i           =    0;
   int         l           =    0;
   char       *p           = NULL;
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   s = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   if (s == NULL)  s = strndup (g_nada, LEN_RECD);
   /*---(mask)---------------------------*/
   ystrlcpy (t, s, LEN_RECD);
   l = strlen (t);
   for (i = 0; i < l; ++i) {
      p = strchr (r, t [i]);
      if (p != NULL)  t [i] = '°';
   }
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, t);
   /*---(clean up)-----------------------*/
   free (r);
   free (s);
   /*---(complete)-----------------------*/
   return;
}

void
ycalc_redact       (void)
{
   int         i           =    0;
   int         l           =    0;
   char       *p           = NULL;
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   s = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   if (s == NULL)  s = strndup (g_nada, LEN_RECD);
   /*---(redact)-------------------------*/
   ystrlcpy (t, s, LEN_RECD);
   l = strlen (t);
   for (i = 0; i < l; ++i) {
      p = strchr (r, t [i]);
      if (p == NULL)  t [i] = '°';
   }
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, t);
   /*---(clean up)-----------------------*/
   free (r);
   free (s);
   /*---(complete)-----------------------*/
   return;
}

void
ycalc_substr       (void)
{
   char        rc          =    0;
   char       *p           = NULL;
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   DEBUG_YCALC   yLOG_info    ("r"         , r);
   s = ycalc_popstr (__FUNCTION__);
   DEBUG_YCALC   yLOG_info    ("s"         , s);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   if (s == NULL)  s = strndup (g_nada, LEN_RECD);
   /*---(find)---------------------------*/
   if (strstr (s, r) != NULL)  rc = 1;
   /*---(return result)------------------*/
   ycalc_pushval (__FUNCTION__, rc);
   /*---(clean up)-----------------------*/
   free (r);
   free (s);
   /*---(complete)-----------------------*/
   return;
}

void
ycalc_listitem     (void)
{
   char        rc          =    0;
   char       *p           = NULL;
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   DEBUG_YCALC   yLOG_info    ("r"         , r);
   s = ycalc_popstr (__FUNCTION__);
   DEBUG_YCALC   yLOG_info    ("s"         , s);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   if (s == NULL)  s = strndup (g_nada, LEN_RECD);
   /*---(find)---------------------------*/
   sprintf (t, ",%s,", r);
   DEBUG_YCALC   yLOG_info    ("t"         , t);
   if (strstr (s, t) != NULL)  rc = 1;
   /*---(return result)------------------*/
   ycalc_pushval (__FUNCTION__, rc);
   /*---(clean up)-----------------------*/
   free (r);
   free (s);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.430.010.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_lower        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   ystrlcpy (t, r, LEN_RECD);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   len = strlen(r);
   ystrlcpy (t, r, LEN_RECD);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   n = ycalc_popval (__FUNCTION__);
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   if (n     <  0  )  n = 0;
   /*---(process)------------------------*/
   len = strlen (r);
   ystrlcpy (t, r, LEN_RECD);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
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

void    /*-> tbd --------------------------------[ ------ [fv.750.010.73]*/ /*-[12.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_beg          (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   n = ycalc_popval (__FUNCTION__);
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   len = strlen (r);
   if (n < 0)     n = 0;
   if (n >= len)  n = len - 1;
   /*---(process)------------------------*/
   int i;
   for (i = n; i <= len; ++i) t[i - n] = r[i];
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, t);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.750.010.73]*/ /*-[12.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_at           (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   n = ycalc_popval (__FUNCTION__);
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   len = strlen (r);
   if (n < 0)     n = 0;
   if (n >= len)  n = len - 1;
   /*---(process)------------------------*/
   t [0] = r [n];
   t [1] = '\0';
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   ystrltrim (r, ySTR_BOTH, LEN_RECD);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   ystrltrim (r, ySTR_HEAD, LEN_RECD);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   ystrltrim (r, ySTR_TAIL, LEN_RECD);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   ystrltrim (r, ySTR_SINGLE, LEN_RECD);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   ystrltrim (r, ySTR_EVERY, LEN_RECD);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   ystrltrim (r, ySTR_MAX, LEN_RECD);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_PRINT);
   DEBUG_YCALC   yLOG_point   ("r"         , r);
   DEBUG_YCALC   yLOG_info    ("r"         , r);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   DEBUG_YCALC   yLOG_info    ("r"         , r);
   ystrltrim (r, ySTR_BOTH, LEN_RECD);
   DEBUG_YCALC   yLOG_info    ("r"         , r);
   ycalc_pushstr (__FUNCTION__, r);
   DEBUG_YCALC   yLOG_note    ("pushed");
   /*---(clean up)-----------------------*/
   free (r);
   DEBUG_YCALC   yLOG_note    ("freed");
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_wrap         (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   DEBUG_YCALC   yLOG_info    ("r"         , r);
   sprintf (t, "å%sæ", r);
   DEBUG_YCALC   yLOG_info    ("t"         , t);
   ycalc_pushstr (__FUNCTION__, t);
   /*---(clean up)-----------------------*/
   free (r);
   DEBUG_YCALC   yLOG_note    ("freed");
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.010.22]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_printnum     (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*> int         x_len       = 0;                                                   <*/
   char        rc          =    0;
   /*---(get arguments)------------------*/
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_PRINT);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   ystrltrim (r, ySTR_BOTH, LEN_RECD);
   rc = ystrl2num (r, &a, LEN_RECD);
   if (rc < 0 ) ycalc_error_set (YCALC_ERROR_EXEC_NUM , NULL);
   else         ycalc_pushval (__FUNCTION__, a);
   /*> x_len = strlen (r);                                                            <* 
    *> if (r [x_len - 1] == '>')  r [x_len - 1] = '\0';                               <* 
    *> ycalc_pushval (__FUNCTION__, atof (r));                                        <*/
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.530.000.43]*/ /*-[11.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_lpad         (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   n = ycalc_popval (__FUNCTION__);
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   if (n     <  0  )  n = 0;
   /*---(process)------------------------*/
   ystrlpad (r, t, '?', '>', n);
   ycalc_pushstr (__FUNCTION__, t);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.530.000.43]*/ /*-[11.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rpad         (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   n = ycalc_popval (__FUNCTION__);
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   if (n     <  0  )  n = 0;
   /*---(process)------------------------*/
   ystrlpad (r, t, '?', '<', n);
   ycalc_pushstr (__FUNCTION__, t);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.630.000.43]*/ /*-[11.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_lppad        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   n = ycalc_popval (__FUNCTION__);
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_PRINT);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   if (n     <  0  )  n = 0;
   /*---(process)------------------------*/
   ystrltrim (r, ySTR_BOTH, LEN_RECD);
   ystrlpad (r, t, '?', '>', n);
   ycalc_pushstr (__FUNCTION__, t);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.630.000.43]*/ /*-[11.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rppad        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   n = ycalc_popval (__FUNCTION__);
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_PRINT);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   if (n     <  0  )  n = 0;
   /*---(process)------------------------*/
   ystrltrim (r, ySTR_BOTH, LEN_RECD);
   ystrlpad (r, t, '?', '<', n);
   ycalc_pushstr (__FUNCTION__, t);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_value        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
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
ycalc_unhex        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   sprintf (t, "õ%s", r);
   ystrl2hex (t, &a, 0);
   /*---(return result)------------------*/
   ycalc_pushval (__FUNCTION__, a);
   /*---(clean up)-----------------------*/
   free (r);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_hex          (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   a = ycalc_popval (__FUNCTION__);
   /*---(process)------------------------*/
   ystrl4hex (a, t, 1, 'x', LEN_LABEL);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, t + 1);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_pint         (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   a = ycalc_popval (__FUNCTION__);
   /*---(process)------------------------*/
   sprintf (t, "%d", (int) a);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, t);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_pintf        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   b = ycalc_popval (__FUNCTION__);
   a = ycalc_popval (__FUNCTION__);
   /*---(process)------------------------*/
   sprintf (t, "%*d", (int) b, (int) a);
   if ((int) a == 0)  t [(int) b -1] = '·';
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, t);
   /*---(complete)-----------------------*/
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.420.000.12]*/ /*-[10.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_salpha       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   ystrlmark  (r, ySTR_ALPHA, LEN_RECD);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   ystrlclean (r, ySTR_ALPHA, LEN_RECD);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   ystrlmark  (r, ySTR_ALNUM, LEN_RECD);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   ystrlclean (r, ySTR_ALNUM, LEN_RECD);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   ystrlmark  (r, ySTR_BASIC, LEN_RECD);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   ystrlclean (r, ySTR_BASIC, LEN_RECD);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   ystrlmark  (r, ySTR_WRITE, LEN_RECD);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   ystrlclean (r, ySTR_WRITE, LEN_RECD);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   ystrlmark  (r, ySTR_EXTEN, LEN_RECD);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   ystrlclean (r, ySTR_EXTEN, LEN_RECD);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   ystrlmark  (r, ySTR_PRINT, LEN_RECD);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   ystrlclean (r, ySTR_PRINT, LEN_RECD);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   ystrlmark  (r, ySTR_SEVEN, LEN_RECD);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(get arguments)------------------*/
   r = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (r == NULL)  r = strndup (g_nada, LEN_RECD);
   /*---(process)------------------------*/
   ystrlclean (r, ySTR_SEVEN, LEN_RECD);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
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
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
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
   ystrlrepl (r, q, s, n, LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, r);
   /*---(clean up)-----------------------*/
   free (q);
   free (r);
   free (s);
   /*---(complete)-----------------------*/
   return;
}

void
ycalc_ditto       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   int         i           =   0;
   /*---(get arguments)------------------*/
   n = ycalc_popval (__FUNCTION__);
   s = ycalc_popstr (__FUNCTION__);
   /*---(defense)------------------------*/
   if (s == NULL)  s = strndup (g_nada, LEN_RECD);
   if (n     <  0  )  n = 0;
   strcpy (t, "");
   for (i = 0; i < n; ++i)   ystrlcat (t, s, LEN_RECD);
   /*---(return result)------------------*/
   ycalc_pushstr (__FUNCTION__, t);
   /*---(clean up)-----------------------*/
   free (s);
   /*---(complete)-----------------------*/
   return;
}
