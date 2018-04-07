/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"

 
static void*   (*s_thinger)     (char *a_label);
static char    (*s_value  )     (void *a_thing, char a_type, double *a_value, char *a_string, char *a_print);
static char    (*s_details)     (void *a_thing, char *a_quality, char *a_string, double *a_value);
static char    (*s_address)     (void *a_thing, int *x, int *y, int *z);



/*====================------------------------------------====================*/
/*===----                         execution stack                      ----===*/
/*====================------------------------------------====================*/
static void  o___STACK___________o () { return; }
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

#define     MAX_STACK         1000
typedef  struct cSTACK tSTACK;
struct cSTACK {
   char      typ;            /* type : r=ref, v=val, s=str                    */
   tCELL    *ref;            /* cell reference                                */
   double    num;            /* constant value                                */
   char     *str;            /* literal string                                */
};
tSTACK      calc__stack   [MAX_STACK];
int         calc__nstack  = 0;

/*
 *   type is one of...
 *      -- v         numeric value
 *      -- s         string
 *      -- r         cell reference
 *
 */

char         /*-> add a vstring to the stack ---------[ ------ [gc.420.202.11]*/ /*-[01.0000.0#5.!]-*/ /*-[--.---.---.--]-*/
yCALC_pushstr       (char *a_func, char *a_new)
{  /*---(design notes)-------------------*//*---------------------------------*/
   /* very simply adds a stack entry with a string literal as its value       */
   /*---(defense: stack overflow)--------*//*---------------------------------*/
   if (calc__nstack >= MAX_STACK) {
      ERROR_add (s_me, PERR_EVAL, s_neval, a_func, TERR_OTHER, "stack full, could not push string");
      return -1;
   }
   /*---(update stack item)--------------*/
   calc__stack[calc__nstack].typ = 's';
   calc__stack[calc__nstack].ref = NULL;
   calc__stack[calc__nstack].num = 0;
   calc__stack[calc__nstack].str = strndup (a_new, LEN_RECD);
   /*---(update stack counter)-----------*/
   ++calc__nstack;
   /*---(complete)-----------------------*/
   return 0;
}

char         /*-> add a value to the stack -----------[ ------ [gc.420.202.11]*/ /*-[01.0000.0#5.!]-*/ /*-[--.---.---.--]-*/
yCALC_pushval       (char *a_func, double a_new)
{  /*---(design notes)-------------------*//*---------------------------------*/
   /* very simply adds a stack entry with a numeric literal as its value      */
   /*---(defense: stack overflow)--------*//*---------------------------------*/
   if (calc__nstack >= MAX_STACK) {
      ERROR_add (s_me, PERR_EVAL, s_neval, a_func, TERR_OTHER, "stack full, could not push value");
      return -1;
   }
   /*---(update stack item)--------------*/
   calc__stack[calc__nstack].typ = 'v';
   calc__stack[calc__nstack].ref = NULL;
   calc__stack[calc__nstack].num = a_new;
   calc__stack[calc__nstack].str = NULL;
   /*---(update stack counter)-----------*/
   ++calc__nstack;
   /*---(complete)-----------------------*/
   return 0;
}

char         /*-> add a reference to the stack -------[ ------ [gc.520.203.21]*/ /*-[01.0000.075.!]-*/ /*-[--.---.---.--]-*/
yCALC_pushref       (char *a_func, tCELL *a_new)
{  /*---(design notes)-------------------*//*---------------------------------*/
   /* adds a cell reference for later intepretation in the calculation        */
   /*---(defense: stack overflow)--------*//*---------------------------------*/
   if (calc__nstack >= MAX_STACK) {
      ERROR_add (s_me, PERR_EVAL, s_neval, a_func, TERR_OTHER, "stack full, could not push reference");
      return -1;
   }
   if (a_new        == NULL     )  return -2;
   /*---(update stack item)--------------*/
   calc__stack[calc__nstack].typ = 'r';
   calc__stack[calc__nstack].ref = a_new;
   calc__stack[calc__nstack].num = 0;
   calc__stack[calc__nstack].str = NULL;
   /*---(update stack counter)-----------*/
   ++calc__nstack;
   /*---(complete)-----------------------*/
   return 0;
}

double       /*-> get an numeric off the stack -------[ ------ [fn.730.205.21]*/ /*-[01.0000.0#5.!]-*/ /*-[--.---.---.--]-*/
yCALC_popval          (char *a_func, char a_seq)
{  /*---(design notes)-------------------*//*---------------------------------*/
   /* always returns a value for the stack entry.                             */
   /* -- for a numeric literal, it returns the number field on the stack item */
   /* -- for a string literal, it returns 0.0 (since strings have no value)   */
   /* -- for a reference, it returns the value contained in that cell         */
   /* -- if it can't figure it out, it returns a 0.0                          */
   /*---(prepare)------------------------*/
   if (calc__nstack <= 0) {
      ERROR_add (s_me, PERR_EVAL, s_neval, a_func, TERR_ARGS , "stack empty, could not get value");
      return 0.0;
   }
   --calc__nstack;
   /*---(handle stack types)-------------*/
   switch (calc__stack[calc__nstack].typ) {
   case 'v' :
      return  calc__stack[calc__nstack].num;
      break;
   case 's' :
      return  0.0;
      break;
   case 'r' :
      return  calc__stack[calc__nstack].ref->v_num;
      break;
   }
   /*---(complete)-----------------------*/
   ERROR_add (s_me, PERR_EVAL, s_neval, a_func, TERR_ARGS , "wrong argument type on stack");
   return 0.0;
}

