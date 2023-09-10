/*============================----beg-of-source---============================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"



static char    s_type    = '-';
static double  s_value   = 0.0;
static char   *s_string  = NULL;
static char    s_list    [LEN_RECD];



static void       *s_owner       = NULL;
static char        s_narg        =    0;
static int         s_neval       =    0;

int                g_error       =    0;



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

static char        s_typ         = S_TYPE_EMPTY;
static tDEP_ROOT  *s_ref         = NULL;
static double      s_num         =  0.0;
static char       *s_str         = NULL;



/*====================------------------------------------====================*/
/*===----                        program level                         ----===*/
/*====================------------------------------------====================*/
static void      o___PROGRAM_________________o (void) {;}

char
ycalc__exec_stack_grab   (int i)
{
   /*---(locals)-----------+-----+-----+-*/
   DEBUG_YCALC   yLOG_senter  (__FUNCTION__);
   DEBUG_YCALC   yLOG_sint    (i);
   DEBUG_YCALC   yLOG_snote   ("grabbing");
   s_typ = s_stack [i].typ;
   s_ref = s_stack [i].ref;
   s_num = s_stack [i].num;
   s_str = s_stack [i].str;
   DEBUG_YCALC   yLOG_schar   (s_typ);
   DEBUG_YCALC   yLOG_spoint  (s_ref);
   DEBUG_YCALC   yLOG_sint    (s_num);
   DEBUG_YCALC   yLOG_snote   (s_str);
   DEBUG_YCALC   yLOG_snote   ("cleared");
   s_stack [i].typ   = S_TYPE_EMPTY;
   s_stack [i].ref   = NULL;
   s_stack [i].num   = 0.0;
   s_stack [i].str   = NULL;
   DEBUG_YCALC   yLOG_schar   (s_stack [i].typ);
   DEBUG_YCALC   yLOG_spoint  (s_stack [i].ref);
   DEBUG_YCALC   yLOG_sint    (s_stack [i].num);
   DEBUG_YCALC   yLOG_snote   (s_stack [i].str);
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ycalc__exec_stack_clear  (char a_init)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   for (i = 0; i < S_MAX_STACK; ++i) {
      s_stack [i].typ   = S_TYPE_EMPTY;
      s_stack [i].ref   = NULL;
      s_stack [i].num   = 0.0;
      if (a_init != 'y' && s_stack [i].str != NULL)   free (s_stack [i].str);
      s_stack [i].str   = NULL;
   }
   s_nstack = 0;
   /*---(complete)-----------------------*/
   return 0;
}

