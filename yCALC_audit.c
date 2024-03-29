/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"


/*
 *
 * metis  ww2��  review and rationalize pointer error types/messages
 *
 *
 *
 *
 *
 *
 */


const tyCALC_TYPES  g_ycalc_types [YCALC_MAX_TYPE] = {
   /*---type------------ -terse-------- -pre -rpn calc -dep -res ---description--------------------------------------- */
   {  YCALC_DATA_BLANK  , "blank"      , ' ', '-', '-', '-', '-', "blank cell"                                         },
   {  YCALC_DATA_STR    , "string"     , ' ', '-', '-', '-', '#', "string literal presented from source field"         },
   {  YCALC_DATA_SFORM  , "str-form"   , '#', 'y', 'y', 'y', '#', "string formula"                                     },
   {  YCALC_DATA_SLIKE  , "str-like"   , '~', 'y', 'y', 'y', '#', "string formula derived from another cell"           },
   {  YCALC_DATA_NUM    , "number"     , ' ', '-', '-', '-', '=', "numeric literal presented in various formats"       },
   {  YCALC_DATA_NFORM  , "num-form"   , '=', 'y', 'y', 'y', '=', "numeric formula"                                    },
   {  YCALC_DATA_NLIKE  , "num-like"   , '~', 'y', 'y', 'y', '=', "numeric formula derived from another cell"          },
   {  YCALC_DATA_MATH   , "num-math"   , '�', 'y', 'y', 'y', '=', "purer mathmatical notation numeric formula"         },
   {  YCALC_DATA_ADDR   , "address"    , '&', 'y', '-', 'y', '-', "address pointer to use in other formulas"           },
   {  YCALC_DATA_CADDR  , "calc-addr"  , '!', 'y', 'y', 'y', 'a', "calculated address pointer to use in formulas"      },
   {  YCALC_DATA_RLIKE  , "cref-like"  , '~', 'y', 'y', 'y', 'a', "calculated address derived from another cell"       },
   {  YCALC_DATA_RANGE  , "range"      , '&', 'y', '-', 'y', '-', "range pointer to use in other formulas"             },
   {  YCALC_DATA_VAR    , "variable"   , '�', '-', '-', '-', '-', "variable title"                                     },
   {  YCALC_DATA_VAR    , "variable"   , '�', '-', '-', '-', '-', "variable title"                                     },
   {  YCALC_DATA_VAR    , "variable"   , '�', '-', '-', '-', '-', "variable title"                                     },
   {  YCALC_DATA_VAR    , "variable"   , '�', '-', '-', '-', '-', "variable title"                                     },
   /*> {  YCALC_DATA_VAR    , "row_var"    , '�', '-', '-', '-', '-', "variable title"                                     },   <*/
   /*> {  YCALC_DATA_VAR    , "col_var"    , '�', '-', '-', '-', '-', "variable title"                                     },   <*/
   {  YCALC_DATA_INTERN , "internal"   , '�', '-', '-', '-', '-', "an actual internal range to use in other formulas"  },
   {  YCALC_DATA_MERGED , "merged"     , '<', '-', '-', 'y', '-', "empty cell used to present merged information"      },
   {  YCALC_DATA_ERROR  , "error"      , ' ', 'y', 'y', 'y', 'e', "error status"                                       },
   {  YCALC_DATA_GARBAGE, "garbage"    , ' ', '-', '-', '-', '-', "marked for garbage collection"                      },
   {  YCALC_DATA_PMERGE , "potential"  , ' ', '-', '-', '-', '#', "potentially used as a merge"                        },
   /*---type------------ -terse-------- -pre -rpn calc -dep -res ---description--------------------------------------- */
   {  0                 , ""           ,  0 ,  0 ,  0 ,  0 ,  0 , ""                                                   },
};
char    YCALC_GROUP_ALL    [LEN_LABEL] = "";
char    YCALC_GROUP_RPN    [LEN_LABEL] = "";
char    YCALC_GROUP_CALC   [LEN_LABEL] = "";
char    YCALC_GROUP_DEPS   [LEN_LABEL] = "";
char    YCALC_GROUP_NUM    [LEN_LABEL] = "";
char    YCALC_GROUP_STR    [LEN_LABEL] = "";
char    YCALC_GROUP_POINT  [LEN_LABEL] = "";
char    YCALC_GROUP_ERR    [LEN_LABEL] = "";
char    YCALC_GROUP_FPRE   [LEN_LABEL] = "";


const tyCALC_ERROR   zCALC_errors     [YCALC_MAX_ERROR] = {
   /* ---abbr-------------- stage   --disp-     ---description-------------------------------------  */
   { YCALC_ERROR_CONF      , 'c' , "#c/cfg"   , "build/exec environment not fully configured"        },
   { YCALC_ERROR_BUILD_RPN , 'b' , "#b/rpn"   , "can not convert source to rpn tokens"               },
   { YCALC_ERROR_BUILD_REF , 'b' , "#b/ref"   , "label/ref null, not allowed, or not real"           },
   { YCALC_ERROR_BUILD_DEP , 'b' , "#b/dep"   , "dependence failed on legal label/ref"               },
   { YCALC_ERROR_BUILD_LIK , 'b' , "#b/lik"   , "build of a like formula failed"                     },
   { YCALC_ERROR_BUILD_PNT , 'b' , "#b/ind"   , "pointer dest label/ref is not addr/range type"      },
   { YCALC_ERROR_BUILD_RNG , 'b' , "#b/rng"   , "can not create range with given coords"             },
   { YCALC_ERROR_BUILD_TOK , 'b' , "#b/tok"   , "rpn token could not be recognized"                  },
   { YCALC_ERROR_BUILD_DUP , 'b' , "#b/dup"   , "duplicate variable so can not be created"           },
   { YCALC_ERROR_BUILD_VAR , 'b' , "#b/var"   , "illegal variable name"                              },
   { YCALC_ERROR_BUILD_FNC , 'b' , "#b/fnc"   , "variable would mask a function by name name"        },
   { YCALC_ERROR_BUILD_CIR , 'b' , "#b/cir"   , "formula contains a circular reference"              },
   { YCALC_ERROR_BUILD_ROO , 'b' , "#b/roo"   , "can not root a cell into the dependency tree"       },
   { YCALC_ERROR_BUILD_TBD , 'b' , "#b/tbd"   , "requested func/oper legal, but not implemented"     },
   { YCALC_ERROR_STACK     , 'e' , "#e/stk"   , "execution stack under or over run"                  },
   { YCALC_ERROR_EXEC_VAL  , 'e' , "#e/val"   , "expected a string, but given a value"               },
   { YCALC_ERROR_EXEC_STR  , 'e' , "#e/str"   , "expected a value, but given a string"               },
   { YCALC_ERROR_EXEC_NUM  , 'e' , "#e/num"   , "not a valid number representation"                  },
   { YCALC_ERROR_EXEC_REF  , 'e' , "#e/ref"   , "calculated dependence is to illegal location"       },
   { YCALC_ERROR_EXEC_VAR  , 'e' , "#e/var"   , "requested variable not defined"                     },
   { YCALC_ERROR_EXEC_CIR  , 'e' , "#e/cir"   , "calculated dependence creates a cirlular loop"      },
   { YCALC_ERROR_EXEC_ARG  , 'e' , "#e/arg"   , "function arguments are invalid/unknown"             },
   { YCALC_ERROR_EXEC_FMT  , 'e' , "#e/fmt"   , "input data format could not be interpreted"         },
   { YCALC_ERROR_EXEC_DATE , 'e' , "#e/dat"   , "date/time value not in a legal range"               },
   { YCALC_ERROR_EXEC_BRNG , 'e' , "#e/beg"   , "beginning of range not legal"                       },
   { YCALC_ERROR_EXEC_ERNG , 'e' , "#e/end"   , "end of range not legal"                             },
   { YCALC_ERROR_EXEC_MISS , 'e' , "#e/mis"   , "lookup or index function can not find result"       },
   { YCALC_ERROR_EXEC_PTR  , 'e' , "#e/nod"   , "expected val/str, but given non-deref pointer"      },
   { YCALC_ERROR_EXEC_IND  , 'e' , "#e/nop"   , "attempt to dereference a non-pointer value"         },
   { YCALC_ERROR_EXEC_MAL  , 'e' , "#e/mal"   , "attempt to dereference malformed pointer ref"       },
   { YCALC_ERROR_EXEC_NULL , 'e' , "#e/nul"   , "attempt to dereference a null pointer"              },
   { YCALC_ERROR_EXEC_PERR , 'e' , "#e/per"   , "attempt to dereference a pointer to an error"       },
   { YCALC_ERROR_EXEC_NADA , 'e' , "#e/bnk"   , "reference points to blank cell, no value"           },
   { YCALC_ERROR_EXEC_ERR  , 'e' , "#e/err"   , "reference points to error cell, no value"           },
   { YCALC_ERROR_EXEC_HUH  , 'e' , "#e/huh"   , "found something wierd in the exex stack"            },
   { YCALC_ERROR_EXEC_OPT  , 'e' , "#e/opt"   , "function option not found in specific list"         },
   { YCALC_ERROR_UNKNOWN   , 'b' , "#boom"    , ""                                                   },
   /* ---abbr-------------- stage   --disp-     ---description-------------------------------------  */
   { 0                     ,  0  , ""         , ""                                                   },
};



