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
#include    <yRPN.h>
#include    <yLOG.h>                    /* heatherly program logger            */
#include    "yCALC.h"



/* rapidly evolving version number to aid with visual change confirmation     */
#define YCALC_VER_NUM   "0.3b"
#define YCALC_VER_TXT   "lppad and rppad are now working"

/*---(string lengths)-----------------*/
#define     LEN_LABEL   20
#define     LEN_STR     200
#define     LEN_RECD    2000

#define     TRUE        1
#define     FALSE       0


/*---(label config)-------------------*/
extern char    (*g_who_named)   (char *a_label, char a_force, void **a_owner, void **a_deproot);        /* pass label of thing, get back deproot of thing  */
extern char    (*g_who_at   )   (int x, int y, int z, char a_force, void **a_owner, void **a_deproot);  /* pass coordinates, get back deproot of thing     */
extern char*   (*g_labeler  )   (void *a_owner);        /* pass deproot->owner, get back label of thing    */
/*---(struct config)-----------------*/
extern char    (*g_enabler  )   (void *a_owner, void *a_deproot);
extern char    (*g_pointer  )   (void *a_owner, char **a_source, char **a_type, double **a_value , char **a_string);
extern char    (*g_reaper   )   (void **a_owner);        /* pass deproot->owner, tries to kill thing        */
/*---(value config)-------------------*/
extern char    (*g_valuer   )   (void *a_owner, char *a_type, double *a_value , char **a_string);
extern char    (*g_addresser)   (void *a_owner, int *x, int *y, int *z);
extern char    (*g_special  )   (void *a_owner, char  a_what, double *a_value , char   **a_string);
extern char    (*g_printer  )   (void *a_owner);
/*---(sequencing)---------------------*/
extern char    (*g_consumer )   (void *a_owner, void *a_deproot, int a_seq, int a_lvl);



typedef     struct      cCALC       tCALC;         /* cell calculation entry    */
typedef     struct      cTERMS      tTERMS;
typedef     struct      cFUNCS      tFUNCS;
typedef     struct      cFCAT       tFCAT;
typedef     struct      cDEP_LINK   tDEP_LINK;
typedef     struct      cDEP_ROOT   tDEP_ROOT;
typedef     struct      cDEP_INFO   tDEP_INFO;




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
struct cCALC {
   /*---(owner)-------------*/
   void       *deproot;         /* pointer to the cell that owns this calc    */
   /*---(contents)----------*/
   char        t;               /* type of calculation element                */
   double      v;               /* numeric literal                            */
   char       *s;               /* string literal                             */
   tDEP_ROOT  *r;               /* reference pointer                          */
   void      (*f) (void);       /* function pointer                           */
   /*---(linked-list)-------*/
   tCALC      *next;            /* pointer to next calc                       */
   tCALC      *prev;            /* pointer to next calc                       */
   /*---(done)--------------*/
};

#define     MAX_TERM     20
struct cTERMS {
   char        type;
   char       *abbr;
   char       *desc;
};
extern tTERMS      s_terms [MAX_TERM];



extern int  g_nfunc;
extern int  g_ndups;
#define     MAX_FUNCS       1000
struct  cFUNCS {
   char        name        [20];       /* operator symbol/name                */
   char        len;                    /* length of name                      */
   void   (*f) (void);                 /* function pointer                    */
   char        type;                   /* type (func, op, or const)           */
   char        terms       [10];       /* number of terms                     */
   char        fcat;                   /* category                            */
   char        desc        [60];       /* descriptive label                   */
};
extern const tFUNCS  g_ycalc_funcs [MAX_FUNCS];


#define     MAX_FCAT          50
struct cFCAT {
   char        fcat;
   char        desc        [LEN_DESC];
};
extern const tFCAT s_fcats  [MAX_FCAT];








extern char   ycalc__unit_answer [LEN_STR ];



extern double      a, b, c, d, e;
extern int         m, n, o, len;
extern char       *q, *r, *s;
extern char        t           [LEN_RECD];
extern char        g_nada      [5];

#define  MAX   1000000000



/*===[[ DEPENDENCIES ]]=======================================================*/



