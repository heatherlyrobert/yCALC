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

char        s_narg        = 0;
int         s_nerror      = 0;
int         s_nbuild      = 0;
int         s_neval       = 0;
int         errornum;
char        errorstr      [LEN_RECD];



tFUNCS  g_funcs [MAX_FUNCS] = {
   /*---(mathmatical operators)-----------*/
   { "+"          ,  1, yCALC_add               , 'o', "v:vv"   , 'm', "ansi-c addition"                                   , "" },
   { "-"          ,  1, yCALC_subtract          , 'o', "v:vv"   , 'm', "ansi-c subtraction"                                , "" },
   { "*"          ,  1, yCALC_multiply          , 'o', "v:vv"   , 'm', "ansi-c multipication"                              , "" },
   { "/"          ,  1, yCALC_divide            , 'o', "v:vv"   , 'm', "ansi-c division"                                   , "" },
   { "%"          ,  1, yCALC_modulus           , 'o', "v:vv"   , 'm', "ansi-c modulus"                                    , "" },
   { "++"         ,  2, yCALC_increment         , 'o', "v:v"    , 'm', "truly x + 1 as base variable uneffected"           , "" },
   { "--"         ,  2, yCALC_decrement         , 'o', "v:v"    , 'm', "truly x - 1 as base variable uneffected"           , "" },
   { ":+"         ,  2, yCALC_increment         , 'o', "v:v"    , 'm', "non-standard, acts like prefix ++"                 , "" },
   { ":-"         ,  2, yCALC_decrement         , 'o', "v:v"    , 'm', "non-standard, acts like prefix --"                 , "" },
   { "-:"         ,  2, yCALC_unaryminus        , 'o', "v:v"    , 'm', "ansi-c unary minus"                                , "" },
   { "+:"         ,  2, yCALC_noop              , 'o', "v:v"    , 'm', "ansi-c unary plus (no effect)"                     , "" },
   /*---(mathmatical functions)-----------*/
   { "exp"        ,  3, yCALC_power             , 'f', "v:vv"   , 'm', "x raised to the power of y"                        , "" },
   { "abs"        ,  3, yCALC_abs               , 'f', "v:v"    , 'm', "ansi-c fabs() removes negative sign"               , "" },
   { "trunc"      ,  5, yCALC_trunc             , 'f', "v:v"    , 'm', "truncate to the nearest integer value"             , "" },
   { "rtrunc"     ,  6, yCALC_rtrunc            , 'f', "v:vv"   , 'm', "truncate to y decimal places"                      , "" },
   { "round"      ,  5, yCALC_round             , 'f', "v:v"    , 'm', "round, up or down, to the nearest integer"         , "" },
   { "rround"     ,  6, yCALC_rround            , 'f', "v:vv"   , 'm', "round, up or down, to y decimal places"            , "" },
   { "ceil"       ,  4, yCALC_ceiling           , 'f', "v:v"    , 'm', "raise up to the nearest integer"                   , "" },
   { "floor"      ,  5, yCALC_floor             , 'f', "v:v"    , 'm', "push down to the nearest integer"                  , "" },
   { "sqrt"       ,  4, yCALC_sqrt              , 'f', "v:v"    , 'm', "ansi-c square root"                                , "" },
   { "cbrt"       ,  4, yCALC_cbrt              , 'f', "v:v"    , 'm', "cube root of x"                                    , "" },
   { "sqr"        ,  3, yCALC_sqr               , 'f', "v:v"    , 'm', "square of x"                                       , "" },
   { "cube"       ,  4, yCALC_cube              , 'f', "v:v"    , 'm', "cube of x"                                         , "" },
   { "rand"       ,  4, yCALC_rand              , 'f', "v:"     , 'm', "random number betreen 0 and 1"                     , "" },
   { "randr"      ,  5, yCALC_randr             , 'f', "v:vv"   , 'm', "random number betreen x and y"                     , "" },
   /*---(relational operators)------------*/
   { "=="         ,  0, yCALC_equal             , 'o', "t:vv"   , 'l', "T if x are y are equal, else F"                    , "" },
   { "!="         ,  0, yCALC_notequal          , 'o', "t:vv"   , 'l', "T if x not equal y, else F"                        , "" },
   { ">"          ,  0, yCALC_greater           , 'o', "t:vv"   , 'l', "T if x greater than y, else F"                     , "" },
   { "<"          ,  0, yCALC_lesser            , 'o', "t:vv"   , 'l', "T if x lesser than y, else F"                      , "" },
   { ">="         ,  0, yCALC_gequal            , 'o', "t:vv"   , 'l', "T if x greater than or equal to y, else F"         , "" },
   { "<="         ,  0, yCALC_lequal            , 'o', "t:vv"   , 'l', "T if x lesser than or equal to y, else F"          , "" },
   { "#="         ,  0, yCALC_sequal            , 'o', "t:ss"   , 'l', "T if n are m are equal, else F"                    , "" },
   { "#!"         ,  0, yCALC_snotequal         , 'o', "t:ss"   , 'l', "T if n not equal m, else F"                        , "" },
   { "#<"         ,  0, yCALC_slesser           , 'o', "t:ss"   , 'l', "T if n greater than m, else F"                     , "" },
   { "#>"         ,  0, yCALC_sgreater          , 'o', "t:ss"   , 'l', "T if n lesser than m, else F"                      , "" },
   /*---(locgical operators)--------------*/
   { "!"          ,  0, yCALC_not               , 'o', "t:v"    , 'l', "T if x F, else F"                                  , "" },
   { "&&"         ,  0, yCALC_and               , 'o', "t:vv"   , 'l', "T if both x and y are T, else F"                   , "" },
   { "||"         ,  0, yCALC_or                , 'o', "t:vv"   , 'l', "T if either x or y is T, else F"                   , "" },
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
   --rce;  if (strchr ("io", myCALC.status) != NULL) {
      DEBUG_PROG   yLOG_note    ("can not re-initialize");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strlcpy (myCALC.status_detail, "- ---- -", LEN_LABEL);
   /*---(initialize)---------------------*/
   DEBUG_PROG   yLOG_note    ("running sub-initializations");
   yCALC__build_init ();
   yCALC__exec_init  ();
   /*---(update status)------------------*/
   myCALC.status = 'I';
   myCALC.status_detail [0] = 'i';
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      special operators                       ----===*/
/*====================------------------------------------====================*/
static void  o___SPECIAL_________o () { return; }

void    /*-> tbd --------------------------------[ leaf   [fv.210.000.00]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
yCALC_noop        (void)
{
   return;
}

char
yCALC_trouble_clear     (void)
{
   myCALC.trouble = G_NO_ERROR;
   return 0;
}




/*====================------------------------------------====================*/
/*===----                    unit testing accessor                     ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /* [------] unit test accessor ------------------------------*/
yCALC__unit_base        (char *a_question, int a_num)
{
   /*---(initialize)---------------------*/
   strlcpy (yCALC__unit_answer, "yCALC_unit, unknown request", 100);
   /*---(string testing)-----------------*/
   if      (strncmp(a_question, "error"     , 20)  == 0) {
      snprintf (yCALC__unit_answer, LEN_STR, "BASE error       : %c", myCALC.trouble);
   }
   /*---(complete)-----------------------*/
   return yCALC__unit_answer;
}

char       /*----: set up program urgents/debugging --------------------------*/
yCALC__unit_quiet       (void)
{
   myCALC.logger = yLOG_begin ("yCALC" , yLOG_SYSTEM, yLOG_QUIET);
   myCALC.trouble = G_NO_ERROR;
   yCALC_init ('s');
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
yCALC__unit_loud        (void)
{
   myCALC.logger = yLOG_begin ("yCALC" , yLOG_SYSTEM, yLOG_NOISE);
   DEBUG_CALC   yLOG_info     ("yCALC"    , yCALC_version   ());
   myCALC.trouble = G_NO_ERROR;
   yCALC_init ('s');
   return 0;
}

char       /*----: stop logging ----------------------------------------------*/
yCALC__unit_end         (void)
{
   yLOG_end     ();
   return 0;
}



/*===[[ END ]]================================================================*/
