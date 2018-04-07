/*===[[ HEADER GUARD ]]=======================================================*/
#ifndef YCALC_priv_hguard
#define YCALC_priv_hguard loaded



#include    <stdio.h>
#include    <stdlib.h>                  /* getenv()                            */
#include    <string.h>
#include    <math.h>  
#include    <time.h>         /* C_ANSI : time, strftime, localtime            */

#include    <yURG.h>                    /* heatherly program logger            */
#include    <ySTR.h>
#include    <yLOG.h>                    /* heatherly program logger            */
#include    "yCALC.h"



/* rapidly evolving version number to aid with visual change confirmation     */
#define YCALC_VER_NUM   "0.0b"
#define YCALC_VER_TXT   "added some basic functions and started the stack"

/*---(string lengths)-----------------*/
#define     LEN_LABEL   20
#define     LEN_STR     200
#define     LEN_RECD    2000

#define     TRUE        1
#define     FALSE       0


typedef struct cLOCAL tLOCAL;
struct cLOCAL {
   /*---(overall)-----------*/
   char        debug;
   int         logger;
   /*---(testing)-----------*/
   int         argc;
   char       *argv        [20];
};
extern  tLOCAL its;


extern int  g_nfunc;
extern int  g_ndups;
#define     MAX_FUNCS       1000
typedef struct cFUNCS  tFUNCS;
struct  cFUNCS {
   char        name        [20];       /* operator symbol/name                */
   char        len;                    /* length of name                      */
   void   (*f) (void);                 /* function pointer                    */
   char        type;                   /* type (func, op, or const)           */
   char        terms       [10];       /* number of terms                     */
   char        fcat;                   /* category                            */
   char        desc        [60];       /* descriptive label                   */
   char        disp        [30];       /* display version of function         */
};
extern tFUNCS  g_funcs [MAX_FUNCS];



extern double      a, b, c, d, e;
extern int         m, n, o, len;
extern double      tot, min, max;
extern int         cnt, cnta, cnts, cntb, cntr;
extern double      entries     [1000];
extern char       *q, *r, *s;
extern char        t           [LEN_RECD];
extern char        nada        [5];

#define  MAX   1000000000

/*> PRIV   tCELL      *s_me;                                                          <*/
extern char        s_narg;
extern int         s_nerror;
extern int         s_nbuild;
extern int         s_neval;
extern int         errornum;
extern char        errorstr      [LEN_RECD];






char        yCALC__unit_quiet       (void);
char        yCALC__unit_loud        (void);
char        yCALC__unit_end         (void);
char*       yCALC__unit             (char *a_question, int a_num);


/*---(placeholder)--------------------*/
void        yCALC_noop              (void);
/*---(mathmatical)--------------------*/
void        yCALC_add               (void);
void        yCALC_subtract          (void);
void        yCALC_multiply          (void);
void        yCALC_divide            (void);
void        yCALC_modulus           (void);
void        yCALC_increment         (void);
void        yCALC_decrement         (void);
void        yCALC_unaryminus        (void);
/*---(mathmatical)--------------------*/
void        yCALC_power             (void);
void        yCALC_abs               (void);
void        yCALC_trunc             (void);
void        yCALC_rtrunc            (void);
void        yCALC_round             (void);
void        yCALC_rround            (void);
void        yCALC_ceiling           (void);
void        yCALC_floor             (void);
void        yCALC_sqrt              (void);
void        yCALC_cbrt              (void);
void        yCALC_sqr               (void);
void        yCALC_cube              (void);
void        yCALC_rand              (void);
void        yCALC_randr             (void);
/*---(num relational)-----------------*/
void        yCALC_equal             (void);
void        yCALC_notequal          (void);
void        yCALC_greater           (void);
void        yCALC_lesser            (void);
void        yCALC_gequal            (void);
void        yCALC_lequal            (void);
/*---(str relational)-----------------*/
void        yCALC_sequal            (void);
void        yCALC_snotequal         (void);
void        yCALC_sgreater          (void);
void        yCALC_slesser           (void);
/*---(logical)------------------------*/
void        yCALC_not               (void);
void        yCALC_and               (void);
void        yCALC_or                (void);



#endif