char*        /*-> get an string off the stack --------[ ------ [fs.A40.20#.31]*/ /*-[02.0000.0#5.!]-*/ /*-[--.---.---.--]-*/
yCALC_popstr       (char *a_func, char a_seq)
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
   if (calc__nstack <= 0) {
      ERROR_add (s_me, PERR_EVAL, s_neval, a_func, TERR_ARGS , "stack empty, could not get string");
      return strndup (nada, LEN_RECD);
   }
   --calc__nstack;
   /*---(handle stack types)-------------*/
   switch (calc__stack[calc__nstack].typ) {
   case 'v' :
      return  strndup (nada, LEN_RECD);
      break;
   case 's' :
      return  calc__stack[calc__nstack].str;
      break;
   case 'r' :
      switch (calc__stack[calc__nstack].ref->t) {
      case  CTYPE_STR    :
         return  strndup (calc__stack[calc__nstack].ref->s    , LEN_RECD);
         break;
      case  CTYPE_MOD    :
         return  strndup (calc__stack[calc__nstack].ref->v_str, LEN_RECD);
         break;
      default            :
         return  strndup (nada, LEN_RECD);
         break;
      }
   default            :
      return  strndup (nada, LEN_RECD);
      break;
   }
   /*---(complete)-----------------------*/
   ERROR_add (s_me, PERR_EVAL, s_neval, a_func, TERR_ARGS , "wrong argument type on stack");
   return strndup (nada, LEN_RECD);
}

tCELL*       /*-> get a reference off the stack ------[ ------ [fp.420.203.21]*/ /*-[01.0000.0F#.!]-*/ /*-[--.---.---.--]-*/
yCALC_popref          (char *a_func, char a_seq)
{  /*---(design notes)-------------------*//*---------------------------------*/
   /*---(prepare)------------------------*/
   if (calc__nstack <= 0) {
      ERROR_add (s_me, PERR_EVAL, s_neval, a_func, TERR_ARGS , "stack empty, could not get reference");
      return NULL;
   }
   --calc__nstack;
   /*---(handle stack types)-------------*/
   switch (calc__stack[calc__nstack].typ) {
   case 'r' :
      return  calc__stack[calc__nstack].ref;
      break;
   }
   /*---(complete)-----------------------*/
   ERROR_add (s_me, PERR_EVAL, s_neval, a_func, TERR_ARGS , "wrong argument type on stack");
   return NULL;
}

tCELL*       /*-> get a reference off the stack ------[ ------ [fp.420.203.21]*/ /*-[01.0000.04#.!]-*/ /*-[--.---.---.--]-*/
yCALC_popprint        (char *a_func, char a_seq)
{  /*---(design notes)-------------------*//*---------------------------------*/
   /*---(prepare)------------------------*/
   if (calc__nstack <= 0) {
      ERROR_add (s_me, PERR_EVAL, s_neval, a_func, TERR_ARGS , "stack empty, could not get printable");
      return NULL;
   }
   --calc__nstack;
   /*---(handle stack types)-------------*/
   switch (calc__stack[calc__nstack].typ) {
   case 'r' :
      return  strndup (calc__stack[calc__nstack].ref->p, LEN_RECD);
      break;
   }
   /*---(complete)-----------------------*/
   ERROR_add (s_me, PERR_EVAL, s_neval, a_func, TERR_ARGS , "wrong argument type on stack");
   return NULL;
}

tCELL*       /*-> get a reference off the stack ------[ ------ [fp.520.204.41]*/ /*-[02.0000.01#.!]-*/ /*-[--.---.---.--]-*/
yCALC_popform         (char *a_func, char a_seq)
{  /*---(design notes)-------------------*//*---------------------------------*/
   /*---(prepare)------------------------*/
   if (calc__nstack <= 0) {
      ERROR_add (s_me, PERR_EVAL, s_neval, a_func, TERR_ARGS , "stack empty, could not get printable");
      return NULL;
   }
   --calc__nstack;
   /*---(handle stack types)-------------*/
   switch (calc__stack[calc__nstack].typ) {
   case 'r' :
      if (strchr (G_CELL_CALC , calc__stack[calc__nstack].ref->t) != 0) {
         return  strndup (calc__stack[calc__nstack].ref->s    , LEN_RECD);
      } else {
         return  strndup (nada , LEN_RECD);
      }
      break;
   }
   /*---(complete)-----------------------*/
   ERROR_add (s_me, PERR_EVAL, s_neval, a_func, TERR_ARGS , "wrong argument type on stack");
   return NULL;
}

tCELL*       /*-> get a reference off the stack ------[ ------ [fp.520.204.41]*/ /*-[02.0000.01#.!]-*/ /*-[--.---.---.--]-*/
yCALC_poprpn          (char *a_func, char a_seq)
{  /*---(design notes)-------------------*//*---------------------------------*/
   /*---(prepare)------------------------*/
   if (calc__nstack <= 0) {
      ERROR_add (s_me, PERR_EVAL, s_neval, a_func, TERR_ARGS , "stack empty, could not get printable");
      return NULL;
   }
   --calc__nstack;
   /*---(handle stack types)-------------*/
   switch (calc__stack[calc__nstack].typ) {
   case 'r' :
      if (strchr (G_CELL_RPN  , calc__stack[calc__nstack].ref->t) != 0) {
         return  strndup (calc__stack[calc__nstack].ref->rpn  , LEN_RECD);
      } else {
         return  strndup (nada , LEN_RECD);
      }
      break;
   }
   /*---(complete)-----------------------*/
   ERROR_add (s_me, PERR_EVAL, s_neval, a_func, TERR_ARGS , "wrong argument type on stack");
   return NULL;
}
