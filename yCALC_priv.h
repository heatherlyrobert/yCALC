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
#define YCALC_VER_NUM   "0.0d"
#define YCALC_VER_TXT   "basic stack pop and push unit testing for references working"

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
   char        status;
   char        trouble;
   /*---(testing)-----------*/
   int         argc;
   char       *argv        [20];
};
extern  tLOCAL myCALC;
#define      G_NO_ERROR       '-'
#define      G_BUILD_ERROR    'b'
#define      G_STACK_ERROR    's'
#define      G_CONF_ERROR     'c'
#define      G_EXEC_ERROR     'e'



/*
 * calculation types
 *    f = function call
 *    n = constant number
 *    l = constant string literal
 *    s = string from another cell source field
 *    m = string from another cell modified field
 *    v = value from another cell value field
 *    x = noop
 */
typedef     struct   cCALC  tCALC;         /* cell calculation entry    */
struct cCALC {
   char      t;               /* type of calculation element                  */
   double    v;               /* numeric literal                              */
   char     *s;               /* string literal                               */
   void     *r;               /* pointer to a cell                            */
   void    (*f) (void);       /* function pointer                             */
   void     *owner;           /* pointer to the cell that owns this calc      */
   tCALC    *next;            /* pointer to next calc                         */
   tCALC    *prev;            /* pointer to next calc                         */
};

#define     MAX_TERM     20
typedef     struct cTERMS   tTERMS;
struct cTERMS {
   char        type;
   char       *abbr;
   char       *desc;
};
extern tTERMS      s_terms [MAX_TERM];



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

extern char   yCALC__unit_answer [LEN_STR ];



extern double      a, b, c, d, e;
extern int         m, n, o, len;
extern double      tot, min, max;
extern int         cnt, cnta, cnts, cntb, cntr;
extern double      entries     [1000];
extern char       *q, *r, *s;
extern char        t           [LEN_RECD];
extern char        g_nada      [5];

#define  MAX   1000000000

/*> PRIV   tCELL      *s_me;                                                          <*/
extern char        s_narg;
extern int         s_nerror;
extern int         s_nbuild;
extern int         s_neval;
extern int         errornum;
extern char        errorstr      [LEN_RECD];






char        yCALC_trouble_clear     (void);
char        yCALC__unit_quiet       (void);
char        yCALC__unit_loud        (void);
char        yCALC__unit_end         (void);
char*       yCALC__unit             (char *a_question, int a_num);


/*===[ EXEC ]=============================================*/
/*---(program)------------------------*/
char        yCALC_exec_init         (void);
/*---(pushing)------------------------*/
char        yCALC_pushstr           (char *a_func, char   *a_string);
char        yCALC_pushval           (char *a_func, double  a_value);
char        yCALC_pushref           (char *a_func, void   *a_thing);
/*---(popping)------------------------*/
double      yCALC_popval            (char *a_func);
char*       yCALC_popstr            (char *a_func);
/*---(unittest)-----------------------*/
char*       yCALC__unit_stack       (char *a_question, int a_num);
char        yCALC__unit_stackset    (int   a_num);
/*---(mock)---------------------------*/
void*       yCALC__unit_thinger     (char *a_label);
char        yCALC__unit_valuer      (void *a_thing, char *a_type, double *a_value, char **a_string);
char        yCALC__unit_detailer    (void *a_thing, char *a_quality, char *a_string, double *a_value);
char        yCALC__unit_addresser   (void *a_thing, int  *x, int *y, int *z);

/*===[ BUILD ]============================================*/
/*---(program)------------------------*/
char        yCALC_build_init        (void);





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