/*====================------------------------------------====================*/
/*===----                          program level                       ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char         /*-> tbd --------------------------------[ shoot  [ge.E74.041.90]*/ /*-[04.0000.10#.!]-*/ /*-[--.---.---.--]-*/
ycalc_audit_init        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   char        t           [LEN_LABEL];
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(object types)-------------------*/
   DEBUG_YCALC   yLOG_note    ("clear validation types");
   ystrlcpy (YCALC_GROUP_ALL  , "", LEN_LABEL);
   ystrlcpy (YCALC_GROUP_RPN  , "", LEN_LABEL);
   ystrlcpy (YCALC_GROUP_CALC , "", LEN_LABEL);
   ystrlcpy (YCALC_GROUP_DEPS , "", LEN_LABEL);
   ystrlcpy (YCALC_GROUP_NUM  , "", LEN_LABEL);
   ystrlcpy (YCALC_GROUP_STR  , "", LEN_LABEL);
   ystrlcpy (YCALC_GROUP_ERR  , "", LEN_LABEL);
   ystrlcpy (YCALC_GROUP_POINT, "", LEN_LABEL);
   ystrlcpy (YCALC_GROUP_FPRE , "", LEN_LABEL);
   /*---(complete info table)------------*/
   DEBUG_YCALC   yLOG_note    ("build cell validation types");
   --rce;
   for (i = 0; i < YCALC_MAX_TYPE; ++i) {
      DEBUG_YCALC_M yLOG_value   ("ENTRY"     , i);
      DEBUG_YCALC_M yLOG_char    ("type"      , g_ycalc_types [i].type);
      /*---(check for end)---------------*/
      if (g_ycalc_types [i].type == 0)  break;
      /*---(add to lists)----------------*/
      sprintf (t, "%c", g_ycalc_types [i].type);
      DEBUG_YCALC_M yLOG_info    ("str type"  , t);
      DEBUG_YCALC_M yLOG_char    ("rpn flag"  , g_ycalc_types [i].rpn);
      strcat (YCALC_GROUP_ALL , t);
      if (g_ycalc_types [i].calc    == 'y')  strcat (YCALC_GROUP_CALC , t);
      if (g_ycalc_types [i].deps    == 'y')  strcat (YCALC_GROUP_DEPS , t);
      if (g_ycalc_types [i].result  == '=')  strcat (YCALC_GROUP_NUM  , t);
      if (g_ycalc_types [i].result  == '#')  strcat (YCALC_GROUP_STR  , t);
      if (g_ycalc_types [i].result  == 'e')  strcat (YCALC_GROUP_ERR  , t);
      if (g_ycalc_types [i].prefix  == '&')  strcat (YCALC_GROUP_POINT, t);
      if (g_ycalc_types [i].rpn     == 'y') {
         strcat  (YCALC_GROUP_RPN , t);
         sprintf (t, "%c", g_ycalc_types [i].prefix);
         if   (g_ycalc_types [i].prefix != ' ' && 
               strchr (YCALC_GROUP_FPRE, g_ycalc_types [i].prefix) == 0) {
            strcat  (YCALC_GROUP_FPRE , t);
         }
      }
      ++c;
   }
   /*---(report out)---------------------*/
   DEBUG_YCALC   yLOG_value   ("c"         , c);
   DEBUG_YCALC   yLOG_info    ("GROUP_ALL" , YCALC_GROUP_ALL  );
   DEBUG_YCALC   yLOG_info    ("GROUP_RPN" , YCALC_GROUP_RPN  );
   DEBUG_YCALC   yLOG_info    ("GROUP_CALC", YCALC_GROUP_CALC );
   DEBUG_YCALC   yLOG_info    ("GROUP_DEPS", YCALC_GROUP_DEPS );
   DEBUG_YCALC   yLOG_info    ("GROUP_NUM" , YCALC_GROUP_NUM  );
   DEBUG_YCALC   yLOG_info    ("GROUP_STR" , YCALC_GROUP_STR  );
   DEBUG_YCALC   yLOG_info    ("GROUP_POIN", YCALC_GROUP_POINT);
   DEBUG_YCALC   yLOG_info    ("GROUP_ERR" , YCALC_GROUP_ERR  );
   DEBUG_YCALC   yLOG_info    ("GROUP_FPRE", YCALC_GROUP_FPRE );
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                            driver                            ----===*/
/*====================------------------------------------====================*/
static void  o___ERRORS__________o () { return; }

char
ycalc_error_clear       (void)
{
   g_error        = YCALC_ERROR_NONE;
   myCALC.trouble = YCALC_ERROR_NONE;
   return 0;
}

char         /*-> tbd --------------------------------[ leaf   [gz.740.541.70]*/ /*-[11.0000.20#.!]-*/ /*-[--.---.---.--]-*/
ycalc_error_set         (int a_error, tDEP_ROOT *a_deproot)
{
   /*---(set error code)-----------------*/
   if (a_error > YCALC_ERROR_NONE) {
      g_error        = a_error;
      myCALC.trouble = a_error;
   }
   /*---(free me pointer)----------------*/
   if (a_deproot != NULL && myCALC.me != NULL) {
      free (myCALC.me);
      myCALC.me = NULL;
   }
   /*---(set error pointer)--------------*/
   if        (a_deproot == YCALC_ERROR_LITERAL) {
      myCALC.me = strdup ("lit");
   } else if (a_deproot == YCALC_ERROR_ARG1) {
      myCALC.me = strdup ("arg1");
   } else if (a_deproot == YCALC_ERROR_ARG2) {
      myCALC.me = strdup ("arg2");
   } else if (a_deproot == YCALC_ERROR_ARG3) {
      myCALC.me = strdup ("arg3");
   } else if (a_deproot == YCALC_ERROR_ARG4) {
      myCALC.me = strdup ("arg4");
   } else if (a_deproot == YCALC_ERROR_INTERN) {
      myCALC.me = strdup ("intern");
   } else if (a_deproot == YCALC_ERROR_UNKNOWN) {
      myCALC.me = strdup ("???");
   } else if (a_deproot != NULL) {
      myCALC.me = strdup (ycalc_call_labeler (a_deproot));
   }
   /*---(complete)-----------------------*/
   return 0;
}

char         /*-> tbd --------------------------------[ leaf   [gz.740.541.70]*/ /*-[11.0000.20#.!]-*/ /*-[--.---.---.--]-*/
ycalc_error_finalize    (char a_error, char *a_type, double *a_value, char **a_string, char *a_note)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   int         n           =   -1;
   char        t           [LEN_RECD];
   char        s           [LEN_RECD];
   /*---(assign error status)------------*/
   if (a_type  != NULL)  *a_type   = YCALC_DATA_ERROR;
   if (a_value != NULL)  *a_value  = 0.0;
   if (a_string != NULL) {
      if (*a_string != NULL) {
         free (*a_string);
         *a_string = NULL;
      }
   }
   /*---(find entry)---------------------*/
   for (i = 0; i < YCALC_MAX_ERROR; ++i) {
      if (zCALC_errors [i].abbr == 0      )   break;
      if (zCALC_errors [i].abbr != a_error)   continue;
      n = i;
      break;
   }
   /*---(main message)-------------------*/
   if (n < 0)    ystrlcpy (t, "#mystry", LEN_LABEL);
   else          ystrlcpy (t, zCALC_errors [i].terse, LEN_LABEL);
   /*---(note/token)---------------------*/
   if (strlen (a_note) > 0)  sprintf (s, "%-7s (%s)", t, a_note);
   else                      ystrlcpy (s, t, LEN_RECD);
   /*---(save)---------------------------*/
   if (a_string != NULL)  *a_string = strdup (s);
   DEBUG_YCALC   yLOG_info    ("ERROR"     , s);
   /*---(find entry)---------------------*/
   return 0;
}

char
ycalc_error_true        (void)
{
   if (g_error != YCALC_ERROR_NONE)  return 1;
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       merging in gyges                       ----===*/
/*====================------------------------------------====================*/
static void  o___MERGES__________o () { return; }

char         /*-> check and coordinate an unmerge ----[ leaf   [ge.622.132.30]*/ /*-[01.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_merge_count       (tDEP_ROOT *a_deproot)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tDEP_LINK  *x_next      = NULL;
   int         x_cnt       =    0;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YCALC   yLOG_spoint  (a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_YCALC   yLOG_sexit   (__FUNCTION__);
      return 0;
   }
   /*---(remove all)---------------------*/
   x_next = a_deproot->pros;
   while (x_next != NULL) {
      DEBUG_YCALC   yLOG_schar   (x_next->type);
      if (x_next->type == G_DEP_MERGED) ++x_cnt;
      x_next = x_next->next;
   }
   DEBUG_YCALC   yLOG_sint    (x_cnt);
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_sexit   (__FUNCTION__);
   return x_cnt;
}

