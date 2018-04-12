/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"




/*====================------------------------------------====================*/
/*===----                         mock system                          ----===*/
/*====================------------------------------------====================*/
static void      o___MOCK____________________o (void) {;}

typedef struct cMOCK  tMOCK;
struct  cMOCK {
   char        label       [LEN_LABEL];
   char        type;
   double      value;
   char        string      [LEN_RECD ];
   int         x;
   int         y;
   int         z;
   void       *ycalc;
};
static tMOCK   s_mocks     [100] = {
   /*---(root)--------------------------------------*/
   { "ROOT"      , 'n' ,     1.00, ""              ,  -1,  -1,  -1,   NULL },
   /*---(done)--------------------------------------*/
   { "0a1"       , 'n' ,     1.00, ""              ,   0,   0,   0,   NULL },
   { "0a2"       , 'n' ,     2.00, ""              ,   0,   1,   0,   NULL },
   { "0a3"       , 'n' ,     3.00, ""              ,   0,   2,   0,   NULL },
   { "0a4"       , 'n' ,     4.00, ""              ,   0,   3,   0,   NULL },
   { "0a5"       , 'n' ,     5.00, ""              ,   0,   4,   0,   NULL },
   { "0a6"       , 'n' ,     6.00, ""              ,   0,   5,   0,   NULL },
   { "0a10"      , 'n' ,    10.00, ""              ,   0,   9,   0,   NULL },
   { "0a30"      , 'n' ,    30.00, ""              ,   0,  29,   0,   NULL },
   { "0a42"      , 'n' ,    42.00, ""              ,   0,  41,   0,   NULL },
   { "1a1"       , 's' ,     0.00, "one"           ,   0,   0,   1,   NULL },
   { "1a2"       , 's' ,     0.00, "onetwo"        ,   0,   1,   1,   NULL },
   { "1a3"       , 's' ,     0.00, "two"           ,   0,   2,   1,   NULL },
   { "1a4"       , 's' ,     0.00, "three"         ,   0,   3,   1,   NULL },
   /*---(two-way dependency testing)----------------*/
   { "0f6"       , 's' ,     0.00, "one"           ,   5,   5,   0,   NULL },
   { "0f7"       , 's' ,     0.00, "two"           ,   5,   6,   0,   NULL },
   { "0g6"       , 's' ,     0.00, "three"         ,   6,   5,   0,   NULL },
   { "0g7"       , 's' ,     0.00, "four"          ,   6,   6,   0,   NULL },
   { "0h8"       , 's' ,     0.00, "five"          ,   7,   7,   0,   NULL },
   { "0k11"      , 's' ,     0.00, "nine"          ,  10,  10,   0,   NULL },
   /*---(done)--------------------------------------*/
   { ""          , 'n' ,     0.00, ""              ,   0,   0,   0,   NULL },
};

char
ycalc__mock_enable      (void)
{
   int         i           =    0;
   tMOCK      *x_mock      = NULL;
   for (i = 0; i < 100; ++i) {
      if (s_mocks [i].label [0] == 0)                break;
      x_mock = s_mocks + i;
      yCALC_enable (x_mock, &x_mock->ycalc);
   }
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      deproot functions                       ----===*/
/*====================------------------------------------====================*/
static void      o___DEPROOT_________________o (void) {;}

void*
ycalc__mock_deproot     (char *a_label)
{
   int         i           =    0;
   int         n           =   -1;
   tMOCK      *x_mock      = NULL;
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
   if (n < 0) {
      DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
      return NULL;
   }
   x_mock = (tMOCK *) (s_mocks + n);
   DEBUG_DEPS   yLOG_spoint  (x_mock->ycalc);
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
   return x_mock->ycalc;
}

void*
ycalc__mock_whois       (int x, int y, int z)
{
   int         i           =    0;
   int         n           =   -1;
   tMOCK      *x_mock      = NULL;
   for (i = 0; i < 100; ++i) {
      if (s_mocks [i].label [0] == 0)                break;
      if (s_mocks [i].x != x)                        continue;
      if (s_mocks [i].y != y)                        continue;
      if (s_mocks [i].z != z)                        continue;
      n = i;
   }
   if (n < 0)  return NULL;
   x_mock = (tMOCK *) (s_mocks + n);
   return x_mock->ycalc;
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
   if (a_type   != NULL)  *a_type   = '-';
   if (a_value  != NULL)  *a_value  = 0.0;
   if (a_string != NULL)  *a_string = g_nada;
   if (a_owner  == NULL)  return -1;
   x_mock    = (tMOCK     *) a_owner;
   if (a_type   != NULL)  *a_type   = x_mock->type;
   if (a_value  != NULL)  *a_value  = x_mock->value;
   if (a_string != NULL)  *a_string = x_mock->string;
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