/*---(dependency root)------------------------------------*/
/*
 *  dependency roots are linked to each calculatable host node/vertex and
 *  provide everything needed to get the calculation job done.
 *
 */
struct      cDEP_ROOT {
   /*---(tie to owner)------*/
   void       *owner;
   int         range;
   /*---(calculation)-------*/
   char       *rpn;          /* rpn version of formula                        */
   int         ncalc;        /* number of calculation tokens                  */
   tCALC      *chead;        /* pointer to head of calculation chain          */
   tCALC      *ctail;        /* pointer to tail of calculation chain          */
   int         cuse;         /* number of times calculated                    */
   /*---(dependencies)------*/
   int         nreq;         /* number of required cells                      */
   tDEP_LINK  *reqs;         /* incomming predesesors to this calc            */
   int         npro;         /* number of dependent cells                     */
   tDEP_LINK  *pros;         /* outgoing successors to this calc              */
   /*---(sequencing)--------*/
   int         slevel;       /* sequence level                                */
   tDEP_ROOT  *snext;        /* next in sequence level                        */
   tDEP_ROOT  *sprev;        /* prev in sequence level                        */
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
   int         count;         /* reuse with same source and target            */
   /*---(cell linked list)---------------*/
   tDEP_LINK  *prev;          /* pointer to prev dependency for source cell   */
   tDEP_LINK  *next;          /* pointer to next dependency for source cell   */
   tDEP_LINK  *match;         /* pointer to matching dependency in ther dir  */
   /*---(deps linked list)---------------*/
   tDEP_LINK  *dprev;         /* pointer to prev dependency in full list      */
   tDEP_LINK  *dnext;         /* pointer to next dependency in full list      */
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
#define     G_DEP_DIRREQ         '>'
#define     G_DEP_DIRPRO         '<'

extern char S_DEP_REQS [10];
extern char S_DEP_PROS [10];
extern char S_DEP_LIKE [10];



typedef struct cMOCK  tMOCK;
struct  cMOCK {
   /*---(main)------------*/
   char       *label;
   char        type;
   char       *source;
   double      value;
   char       *string;
   /*---(printable)-------*/
   int         width;
   char        align;
   char        format;
   int         decs;
   char       *print;
   /*---(ycalc)-----------*/
   void       *ycalc;
   /*---(linking)---------*/
   tMOCK      *next;
   tMOCK      *prev;
   /*---(done)------------*/
};
extern tMOCK   s_mocks     [500];

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
   /*---(mock)--------------*/
   tMOCK      *mroot;
   tMOCK      *mhead;
   tMOCK      *mtail;
   int         mcount;
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
   void       *owner;
   void       *deproot;
   /*---(done)--------------*/
};
extern  tLOCAL myCALC;



#define   YCALC_MAX_ERROR       100
typedef   struct cyCALC_ERROR  tyCALC_ERROR;
struct cyCALC_ERROR {
   char        abbr;
   char        phase;
   char        terse       [LEN_LABEL];
   char        desc        [LEN_DESC ];
};
extern const tyCALC_ERROR   zCALC_errors     [YCALC_MAX_ERROR];


#define     G_NO_ERROR         '-'
#define     G_ERROR_RANGE      'R'

#define     YCALC_ERROR_CONF       'C'
#define     YCALC_ERROR_STACK      's'
#define     YCALC_ERROR_BUILD_RPN  'r'
#define     YCALC_ERROR_BUILD_REF  '@'
#define     YCALC_ERROR_BUILD_DEP  'd'
#define     YCALC_ERROR_BUILD_CIR  'c'
#define     YCALC_ERROR_BUILD_PNT  '&'
#define     YCALC_ERROR_BUILD_RNG  ':'
#define     YCALC_ERROR_BUILD_TOK  '?'

#define     YCALC_ERROR_EXEC_PTR   '*'
#define     YCALC_ERROR_EXEC_FMT   'F'
#define     YCALC_ERROR_EXEC_DATE  'D'
#define     YCALC_ERROR_EXEC_BRNG  '<'
#define     YCALC_ERROR_EXEC_ERNG  '>'
#define     YCALC_ERROR_EXEC_MISS  '£'

#define     YCALC_ERROR_UNKNOWN    'U'

