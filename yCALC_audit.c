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
   {  YCALC_DATA_INTERN , "internal"   , '­', 'y', '-', 'y', '-', "an actual internal range to use in other formulas"  },
   {  YCALC_DATA_MERGED , "merged"     , '<', '-', '-', 'y', '-', "empty cell used to present merged information"      },
   {  YCALC_DATA_ERROR  , "error"      , ' ', '-', '-', 'y', 'e', "error status"                                       },
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
   { YCALC_ERROR_BUILD_RPN , 'b' , "#b/rpn"   , "can not convert source to rpn tokens"               },
   { YCALC_ERROR_BUILD_REF , 'b' , "#b/ref"   , "label/ref null, not allowed, or not real"           },
   { YCALC_ERROR_BUILD_DEP , 'b' , "#b/dep"   , "dependence failed on legal label/ref"               },
   { YCALC_ERROR_BUILD_CIR , 'b' , "#b/cir"   , "dependence would create a circular loop"            },
   { YCALC_ERROR_BUILD_PNT , 'b' , "#b/pnt"   , "pointer dest label/ref is not addr/range type"      },
   { YCALC_ERROR_BUILD_TOK , 'b' , "#b/tok"   , "rpn token could not be recognized"                  },
   { G_ERROR_RANGE         , 'b' , "#range"   , ""                                                   },
   { G_ERROR_TOKEN         , 'b' , "#token"   , ""                                                   },
   { G_ERROR_UNKNOWN       , 'b' , "#boom"    , ""                                                   },
   /* ---abbr-------------- stage   --disp-     ---description-------------------------------------  */
   { 0                     ,  0  , ""         , ""                                                   },
};



