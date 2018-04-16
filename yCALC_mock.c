/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"




/*====================------------------------------------====================*/
/*===----                         mock system                          ----===*/
/*====================------------------------------------====================*/
static void      o___MOCK____________________o (void) {;}

tMOCK   s_mocks     [100] = {
   /*---(root)--------------------------------------*/
   { "ROOT"      , 'n' ,     0.00, NULL            ,  -1,  -1,  -1,   NULL },
   /*---(done)--------------------------------------*/
   { "0a1"       , 'n' ,     1.00, NULL            ,   0,   0,   0,   NULL },
   { "0a2"       , 'n' ,     2.00, NULL            ,   0,   1,   0,   NULL },
   { "0a3"       , 'n' ,     3.00, NULL            ,   0,   2,   0,   NULL },
   { "0a4"       , 'n' ,     4.00, NULL            ,   0,   3,   0,   NULL },
   { "0a5"       , 'n' ,     5.00, NULL            ,   0,   4,   0,   NULL },
   { "0a6"       , 'n' ,     6.00, NULL            ,   0,   5,   0,   NULL },
   { "0a7"       , 'n' ,     7.00, NULL            ,   0,   6,   0,   NULL },
   { "0a8"       , 'n' ,     8.00, NULL            ,   0,   7,   0,   NULL },
   { "0a9"       , 'n' ,     9.00, NULL            ,   0,   8,   0,   NULL },
   { "0a10"      , 'n' ,    10.00, NULL            ,   0,   9,   0,   NULL },
   { "0a30"      , 'n' ,    30.00, NULL            ,   0,  29,   0,   NULL },
   { "0a42"      , 'n' ,    42.00, NULL            ,   0,  41,   0,   NULL },
   { "1a1"       , 's' ,     0.00, NULL            ,   0,   0,   1,   NULL },
   { "1a2"       , 's' ,     0.00, NULL            ,   0,   1,   1,   NULL },
   { "1a3"       , 's' ,     0.00, NULL            ,   0,   2,   1,   NULL },
   { "1a4"       , 's' ,     0.00, NULL            ,   0,   3,   1,   NULL },
   /*---(two-way dependency testing)----------------*/
   { "0f6"       , 's' ,     0.00, NULL            ,   5,   5,   0,   NULL },
   { "0f7"       , 's' ,     0.00, NULL            ,   5,   6,   0,   NULL },
   { "0g6"       , 's' ,     0.00, NULL            ,   6,   5,   0,   NULL },
   { "0g7"       , 's' ,     0.00, NULL            ,   6,   6,   0,   NULL },
   { "0h8"       , 's' ,     0.00, NULL            ,   7,   7,   0,   NULL },
   { "0k11"      , 's' ,     0.00, NULL            ,  10,  10,   0,   NULL },
   { "1k11"      , 's' ,     0.00, NULL            ,  10,  10,   1,   NULL },
   { "2k11"      , 's' ,     0.00, NULL            ,  10,  10,   2,   NULL },
   { "3k11"      , 's' ,     0.00, NULL            ,  10,  10,   3,   NULL },
   { "4k11"      , 's' ,     0.00, NULL            ,  10,  10,   4,   NULL },
   /*---(done)--------------------------------------*/
   { ""          , 'n' ,     0.00, NULL            ,   0,   0,   0,   NULL },
};

char
ycalc__mock_enable      (void)
{
   int         i           =    0;
   tMOCK      *x_mock      = NULL;
   for (i = 0; i < 100; ++i) {
      if (s_mocks [i].label [0] == 0)                break;
      x_mock = s_mocks + i;
      yCALC_enable (x_mock);
   }
   return 0;
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
ycalc__mock_who_named   (char *a_label, void **a_owner, void **a_deproot)
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
   for (i = 0; i < 100; ++i) {
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
ycalc__mock_who_at      (int x, int y, int z, void **a_owner, void **a_deproot)
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
   for (i = 0; i < 100; ++i) {
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
ycalc__mock_valuer      (void *a_owner, char *a_type, double *a_value, char **a_string)
{
   tMOCK      *x_mock      = NULL;
   DEBUG_DEPS   yLOG_senter  (__FUNCTION__);
   DEBUG_DEPS   yLOG_spoint  (a_owner);
   if (a_type   != NULL)  *a_type   = '-';
   if (a_value  != NULL)  *a_value  = 0.0;
   if (a_string != NULL)  *a_string = g_nada;
   if (a_owner  == NULL)  return -1;
   x_mock    = (tMOCK     *) a_owner;
   if (a_type   != NULL)  *a_type   = x_mock->type;
   if (a_value  != NULL)  *a_value  = x_mock->value;
   if (a_string != NULL) {
      if (x_mock->string == NULL)  *a_string = g_nada;
      else                         *a_string = x_mock->string;
   }
   DEBUG_DEPS   yLOG_schar   (x_mock->type);
   DEBUG_DEPS   yLOG_sdouble (x_mock->value);
   DEBUG_DEPS   yLOG_snote   (x_mock->string);
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ycalc__mock_addresser   (void *a_owner, int *x, int *y, int *z)
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
   if (a_label == NULL)   strlcpy (x_label, "---"  , LEN_LABEL);
   else                   strlcpy (x_label, a_label, LEN_LABEL);
   rc = ycalc__mock_who_named (x_label, &x_owner, &x_deproot);
   if (rc        <  0   )  strlcpy (x_rnote , "error"   , LEN_LABEL);
   else                    strlcpy (x_rnote , "good"    , LEN_LABEL);
   if (x_owner   == NULL)  strlcpy (x_onote , "-----"   , LEN_LABEL);
   else                    strlcpy (x_onote , "exists"  , LEN_LABEL);
   if (x_deproot == NULL)  strlcpy (x_dnote , "-----"   , LEN_LABEL);
   else                    strlcpy (x_dnote , "enabled" , LEN_LABEL);

   if      (x_owner == NULL)              strlcpy (x_string, "-", LEN_LABEL);
   else if (x_owner->string == NULL)      strlcpy (x_string, "-", LEN_LABEL);
   else if (x_owner->string [0] == NULL)  strlcpy (x_string, "-", LEN_LABEL);
   else                                   strlcpy (x_string, x_owner->string, LEN_LABEL);
   if      (x_owner == NULL)              x_value = 0.0;
   else                                   x_value = x_owner->value;
   /*---(dependency list)----------------*/
   if      (strcmp (a_question, "status"   )      == 0) {
      snprintf (ycalc__unit_answer, LEN_RECD, "yCALC mock status: %-5s %-8s %-10s %s", x_label, x_rnote, x_onote, x_dnote);
   }
   else if (strcmp (a_question, "value"    )      == 0) {
      snprintf (ycalc__unit_answer, LEN_RECD, "yCALC mock value : %-5s %8.2lf %-.35s", x_label, x_value, x_string);
   }
   /*---(complete)-----------------------*/
   return ycalc__unit_answer;
}