extern int       g_error;




#define     G_TYPE_EMPTY        '-'
#define     G_TYPE_VAL          'v'
#define     G_TYPE_STR          's'
#define     G_TYPE_REF          'r'
#define     G_TYPE_FUNC         'f'
#define     G_TYPE_NOOP         'x'




#define       G_SPECIAL_RPN      'R'
#define       G_SPECIAL_NCALC    '0'
#define       G_SPECIAL_PROS     'p'
#define       G_SPECIAL_NPRO     '1'
#define       G_SPECIAL_REQS     'r'
#define       G_SPECIAL_NREQ     '2'
#define       G_SPECIAL_LIKE     'l'
#define       G_SPECIAL_LEVEL    's'
#define       G_SPECIAL_XPOS     'X'
#define       G_SPECIAL_YPOS     'Y'
#define       G_SPECIAL_ZPOS     'Z'
#define       G_SPECIAL_ALLPOS   '@'




typedef   unsigned long long   ullong;
extern long   time_zone;



/*===[[ OBJECT TYPES ]]=======================================================*/
#define   YCALC_MAX_TYPE    15
typedef struct cyCALC_TYPES  tyCALC_TYPES;
struct cyCALC_TYPES {
   char        type;                        /* cell type                      */
   char        terse       [LEN_LABEL];     /* short description of cell type */
   char        prefix;                      /* prefix in cell source string   */
   char        rpn;                         /* require processing by rpn      */
   char        calc;                        /* must it be calculated          */
   char        deps;                        /* must follow the dependencies   */
   char        result;                      /* what type is the result        */
   char        desc        [LEN_DESC ];     /* description of cell type       */
};
extern const   tyCALC_TYPES  g_ycalc_types [YCALC_MAX_TYPE];




/*===[[ FUNCTION PROTOTYPES ]]================================================*/


char        ycalc_not_ready         (void);
char        ycalc_handle_error      (char a_error, char *a_type, double *a_value, char **a_string, char *a_note);


/*---(malloc)-------------------------*/
char        ycalc__deps_new         (tDEP_LINK **a_dep);
char        ycalc__deps_free        (tDEP_LINK **a_dep);
/*---(program)------------------------*/
char        ycalc_deps_init         (void);
char        ycalc__deps_purge       (void);
char        ycalc_deps_wrap         (void);
char*       ycalc__unit_deps        (char *a_question, char *a_label);

char        ycalc_deps_create       (char a_type, tDEP_ROOT **a_source, tDEP_ROOT **a_target);
char        ycalc_deps_delete       (char a_type, tDEP_ROOT **a_source, tDEP_ROOT **a_target, void **a_owner);
char        ycalc_deps_delcalcref   (tDEP_ROOT *a_deproot);
char        ycalc_deps_wipe_reqs    (void **a_owner, tDEP_ROOT **a_deproot);

char        ycalc__deps_rooting     (tDEP_ROOT *a_curr, char a_type);
char        ycalc__deps_circle      (int a_level, tDEP_ROOT *a_source, tDEP_ROOT *a_target, long a_stamp);

/*===[[ AUDIT ]]==========================================*/
/*---(classify)-----------------------*/
char        ycalc_shared_verify     (char **a_source, char *a_type, double *a_value, char **a_string);
char        ycalc_classify_clear    (void **a_owner, tDEP_ROOT **a_deproot, char *a_type, double *a_value, char **a_string);
char        ycalc_classify_trusted  (void **a_owner, tDEP_ROOT **a_deproot, char **a_source, char *a_type, double *a_value, char **a_string);
char        ycalc_classify_detail   (void **a_owner, tDEP_ROOT **a_deproot, char **a_source, char *a_type, double *a_value, char **a_string);
char        ycalc_classify_owner    (void **a_owner, tDEP_ROOT **a_deproot);
char        ycalc_classify_label    (char *a_label);

char        ycalc_audit_init        (void);
char        ycalc__audit_disp_reqs  (tDEP_ROOT *a_me, char* a_list);
char        ycalc__audit_disp_pros  (tDEP_ROOT *a_me, char* a_list);
char        ycalc__audit_disp_like  (tDEP_ROOT *a_me, char* a_list);


