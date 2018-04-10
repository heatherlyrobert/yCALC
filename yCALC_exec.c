/*============================----beg-of-source---============================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"



char    (*g_thinger  )   (char *a_label, void **a_thing  );
char    (*g_valuer   )   (void *a_thing, char  *a_type   , double *a_value  , char   **a_string);
char    (*g_detailer )   (void *a_thing, char  *a_quality, char   *a_string , double  *a_value);
char    (*g_addresser)   (void *a_thing, int   *x        , int    *y        , int     *z);
char    (*g_lister   )   (void *a_thing, char  *a_quality, char   *a_list   );



static char    s_type    = '-';
static double  s_value   = 0.0;
static char   *s_string  = NULL;
static int     x, y, z;


/*====================------------------------------------====================*/
/*===----                         execution stack                      ----===*/
/*====================------------------------------------====================*/
/*
 *   as the rpn calculation is read from right-to-left, the execution stack
 *   stores all initial and intermediate values until required by an operator.
 *   the results of each interim calculation are also placed back on the stack
 *   until needed by the next operator.  as a result, the size of the stack will
 *   vary wildly during the calculation.
 *
 *   as stated before, a stack is an incredibly simple and fast data structure,
 *   especially when implemented within an array.  this is one of the myriad of
 *   benefits to using postfix/rpn calculations -- they only require a stack
 *   data structure and no transformation at execution time making it fast.
 *
 *
 *
 *   the execution stack handles all values during calculation.  when a value
 *   is read, it is pushed on the stack.  when an operator is read, enough
 *   values are then popped off the stack to fulfill the operators requirements.
 *   at completion, the stack should be exactly empty.
 *
 *
 *
 *   the execution stack handles all values as they come before the operators
 *   and functions in rpn/postfix notation.  the results of interim calculations
 *   are also pushed on the stack so the size of the stack varies through-out
 *   the calculation.
 *
 *   at the end of the calculation, the stack must be empty or the calculation
 *   was misformed and an error is reported.  also, if a calculation needs
 *   values and none are left, this should also report as an error.
 *
 *   it is critical that a simple stack structure is used so that the
 *   calculations are quick and reliable.  in this case i chose an array of
 *   value entries.  i also capped the array at an arbitrarily large value.  you
 *   can change this value to whatever you like.
 *
 *   the stack can only hold three types of values: string literal, numeric
 *   literal, and a reference to a cell that contains either the string or
 *   numeric value desired.  the reference is the equivalent of using variables
 *   in a programming language.
 *
 *   if you wanted to have fun, cells could act as pointers allowing a wide
 *   variety of redirection styles, but i don't have an obvious use case that
 *   justifies that yet.  it would be dangerous to build it out before really
 *   understanding its uses or power.
 *
 *   the three value types could have been handled as a union to save space, but
 *   i opted for having separate items so that i would not make dangerous
 *   mistakes with pointers.  this is certainly not a lot of space sacrified
 *   for simplicity.  this can also be easily changed if you like.
 *
 *   the calc_stack structure can only be used by the five dedicated functions
 *   which in turn can only be used within the CALC module.  i could have made
 *   them static to ensure this, but i have an external unit test that needs
 *   access.
 *
 */

#define     S_TYPE_EMPTY        '-'
#define     S_TYPE_NUM          'n'
#define     S_TYPE_STR          's'
#define     S_TYPE_REF          'r'

#define     S_MAX_STACK         99
typedef  struct cSTACK tSTACK;
struct cSTACK {
   char        typ;            /* type : r=ref, v=val, s=str                    */
   void       *ref;            /* cell reference                                */
   double      num;            /* constant value                                */
   char       *str;            /* literal string                                */
};
static tSTACK  s_stack   [S_MAX_STACK];
static int     s_nstack  = 0;



/*====================------------------------------------====================*/
/*===----                        program level                         ----===*/
/*====================------------------------------------====================*/
static void      o___PROGRAM_________________o (void) {;}

