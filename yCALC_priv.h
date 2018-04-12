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
#define YCALC_VER_NUM   "0.0j"
#define YCALC_VER_TXT   "basic formula-driven dependencies unit tested nicely."

/*---(string lengths)-----------------*/
#define     LEN_LABEL   20
#define     LEN_STR     200
#define     LEN_RECD    2000

#define     TRUE        1
#define     FALSE       0



extern void*   (*g_deproot  )   (char *a_label);        /* pass label of thing, get back deproot of thing  */
extern void*   (*g_whois    )   (int x, int y, int z);  /* pass coordinates, get back deproot of thing     */

extern char*   (*g_labeler  )   (void *a_owner);        /* pass deproot->owner, get back label of thing    */
extern char    (*g_valuer   )   (void *a_owner, char  *a_type   , double *a_value , char   **a_string);
extern char    (*g_addresser)   (void *a_owner, int   *x        , int    *y       , int     *z);
extern char    (*g_detailer )   (void *a_owner, char  *a_quality, char   *a_string, double  *a_value);
extern char    (*g_reaper   )   (void *a_owner);        /* pass deproot->owner, tries to kill thing        */


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
   /*---(owner)-------------*/
   void     *deproot;         /* pointer to the cell that owns this calc      */
   /*---(contents)----------*/
   char      t;               /* type of calculation element                  */
   double    v;               /* numeric literal                              */
   char     *s;               /* string literal                               */
   void     *r;               /* reference pointer                            */
   void    (*f) (void);       /* function pointer                             */
   /*---(linked-list)-------*/
   tCALC    *next;            /* pointer to next calc                         */
   tCALC    *prev;            /* pointer to next calc                         */
   /*---(done)--------------*/
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

extern char   ycalc__unit_answer [LEN_STR ];



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
   int         ncalc;        /* number of calculation tokens                  */
   tCALC      *chead;        /* pointer to head of calculation chain          */
   tCALC      *ctail;        /* pointer to tail of calculation chain          */
   int         cuse;         /* number of times calculated                    */
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
   tDEP_ROOT  *rprev;         /* pointer to prev dep root in full list        */
   tDEP_ROOT  *rnext;         /* pointer to next dep root in full list        */
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

#define     G_DEP_ROOT           'r'
#define     G_DEP_UNROOT         'u'
#define     G_DEP_CHECKROOT      'a'


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
   char        status;
   char        status_detail    [LEN_LABEL];
   char        trouble;
   /*---(testing)-----------*/
   int         argc;
   char       *argv        [20];
   /*---(dep_root)----------*/
   tDEP_ROOT  *rroot;
   tDEP_ROOT  *rhead;
   tDEP_ROOT  *rtail;
   int         rcount;
   /*---(dep_link)----------*/
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




/*---(malloc)---------------------*/
char        ycalc__deps_new         (tDEP_LINK **a_dep);
char        ycalc__deps_free        (tDEP_LINK **a_dep);
/*---(program)--------------------*/
char        ycalc_deps_init         (void);
char        ycalc__deps_purge       (void);
char        ycalc_deps_wrap         (void);
char*       ycalc__unit_deps        (char *a_question, void *a_point);

char        ycalc__deps_rooting     (tDEP_ROOT *a_curr, char a_type);
char        ycalc__deps_circle      (int a_level, tDEP_ROOT *a_source, tDEP_ROOT *a_target, long a_stamp);


char        ycalc__audit_disp_reqs  (tDEP_ROOT *a_me, char* a_list);
char        ycalc__audit_disp_pros  (tDEP_ROOT *a_me, char* a_list);
char        ycalc__audit_disp_like  (tDEP_ROOT *a_me, char* a_list);



char        ycalc_trouble_clear     (void);
char        ycalc__unit_quiet       (void);
char        ycalc__unit_loud        (void);
char        ycalc__unit_end         (void);




/*===[ BUILD ]============================================*/
/*---(program)------------------------*/
char        ycalc_build_init        (void);



/*===[ EXEC ]=============================================*/
/*---(program)------------------------*/
char        ycalc_exec_init         (void);
/*---(pushing)------------------------*/
char        ycalc_pushstr           (char *a_func, char   *a_string);
char        ycalc_pushval           (char *a_func, double  a_value);
char        ycalc_pushref           (char *a_func, void   *a_thing);
/*---(popping)------------------------*/
double      ycalc_popval            (char *a_func);
char*       ycalc_popstr            (char *a_func);
/*---(unittest)-----------------------*/
char*       ycalc__unit_stack       (char *a_question, int a_num);
char        ycalc__unit_stackset    (int   a_num);


/*---(mock overall)-------------------*/
char        ycalc__mock_enable      (void);
/*---(mock build)---------------------*/
void*       ycalc__mock_deproot     (char *a_label);
char*       ycalc__mock_labeler     (void *a_thing);
char        ycalc__mock_reaper      (void *a_thing);
void*       ycalc__mock_whois       (int x, int y, int z);
/*---(mock execute)-------------------*/
char        ycalc__mock_valuer      (void *a_thing, char *a_type, double *a_value, char **a_string);
char        ycalc__mock_addresser   (void *a_thing, int  *x, int *y, int *z);
char        ycalc__mock_detailer    (void *a_thing, char *a_quality, char *a_string, double *a_value);





/*---(placeholder)--------------------*/
void        ycalc_noop              (void);
/*---(mathmatical)--------------------*/
void        ycalc_add               (void);
void        ycalc_subtract          (void);
void        ycalc_multiply          (void);
void        ycalc_divide            (void);
void        ycalc_modulus           (void);
void        ycalc_increment         (void);
void        ycalc_decrement         (void);
void        ycalc_unaryminus        (void);
/*---(mathmatical)--------------------*/
void        ycalc_power             (void);
void        ycalc_abs               (void);
void        ycalc_trunc             (void);
void        ycalc_rtrunc            (void);
void        ycalc_round             (void);
void        ycalc_rround            (void);
void        ycalc_ceiling           (void);
void        ycalc_floor             (void);
void        ycalc_sqrt              (void);
void        ycalc_cbrt              (void);
void        ycalc_sqr               (void);
void        ycalc_cube              (void);
void        ycalc_rand              (void);
void        ycalc_randr             (void);
/*---(num relational)-----------------*/
void        ycalc_equal             (void);
void        ycalc_notequal          (void);
void        ycalc_greater           (void);
void        ycalc_lesser            (void);
void        ycalc_gequal            (void);
void        ycalc_lequal            (void);
/*---(str relational)-----------------*/
void        ycalc_sequal            (void);
void        ycalc_snotequal         (void);
void        ycalc_sgreater          (void);
void        ycalc_slesser           (void);
/*---(logical)------------------------*/
void        ycalc_not               (void);
void        ycalc_and               (void);
void        ycalc_or                (void);



#endif