char        ycalc_merge_count       (tDEP_ROOT *a_deproot);


char        ycalc_trouble_clear     (void);
char        ycalc__unit_quiet       (void);
char        ycalc__unit_loud        (void);
char        ycalc__unit_end         (void);


int         ycalc_range_init        (void);
char*       ycalc_range_label       (int n);
int         ycalc_range_nonrange    (tDEP_ROOT *a_deproot);
char        ycalc_range_delete      (tDEP_ROOT *a_deproot, tDEP_ROOT *a_range);
char        ycalc_range_unhook      (void **a_owner, tDEP_ROOT **a_deproot);
char        ycalc_range_deproot     (char *a_name, tDEP_ROOT **a_deproot);
char        ycalc_range_use         (tDEP_ROOT  *a_src, int bx, int ex, int by, int ey, int bz, int ez, tDEP_ROOT **a_range);
char        ycalc_range_include     (tDEP_ROOT **a_src, int x, int y, int z);


/*===[ BUILD ]============================================*/
/*---(program)------------------------*/
char        ycalc_build_init        (void);

char        ycalc_build_trusted     (tDEP_ROOT *a_deproot, char **a_source, char *a_type, double *a_value, char **a_string);
char        ycalc_build_detail      (void *a_owner, tDEP_ROOT *a_deproot, char **a_source, char *a_type, double *a_value, char **a_string);
char        ycalc_build_owner       (void *a_owner, tDEP_ROOT *a_deproot);
char        ycalc_build_label       (char *a_label);



char        ycalc_calc_wipe         (tDEP_ROOT *a_deproot);



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
/*---(running)------------------------*/
char        ycalc_execute_trusted   (tDEP_ROOT *a_deproot, char *a_type, double *a_value, char **a_string);
char        ycalc_execute_detail    (tDEP_ROOT *a_deproot, char **a_source, char *a_type, double *a_value, char **a_string);
char        ycalc_execute_owner     (void *a_owner, tDEP_ROOT *a_deproot);
char        ycalc_execute_label     (char *a_label);
char        ycalc_execute_auto      (void *a_owner, tDEP_ROOT *a_deproot, int a_seq, int a_lvl);
/*---(unittest)-----------------------*/
char*       ycalc__unit_stack       (char *a_question, int a_num);
char        ycalc__unit_stackset    (int   a_num);


/*---(mock overall)-------------------*/
char        ycalc__mock_prepare     (void);
char        ycalc__mock_cleanup     (void);
/*---(exist)--------------------------*/
char        ycalc__mock_enabler     (void *a_owner, void *a_deproot);
char        ycalc__mock_pointer     (void *a_owner, char **a_source, char **a_type, double **a_value, char **a_string);
char        ycalc__mock_reaper      (void **a_owner);
/*---(label)--------------------------*/
char        ycalc__mock_named       (char *a_label      , char a_force, void **a_owner, void **a_deproot);
char        ycalc__mock_whos_at     (int x, int y, int z, char a_force, void **a_owner, void **a_deproot);
char*       ycalc__mock_labeler     (void *a_owner);
/*---(value)--------------------------*/
char        ycalc__mock_valuer      (void *a_thing, char *a_type, double *a_value, char **a_string);
char        ycalc__mock_address     (void *a_thing, int  *x, int *y, int *z);
char        ycalc__mock_special     (void *a_owner, char a_what, double *a_value, char **a_string);
char        ycalc__mock_printer     (void *a_owner);
/*---(unit testing)-------------------*/
char        ycalc__mock_special_set (char *a_label, char *a_source, double a_value, char *a_print);
char        ycalc__mock_source      (char *a_label, char *a_source);
char        ycalc__mock_whole       (char *a_label, char *a_source, char a_format, char a_decs, char a_aign, char a_width);
char*       ycalc__unit_mock        (char *a_question, char *a_label);



/*===[ SEQ ]==============================================*/
char        ycalc__seq_clear        (void);
char        ycalc__seq_add          (char a_level, tDEP_ROOT *a_deproot);
char        ycalc__seq_del          (tDEP_ROOT *a_deproot);
char        ycalc__seq_recursion    (int a_level, tDEP_LINK *a_dep, char a_dir, long a_stamp);;
char        ycalc__seq_driver       (tDEP_ROOT *a_deproot, char a_dir_rec, char a_dir_act, long a_stamp, void *g_consumer);
char        ycalc__seq_list         (char *a_list);



