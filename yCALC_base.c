/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"



tLOCAL      myCALC;

double      a, b, c, d, e;
int         m, n, o, len;
double      tot, min, max;
int         cnt, cnta, cnts, cntb, cntr;
double      entries     [1000];
char       *q, *r, *s;
char        t           [LEN_RECD];
char        g_nada       [5] = "";




tFUNCS  g_funcs [MAX_FUNCS] = {
   /*---(mathmatical operators)-----------*/
   { "+"          ,  1, ycalc_add               , 'o', "v:vv"   , 'm', "ansi-c addition"                                   , "" },
   { "-"          ,  1, ycalc_subtract          , 'o', "v:vv"   , 'm', "ansi-c subtraction"                                , "" },
   { "*"          ,  1, ycalc_multiply          , 'o', "v:vv"   , 'm', "ansi-c multipication"                              , "" },
   { "/"          ,  1, ycalc_divide            , 'o', "v:vv"   , 'm', "ansi-c division"                                   , "" },
   { "%"          ,  1, ycalc_modulus           , 'o', "v:vv"   , 'm', "ansi-c modulus"                                    , "" },
   { "++"         ,  2, ycalc_increment         , 'o', "v:v"    , 'm', "truly x + 1 as base variable uneffected"           , "" },
   { "--"         ,  2, ycalc_decrement         , 'o', "v:v"    , 'm', "truly x - 1 as base variable uneffected"           , "" },
   { ":+"         ,  2, ycalc_increment         , 'o', "v:v"    , 'm', "non-standard, acts like prefix ++"                 , "" },
   { ":-"         ,  2, ycalc_decrement         , 'o', "v:v"    , 'm', "non-standard, acts like prefix --"                 , "" },
   { "-:"         ,  2, ycalc_unaryminus        , 'o', "v:v"    , 'm', "ansi-c unary minus"                                , "" },
   { "+:"         ,  2, ycalc_noop              , 'o', "v:v"    , 'm', "ansi-c unary plus (no effect)"                     , "" },
   /*---(mathmatical functions)-----------*/
   { "exp"        ,  3, ycalc_power             , 'f', "v:vv"   , 'm', "x raised to the power of y"                        , "" },
   { "abs"        ,  3, ycalc_abs               , 'f', "v:v"    , 'm', "ansi-c fabs() removes negative sign"               , "" },
   { "trunc"      ,  5, ycalc_trunc             , 'f', "v:v"    , 'm', "truncate to the nearest integer value"             , "" },
   { "rtrunc"     ,  6, ycalc_rtrunc            , 'f', "v:vv"   , 'm', "truncate to y decimal places"                      , "" },
   { "round"      ,  5, ycalc_round             , 'f', "v:v"    , 'm', "round, up or down, to the nearest integer"         , "" },
   { "rround"     ,  6, ycalc_rround            , 'f', "v:vv"   , 'm', "round, up or down, to y decimal places"            , "" },
   { "ceil"       ,  4, ycalc_ceiling           , 'f', "v:v"    , 'm', "raise up to the nearest integer"                   , "" },
   { "floor"      ,  5, ycalc_floor             , 'f', "v:v"    , 'm', "push down to the nearest integer"                  , "" },
   { "sqrt"       ,  4, ycalc_sqrt              , 'f', "v:v"    , 'm', "ansi-c square root"                                , "" },
   { "cbrt"       ,  4, ycalc_cbrt              , 'f', "v:v"    , 'm', "cube root of x"                                    , "" },
   { "sqr"        ,  3, ycalc_sqr               , 'f', "v:v"    , 'm', "square of x"                                       , "" },
   { "cube"       ,  4, ycalc_cube              , 'f', "v:v"    , 'm', "cube of x"                                         , "" },
   { "rand"       ,  4, ycalc_rand              , 'f', "v:"     , 'm', "random number betreen 0 and 1"                     , "" },
   { "randr"      ,  5, ycalc_randr             , 'f', "v:vv"   , 'm', "random number betreen x and y"                     , "" },
   /*---(relational operators)------------*/
   { "=="         ,  2, ycalc_equal             , 'o', "t:vv"   , 'l', "T if x are y are equal, else F"                    , "" },
   { "!="         ,  2, ycalc_notequal          , 'o', "t:vv"   , 'l', "T if x not equal y, else F"                        , "" },
   { ">"          ,  1, ycalc_greater           , 'o', "t:vv"   , 'l', "T if x greater than y, else F"                     , "" },
   { "<"          ,  1, ycalc_lesser            , 'o', "t:vv"   , 'l', "T if x lesser than y, else F"                      , "" },
   { ">="         ,  2, ycalc_gequal            , 'o', "t:vv"   , 'l', "T if x greater than or equal to y, else F"         , "" },
   { "<="         ,  2, ycalc_lequal            , 'o', "t:vv"   , 'l', "T if x lesser than or equal to y, else F"          , "" },
   { "#="         ,  2, ycalc_sequal            , 'o', "t:ss"   , 'l', "T if n are m are equal, else F"                    , "" },
   { "#!"         ,  2, ycalc_snotequal         , 'o', "t:ss"   , 'l', "T if n not equal m, else F"                        , "" },
   { "#<"         ,  2, ycalc_slesser           , 'o', "t:ss"   , 'l', "T if n greater than m, else F"                     , "" },
   { "#>"         ,  2, ycalc_sgreater          , 'o', "t:ss"   , 'l', "T if n lesser than m, else F"                      , "" },
   /*---(locgical operators)--------------*/
   { "!"          ,  1, ycalc_not               , 'o', "t:v"    , 'l', "T if x F, else F"                                  , "" },
   { "&&"         ,  2, ycalc_and               , 'o', "t:vv"   , 'l', "T if both x and y are T, else F"                   , "" },
   { "||"         ,  2, ycalc_or                , 'o', "t:vv"   , 'l', "T if either x or y is T, else F"                   , "" },
   /*---(logical functions)---------------*/
   { "if"         ,  2, ycalc_if                , 'f', "v:vvv"  , 'l', "if x is T, y, else z"                              , "" },
   { "ifs"        ,  3, ycalc_ifs               , 'f', "s:vss"  , 'l', "if x is T, n, else m"                              , "" },
   { "within"     ,  6, ycalc_within            , 'f', "t:vvv"  , 'l', "if y is within the range of x to z, then T"        , "" },
   { "approx"     ,  6, ycalc_approx            , 'f', "t:vvv"  , 'l', "if x is within the range of y +/- z, then T"       , "" },
   /*---(end-of-s_funcs)--------------------*/
   { "END"        ,  0, NULL                    , '-', ""       , '-', ""                                                  , "" },
};




