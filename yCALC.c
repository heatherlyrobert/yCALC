/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"



tLOCAL    its;





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




/*====================------------------------------------====================*/
/*===----                    unit testing accessor                     ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

#define       LEN_TEXT  2000
char          unit_answer [ LEN_TEXT ];

char*            /* [------] unit test accessor ------------------------------*/
yCALC__unit             (char *a_question, int a_num)
{
   /*---(initialize)---------------------*/
   strlcpy (unit_answer, "yCALC_unit, unknown request", 100);
   /*---(string testing)-----------------*/
   /*> if      (strncmp(a_question, "string"    , 20)  == 0) {                        <* 
    *>    snprintf (unit_answer, LEN_TEXT, "yCALC string     : [%s]", its.strtest);   <* 
    *> }                                                                              <*/
   /*---(complete)-----------------------*/
   return unit_answer;
}

char       /*----: set up program urgents/debugging --------------------------*/
yCALC__unit_quiet       (void)
{
   its.logger = yLOG_begin ("yCALC" , yLOG_SYSTEM, yLOG_QUIET);
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
yCALC__unit_loud        (void)
{
   its.logger = yLOG_begin ("yCALC" , yLOG_SYSTEM, yLOG_NOISE);
   DEBUG_CALC   yLOG_info     ("yCALC"    , yCALC_version   ());
   return 0;
}

char       /*----: stop logging ----------------------------------------------*/
yCALC__unit_end         (void)
{
   yLOG_end     ();
   return 0;
}



/*===[[ END ]]================================================================*/