/*===[ CALLS ]============================================*/
char        ycalc_call_reaper       (void **a_owner, tDEP_ROOT **a_deproot);
char        ycalc_call_who_named    (char *a_label,       char a_force, void **a_owner, void **a_deproot);
char        ycalc_call_who_at       (int x, int y, int z, char a_force, void **a_owner, void **a_deproot);
char*       ycalc_call_labeler      (tDEP_ROOT *a_deproot);


char        ycalc_trig_init         (void);



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
void        ycalc_nand              (void);
void        ycalc_nor               (void);
void        ycalc_xor               (void);
void        ycalc_if                (void);
void        ycalc_ifs               (void);
void        ycalc_within            (void);
void        ycalc_approx            (void);
void        ycalc_concat            (void);
void        ycalc_concatplus        (void);
/*---(string functions)----------------*/
void        ycalc_len               (void);
void        ycalc_left              (void);
void        ycalc_right             (void);
void        ycalc_mid               (void);
void        ycalc_trim              (void);
void        ycalc_ltrim             (void);
void        ycalc_rtrim             (void);
void        ycalc_strim             (void);
void        ycalc_etrim             (void);
void        ycalc_mtrim             (void);
void        ycalc_printstr          (void);
void        ycalc_printnum          (void);
void        ycalc_lpad              (void);
void        ycalc_rpad              (void);
void        ycalc_lppad             (void);
void        ycalc_rppad             (void);
void        ycalc_find              (void);
void        ycalc_replace           (void);
/*---(conversion functions)------------*/
void        ycalc_lower             (void);
void        ycalc_upper             (void);
void        ycalc_char              (void);
void        ycalc_code              (void);
void        ycalc_value             (void);
void        ycalc_salpha            (void);
void        ycalc_salphac           (void);
void        ycalc_salnum            (void);
void        ycalc_salnumc           (void);
void        ycalc_sbasic            (void);
void        ycalc_sbasicc           (void);
void        ycalc_swrite            (void);
void        ycalc_swritec           (void);
void        ycalc_sexten            (void);
void        ycalc_sextenc           (void);
void        ycalc_sprint            (void);
void        ycalc_sprintc           (void);
void        ycalc_sseven            (void);
void        ycalc_ssevenc           (void);
/*---(object audit functions)----------*/
void        ycalc_type              (void);
void        ycalc_isblank           (void);
void        ycalc_isvalue           (void);
void        ycalc_istext            (void);
void        ycalc_isnum             (void);
void        ycalc_isfor             (void);
void        ycalc_isstr             (void);
void        ycalc_ismod             (void);
void        ycalc_iscalc            (void);
void        ycalc_islit             (void);
void        ycalc_ispoint           (void);
void        ycalc_iserror           (void);
void        ycalc_xpos              (void);
void        ycalc_ypos              (void);
void        ycalc_zpos              (void);
void        ycalc_me                (void);
void        ycalc_label             (void);
void        ycalc_tab               (void);
void        ycalc_col               (void);
void        ycalc_row               (void);
void        ycalc_formula           (void);
void        ycalc_rpn               (void);
void        ycalc_ncalc             (void);
void        ycalc_reqs              (void);
void        ycalc_nreq              (void);
void        ycalc_pros              (void);
void        ycalc_npro              (void);
void        ycalc_level             (void);
void        ycalc_pointer           (void);
/*---(trig functions)------------------*/
void        ycalc_radians           (void);
void        ycalc_degrees           (void);
void        ycalc_pi                (void);
void        ycalc_hypot             (void);
void        ycalc_side              (void);
/*------*/
void        ycalc_sin               (void);
void        ycalc_sinr              (void);
void        ycalc_cos               (void);
void        ycalc_cosr              (void);
void        ycalc_tan               (void);
void        ycalc_tanr              (void);
void        ycalc_cot               (void);
void        ycalc_cotr              (void);
void        ycalc_sec               (void);
void        ycalc_secr              (void);
void        ycalc_csc               (void);
void        ycalc_cscr              (void);
void        ycalc_xsec              (void);
void        ycalc_xsecr             (void);
void        ycalc_xcsc              (void);
void        ycalc_xcscr             (void);
/*------*/
void        ycalc_vsin              (void);
void        ycalc_vsinr             (void);
void        ycalc_vcos              (void);
void        ycalc_vcosr             (void);
void        ycalc_csin              (void);
void        ycalc_csinr             (void);
void        ycalc_ccos              (void);
void        ycalc_ccosr             (void);
void        ycalc_hvsin             (void);
void        ycalc_hvsinr            (void);
void        ycalc_hvcos             (void);
void        ycalc_hvcosr            (void);
void        ycalc_hcsin             (void);
void        ycalc_hcsinr            (void);
void        ycalc_hccos             (void);
void        ycalc_hccosr            (void);
/*------*/
void        ycalc_crd               (void);
void        ycalc_crdr              (void);
void        ycalc_sag               (void);
void        ycalc_sagr              (void);
void        ycalc_apo               (void);
void        ycalc_apor              (void);
void        ycalc_csg               (void);
void        ycalc_csgr              (void);
void        ycalc_hcrd              (void);
void        ycalc_hcrdr             (void);
void        ycalc_scrd              (void);
void        ycalc_scrdr             (void);
void        ycalc_vcrd              (void);
void        ycalc_vcrdr             (void);
void        ycalc_hvcrd             (void);
void        ycalc_hvcrdr            (void);
void        ycalc_asin              (void);
void        ycalc_asinr             (void);
void        ycalc_acos              (void);
void        ycalc_acosr             (void);
void        ycalc_atan              (void);
void        ycalc_atanr             (void);
void        ycalc_atan2             (void);
void        ycalc_atanr2            (void);

