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
   { "0a1"       , 'n' ,     1.00, ""              ,   0,   0,   0  , NULL },
   { "0a2"       , 'n' ,     2.00, ""              ,   0,   1,   0  , NULL },
   { "0a3"       , 'n' ,     3.00, ""              ,   0,   2,   0  , NULL },
   { "0a4"       , 'n' ,     4.00, ""              ,   0,   3,   0  , NULL },
   { "0a5"       , 'n' ,     5.00, ""              ,   0,   4,   0  , NULL },
   { "0a6"       , 'n' ,     6.00, ""              ,   0,   5,   0  , NULL },
   { "0a10"      , 'n' ,    10.00, ""              ,   0,   9,   0  , NULL },
   { "0a30"      , 'n' ,    30.00, ""              ,   0,  29,   0  , NULL },
   { "0a42"      , 'n' ,    42.00, ""              ,   0,  41,   0  , NULL },
   { "1a1"       , 's' ,     0.00, "one"           ,   0,   0,   1  , NULL },
   { "1a2"       , 's' ,     0.00, "onetwo"        ,   0,   1,   1  , NULL },
   { "1a3"       , 's' ,     0.00, "two"           ,   0,   2,   1  , NULL },
   { "1a4"       , 's' ,     0.00, "three"         ,   0,   3,   1  , NULL },
   { ""          , 'n' ,     0.00, ""              ,   0,   0,   0  , NULL },
};



void*
ycalc__mock_thinger     (char *a_label)
{
   int         i           =    0;
   int         n           =   -1;
   if (a_label == NULL)  return NULL;
   for (i = 0; i < 100; ++i) {
      if (s_mocks [i].label [0] == 0)                break;
      if (strcmp (s_mocks [i].label, a_label) != 0)  continue;
      n = i;
   }
   if (n < 0)  return NULL;
   return s_mocks + n;
}

char*
ycalc__mock_labeler     (void *a_thing)
{
   tMOCK      *x_thing     = NULL;
   x_thing = (tMOCK *) a_thing;
   return x_thing->label;
}

char
ycalc__mock_valuer      (void *a_thing, char *a_type, double *a_value, char **a_string)
{
   tMOCK      *x_thing     = NULL;
   x_thing = (tMOCK *) a_thing;
   if (a_type   != NULL)  *a_type   = x_thing->type;
   if (a_value  != NULL)  *a_value  = x_thing->value;
   if (a_string != NULL)  *a_string = x_thing->string;
   return 0;
}

char
ycalc__mock_detailer    (void *a_thing, char *a_quality, char *a_string, double *a_value)
{
   return 0;
}

char
ycalc__mock_addresser   (void *a_thing, int *x, int *y, int *z)
{
   tMOCK      *x_thing     = NULL;
   x_thing = (tMOCK *) a_thing;
   if (x != NULL)  *x   = x_thing->x;
   if (y != NULL)  *y   = x_thing->y;
   if (z != NULL)  *z   = x_thing->z;
   return 0;
}

