/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"




/*====================------------------------------------====================*/
/*===----                         mock system                          ----===*/
/*====================------------------------------------====================*/
static void      o___MOCK____________________o (void) {;}

tMOCK   s_mocks     [500] = {
   /*---(root)--------------------------------------*/
   { "ROOT"   , 'n', NULL,     0.00, NULL, NULL,  -1,  -1,  -1,   NULL },
   /*---(done)--------------------------------------*/
   { "0a1"    , 'n', NULL,     1.00, NULL, NULL,   0,   0,   0,   NULL },
   { "0a2"    , 'n', NULL,     2.00, NULL, NULL,   0,   1,   0,   NULL },
   { "0a3"    , 'n', NULL,     3.00, NULL, NULL,   0,   2,   0,   NULL },
   { "0a4"    , 'n', NULL,     4.00, NULL, NULL,   0,   3,   0,   NULL },
   { "0a5"    , 'n', NULL,     5.00, NULL, NULL,   0,   4,   0,   NULL },
   { "0a6"    , 'n', NULL,     6.00, NULL, NULL,   0,   5,   0,   NULL },
   { "0a7"    , 'n', NULL,     7.00, NULL, NULL,   0,   6,   0,   NULL },
   { "0a8"    , 'n', NULL,     8.00, NULL, NULL,   0,   7,   0,   NULL },
   { "0a9"    , 'n', NULL,     9.00, NULL, NULL,   0,   8,   0,   NULL },
   { "0a10"   , 'n', NULL,    10.00, NULL, NULL,   0,   9,   0,   NULL },
   { "0a11"   , 'n', NULL,    11.00, NULL, NULL,   0,  10,   0,   NULL },
   { "0a30"   , 'n', NULL,    30.00, NULL, NULL,   0,  29,   0,   NULL },
   { "0a42"   , 'n', NULL,    42.00, NULL, NULL,   0,  41,   0,   NULL },
   { "1a1"    , 's', NULL,     0.00, NULL, NULL,   0,   0,   1,   NULL },
   { "1a2"    , 's', NULL,     0.00, NULL, NULL,   0,   1,   1,   NULL },
   { "1a3"    , 's', NULL,     0.00, NULL, NULL,   0,   2,   1,   NULL },
   { "1a4"    , 's', NULL,     0.00, NULL, NULL,   0,   3,   1,   NULL },
   { "0b1"    , 's', NULL,     0.00, NULL, NULL,   1,   0,   0,   NULL },
   { "0b2"    , 's', NULL,     0.00, NULL, NULL,   1,   1,   0,   NULL },
   { "0b3"    , 's', NULL,     0.00, NULL, NULL,   1,   2,   0,   NULL },
   { "0b4"    , 's', NULL,     0.00, NULL, NULL,   1,   3,   0,   NULL },
   { "0b5"    , 's', NULL,     0.00, NULL, NULL,   1,   4,   0,   NULL },
   { "0b6"    , 's', NULL,     0.00, NULL, NULL,   1,   5,   0,   NULL },
   { "0b7"    , 's', NULL,     0.00, NULL, NULL,   1,   6,   0,   NULL },
   { "0b8"    , 's', NULL,     0.00, NULL, NULL,   1,   7,   0,   NULL },
   { "0b9"    , 's', NULL,     0.00, NULL, NULL,   1,   8,   0,   NULL },
   { "0b10"   , 's', NULL,     0.00, NULL, NULL,   1,   9,   0,   NULL },
   { "0b11"   , 's', NULL,     0.00, NULL, NULL,   1,  10,   0,   NULL },
   { "0b12"   , 's', NULL,     0.00, NULL, NULL,   1,  11,   0,   NULL },
   { "0b13"   , 's', NULL,     0.00, NULL, NULL,   1,  12,   0,   NULL },
   { "0b14"   , 's', NULL,     0.00, NULL, NULL,   1,  13,   0,   NULL },
   { "0b15"   , 's', NULL,     0.00, NULL, NULL,   1,  14,   0,   NULL },
   { "0b16"   , 's', NULL,     0.00, NULL, NULL,   1,  15,   0,   NULL },
   { "0b17"   , 's', NULL,     0.00, NULL, NULL,   1,  17,   0,   NULL },
   { "0c1"    , 's', NULL,     0.00, NULL, NULL,   2,   0,   0,   NULL },
   { "0c2"    , 's', NULL,     0.00, NULL, NULL,   2,   1,   0,   NULL },
   { "0c3"    , 's', NULL,     0.00, NULL, NULL,   2,   2,   0,   NULL },
   { "0c4"    , 's', NULL,     0.00, NULL, NULL,   2,   3,   0,   NULL },
   { "0c5"    , 's', NULL,     0.00, NULL, NULL,   2,   4,   0,   NULL },
   { "0c6"    , 's', NULL,     0.00, NULL, NULL,   2,   5,   0,   NULL },
   { "0c7"    , 's', NULL,     0.00, NULL, NULL,   2,   6,   0,   NULL },
   { "0c8"    , 's', NULL,     0.00, NULL, NULL,   2,   7,   0,   NULL },
   { "0c9"    , 's', NULL,     0.00, NULL, NULL,   2,   8,   0,   NULL },
   { "0c10"   , 's', NULL,     0.00, NULL, NULL,   2,   9,   0,   NULL },
   { "0c11"   , 's', NULL,     0.00, NULL, NULL,   2,  10,   0,   NULL },
   { "0c12"   , 's', NULL,     0.00, NULL, NULL,   2,  11,   0,   NULL },
   { "0c13"   , 's', NULL,     0.00, NULL, NULL,   2,  12,   0,   NULL },
   { "0c14"   , 's', NULL,     0.00, NULL, NULL,   2,  13,   0,   NULL },
   { "0c15"   , 's', NULL,     0.00, NULL, NULL,   2,  14,   0,   NULL },
   { "0c16"   , 's', NULL,     0.00, NULL, NULL,   2,  15,   0,   NULL },
   { "0c17"   , 's', NULL,     0.00, NULL, NULL,   2,  16,   0,   NULL },
   { "0d1"    , 's', NULL,     0.00, NULL, NULL,   3,   0,   0,   NULL },
   { "0d2"    , 's', NULL,     0.00, NULL, NULL,   3,   1,   0,   NULL },
   { "0d3"    , 's', NULL,     0.00, NULL, NULL,   3,   2,   0,   NULL },
   { "0d4"    , 's', NULL,     0.00, NULL, NULL,   3,   3,   0,   NULL },
   { "0d5"    , 's', NULL,     0.00, NULL, NULL,   3,   4,   0,   NULL },
   { "0d6"    , 's', NULL,     0.00, NULL, NULL,   3,   5,   0,   NULL },
   { "0d7"    , 's', NULL,     0.00, NULL, NULL,   3,   6,   0,   NULL },
   { "0d8"    , 's', NULL,     0.00, NULL, NULL,   3,   7,   0,   NULL },
   { "0d9"    , 's', NULL,     0.00, NULL, NULL,   3,   8,   0,   NULL },
   { "0d10"   , 's', NULL,     0.00, NULL, NULL,   3,   9,   0,   NULL },
   { "0d11"   , 's', NULL,     0.00, NULL, NULL,   3,  10,   0,   NULL },
   { "0d12"   , 's', NULL,     0.00, NULL, NULL,   3,  11,   0,   NULL },
   { "0d13"   , 's', NULL,     0.00, NULL, NULL,   3,  12,   0,   NULL },
   { "0d14"   , 's', NULL,     0.00, NULL, NULL,   3,  13,   0,   NULL },
   { "0d15"   , 's', NULL,     0.00, NULL, NULL,   3,  14,   0,   NULL },
   { "0d16"   , 's', NULL,     0.00, NULL, NULL,   3,  15,   0,   NULL },
   { "0d17"   , 's', NULL,     0.00, NULL, NULL,   3,  16,   0,   NULL },
   { "0d18"   , 's', NULL,     0.00, NULL, NULL,   3,  17,   0,   NULL },
   /*---(two-way dependency testing)----------------*/
   { "0f6"    , 's', NULL,     0.00, NULL, NULL,   5,   5,   0,   NULL },
   { "0f7"    , 's', NULL,     0.00, NULL, NULL,   5,   6,   0,   NULL },
   { "0g6"    , 's', NULL,     0.00, NULL, NULL,   6,   5,   0,   NULL },
   { "0g7"    , 's', NULL,     0.00, NULL, NULL,   6,   6,   0,   NULL },
   { "0h8"    , 's', NULL,     0.00, NULL, NULL,   7,   7,   0,   NULL },
   { "0k11"   , 's', NULL,     0.00, NULL, NULL,  10,  10,   0,   NULL },
   { "1k11"   , 's', NULL,     0.00, NULL, NULL,  10,  10,   1,   NULL },
   { "2k11"   , 's', NULL,     0.00, NULL, NULL,  10,  10,   2,   NULL },
   { "3k11"   , 's', NULL,     0.00, NULL, NULL,  10,  10,   3,   NULL },
   { "4k11"   , 's', NULL,     0.00, NULL, NULL,  10,  10,   4,   NULL },
   /*---(done)--------------------------------------*/
   { ""       , 'n', NULL,     0.00, NULL, NULL,   0,   0,   0,   NULL },
};