void        ycalc_dist              (void);
void        ycalc_tabs              (void);
void        ycalc_cols              (void);
void        ycalc_rows              (void);
void        ycalc_sum               (void);
void        ycalc_every             (void);
void        ycalc_filled            (void);
void        ycalc_numbers           (void);
void        ycalc_strings           (void);
void        ycalc_blanks            (void);
void        ycalc_pointers          (void);
void        ycalc_empty             (void);
void        ycalc_calcs             (void);
void        ycalc_min               (void);
void        ycalc_max               (void);
void        ycalc_range             (void);
void        ycalc_average           (void);
void        ycalc_quarter1          (void);
void        ycalc_median            (void);
void        ycalc_quarter3          (void);
void        ycalc_rangeq            (void);
void        ycalc_mode              (void);
void        ycalc_stddev            (void);
void        ycalc_skew              (void);

void        ycalc_rel_x             (void);
void        ycalc_rel_y             (void);
void        ycalc_rel_z             (void);
void        ycalc_rel_xy            (void);
void        ycalc_rel_xyz           (void);
void        ycalc_abs_x             (void);
void        ycalc_abs_y             (void);
void        ycalc_abs_z             (void);
void        ycalc_abs_xy            (void);
void        ycalc_abs_xyz           (void);
void        ycalc_off_x             (void);
void        ycalc_off_y             (void);
void        ycalc_off_z             (void);
void        ycalc_off_xy            (void);
void        ycalc_off_xyz           (void);
void        ycalc_address           (void);

void        ycalc_vlookup           (void);
void        ycalc_hlookup           (void);
void        ycalc_entry             (void);
void        ycalc_index             (void);

void        ycalc_now               (void);
void        ycalc_now               (void);
void        ycalc_year              (void);
void        ycalc_month             (void);
void        ycalc_day               (void);
void        ycalc_hour              (void);
void        ycalc_minute            (void);
void        ycalc_second            (void);
void        ycalc_weekday           (void);
void        ycalc_weeknum           (void);
void        ycalc_daynum            (void);
void        ycalc_timevalue         (void);
void        ycalc_timevalue         (void);
void        ycalc_date              (void);
void        ycalc_time              (void);
void        ycalc_datepart          (void);
void        ycalc_timepart          (void);
void        ycalc_timezonesec       (void);

#endif