char         /*-> find the merge source --------------[ leaf   [gp.420.113.30]*/ /*-[01.0000.306.#]-*/ /*-[--.---.---.--]-*/
ycalc__merge_leftmost   (tDEP_ROOT *a_deproot, tDEP_ROOT **a_origin)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tDEP_ROOT  *x_deproot   = NULL;
   tDEP_LINK  *x_next      = NULL;
   char        x_label     [LEN_LABEL] = "";
   /*---(begin)--------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (a_deproot == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(find head)----------------------*/
   *a_origin = NULL;
   x_next = a_deproot->pros;
   while (x_next != NULL) {
      ystrlcpy (x_label, ycalc_call_labeler (x_next->target), LEN_LABEL);
      DEBUG_YCALC   yLOG_complex ("look at"   , "%-10.10p, %c, %s", x_next, x_next->type, x_label);
      if (G_DEP_BLEED == x_next->type) {
         DEBUG_YCALC   yLOG_note    ("FOUND");
         *a_origin = x_next->target;
         break;
      }
      x_next = x_next->next;
   }
   /*---(check none)---------------------*/
   --rce;  if (*a_origin == NULL) {
      DEBUG_YCALC   yLOG_note    ("no leftward merge providers found");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> check and coordinate an unmerge ----[ ------ [ge.630.133.34]*/ /*-[01.0000.013.4]-*/ /*-[--.---.---.--]-*/
ycalc__unmerge_right    (tDEP_ROOT **a_deproot, int a_start)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tDEP_LINK  *x_next      = NULL;
   void       *x_owner     = NULL;
   tDEP_ROOT  *x_deproot   = NULL;
   int         x           =    0;
   char      **x_source    = NULL;
   char       *x_type      = NULL;
   tDEP_LINK  *x_save      = NULL;
   char        x_interior  =    0;
   char        x_label     [LEN_LABEL];
   tDEP_ROOT  *x_src       = NULL;
   tDEP_ROOT  *x_dst       = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   DEBUG_YCALC   yLOG_value   ("a_start"   , a_start);
   /*---(state check)--------------------*/
   DEBUG_YCALC   yLOG_char    ("state"     , myCALC.state);
   if (myCALC.state == 'L')  {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return 0;
   }
   /*---(defense)------------------------*/
   DEBUG_YCALC   yLOG_point   ("a_deproot" , a_deproot);
   --rce; if (a_deproot == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("*a_deproot", *a_deproot);
   --rce; if (*a_deproot == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   ystrlcpy (x_label, ycalc_call_labeler (*a_deproot), LEN_LABEL);
   DEBUG_YCALC   yLOG_info    ("label"     , x_label);
   /*---(remove all)---------------------*/
   x_next = (*a_deproot)->reqs;
   DEBUG_YCALC   yLOG_point   ("x_next"    , x_next);
   while (x_next != NULL) {
      DEBUG_YCALC   yLOG_complex ("x_next"    , "%-10.10p, %c", x_next, x_next->type);
      x_save = x_next;
      if (x_next->type == G_DEP_MERGED) {
         x_owner   = x_next->target->owner;
         x_deproot = x_next->target;
         rc = myCALC.e_addresser (x_owner, NULL, &x, NULL, NULL);
         DEBUG_YCALC   yLOG_value   ("x"         , x);
         if (x >= a_start) {
            rc = myCALC.e_pointer (x_owner, &x_source, &x_type, NULL, NULL);
            DEBUG_YCALC   yLOG_complex ("pointers"  , "%-10.10p, %-10.10p, %c", *x_source, x_type, *x_type);
            if (*x_source != NULL && strcmp (*x_source, "<") == NULL) {
               if (x == a_start)  x_interior = 1;
               *x_type = YCALC_DATA_PMERGE;
               DEBUG_YCALC   yLOG_char    ("new type"  , *x_type);
            }
            x_src = x_next->source;
            x_dst = x_next->target;
            DEBUG_YCALC   yLOG_point   ("deproot"   , x_next->target);
            DEBUG_YCALC   yLOG_point   ("owner"     , x_next->target->owner);
            /*> rc = ycalc_deps_delete (G_DEP_REQUIRE, &(x_next->source), &(x_next->target), &(x_next->target->owner));   <*/
            rc = ycalc_deps_delete (G_DEP_REQUIRE, &x_src, &x_dst, &x_owner);
            DEBUG_YCALC   yLOG_value   ("delete"    , rc);
            DEBUG_YCALC   yLOG_point   ("x_owner"   , x_owner);
            DEBUG_YCALC   yLOG_point   ("x_deproot" , x_deproot);
            if (x_owner != NULL)  rc = myCALC.e_printer (x_owner);
         }
      }
      x_next = x_save->next;
   }
   DEBUG_YCALC   yLOG_value   ("x_interior", x_interior);
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return x_interior;
}

char         /*-> find the merge source --------------[ leaf   [gp.420.113.30]*/ /*-[01.0000.306.#]-*/ /*-[--.---.---.--]-*/
ycalc__merge_right      (tDEP_ROOT *a_source, tDEP_ROOT *a_current)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         b, x, y, z, x_off;
   int         u, v, w;
   int         i           =    0;
   void       *x_owner     = NULL;
   tDEP_ROOT  *x_next      = NULL;
   char      **x_source    = NULL;
   char       *x_type      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YCALC   yLOG_point   ("a_source"  , a_source);
   --rce;  if (a_source == NULL)  {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("a_current" , a_current);
   --rce;  if (a_current == NULL)  {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(locate source)------------------*/
   rc = myCALC.e_addresser (a_source->owner, &b, &x, &y, &z);
   DEBUG_YCALC   yLOG_value   ("addresser" , rc);
   if (rc < 0)  {
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   DEBUG_YCALC   yLOG_complex ("source"    , "%-10p, %3db, %3dx, %3dy, %3dz", a_source, b, x, y, z);
   /*---(locate current)-----------------*/
   DEBUG_YCALC   yLOG_point   ("->owner"   , a_current->owner);
   rc = myCALC.e_addresser (a_current->owner, NULL, &x_off, NULL, NULL);
   /*> rc = myCALC.e_addresser (a_current->owner, &u, &x_off, &v, &w);                       <*/
   DEBUG_YCALC   yLOG_value   ("addresser" , rc);
   if (rc < 0)  {
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   DEBUG_YCALC   yLOG_complex ("current"   , "%-10p, %3db, %3dx, %3dy, %3dz", a_current, b, x_off, y, z);
   /*---(look for merges)----------------*/
   for (i = x_off + 1; i < x + 20; ++i) {
      DEBUG_YCALC   yLOG_value   ("i"         , i);
      /*---(check location)----*/
      rc = ycalc_call_who_at (b, i, y, z, YCALC_LOOK, &x_owner, &x_next);
      DEBUG_YCALC   yLOG_complex ("look"      , "%4drc, %-10.10p, %-10.10p", rc, x_owner, x_next);
      if (rc < 0)                              break;
      if (x_owner == NULL)                     break;
      /*---(get pointers)------*/
      rc = myCALC.e_pointer (x_owner, &x_source, &x_type, NULL, NULL);
      DEBUG_YCALC   yLOG_complex ("pointer"   , "%4drc, %-10.10p, %-10.10p", rc, x_source, x_type);
      if (rc < 0) {
         DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
         return 0;
      }
      DEBUG_YCALC   yLOG_complex ("owner"     , "%-10p, %3db, %3dx, %3dy, %3dz, %c", x_owner, b, i, y, z, *x_type);
      /*---(filter)------------*/
      if (*x_source == NULL)                break;
      if (strcmp (*x_source, "<") != 0)     break;
      /*---(force)-------------*/
      rc = ycalc_call_who_at (b, i, y, z, YCALC_FULL, &x_owner, &x_next);
      DEBUG_YCALC   yLOG_complex ("force"     , "%4drc, %-10.10p, %-10.10p", rc, x_owner, x_next);
      rc = ycalc_deps_create (G_DEP_MERGED, &a_source, &x_next);
      DEBUG_YCALC   yLOG_value   ("create"    , rc);
      if (rc < 0) {
         DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
         return 0;
      }
      /*---(set as merged)-----*/
      *x_type = YCALC_DATA_MERGED;
      /*---(free up right)-----*/
      rc = ycalc__unmerge_right (&x_next, 0);
      /*---(done)--------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

static void *s_origin = NULL;

char         /*-> check and coordinate an unmerge ----[ ------ [ge.630.133.34]*/ /*-[01.0000.013.4]-*/ /*-[--.---.---.--]-*/
ycalc_unmerge           (tDEP_ROOT **a_deproot, char *a_type)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x           =    0;
   tDEP_ROOT  *x_origin    = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(state check)--------------------*/
   DEBUG_YCALC   yLOG_char    ("state"     , myCALC.state);
   if (myCALC.state == 'L')  {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return 0;
   }
   /*---(defense)------------------------*/
   DEBUG_YCALC   yLOG_point   ("a_deproot" , a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("*a_deproot", *a_deproot);
   --rce;  if (*a_deproot == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, -rce);
      return -rce;
   }
   /*---(check if merge root)------------*/
   if (*a_type != YCALC_DATA_MERGED) {
      DEBUG_YCALC   yLOG_note    ("process merge origin delete");
      rc = ycalc__unmerge_right (a_deproot, 0);
   }
   /*---(check merged cells)-------------*/
   else {
      DEBUG_YCALC   yLOG_note    ("process merge interior delete");
      /*---(get column)-----------*/
      rc = myCALC.e_addresser ((*a_deproot)->owner, NULL, &x, NULL, NULL);
      if (rc < 0) {
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, -rce);
         return rce;
      }
      /*---(find source)----------*/
      rc = ycalc__merge_leftmost (*a_deproot, &x_origin);
      if (rc < 0) {
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, -rce);
         return rce;
      }
      /*---(unmerge right)--------*/
      rc = ycalc__unmerge_right (&x_origin, x);
      if (rc < 0) {
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, -rce);
         return rce;
      }
      /*---(update)---------------*/
      s_origin   = x_origin->owner;
      /*---(done)-----------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> coordinate a merge -----------------[ ------ [ge.640.134.44]*/ /*-[02.0000.013.6]-*/ /*-[--.---.---.--]-*/
ycalc_merge_check    (tDEP_ROOT *a_source)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         b, x, y, z, x_off;
   int         i           =    0;
   void       *x_owner     = NULL;
   tDEP_ROOT  *x_next      = NULL;
   char      **x_source    = NULL;
   char       *x_type      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YCALC   yLOG_point   ("a_source"  , a_source);
   --rce;  if (a_source == NULL)  {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check source)-------------------*/
   rc = myCALC.e_pointer (a_source->owner, NULL, &x_type, NULL, NULL);
   DEBUG_YCALC   yLOG_char    ("x_type"    , *x_type);
   if (*x_type == YCALC_DATA_BLANK || *x_type == YCALC_DATA_GARBAGE)  {
      DEBUG_YCALC   yLOG_note    ("never merge on blanks/garbage");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(locate source)------------------*/
   rc = myCALC.e_addresser (a_source->owner, &b, &x, &y, &z);
   DEBUG_YCALC   yLOG_value   ("addresser" , rc);
   if (rc < 0)  {
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   DEBUG_YCALC   yLOG_complex ("source"    , "%-10p, %c, %3db, %3dx, %3dy, %3dz", a_source, *x_type, b, x, y, z);
   /*---(look for merges)----------------*/
   for (i = x + 1; i < x + 20; ++i) {
      DEBUG_YCALC   yLOG_value   ("i"         , i);
      /*---(retrieve)-----------------------*/
      rc = ycalc_call_who_at (b, i, y, z, YCALC_LOOK, &x_owner, &x_next);
      if (rc < 0)                              break;
      if (x_owner == NULL)                     break;
      /*---(fill pointers)------------------*/
      rc = myCALC.e_pointer (x_owner, &x_source, &x_type, NULL, NULL);
      DEBUG_YCALC   yLOG_value   ("pointer"    , rc);
      if (rc < 0) {
         DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
         return 0;
      }
      DEBUG_YCALC   yLOG_complex ("owner"     , "%-10p, %3db, %3dx, %3dy, %3dz, %c", x_owner, b, i, y, z, *x_type);
      if (*x_source == NULL)                break;
      if (strcmp (*x_source, "<") != 0)     break;
      rc = ycalc_call_who_at (b, i, y, z, YCALC_FULL, &x_owner, &x_next);
      rc = ycalc_deps_create (G_DEP_MERGED, &a_source, &x_next);
      DEBUG_YCALC   yLOG_value   ("create"    , rc);
      if (rc < 0) {
         DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
         return 0;
      }
      *x_type = YCALC_DATA_MERGED;
      rc = ycalc__unmerge_right (&x_next, 0);
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> coordinate a merge -----------------[ ------ [ge.640.134.44]*/ /*-[02.0000.013.6]-*/ /*-[--.---.---.--]-*/
ycalc_merge          (tDEP_ROOT **a_deproot)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   void       *x_owner     = NULL;
   tDEP_ROOT  *x_left      = NULL;
   char      **x_source    = NULL;
   char       *x_type      =  '-';
   int         b, x, y, z;
   tDEP_ROOT  *x_origin    = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YCALC   yLOG_point   ("a_deproot" , a_deproot);
   --rce;  if (a_deproot  == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("*a_deproot", *a_deproot);
   --rce;  if (*a_deproot == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(look left)----------------------*/
   rc = myCALC.e_addresser ((*a_deproot)->owner, &b, &x, &y, &z);
   DEBUG_YCALC   yLOG_value   ("addresser" , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_complex ("loc"       , "b=%-4d, x=%-4d, y=%-4d, z=%-4d", b, x, y, z);
   --rce;  if (x  < 1) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get owner to left)--------------*/
   DEBUG_YCALC   yLOG_note    ("LOOK LEFT FOR MERGE OWNER");
   rc = ycalc_call_who_at (b, x - 1, y, z, YCALC_LOOK, &x_owner, &x_left);
   DEBUG_YCALC   yLOG_value   ("who_at"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("x_owner"   , x_owner);
   --rce;  if (x_owner == NULL) {
      DEBUG_YCALC   yLOG_note    ("nothing to the left to merge with");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*> rc = myCALC.e_pointer (&((*a_deproot)->owner), NULL, &x_type, NULL, NULL);            <*/
   rc = myCALC.e_pointer (x_owner, &x_source, &x_type, NULL, NULL);
   DEBUG_YCALC   yLOG_value   ("pointers"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_char    ("*x_type"   , *x_type);
   DEBUG_YCALC   yLOG_info    ("*x_source" , *x_source);
   /*---(handle merge append)------------*/
   if (*x_type == YCALC_DATA_MERGED) {
      DEBUG_YCALC   yLOG_note    ("handle appending to existing merge");
      DEBUG_YCALC   yLOG_point   ("x_left"    , x_left);
      --rce;  if (x_left == NULL) {
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      rc = ycalc__merge_leftmost (x_left, &x_origin);
      DEBUG_YCALC   yLOG_value   ("leftmost"  , rc);
      --rce;  if (rc < 0 || x_origin == NULL) {
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*> DEBUG_YCALC   yLOG_info    ("x_origin"  , x_origin->owner->label);           <*/
      rc = ycalc_deps_create (G_DEP_MERGED, &x_origin, a_deproot);
      DEBUG_YCALC   yLOG_value   ("create"    , rc);
      --rce;  if (rc < 0) {
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      x_left = x_origin;
   }
   /*---(handle unmergable)--------------*/
   else if (*x_type == YCALC_DATA_BLANK || *x_type == YCALC_DATA_GARBAGE) {
      DEBUG_YCALC   yLOG_note    ("nothing to merge");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(not to merge)-------------------*/
   else if (*x_source != NULL && strcmp (*x_source, "<") == 0) {
      DEBUG_YCALC   yLOG_note    ("unmerged merge indicator to left");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(handle new merge)---------------*/
   else {
      DEBUG_YCALC   yLOG_note    ("handle new merge");
      DEBUG_YCALC   yLOG_point   ("x_left"    , x_left);
      if (x_left == NULL) {
         rc = ycalc_call_who_at (b, x - 1, y, z, YCALC_FULL, &x_owner, &x_left);
      }
      rc = ycalc_deps_create (G_DEP_MERGED, &x_left  , a_deproot);
      DEBUG_YCALC   yLOG_value   ("create"    , rc);
      --rce;  if (rc < 0) {
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(look to the right)--------------*/
   DEBUG_YCALC   yLOG_point   ("x_left"    , x_left);
   rc = ycalc__merge_right (x_left, *a_deproot);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         classification                       ----===*/
/*====================------------------------------------====================*/
static void  o___CLASSIFY________o () { return; }

char
ycalc_shared_verify       (char **a_source, char *a_type, double *a_value, char **a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;           /* return code for errors         */
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(check pointers)-----------------*/
   DEBUG_YCALC   yLOG_point   ("a_source"   , a_source);
   --rce;  if (a_source == NULL) {
      DEBUG_YCALC   yLOG_note    ("source pointer not given, no input to start");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("*a_source"  , *a_source);
   DEBUG_YCALC   yLOG_point   ("a_type"     , a_type);
   --rce;  if (a_type == NULL) {
      DEBUG_YCALC   yLOG_note    ("type pointer not given, can not classify");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_char    ("*a_type"    , *a_type);
   DEBUG_YCALC   yLOG_point   ("a_value"    , a_value);
   --rce;  if (a_value == NULL) {
      DEBUG_YCALC   yLOG_note    ("value pointer not given, no numeric output possible");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_double  ("*a_value"   , *a_value);
   DEBUG_YCALC   yLOG_point   ("a_string"   , a_string);
   --rce;  if (a_string == NULL) {
      DEBUG_YCALC   yLOG_note    ("string pointer not given, no string/error output possible");
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("*a_string"  , *a_string);
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc_classify_clear    (void **a_owner, tDEP_ROOT **a_deproot, char *a_type, double *a_value, char **a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;           /* return code for errors         */
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(clear calculation)-----------*/
   DEBUG_YCALC   yLOG_note    ("wipe existing calculations/rpn");
   if (a_deproot != NULL)  rc = ycalc_calc_wipe  (*a_deproot);
   DEBUG_YCALC   yLOG_value   ("calc"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(clear merges/complex)--------*/
   DEBUG_YCALC   yLOG_note    ("wipe merge dependencies");
   s_origin = NULL;
   if (a_deproot != NULL)  rc = ycalc_unmerge (a_deproot, a_type);
   DEBUG_YCALC   yLOG_value   ("unmerge"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("*a_deproot", *a_deproot);
   /*> --rce;  if (*a_deproot == NULL) {                                              <* 
    *>    DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);                              <* 
    *>    return rce;                                                                 <* 
    *> }                                                                              <*/
   /*---(clear dependencies)----------*/
   DEBUG_YCALC   yLOG_note    ("wipe removable dependencies");
   if (a_deproot != NULL) {
      rc = ycalc_deps_wipe_reqs (a_owner, a_deproot);
      DEBUG_YCALC   yLOG_value   ("cleanser"  , rc);
      --rce;  if (rc < 0) {
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(clear type)------------------*/
   *a_type = YCALC_DATA_BLANK;
   DEBUG_YCALC   yLOG_char    ("*a_type"   , *a_type);
   if (a_deproot != NULL && *a_deproot != NULL) {
      (*a_deproot)->btype = *a_type;
      DEBUG_YCALC   yLOG_char    ("btype"     , (*a_deproot)->btype);
   }
   /*---(string)----------------------*/
   DEBUG_YCALC   yLOG_point   ("a_string"  , a_string);
   if (a_string != NULL) {
      DEBUG_YCALC   yLOG_point   ("*a_string" , *a_string);
      if (*a_string != NULL) {
         DEBUG_YCALC   yLOG_info    ("*a_string" , *a_string);
         free (*a_string);
      }
      *a_string = NULL;
      DEBUG_YCALC   yLOG_point   ("*a_string" , *a_string);
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc__classify_content   (char **a_source, char *a_type, double *a_value)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;           /* return code for errors         */
   char        rc          =    0;
   int         x_len       =    0;
   double      x_value     =  0.0;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(blanks)-------------------------*/
   DEBUG_YCALC   yLOG_point   ("*a_source"    , *a_source);
   if (*a_source == NULL || *a_source [0] == 0) {
      *a_type   = YCALC_DATA_BLANK;
      DEBUG_YCALC   yLOG_note    ("null/empty object");
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   DEBUG_YCALC   yLOG_info    ("*a_source"     , *a_source);
   DEBUG_YCALC   yLOG_char    ("prefix"        , *a_source [0]);
   /*---(merges)-------------------------*/
   x_len = ystrllen (*a_source, LEN_RECD);
   DEBUG_YCALC   yLOG_value   ("x_len"     , x_len);
   if (x_len == 1) {
      if (*a_source [0] == '<') {
         DEBUG_YCALC   yLOG_note    ("merged object");
         *a_type   = YCALC_DATA_MERGED;
         DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
         return 0;
      }
      if (strchr (YCALC_GROUP_FPRE, *a_source [0]) != NULL) {
         DEBUG_YCALC   yLOG_note    ("short string");
         *a_type   = YCALC_DATA_STR;
         DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
         return 0;
      }
      if (strchr (YSTR_NUMBER, *a_source [0]) == NULL) {
         DEBUG_YCALC   yLOG_note    ("short string");
         *a_type   = YCALC_DATA_STR;
         DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
         return 0;
      }
   }
   /*---(variables)----------------------*/
   if (strchr ("�����", *a_source [0]) != NULL) {
      DEBUG_YCALC   yLOG_note    ("found variable");
      *a_type = YCALC_DATA_VAR;
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(formulas)-----------------------*/
   DEBUG_YCALC   yLOG_info    ("valid"     , YCALC_GROUP_FPRE);
   --rce;  if (strchr (YCALC_GROUP_FPRE, *a_source [0]) != NULL) {
      DEBUG_YCALC   yLOG_note    ("formula prefix found");
      /*---(handle types)-------------------*/
      if      (*a_source [0] == '&') {
         if (strstr (*a_source, "..") == NULL)  *a_type = YCALC_DATA_ADDR;
         else                                   *a_type = YCALC_DATA_RANGE;
      }
      else if (*a_source [0] == '!')  *a_type = YCALC_DATA_CADDR;
      else if (*a_source [0] == '�')  *a_type = YCALC_DATA_INTERN;
      else if (*a_source [0] == '=')  *a_type = YCALC_DATA_NFORM;
      else if (*a_source [0] == '#')  *a_type = YCALC_DATA_SFORM;
      else if (*a_source [0] == '~')  *a_type = YCALC_DATA_NLIKE;
      else if (*a_source [0] == '�')  *a_type = YCALC_DATA_MATH;
      /*--> can not distinquish num-like vs str-like until build  */
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(numbers)------------------------*/
   rc = ystrl2num (*a_source, &x_value, LEN_RECD);
   if (rc >= 0) {
      DEBUG_YCALC   yLOG_note    ("numeric literal");
      *a_type   = YCALC_DATA_NUM;
      *a_value  = x_value;
      DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(default string)-----------------*/
   DEBUG_YCALC   yLOG_note    ("default string literal");
   *a_type   = YCALC_DATA_STR;
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc_classify_trusted  (void **a_owner, tDEP_ROOT **a_deproot, char **a_source, char *a_type, double *a_value, char **a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;           /* return code for errors         */
   char        rc          =    0;           /* return code for errors         */
   char        x_intern    =  '-';
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YCALC   yLOG_point   ("*a_source" , *a_source);
   if (*a_source != NULL && *a_source [0] == YCALC_DATA_INTERN) x_intern = 'y';
   /*---(clear)--------------------------*/
   if (x_intern != 'y') {
      rc = ycalc_classify_clear (a_owner, a_deproot, a_type, a_value, a_string);
      DEBUG_YCALC   yLOG_value   ("clear"     , rc);
      --rce;  if (rc < 0) {
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   } else {
      DEBUG_YCALC   yLOG_note    ("do not clear internal summary owners");
   }
   /*---(defense)------------------------*/
   DEBUG_YCALC   yLOG_point   ("*a_owner"  , *a_owner);
   --rce;  if (*a_owner == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("*a_deproot", *a_deproot);
   /*---(reset all values)---------------*/
   if (x_intern != 'y') {
      DEBUG_YCALC   yLOG_note    ("initialize to blank data item");
      *a_type   = YCALC_DATA_BLANK;
      *a_value  = 0.0;
   } else {
      DEBUG_YCALC   yLOG_note    ("do not initialize internal summary owners");
   }
   /*---(prepare)------------------------*/
   rc = ycalc__classify_content   (a_source, a_type, a_value);
   DEBUG_YCALC   yLOG_value   ("content"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(classified)---------------------*/
   DEBUG_YCALC   yLOG_char    ("*a_type"   , *a_type);
   if (a_deproot != NULL && *a_deproot != NULL) {
      (*a_deproot)->btype = *a_type;
      DEBUG_YCALC   yLOG_char    ("btype"     , (*a_deproot)->btype);
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc_classify_detail   (void **a_owner, tDEP_ROOT **a_deproot, char **a_source, char *a_type, double *a_value, char **a_string)
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
   /*---(prepare)------------------------*/
   rc = ycalc_shared_verify (a_source, a_type, a_value, a_string);
   DEBUG_YCALC   yLOG_value   ("verify"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ycalc_classify_trusted (a_owner, a_deproot, a_source, a_type, a_value, a_string);
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
ycalc_classify_owner    (void **a_owner, tDEP_ROOT **a_deproot)
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
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("*a_owner"  , *a_owner);
   --rce;  if (a_owner == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("a_deproot" , a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("*a_deproot", *a_deproot);
   /*---(fill pointers)------------------*/
   rc = myCALC.e_pointer (*a_owner, &x_source, &x_type, &x_value, &x_string);
   DEBUG_YCALC   yLOG_value   ("pointer"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ycalc_classify_detail     (a_owner, a_deproot, x_source, x_type, x_value, x_string);
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
ycalc_classify_label    (char *a_label)
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
   rc = ycalc_classify_owner (&x_owner, &x_deproot);
   DEBUG_YCALC   yLOG_value   ("owner"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        overall handler                       ----===*/
/*====================------------------------------------====================*/
static void  o___HANDLE__________o () { return; }

char
yCALC_handle            (char *a_label)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;           /* return code for errors         */
   char        rc          =    0;           /* return code for errors         */
   void       *x_owner     = NULL;
   tDEP_ROOT  *x_deproot   = NULL;
   char      **x_source    = NULL;
   char       *x_type      = NULL;
   double     *x_value     = NULL;
   char      **x_string    = NULL;
   int         x_len       =    0;
   int         b, x, y, z;
   tDEP_ROOT  *x_origin    = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (ycalc_not_ready ())  {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("a_label"    , a_label);
   --rce;  if (a_label == NULL) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_info    ("a_label"    , a_label);
   /*---(get label)----------------------*/
   rc = ycalc_call_who_named  (a_label, YCALC_FULL, &x_owner, &x_deproot);
   DEBUG_YCALC   yLOG_value   ("who_named"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YCALC   yLOG_point   ("x_owner"    , x_owner);
   DEBUG_YCALC   yLOG_point   ("x_deproot"  , x_deproot);
   /*---(variables)----------------------*/
   if (x_deproot != NULL) {
      DEBUG_YCALC   yLOG_point   ("btype"      , x_deproot->btype);
      if (x_deproot->btype == YCALC_DATA_VAR) {
         DEBUG_YCALC   yLOG_note    ("delete existing var status");
         rc = ycalc_vars_del (a_label, &x_deproot);
         /*---(recreate deproot)--------*/
         rc = ycalc_call_who_named  (a_label, YCALC_FULL, &x_owner, &x_deproot);
         DEBUG_YCALC   yLOG_value   ("who_named"  , rc);
         --rce;  if (rc < 0) {
            DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
         DEBUG_YCALC   yLOG_point   ("x_owner"    , x_owner);
         DEBUG_YCALC   yLOG_point   ("x_deproot"  , x_deproot);
      }
   }
   /*---(fill pointers)------------------*/
   rc = myCALC.e_pointer (x_owner, &x_source, &x_type, &x_value, &x_string);
   DEBUG_YCALC   yLOG_value   ("pointer"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ycalc_shared_verify (x_source, x_type, x_value, x_string);
   DEBUG_YCALC   yLOG_value   ("prepare"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(classify)-----------------------*/
   rc = ycalc_classify_trusted (&x_owner, &x_deproot, x_source, x_type, x_value, x_string);
   DEBUG_YCALC   yLOG_value   ("classify"  , rc);
   DEBUG_YCALC   yLOG_char    ("*x_type"   , *x_type);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(merges)-------------------------*/
   if (*x_type == YCALC_DATA_MERGED || *x_type == YCALC_DATA_PMERGE) {
      rc = ycalc_merge (&x_deproot);
      if (rc < 0) {
         *x_type = YCALC_DATA_PMERGE;
      }
   } else {
      rc = ycalc_merge_check    (x_deproot);
   }
   DEBUG_YCALC   yLOG_value   ("merging"   , rc);
   /*---(ranges)-------------------------*/
   DEBUG_YCALC   yLOG_point   ("x_owner"   , x_owner);
   rc = myCALC.e_addresser (x_owner, &b, &x, &y, &z);
   DEBUG_YCALC   yLOG_value   ("addresser" , rc);
   --rce;  if (rc < 0) {
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (*x_type != YCALC_DATA_BLANK) {
      rc = ycalc_range_include (&x_deproot, b, x, y, z);
      DEBUG_YCALC   yLOG_value   ("ranges"    , rc);
      --rce;  if (rc < 0) {
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(calculation)--------------------*/
   DEBUG_YCALC   yLOG_note    ("now consider build and execution");
   DEBUG_YCALC   yLOG_char    ("*x_type"   , *x_type);
   DEBUG_YCALC   yLOG_info    ("valid"     , YCALC_GROUP_RPN);
   /*---(variables)----------------------*/
   if (*x_type == YCALC_DATA_VAR) {
      rc = ycalc_build_variable (x_deproot, a_label, b, x, y, z, x_source, x_type, x_value, x_string);
      DEBUG_YCALC   yLOG_value   ("variable"  , rc);
      /*> /+---BEG TESTING NEW CODE----------+/                                       <* 
       *> DEBUG_YCALC   yLOG_note    ("calling printer");                             <* 
       *> rc = myCALC.e_printer (x_owner);                                            <* 
       *> DEBUG_YCALC   yLOG_value   ("printer"   , rc);                              <* 
       *> --rce;  if (rc < 0) {                                                       <* 
       *>    DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);                          <* 
       *>    return rce;                                                              <* 
       *> }                                                                           <* 
       *> /+---END TESTING NEW CODE----------+/                                       <* 
       *> DEBUG_YCALC   yLOG_exit    (__FUNCTION__);                                  <* 
       *> return 0;                                                                   <*/
   }
   /*---(build)--------------------------*/
   if (strchr (YCALC_GROUP_RPN , *x_type) != NULL) {
      rc = ycalc_build_trusted    (x_deproot, x_source, x_type, x_value, x_string);
      DEBUG_YCALC   yLOG_value   ("build"     , rc);
   }
   /*---(execute)------------------------*/
   if (*x_type != YCALC_DATA_ERROR && strchr (YCALC_GROUP_CALC, *x_type) != NULL) {
      rc = ycalc_execute_trusted  (x_deproot, x_type, x_value, x_string);
      DEBUG_YCALC   yLOG_value   ("execute"   , rc);
   }
   /*---(calc upward)--------------------*/
   if (x_deproot != NULL) {
      rc = yCALC_seq_up (x_deproot, ycalc_execute_auto);
      DEBUG_YCALC   yLOG_value   ("seq"       , rc);
      --rce;  if (rc < 0)  {
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*> ycalc__mock_list ();                                                           <*/
   /*---(call reaper)--------------------*/
   DEBUG_YCALC   yLOG_note    ("then, cleanup as necessary");
   if (*x_source == NULL || strcmp (*x_source, "<") != 0) {
      rc = ycalc_call_reaper (&x_owner, &x_deproot);
      DEBUG_YCALC   yLOG_value   ("reaper"    , rc);
      DEBUG_YCALC   yLOG_point   ("x_owner"   , x_owner);
      DEBUG_YCALC   yLOG_point   ("x_deproot" , x_deproot);
   }
   /*---(call printer)-------------------*/
   if (x_owner != NULL) {
      DEBUG_YCALC   yLOG_note    ("check for merge");
      if (*x_type == YCALC_DATA_MERGED) {
         DEBUG_YCALC   yLOG_note    ("merged, find source");
         DEBUG_YCALC   yLOG_point   ("x_deproot" , x_deproot);
         rc = ycalc__merge_leftmost (x_deproot, &x_origin);
         DEBUG_YCALC   yLOG_point   ("x_origin"  , x_origin);
         if (x_origin != NULL)  x_owner = x_origin->owner;
      }
      DEBUG_YCALC   yLOG_point   ("x_owner"   , x_owner);
      DEBUG_YCALC   yLOG_note    ("calling printer");
      rc = myCALC.e_printer (x_owner);
      DEBUG_YCALC   yLOG_value   ("printer"   , rc);
      --rce;  if (rc < 0) {
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*> ycalc__mock_list ();                                                           <*/
   DEBUG_YCALC   yLOG_point   ("s_origin"  , s_origin);
   if (s_origin != NULL) {
      rc = myCALC.e_printer (s_origin);
   }
   /*---(ripple to likes)----------------*/
   if (x_deproot != NULL) {
      rc = ycalc_build_ripple (x_deproot, G_DEP_LIKE);
      --rce;  if (rc < 0) {
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*> ycalc__mock_list ();                                                           <*/
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                          sorting list                        ----===*/
/*====================------------------------------------====================*/
static void  o___SORT____________o () { return; }

static long s_array     [1000];          /* working list as integers       */
static int  s_narray    =  0;            /* working count of entries       */

static long s_bf        = 1000000000000;
static long s_xf        = 100000000;
static long s_yf        = 10000;
static long s_zf        = 1;

char         /*-> sort cell label list ---------------[ ------ [ge.RE5.1H6.A2]*/ /*-[03.0000.11#.!]-*/ /*-[--.---.---.--]-*/
ycalc__sort_prep   (char *a_list)
{
   /*---(locals)-----------+-----------+-*//*---------------------------------*/
   char        rce         = -10;           /* return code for errors         */
   char        x_list      [LEN_RECD];      /* copy of the list for parsing   */
   char       *p           = NULL;          /* strtok parsing pointer         */
   char       *q           = ",;";          /* strtok delimeter string        */
   char       *r           = NULL;
   int         i           =  0;            /* loop iterator -- entry         */
   int         b           =  0;            /* col of current entry           */
   int         x           =  0;            /* col of current entry           */
   int         y           =  0;            /* row of current entry           */
   int         z           =  0;            /* tab of current entry           */
   char        rc          =  0;            /* return code                    */
   void       *x_owner     = NULL;
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSORT   yLOG_point   ("a_list"    , a_list);
   --rce;  if (a_list == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSORT   yLOG_info    ("a_list"    , a_list);
   /*---(initialize)---------------------*/
   DEBUG_YSORT   yLOG_note    ("initialize");
   for (i = 0; i < 1000; ++i)  s_array [i] = 0;
   s_narray = 0;
   /*---(parse/load)---------------------*/
   DEBUG_YSORT   yLOG_note    ("load the array");
   ystrlcpy (x_list, a_list,  LEN_RECD);
   ystrlcpy (a_list, "#PREP", LEN_RECD);
   p = strtok_r (x_list, q, &r);
   s_narray = 0;
   --rce;  while (p != NULL) {
      DEBUG_YSORT   yLOG_info    ("parse"     , p);
      rc = myCALC.e_who_named  (p, YCALC_LOOK, &x_owner, NULL);
      DEBUG_YSORT   yLOG_value   ("who_named" , rc);
      if (rc < 0)  {
         DEBUG_YSORT   yLOG_note    ("could not find owner, EXITING");
         DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      rc = myCALC.e_addresser (x_owner, &b, &x, &y, &z);
      DEBUG_YSORT   yLOG_value   ("addresser" , rc);
      if (rc < 0)  {
         DEBUG_YSORT   yLOG_note    ("could not parse, EXITING");
         DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_YSORT   yLOG_complex ("parts"     , "b=%04d, x=%04d, y=%04d, z=%04d", b, x, y, z);
      ++b;
      ++x;
      ++y;
      ++z;
      DEBUG_YSORT   yLOG_complex ("inserted"  , "b=%04d, x=%04d, y=%04d, z=%04d", b, x, y, z);
      s_array [s_narray] = (b * s_bf) + (x * s_xf) + (y * s_yf) + z;
      DEBUG_YSORT   yLOG_pair    (s_narray         , s_array [s_narray]);
      p = strtok_r (NULL  , q, &r);
      ++s_narray;
   }
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> sort cell label list ---------------[ ------ [ge.RE5.1H6.A2]*/ /*-[03.0000.11#.!]-*/ /*-[--.---.---.--]-*/
ycalc__sort_itself (void)
{
   /*---(locals)-----------+-----------+-*//*---------------------------------*/
   int         i           =  0;            /* loop iterator -- entry         */
   long        a           =  0;            /* comparison entry one           */
   long        b           =  0;            /* comparison entry two           */
   int         tele        = -1;            /* teleport point to speed sort   */
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(sort)---------------------------*/
   i = 1;
   while (i < s_narray) {
      /*---(load vars)-------------------*/
      a = s_array [i - 1];
      b = s_array [i];
      DEBUG_YSORT   yLOG_complex ("current"   , "compare i,%3d (t,%3d) =  a,%10d to b,%10d", i, tele, a, b);
      /*---(compare)---------------------*/
      if (i == 0 || a <= b) {
         if (tele >= 0) {
            i    = tele;
            tele = -1;
         } else {
            ++i;
         }
         continue;
      }
      /*---(swap)------------------------*/
      s_array [i - 1] = b;
      s_array [i]     = a;
      a = s_array [i - 1];
      b = s_array [i];
      DEBUG_YSORT   yLOG_complex ("swapped"   , "now     i,%3d (t,%3d) =  a,%10d to b,%10d", i, tele, a, b);
      /*---(update)----------------------*/
      if (tele < 0) tele = i;
      if (i > 1) --i;
   }
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> sort cell label list ---------------[ ------ [ge.RE5.1H6.A2]*/ /*-[03.0000.11#.!]-*/ /*-[--.---.---.--]-*/
ycalc__sort_wrap   (char *a_list)
{
   /*---(locals)-----------+-----------+-*//*---------------------------------*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;            /* loop iterator -- entry         */
   int         b           =    0;            /* col of current entry           */
   int         x           =    0;            /* col of current entry           */
   int         y           =    0;            /* row of current entry           */
   int         z           =    0;            /* tab of current entry           */
   long        x_rem       =    0;
   void       *x_owner     = NULL;
   tDEP_ROOT  *x_deproot   = NULL;
   char        x_label     [LEN_LABEL];     /* label for sorted entry         */
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   ystrlcpy (a_list, ",", LEN_RECD);
   for (i = 0; i < s_narray; ++i) {
      DEBUG_YSORT   yLOG_value   ("value"   , s_array[i]);
      x_rem  = s_array [i];
      b      = x_rem / s_bf;
      x_rem -= b * s_bf;
      x      = x_rem / s_xf;
      x_rem -= x * s_xf;
      y      = x_rem / s_yf;
      x_rem -= y * s_yf;
      z      = x_rem;
      --b;
      --x;
      --y;
      --z;
      DEBUG_YSORT   yLOG_complex ("parts"     , "b=%04d, x=%04d, y=%04d, z=%04d", b, x, y, z);
      rc = ycalc_call_who_at (b, x, y, z, YCALC_LOOK, &x_owner, &x_deproot);
      if      (x_owner   == NULL)   sprintf (x_label, "[#%d]", i);
      else if (rc        <  0   )   sprintf (x_label, "<#%d>", i);
      else if (x_deproot != NULL)   ystrlcpy (x_label, ycalc_call_labeler (x_deproot), LEN_LABEL);
      else                          sprintf (x_label, "{#%d}", i);
      DEBUG_YSORT   yLOG_info    ("label"   , x_label);
      ystrlcat (a_list, x_label, LEN_RECD);
      ystrlcat (a_list, ","    , LEN_RECD);
   }
   DEBUG_YSORT   yLOG_info    ("final"     , a_list);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> sort cell label list ---------------[ ------ [ge.RE5.1H6.A2]*/ /*-[03.0000.11#.!]-*/ /*-[--.---.---.--]-*/
ycalc__audit_sort  (char *a_list)
{
   /*---(purpose)-------------------------------------------------------------* 
    * for unit testing to be reliable, cosmetic changes must not result in    *
    * failures or the need to update the unit test script.  label lists from  *
    * the dependency linked lists are a prime example.  they are added in the *
    * order they are encountered, but simple reordering of operations can     *
    * make them appear different even if the same cells are listed.  this     *
    * routine simply sorts a list of dependencies so they are more stable.    */
   /*---(design notes)--------------------------------------------------------* 
    * there are many sort algorithms which can handle vast amounts of complex *
    * data, but they are simply overkill and problematic for small, simple    *
    * lists.  the teleporting gnome sort is my personal favorite due to its   *
    * simplicity which in turn leads to maintainability and confidence.       */
   /*---(oddities)------------------------------------------------------------* 
    * one is added to tab, col, and row before storage in the array and       *
    * sorting as the all three are consolidated mathmatically into a single   *
    * fieild and zeros are just a little harder to debug ;)                   */
   /*---(locals)-----------+-----------+-*//*---------------------------------*/
   char        rce         = -10;           /* return code for errors         */
   char        rc          =  0;            /* return code                    */
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(defense: null)------------------*/
   --rce;  if (a_list  == NULL)  {
      DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(sort)---------------------------*/
   if (rc == 0)   rc = ycalc__sort_prep   (a_list);
   if (rc == 0)   rc = ycalc__sort_itself ();
   if (rc == 0)   rc = ycalc__sort_wrap   (a_list);
   /*---(complete)-----------------------*/
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                         information                          ----===*/
/*====================------------------------------------====================*/
static void  o___INFO____________o () { return; }

char
ycalc__audit_disp_range    (tDEP_ROOT *a_range, char a_start, char *a_list)
{
   /*---(locals)-----------+-----------+-*/
   tDEP_LINK  *n           = NULL;
   char        rce         = -10;
   /*---(setup)--------------------------*/
   DEBUG_YSORT   yLOG_char    ("a_start"   , a_start);
   --rce;  switch (a_start) {
   case 'R' :  n = a_range->reqs; break;
   case 'P' :  n = a_range->pros; break;
   default         :  return rce;
   }
   DEBUG_YSORT   yLOG_info    ("a_list"    , a_list);
   /*---(walk the list)------------------*/
   while (n != NULL) {
      ystrlcat (a_list, ycalc_call_labeler (n->target), LEN_RECD);
      ystrlcat (a_list, ","                           , LEN_RECD);
      n = n->next;
   }
   DEBUG_YSORT   yLOG_info    ("a_list"    , a_list);
   /*---(complete)-----------------------*/
   return 0;
}

char         /*-> list dependencies ------------------[ leaf   [fe.740.424.60]*/ /*-[02.0000.034.!]-*/ /*-[--.---.---.--]-*/
ycalc__audit_disp_master   (tDEP_ROOT *a_me, char *a_list, char a_start, char *a_types, char a_extra)
{  /*---(design notes)-------------------*/
   /* combined logic from the three accessors below as they were 90% the same */
   /*---(locals)-----------+-----------+-*/
   tDEP_LINK  *n           = NULL;
   char        rce         = -10;
   uchar       x_label     [LEN_LABEL] = "";
   uchar       x_type      [LEN_TERSE] = "";
   /*---(header)-------------------------*/
   DEBUG_YSORT   yLOG_enter   (__FUNCTION__);
   /*---(defenses)-----------------------*/
   DEBUG_YSORT   yLOG_point   ("a_list"    , a_list);
   --rce;  if (a_list  == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   ystrlcpy (a_list, "-", LEN_RECD);   /* special for a null list */
   DEBUG_YSORT   yLOG_info    ("a_list"    , a_list);
   DEBUG_YSORT   yLOG_point   ("a_me"      , a_me);
   --rce;  if (a_me    == NULL) {
      DEBUG_YSORT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(setup)--------------------------*/
   ystrlcpy (a_list, ",", LEN_RECD);
   DEBUG_YSORT   yLOG_char    ("a_start"   , a_start);
   --rce;  switch (a_start) {
   case 'R' :  n = a_me->reqs; break;
   case 'P' :  n = a_me->pros; break;
   default         :  return rce;
   }
   DEBUG_YSORT   yLOG_info    ("a_list"    , a_list);
   /*---(walk the list)------------------*/
   DEBUG_YSORT   yLOG_info    ("a_types"   , a_types);
   while (n != NULL) {
      if (strchr (a_types, n->type) != 0) {
         ystrlcpy (x_label, ycalc_call_labeler (n->target), LEN_LABEL);
         if (x_label [0] == (uchar) '�') {
            ystrlcat (a_list, x_label, LEN_RECD);
            ystrlcat (a_list, ","    , LEN_RECD);
            ycalc__audit_disp_range (n->target, a_start, a_list);
            ystrlcat (a_list, ","    , LEN_RECD);
         } else {
            ystrlcat (a_list, x_label, LEN_RECD);
            if (a_extra == 'y') {
               sprintf (x_type, "%c", n->type);
               ystrlcat (a_list, x_type , LEN_RECD);
            }
            ystrlcat (a_list, ","    , LEN_RECD);
         }
      }
      n = n->next;
   }
   /*---(catch empty)--------------------*/
   if (strcmp (a_list, ",") == 0)   ystrlcpy (a_list, ".", LEN_RECD);
   else if (a_extra == '-')  ycalc__audit_sort (a_list);
   DEBUG_YSORT   yLOG_info    ("a_list"    , a_list);
   DEBUG_YSORT   yLOG_exit    (__FUNCTION__);
   /*---(complete)-----------------------*/
   return 0;
}

char       yCALC_disp_reqs      (void *a_me, char *a_list) { return ycalc__audit_disp_master ((tDEP_ROOT *) a_me, a_list, 'R', S_DEP_REQS, '-'); }
char       yCALC_disp_reqsplus  (void *a_me, char *a_list) { return ycalc__audit_disp_master ((tDEP_ROOT *) a_me, a_list, 'R', S_DEP_REQS, 'y'); }
char       yCALC_disp_pros      (void *a_me, char *a_list) { return ycalc__audit_disp_master ((tDEP_ROOT *) a_me, a_list, 'P', S_DEP_PROS, '-'); }
char       yCALC_disp_prosplus  (void *a_me, char *a_list) { return ycalc__audit_disp_master ((tDEP_ROOT *) a_me, a_list, 'P', S_DEP_PROS, 'y'); }
char       yCALC_disp_like      (void *a_me, char *a_list) { return ycalc__audit_disp_master ((tDEP_ROOT *) a_me, a_list, 'R', "S"       , '-'); }
char       yCALC_disp_copy      (void *a_me, char *a_list) { return ycalc__audit_disp_master ((tDEP_ROOT *) a_me, a_list, 'P', "l"       , '-'); }



/*====================------------------------------------====================*/
/*===----                       sheet-functions                        ----===*/
/*====================------------------------------------====================*/
static void  o___FORMULAS________o () { return; }

void         /*-> tbd --------------------------------[ ------ [gv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_type          (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   ycalc_pushval (__FUNCTION__, a);
   return;
}

void
ycalc_typealpha     (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc_type ();
   ycalc_char ();
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_typestr       (void)
{
   char        b           =  0;
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   b = a;
   switch (b) {
   case YCALC_DATA_BLANK  :   strcpy (t, "blank");   break;
   case YCALC_DATA_STR    :   strcpy (t, "slit");    break;
   case YCALC_DATA_SFORM  :   strcpy (t, "scalc");   break;
   case YCALC_DATA_SLIKE  :   strcpy (t, "slike");   break;
   case YCALC_DATA_NUM    :   strcpy (t, "nlit");    break;
   case YCALC_DATA_NFORM  :   strcpy (t, "ncalc");   break;
   case YCALC_DATA_NLIKE  :   strcpy (t, "nlike");   break;
   case YCALC_DATA_MATH   :   strcpy (t, "math");    break;
   case YCALC_DATA_ADDR   :   strcpy (t, "ref");     break;
   case YCALC_DATA_CADDR  :   strcpy (t, "cref");    break;
   case YCALC_DATA_RLIKE  :   strcpy (t, "rlike");   break;
   case YCALC_DATA_RANGE  :   strcpy (t, "rref");    break;
   case YCALC_DATA_VAR    :   strcpy (t, "var");     break;
   case YCALC_DATA_MERGED :   strcpy (t, "merged");  break;
   case YCALC_DATA_ERROR  :   strcpy (t, "error");   break;
   }
   ycalc_pushstr (__FUNCTION__, t);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.210.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_isblank       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (a == YCALC_DATA_BLANK)   ycalc_pushval (__FUNCTION__, TRUE);
   else                         ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.210.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_isvalue       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (strchr (YCALC_GROUP_NUM, a) != NULL) ycalc_pushval (__FUNCTION__, TRUE);
   else                                     ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.210.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_isnlit        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (a == YCALC_DATA_NUM    ) ycalc_pushval (__FUNCTION__, TRUE);
   else                         ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.210.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_isncalc       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (a == YCALC_DATA_NFORM  ) ycalc_pushval (__FUNCTION__, TRUE);
   else                         ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.210.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_isnmath       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (a == YCALC_DATA_MATH   ) ycalc_pushval (__FUNCTION__, TRUE);
   else                         ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.210.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_isnlike       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (a == YCALC_DATA_NLIKE  ) ycalc_pushval (__FUNCTION__, TRUE);
   else                         ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.210.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_istext        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (strchr (YCALC_GROUP_STR, a) != NULL) ycalc_pushval (__FUNCTION__, TRUE);
   else                                     ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.210.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_isslit        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (a == YCALC_DATA_STR    ) ycalc_pushval (__FUNCTION__, TRUE);
   else                         ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.210.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_isscalc       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (a == YCALC_DATA_SFORM  ) ycalc_pushval (__FUNCTION__, TRUE);
   else                         ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.210.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_isslike       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (a == YCALC_DATA_SLIKE  ) ycalc_pushval (__FUNCTION__, TRUE);
   else                         ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.210.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_iscalc        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if      (a == YCALC_DATA_NFORM  ) ycalc_pushval (__FUNCTION__, TRUE);
   else if (a == YCALC_DATA_SFORM  ) ycalc_pushval (__FUNCTION__, TRUE);
   else                              ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.210.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_islike        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if      (a == YCALC_DATA_NLIKE  ) ycalc_pushval (__FUNCTION__, TRUE);
   else if (a == YCALC_DATA_SLIKE  ) ycalc_pushval (__FUNCTION__, TRUE);
   else if (a == YCALC_DATA_RLIKE  ) ycalc_pushval (__FUNCTION__, TRUE);
   else                              ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_isform        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (strchr (YCALC_GROUP_CALC, a) != NULL) ycalc_pushval (__FUNCTION__, TRUE);
   else                                      ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.210.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_islit         (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if      (a == YCALC_DATA_NUM  ) ycalc_pushval (__FUNCTION__, TRUE);
   else if (a == YCALC_DATA_STR  ) ycalc_pushval (__FUNCTION__, TRUE);
   else                            ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.220.000.32]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_isnref        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if      (a == YCALC_DATA_ADDR ) ycalc_pushval (__FUNCTION__, TRUE);
   else                            ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.220.000.32]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_iscref        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if      (a == YCALC_DATA_CADDR) ycalc_pushval (__FUNCTION__, TRUE);
   else                            ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.220.000.32]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_isrlike       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if      (a == YCALC_DATA_RLIKE) ycalc_pushval (__FUNCTION__, TRUE);
   else                            ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.220.000.32]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_isaddr        (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if      (a == YCALC_DATA_ADDR ) ycalc_pushval (__FUNCTION__, TRUE);
   else if (a == YCALC_DATA_CADDR) ycalc_pushval (__FUNCTION__, TRUE);
   else if (a == YCALC_DATA_RLIKE) ycalc_pushval (__FUNCTION__, TRUE);
   else                            ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.210.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_isrange       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (a == YCALC_DATA_RANGE  ) ycalc_pushval (__FUNCTION__, TRUE);
   else                         ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.220.000.32]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_ispoint       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if      (a == YCALC_DATA_RANGE) ycalc_pushval (__FUNCTION__, TRUE);
   else if (a == YCALC_DATA_ADDR ) ycalc_pushval (__FUNCTION__, TRUE);
   else if (a == YCALC_DATA_CADDR) ycalc_pushval (__FUNCTION__, TRUE);
   else if (a == YCALC_DATA_RLIKE) ycalc_pushval (__FUNCTION__, TRUE);
   else                            ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.210.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_isvar         (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (a == YCALC_DATA_VAR  )   ycalc_pushval (__FUNCTION__, TRUE);
   else                         ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.210.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_ismerge       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (a == YCALC_DATA_MERGED ) ycalc_pushval (__FUNCTION__, TRUE);
   else                         ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.210.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_iserror       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (strchr (YCALC_GROUP_ERR , a) != NULL) ycalc_pushval (__FUNCTION__, TRUE);
   else                                      ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_upos          (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   m = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_UPOS);
   ycalc_pushval (__FUNCTION__, m);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_xpos          (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   m = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_XPOS);
   ycalc_pushval (__FUNCTION__, m);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_ypos          (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   m = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_YPOS);
   ycalc_pushval (__FUNCTION__, m);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_zpos          (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   m = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_ZPOS);
   ycalc_pushval (__FUNCTION__, m);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_me            (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc_pushref (__FUNCTION__, myCALC.deproot, myCALC.label);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_label         (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_LABEL);
   ycalc_pushstr (__FUNCTION__, r);
   free (r);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_formula       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_SOURCE);
   ycalc_pushstr (__FUNCTION__, r);
   free (r);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rpn           (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_RPN);
   ycalc_pushstr (__FUNCTION__, r);
   free (r);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rpn_basic     (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_SOURCE);
   yRPN_gyges (r, NULL, NULL, LEN_RECD, 0);
   yRPN_get   (YRPN_SHUNTED, t, NULL);
   ycalc_pushstr (__FUNCTION__, t);
   free (r);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rpn_tokens    (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_SOURCE);
   yRPN_gyges (r, NULL, NULL, LEN_RECD, 0);
   yRPN_get   (YRPN_TOKENS , t, NULL);
   ycalc_pushstr (__FUNCTION__, t);
   free (r);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rpn_parsed    (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_SOURCE);
   yRPN_gyges (r, NULL, NULL, LEN_RECD, 0);
   yRPN_get   (YRPN_PARSED , t, NULL);
   ycalc_pushstr (__FUNCTION__, t);
   free (r);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rpn_detail    (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_SOURCE);
   yRPN_gyges (r, NULL, NULL, LEN_RECD, 0);
   yRPN_get   (YRPN_DETAIL , t, NULL);
   ycalc_pushstr (__FUNCTION__, t);
   free (r);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rpn_debug     (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_SOURCE);
   yRPN_gyges (r, NULL, NULL, LEN_RECD, 0);
   yRPN_get   (YRPN_DEBUG  , t, NULL);
   ycalc_pushstr (__FUNCTION__, t);
   free (r);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rpn_pretty    (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_SOURCE);
   yRPN_gyges (r, NULL, NULL, LEN_RECD, 0);
   yRPN_get   (YRPN_PRETTY , t, NULL);
   ycalc_pushstr (__FUNCTION__, t);
   free (r);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rpn_mathy     (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_SOURCE);
   yRPN_gyges (r, NULL, NULL, LEN_RECD, 0);
   yRPN_get   (YRPN_MATHY  , t, NULL);
   ycalc_pushstr (__FUNCTION__, t);
   free (r);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rpn_exact     (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_SOURCE);
   yRPN_gyges (r, NULL, NULL, LEN_RECD, 0);
   yRPN_get   (YRPN_EXACT  , t, NULL);
   ycalc_pushstr (__FUNCTION__, t);
   free (r);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_reqs          (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_REQS);
   ycalc_pushstr (__FUNCTION__, r);
   free (r);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_pros          (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_PROS);
   ycalc_pushstr (__FUNCTION__, r);
   free (r);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_ncalc         (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_NCALC);
   ycalc_pushval (__FUNCTION__, a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_nreq          (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_NREQ);
   ycalc_pushval (__FUNCTION__, a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_npro          (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_NPRO);
   ycalc_pushval (__FUNCTION__, a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_level         (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_LEVEL);
   ycalc_pushval (__FUNCTION__, a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_pointer       (void)
{
   DEBUG_YCALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tDEP_ROOT  *x_ref       = NULL;
   tCALC      *x_calc      = NULL;
   char       *x_string    = NULL;
   tDEP_ROOT  *x_final     = NULL;
   char       *x_type      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YCALC   yLOG_enter   (__FUNCTION__);
   /*---(get the reference)--------------*/
   x_ref = ycalc_popref (__FUNCTION__);
   DEBUG_YCALC   yLOG_point   ("x_ref"     , x_ref);
   --rce;  if (x_ref == NULL) {
      ycalc_error_set (YCALC_ERROR_EXEC_MAL, NULL);
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return;
   }
   DEBUG_YCALC   yLOG_char    ("->btype"    , x_ref->btype);
   ycalc_error_set (YCALC_ERROR_NONE, x_ref);
   DEBUG_YCALC   yLOG_info    ("myCALC.me"  , myCALC.me);
   /*---(dereference)--------------------*/
   x_calc = x_ref->chead;
   DEBUG_YCALC   yLOG_point   ("x_calc"    , x_calc);
   --rce;  if (x_calc == NULL) {
      ycalc_error_set (YCALC_ERROR_EXEC_MAL, x_ref);
      DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
      return;
   }
   DEBUG_YCALC   yLOG_char    ("x_calc->t" , x_calc->t);
   /*---(check for address)--------------*/
   --rce;  if (x_ref->btype == YCALC_DATA_ADDR) {
      DEBUG_YCALC   yLOG_note    ("returning redirection for literal address");
      if (x_ref->ncalc != 1) {
         ycalc_error_set (YCALC_ERROR_EXEC_MAL, x_ref);
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return;
      }
      if (x_calc->t != G_TYPE_REF) {
         ycalc_error_set (YCALC_ERROR_EXEC_IND, x_ref);
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return;
      }
      DEBUG_YCALC   yLOG_point   ("x_calc->r"  , x_calc->r);
      if (x_calc->r == NULL) {
         ycalc_error_set (YCALC_ERROR_EXEC_NULL, x_ref);
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return;
      }
      /*> rc = myCALC.e_pointer (x_calc->r->owner, NULL, &x_type, NULL, NULL);        <* 
       *> DEBUG_YCALC   yLOG_char    ("type"      , x_type);                          <* 
       *> if (rc < 0 || x_type == YCALC_DATA_ERROR) {                                 <* 
       *>    ycalc_error_set (YCALC_ERROR_EXEC_PERR, x_ref);                          <* 
       *>    DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);                          <* 
       *>    return;                                                                  <* 
       *> }                                                                           <*/
      /*---(push reference back)------------*/
      DEBUG_YCALC   yLOG_info    ("x_calc->s"  , x_calc->s);
      ycalc_pushref (__FUNCTION__, x_calc->r, x_calc->s);
   }
   /*---(check for calc-address)---------*/
   --rce;  if (x_ref->btype == YCALC_DATA_CADDR || x_ref->btype == YCALC_DATA_RLIKE) {
      DEBUG_YCALC   yLOG_note    ("returning redirection for calculated address");
      if (myCALC.e_valuer == NULL) {
         ycalc_error_set (YCALC_ERROR_CONF, NULL);
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return;
      }
      myCALC.e_valuer (x_ref->owner, NULL, NULL, &x_string);
      DEBUG_YCALC   yLOG_point   ("x_string"   , x_string);
      if (x_string == NULL) {
         ycalc_error_set (YCALC_ERROR_CONF, NULL);
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return;
      }
      DEBUG_YCALC   yLOG_info    ("x_string"  , x_string);
      ycalc_call_who_named (x_string, YCALC_FULL, NULL, &x_final);
      DEBUG_YCALC   yLOG_point   ("x_final"    , x_final);
      if (x_final == NULL) {
         ycalc_error_set (YCALC_ERROR_EXEC_NULL, x_final);
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return;
      }
      /*> rc = myCALC.e_pointer (x_final->owner, NULL, &x_type, NULL, NULL);          <* 
       *> DEBUG_YCALC   yLOG_char    ("type"      , x_type);                          <* 
       *> if (rc < 0 || x_type == YCALC_DATA_ERROR) {                                 <* 
       *>    ycalc_error_set (YCALC_ERROR_EXEC_PERR, x_ref);                          <* 
       *>    DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);                          <* 
       *>    return;                                                                  <* 
       *> }                                                                           <*/
      /*---(push reference back)------------*/
      ycalc_pushref (__FUNCTION__, x_final, x_string);
   }
   /*---(check for range)----------------*/
   --rce;  if (x_ref->btype == YCALC_DATA_RANGE) {
      DEBUG_YCALC   yLOG_note    ("returning redirection for literal range");
      if (x_ref->ncalc != 3) {
         ycalc_error_set (YCALC_ERROR_EXEC_MAL, x_ref);
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return;
      }
      x_calc = x_calc->next->next;
      DEBUG_YCALC   yLOG_point   ("x_calc"    , x_calc);
      if (x_calc == NULL) {
         ycalc_error_set (YCALC_ERROR_EXEC_MAL, x_ref);
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return;
      }
      DEBUG_YCALC   yLOG_char    ("x_calc->t" , x_calc->t);
      if (x_calc->t != G_TYPE_REF) {
         ycalc_error_set (YCALC_ERROR_EXEC_IND, x_ref);
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return;
      }
      DEBUG_YCALC   yLOG_point   ("x_calc->r"  , x_calc->r);
      if (x_calc->r == NULL) {
         ycalc_error_set (YCALC_ERROR_EXEC_NULL, x_ref);
         DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);
         return;
      }
      /*> rc = myCALC.e_pointer (x_calc->r->owner, NULL, &x_type, NULL, NULL);        <* 
       *> DEBUG_YCALC   yLOG_char    ("type"      , x_type);                          <* 
       *> if (rc < 0 || x_type == YCALC_DATA_ERROR) {                                 <* 
       *>    ycalc_error_set (YCALC_ERROR_EXEC_PERR, x_ref);                          <* 
       *>    DEBUG_YCALC   yLOG_exitr   (__FUNCTION__, rce);                          <* 
       *>    return;                                                                  <* 
       *> }                                                                           <*/
      /*---(push reference back)------------*/
      DEBUG_YCALC   yLOG_info    ("x_calc->s"  , x_calc->s);
      ycalc_pushref (__FUNCTION__, x_calc->r, x_calc->s);
   }
   /*---(complete)-----------------------*/
   DEBUG_YCALC   yLOG_exit    (__FUNCTION__);
   return;
}




