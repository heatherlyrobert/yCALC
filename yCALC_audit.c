/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"



const tyCALC_TYPES  g_ycalc_types [YCALC_MAX_TYPE] = {
   /*---type------------ -terse-------- -pre -rpn calc -dep -res ---description--------------------------------------- */
   {  YCALC_DATA_BLANK  , "blank"      , ' ', '-', '-', '-', '-', "blank cell"                                         },
   {  YCALC_DATA_STR    , "string"     , ' ', '-', '-', '-', '#', "string literal presented from source field"         },
   {  YCALC_DATA_SFORM  , "str-form"   , '#', 'y', 'y', 'y', '#', "string formula"                                     },
   {  YCALC_DATA_SLIKE  , "str-like"   , '~', 'y', 'y', 'y', '#', "string formula derived from another cell"           },
   {  YCALC_DATA_NUM    , "number"     , ' ', '-', '-', '-', '=', "numeric literal presented in various formats"       },
   {  YCALC_DATA_NFORM  , "num-form"   , '=', 'y', 'y', 'y', '=', "numeric formula"                                    },
   {  YCALC_DATA_NLIKE  , "num-like"   , '~', 'y', 'y', 'y', '=', "numeric formula derived from another cell"          },
   {  YCALC_DATA_ADDR   , "address"    , '&', 'y', '-', 'y', '-', "address pointer to use in other formulas"           },
   {  YCALC_DATA_RANGE  , "range"      , '&', 'y', '-', 'y', '-', "range pointer to use in other formulas"             },
   {  YCALC_DATA_INTERN , "internal"   , '®', '-', '-', '-', '-', "an actual internal range to use in other formulas"  },
   {  YCALC_DATA_MERGED , "merged"     , '<', '-', '-', 'y', '-', "empty cell used to present merged information"      },
   {  YCALC_DATA_ERROR  , "error"      , ' ', 'y', 'y', 'y', 'e', "error status"                                       },
   {  YCALC_DATA_GARBAGE, "garbage"    , ' ', '-', '-', '-', '-', "marked for garbage collection"                      },
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
   { YCALC_ERROR_BUILD_CIR , 'b' , "#b/cir"   , "dependence would create a circular loop"            },
   { YCALC_ERROR_BUILD_PNT , 'b' , "#b/pnt"   , "pointer dest label/ref is not addr/range type"      },
   { YCALC_ERROR_BUILD_RNG , 'b' , "#b/rng"   , "can not create range with given coords"             },
   { YCALC_ERROR_BUILD_TOK , 'b' , "#b/tok"   , "rpn token could not be recognized"                  },
   { YCALC_ERROR_STACK     , 'e' , "#e/stk"   , "execution stack under or over run"                  },
   { YCALC_ERROR_EXEC_VAL  , 'e' , "#e/val"   , "expected a string, but given a value"               },
   { YCALC_ERROR_EXEC_STR  , 'e' , "#e/str"   , "expected a value, but given a string"               },
   { YCALC_ERROR_EXEC_REF  , 'e' , "#e/ref"   , "calculated dependence is to illegal location"       },
   { YCALC_ERROR_EXEC_CIR  , 'e' , "#e/cir"   , "calculated dependence creates a cirlular loop"      },
   { YCALC_ERROR_EXEC_FMT  , 'e' , "#e/fmt"   , "input data format could not be interpreted"         },
   { YCALC_ERROR_EXEC_DATE , 'e' , "#e/dat"   , "date/time value not in a legal range"               },
   { YCALC_ERROR_EXEC_BRNG , 'e' , "#e/beg"   , "beginning of range not legal"                       },
   { YCALC_ERROR_EXEC_ERNG , 'e' , "#e/end"   , "end of range not legal"                             },
   { YCALC_ERROR_EXEC_MISS , 'e' , "#e/mis"   , "lookup or index function can not find result"       },
   { YCALC_ERROR_EXEC_PTR  , 'e' , "#e/ptr"   , "pointer dereferencing not valid"                    },
   { YCALC_ERROR_EXEC_PTRR , 'e' , "#e/ptR"   , "pointer dereferencing not valid on endoint"         },
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
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(object types)-------------------*/
   DEBUG_PROG   yLOG_note    ("clear validation types");
   strlcpy (YCALC_GROUP_ALL  , "", LEN_LABEL);
   strlcpy (YCALC_GROUP_RPN  , "", LEN_LABEL);
   strlcpy (YCALC_GROUP_CALC , "", LEN_LABEL);
   strlcpy (YCALC_GROUP_DEPS , "", LEN_LABEL);
   strlcpy (YCALC_GROUP_NUM  , "", LEN_LABEL);
   strlcpy (YCALC_GROUP_STR  , "", LEN_LABEL);
   strlcpy (YCALC_GROUP_ERR  , "", LEN_LABEL);
   strlcpy (YCALC_GROUP_POINT, "", LEN_LABEL);
   strlcpy (YCALC_GROUP_FPRE , "", LEN_LABEL);
   /*---(complete info table)------------*/
   DEBUG_PROG   yLOG_note    ("build cell validation types");
   --rce;
   for (i = 0; i < YCALC_MAX_TYPE; ++i) {
      DEBUG_PROG_M yLOG_value   ("ENTRY"     , i);
      DEBUG_PROG_M yLOG_char    ("type"      , g_ycalc_types [i].type);
      /*---(check for end)---------------*/
      if (g_ycalc_types [i].type == 0)  break;
      /*---(add to lists)----------------*/
      sprintf (t, "%c", g_ycalc_types [i].type);
      DEBUG_PROG_M yLOG_info    ("str type"  , t);
      DEBUG_PROG_M yLOG_char    ("rpn flag"  , g_ycalc_types [i].rpn);
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
   DEBUG_PROG   yLOG_value   ("c"         , c);
   DEBUG_PROG   yLOG_info    ("GROUP_ALL" , YCALC_GROUP_ALL  );
   DEBUG_PROG   yLOG_info    ("GROUP_RPN" , YCALC_GROUP_RPN  );
   DEBUG_PROG   yLOG_info    ("GROUP_CALC", YCALC_GROUP_CALC );
   DEBUG_PROG   yLOG_info    ("GROUP_DEPS", YCALC_GROUP_DEPS );
   DEBUG_PROG   yLOG_info    ("GROUP_NUM" , YCALC_GROUP_NUM  );
   DEBUG_PROG   yLOG_info    ("GROUP_STR" , YCALC_GROUP_STR  );
   DEBUG_PROG   yLOG_info    ("GROUP_POIN", YCALC_GROUP_POINT);
   DEBUG_PROG   yLOG_info    ("GROUP_ERR" , YCALC_GROUP_ERR  );
   DEBUG_PROG   yLOG_info    ("GROUP_FPRE", YCALC_GROUP_FPRE );
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
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
   *a_type   = YCALC_DATA_ERROR;
   *a_value  = 0.0;
   if (*a_string != NULL) {
      free (*a_string);
      *a_string = NULL;
   }
   /*---(find entry)---------------------*/
   for (i = 0; i < YCALC_MAX_ERROR; ++i) {
      if (zCALC_errors [i].abbr == 0      )   break;
      if (zCALC_errors [i].abbr != a_error)   continue;
      n = i;
      break;
   }
   /*---(main message)-------------------*/
   if (n < 0)    strlcpy (t, "#mystry", LEN_LABEL);
   else          strlcpy (t, zCALC_errors [i].terse, LEN_LABEL);
   /*---(note/token)---------------------*/
   if (strlen (a_note) > 0)  sprintf (s, "%-7s (%s)", t, a_note);
   else                      strlcpy (s, t, LEN_RECD);
   /*---(save)---------------------------*/
   *a_string = strdup (s);
   DEBUG_CALC   yLOG_info    ("ERROR"     , s);
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
   DEBUG_DEPS   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CALC   yLOG_spoint  (a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_CALC   yLOG_sexit   (__FUNCTION__);
      return 0;
   }
   /*---(remove all)---------------------*/
   x_next = a_deproot->pros;
   while (x_next != NULL) {
      DEBUG_DEPS   yLOG_schar   (x_next->type);
      if (x_next->type == G_DEP_MERGED) ++x_cnt;
      x_next = x_next->next;
   }
   DEBUG_DEPS   yLOG_sint    (x_cnt);
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
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
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (a_deproot == NULL) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(find head)----------------------*/
   *a_origin = NULL;
   x_next = a_deproot->pros;
   while (x_next != NULL) {
      strlcpy (x_label, ycalc_call_labeler (x_next->target), LEN_LABEL);
      DEBUG_DEPS   yLOG_complex ("look at"   , "%-10.10p, %c, %s", x_next, x_next->type, x_label);
      if (G_DEP_BLEED == x_next->type) {
         DEBUG_DEPS   yLOG_note    ("FOUND");
         *a_origin = x_next->target;
         break;
      }
      x_next = x_next->next;
   }
   /*---(check none)---------------------*/
   --rce;  if (*a_origin == NULL) {
      DEBUG_DEPS   yLOG_note    ("no leftward merge providers found");
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
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
   /*---(header)-------------------------*/
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   DEBUG_DEPS   yLOG_value   ("a_start"   , a_start);
   /*---(state check)--------------------*/
   DEBUG_DEPS   yLOG_char    ("state"     , myCALC.state);
   if (myCALC.state == 'L')  {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return 0;
   }
   /*---(defense)------------------------*/
   DEBUG_DEPS   yLOG_point   ("a_deproot" , a_deproot);
   --rce; if (a_deproot == NULL) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_point   ("*a_deproot", *a_deproot);
   --rce; if (*a_deproot == NULL) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_info    ("label"     , ycalc_call_labeler (*a_deproot));
   /*---(remove all)---------------------*/
   x_next = (*a_deproot)->reqs;
   DEBUG_DEPS   yLOG_point   ("x_next"    , x_next);
   while (x_next != NULL) {
      DEBUG_DEPS   yLOG_char    ("type"      , x_next->type);
      x_save = x_next;
      if (x_next->type == G_DEP_MERGED) {
         x_owner   = x_next->target->owner;
         x_deproot = x_next->target;
         rc = g_addresser (x_owner, NULL, &x, NULL, NULL);
         DEBUG_DEPS   yLOG_value   ("x"         , x);
         if (x >= a_start) {
            rc = g_pointer (x_owner, &x_source, &x_type, NULL, NULL);
            DEBUG_DEPS   yLOG_complex ("pointers"  , "%-10.10p, %-10.10p, %c", *x_source, x_type, *x_type);
            if (*x_source != NULL && strcmp (*x_source, "<") == NULL) {
               if (x == a_start)  x_interior = 1;
               *x_type = YCALC_DATA_GARBAGE;
               if (*x_source != NULL)  {
                  free (*x_source);
                  *x_source = NULL;
               }
            }
            rc = ycalc_deps_delete  (x_next->type, a_deproot, &(x_next->target), &(x_next->target->owner));
            DEBUG_DEPS   yLOG_value   ("delete"    , rc);
         }
      }
      x_next = x_save->next;
   }
   DEBUG_DEPS   yLOG_value   ("x_interior", x_interior);
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
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
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_DEPS   yLOG_point   ("a_source"  , a_source);
   --rce;  if (a_source == NULL)  {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_point   ("a_current" , a_current);
   --rce;  if (a_current == NULL)  {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(locate source)------------------*/
   rc = g_addresser (a_source->owner, &b, &x, &y, &z);
   DEBUG_DEPS   yLOG_value   ("addresser" , rc);
   if (rc < 0)  {
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   DEBUG_DEPS   yLOG_complex ("source"    , "%-10p, %3db, %3dx, %3dy, %3dz", a_source, b, x, y, z);
   /*---(locate current)-----------------*/
   DEBUG_DEPS   yLOG_point   ("->owner"   , a_current->owner);
   rc = g_addresser (a_current->owner, NULL, &x_off, NULL, NULL);
   /*> rc = g_addresser (a_current->owner, &u, &x_off, &v, &w);                       <*/
   DEBUG_DEPS   yLOG_value   ("addresser" , rc);
   if (rc < 0)  {
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   DEBUG_DEPS   yLOG_complex ("current"   , "%-10p, %3db, %3dx, %3dy, %3dz", a_current, b, x_off, y, z);
   /*---(look for merges)----------------*/
   for (i = x_off + 1; i < x + 20; ++i) {
      DEBUG_DEPS   yLOG_value   ("i"         , i);
      /*---(check location)----*/
      rc = ycalc_call_who_at (b, i, y, z, YCALC_LOOK, &x_owner, &x_next);
      DEBUG_DEPS   yLOG_complex ("look"      , "%4drc, %-10.10p, %-10.10p", rc, x_owner, x_next);
      if (rc < 0)                              break;
      if (x_owner == NULL)                     break;
      /*---(get pointers)------*/
      rc = g_pointer (x_owner, &x_source, &x_type, NULL, NULL);
      DEBUG_DEPS   yLOG_complex ("pointer"   , "%4drc, %-10.10p, %-10.10p", rc, x_source, x_type);
      if (rc < 0) {
         DEBUG_CALC   yLOG_exit    (__FUNCTION__);
         return 0;
      }
      DEBUG_DEPS   yLOG_complex ("owner"     , "%-10p, %3db, %3dx, %3dy, %3dz, %c", x_owner, b, i, y, z, *x_type);
      /*---(filter)------------*/
      if (*x_source == NULL)                break;
      if (strcmp (*x_source, "<") != 0)     break;
      /*---(force)-------------*/
      rc = ycalc_call_who_at (b, i, y, z, YCALC_FULL, &x_owner, &x_next);
      DEBUG_DEPS   yLOG_complex ("force"     , "%4drc, %-10.10p, %-10.10p", rc, x_owner, x_next);
      rc = ycalc_deps_create (G_DEP_MERGED, &a_source, &x_next);
      DEBUG_DEPS   yLOG_value   ("create"    , rc);
      if (rc < 0) {
         DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
         return 0;
      }
      /*---(set as merged)-----*/
      *x_type = YCALC_DATA_MERGED;
      /*---(free up right)-----*/
      rc = ycalc__unmerge_right (&x_next, 0);
      /*---(done)--------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
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
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   /*---(state check)--------------------*/
   DEBUG_DEPS   yLOG_char    ("state"     , myCALC.state);
   if (myCALC.state == 'L')  {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return 0;
   }
   /*---(defense)------------------------*/
   DEBUG_DEPS   yLOG_point   ("a_deproot" , a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_point   ("*a_deproot", *a_deproot);
   --rce;  if (*a_deproot == NULL) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, -rce);
      return -rce;
   }
   /*---(check if merge root)------------*/
   if (*a_type != YCALC_DATA_MERGED) {
      DEBUG_DEPS   yLOG_note    ("process merge origin delete");
      rc = ycalc__unmerge_right (a_deproot, 0);
   }
   /*---(check merged cells)-------------*/
   else {
      DEBUG_DEPS   yLOG_note    ("process merge interior delete");
      /*---(get column)-----------*/
      rc = g_addresser ((*a_deproot)->owner, NULL, &x, NULL, NULL);
      if (rc < 0) {
         DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, -rce);
         return rce;
      }
      /*---(find source)----------*/
      rc = ycalc__merge_leftmost (*a_deproot, &x_origin);
      if (rc < 0) {
         DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, -rce);
         return rce;
      }
      /*---(unmerge right)--------*/
      rc = ycalc__unmerge_right (&x_origin, x);
      if (rc < 0) {
         DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, -rce);
         return rce;
      }
      /*---(update)---------------*/
      s_origin   = x_origin->owner;
      if (rc > 0)  *a_deproot = NULL;
      /*---(done)-----------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
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
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_DEPS   yLOG_point   ("a_source"  , a_source);
   --rce;  if (a_source == NULL)  {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check source)-------------------*/
   rc = g_pointer (a_source->owner, NULL, &x_type, NULL, NULL);
   DEBUG_DEPS   yLOG_char    ("x_type"    , *x_type);
   if (*x_type == YCALC_DATA_BLANK || *x_type == YCALC_DATA_GARBAGE)  {
      DEBUG_DEPS   yLOG_note    ("never merge on blanks/garbage");
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(locate source)------------------*/
   rc = g_addresser (a_source->owner, &b, &x, &y, &z);
   DEBUG_DEPS   yLOG_value   ("addresser" , rc);
   if (rc < 0)  {
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   rc = g_pointer (a_source->owner, NULL, &x_type, NULL, NULL);
   DEBUG_DEPS   yLOG_complex ("source"    , "%-10p, %c, %3db, %3dx, %3dy, %3dz", a_source, *x_type, b, x, y, z);
   if (*x_type == YCALC_DATA_BLANK || *x_type == YCALC_DATA_GARBAGE) {
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(look for merges)----------------*/
   for (i = x + 1; i < x + 20; ++i) {
      DEBUG_DEPS   yLOG_value   ("i"         , i);
      /*---(retrieve)-----------------------*/
      rc = ycalc_call_who_at (b, i, y, z, YCALC_LOOK, &x_owner, &x_next);
      if (rc < 0)                              break;
      if (x_owner == NULL)                     break;
      /*---(fill pointers)------------------*/
      rc = g_pointer (x_owner, &x_source, &x_type, NULL, NULL);
      DEBUG_DEPS   yLOG_value   ("pointer"    , rc);
      if (rc < 0) {
         DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
         return 0;
      }
      DEBUG_DEPS   yLOG_complex ("owner"     , "%-10p, %3db, %3dx, %3dy, %3dz, %c", x_owner, b, i, y, z, *x_type);
      if (*x_source == NULL)                break;
      if (strcmp (*x_source, "<") != 0)     break;
      rc = ycalc_call_who_at (b, i, y, z, YCALC_FULL, &x_owner, &x_next);
      rc = ycalc_deps_create (G_DEP_MERGED, &a_source, &x_next);
      DEBUG_DEPS   yLOG_value   ("create"    , rc);
      if (rc < 0) {
         DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
         return 0;
      }
      *x_type = YCALC_DATA_MERGED;
      rc = ycalc__unmerge_right (&x_next, 0);
   }
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
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
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   /*---(look left)----------------------*/
   rc = g_addresser ((*a_deproot)->owner, &b, &x, &y, &z);
   DEBUG_DEPS   yLOG_value   ("addresser" , rc);
   --rce;  if (rc < 0) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_complex ("loc"       , "b=%-4d, x=%-4d, y=%-4d, z=%-4d", b, x, y, z);
   --rce;  if (x  < 1) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get owner to left)--------------*/
   DEBUG_DEPS   yLOG_note    ("LOOK LEFT FOR MERGE OWNER");
   rc = ycalc_call_who_at (b, x - 1, y, z, YCALC_LOOK, &x_owner, &x_left);
   DEBUG_DEPS   yLOG_value   ("who_at"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_point   ("x_owner"   , x_owner);
   --rce;  if (x_owner == NULL) {
      DEBUG_DEPS   yLOG_note    ("nothing to the left to merge with");
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*> rc = g_pointer (&((*a_deproot)->owner), NULL, &x_type, NULL, NULL);            <*/
   rc = g_pointer (x_owner, &x_source, &x_type, NULL, NULL);
   DEBUG_DEPS   yLOG_value   ("pointers"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_char    ("*x_type"   , *x_type);
   DEBUG_DEPS   yLOG_info    ("*x_source" , *x_source);
   /*---(handle merge append)------------*/
   if (*x_type == YCALC_DATA_MERGED) {
      DEBUG_DEPS   yLOG_note    ("handle appending to existing merge");
      DEBUG_DEPS   yLOG_point   ("x_left"    , x_left);
      --rce;  if (x_left == NULL) {
         DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      rc = ycalc__merge_leftmost (x_left, &x_origin);
      DEBUG_DEPS   yLOG_value   ("leftmost"  , rc);
      --rce;  if (rc < 0 || x_origin == NULL) {
         DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*> DEBUG_DEPS   yLOG_info    ("x_origin"  , x_origin->owner->label);           <*/
      rc = ycalc_deps_create (G_DEP_MERGED, &x_origin, a_deproot);
      DEBUG_DEPS   yLOG_value   ("create"    , rc);
      --rce;  if (rc < 0) {
         DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      x_left = x_origin;
   }
   /*---(handle unmergable)--------------*/
   else if (*x_type == YCALC_DATA_BLANK || *x_type == YCALC_DATA_GARBAGE) {
      DEBUG_DEPS   yLOG_note    ("nothing to merge");
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(not to merge)-------------------*/
   else if (*x_source != NULL && strcmp (*x_source, "<") == 0) {
      DEBUG_DEPS   yLOG_note    ("unmerged merge indicator to left");
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(handle new merge)---------------*/
   else {
      DEBUG_DEPS   yLOG_note    ("handle new merge");
      DEBUG_DEPS   yLOG_point   ("x_left"    , x_left);
      if (x_left == NULL) {
         rc = ycalc_call_who_at (b, x - 1, y, z, YCALC_FULL, &x_owner, &x_left);
      }
      rc = ycalc_deps_create (G_DEP_MERGED, &x_left  , a_deproot);
      DEBUG_DEPS   yLOG_value   ("create"    , rc);
      --rce;  if (rc < 0) {
         DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(look to the right)--------------*/
   DEBUG_DEPS   yLOG_point   ("x_left"    , x_left);
   rc = ycalc__merge_right (x_left, *a_deproot);
   --rce;  if (rc < 0) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
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
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(check pointers)-----------------*/
   DEBUG_CALC   yLOG_point   ("a_source"   , a_source);
   --rce;  if (a_source == NULL) {
      DEBUG_CALC   yLOG_note    ("source pointer not given, no input to start");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_point   ("*a_source"  , *a_source);
   DEBUG_CALC   yLOG_point   ("a_type"     , a_type);
   --rce;  if (a_type == NULL) {
      DEBUG_CALC   yLOG_note    ("type pointer not given, can not classify");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_char    ("*a_type"    , *a_type);
   DEBUG_CALC   yLOG_point   ("a_value"    , a_value);
   --rce;  if (a_value == NULL) {
      DEBUG_CALC   yLOG_note    ("value pointer not given, no numeric output possible");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_double  ("*a_value"   , *a_value);
   DEBUG_CALC   yLOG_point   ("a_string"   , a_string);
   --rce;  if (a_string == NULL) {
      DEBUG_CALC   yLOG_note    ("string pointer not given, no string/error output possible");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_point   ("*a_string"  , *a_string);
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc_classify_clear    (void **a_owner, tDEP_ROOT **a_deproot, char *a_type, double *a_value, char **a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;           /* return code for errors         */
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(clear calculation)-----------*/
   DEBUG_CALC   yLOG_note    ("wipe existing calculations/rpn");
   if (a_deproot != NULL)  rc = ycalc_calc_wipe  (*a_deproot);
   DEBUG_CALC   yLOG_value   ("calc"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(clear merges/complex)--------*/
   DEBUG_CALC   yLOG_note    ("wipe merge dependencies");
   s_origin = NULL;
   if (a_deproot != NULL)  rc = ycalc_unmerge (a_deproot, a_type);
   DEBUG_CALC   yLOG_value   ("unmerge"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_point   ("*a_deproot", *a_deproot);
   --rce;  if (*a_deproot == NULL) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(clear dependencies)----------*/
   DEBUG_CALC   yLOG_note    ("wipe removable dependencies");
   if (a_deproot != NULL)  rc = ycalc_deps_wipe_reqs (a_owner, a_deproot);
   DEBUG_CALC   yLOG_value   ("cleanser"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(clear type)------------------*/
   *a_type = YCALC_DATA_BLANK;
   DEBUG_CALC   yLOG_char    ("*a_type"   , *a_type);
   if (a_deproot != NULL && *a_deproot != NULL) {
      (*a_deproot)->btype = *a_type;
      DEBUG_CALC   yLOG_char    ("btype"     , (*a_deproot)->btype);
   }
   /*---(string)----------------------*/
   DEBUG_CALC   yLOG_point   ("a_string"  , a_string);
   if (a_string != NULL) {
      DEBUG_CALC   yLOG_point   ("*a_string" , *a_string);
      if (*a_string != NULL) {
         DEBUG_CALC   yLOG_info    ("*a_string" , *a_string);
         free (*a_string);
      }
      *a_string = NULL;
      DEBUG_CALC   yLOG_point   ("*a_string" , *a_string);
   }
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
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
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(blanks)-------------------------*/
   DEBUG_CALC   yLOG_point   ("*a_source"    , *a_source);
   if (*a_source == NULL || *a_source [0] == 0) {
      *a_type   = YCALC_DATA_BLANK;
      DEBUG_CALC   yLOG_note    ("null/empty object");
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   DEBUG_CALC   yLOG_info    ("*a_source"     , *a_source);
   /*---(merges)-------------------------*/
   x_len = strllen (*a_source, LEN_RECD);
   DEBUG_CALC   yLOG_value   ("x_len"     , x_len);
   if (x_len == 1 && *a_source [0] == '<') {
      DEBUG_CALC   yLOG_note    ("merged object");
      *a_type   = YCALC_DATA_MERGED;
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(formulas)-----------------------*/
   DEBUG_CALC   yLOG_info    ("valid"     , YCALC_GROUP_FPRE);
   --rce;  if (strchr (YCALC_GROUP_FPRE, *a_source [0]) != NULL) {
      DEBUG_CALC   yLOG_note    ("formula prefix found");
      /*---(handle types)-------------------*/
      if      (*a_source [0] == '&') {
         if (strstr (*a_source, "..") == NULL)  *a_type = YCALC_DATA_ADDR;
         else                                   *a_type = YCALC_DATA_RANGE;
      }
      else if (*a_source [0] == '®')  *a_type = YCALC_DATA_INTERN;
      else if (*a_source [0] == '=')  *a_type = YCALC_DATA_NFORM;
      else if (*a_source [0] == '#')  *a_type = YCALC_DATA_SFORM;
      else if (*a_source [0] == '~')  *a_type = YCALC_DATA_NLIKE;
      /*--> can not distinquish num-like vs str-like until build  */
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(numbers)------------------------*/
   rc = strl2num (*a_source, &x_value, LEN_RECD);
   if (rc >= 0) {
      DEBUG_CALC   yLOG_note    ("numeric literal");
      *a_type   = YCALC_DATA_NUM;
      *a_value  = x_value;
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(default string)-----------------*/
   DEBUG_CALC   yLOG_note    ("default string literal");
   *a_type   = YCALC_DATA_STR;
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
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
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CALC   yLOG_point   ("*a_source" , *a_source);
   if (*a_source != NULL && *a_source [0] == YCALC_DATA_INTERN) x_intern = 'y';
   /*---(clear)--------------------------*/
   if (x_intern != 'y') {
      rc = ycalc_classify_clear (a_owner, a_deproot, a_type, a_value, a_string);
      DEBUG_CALC   yLOG_value   ("clear"     , rc);
      --rce;  if (rc < 0) {
         DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   } else {
      DEBUG_CALC   yLOG_note    ("do not clear internal summary owners");
   }
   /*---(defense)------------------------*/
   DEBUG_CALC   yLOG_point   ("*a_owner"  , *a_owner);
   --rce;  if (*a_owner == NULL) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_point   ("*a_deproot", *a_deproot);
   /*---(reset all values)---------------*/
   if (x_intern != 'y') {
      DEBUG_CALC   yLOG_note    ("initialize to blank data item");
      *a_type   = YCALC_DATA_BLANK;
      *a_value  = 0.0;
   } else {
      DEBUG_CALC   yLOG_note    ("do not initialize internal summary owners");
   }
   /*---(prepare)------------------------*/
   rc = ycalc__classify_content   (a_source, a_type, a_value);
   DEBUG_CALC   yLOG_value   ("content"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(classified)---------------------*/
   DEBUG_CALC   yLOG_char    ("*a_type"   , *a_type);
   if (a_deproot != NULL && *a_deproot != NULL) {
      (*a_deproot)->btype = *a_type;
      DEBUG_CALC   yLOG_char    ("btype"     , (*a_deproot)->btype);
   }
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc_classify_detail   (void **a_owner, tDEP_ROOT **a_deproot, char **a_source, char *a_type, double *a_value, char **a_string)
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
   /*---(prepare)------------------------*/
   rc = ycalc_shared_verify (a_source, a_type, a_value, a_string);
   DEBUG_CALC   yLOG_value   ("verify"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ycalc_classify_trusted (a_owner, a_deproot, a_source, a_type, a_value, a_string);
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
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (ycalc_not_ready ()) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(defense)------------------------*/
   DEBUG_CALC   yLOG_point   ("a_owner"   , a_owner);
   --rce;  if (a_owner == NULL) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_point   ("*a_owner"  , *a_owner);
   --rce;  if (a_owner == NULL) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_point   ("a_deproot" , a_deproot);
   --rce;  if (a_deproot == NULL) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_point   ("*a_deproot", *a_deproot);
   /*---(fill pointers)------------------*/
   rc = g_pointer (*a_owner, &x_source, &x_type, &x_value, &x_string);
   DEBUG_CALC   yLOG_value   ("pointer"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ycalc_classify_detail     (a_owner, a_deproot, x_source, x_type, x_value, x_string);
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
ycalc_classify_label    (char *a_label)
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
   rc = ycalc_classify_owner (&x_owner, &x_deproot);
   DEBUG_CALC   yLOG_value   ("owner"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
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
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (ycalc_not_ready ())  {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get label)----------------------*/
   rc = ycalc_call_who_named  (a_label, YCALC_FULL, &x_owner, &x_deproot);
   DEBUG_CALC   yLOG_value   ("who_named"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_point   ("x_owner"    , x_owner);
   DEBUG_CALC   yLOG_point   ("x_deproot"  , x_deproot);
   /*---(fill pointers)------------------*/
   rc = g_pointer (x_owner, &x_source, &x_type, &x_value, &x_string);
   DEBUG_CALC   yLOG_value   ("pointer"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ycalc_shared_verify (x_source, x_type, x_value, x_string);
   DEBUG_CALC   yLOG_value   ("prepare"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(classify)-----------------------*/
   rc = ycalc_classify_trusted (&x_owner, &x_deproot, x_source, x_type, x_value, x_string);
   DEBUG_CALC   yLOG_value   ("classify"  , rc);
   DEBUG_CALC   yLOG_char    ("*x_type"   , *x_type);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(merges)-------------------------*/
   if (*x_type == YCALC_DATA_MERGED) {
      rc = ycalc_merge (&x_deproot);
      if (rc < 0) {
         *x_type = YCALC_DATA_STR;
      }
   } else {
      rc = ycalc_merge_check    (x_deproot);
   }
   DEBUG_CALC   yLOG_value   ("merging"   , rc);
   /*---(ranges)-------------------------*/
   rc = g_addresser (x_owner, &b, &x, &y, &z);
   DEBUG_CALC   yLOG_value   ("addresser" , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (*x_type != YCALC_DATA_BLANK) {
      rc = ycalc_range_include (&x_deproot, b, x, y, z);
      DEBUG_CALC   yLOG_value   ("ranges"    , rc);
      --rce;  if (rc < 0) {
         DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(calculation)--------------------*/
   DEBUG_CALC   yLOG_note    ("now consider build and execution");
   DEBUG_CALC   yLOG_char    ("*x_type"   , *x_type);
   DEBUG_CALC   yLOG_info    ("valid"     , YCALC_GROUP_RPN);
   /*---(build)--------------------------*/
   if (strchr (YCALC_GROUP_RPN , *x_type) != NULL) {
      rc = ycalc_build_trusted    (x_deproot, x_source, x_type, x_value, x_string);
      DEBUG_CALC   yLOG_value   ("build"     , rc);
   }
   /*---(execute)------------------------*/
   if (*x_type != YCALC_DATA_ERROR && strchr (YCALC_GROUP_CALC, *x_type) != NULL) {
      rc = ycalc_execute_trusted  (x_deproot, x_type, x_value, x_string);
      DEBUG_CALC   yLOG_value   ("execute"   , rc);
   }
   /*---(calc upward)--------------------*/
   if (x_deproot != NULL) {
      rc = yCALC_seq_up (x_deproot, ycalc_execute_auto);
      DEBUG_CALC   yLOG_value   ("seq"       , rc);
      --rce;  if (rc < 0)  {
         DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*> ycalc__mock_list ();                                                           <*/
   /*---(call reaper)--------------------*/
   DEBUG_CALC   yLOG_note    ("then, cleanup as necessary");
   rc = ycalc_call_reaper (&x_owner, &x_deproot);
   DEBUG_CALC   yLOG_value   ("reaper"    , rc);
   DEBUG_CALC   yLOG_point   ("x_owner"   , x_owner);
   /*> ycalc__mock_list ();                                                           <*/
   /*---(call printer)-------------------*/
   if (x_owner != NULL) {
      if (*x_type == YCALC_DATA_MERGED) {
         DEBUG_CALC   yLOG_note    ("merged, find source");
         DEBUG_CALC   yLOG_point   ("x_deproot" , x_deproot);
         rc = ycalc__merge_leftmost (x_deproot, &x_origin);
         x_owner = x_origin->owner;
      }
      rc = g_printer (x_owner);
      DEBUG_CALC   yLOG_value   ("printer"   , rc);
      --rce;  if (rc < 0) {
         DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*> ycalc__mock_list ();                                                           <*/
   DEBUG_CALC   yLOG_point   ("s_origin"  , s_origin);
   if (s_origin != NULL) {
      rc = g_printer (s_origin);
   }
   /*---(ripple to likes)----------------*/
   if (x_deproot != NULL) {
      rc = ycalc_build_ripple (x_deproot);
      --rce;  if (rc < 0) {
         DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*> ycalc__mock_list ();                                                           <*/
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
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
   DEBUG_SORT    yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_SORT    yLOG_point   ("a_list"    , a_list);
   --rce;  if (a_list == NULL) {
      DEBUG_SORT    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SORT    yLOG_info    ("a_list"    , a_list);
   /*---(initialize)---------------------*/
   DEBUG_SORT    yLOG_note    ("initialize");
   for (i = 0; i < 1000; ++i)  s_array [i] = 0;
   s_narray = 0;
   /*---(parse/load)---------------------*/
   DEBUG_SORT    yLOG_note    ("load the array");
   strlcpy (x_list, a_list,  LEN_RECD);
   strlcpy (a_list, "#PREP", LEN_RECD);
   p = strtok_r (x_list, q, &r);
   s_narray = 0;
   --rce;  while (p != NULL) {
      DEBUG_SORT    yLOG_info    ("parse"     , p);
      rc = g_who_named  (p, YCALC_LOOK, &x_owner, NULL);
      DEBUG_SORT    yLOG_value   ("who_named" , rc);
      if (rc < 0)  {
         DEBUG_SORT    yLOG_note    ("could not find owner, EXITING");
         DEBUG_SORT    yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      rc = g_addresser (x_owner, &b, &x, &y, &z);
      DEBUG_SORT    yLOG_value   ("addresser" , rc);
      if (rc < 0)  {
         DEBUG_SORT    yLOG_note    ("could not parse, EXITING");
         DEBUG_SORT    yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_SORT    yLOG_complex ("parts"     , "b=%04d, x=%04d, y=%04d, z=%04d", b, x, y, z);
      ++b;
      ++x;
      ++y;
      ++z;
      DEBUG_SORT    yLOG_complex ("inserted"  , "b=%04d, x=%04d, y=%04d, z=%04d", b, x, y, z);
      s_array [s_narray] = (b * s_bf) + (x * s_xf) + (y * s_yf) + z;
      DEBUG_SORT    yLOG_pair    (s_narray         , s_array [s_narray]);
      p = strtok_r (NULL  , q, &r);
      ++s_narray;
   }
   DEBUG_SORT    yLOG_exit    (__FUNCTION__);
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
   DEBUG_SORT    yLOG_enter   (__FUNCTION__);
   /*---(sort)---------------------------*/
   i = 1;
   while (i < s_narray) {
      /*---(load vars)-------------------*/
      a = s_array [i - 1];
      b = s_array [i];
      DEBUG_SORT    yLOG_complex ("current"   , "compare i,%3d (t,%3d) =  a,%10d to b,%10d", i, tele, a, b);
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
      DEBUG_SORT    yLOG_complex ("swapped"   , "now     i,%3d (t,%3d) =  a,%10d to b,%10d", i, tele, a, b);
      /*---(update)----------------------*/
      if (tele < 0) tele = i;
      if (i > 1) --i;
   }
   DEBUG_SORT    yLOG_exit    (__FUNCTION__);
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
   DEBUG_SORT    yLOG_enter   (__FUNCTION__);
   strlcpy (a_list, ",", LEN_RECD);
   for (i = 0; i < s_narray; ++i) {
      DEBUG_SORT    yLOG_value   ("value"   , s_array[i]);
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
      DEBUG_SORT    yLOG_complex ("parts"     , "b=%04d, x=%04d, y=%04d, z=%04d", b, x, y, z);
      rc = ycalc_call_who_at (b, x, y, z, YCALC_LOOK, &x_owner, &x_deproot);
      if      (x_owner   == NULL)   sprintf (x_label, "[#%d]", i);
      else if (rc        <  0   )   sprintf (x_label, "<#%d>", i);
      else if (x_deproot != NULL)   strlcpy (x_label, ycalc_call_labeler (x_deproot), LEN_LABEL);
      else                          sprintf (x_label, "{#%d}", i);
      DEBUG_SORT    yLOG_info    ("label"   , x_label);
      strlcat (a_list, x_label, LEN_RECD);
      strlcat (a_list, ","    , LEN_RECD);
   }
   DEBUG_SORT    yLOG_info    ("final"     , a_list);
   /*---(complete)-----------------------*/
   DEBUG_SORT    yLOG_exit    (__FUNCTION__);
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
   DEBUG_SORT    yLOG_enter   (__FUNCTION__);
   /*---(defense: null)------------------*/
   --rce;  if (a_list  == NULL)  {
      DEBUG_SORT    yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(sort)---------------------------*/
   if (rc == 0)   rc = ycalc__sort_prep   (a_list);
   if (rc == 0)   rc = ycalc__sort_itself ();
   if (rc == 0)   rc = ycalc__sort_wrap   (a_list);
   /*---(complete)-----------------------*/
   DEBUG_SORT    yLOG_exit    (__FUNCTION__);
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
   DEBUG_SORT    yLOG_char    ("a_start"   , a_start);
   --rce;  switch (a_start) {
   case 'R' :  n = a_range->reqs; break;
   case 'P' :  n = a_range->pros; break;
   default         :  return rce;
   }
   DEBUG_SORT    yLOG_info    ("a_list"    , a_list);
   /*---(walk the list)------------------*/
   while (n != NULL) {
      strlcat (a_list, ycalc_call_labeler (n->target), LEN_RECD);
      strlcat (a_list, ","                           , LEN_RECD);
      n = n->next;
   }
   DEBUG_SORT    yLOG_info    ("a_list"    , a_list);
   /*---(complete)-----------------------*/
   return 0;
}

char         /*-> list dependencies ------------------[ leaf   [fe.740.424.60]*/ /*-[02.0000.034.!]-*/ /*-[--.---.---.--]-*/
ycalc__audit_disp_master   (tDEP_ROOT *a_me, char *a_list, char a_start, char *a_types)
{  /*---(design notes)-------------------*/
   /* combined logic from the three accessors below as they were 90% the same */
   /*---(locals)-----------+-----------+-*/
   tDEP_LINK  *n           = NULL;
   char        rce         = -10;
   uchar       x_label     [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_SORT    yLOG_enter   (__FUNCTION__);
   /*---(defenses)-----------------------*/
   DEBUG_SORT    yLOG_point   ("a_list"    , a_list);
   --rce;  if (a_list  == NULL) {
      DEBUG_SORT    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strlcpy (a_list, "-", LEN_RECD);   /* special for a null list */
   DEBUG_SORT    yLOG_info    ("a_list"    , a_list);
   DEBUG_SORT    yLOG_point   ("a_me"      , a_me);
   --rce;  if (a_me    == NULL) {
      DEBUG_SORT    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(setup)--------------------------*/
   strlcpy (a_list, ",", LEN_RECD);
   DEBUG_SORT    yLOG_char    ("a_start"   , a_start);
   --rce;  switch (a_start) {
   case 'R' :  n = a_me->reqs; break;
   case 'P' :  n = a_me->pros; break;
   default         :  return rce;
   }
   DEBUG_SORT    yLOG_info    ("a_list"    , a_list);
   /*---(walk the list)------------------*/
   DEBUG_SORT    yLOG_info    ("a_types"   , a_types);
   while (n != NULL) {
      if (strchr (a_types, n->type) != 0) {
         strlcpy (x_label, ycalc_call_labeler (n->target), LEN_LABEL);
         if (x_label [0] == (uchar) '®') {
            strlcat (a_list, x_label, LEN_RECD);
            strlcat (a_list, ","    , LEN_RECD);
            ycalc__audit_disp_range (n->target, a_start, a_list);
            strlcat (a_list, ","    , LEN_RECD);
         } else {
            strlcat (a_list, x_label, LEN_RECD);
            strlcat (a_list, ","    , LEN_RECD);
         }
      }
      n = n->next;
   }
   /*---(catch empty)--------------------*/
   if (strcmp (a_list, ",") == 0)   strlcpy (a_list, ".", LEN_RECD);
   else  ycalc__audit_sort (a_list);
   DEBUG_SORT    yLOG_info    ("a_list"    , a_list);
   DEBUG_SORT    yLOG_exit    (__FUNCTION__);
   /*---(complete)-----------------------*/
   return 0;
}

char       yCALC_disp_reqs      (void *a_me, char *a_list) { return ycalc__audit_disp_master ((tDEP_ROOT *) a_me, a_list, 'R', S_DEP_REQS); }
char       yCALC_disp_pros      (void *a_me, char *a_list) { return ycalc__audit_disp_master ((tDEP_ROOT *) a_me, a_list, 'P', S_DEP_PROS); }
char       yCALC_disp_like      (void *a_me, char *a_list) { return ycalc__audit_disp_master ((tDEP_ROOT *) a_me, a_list, 'P', S_DEP_LIKE); }



/*====================------------------------------------====================*/
/*===----                       sheet-functions                        ----===*/
/*====================------------------------------------====================*/
static void  o___FORMULAS________o () { return; }

void         /*-> tbd --------------------------------[ ------ [gv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_type          (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   ycalc_pushval (__FUNCTION__, a);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.210.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_isblank       (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (a == YCALC_DATA_BLANK)   ycalc_pushval (__FUNCTION__, TRUE);
   else                         ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.210.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_isvalue       (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (strchr (YCALC_GROUP_NUM, a) != NULL) ycalc_pushval (__FUNCTION__, TRUE);
   else                                     ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.210.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_istext        (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (strchr (YCALC_GROUP_STR, a) != NULL) ycalc_pushval (__FUNCTION__, TRUE);
   else                                     ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_iscalc        (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (strchr (YCALC_GROUP_CALC, a) != NULL) ycalc_pushval (__FUNCTION__, TRUE);
   else                                      ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.210.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_islit         (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (a == YCALC_DATA_ERROR)                     ycalc_pushval (__FUNCTION__, FALSE);
   else if (strchr (YCALC_GROUP_CALC, a) == NULL) ycalc_pushval (__FUNCTION__, TRUE);
   else                                           ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.220.000.32]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_ispoint       (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if      (a == YCALC_DATA_RANGE) ycalc_pushval (__FUNCTION__, TRUE);
   else if (a == YCALC_DATA_ADDR ) ycalc_pushval (__FUNCTION__, TRUE);
   else                            ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.210.000.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_iserror       (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (strchr (YCALC_GROUP_ERR , a) != NULL) ycalc_pushval (__FUNCTION__, TRUE);
   else                                      ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void         /*-> tbd --------------------------------[ ------ [gv.210.000.02]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_bpos          (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   m = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_BPOS);
   ycalc_pushval (__FUNCTION__, m);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_xpos          (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   m = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_XPOS);
   ycalc_pushval (__FUNCTION__, m);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_ypos          (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   m = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_YPOS);
   ycalc_pushval (__FUNCTION__, m);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_zpos          (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   m = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_ZPOS);
   ycalc_pushval (__FUNCTION__, m);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_me            (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   ycalc_pushref (__FUNCTION__, myCALC.deproot, myCALC.label);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_label         (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_LABEL);
   ycalc_pushstr (__FUNCTION__, r);
   free (r);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_formula       (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_SOURCE);
   ycalc_pushstr (__FUNCTION__, r);
   free (r);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rpn           (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_RPN);
   ycalc_pushstr (__FUNCTION__, r);
   free (r);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_reqs          (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_REQS);
   ycalc_pushstr (__FUNCTION__, r);
   free (r);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_pros          (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_PROS);
   ycalc_pushstr (__FUNCTION__, r);
   free (r);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_ncalc         (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_NCALC);
   ycalc_pushval (__FUNCTION__, a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_nreq          (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_NREQ);
   ycalc_pushval (__FUNCTION__, a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_npro          (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_NPRO);
   ycalc_pushval (__FUNCTION__, a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_level         (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_LEVEL);
   ycalc_pushval (__FUNCTION__, a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_pointer       (void)
{
   DEBUG_CALC   yLOG_info    ("running"   , __FUNCTION__);
   /*---(locals)-----------+-----+-----+-*/
   tDEP_ROOT  *x_ref       = NULL;
   tCALC      *x_calc      = NULL;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(get the reference)--------------*/
   x_ref = ycalc_popref (__FUNCTION__);
   DEBUG_CALC   yLOG_point   ("x_ref"     , x_ref);
   if (x_ref == NULL) {
      ycalc_error_set (YCALC_ERROR_EXEC_PTR, NULL);
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   ycalc_error_set (YCALC_ERROR_NONE, x_ref);
   DEBUG_CALC   yLOG_info    ("myCALC.me"  , myCALC.me);
   DEBUG_CALC   yLOG_value   ("ncalc"     , x_ref->ncalc);
   if (x_ref->ncalc != 1 && x_ref->ncalc != 3) {
      ycalc_error_set (YCALC_ERROR_EXEC_PTR, x_ref);
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   /*---(dereference)--------------------*/
   x_calc = x_ref->chead;
   DEBUG_CALC   yLOG_point   ("x_calc"    , x_calc);
   if (x_calc == NULL) {
      ycalc_error_set (YCALC_ERROR_EXEC_PTR, x_ref);
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   DEBUG_CALC   yLOG_char    ("x_calc->t" , x_calc->t);
   /*---(check for address)--------------*/
   if (x_ref->ncalc == 1) {
      if (x_calc->t != G_TYPE_REF) {
         ycalc_error_set (YCALC_ERROR_EXEC_PTR, x_ref);
         DEBUG_CALC   yLOG_exit    (__FUNCTION__);
         return;
      }
      DEBUG_CALC   yLOG_point   ("x_calc->r"  , x_calc->r);
      if (x_calc->r == NULL) {
         ycalc_error_set (YCALC_ERROR_EXEC_PTR, x_ref);
         DEBUG_CALC   yLOG_exit    (__FUNCTION__);
         return;
      }
   }
   /*---(check for range)----------------*/
   else if (x_ref->ncalc == 3) {
      x_calc = x_calc->next->next;
      DEBUG_CALC   yLOG_point   ("x_calc"    , x_calc);
      if (x_calc == NULL) {
         ycalc_error_set (YCALC_ERROR_EXEC_PTR, x_ref);
         DEBUG_CALC   yLOG_exit    (__FUNCTION__);
         return;
      }
      DEBUG_CALC   yLOG_char    ("x_calc->t" , x_calc->t);
      if (x_calc->t != G_TYPE_REF) {
         ycalc_error_set (YCALC_ERROR_EXEC_PTR, x_ref);
         DEBUG_CALC   yLOG_exit    (__FUNCTION__);
         return;
      }
      DEBUG_CALC   yLOG_point   ("x_calc->r"  , x_calc->r);
      if (x_calc->r == NULL) {
         ycalc_error_set (YCALC_ERROR_EXEC_PTR, x_ref);
         DEBUG_CALC   yLOG_exit    (__FUNCTION__);
         return;
      }
   }
   /*---(push reference back)------------*/
   DEBUG_CALC   yLOG_info    ("x_calc->s"  , x_calc->s);
   ycalc_pushref (__FUNCTION__, x_calc->r, x_calc->s);
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return;
}