char
ycalc__mock_prepare     (void)
{
   char        rc          =    0;
   if (rc == 0)  rc = yCALC_exist_config (ycalc__mock_enabler, ycalc__mock_pointer, ycalc__mock_reaper);
   if (rc == 0)  rc = yCALC_label_config (ycalc__mock_named  , ycalc__mock_whos_at, ycalc__mock_labeler);
   if (rc == 0)  rc = yCALC_value_config (ycalc__mock_valuer , ycalc__mock_address, ycalc__mock_special);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                      deproot functions                       ----===*/
/*====================------------------------------------====================*/
static void      o___DEPROOT_________________o (void) {;}

char
ycalc__mock_enabler     (void *a_owner, void *a_deproot)
{
   char        rce         =  -10;
   int         i           =    0;
   tMOCK      *x_mock      = NULL;
   DEBUG_DEPS   yLOG_senter  (__FUNCTION__);
   DEBUG_DEPS   yLOG_spoint  (a_owner);
   --rce;  if (a_owner == NULL) {
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_mock = (tMOCK *) a_owner;
   DEBUG_DEPS   yLOG_spoint  (a_deproot);
   x_mock->ycalc = a_deproot;
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ycalc__mock_named       (char *a_label, void **a_owner, void **a_deproot)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   int         n           =   -1;
   tMOCK      *x_mock      = NULL;
   /*---(header)-------------------------*/
   DEBUG_DEPS   yLOG_senter  (__FUNCTION__);
   DEBUG_DEPS   yLOG_spoint  (a_label);
   if (a_label == NULL) {
      DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
      return NULL;
   }
   DEBUG_DEPS   yLOG_snote   (a_label);
   for (i = 0; i < 500; ++i) {
      if (s_mocks [i].label [0] == 0)                break;
      if (strcmp (s_mocks [i].label, a_label) != 0)  continue;
      DEBUG_DEPS   yLOG_snote   ("FOUND");
      n = i;
   }
   DEBUG_DEPS   yLOG_sint    (n);
   /*---(handle failure)-----------------*/
   --rce;  if (n < 0) {
      DEBUG_DEPS   yLOG_snote   ("FAILURE");
      if (a_owner   != NULL)  *a_owner   = NULL;
      if (a_deproot != NULL)  *a_deproot = NULL;
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(handle normal)------------------*/
   DEBUG_DEPS   yLOG_snote   ("success");
   x_mock = (tMOCK *) (s_mocks + n);
   DEBUG_DEPS   yLOG_spoint  (x_mock);
   if (a_owner   != NULL)  *a_owner   = x_mock;
   DEBUG_DEPS   yLOG_spoint  (x_mock->ycalc);
   if (a_deproot != NULL)  *a_deproot = x_mock->ycalc;
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ycalc__mock_whos_at     (int x, int y, int z, void **a_owner, void **a_deproot)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   int         n           =   -1;
   tMOCK      *x_mock      = NULL;
   /*---(header)-------------------------*/
   DEBUG_DEPS   yLOG_senter  (__FUNCTION__);
   DEBUG_DEPS   yLOG_sint    (x);
   DEBUG_DEPS   yLOG_sint    (y);
   DEBUG_DEPS   yLOG_sint    (z);
   /*---(search)-------------------------*/
   for (i = 0; i < 500; ++i) {
      if (s_mocks [i].label [0] == 0)                break;
      if (s_mocks [i].x != x)                        continue;
      if (s_mocks [i].y != y)                        continue;
      if (s_mocks [i].z != z)                        continue;
      DEBUG_DEPS   yLOG_snote   ("FOUND");
      n = i;
   }
   DEBUG_DEPS   yLOG_sint    (n);
   /*---(handle failure)-----------------*/
   --rce;  if (n < 0) {
      DEBUG_DEPS   yLOG_snote   ("FAILURE");
      if (a_owner   != NULL)  *a_owner   = NULL;
      if (a_deproot != NULL)  *a_deproot = NULL;
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(handle normal)------------------*/
   DEBUG_DEPS   yLOG_snote   ("success");
   x_mock = (tMOCK *) (s_mocks + n);
   DEBUG_DEPS   yLOG_spoint  (x_mock);
   if (a_owner   != NULL)  *a_owner   = x_mock;
   DEBUG_DEPS   yLOG_spoint  (x_mock->ycalc);
   if (a_deproot != NULL)  *a_deproot = x_mock->ycalc;
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       owner functions                        ----===*/
/*====================------------------------------------====================*/
static void      o___OWNER___________________o (void) {;}

char*
ycalc__mock_labeler     (void *a_owner)
{
   tMOCK      *x_mock      = NULL;
   DEBUG_DEPS   yLOG_senter  (__FUNCTION__);
   DEBUG_DEPS   yLOG_spoint  (a_owner);
   if (a_owner == NULL) {
      DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
      return g_nada;
   }
   x_mock = (tMOCK *) a_owner;
   DEBUG_DEPS   yLOG_snote   (x_mock->label);
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
   return x_mock->label;
}

char
ycalc__mock_reaper      (void *a_owner)
{
   return 0;
}

char
ycalc__mock_pointer     (void *a_owner, char **a_source, char **a_type, double **a_value, char **a_string)
{
   char        rce         =  -10;
   tMOCK      *x_mock      = NULL;
   DEBUG_DEPS   yLOG_senter  (__FUNCTION__);
   DEBUG_DEPS   yLOG_spoint  (a_owner);
   --rce;  if (a_owner  == NULL) {
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_mock    = (tMOCK     *) a_owner;
   if (a_source != NULL) {
      DEBUG_DEPS   yLOG_spoint  (*a_source);
      *a_source = &x_mock->source;
      DEBUG_DEPS   yLOG_spoint  (*a_source);
   }
   if (a_type  != NULL) {
      DEBUG_DEPS   yLOG_spoint  (*a_type);
      *a_type   = &x_mock->type;
      DEBUG_DEPS   yLOG_spoint  (*a_type);
   }
   if (a_value  != NULL) {
      DEBUG_DEPS   yLOG_spoint  (*a_value);
      *a_value  = &x_mock->value;
      DEBUG_DEPS   yLOG_spoint  (*a_value);
   }
   if (a_string != NULL) {
      DEBUG_DEPS   yLOG_spoint  (*a_string);
      *a_string = &x_mock->string;
      DEBUG_DEPS   yLOG_spoint  (*a_string);
   }
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ycalc__mock_valuer      (void *a_owner, char *a_type, double *a_value, char **a_string)
{
   char        rce         =  -10;
   tMOCK      *x_mock      = NULL;
   DEBUG_DEPS   yLOG_senter  (__FUNCTION__);
   DEBUG_DEPS   yLOG_spoint  (a_owner);
   if (a_type   != NULL)  *a_type   = '-';
   if (a_value  != NULL)  *a_value  = 0.0;
   if (a_string != NULL)  *a_string = g_nada;
   --rce;  if (a_owner  == NULL) {
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_mock    = (tMOCK     *) a_owner;
   if (a_type   != NULL) {
      *a_type   = x_mock->type;
      DEBUG_DEPS   yLOG_schar   (*a_type);
   }
   if (a_value  != NULL) {
      *a_value  = x_mock->value;
      DEBUG_DEPS   yLOG_sdouble (*a_value);
   }
   if (a_string != NULL) {
      if      (x_mock->type == 'E')     *a_string   = g_nada;
      else if (x_mock->type == 's' && x_mock->source != NULL)  *a_string = x_mock->source;
      else if (x_mock->string != NULL)  *a_string = x_mock->string;
      else                              *a_string = g_nada;
      DEBUG_DEPS   yLOG_snote   (*a_string);
   }
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ycalc__mock_address     (void *a_owner, int *x, int *y, int *z)
{
   tMOCK      *x_mock      = NULL;
   if (x != NULL)  *x   = 0;
   if (y != NULL)  *y   = 0;
   if (z != NULL)  *z   = 0;
   if (a_owner == NULL)  return -1;
   x_mock    = (tMOCK     *) a_owner;
   if (x != NULL)  *x   = x_mock->x;
   if (y != NULL)  *y   = x_mock->y;
   if (z != NULL)  *z   = x_mock->z;
   return 0;
}

char
ycalc__mock_special     (void *a_owner, char a_what, double *a_value, char **a_string)
{
   char        rce         =  -10;
   tMOCK      *x_owner     = NULL;
   tDEP_ROOT  *x_deproot   = NULL;
   DEBUG_DEPS   yLOG_senter  (__FUNCTION__);
   DEBUG_DEPS   yLOG_spoint  (a_owner);
   DEBUG_DEPS   yLOG_schar   (a_what);
   if (a_value  != NULL)  *a_value  = 0.0;
   if (a_string != NULL)  *a_string = g_nada;
   --rce;  if (a_owner   == NULL) {
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_owner   = (tMOCK     *) a_owner;
   x_deproot = (tDEP_ROOT *) x_owner->ycalc;
   --rce;  if (x_deproot == NULL) {
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  switch (a_what) {
   case G_SPECIAL_SOURCE :
      if (a_string != NULL)  *a_string = x_owner->source;
      break;
   case G_SPECIAL_PRINT  :
      if (a_string != NULL)  *a_string = x_owner->print;
      break;
   case G_SPECIAL_TYPE   :
      if (a_value  != NULL)  *a_value  = x_owner->type;
      break;
   }
   DEBUG_DEPS   yLOG_snote   (*a_string);
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ycalc__mock_source      (char *a_label, char *a_source)
{
   char        rce         =  -10;
   char        rc          =    0;
   tMOCK      *x_owner     = NULL;
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   rc = ycalc_call_who_named (a_label, &x_owner, NULL);
   DEBUG_DEPS   yLOG_value   ("rc"        , rc);
   --rce;  if (rc < 0) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_point   ("source"    , x_owner->source);
   DEBUG_DEPS   yLOG_info    ("source"    , x_owner->source);
   if (x_owner->source != NULL) {
      DEBUG_DEPS   yLOG_note    ("must free");
      free (x_owner->source);
      x_owner->source = NULL;
   }
   if (a_source != NULL) {
      x_owner->source = strdup (a_source);
      DEBUG_DEPS   yLOG_point   ("source"    , x_owner->source);
      DEBUG_DEPS   yLOG_info    ("source"    , x_owner->source);
   }
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc__mock_special_set (char *a_label, char *a_source, double a_value, char *a_print)
{
   char        rce         =  -10;
   char        rc          =    0;
   tMOCK      *x_owner     = NULL;
   rc = ycalc_call_who_named (a_label, &x_owner, NULL);
   --rce;  if (rc < 0)  return rce;
   if (a_source != NULL) {
      if (x_owner->source != NULL) {
         free (x_owner->source);
         x_owner->source = NULL;
      }
      x_owner->source = strdup (a_source);
   }
   if (a_value  != -666) {
      x_owner->value  = a_value;
   }
   if (a_print  != NULL) {
      if (x_owner->print != NULL) {
         free (x_owner->print);
         x_owner->print = NULL;
      }
      x_owner->print  = strdup (a_print);
   }
   return 0;
}

char
ycalc__mock_whole       (char *a_label, char *a_source, char a_format, char a_decs, char a_align, char a_width)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tMOCK      *x_owner     = NULL;
   tDEP_ROOT  *x_deproot   = NULL;
   char        t           [LEN_RECD];
   char        x_out       [LEN_RECD];
   char        x_len       =    0;
   FILE       *f           = NULL;
   /*---(header)-------------------------*/
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   /*---(get location)-------------------*/
   DEBUG_DEPS   yLOG_point   ("a_label"   , a_label);
   --rce;  if (a_label == NULL) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_info    ("a_label"   , a_label);
   rc = ycalc_call_who_named (a_label, &x_owner, &x_deproot);
   DEBUG_DEPS   yLOG_value   ("rc"        , rc);
   --rce;  if (rc < 0) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_point   ("x_owner"   , x_owner);
   DEBUG_DEPS   yLOG_point   ("x_deproot" , x_deproot);
   DEBUG_DEPS   yLOG_point   ("a_source"  , a_source);
   --rce;  if (a_source == NULL) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_info    ("a_source"  , a_source);
   /*---(load/clear)---------------------*/
   x_owner->source = strdup (a_source);
   if (x_owner->print  != NULL) {
      free (x_owner->print);
      x_owner->print  = NULL;
   }
   /*---(classify)-----------------------*/
   rc = yCALC_handle (a_label);
   /*> rc = ycalc_classify_trusted (x_deproot, &x_owner->source, &x_owner->type, &x_owner->value, &x_owner->string);   <*/
   /*---(handle formulas)----------------*/
   /*> switch (x_owner->type) {                                                                                          <* 
    *> case 'n' :                                                                                                        <* 
    *>    DEBUG_DEPS   yLOG_note    ("numeric literal type");                                                            <* 
    *>    x_owner->value  = atof (a_source);                                                                             <* 
    *>    break;                                                                                                         <* 
    *> case '=' : case '#' : case '&' :                                                                                  <* 
    *>    DEBUG_DEPS   yLOG_note    ("formula type");                                                                    <* 
    *>    rc = ycalc_build_trusted   (x_deproot, &x_owner->source, &x_owner->type, &x_owner->value, &x_owner->string);   <* 
    *>    DEBUG_DEPS   yLOG_value   ("build"     , rc);                                                                  <* 
    *>    --rce;  if (rc < 0) {                                                                                          <* 
    *>       DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);                                                              <* 
    *>       return rce;                                                                                                 <* 
    *>    }                                                                                                              <* 
    *>    rc = ycalc_execute_trusted (x_deproot, &x_owner->type, &x_owner->value, &x_owner->string);                     <* 
    *>    DEBUG_DEPS   yLOG_value   ("exec"      , rc);                                                                  <* 
    *>    --rce;  if (rc < 0) {                                                                                          <* 
    *>       DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);                                                              <* 
    *>       return rce;                                                                                                 <* 
    *>    }                                                                                                              <* 
    *>    break;                                                                                                         <* 
    *> }                                                                                                                 <*/
   /*---(handle results)-----------------*/
   if (strchr ("=n", x_owner->type) != NULL) {
      strl4main (x_owner->value, t, a_decs, a_format, LEN_RECD);
   } else {
      if (x_owner->string != NULL)  strlcpy (t, x_owner->string, LEN_RECD);
      else                          strlcpy (t, x_owner->source, LEN_RECD);
   }
   strlpad (t, x_out, ' ', a_align, a_width - 1);
   x_owner->print = strdup (x_out);
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNIT_TEST_______o () { return; }

char*        /*-> unit testing accessor --------------[ ------ [gs.HA0.1B3.K5]*/ /*-[02.0000.00#.#]-*/ /*-[--.---.---.--]-*/
ycalc__unit_mock        (char *a_question, char *a_label)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   tMOCK      *x_owner     = NULL;
   tDEP_ROOT  *x_deproot   = NULL;
   char        x_label     [LEN_LABEL];
   char        x_rnote     [LEN_LABEL];
   char        x_onote     [LEN_LABEL];
   char        x_dnote     [LEN_LABEL];
   char        x_string    [LEN_RECD ];
   double      x_value     =  0.0;
   /*---(preprare)-----------------------*/
   strcpy (ycalc__unit_answer, "yCALC            : question not understood");
   /*---(label)--------------------------*/
   if (a_label == NULL)   strlcpy (x_label, "---"  , LEN_LABEL);
   else                   strlcpy (x_label, a_label, LEN_LABEL);
   /*---(owner/deproot)------------------*/
   rc = ycalc__mock_named (x_label, &x_owner, &x_deproot);
   if (rc        <  0   )  strlcpy (x_rnote , "error"   , LEN_LABEL);
   else                    strlcpy (x_rnote , "good"    , LEN_LABEL);
   if (x_owner   == NULL)  strlcpy (x_onote , "-----"   , LEN_LABEL);
   else                    strlcpy (x_onote , "exists"  , LEN_LABEL);
   if (x_deproot == NULL)  strlcpy (x_dnote , "-----"   , LEN_LABEL);
   else                    strlcpy (x_dnote , "enabled" , LEN_LABEL);
   /*---(string)-------------------------*/
   if      (x_owner == NULL)              strlcpy (x_string, "", LEN_RECD );
   else if (strchr ("s#5E", x_owner->type) == NULL)   strlcpy (x_string, "", LEN_RECD );
   else if (x_owner->string != NULL)      strlcpy (x_string, x_owner->string, LEN_RECD );
   else if (x_owner->type == 's' && x_owner->source != NULL)         strlcpy (x_string, x_owner->source, LEN_RECD );
   else                                   strlcpy (x_string, "", LEN_RECD );
   /*---(value)--------------------------*/
   if      (x_owner == NULL)              x_value = 0.0;
   else                                   x_value = x_owner->value;
   /*---(dependency list)----------------*/
   if      (strcmp (a_question, "status"   )      == 0) {
      snprintf (ycalc__unit_answer, LEN_RECD, "yCALC mock status: %-5s %-8s %-10s %s", x_label, x_rnote, x_onote, x_dnote);
   }
   else if (strcmp (a_question, "value"    )      == 0) {
      snprintf (ycalc__unit_answer, LEN_RECD, "yCALC mock value : %-5s %c %8.2lf :%-.45s:", x_label, x_owner->type, x_value, x_string);
   }
   else if (strcmp (a_question, "double"   )      == 0) {
      snprintf (ycalc__unit_answer, LEN_RECD, "yCALC mock double: %-5s %c %16.6lf", x_label, x_owner->type, x_value);
   }
   /*---(complete)-----------------------*/
   return ycalc__unit_answer;
}