/*====================------------------------------------====================*/
/*===----                          program level                       ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char
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
static void  o___DRIVER__________o () { return; }

char
ycalc_handle_error      (char a_error, char *a_type, double *a_value, char **a_string, char *a_note)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   int         n           =   -1;
   char        t           [LEN_RECD];
   char        s           [LEN_RECD];
   /*---(assign error status)------------*/
   *a_type   = YCALC_DATA_ERROR;
   *a_value  = 0.0;
   if (*a_string != NULL)   free (*a_string);
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
   /*---(find entry)---------------------*/
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
   /*---(clear dependencies)----------*/
   DEBUG_CALC   yLOG_note    ("wipe removable dependencies");
   if (a_deproot != NULL)  rc = ycalc_deps_wipe_reqs (a_owner, a_deproot);
   DEBUG_CALC   yLOG_value   ("cleanser"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
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
      else if (*a_source [0] == '­')  *a_type = YCALC_DATA_INTERN;
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
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(clear)--------------------------*/
   rc = ycalc_classify_clear (a_owner, a_deproot, a_type, a_value, a_string);
   DEBUG_CALC   yLOG_value   ("clear"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_point   ("*a_owner"  , *a_owner);
   --rce;  if (*a_owner == NULL) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_point   ("*a_deproot", *a_deproot);
   /*---(reset all values)---------------*/
   DEBUG_CALC   yLOG_note    ("initialize to blank data item");
   *a_type   = YCALC_DATA_BLANK;
   *a_value  = 0.0;
   if (*a_string != NULL) {
      free (*a_string);
      *a_string = NULL;
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
   int         x, y, z;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (ycalc_not_ready ())  {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
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
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(ranges)-------------------------*/
   rc = g_addresser (x_owner, &x, &y, &z);
   DEBUG_CALC   yLOG_value   ("addresser" , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (*x_type != YCALC_DATA_BLANK) {
      rc = ycalc_range_include (x_deproot, x, y, z);
      DEBUG_CALC   yLOG_value   ("ranges"    , rc);
      --rce;  if (rc < 0) {
         DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(build)--------------------------*/
   DEBUG_CALC   yLOG_note    ("now consider build and execution");
   DEBUG_CALC   yLOG_char    ("*x_type"   , *x_type);
   DEBUG_CALC   yLOG_info    ("valid"     , YCALC_GROUP_RPN);
   if (strchr (YCALC_GROUP_RPN , *x_type) != NULL) {
      rc = ycalc_build_trusted    (x_deproot, x_source, x_type, x_value, x_string);
      DEBUG_CALC   yLOG_value   ("build"     , rc);
      --rce;  if (rc < 0) {
         DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(execute)------------------------*/
   DEBUG_CALC   yLOG_char    ("*x_type"   , *x_type);
   DEBUG_CALC   yLOG_info    ("valid"     , YCALC_GROUP_CALC);
   if (strchr (YCALC_GROUP_CALC, *x_type) != NULL) {
      rc = ycalc_execute_trusted  (x_deproot, x_type, x_value, x_string);
      DEBUG_CALC   yLOG_value   ("execute"   , rc);
      --rce;  if (rc < 0) {
         DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(call reaper)--------------------*/
   DEBUG_CALC   yLOG_note    ("then, cleanup as necessary");
   rc = ycalc_call_reaper (&x_owner, &x_deproot);
   DEBUG_CALC   yLOG_value   ("reaper"    , rc);
   --rce;  if (rc == 0) {
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                          sorting list                        ----===*/
/*====================------------------------------------====================*/
static void  o___SORT____________o () { return; }

static int  s_array     [1000];          /* working list as integers       */
static int  s_narray    =  0;            /* working count of entries       */

char         /*-> sort cell label list ---------------[ ------ [ge.RE5.1H6.A2]*/ /*-[03.0000.11#.!]-*/ /*-[--.---.---.--]-*/
ycalc__sort_prep   (char *a_list)
{
   /*---(locals)-----------+-----------+-*//*---------------------------------*/
   char        rce         = -10;           /* return code for errors         */
   char        x_list      [1000] = "";     /* copy of the list for parsing   */
   char       *p           = NULL;          /* strtok parsing pointer         */
   char       *q           = ",;";          /* strtok delimeter string        */
   char       *r           = NULL;
   int         i           =  0;            /* loop iterator -- entry         */
   int         x           =  0;            /* col of current entry           */
   int         y           =  0;            /* row of current entry           */
   int         z           =  0;            /* tab of current entry           */
   char        rc          =  0;            /* return code                    */
   tDEP_ROOT  *x_deproot   = NULL;
   /*---(header)-------------------------*/
   DEBUG_SORT    yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   DEBUG_SORT    yLOG_note    ("initialize");
   for (i = 0; i < 1000; ++i)  s_array [i] = 0;
   s_narray = 0;
   /*---(parse/load)---------------------*/
   DEBUG_SORT    yLOG_note    ("load the array");
   strcpy (x_list, a_list);
   p = strtok_r (x_list, q, &r);
   s_narray = 0;
   --rce;  while (p != NULL) {
      DEBUG_SORT    yLOG_info    ("parse"     , p);
      rc = str2gyges (p, &x, &y, &z, NULL, 0);
      DEBUG_SORT    yLOG_value   ("rc"        , rc);
      if (rc < 0)  {
         DEBUG_SORT    yLOG_note    ("could not parse, EXITING");
         DEBUG_SORT    yLOG_exit    (__FUNCTION__);
         return rce;
      }
      DEBUG_SORT    yLOG_complex ("parts"     , "z=%04d, x=%04d, y=%04d", z, x, y);
      ++z;
      ++x;
      ++y;
      DEBUG_SORT    yLOG_complex ("inserted"  , "z=%04d, x=%04d, y=%04d", z, x, y);
      s_array [s_narray] = (z * 10000000) + (x * 10000) + y;
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
   int         a           =  0;            /* comparison entry one           */
   int         b           =  0;            /* comparison entry two           */
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
   int         i           =  0;            /* loop iterator -- entry         */
   int         x           =  0;            /* col of current entry           */
   int         y           =  0;            /* row of current entry           */
   int         z           =  0;            /* tab of current entry           */
   tDEP_ROOT  *x_deproot   = NULL;
   char        x_label     [15];            /* label for sorted entry         */
   /*---(header)-------------------------*/
   DEBUG_SORT    yLOG_enter   (__FUNCTION__);
   strcpy (a_list, ",");
   for (i = 0; i < s_narray; ++i) {
      DEBUG_SORT    yLOG_value   ("value"   , s_array[i]);
      z     = s_array[i] / 10000000;
      x     = (s_array[i] - (z * 10000000))  / 10000;
      y     = (s_array[i] - (z * 10000000) - (x * 10000));
      DEBUG_SORT    yLOG_complex ("removed"   , "z=%04d, x=%04d, y=%04d", z, x, y);
      --z;
      --x;
      --y;
      DEBUG_SORT    yLOG_complex ("parts"     , "z=%04d, x=%04d, y=%04d", z, x, y);
      str4gyges (x, y, z, 0, x_label);
      DEBUG_SORT    yLOG_info    ("label"   , x_label);
      strcat (a_list, x_label);
      strcat (a_list, ",");
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

char         /*-> list dependencies ------------------[ leaf   [fe.740.424.60]*/ /*-[02.0000.034.!]-*/ /*-[--.---.---.--]-*/
ycalc__audit_disp_master   (tDEP_ROOT *a_me, char *a_list, char a_start, char *a_types)
{  /*---(design notes)-------------------*/
   /* combined logic from the three accessors below as they were 90% the same */
   /*---(locals)-----------+-----------+-*/
   tDEP_LINK  *n           = NULL;
   char        rce         = -10;
   /*---(defenses)-----------------------*/
   --rce;  if (a_list  == NULL)  return rce;
   strncpy (a_list, "-", LEN_RECD);   /* special for a null list */
   --rce;  if (a_me    == NULL)  return rce;
   /*---(setup)--------------------------*/
   strncpy (a_list, ",", LEN_RECD);
   --rce;  switch (a_start) {
   case 'R' :  n = a_me->reqs; break;
   case 'P' :  n = a_me->pros; break;
   default         :  return rce;
   }
   /*---(walk the list)------------------*/
   while (n != NULL) {
      if (strchr (a_types, n->type) != 0) {
         strlcat (a_list, ycalc_call_labeler (n->target), LEN_RECD);
         strlcat (a_list, ","                           , LEN_RECD);
      }
      n = n->next;
   }
   /*---(catch empty)--------------------*/
   if (strcmp (a_list, ",") == 0)   strcpy (a_list, ".");
   else  ycalc__audit_sort (a_list);
   /*---(complete)-----------------------*/
   return 0;
}

char       ycalc_audit_disp_reqs      (tDEP_ROOT *a_me, char *a_list) { return ycalc__audit_disp_master (a_me, a_list, 'R', S_DEP_REQS); }
char       ycalc_audit_disp_pros      (tDEP_ROOT *a_me, char *a_list) { return ycalc__audit_disp_master (a_me, a_list, 'P', S_DEP_PROS); }
char       ycalc_audit_disp_like      (tDEP_ROOT *a_me, char *a_list) { return ycalc__audit_disp_master (a_me, a_list, 'P', S_DEP_LIKE); }


void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_type          (void)
{
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   ycalc_pushval (__FUNCTION__, a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_isblank       (void)
{
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (a == YCALC_DATA_BLANK)   ycalc_pushval (__FUNCTION__, TRUE);
   else                         ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_isvalue       (void)
{
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (strchr (YCALC_GROUP_NUM, a) != NULL) ycalc_pushval (__FUNCTION__, TRUE);
   else                                     ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_istext        (void)
{
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (strchr (YCALC_GROUP_STR, a) != NULL) ycalc_pushval (__FUNCTION__, TRUE);
   else                                     ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_iscalc        (void)
{
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (strchr (YCALC_GROUP_CALC, a) != NULL) ycalc_pushval (__FUNCTION__, TRUE);
   else                                      ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_islit         (void)
{
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (strchr (YCALC_GROUP_CALC, a) == NULL) ycalc_pushval (__FUNCTION__, TRUE);
   else                                      ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_ispoint       (void)
{
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if      (a == YCALC_DATA_RANGE) ycalc_pushval (__FUNCTION__, TRUE);
   else if (a == YCALC_DATA_ADDR ) ycalc_pushval (__FUNCTION__, TRUE);
   else                            ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_iserror       (void)
{
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_TYPE);
   if (strchr (YCALC_GROUP_ERR , a) != NULL) ycalc_pushval (__FUNCTION__, TRUE);
   else                                      ycalc_pushval (__FUNCTION__, FALSE);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_xpos          (void)
{
   m = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_XPOS);
   ycalc_pushval (__FUNCTION__, m);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_ypos          (void)
{
   m = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_YPOS);
   ycalc_pushval (__FUNCTION__, m);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_zpos          (void)
{
   m = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_ZPOS);
   ycalc_pushval (__FUNCTION__, m);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_me            (void)
{
   ycalc_pushref (__FUNCTION__, myCALC.deproot);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_label         (void)
{
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_LABEL);
   ycalc_pushstr (__FUNCTION__, strndup (r, LEN_RECD));
   free (r);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_formula       (void)
{
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_SOURCE);
   ycalc_pushstr (__FUNCTION__, strndup (r, LEN_RECD));
   free (r);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_rpn           (void)
{
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_RPN);
   ycalc_pushstr (__FUNCTION__, strndup (r, LEN_RECD));
   free (r);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_reqs          (void)
{
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_REQS);
   ycalc_pushstr (__FUNCTION__, strndup (r, LEN_RECD));
   free (r);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_pros          (void)
{
   r = ycalc_popstr_plus (__FUNCTION__, G_SPECIAL_PROS);
   ycalc_pushstr (__FUNCTION__, strndup (r, LEN_RECD));
   free (r);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_ncalc         (void)
{
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_NCALC);
   ycalc_pushval (__FUNCTION__, a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_nreq          (void)
{
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_NREQ);
   ycalc_pushval (__FUNCTION__, a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_npro          (void)
{
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_NPRO);
   ycalc_pushval (__FUNCTION__, a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_level         (void)
{
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_LEVEL);
   ycalc_pushval (__FUNCTION__, a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_pointer       (void)
{
   /*---(locals)-----------+-----+-----+-*/
   tDEP_ROOT  *x_ref       = NULL;
   tDEP_ROOT  *x_other     = NULL;
   tCALC      *x_beg       = NULL;
   tCALC      *x_end       = NULL;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(get the reference)--------------*/
   x_ref = ycalc_popref (__FUNCTION__);
   DEBUG_CALC   yLOG_point   ("x_ref"     , x_ref);
   if (x_ref == NULL) {
      myCALC.trouble = G_ERROR_POINTER;
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   /*---(first)--------------------------*/
   x_beg = x_ref->chead;
   DEBUG_CALC   yLOG_point   ("x_beg"     , x_beg);
   if (x_beg == NULL || x_beg->t != G_TYPE_REF || x_beg->r == NULL) {
      myCALC.trouble = G_ERROR_POINTER;
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   DEBUG_CALC   yLOG_point   ("x_beg->r"  , x_beg->r);
   ycalc_pushref (__FUNCTION__, x_beg->r);
   DEBUG_CALC   yLOG_value   ("ncalc"     , x_ref->ncalc);
   if (x_ref->ncalc == 1) {
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   /*---(second)-------------------------*/
   if (x_ref->ncalc != 3) {
      myCALC.trouble = G_ERROR_POINTER;
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   x_end = x_beg->next;
   DEBUG_CALC   yLOG_point   ("x_end"     , x_end);
   if (x_end == NULL || x_end->t != G_TYPE_REF || x_end->r == NULL) {
      myCALC.trouble = G_ERROR_POINTER;
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return;
   }
   DEBUG_CALC   yLOG_point   ("x_end->r"  , x_end->r);
   ycalc_pushref (__FUNCTION__, x_end->r);
   /*---(last)---------------------------*/
   x_end = x_beg->next;
   DEBUG_CALC   yLOG_point   ("x_end"     , x_end);
   if (x_end == NULL || x_end->t != G_TYPE_NOOP) {
      myCALC.trouble = G_ERROR_POINTER;
      return;
   }
   /*---(complete)-----------------------*/
   return;
}