char
yCALC__exec_init         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(globals)------------------------*/
   DEBUG_PROG   yLOG_note    ("clearing stack");
   for (i = 0; i < S_MAX_STACK; ++i) {
      s_stack [i].typ   = S_TYPE_EMPTY;
      s_stack [i].ref   = NULL;
      s_stack [i].num   = 0.0;
      s_stack [i].str   = NULL;
   }
   s_nstack = 0;
   /*---(functions)----------------------*/
   DEBUG_PROG   yLOG_note    ("clearing function calls");
   g_thinger   = NULL;
   g_valuer    = NULL;
   g_detailer  = NULL;
   g_addresser = NULL;
   g_lister    = NULL;
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yCALC_exec_config       (void *a_thinger, void *a_valuer, void *a_detailer, void *a_addresser)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_PROG   yLOG_char    ("status"    , myCALC.status);
   --rce;  if (strchr ("IO", myCALC.status) == NULL) {
      DEBUG_PROG   yLOG_note    ("must initialize before configuring");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update thinger)-----------------*/
   DEBUG_PROG   yLOG_point   ("thinger"   , a_thinger);
   --rce;  if (a_thinger   == NULL) {
      DEBUG_PROG   yLOG_error   ("thinger"   , "without this callback, references, ranges, and variables can not function");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   g_thinger   = a_thinger;
   /*---(update valuer)------------------*/
   DEBUG_PROG   yLOG_point   ("valuer"    , a_valuer);
   --rce;  if (a_valuer    == NULL) {
      DEBUG_PROG   yLOG_error   ("valuer"    , "without this callback, references, ranges, and variables can not function");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   g_valuer    = a_valuer;
   /*---(update detailer)----------------*/
   DEBUG_PROG   yLOG_point   ("detailer"  , a_detailer);
   --rce;  if (a_detailer  == NULL) {
      DEBUG_PROG   yLOG_warn    ("detailer"  , "without this callback, a few functions may not be allowed");
   }
   g_detailer  = a_detailer;
   /*---(update addresser)---------------*/
   DEBUG_PROG   yLOG_point   ("addresser" , a_addresser);
   --rce;  if (a_addresser == NULL) {
      DEBUG_PROG   yLOG_warn    ("addresser" , "without this callback, a few functions may not be allowed");
   }
   g_addresser = a_addresser;
   /*---(update)-------------------------*/
   DEBUG_PROG   yLOG_note    ("updating status");
   myCALC.status_detail [3] = 'e';
   if (myCALC.status_detail [2] == 'b') {
      myCALC.status = 'O';
      myCALC.status_detail [6] = 'o';
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yCALC_debug_config      (void *a_lister)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_PROG   yLOG_char    ("status"    , myCALC.status);
   --rce;  if (strchr ("IO", myCALC.status) == NULL) {
      DEBUG_PROG   yLOG_note    ("must initialize before configuring");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update lister)-----------------*/
   DEBUG_PROG   yLOG_point   ("lister"   , a_lister);
   --rce;  if (a_lister   == NULL) {
      DEBUG_PROG   yLOG_warn    ("lister"   , "without this callback, only a few debugging functions will not work");
   }
   g_lister   = a_lister;
   /*---(update)-------------------------*/
   DEBUG_PROG   yLOG_note    ("updating status");
   myCALC.status_detail [4] = 'd';
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        adding to stack                       ----===*/
/*====================------------------------------------====================*/
static void      o___PUSHING_________________o (void) {;}

char         /*-> add a value to the stack -----------[ ------ [gc.420.202.11]*/ /*-[01.0000.0#5.!]-*/ /*-[--.---.---.--]-*/
yCALC_pushval           (char *a_func, double a_value)
{
   /*---(defense: stack overflow)--------*/
   if (s_nstack >= S_MAX_STACK) {
      myCALC.trouble = G_ERROR_STACK;
      return 0;
   }
   /*---(update stack item)--------------*/
   s_stack [s_nstack].typ = S_TYPE_NUM;
   s_stack [s_nstack].ref = NULL;
   s_stack [s_nstack].num = a_value;
   s_stack [s_nstack].str = NULL;
   /*---(update stack counter)-----------*/
   ++s_nstack;
   /*---(complete)-----------------------*/
   return 0;
}

char         /*-> add a string to the stack ----------[ ------ [gc.420.202.11]*/ /*-[01.0000.0#5.!]-*/ /*-[--.---.---.--]-*/
yCALC_pushstr           (char *a_func, char *a_string)
{
   /*---(defense: stack overflow)--------*/
   if (s_nstack >= S_MAX_STACK) {
      myCALC.trouble = G_ERROR_STACK;
      return 0;
   }
   /*---(update stack item)--------------*/
   s_stack [s_nstack].typ = S_TYPE_STR;
   s_stack [s_nstack].ref = NULL;
   s_stack [s_nstack].num = 0;
   s_stack [s_nstack].str = strndup (a_string, LEN_RECD);
   /*---(update stack counter)-----------*/
   ++s_nstack;
   /*---(complete)-----------------------*/
   return 0;
}

char         /*-> add a reference to the stack -------[ ------ [gc.520.203.21]*/ /*-[01.0000.075.!]-*/ /*-[--.---.---.--]-*/
yCALC_pushref           (char *a_func, void *a_thing)
{
   /*---(defense: stack overflow)--------*/
   if (s_nstack >= S_MAX_STACK) {
      myCALC.trouble = G_ERROR_STACK;
      return 0;
   }
   if (a_thing      == NULL     )  return -2;
   /*---(update stack item)--------------*/
   s_stack[s_nstack].typ = S_TYPE_REF;
   s_stack[s_nstack].ref = a_thing;
   s_stack[s_nstack].num = 0;
   s_stack[s_nstack].str = NULL;
   /*---(update stack counter)-----------*/
   ++s_nstack;
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      removing from stack                     ----===*/
/*====================------------------------------------====================*/
static void      o___POPPING_________________o (void) {;}

double       /*-> get an numeric off the stack -------[ ------ [fn.730.205.21]*/ /*-[01.0000.0#5.!]-*/ /*-[--.---.---.--]-*/
yCALC_popval            (char *a_func)
{  /*---(design notes)-------------------*//*---------------------------------*/
   /* always returns a value for the stack entry.                             */
   /* -- for a numeric literal, it returns the number field on the stack item */
   /* -- for a string literal, it returns 0.0 (since strings have no value)   */
   /* -- for a reference, it returns the value contained in that cell         */
   /* -- if it can't figure it out, it returns a 0.0                          */
   /*---(prepare)------------------------*/
   if (s_nstack <= 0) {
      myCALC.trouble = G_ERROR_STACK;
      return 0.0;
   }
   --s_nstack;
   /*---(handle stack types)-------------*/
   switch (s_stack [s_nstack].typ) {
   case S_TYPE_NUM :
      return  s_stack [s_nstack].num;
      break;
   case S_TYPE_STR :
      return  0.0;
      break;
   case S_TYPE_REF :
      if (g_valuer == NULL) {
         myCALC.trouble = G_ERROR_CONF;
         return 0.0;
      }
      g_valuer (s_stack [s_nstack].ref, &s_type, &s_value, s_string);
      return  s_value;
      break;
   }
   /*---(complete)-----------------------*/
   myCALC.trouble = G_ERROR_STACK;
   return 0.0;
}

char*        /*-> get an string off the stack --------[ ------ [fs.A40.20#.31]*/ /*-[02.0000.0#5.!]-*/ /*-[--.---.---.--]-*/
yCALC_popstr            (char *a_func)
{  /*---(design notes)-------------------*//*---------------------------------*/
   /* always returns a string for the stack entry.                            */
   /* -- numeric literal              , return an empty string            */
   /* -- string literal               , return string from the stack      */
   /* -- reference that's unused      , return an empty string            */
   /* -- reference that's numeric     , return an empty string            */
   /* -- reference that's string      , return an string                  */
   /* -- reference that's num formula , return an empty string            */
   /* -- reference that's str formula , return the modified value         */
   /* -- reference that's unknown     , return an empty string            */
   /* -- if it can't figure it out    , return an empty string            */
   /*---(prepare)------------------------*/
   if (s_nstack <= 0) {
      myCALC.trouble = G_ERROR_STACK;
      return strndup (g_nada, LEN_RECD);
   }
   --s_nstack;
   /*---(handle stack types)-------------*/
   switch (s_stack[s_nstack].typ) {
   case S_TYPE_NUM :
      return  strndup (g_nada, LEN_RECD);
      break;
   case S_TYPE_STR :
      return  s_stack[s_nstack].str;
      break;
   case S_TYPE_REF :
      s_string = NULL;
      if (g_valuer == NULL) {
         myCALC.trouble = G_ERROR_CONF;
         return  strndup (g_nada, LEN_RECD);
      }
      g_valuer (s_stack [s_nstack].ref, &s_type, &s_value, &s_string);
      if (s_string == NULL) {
         myCALC.trouble = G_ERROR_CONF;
         return  strndup (g_nada, LEN_RECD);
      }
      return  strndup (s_string, LEN_RECD);
   }
   /*---(complete)-----------------------*/
   myCALC.trouble = G_ERROR_STACK;
   return strndup (g_nada, LEN_RECD);
}



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
   tCALC      *calc;
};
static tMOCK   s_mocks     [100] = {
   { "0a1"       , 'n' ,     1.00, ""              ,   0,   0,   0, NULL },
   { "0a2"       , 'n' ,     2.00, ""              ,   0,   1,   0, NULL },
   { "0a3"       , 'n' ,     3.00, ""              ,   0,   2,   0, NULL },
   { "0a4"       , 'n' ,     4.00, ""              ,   0,   3,   0, NULL },
   { "0a5"       , 'n' ,     5.00, ""              ,   0,   4,   0, NULL },
   { "0a6"       , 'n' ,     6.00, ""              ,   0,   5,   0, NULL },
   { "0a10"      , 'n' ,    10.00, ""              ,   0,   9,   0, NULL },
   { "0a30"      , 'n' ,    30.00, ""              ,   0,  29,   0, NULL },
   { "0a42"      , 'n' ,    42.00, ""              ,   0,  41,   0, NULL },
   { "1a1"       , 's' ,     0.00, "one"           ,   0,   0,   1, NULL },
   { "1a2"       , 's' ,     0.00, "onetwo"        ,   0,   1,   1, NULL },
   { "1a3"       , 's' ,     0.00, "two"           ,   0,   2,   1, NULL },
   { "1a4"       , 's' ,     0.00, "three"         ,   0,   3,   1, NULL },
   { ""          , 'n' ,     0.00, ""              ,   0,   0,   0, NULL },
};

void*
yCALC__unit_thinger     (char *a_label)
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

char
yCALC__unit_valuer      (void *a_thing, char *a_type, double *a_value, char **a_string)
{
   tMOCK      *x_thing     = NULL;
   x_thing = (tMOCK *) a_thing;
   if (a_type   != NULL)  *a_type   = x_thing->type;
   if (a_value  != NULL)  *a_value  = x_thing->value;
   if (a_string != NULL)  *a_string = x_thing->string;
   return 0;
}

char
yCALC__unit_detailer    (void *a_thing, char *a_quality, char *a_string, double *a_value)
{
   return 0;
}

char
yCALC__unit_addresser   (void *a_thing, int *x, int *y, int *z)
{
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char          yCALC__unit_answer [LEN_STR ];

char*        /*-> unit testing accessor --------------[ light  [us.IA0.2A5.X3]*/ /*-[02.0000.00#.#]-*/ /*-[--.---.---.--]-*/
yCALC__unit_stack       (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   char        x_type      =  '-';
   tMOCK      *x_thing     = NULL;
   char        x_label     [LEN_LABEL];
   /*---(initialize)---------------------*/
   strlcpy (yCALC__unit_answer, "yCALC_unit, unknown request", 100);
   /*---(string testing)-----------------*/
   if      (strncmp (a_question, "top"       , 20)  == 0) {
      if (s_nstack <= 0)   snprintf (yCALC__unit_answer, LEN_STR, "STACK top   (%2d) : %c %8.2lf %-10p %-.30s", s_nstack, S_TYPE_EMPTY, 0.0, NULL, "---");
      else {
         switch (s_stack [s_nstack - 1].typ) {
         case S_TYPE_NUM :
         case S_TYPE_STR :
            snprintf (yCALC__unit_answer, LEN_STR, "STACK top   (%2d) : %c %8.2lf %-10p %-.30s", s_nstack, s_stack [s_nstack - 1].typ, s_stack [s_nstack - 1].num, s_stack [s_nstack - 1].ref, (s_stack [s_nstack - 1].str == NULL) ? "---" : s_stack [s_nstack - 1].str);
            break;
         case S_TYPE_REF :
            x_thing = (tMOCK *) (s_stack [s_nstack - 1].ref);
            strlcpy (x_label, x_thing->label, LEN_LABEL);
            snprintf (yCALC__unit_answer, LEN_STR, "STACK top   (%2d) : %c %8.2lf %-10.10s %-.30s", s_nstack, s_stack [s_nstack - 1].typ, s_stack [s_nstack - 1].num, x_label, (s_stack [s_nstack - 1].str == NULL) ? "---" : s_stack [s_nstack - 1].str);
            break;
         }
      }
   }
   /*---(complete)-----------------------*/
   return yCALC__unit_answer;
}

char yCALC__unit_stackset    (int a_num) { s_nstack = a_num; return 0; }




/*============================----end-of-source---============================*/
