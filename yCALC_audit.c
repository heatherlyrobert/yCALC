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
   {  YCALC_DATA_MERGED , "merged"     , '<', '-', '-', 'y', '-', "empty cell used to present merged information"      },
   {  YCALC_DATA_ERROR  , "error"      , ' ', '-', '-', '-', 'e', "error status"                                       },
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
   /* ---abbr-------- stage   --disp-     ---description-------------------------------------  */
   { G_ERROR_THING   , 'b' , "#ref"     , ""                                                   },
   { G_ERROR_RANGE   , 'b' , "#range"   , ""                                                   },
   { G_ERROR_DEPEND  , 'b' , "#dep"     , ""                                                   },
   { G_ERROR_TOKEN   , 'b' , "#token"   , ""                                                   },
   { G_ERROR_UNKNOWN , 'b' , "#boom"    , ""                                                   },
   { G_ERROR_POINTER , 'b' , "#point"   , ""                                                   },
   /* ---abbr-------- stage   --disp-     ---description-------------------------------------  */
   { 0               ,  0  , ""         , ""                                                   },
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
yCALC__handle_error     (char a_error, char *a_type, double *a_value, char **a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   int         n           =   -1;
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
   /*---(handle miss)--------------------*/
   if (n < 0)    *a_string = strdup ("#mystry");
   else          *a_string = strdup (zCALC_errors [i].terse);
   /*---(find entry)---------------------*/
   return 0;
}

