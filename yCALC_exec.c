/*============================----beg-of-source---============================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"



static char    s_type    = '-';
static double  s_value   = 0.0;
static char   *s_string  = NULL;
static int     x, y, z;
static char    s_list    [LEN_RECD];



static void       *s_owner       = NULL;
static char        s_narg        =    0;
static int         s_nerror      =    0;
static int         s_nbuild      =    0;
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



/*====================------------------------------------====================*/
/*===----                        program level                         ----===*/
/*====================------------------------------------====================*/
static void      o___PROGRAM_________________o (void) {;}

char
ycalc__exec_stack_clear  (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   for (i = 0; i < S_MAX_STACK; ++i) {
      s_stack [i].typ   = S_TYPE_EMPTY;
      s_stack [i].ref   = NULL;
      s_stack [i].num   = 0.0;
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
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(globals)------------------------*/
   DEBUG_PROG   yLOG_note    ("clearing stack");
   ycalc__exec_stack_clear ();
   /*---(globals)------------------------*/
   DEBUG_PROG   yLOG_note    ("global variables");
   myCALC.deproot = NULL;
   myCALC.owner   = NULL;
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
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
      myCALC.trouble = YCALC_ERROR_STACK;
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
ycalc_pushstr           (char *a_func, char *a_string)
{
   /*---(defense: stack overflow)--------*/
   if (s_nstack >= S_MAX_STACK) {
      myCALC.trouble = YCALC_ERROR_STACK;
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
ycalc_pushref           (char *a_func, void *a_thing)
{
   /*---(defense: stack overflow)--------*/
   if (s_nstack >= S_MAX_STACK) {
      myCALC.trouble = YCALC_ERROR_STACK;
      return 0;
   }
   if (a_thing      == NULL     ) {
      return 0;
   }
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
ycalc_popval            (char *a_func)
{  /*---(design notes)-------------------*//*---------------------------------*/
   /* always returns a value for the stack entry.                             */
   /* -- for a numeric literal, it returns the number field on the stack item */
   /* -- for a string literal, it returns 0.0 (since strings have no value)   */
   /* -- for a reference, it returns the value contained in that cell         */
   /* -- if it can't figure it out, it returns a 0.0                          */
   /*---(local)--------------------------*/
   tDEP_ROOT  *x_deproot   = NULL;
   /*---(prepare)------------------------*/
   if (s_nstack <= 0) {
      myCALC.trouble = YCALC_ERROR_STACK;
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
         myCALC.trouble = YCALC_ERROR_CONF;
         return 0.0;
      }
      x_deproot = s_stack [s_nstack].ref;
      g_valuer (x_deproot->owner, &s_type, &s_value, NULL);
      if (s_type == YCALC_DATA_ADDR)   g_error = YCALC_ERROR_EXEC_PTR;
      if (s_type == YCALC_DATA_RANGE)  g_error = YCALC_ERROR_EXEC_PTR;
      return  s_value;
      break;
   }
   /*---(complete)-----------------------*/
   myCALC.trouble = YCALC_ERROR_STACK;
   return 0.0;
}

char*        /*-> get an string off the stack --------[ ------ [fs.A40.20#.31]*/ /*-[02.0000.0#5.!]-*/ /*-[--.---.---.--]-*/
ycalc_popstr            (char *a_func)
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
   /*---(local)--------------------------*/
   tDEP_ROOT  *x_deproot   = NULL;
   /*---(prepare)------------------------*/
   if (s_nstack <= 0) {
      myCALC.trouble = YCALC_ERROR_STACK;
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
         myCALC.trouble = YCALC_ERROR_CONF;
         return  strndup (g_nada, LEN_RECD);
      }
      x_deproot = s_stack [s_nstack].ref;
      g_valuer (x_deproot->owner, &s_type, NULL, &s_string);
      if (s_type == YCALC_DATA_ADDR)   g_error = YCALC_ERROR_EXEC_PTR;
      if (s_type == YCALC_DATA_RANGE)  g_error = YCALC_ERROR_EXEC_PTR;
      if (s_string == NULL) return  strndup (g_nada  , LEN_RECD);
      else                  return  strndup (s_string, LEN_RECD);
   }
   /*---(complete)-----------------------*/
   myCALC.trouble = YCALC_ERROR_STACK;
   return strndup (g_nada, LEN_RECD);
}

tDEP_ROOT*   /*-> get an string off the stack --------[ ------ [fs.A40.20#.31]*/ /*-[02.0000.0#5.!]-*/ /*-[--.---.---.--]-*/
ycalc_popref            (char *a_func)
{
   /*---(local)--------------------------*/
   tDEP_ROOT  *x_deproot   = NULL;
   /*---(prepare)------------------------*/
   if (s_nstack <= 0) {
      myCALC.trouble = YCALC_ERROR_STACK;
      return strndup (g_nada, LEN_RECD);
   }
   --s_nstack;
   /*---(handle stack types)-------------*/
   switch (s_stack[s_nstack].typ) {
   case S_TYPE_NUM :
      g_error        = YCALC_ERROR_EXEC_PTR;
      myCALC.trouble = YCALC_ERROR_EXEC_PTR;
      return  NULL;
      break;
   case S_TYPE_STR :
      g_error        = YCALC_ERROR_EXEC_PTR;
      myCALC.trouble = YCALC_ERROR_EXEC_PTR;
      return  NULL;
      break;
   case S_TYPE_REF :
      return s_stack [s_nstack].ref;
   }
   /*---(complete)-----------------------*/
   myCALC.trouble = YCALC_ERROR_STACK;
   return NULL;
}

int          /*-> get an string off the stack --------[ ------ [fs.A40.20#.31]*/ /*-[02.0000.0#5.!]-*/ /*-[--.---.---.--]-*/
ycalc_popval_plus       (char *a_func, char a_what)
{
   /*---(local)--------------------------*/
   tDEP_ROOT  *x_deproot   = NULL;
   /*---(prepare)------------------------*/
   if (s_nstack <= 0) {
      myCALC.trouble = YCALC_ERROR_STACK;
      return strndup (g_nada, LEN_RECD);
   }
   --s_nstack;
   /*---(handle stack types)-------------*/
   switch (s_stack[s_nstack].typ) {
   case S_TYPE_NUM :
   case S_TYPE_STR :
      return  0;
      break;
   case S_TYPE_REF :
      if (g_valuer == NULL) {
         myCALC.trouble = YCALC_ERROR_CONF;
         return  strndup (g_nada, LEN_RECD);
      }
      x_deproot = s_stack [s_nstack].ref;
      switch (a_what) {
      case G_SPECIAL_TYPE   :
         g_valuer  (x_deproot->owner, &s_type, NULL, NULL);
         return s_type;
         break;
      case G_SPECIAL_NCALC  :
         return x_deproot->ncalc;
         break;
      case G_SPECIAL_NPRO   :
         return x_deproot->npro;
         break;
      case G_SPECIAL_NREQ   :
         return x_deproot->nreq;
         break;
      case G_SPECIAL_LEVEL  :
         return x_deproot->slevel;
         break;
      case G_SPECIAL_XPOS   :
         g_addresser (x_deproot->owner, &x, &y, &z);
         return x;
         break;
      case G_SPECIAL_YPOS   :
         g_addresser (x_deproot->owner, &x, &y, &z);
         return y;
         break;
      case G_SPECIAL_ZPOS   :
         g_addresser (x_deproot->owner, &x, &y, &z);
         return z;
         break;
      case G_SPECIAL_ALLPOS :
         g_addresser (x_deproot->owner, &x, &y, &z);
         m = x;
         n = y;
         o = z;
         return 0;
         break;
      }
   }
   /*---(complete)-----------------------*/
   myCALC.trouble = YCALC_ERROR_STACK;
   return strndup (g_nada, LEN_RECD);
}

char*        /*-> get an string off the stack --------[ ------ [fs.A40.20#.31]*/ /*-[02.0000.0#5.!]-*/ /*-[--.---.---.--]-*/
ycalc_popstr_plus       (char *a_func, char a_what)
{
   /*---(local)--------------------------*/
   tDEP_ROOT  *x_deproot   = NULL;
   /*---(prepare)------------------------*/
   if (s_nstack <= 0) {
      myCALC.trouble = YCALC_ERROR_STACK;
      return strndup (g_nada, LEN_RECD);
   }
   --s_nstack;
   /*---(handle stack types)-------------*/
   switch (s_stack[s_nstack].typ) {
   case S_TYPE_NUM :
   case S_TYPE_STR :
      return  strndup (g_nada, LEN_RECD);
      break;
   case S_TYPE_REF :
      s_string = NULL;
      if (g_valuer  == NULL || g_special == NULL) {
         myCALC.trouble = YCALC_ERROR_CONF;
         return  strndup (g_nada, LEN_RECD);
      }
      x_deproot = s_stack [s_nstack].ref;
      switch (a_what) {
      case G_SPECIAL_SOURCE :
         g_special (x_deproot->owner, G_SPECIAL_SOURCE, NULL, &s_string);
         if (s_string == NULL) return  strndup (g_nada  , LEN_RECD);
         else                  return  strndup (s_string, LEN_RECD);
         break;
      case G_SPECIAL_LABEL  :
         return strndup (ycalc_call_labeler (x_deproot), LEN_RECD);
         break;
      case G_SPECIAL_PRINT  :
         g_special (x_deproot->owner, G_SPECIAL_PRINT , NULL, &s_string);
         if (s_string == NULL) return  strndup (g_nada  , LEN_RECD);
         else                  return  strndup (s_string, LEN_RECD);
         break;
      case G_SPECIAL_RPN    :
         s_string = x_deproot->rpn;
         if (s_string == NULL) return  strndup (g_nada  , LEN_RECD);
         else                  return  strndup (s_string, LEN_RECD);
         break;
      case G_SPECIAL_PROS   :
         yCALC_disp_pros      (x_deproot, s_list);
         if (s_list   == NULL) return  strndup (g_nada  , LEN_RECD);
         else                  return  strndup (s_list  , LEN_RECD);
         break;
      case G_SPECIAL_REQS   :
         yCALC_disp_reqs      (x_deproot, s_list);
         if (s_list   == NULL) return  strndup (g_nada  , LEN_RECD);
         else                  return  strndup (s_list  , LEN_RECD);
         break;
      case G_SPECIAL_LIKE   :
         yCALC_disp_like      (x_deproot, s_list);
         if (s_list   == NULL) return  strndup (g_nada  , LEN_RECD);
         else                  return  strndup (s_list  , LEN_RECD);
         break;
      default               :
         g_special (x_deproot->owner, a_what, NULL, &s_string);
         if (s_string == NULL) return  strndup (g_nada  , LEN_RECD);
         else                  return  strndup (s_string, LEN_RECD);
         break;
      }
   }
   /*---(complete)-----------------------*/
   myCALC.trouble = YCALC_ERROR_STACK;
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
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(check return type)--------------*/
   DEBUG_CALC   yLOG_point   ("a_type"    , a_type);
   --rce;  if (a_type == NULL) {
      DEBUG_CALC   yLOG_note    ("calculation result available");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_char    ("*a_type"   , *a_type);
   --rce;  if (*a_type == YCALC_DATA_ERROR) {
      DEBUG_CALC   yLOG_note    ("build was in error, can not run");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_info    ("valid"     , YCALC_GROUP_CALC);
   --rce;  if (strchr (YCALC_GROUP_CALC, *a_type) == NULL) {
      DEBUG_CALC   yLOG_note    ("not a calculated type");
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(prepare)------------------------*/
   ycalc__exec_stack_clear ();
   *a_value = 0.0;
   if (*a_string != NULL)  *a_string = strndup ("", LEN_RECD);
   /*---(check calculation)--------------*/
   DEBUG_CALC   yLOG_point   ("chead"     , a_deproot->chead);
   --rce;  if (a_deproot->chead == NULL) {
      DEBUG_CALC   yLOG_note    ("calculation is null, never built");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(reset globals)------------------*/
   s_owner  = a_deproot->owner;
   s_neval  = 0;
   g_error  = 0;
   /*---(clear calcrefs)-----------------*/
   ycalc_deps_delcalcref (a_deproot);
   /*---(complete)-------------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc__exec_wrap        (char *a_type, double *a_value, char **a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_label     [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(check errors)-------------------*/
   DEBUG_CALC   yLOG_value   ("g_error"   , g_error);
   --rce;  if (g_error !=  0) {
      *a_type = YCALC_DATA_ERROR;
      ycalc_handle_error (g_error, a_type, a_value, a_string, "");
      /*> a_curr->t = YCALC_DATA_ERROR;                                                            <* 
       *> a_curr->v_str = strndup (errorstr, LEN_RECD);                               <*/
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (g_error !=  0) {
      *a_type = YCALC_DATA_ERROR;
      ycalc_handle_error (g_error, a_type, a_value, a_string, "");
      /*> a_curr->t = YCALC_DATA_ERROR;                                                            <* 
       *> a_curr->v_str = strndup (errorstr, LEN_RECD);                               <*/
      DEBUG_CALC   yLOG_value   ("g_error"  , g_error);
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(stack)--------------------------*/
   --rce;  if (s_nstack < 1) {
      DEBUG_CALC   yLOG_note    ("nothing left in stack to retrieve at end");
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   --rce;  if (s_nstack > 1) {
      DEBUG_CALC   yLOG_note    ("extra data in stack at end");
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(results)------------------------*/
   switch (*a_type) {
   case YCALC_DATA_NFORM : case YCALC_DATA_NLIKE :
      *a_value = ycalc_popval (__FUNCTION__);
      --rce;  if (g_error !=  0) {
         *a_type = YCALC_DATA_ERROR;
         ycalc_handle_error (g_error, a_type, a_value, a_string, "");
         DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_CALC   yLOG_value   ("value"     , *a_value);
      break;
   case YCALC_DATA_SFORM : case YCALC_DATA_SLIKE :
      *a_string = ycalc_popstr (__FUNCTION__);
      --rce;  if (g_error !=  0) {
         *a_type = YCALC_DATA_ERROR;
         ycalc_handle_error (g_error, a_type, a_value, a_string, "");
         DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_CALC   yLOG_info    ("string"    , *a_string);
      break;
   }
   /*---(complete)-------------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
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
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   rc = ycalc__exec_prepare (a_deproot, a_type, a_value, a_string);
   DEBUG_CALC   yLOG_value   ("prepare"   , rc);
   --rce;  if (rc < 0)  {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(main loop)----------------------*/
   myCALC.deproot = a_deproot;
   myCALC.owner   = a_deproot->owner;
   x_calc   = a_deproot->chead;
   DEBUG_CALC   yLOG_value   ("ncalc"     , a_deproot->ncalc);
   DEBUG_CALC   yLOG_value   ("nstack"    , s_nstack);
   while (x_calc != NULL) {
      ++s_neval;
      DEBUG_CALC   yLOG_complex ("element"   , "typ=%c, val=%F, str=%-9p, ref=%-9p, fnc=%-9p", x_calc->t, x_calc->v, x_calc->s, x_calc->r, x_calc->f);
      s_narg = 0;
      switch (x_calc->t) {
      case G_TYPE_VAL  :
         ycalc_pushval (__FUNCTION__, x_calc->v);
         break;
      case G_TYPE_STR  :
         ycalc_pushstr (__FUNCTION__, x_calc->s);
         break;
      case G_TYPE_REF  :
         ycalc_pushref (__FUNCTION__, x_calc->r);
         break;
      case G_TYPE_FUNC :
         x_calc->f();
         break;
      case G_TYPE_NOOP :
         break;
      default  :
         /*> ERROR_add (s_me, PERR_EVAL, s_neval, __FUNCTION__, TERR_OTHER, "bad calculation type requested");   <*/
         g_error = -66;
         break;
      }
      if (g_error != 0) break;
      x_calc = x_calc->next;
      DEBUG_CALC   yLOG_value   ("nstack"    , s_nstack);
   }
   myCALC.deproot = NULL;
   myCALC.owner   = NULL;
   /*---(check results)------------------*/
   rc = ycalc__exec_wrap    (a_type, a_value, a_string);
   DEBUG_CALC   yLOG_value   ("wrap"      , rc);
   --rce;  if (rc < 0)  {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc_execute_detail    (tDEP_ROOT *a_deproot, char **a_source, char *a_type, double *a_value, char **a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;           /* return code for errors         */
   char        rc          =    0;           /* return code for errors         */
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (ycalc_not_ready ()) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(defense)------------------------*/
   DEBUG_CALC   yLOG_point   ("a_deproot" , a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_CALC   yLOG_note    ("a_deproot not set, nothing to do");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ycalc_shared_verify (a_source, a_type, a_value, a_string);
   DEBUG_CALC   yLOG_value   ("verify"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ycalc_execute_trusted (a_deproot, a_type, a_value, a_string);
   DEBUG_CALC   yLOG_value   ("trusted"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
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
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (ycalc_not_ready ()) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(defense)------------------------*/
   DEBUG_CALC   yLOG_point   ("a_owner"   , a_owner);
   --rce;  if (a_owner == NULL) {
      DEBUG_CALC   yLOG_note    ("a_owner not set, nothing to do");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_point   ("a_deproot" , a_deproot);
   /*---(fill pointers)------------------*/
   rc = g_pointer (a_owner, &x_source, &x_type, &x_value, &x_string);
   DEBUG_CALC   yLOG_value   ("pointer"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ycalc_execute_detail      (a_deproot, x_source, x_type, x_value, x_string);
   DEBUG_CALC   yLOG_value   ("detail"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
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
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (ycalc_not_ready ()) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get owner/deproot)--------------*/
   rc = g_who_named  (a_label, YCALC_OWNR, &x_owner, &x_deproot);
   DEBUG_CALC   yLOG_value   ("who_named"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(call classify)------------------*/
   rc = ycalc_execute_owner (x_owner, x_deproot);
   DEBUG_CALC   yLOG_value   ("owner"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

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
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   DEBUG_CALC   yLOG_point   ("a_owner"    , a_owner);
   DEBUG_CALC   yLOG_point   ("a_deproot"  , a_deproot);
   DEBUG_CALC   yLOG_info    ("label"      , ycalc_call_labeler (a_deproot));
   /*---(fill pointers)------------------*/
   rc = g_pointer (a_owner, &x_source, &x_type, &x_value, &x_string);
   DEBUG_CALC   yLOG_value   ("pointer"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(execute)------------------------*/
   rc = ycalc_execute_detail      (a_deproot, x_source, x_type, x_value, x_string);
   DEBUG_CALC   yLOG_value   ("detail"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(printable)----------------------*/
   rc = g_printer (a_owner);
   DEBUG_CALC   yLOG_value   ("printer"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char          ycalc__unit_answer [LEN_STR ];

char*        /*-> unit testing accessor --------------[ light  [us.IA0.2A5.X3]*/ /*-[02.0000.00#.#]-*/ /*-[--.---.---.--]-*/
ycalc__unit_stack       (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   char        x_type      =  '-';
   tDEP_ROOT  *x_deproot   = NULL;
   char        x_label     [LEN_LABEL];
   /*---(initialize)---------------------*/
   strlcpy (ycalc__unit_answer, "yCALC_unit, unknown request", 100);
   /*---(string testing)-----------------*/
   if      (strncmp (a_question, "top"       , 20)  == 0) {
      if (s_nstack <= 0)   snprintf (ycalc__unit_answer, LEN_STR, "STACK top   (%2d) : %c %8.2lf %-10p %-.30s", s_nstack, S_TYPE_EMPTY, 0.0, NULL, "---");
      else {
         switch (s_stack [s_nstack - 1].typ) {
         case S_TYPE_NUM :
         case S_TYPE_STR :
            snprintf (ycalc__unit_answer, LEN_STR, "STACK top   (%2d) : %c %8.2lf %-10p %-.30s", s_nstack, s_stack [s_nstack - 1].typ, s_stack [s_nstack - 1].num, s_stack [s_nstack - 1].ref, (s_stack [s_nstack - 1].str == NULL) ? "---" : s_stack [s_nstack - 1].str);
            break;
         case S_TYPE_REF :
            x_deproot = s_stack [s_nstack - 1].ref;
            strlcpy (x_label, ycalc__mock_labeler (x_deproot->owner), LEN_LABEL);
            snprintf (ycalc__unit_answer, LEN_STR, "STACK top   (%2d) : %c %8.2lf %-10.10s %-.30s", s_nstack, s_stack [s_nstack - 1].typ, s_stack [s_nstack - 1].num, x_label, (s_stack [s_nstack - 1].str == NULL) ? "---" : s_stack [s_nstack - 1].str);
            break;
         }
      }
   }
   /*---(complete)-----------------------*/
   return ycalc__unit_answer;
}

char ycalc__unit_stackset    (int a_num) { s_nstack = a_num; return 0; }




/*============================----end-of-source---============================*/
