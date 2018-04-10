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
#define YCALC_VER_NUM   "0.0g"
#define YCALC_VER_TXT   "dep new and free are working and unit tested"

/*---(string lengths)-----------------*/
#define     LEN_LABEL   20
#define     LEN_STR     200
#define     LEN_RECD    2000

#define     TRUE        1
#define     FALSE       0



extern char    (*g_cleanser )   (void *a_thing);
extern char    (*g_creater  )   (char  a_type , void *a_origin , void   *a_target);
extern char    (*g_delcref  )   (char  a_type , void *a_origin , void   *a_target);
extern char    (*g_ranger   )   (void *a_thing, int x1, int y1, int z1, int x2, int y2, int z2);

extern char    (*g_thinger  )   (char *a_label, void **a_thing  );
extern char    (*g_valuer   )   (void *a_thing, char  *a_type   , double *a_value , char   **a_string);
extern char    (*g_detailer )   (void *a_thing, char  *a_quality, char   *a_string, double  *a_value);
extern char    (*g_addresser)   (void *a_thing, int   *x        , int    *y       , int     *z);
extern char    (*g_lister   )   (void *a_thing, char  *a_quality, char   *a_list  );


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
   void     *r;               /* reference pointer                            */
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




/*===[[ DEPENDENCIES ]]=======================================================*/
typedef     struct      cDEP_LINK   tDEP_LINK;
typedef     struct      cDEP_ROOT   tDEP_ROOT;
typedef     struct      cDEP_INFO   tDEP_INFO;



/*---(dependency root)------------------------------------*/
/*
 *  dependency roots are linked to each calculatable host node/vertex and
 *  provide everything needed to get the calculation job done.
 *
 */
struct      cDEP_ROOT {
   /*---(tie to owner)------*/
   void       *owner;
   /*---(calculation)-------*/
   char       *rpn;          /* rpn version of formula                        */
   char        nrpn;         /* number of calculation tokens                  */
   tCALC      *calc;         /* pointer to head of calculation line           */
   /*---(dependencies)------*/
   char        nreq;         /* number of required cells                      */
   tDEP_LINK  *reqs;         /* incomming predesesors to this calc            */
   char        npro;         /* number of dependent cells                     */
   tDEP_LINK  *pros;         /* outgoing successors to this calc              */
   /*---(sequencing)--------*/
   int         slevel;       /* sequence level                                */
   void       *snext;        /* next in sequence level                        */
   void       *sprev;        /* prev in sequence level                        */
   /*---(stamping)----------*/
   long        u;            /* timestamp of last update run                  */
   /*---(full linked list)---------------*/
   tDEP_ROOT  *fprev;         /* pointer to prev dep root in full list        */
   tDEP_ROOT  *fnext;         /* pointer to next dep root in full list        */
   /*---(done)--------------*/
};



/*---(denpendency link)------------------*/
/*
 * dependencies are carried on one-way links.  these links carry little
 * information, but carry direct pointers to every related structure so that
 * once the dependency graph is needed, it is fast, efficient, and right.
 *
 */
struct      cDEP_LINK  {
   /*---(dependency)---------------------*/
   char        type;          /* type of connection                           */
   tDEP_ROOT  *source;        /* pointer to source cell                       */
   tDEP_ROOT  *target;        /* pointer to target cell                       */
   /*---(cell linked list)---------------*/
   tDEP_LINK  *prev;          /* pointer to prev dependency for source cell   */
   tDEP_LINK  *next;          /* pointer to next dependency for source cell   */
   tDEP_LINK  *match;         /* pointer to matching dependency in ther dir  */
   /*---(deps linked list)---------------*/
   tDEP_LINK  *dprev;         /* pointer to prev dependency in full list      */
   tDEP_LINK  *dnext;         /* pointer to next dependency in full list      */
   /*---(statistics)---------------------*/
   int         count;         /* number of times used for dep/calc            */
   /*---(done)---------------------------*/
};
static tDEP_LINK     *s_hdep;
static tDEP_LINK     *s_tdep;
static int       s_ndep;



/*---(dependency type)-------------------*/
/*
 * dependencies are complex and relate many different types of nodes/vertices.
 * in order to keep all the dependencies sorted out and properly classified,
 * a more formal system was required.
 *
 */
#define     MAX_DEPTYPE     30
struct cDEP_INFO {
   cchar       type;                   /* connection type                     */
   cchar       match;                  /* matching connect type               */
   cchar       dir;                    /* direction (require vs provide)      */
   cchar       desc        [50];       /* description of dependency type      */
   char        match_index;            /* index of matching type              */
   int         count;                  /* current count of type               */
   int         total;                  /* total of type ever created          */
};
tDEP_INFO   g_dep_info [MAX_DEPTYPE];

#define     G_DEP_DIRNONE        ' '
#define     G_DEP_DIRREQ         '-'
#define     G_DEP_DIRPRO         '+'

extern char S_DEP_REQS [10];
extern char S_DEP_PROS [10];
extern char S_DEP_LIKE [10];



/*===[[ ACCESSOR ]]===========================================================*/

typedef struct cLOCAL tLOCAL;
struct cLOCAL {
   /*---(overall)-----------*/
   char        debug;
   int         logger;
   char        status_detail    [LEN_LABEL];
   char        status;
   char        trouble;
   /*---(testing)-----------*/
   int         argc;
   char       *argv        [20];
   /*---(data)--------------*/
   tDEP_ROOT  *droot;
   tDEP_LINK  *dhead;
   tDEP_LINK  *dtail;
   int         dcount;
   /*---(done)--------------*/
};
extern  tLOCAL myCALC;



#define     G_NO_ERROR       '-'
#define     G_ERROR_BUILD    'b'
#define     G_ERROR_STACK    's'
#define     G_ERROR_CONF     'c'
#define     G_ERROR_EXEC     'e'
#define     G_ERROR_RANGE    'R'
#define     G_ERROR_THING    'T'
#define     G_ERROR_DEPEND   'D'
#define     G_ERROR_TOKEN    'E'
#define     G_ERROR_UNKNOWN  'U'



#define     G_TYPE_EMPTY        '-'
#define     G_TYPE_NUM          'n'
#define     G_TYPE_STR          's'
#define     G_TYPE_REF          'r'




char        yCALC__deps_new         (tDEP_LINK **a_dep);
char        yCALC_deps_init         (void);
char        yCALC__deps_purge       (void);
char        yCALC_deps_wrap         (void);
char        yCALC__deps_free        (tDEP_LINK **a_dep);
char*       yDEP__unit              (char *a_question, void *a_point);



char        yCALC_trouble_clear     (void);
char        yCALC__unit_quiet       (void);
char        yCALC__unit_loud        (void);
char        yCALC__unit_end         (void);
char*       yCALC__unit_deps        (char *a_question, void *a_point);




/*===[ BUILD ]============================================*/
/*---(program)------------------------*/
char        yCALC__build_init       (void);



/*===[ EXEC ]=============================================*/
/*---(program)------------------------*/
char        yCALC__exec_init        (void);
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
