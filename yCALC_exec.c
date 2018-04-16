/*============================----beg-of-source---============================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"




static char    s_type    = '-';
static double  s_value   = 0.0;
static char   *s_string  = NULL;
static int     x, y, z;



static void       *s_owner       = NULL;
static char        s_narg        =    0;
static int         s_nerror      =    0;
static int         s_nbuild      =    0;
static int         s_neval       =    0;

static int         s_error       =    0;


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
ycalc_exec_init          (void)
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
   g_valuer    = NULL;
   g_addresser = NULL;
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yCALC_exec_config       (void *a_valuer, void *a_addresser)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_PROG   yLOG_char    ("status"    , myCALC.status);
   --rce;  if (myCALC.status == NULL || strchr ("IO", myCALC.status) == NULL) {
      DEBUG_PROG   yLOG_note    ("must initialize before configuring");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update valuer)------------------*/
   DEBUG_PROG   yLOG_point   ("valuer"    , a_valuer);
   --rce;  if (a_valuer    == NULL) {
      DEBUG_PROG   yLOG_error   ("valuer"    , "without this callback, references, ranges, and variables can not function");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   g_valuer    = a_valuer;
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



/*====================------------------------------------====================*/
/*===----                        adding to stack                       ----===*/
/*====================------------------------------------====================*/
static void      o___PUSHING_________________o (void) {;}

char         /*-> add a value to the stack -----------[ ------ [gc.420.202.11]*/ /*-[01.0000.0#5.!]-*/ /*-[--.---.---.--]-*/
ycalc_pushval           (char *a_func, double a_value)
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
ycalc_pushstr           (char *a_func, char *a_string)
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
ycalc_pushref           (char *a_func, void *a_thing)
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
      x_deproot = s_stack [s_nstack].ref;
      g_valuer (x_deproot->owner, NULL, &s_value, NULL);
      return  s_value;
      break;
   }
   /*---(complete)-----------------------*/
   myCALC.trouble = G_ERROR_STACK;
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
      x_deproot = s_stack [s_nstack].ref;
      g_valuer (x_deproot->owner, NULL, NULL, &s_string);
      if (s_string == NULL) return  strndup (g_nada  , LEN_RECD);
      else                  return  strndup (s_string, LEN_RECD);
   }
   /*---(complete)-----------------------*/
   myCALC.trouble = G_ERROR_STACK;
   return strndup (g_nada, LEN_RECD);
}



/*====================------------------------------------====================*/
/*===----                        driver support                        ----===*/
/*====================------------------------------------====================*/
static void      o___SUPPORT_________________o (void) {;}

char
ycalc__exec_prepare     (tDEP_ROOT *a_deproot, char a_type, double *a_value, char **a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_label     [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(default settings)---------------*/
   if (a_value  != NULL)  *a_value = 0.0;
   if (a_string != NULL && *a_string != NULL)  strlcpy (*a_string, "", LEN_RECD);
   /*---(check status)-------------------*/
   DEBUG_CALC   yLOG_char    ("status"    , myCALC.status);
   --rce;  if (myCALC.status != 'O') {
      DEBUG_PROG   yLOG_note    ("must initialize and configure before use");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check deproot)------------------*/
   DEBUG_CALC   yLOG_point   ("a_deproot" , a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_CALC   yLOG_note    ("can not calculate a null deproot");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_info    ("label"     , g_labeler (a_deproot->owner));
   /*---(check calculation)--------------*/
   DEBUG_CALC   yLOG_point   ("chead"     , a_deproot->chead);
   --rce;  if (a_deproot->chead == NULL) {
      DEBUG_CALC   yLOG_note    ("calculation is null, never built");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check return type)--------------*/
   DEBUG_CALC   yLOG_char    ("a_type"    , a_type);
   --rce;  if (strchr ("=#", a_type) == NULL) {
      DEBUG_CALC   yLOG_note    ("calculation result not understood");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check return variables)---------*/
   --rce;  if (a_type == '=' && a_value == NULL) {
      DEBUG_CALC   yLOG_note    ("=, but numeric return value is null");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_type == '#' && a_string == NULL) {
      DEBUG_CALC   yLOG_note    ("#, but string return value is null");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(reset globals)------------------*/
   s_owner  = a_deproot->owner;
   s_neval  = 0;
   s_error  = 0;
   /*> strcpy (errorstr, "");                                                         <*/
   /*---(complete)-------------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc__exec_wrap        (char a_type, double *a_value, char **a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_label     [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(check errors)-------------------*/
   DEBUG_CALC   yLOG_value   ("s_error"   , s_error);
   --rce;  if (s_error <  0) {
      /*> a_curr->t = 'E';                                                            <* 
       *> a_curr->v_str = strndup (errorstr, LEN_RECD);                               <*/
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (s_error >  0) {
      /*> a_curr->t = 'E';                                                            <* 
       *> a_curr->v_str = strndup (errorstr, LEN_RECD);                               <*/
      DEBUG_CALC   yLOG_value   ("s_error"  , s_error);
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
   if (a_type == '=') {
      *a_value = ycalc_popval (__FUNCTION__);
      DEBUG_CALC   yLOG_value   ("value"     , *a_value);
   }
   if (a_type == '#') {
      *a_string = ycalc_popstr (__FUNCTION__);
      DEBUG_CALC   yLOG_info    ("string"    , *a_string);
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
yCALC_exec             (void *a_deproot, char a_type, double *a_value, char **a_string)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tDEP_ROOT  *x_deproot   = NULL;
   tCALC      *x_calc      = NULL;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   x_deproot = (tDEP_ROOT *) a_deproot;
   rc = ycalc__exec_prepare (x_deproot, a_type, a_value, a_string);
   DEBUG_CALC   yLOG_value   ("prepare"   , rc);
   --rce;  if (rc < 0)  {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(main loop)----------------------*/
   x_calc   = x_deproot->chead;
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
         s_error = -66;
         break;
      }
      if (s_error != 0) break;
      x_calc = x_calc->next;
   }
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

char         /*-> delete a two-way dependency --------[ ------ [ge.M88.15#.B6]*/ /*-[01.0000.526.H]-*/ /*-[--.---.---.--]-*/
yCALC_exec_label        (char *a_label, char a_type, double *a_value, char **a_string)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   char        rc          = 0;
   tDEP_ROOT  *x_deproot   = NULL;
   /*---(prepare)------------------------*/
   rc = ycalc_call_who_named (a_label, NULL, &x_deproot);
   DEBUG_CALC   yLOG_value   ("who named" , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_value   ("x_deproot" , x_deproot);
   --rce;  if (x_deproot == NULL) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(run)----------------------------*/
   rc = yCALC_exec (x_deproot, a_type, a_value, a_string);
   DEBUG_CALC   yLOG_value   ("exec"      , rc);
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