char
ycalc_exec_init          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(globals)------------------------*/
   DEBUG_YCALC   yLOG_note    ("clearing stack");
   ycalc__exec_stack_clear ('y');
   /*---(globals)------------------------*/
   DEBUG_YCALC   yLOG_note    ("global variables");
   myCALC.deproot = NULL;
   myCALC.owner   = NULL;
   myCALC.me      = NULL;
   myCALC.label   = NULL;
   DEBUG_YCALC   yLOG_note    ("shared variables");
   s_type         = '-';
   s_value        = 0.0;
   s_string       = NULL;
   strcpy (s_list, "");
   s_owner        = NULL;
   s_narg         =    0;
   s_neval        =    0;
   DEBUG_YCALC   yLOG_note    ("error variables");
   g_error        =    0;
   s_typ          = S_TYPE_EMPTY;
   s_ref          = NULL;
   s_num          =  0.0;
   s_str          = NULL;
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc_exec_wrap          (void)
{
   ycalc__exec_stack_clear ('-');
   if (myCALC.me != NULL) {
      free (myCALC.me);
      myCALC.me = NULL;
   }
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        adding to stack                       ----===*/
/*====================------------------------------------====================*/
static void      o___PUSHING_________________o (void) {;}

char         /*-> add a value to the stack -----------[ ------ [gc.420.202.11]*/ /*-[01.0000.0#5.!]-*/ /*-[--.---.---.--]-*/
ycalc_pushval           (char *a_func, double a_value)
{
   /*---(defense: stack overflow)--------*/
   if (s_nstack >= S_MAX_STACK) {
      ycalc_error_set (YCALC_ERROR_STACK, NULL);
      return 0;
   }
   /*---(update stack item)--------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*> DEBUG_YCALC   yLOG_senter  (__FUNCTION__);                                     <*/
   /*> DEBUG_YCALC   yLOG_sint    (s_nstack);                                         <*/
   DEBUG_YCALC   yLOG_value   ("s_nstack"  , s_nstack);
   s_stack [s_nstack].typ = S_TYPE_NUM;
   DEBUG_YCALC   yLOG_value   ("typ"       , s_stack [s_nstack].typ);
   s_stack [s_nstack].ref = NULL;
   s_stack [s_nstack].num = a_value;
   DEBUG_YCALC   yLOG_value   ("num"       , s_stack [s_nstack].num);
   s_stack [s_nstack].str = NULL;
   /*---(update stack counter)-----------*/
   ++s_nstack;
   DEBUG_YCALC   yLOG_value   ("s_nstack"  , s_nstack);
   /*---(complete)-----------------------*/
   /*> DEBUG_YCALC   yLOG_sexit   (__FUNCTION__);                                     <*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> add a string to the stack ----------[ ------ [gc.420.202.11]*/ /*-[01.0000.0#5.!]-*/ /*-[--.---.---.--]-*/
ycalc_pushstr           (char *a_func, char *a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char       *p           = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense: stack overflow)--------*/
   DEBUG_YCALC   yLOG_value   ("s_nstack"  , s_nstack);
   DEBUG_YCALC   yLOG_value   ("MAX"       , S_MAX_STACK);
   --rce;  if (s_nstack >= S_MAX_STACK) {
      ycalc_error_set (YCALC_ERROR_STACK, NULL);
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("a_string"  , a_string);
   --rce;  if (a_string == NULL) {
      ycalc_error_set (YCALC_ERROR_STACK, NULL);
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_info    ("a_string"  , a_string);
   /*---(update stack item)--------------*/
   s_stack [s_nstack].typ = S_TYPE_STR;
   DEBUG_YCALC   yLOG_char    ("typ"       , s_stack [s_nstack].typ);
   s_stack [s_nstack].ref = NULL;
   DEBUG_YCALC   yLOG_point   ("ref"       , s_stack [s_nstack].ref);
   s_stack [s_nstack].num = 0;
   DEBUG_YCALC   yLOG_value   ("num"       , s_stack [s_nstack].num);
   /*> if (s_stack[s_nstack].str != NULL) free (s_stack[s_nstack].str);               <*/
   DEBUG_YCALC   yLOG_note    ("before");
   /*> p = strndup (a_string, LEN_RECD);                                              <*/
   p = strndup (a_string, LEN_RECD);
   DEBUG_YCALC   yLOG_note    ("after");
   /*> DEBUG_YCALC   yLOG_point   ("p"         , p);                                  <*/
   s_stack [s_nstack].str = p;
   /*---(update stack counter)-----------*/
   ++s_nstack;
   DEBUG_YCALC   yLOG_value   ("s_nstack"  , s_nstack);
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> add a reference to the stack -------[ ------ [gc.520.203.21]*/ /*-[01.0000.075.!]-*/ /*-[--.---.---.--]-*/
ycalc_pushref           (char *a_func, void *a_thing, char *a_label)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char       *p           = NULL;
   char       *x_type      = NULL;
   char      **x_string    = NULL;
   tDEP_ROOT  *x_deproot   = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense: stack overflow)--------*/
   DEBUG_YCALC   yLOG_value   ("s_nstack"  , s_nstack);
   DEBUG_YCALC   yLOG_value   ("MAX"       , S_MAX_STACK);
   --rce;  if (s_nstack >= S_MAX_STACK) {
      ycalc_error_set (YCALC_ERROR_STACK, NULL);
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("a_thing"   , a_thing);
   --rce;  if (a_thing  == NULL) {
      ycalc_error_set (YCALC_ERROR_EXEC_NULL, NULL);
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_deproot = (tDEP_ROOT *) a_thing;
   DEBUG_YCALC   yLOG_info    ("label"     , ycalc_call_labeler (x_deproot));
   DEBUG_YCALC   yLOG_point   ("a_label"   , a_label);
   --rce;  if (a_label  == NULL) {
      ycalc_error_set (YCALC_ERROR_STACK, NULL);
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_info    ("a_label"   , a_label);
   DEBUG_YCALC   yLOG_point   ("owner"     , x_deproot->owner);
   --rce;  if (x_deproot->owner == NULL) {
      ycalc_error_set (YCALC_ERROR_EXEC_NULL, x_deproot);
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = myCALC.e_pointer (x_deproot->owner, NULL, &x_type, NULL, &x_string);
   DEBUG_YCALC   yLOG_complex ("type"      , "%c, %4d", *x_type, *x_type);
   DEBUG_YCALC   yLOG_complex ("btype"     , "%c, %4d", x_deproot->btype, x_deproot->btype);
   DEBUG_YCALC   yLOG_value   ("nreq"      , x_deproot->nreq);
   DEBUG_YCALC   yLOG_value   ("npro"      , x_deproot->npro);
   DEBUG_YCALC   yLOG_value   ("ncalc"     , x_deproot->ncalc);
   DEBUG_YCALC   yLOG_info    ("string"    , *x_string);
   --rce;  if (x_deproot->btype == YCALC_DATA_ERROR || *x_type == YCALC_DATA_ERROR) {
      ycalc_error_set (YCALC_ERROR_EXEC_ERR , x_deproot);
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (x_string != NULL && *x_string != NULL) {
      if (strncmp ("#b/", *x_string, 3) == 0 || strncmp ("#e/", *x_string, 3) == 0) {
         ycalc_error_set (YCALC_ERROR_EXEC_ERR , x_deproot);
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(update stack item)--------------*/
   DEBUG_YCALC   yLOG_value   ("s_nstack"  , s_nstack);
   s_stack[s_nstack].typ = S_TYPE_REF;
   s_stack[s_nstack].ref = a_thing;
   s_stack[s_nstack].num = 0;
   p = strndup (a_label, LEN_LABEL);
   DEBUG_YCALC   yLOG_point   ("p"         , p);
   s_stack[s_nstack].str = p;
   /*---(update stack counter)-----------*/
   ++s_nstack;
   DEBUG_YCALC   yLOG_value   ("s_nstack"  , s_nstack);
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      removing from stack                     ----===*/
/*====================------------------------------------====================*/
static void      o___POPPING_________________o (void) {;}

double       /*-> get an numeric off the stack -------[ ------ [fn.730.205.21]*/ /*-[01.0000.0#5.!]-*/ /*-[--.---.---.--]-*/
ycalc_popval            (char *a_func)
{
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   DEBUG_YCALC   yLOG_value   ("s_nstack"  , s_nstack);
   --rce;  if (s_nstack <= 0) {
      ycalc_error_set (YCALC_ERROR_STACK, NULL);
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return 0.0;
   }
   /*---(prepare)------------------------*/
   --s_nstack;
   DEBUG_YCALC   yLOG_value   ("s_nstack"  , s_nstack);
   ycalc__exec_stack_grab (s_nstack);
   if (s_str != NULL)   { free (s_str); s_str = NULL; }
   /*---(handle stack types)-------------*/
   DEBUG_YCALC   yLOG_char    ("s_typ"     , s_typ);
   --rce;  switch (s_typ) {
   case S_TYPE_NUM :
      DEBUG_YCALC   yLOG_note    ("number type, perfect");
      DEBUG_YCALC   yLOG_value   ("s_num"     , s_num);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return s_num;
      break;
   case S_TYPE_STR :
      DEBUG_YCALC   yLOG_note    ("string type, error");
      ycalc_error_set (YCALC_ERROR_EXEC_STR, YCALC_ERROR_LITERAL);
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return  0.0;
      break;
   case S_TYPE_REF :
      DEBUG_YCALC   yLOG_note    ("reference type, look it up");
      if (myCALC.e_valuer == NULL) {
         ycalc_error_set (YCALC_ERROR_CONF, NULL);
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return 0.0;
      }
      DEBUG_YCALC   yLOG_point   ("s_ref"     , s_ref);
      DEBUG_YCALC   yLOG_point   ("owner"     , s_ref->owner);
      myCALC.e_valuer (s_ref->owner, &s_type, &s_value, NULL);
      DEBUG_YCALC   yLOG_char    ("s_type"    , s_type);
      DEBUG_YCALC   yLOG_value   ("s_value"   , s_value);
      switch (s_type) {
      case YCALC_DATA_ADDR   : ycalc_error_set (YCALC_ERROR_EXEC_PTR , s_ref);    break;
      case YCALC_DATA_CADDR  : ycalc_error_set (YCALC_ERROR_EXEC_PTR , s_ref);    break;
      case YCALC_DATA_RLIKE  : ycalc_error_set (YCALC_ERROR_EXEC_PTR , s_ref);    break;
      case YCALC_DATA_RANGE  : ycalc_error_set (YCALC_ERROR_EXEC_PTR , s_ref);    break;
      case YCALC_DATA_BLANK  : ycalc_error_set (YCALC_ERROR_EXEC_NADA, s_ref);    break;
      case YCALC_DATA_ERROR  : ycalc_error_set (YCALC_ERROR_EXEC_ERR , s_ref);    break;
      case YCALC_DATA_STR    : ycalc_error_set (YCALC_ERROR_EXEC_STR , s_ref);    break;
      case YCALC_DATA_SFORM  : ycalc_error_set (YCALC_ERROR_EXEC_STR , s_ref);    break;
      case YCALC_DATA_SLIKE  : ycalc_error_set (YCALC_ERROR_EXEC_STR , s_ref);    break;
      case YCALC_DATA_MERGED : ycalc_error_set (YCALC_ERROR_EXEC_NADA, s_ref);    break;
      }
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return  s_value;
      break;
   default  :
      DEBUG_YCALC   yLOG_note    ("unknown type");
      ycalc_error_set (YCALC_ERROR_EXEC_HUH, NULL);
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return  0.0;
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_note    ("trouble");
   --rce;
   ycalc_error_set (YCALC_ERROR_STACK, NULL);
   DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
   return 0.0;
}

char*        /*-> get an string off the stack --------[ ------ [fs.A40.20#.31]*/ /*-[02.0000.0#5.!]-*/ /*-[--.---.---.--]-*/
ycalc_popstr            (char *a_func)
{
   /*---(local)--------------------------*/
   char        rce         =  -10;
   char       *x_str       = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   DEBUG_YCALC   yLOG_value   ("s_nstack"  , s_nstack);
   --rce;  if (s_nstack <= 0) {
      ycalc_error_set (YCALC_ERROR_STACK, NULL);
      x_str = strndup (g_nada, LEN_RECD);
      DEBUG_YCALC   yLOG_point   ("s_str"     , s_str);
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return x_str;
   }
   /*---(prepare)------------------------*/
   --s_nstack;
   DEBUG_YCALC   yLOG_value   ("s_nstack"  , s_nstack);
   ycalc__exec_stack_grab (s_nstack);
   /*---(handle stack types)-------------*/
   --rce;  switch (s_typ) {
   case S_TYPE_NUM :
      DEBUG_YCALC   yLOG_note    ("from number, so nada");
      if (s_str != NULL)   { free (s_str); s_str = NULL; }
      ycalc_error_set (YCALC_ERROR_EXEC_VAL, YCALC_ERROR_LITERAL);
      x_str = strndup (g_nada, LEN_RECD);
      DEBUG_YCALC   yLOG_point   ("s_str"     , s_str);
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return x_str;
      break;
   case S_TYPE_STR :
      DEBUG_YCALC   yLOG_note    ("from string");
      DEBUG_YCALC   yLOG_point   ("s_str"     , s_str);
      DEBUG_YCALC   yLOG_info    ("s_str"     , s_str);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return s_str;
      break;
   case S_TYPE_REF :
      DEBUG_YCALC   yLOG_note    ("reference type, look it up");
      s_string = NULL;
      if (s_str != NULL)   { free (s_str); s_str = NULL; }
      if (myCALC.e_valuer == NULL) {
         ycalc_error_set (YCALC_ERROR_CONF, NULL);
         x_str = strndup (g_nada, LEN_RECD);
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return x_str;
      }
      DEBUG_YCALC   yLOG_point   ("s_ref"     , s_ref);
      DEBUG_YCALC   yLOG_point   ("owner"     , s_ref->owner);
      myCALC.e_valuer (s_ref->owner, &s_type, NULL, &s_string);
      DEBUG_YCALC   yLOG_char    ("s_type"    , s_type);
      DEBUG_YCALC   yLOG_value   ("s_value"   , s_value);
      switch (s_type) {
      case YCALC_DATA_ADDR   : ycalc_error_set (YCALC_ERROR_EXEC_PTR , s_ref);    break;
      case YCALC_DATA_CADDR  : ycalc_error_set (YCALC_ERROR_EXEC_PTR , s_ref);    break;
      case YCALC_DATA_RLIKE  : ycalc_error_set (YCALC_ERROR_EXEC_PTR , s_ref);    break;
      case YCALC_DATA_RANGE  : ycalc_error_set (YCALC_ERROR_EXEC_PTR , s_ref);    break;
      case YCALC_DATA_BLANK  : ycalc_error_set (YCALC_ERROR_EXEC_NADA, s_ref);    break;
      case YCALC_DATA_ERROR  : ycalc_error_set (YCALC_ERROR_EXEC_ERR , s_ref);    break;
      case YCALC_DATA_NUM    : ycalc_error_set (YCALC_ERROR_EXEC_VAL , s_ref);    break;
      case YCALC_DATA_NFORM  : ycalc_error_set (YCALC_ERROR_EXEC_VAL , s_ref);    break;
      case YCALC_DATA_NLIKE  : ycalc_error_set (YCALC_ERROR_EXEC_VAL , s_ref);    break;
      case YCALC_DATA_MERGED : ycalc_error_set (YCALC_ERROR_EXEC_NADA, s_ref);    break;
      }
      if   (s_string == NULL) x_str = strndup (g_nada  , LEN_RECD);
      else                    x_str = strndup (s_string, LEN_RECD);
      DEBUG_YCALC   yLOG_point   ("s_str"     , s_str);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return x_str;
      break;
   default  :
      DEBUG_YCALC   yLOG_note    ("from other");
      if (s_str != NULL)   { free (s_str); s_str = NULL; }
      ycalc_error_set (YCALC_ERROR_EXEC_HUH, NULL);
      x_str = strndup (g_nada, LEN_RECD);
      DEBUG_YCALC   yLOG_point   ("s_str"     , s_str);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return x_str;
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      break;
   }
   /*---(trouble)------------------------*/
   DEBUG_YCALC   yLOG_note    ("trouble");
   ycalc_error_set (YCALC_ERROR_STACK, NULL);
   x_str = strndup (g_nada, LEN_RECD);
   DEBUG_YCALC   yLOG_point   ("s_str"     , s_str);
   /*---(complete)-----------------------*/
   --rce;
   DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
   return x_str;
}

tDEP_ROOT*   /*-> get an string off the stack --------[ ------ [fs.A40.20#.31]*/ /*-[02.0000.0#5.!]-*/ /*-[--.---.---.--]-*/
ycalc_popref            (char *a_func)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char       *x_type      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   DEBUG_YCALC   yLOG_value   ("s_nstack"  , s_nstack);
   if (s_nstack <= 0) {
      ycalc_error_set (YCALC_ERROR_STACK, NULL);
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return NULL;
   }
   /*---(prepare)------------------------*/
   --s_nstack;
   ycalc__exec_stack_grab (s_nstack);
   if (s_str != NULL)   { free (s_str); s_str = NULL; }
   /*---(handle stack types)-------------*/
   --rce;  switch (s_typ) {
   case S_TYPE_NUM :
      DEBUG_YCALC   yLOG_note    ("found number, not pointer");
      ycalc_error_set (YCALC_ERROR_EXEC_IND , YCALC_ERROR_LITERAL);
      DEBUG_YCALC   yLOG_info    ("1yCALC.me" , myCALC.me);
      return  NULL;
      break;
   case S_TYPE_STR :
      DEBUG_YCALC   yLOG_note    ("found string, not pointer");
      ycalc_error_set (YCALC_ERROR_EXEC_IND , YCALC_ERROR_LITERAL);
      DEBUG_YCALC   yLOG_info    ("2yCALC.me" , myCALC.me);
      return  NULL;
      break;
   case S_TYPE_REF :
      ycalc_error_set (YCALC_ERROR_NONE , s_ref);
      DEBUG_YCALC   yLOG_info    ("3yCALC.me" , myCALC.me);
      DEBUG_YCALC   yLOG_point   ("s_ref"     , s_ref);
      if (s_ref == NULL) {
         ycalc_error_set (YCALC_ERROR_EXEC_NULL, s_ref);
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return NULL;
      }
      DEBUG_YCALC   yLOG_info    ("label"     , ycalc_call_labeler (s_ref));
      DEBUG_YCALC   yLOG_point   ("owner"     , s_ref->owner);
      if (s_ref->owner == NULL) {
         ycalc_error_set (YCALC_ERROR_EXEC_NULL, s_ref);
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return NULL;
      }
      rc = myCALC.e_pointer (s_ref->owner, NULL, &x_type, NULL, NULL);
      DEBUG_YCALC   yLOG_char    ("type"      , x_type);
      DEBUG_YCALC   yLOG_char    ("btype"     , s_ref->btype);
      if (s_ref->btype == YCALC_DATA_ERROR || x_type == YCALC_DATA_ERROR) {
         ycalc_error_set (YCALC_ERROR_EXEC_ERR , s_ref);
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return NULL;
      }
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return s_ref;
      break;
   default  :
      DEBUG_YCALC   yLOG_note    ("found unknown, not pointer");
      ycalc_error_set (YCALC_ERROR_EXEC_HUH, YCALC_ERROR_UNKNOWN);
      DEBUG_YCALC   yLOG_info    ("4yCALC.me" , myCALC.me);
      return  NULL;
      break;
   }
   /*---(complete)-----------------------*/
   ycalc_error_set (YCALC_ERROR_STACK, NULL);
   DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
   return NULL;
}

int          /*-> get an string off the stack --------[ ------ [fs.A40.20#.31]*/ /*-[02.0000.0#5.!]-*/ /*-[--.---.---.--]-*/
ycalc_popval_plus       (char *a_func, char a_what)
{
   /*---(local)--------------------------*/
   int         u, x, y, z;
   /*---(prepare)------------------------*/
   if (s_nstack <= 0) {
      ycalc_error_set (YCALC_ERROR_STACK, NULL);
      return 0;
   }
   /*---(prepare)------------------------*/
   --s_nstack;
   ycalc__exec_stack_grab (s_nstack);
   if (s_str != NULL)   { free (s_str); s_str = NULL; }
   /*---(handle stack types)-------------*/
   switch (s_typ) {
   case S_TYPE_NUM :
   case S_TYPE_STR :
      ycalc_error_set (YCALC_ERROR_EXEC_REF, NULL);
      return 0;
      break;
   case S_TYPE_REF :
      if (myCALC.e_valuer == NULL) {
         ycalc_error_set (YCALC_ERROR_CONF, NULL);
         return 0;
      }
      switch (a_what) {
      case G_SPECIAL_TYPE   :
         myCALC.e_valuer  (s_ref->owner, &s_type, NULL, NULL);
         return s_type;
         break;
      case G_SPECIAL_NCALC  :
         return s_ref->ncalc;
         break;
      case G_SPECIAL_NPRO   :
         return s_ref->npro;
         break;
      case G_SPECIAL_NREQ   :
         return s_ref->nreq;
         break;
      case G_SPECIAL_LEVEL  :
         return s_ref->slevel;
         break;
      case G_SPECIAL_UPOS   :
         myCALC.e_addresser (s_ref->owner, &u, &x, &y, &z);
         return u;
         break;
      case G_SPECIAL_XPOS   :
         myCALC.e_addresser (s_ref->owner, &u, &x, &y, &z);
         return x;
         break;
      case G_SPECIAL_YPOS   :
         myCALC.e_addresser (s_ref->owner, &u, &x, &y, &z);
         return y;
         break;
      case G_SPECIAL_ZPOS   :
         myCALC.e_addresser (s_ref->owner, &u, &x, &y, &z);
         return z;
         break;
      case G_SPECIAL_ALLPOS :
         myCALC.e_addresser (s_ref->owner, &u, &x, &y, &z);
         s_buf = u;
         m     = x;
         n     = y;
         o     = z;
         return 0;
         break;
      default  :
         ycalc_error_set (YCALC_ERROR_EXEC_OPT , s_ref);
         return  0;
         break;
      }
   default  :
      ycalc_error_set (YCALC_ERROR_EXEC_HUH, NULL);
      return  0;
      break;
   }
   /*---(complete)-----------------------*/
   ycalc_error_set (YCALC_ERROR_STACK, NULL);
   return strndup (g_nada, LEN_RECD);
}

char*        /*-> get an string off the stack --------[ ------ [fs.A40.20#.31]*/ /*-[02.0000.0#5.!]-*/ /*-[--.---.---.--]-*/
ycalc_popstr_plus       (char *a_func, char a_what)
{
   char        rce         =  -10;
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   DEBUG_YCALC   yLOG_value   ("s_nstack"  , s_nstack);
   --rce;  if (s_nstack <= 0) {
      ycalc_error_set (YCALC_ERROR_STACK, NULL);
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return strndup (g_nada, LEN_RECD);
   }
   /*---(prepare)------------------------*/
   --s_nstack;
   ycalc__exec_stack_grab (s_nstack);
   if (s_str != NULL)   { free (s_str); s_str = NULL; }
   /*---(handle wrong types)-------------*/
   --rce;  switch (s_typ) {
   case S_TYPE_NUM : case S_TYPE_STR :
      DEBUG_YCALC   yLOG_note    ("found number/string, not compatible");
      ycalc_error_set (YCALC_ERROR_EXEC_REF, NULL);
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return  strndup (g_nada, LEN_RECD);
      break;
   case S_TYPE_REF :
      break;
   default         :
      DEBUG_YCALC   yLOG_note    ("found other non-ref, not compatible");
      ycalc_error_set (YCALC_ERROR_EXEC_HUH, NULL);
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return strndup (g_nada, LEN_RECD);
      break;
   }
   /*---(handle stack types)-------------*/
   s_string = NULL;
   if (myCALC.e_valuer  == NULL || myCALC.e_special == NULL) {
      DEBUG_YCALC   yLOG_note    ("no e_valuer or e_special");
      ycalc_error_set (YCALC_ERROR_CONF, NULL);
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return  strndup (g_nada, LEN_RECD);
   }
   /*---(handle return types)------------*/
   switch (a_what) {
   case G_SPECIAL_SOURCE :
      myCALC.e_special (s_ref->owner, G_SPECIAL_SOURCE, NULL, &s_string);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      if (s_string == NULL) return  strndup (g_nada  , LEN_RECD);
      else                  return  strndup (s_string, LEN_RECD);
      break;
   case G_SPECIAL_LABEL  :
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return strndup (ycalc_call_labeler (s_ref), LEN_RECD);
      break;
   case G_SPECIAL_PRINT  :
      myCALC.e_special (s_ref->owner, G_SPECIAL_PRINT , NULL, &s_string);
      DEBUG_YCALC   yLOG_point   ("s_string"  , s_string);
      DEBUG_YCALC   yLOG_info    ("s_string"  , s_string);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      if (s_string == NULL) return  strdup (g_nada);
      else                  return  strdup (s_string);
      break;
   case G_SPECIAL_RPN    :
      s_string = s_ref->rpn;
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      if (s_string == NULL) return  strndup (g_nada  , LEN_RECD);
      else                  return  strndup (s_string, LEN_RECD);
      break;
   case G_SPECIAL_PROS   :
      yCALC_disp_pros      (s_ref, s_list);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      if (s_list   == NULL) return  strndup (g_nada  , LEN_RECD);
      else                  return  strndup (s_list  , LEN_RECD);
      break;
   case G_SPECIAL_REQS   :
      yCALC_disp_reqs      (s_ref, s_list);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      if (s_list   == NULL) return  strndup (g_nada  , LEN_RECD);
      else                  return  strndup (s_list  , LEN_RECD);
      break;
   case G_SPECIAL_LIKE   :
      yCALC_disp_like      (s_ref, s_list);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      if (s_list   == NULL) return  strndup (g_nada  , LEN_RECD);
      else                  return  strndup (s_list  , LEN_RECD);
      break;
   default               :
      myCALC.e_special (s_ref->owner, a_what, NULL, &s_string);
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      if (s_string == NULL) return  strndup (g_nada  , LEN_RECD);
      else                  return  strndup (s_string, LEN_RECD);
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_note    ("completely unhandled");
   ycalc_error_set (YCALC_ERROR_STACK, NULL);
   --rce;
   DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
   return strndup (g_nada, LEN_RECD);
}



/*====================------------------------------------====================*/
/*===----                        driver support                        ----===*/
/*====================------------------------------------====================*/
static void      o___SUPPORT_________________o (void) {;}

char
ycalc__exec_prepare     (tDEP_ROOT *a_deproot, char *a_type, double *a_value, char **a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_label     [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(check return type)--------------*/
   DEBUG_YCALC   yLOG_point   ("a_type"    , a_type);
   --rce;  if (a_type == NULL) {
      DEBUG_YCALC   yLOG_note    ("calculation result available");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_char    ("*a_type"   , *a_type);
   DEBUG_YCALC   yLOG_char    ("btype"     , a_deproot->btype);
   *a_type = a_deproot->btype;   /* reset to build (pre-exec) status */
   DEBUG_YCALC   yLOG_char    ("*a_type"   , *a_type);
   --rce;  if (*a_type == YCALC_DATA_ERROR) {
      DEBUG_YCALC   yLOG_note    ("build was in error, can not run");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_info    ("valid"     , YCALC_GROUP_CALC);
   --rce;  if (strchr (YCALC_GROUP_CALC, *a_type) == NULL) {
      DEBUG_YCALC   yLOG_note    ("not a calculated type");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(prepare)------------------------*/
   ycalc__exec_stack_clear ('-');
   *a_value = 0.0;
   if (*a_string != NULL) {
      free (*a_string);
      *a_string = NULL;
   }
   /*---(check calculation)--------------*/
   DEBUG_YCALC   yLOG_point   ("chead"     , a_deproot->chead);
   --rce;  if (a_deproot->chead == NULL) {
      DEBUG_YCALC   yLOG_note    ("calculation is null, never built");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(reset globals)------------------*/
   s_owner  = a_deproot->owner;
   s_neval  = 0;
   ycalc_error_clear ();
   /*---(clear calcrefs)-----------------*/
   ycalc_deps_delcalcref (a_deproot);
   /*> ycalc__mock_list ();                                                           <*/
   /*---(complete)-------------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc__exec_wrap        (char *a_type, double *a_value, char **a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char       *p           = NULL;
   char        x_label     [LEN_LABEL] = "";
   tDEP_ROOT  *x_ref       = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*> ycalc__mock_list ();                                                           <*/
   /*---(check errors)-------------------*/
   DEBUG_YCALC   yLOG_value   ("g_error"   , g_error);
   --rce;  if (ycalc_error_true ()) {
      *a_type = YCALC_DATA_ERROR;
      DEBUG_YCALC   yLOG_info    ("myCALC.me" , myCALC.me);
      ycalc_error_finalize (g_error, a_type, a_value, a_string, myCALC.me);
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(stack)--------------------------*/
   --rce;  if (s_nstack < 1) {
      DEBUG_YCALC   yLOG_note    ("nothing left in stack to retrieve at end");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   --rce;  if (s_nstack > 1) {
      DEBUG_YCALC   yLOG_note    ("extra data in stack at end");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(results)------------------------*/
   switch (*a_type) {
   case YCALC_DATA_NFORM : case YCALC_DATA_NLIKE : case YCALC_DATA_MATH  :
      DEBUG_YCALC   yLOG_note    ("handling as numeric");
      *a_value = ycalc_popval (__FUNCTION__);
      --rce;  if (ycalc_error_true ()) {
         *a_type = YCALC_DATA_ERROR;
         ycalc_error_finalize (g_error, a_type, a_value, a_string, myCALC.me);
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_YCALC   yLOG_value   ("value"     , *a_value);
      break;
   case YCALC_DATA_SFORM : case YCALC_DATA_SLIKE :
      DEBUG_YCALC   yLOG_note    ("handling as string");
      *a_string = ycalc_popstr (__FUNCTION__);
      --rce;  if (ycalc_error_true ()) {
         *a_type = YCALC_DATA_ERROR;
         ycalc_error_finalize (g_error, a_type, a_value, a_string, myCALC.me);
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_YCALC   yLOG_info    ("string"    , *a_string);
      break;
   case YCALC_DATA_CADDR : case YCALC_DATA_RLIKE :
      DEBUG_YCALC   yLOG_note    ("handling as calc address");
      x_ref = ycalc_popref (__FUNCTION__);
      DEBUG_YCALC   yLOG_point   ("x_ref"     , x_ref);
      if (x_ref == NULL) {
         *a_type = YCALC_DATA_ERROR;
         ycalc_error_finalize (g_error, a_type, a_value, a_string, myCALC.me);
         DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
         return 0;
      }
      *a_string = strndup (ycalc_call_labeler (x_ref), LEN_LABEL);
      --rce;  if (ycalc_error_true ()) {
         *a_type = YCALC_DATA_ERROR;
         ycalc_error_finalize (g_error, a_type, a_value, a_string, myCALC.me);
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_YCALC   yLOG_info    ("string"    , *a_string);
      break;
   }
   /*> ycalc__mock_list ();                                                           <*/
   /*---(complete)-------------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc__exec_free        (void)
{
   myCALC.deproot = NULL;
   myCALC.owner   = NULL;
   if (myCALC.label != NULL) {
      free (myCALC.label);
      myCALC.label   = NULL;
   }
   myCALC.label = NULL;
   if (myCALC.me    != NULL) {
      free (myCALC.me);
      myCALC.me      = NULL;
   }
   return 0;
}

char
ycalc__exec_globals     (tDEP_ROOT *a_deproot)
{
   ycalc__exec_free  ();
   myCALC.deproot = a_deproot;
   myCALC.owner   = a_deproot->owner;
   myCALC.label   = strdup (ycalc_call_labeler (a_deproot));
   myCALC.me      = strdup ("???");
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     execution driver                         ----===*/
/*====================------------------------------------====================*/
static void      o___DRIVER__________________o (void) {;}

char         /*-> evaluate a calculation -------------[ ------ [ge.J95.146.A6]*/ /*-[02.0000.204.!]-*/ /*-[--.---.---.--]-*/
ycalc_execute_trusted   (tDEP_ROOT *a_deproot, char *a_type, double *a_value, char **a_string)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tCALC      *x_calc      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   rc = ycalc__exec_prepare (a_deproot, a_type, a_value, a_string);
   DEBUG_YCALC   yLOG_value   ("prepare"   , rc);
   --rce;  if (rc < 0)  {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   ycalc__exec_globals (a_deproot);
   x_calc   = a_deproot->chead;
   DEBUG_YCALC   yLOG_value   ("ncalc"     , a_deproot->ncalc);
   DEBUG_YCALC   yLOG_value   ("nstack"    , s_nstack);
   /*---(main loop)----------------------*/
   while (x_calc != NULL) {
      ++s_neval;
      DEBUG_YCALC   yLOG_complex ("exec_step" , "typ=%c, val=%F, str=%-9p, ref=%-9p, fnc=%-9p", x_calc->t, x_calc->v, x_calc->s, x_calc->r, x_calc->f);
      s_narg = 0;
      switch (x_calc->t) {
      case G_TYPE_VAL  :
         rc = ycalc_pushval (__FUNCTION__, x_calc->v);
         DEBUG_YCALC   yLOG_value   ("pushval"   , s_nstack);
         break;
      case G_TYPE_STR  :
         rc = ycalc_pushstr (__FUNCTION__, x_calc->s);
         DEBUG_YCALC   yLOG_value   ("pushstr"   , s_nstack);
         break;
      case G_TYPE_REF  :
         rc = ycalc_pushref (__FUNCTION__, x_calc->r, x_calc->s);
         DEBUG_YCALC   yLOG_value   ("pushref"   , s_nstack);
         break;
      case G_TYPE_FUNC :
         x_calc->f();
         DEBUG_YCALC   yLOG_value   ("func()"    , s_nstack);
         break;
      case G_TYPE_NOOP :
         DEBUG_YCALC   yLOG_value   ("noop"      , s_nstack);
         break;
      default  :
         DEBUG_YCALC   yLOG_value   ("error"     , s_nstack);
         ycalc_error_set (YCALC_ERROR_EXEC_STEP, NULL);
         break;
      }
      if (rc < 0 || ycalc_error_true ()) {
         DEBUG_YCALC   yLOG_value   ("ERROR"     , g_error);
         DEBUG_YCALC   yLOG_info    ("myCALC.me" , myCALC.me);
         break;
      }
      x_calc = x_calc->next;
   }
   /*---(check results)------------------*/
   rc = ycalc__exec_wrap    (a_type, a_value, a_string);
   DEBUG_YCALC   yLOG_value   ("wrap"      , rc);
   ycalc__exec_free    ();
   --rce;  if (rc < 0)  {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc_execute_detail    (tDEP_ROOT *a_deproot, char **a_source, char *a_type, double *a_value, char **a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;           /* return code for errors         */
   char        rc          =    0;           /* return code for errors         */
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (ycalc_not_ready ()) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(defense)------------------------*/
   DEBUG_YCALC   yLOG_point   ("a_deproot" , a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_YCALC   yLOG_note    ("a_deproot not set, nothing to do");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ycalc_shared_verify (a_source, a_type, a_value, a_string);
   DEBUG_YCALC   yLOG_value   ("verify"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ycalc_execute_trusted (a_deproot, a_type, a_value, a_string);
   DEBUG_YCALC   yLOG_value   ("trusted"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc_execute_owner     (void *a_owner, tDEP_ROOT *a_deproot)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;           /* return code for errors         */
   char        rc          =    0;           /* return code for errors         */
   char      **x_source    = NULL;
   char       *x_type      = NULL;
   double     *x_value     = NULL;
   char      **x_string    = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (ycalc_not_ready ()) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(defense)------------------------*/
   DEBUG_YCALC   yLOG_point   ("a_owner"   , a_owner);
   --rce;  if (a_owner == NULL) {
      DEBUG_YCALC   yLOG_note    ("a_owner not set, nothing to do");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("a_deproot" , a_deproot);
   /*---(fill pointers)------------------*/
   rc = myCALC.e_pointer (a_owner, &x_source, &x_type, &x_value, &x_string);
   DEBUG_YCALC   yLOG_value   ("pointer"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ycalc_execute_detail      (a_deproot, x_source, x_type, x_value, x_string);
   DEBUG_YCALC   yLOG_value   ("detail"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc_execute_label     (char *a_label)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;           /* return code for errors         */
   char        rc          =    0;           /* return code for errors         */
   void       *x_owner     = NULL;
   tDEP_ROOT  *x_deproot   = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (ycalc_not_ready ()) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get owner/deproot)--------------*/
   rc = myCALC.e_who_named  (a_label, YCALC_OWNR, &x_owner, &x_deproot);
   DEBUG_YCALC   yLOG_value   ("who_named"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(call classify)------------------*/
   rc = ycalc_execute_owner (x_owner, x_deproot);
   DEBUG_YCALC   yLOG_value   ("owner"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char yCALC_exec              (char *a_label) { return ycalc_execute_label (a_label); }

char
ycalc_execute_auto      (void *a_owner, tDEP_ROOT *a_deproot, int a_seq, int a_lvl)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;           /* return code for errors         */
   char        rc          =    0;           /* return code for errors         */
   char      **x_source    = NULL;
   char       *x_type      = NULL;
   double     *x_value     = NULL;
   char      **x_string    = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   DEBUG_YCALC   yLOG_point   ("a_owner"    , a_owner);
   DEBUG_YCALC   yLOG_point   ("a_deproot"  , a_deproot);
   DEBUG_YCALC   yLOG_info    ("autocalc"   , ycalc_call_labeler (a_deproot));
   /*---(fill pointers)------------------*/
   rc = myCALC.e_pointer (a_owner, &x_source, &x_type, &x_value, &x_string);
   DEBUG_YCALC   yLOG_value   ("pointer"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(execute)------------------------*/
   rc = ycalc_execute_detail      (a_deproot, x_source, x_type, x_value, x_string);
   DEBUG_YCALC   yLOG_value   ("detail"    , rc);
   /*---(printable)----------------------*/
   rc = myCALC.e_printer (a_owner);
   DEBUG_YCALC   yLOG_value   ("printer"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char          ycalc__unit_answer [LEN_RECD];

char*        /*-> unit testing accessor --------------[ light  [us.IA0.2A5.X3]*/ /*-[02.0000.00#.#]-*/ /*-[--.---.---.--]-*/
ycalc__unit_stack       (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   char        x_type      =  '-';
   tDEP_ROOT  *x_deproot   = NULL;
   char        x_label     [LEN_LABEL];
   /*---(initialize)---------------------*/
   ystrlcpy (ycalc__unit_answer, "yCALC_unit, unknown request", 100);
   /*---(string testing)-----------------*/
   if      (strncmp (a_question, "top"       , 20)  == 0) {
      if (s_nstack <= 0)   snprintf (ycalc__unit_answer, LEN_FULL, "STACK top   (%2d) : %c %8.2lf %-10p %-.30s", s_nstack, S_TYPE_EMPTY, 0.0, NULL, "---");
      else {
         switch (s_stack [s_nstack - 1].typ) {
         case S_TYPE_NUM :
         case S_TYPE_STR :
            snprintf (ycalc__unit_answer, LEN_FULL, "STACK top   (%2d) : %c %8.2lf %-10p %-.30s", s_nstack, s_stack [s_nstack - 1].typ, s_stack [s_nstack - 1].num, s_stack [s_nstack - 1].ref, (s_stack [s_nstack - 1].str == NULL) ? "---" : s_stack [s_nstack - 1].str);
            break;
         case S_TYPE_REF :
            x_deproot = s_stack [s_nstack - 1].ref;
            ystrlcpy (x_label, ycalc__mock_labeler (x_deproot->owner), LEN_LABEL);
            snprintf (ycalc__unit_answer, LEN_FULL, "STACK top   (%2d) : %c %8.2lf %-10.10s %-.30s", s_nstack, s_stack [s_nstack - 1].typ, s_stack [s_nstack - 1].num, x_label, (s_stack [s_nstack - 1].str == NULL) ? "---" : s_stack [s_nstack - 1].str);
            break;
         }
      }
   }
   /*---(complete)-----------------------*/
   return ycalc__unit_answer;
}

char ycalc__unit_stackset    (int a_num) { s_nstack = a_num; return 0; }




/*============================----end-of-source---============================*/