/*====================------------------------------------====================*/
/*===----                           utility                            ----===*/
/*====================------------------------------------====================*/
static void      o___UTILITY_________________o (void) {;}

char        yCALC_ver     [500];

char*      /* ---- : return library versioning information -------------------*/
yCALC_version       (void)
{
   char    t [20] = "";
#if    __TINYC__ > 0
   strlcpy (t, "[tcc built  ]", 15);
#elif  __GNUC__  > 0
   strlcpy (t, "[gnu gcc    ]", 15);
#elif  __HEPH__  > 0
   strncpy (t, "[hephaestus ]", 15);
#else
   strlcpy (t, "[unknown    ]", 15);
#endif
   snprintf (yCALC_ver, 100, "%s   %s : %s", t, YCALC_VER_NUM, YCALC_VER_TXT);
   return yCALC_ver;
}

char
yCALC_init              (char a_style)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_PROG   yLOG_char    ("status"    , myCALC.status);
   --rce;  if (myCALC.status != NULL && strchr ("IO", myCALC.status) != NULL) {
      DEBUG_PROG   yLOG_note    ("can not re-initialize");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strlcpy (myCALC.status_detail, "- ---- -", LEN_LABEL);
   /*---(initialize)---------------------*/
   DEBUG_PROG   yLOG_note    ("running sub-initializations");
   ycalc_build_init  ();
   ycalc_exec_init   ();
   ycalc_deps_init   ();
   /*---(update status)------------------*/
   myCALC.status = 'I';
   myCALC.status_detail [0] = 'i';
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yCALC_wrap              (void)
{
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(shutdown)-----------------------*/
   ycalc_deps_wrap   ();
   myCALC.status = '-';
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      special operators                       ----===*/
/*====================------------------------------------====================*/
static void  o___SPECIAL_________o () { return; }

void    /*-> tbd --------------------------------[ leaf   [fv.210.000.00]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ycalc_noop        (void)
{
   return;
}

char
ycalc_trouble_clear     (void)
{
   myCALC.trouble = G_NO_ERROR;
   return 0;
}




/*====================------------------------------------====================*/
/*===----                    unit testing accessor                     ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /* [------] unit test accessor ------------------------------*/
ycalc__unit_base        (char *a_question, int a_num)
{
   /*---(initialize)---------------------*/
   strlcpy (ycalc__unit_answer, "yCALC_unit, unknown request", 100);
   /*---(string testing)-----------------*/
   if      (strncmp(a_question, "error"     , 20)  == 0) {
      snprintf (ycalc__unit_answer, LEN_STR, "BASE error       : %c", myCALC.trouble);
   }
   /*---(complete)-----------------------*/
   return ycalc__unit_answer;
}

char       /*----: set up program urgents/debugging --------------------------*/
ycalc__unit_quiet       (void)
{
   myCALC.logger = yLOG_begin ("yCALC" , yLOG_SYSTEM, yLOG_QUIET);
   myCALC.trouble = G_NO_ERROR;
   yCALC_init ('s');
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
ycalc__unit_loud        (void)
{
   char       *x_args [4]  = { "yCALC_debug","@@kitchen","@@CALC", "@@SORT" };
   yURG_logger (4, x_args);
   yURG_urgs   (4, x_args);
   DEBUG_CALC   yLOG_info     ("yCALC"    , yCALC_version   ());
   myCALC.trouble = G_NO_ERROR;
   yCALC_init ('s');
   return 0;
}

char       /*----: stop logging ----------------------------------------------*/
ycalc__unit_end         (void)
{
   yCALC_wrap   ();
   yLOG_end     ();
   return 0;
}



/*===[[ END ]]================================================================*/