char
yCALC__handle_prepare   (char *a_type, double *a_value, char **a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;           /* return code for errors         */
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CALC   yLOG_char    ("status"    , myCALC.status);
   --rce;  if (myCALC.status != 'O') {
      DEBUG_PROG   yLOG_note    ("must initialize and configure before use");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_point   ("a_type"     , a_type);
   --rce;  if (a_type == NULL) {
      DEBUG_CALC   yLOG_note    ("type pointer not given, can not classify");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_point   ("a_value"    , a_value);
   --rce;  if (a_value == NULL) {
      DEBUG_CALC   yLOG_note    ("value pointer not given, nothing to do");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_point   ("a_string"   , a_string);
   --rce;  if (a_string == NULL) {
      DEBUG_CALC   yLOG_note    ("string pointer not given, nothing to do");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   *a_type   = YCALC_DATA_BLANK;
   *a_value  = 0.0;
   if (*a_string != NULL) {
      free (*a_string);
      *a_string = NULL;
   }
   /*---(complete)-----------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yCALC__handle_formula   (char *a_label, char *a_src, char *a_type, double *a_value, char **a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;           /* return code for errors         */
   char        rc          =    0;
   char       *x_rpn       = NULL;
   int         x_nrpn      =    0;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(generate rpn)-------------------*/
   x_rpn = yRPN_spreadsheet (a_src + 1, &x_nrpn, 0);
   DEBUG_CALC   yLOG_value   ("x_nrpn"    , x_nrpn);
   --rce;  if (x_nrpn <= 0) {
      yCALC__handle_error (YCALC_ERROR_RPN, a_type, a_value, a_string);
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_point   ("x_rpn"     , x_rpn);
   --rce;  if (x_rpn == NULL) {
      yCALC__handle_error (YCALC_ERROR_RPN, a_type, a_value, a_string);
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_info    ("x_rpn"     , x_rpn);
   /*---(handle types)-------------------*/
   --rce;  if (a_src [0] == '&') {
      switch (x_nrpn) {
      case  1 : *a_type = YCALC_DATA_ADDR;   break;
      case  3 : *a_type = YCALC_DATA_RANGE;  break;
      default : yCALC__handle_error (YCALC_ERROR_POINT, a_type, a_value, a_string);
                DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
                return rce;
      }
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   else if (a_src [0] == '=')  *a_type = YCALC_DATA_NFORM;
   else if (a_src [0] == '#')  *a_type = YCALC_DATA_SFORM;
   /*---(build)--------------------------*/
   rc = yCALC_build_label (a_label, x_rpn, a_type, a_string);
   DEBUG_DEPS   yLOG_value   ("build"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(execute)------------------------*/
   rc = yCALC_exec_label  (a_label, a_type, a_value, a_string);
   DEBUG_DEPS   yLOG_value   ("exec"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yCALC_handle            (char *a_label, char *a_src, char *a_type, double *a_value, char **a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;           /* return code for errors         */
   char        rc          =    0;           /* return code for errors         */
   int         x_len       =    0;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   rc = yCALC__handle_prepare (a_type, a_value, a_string);
   DEBUG_CALC   yLOG_value   ("prepare"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(blanks)-------------------------*/
   DEBUG_CALC   yLOG_point   ("a_src"     , a_src);
   if (a_src == NULL) {
      if (a_type   != NULL)  *a_type   = YCALC_DATA_BLANK;
      DEBUG_CALC   yLOG_note    ("null object");
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   DEBUG_CALC   yLOG_info    ("a_src"     , a_src);
   if (a_src [0] == 0) {
      if (a_type   != NULL)  *a_type   = YCALC_DATA_BLANK;
      DEBUG_CALC   yLOG_note    ("blank object");
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   DEBUG_CALC   yLOG_info    ("a_src"     , a_src);
   /*---(merges)-------------------------*/
   x_len = strllen (a_src, LEN_RECD);
   DEBUG_CALC   yLOG_value   ("x_len"     , x_len);
   if (x_len == 1 && a_src [0] == '<') {
      DEBUG_CALC   yLOG_note    ("merged object");
      *a_type   = YCALC_DATA_MERGED;
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(formulas)-----------------------*/
   DEBUG_CELL   yLOG_info    ("valid"     , YCALC_GROUP_FPRE);
   --rce;  if (strchr (YCALC_GROUP_FPRE, a_src [0]) != NULL) {
      DEBUG_CALC   yLOG_note    ("formula object");
      rc = yCALC__handle_formula (a_label, a_src, a_type, a_value, a_string);
      DEBUG_CALC   yLOG_value   ("rc"        , rc);
      DEBUG_CALC   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(numbers)------------------------*/
   rc = strl2num (a_src, a_value, LEN_RECD);
   DEBUG_CALC   yLOG_value   ("rc"        , rc);
   if (rc >= 0) {
      DEBUG_CALC   yLOG_note    ("numeric literal");
      *a_type   = YCALC_DATA_NUM;
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
      ycalc_call_who_named (p, NULL, &x_deproot);
      DEBUG_SORT    yLOG_point   ("x_deproot" , x_deproot);
      if (x_deproot == NULL)  {
         DEBUG_SORT    yLOG_note    ("could not find deproot, EXITING");
         DEBUG_SORT    yLOG_exit    (__FUNCTION__);
         return rce;
      }
      rc = g_addresser (x_deproot->owner, &x, &y, &z);
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
      s_array [s_narray] = (z * 100000000) + (x * 100000) + (y * 10);
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
      z     = s_array[i] / 100000000;
      x     = (s_array[i] - (z * 100000000))  / 100000;
      y     = (s_array[i] - (z * 100000000) - (x * 100000))         / 10;
      DEBUG_SORT    yLOG_complex ("removed"   , "z=%04d, x=%04d, y=%04d", z, x, y);
      --z;
      --x;
      --y;
      DEBUG_SORT    yLOG_complex ("parts"     , "z=%04d, x=%04d, y=%04d", z, x, y);
      ycalc_call_who_at (x, y, z, NULL, &x_deproot);
      if (x_deproot == NULL)  {
         DEBUG_SORT    yLOG_note    ("could not find deproot, EXITING");
         DEBUG_SORT    yLOG_exit    (__FUNCTION__);
         return rce;
      }
      strlcpy (x_label, g_labeler (x_deproot->owner), LEN_LABEL);
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
         strlcat (a_list, g_labeler (n->target->owner), LEN_RECD);
         strlcat (a_list, ","                         , LEN_RECD);
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

/*> switch (a_what) {                                                                 <* 
 *> case G_SPECIAL_TYPE   :                                                           <* 
 *>    g_valuer  (x_deproot->owner, &s_type, NULL, NULL);                             <* 
 *>    return s_type;                                                                 <* 
 *>    break;                                                                         <* 
 *> case G_SPECIAL_NCALC  :                                                           <* 
 *>    return x_deproot->ncalc;                                                       <* 
 *>    break;                                                                         <* 
 *> case G_SPECIAL_NPRO   :                                                           <* 
 *>    return x_deproot->npro;                                                        <* 
 *>    break;                                                                         <* 
 *> case G_SPECIAL_NREQ   :                                                           <* 
 *>    return x_deproot->nreq;                                                        <* 
 *>    break;                                                                         <* 
 *> case G_SPECIAL_LEVEL  :                                                           <* 
 *>    return x_deproot->slevel;                                                      <* 
 *>    break;                                                                         <* 
 *> }                                                                                 <*/



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
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_XPOS);
   ycalc_pushval (__FUNCTION__, a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_ypos          (void)
{
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_YPOS);
   ycalc_pushval (__FUNCTION__, a);
   return;
}

void    /*-> tbd --------------------------------[ ------ [fv.220.010.22]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_zpos          (void)
{
   a = ycalc_popval_plus (__FUNCTION__, G_SPECIAL_ZPOS);
   ycalc_pushval (__FUNCTION__, a);
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




