/*===[[ HEADER GUARD ]]=======================================================*/
#ifndef YCALC_priv_hguard
#define YCALC_priv_hguard loaded



/*===[[ HEADER BEG ]]=========================================================*/
/*                      ´·········1·········2·········3·········4·········5·········6·········7*/
/*--------- 12345678901 ´123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/
/*········· ··········· ´·····························´········································*/
#define     P_NAME      "yCALC"
/*········· ··········· ´·····························´········································*/
#define     P_FOCUS     "RS (run-time support)"
#define     P_NICHE     "ca (calculation)"
#define     P_SUBJECT   "calculation engine"
#define     P_PURPOSE   "fast, powerful, flexible, shared calculation engine"
/*········· ··········· ´·····························´········································*/
#define     P_NAMESAKE  "kottus-hecatoncheires (hundred-handed)"
#define     P_PRONOUNCE ""
#define     P_HERITAGE  "kottus, big-limbed, is one of the three hecatoncheires" 
#define     P_BRIEFLY   "one-hundred handed monster"
#define     P_IMAGERY   "ugly, impossibly powerful, one-hundred handed, fifty headed giant"
#define     P_REASON    ""
/*········· ··········· ´·····························´········································*/
#define     P_ONELINE   P_NAMESAKE " " P_SUBJECT
/*········· ··········· ´·····························´········································*/
#define     P_HOMEDIR   "/home/system/yCALC.calculation_engine"
#define     P_BASENAME  "ylibCALC.so"
#define     P_FULLPATH  "/usr/local/lib64/libyCALC.so"
#define     P_SUFFIX    "···"
#define     P_CONTENT   "···"
/*········· ··········· ´·····························´········································*/
#define     P_SYSTEM    "gnu/linux   (powerful, ubiquitous, technical, and hackable)"
#define     P_LANGUAGE  "ansi-c      (wicked, limitless, universal, and everlasting)"
#define     P_COMPILER  "gcc 11.3.0"
#define     P_CODESIZE  "large       (appoximately 10,000 slocl)"
/*········· ··········· ´·····························´········································*/
#define     P_DEPSTDC   "stdio,stdlib,string,math,time.malloc"
#define     P_DEPPOSIX  ""
#define     P_DEPCORE   "yURG,yLOG,ySTR"
#define     P_DEPVIKEYS ""
#define     P_DEPOTHER  "yRPN"
#define     P_DEPGRAPH  ""
#define     P_DEPHEAD   ""
/*········· ··········· ´·····························´········································*/
#define     P_AUTHOR    "heatherlyrobert"
#define     P_CREATED   "2014-10"
/*········· ··········· ´·····························´········································*/
#define     P_VERMAJOR  "0.X = reading for full gyges use"
#define     P_VERMINOR  "0.5 = find pernicious memory and malloc troubles"
#define     P_VERNUM    "0.5o"
#define     P_VERTXT    "added scientific notation operators Ë, Ë-, and Ë+"
/*········· ··········· ´·····························´········································*/
#define     P_PRIORITY  "direct, simple, brief, vigorous, and lucid (h.w. fowler)"
#define     P_PRINCIPAL "[grow a set] and build your wings on the way down (r. bradbury)"
#define     P_REMINDER  "there are many better options, but i *own* every byte of this one"
/*········· ··········· ´·····························´········································*/
/*--------- 12345678901 ´123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/
/*                      ´·········1·········2·········3·········4·········5·········6·········7*/
/*===[[ HEADER END ]]=========================================================*/



#include    <stdio.h>
#include    <stdlib.h>                  /* getenv()                            */
#include    <string.h>
#include    <math.h>  
#include    <time.h>         /* C_ANSI : time, strftime, localtime            */

#include    <yLOG.h>                    /* heatherly program logger            */
#include    <yURG.h>                    /* heatherly program logger            */
#include    <ySTR.h>
#include    <yRPN.h>
#include    "yCALC.h"




#define     TRUE        1
#define     FALSE       0




typedef     struct      cCALC       tCALC;         /* cell calculation entry    */
typedef     struct      cFUNCS      tFUNCS;
typedef     struct      cVARS       tVARS;
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


static struct cVARS {
   /*---(master)------------*/
   char        kind;
   char        name        [LEN_LABEL];
   char        title       [LEN_LABEL];
   char        content     [LEN_LABEL];
   /*---(linked-list)-------*/
   tVARS      *next;            /* pointer to next calc                       */
   tVARS      *prev;            /* pointer to next calc                       */
   /*---(done)--------------*/
};






extern char   ycalc__unit_answer [LEN_RECD];



extern double      a, b, c, d, e;
extern int         s_buf, m, n, o, len;
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
   void       *owner;        /* back pointer to owning object                 */
   int         range;        /* if assigned as a range                        */
   char        btype;        /* build type, before execution troubles         */
   /*---(calculation)-------*/
   int         nrpn;         /* number of rnp tokens                          */
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
/*> extern static tDEP_LINK     *s_hdep;                                              <*/
/*> extern static tDEP_LINK     *s_tdep;                                              <*/
/*> extern static int       s_ndep;                                                   <*/

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
extern tDEP_INFO   g_dep_info [MAX_DEPTYPE];

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
   char        state;                  /* '-' normal, 'L' file load, ...      */
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
   char       *me;
   char       *label;
   /*---(label config)-------------------*/
   char      (*e_who_named)   (char *a_label, char a_force, void **a_owner, void **a_deproot);        /* pass label of thing, get back deproot of thing  */
   char      (*e_who_at   )   (int b, int x, int y, int z, char a_force, void **a_owner, void **a_deproot);  /* pass coordinates, get back deproot of thing     */
   char*     (*e_labeler  )   (void *a_owner);        /* pass deproot->owner, get back label of thing    */
   /*---(struct config)-----------------*/
   char      (*e_enabler  )   (void *a_owner, void *a_deproot);
   char      (*e_pointer  )   (void *a_owner, char **a_source, char **a_type, double **a_value , char **a_string);
   char      (*e_reaper   )   (void **a_owner);        /* pass deproot->owner, tries to kill thing        */
   /*---(value config)-------------------*/
   char      (*e_valuer   )   (void *a_owner, char *a_type, double *a_value , char **a_string);
   char      (*e_addresser)   (void *a_owner, int *b, int *x, int *y, int *z);
   char      (*e_special  )   (void *a_owner, char  a_what, double *a_value , char   **a_string);
   char      (*e_printer  )   (void *a_owner);
   /*---(sequencing)---------------------*/
   char      (*e_consumer )   (void *a_owner, void *a_deproot, int a_seq, int a_lvl);
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


#define     YCALC_ERROR_NONE       NULL
#define     YCALC_ERROR_LITERAL    0x1
#define     YCALC_ERROR_UNKNOWN    0x2
#define     YCALC_ERROR_ARG1       0x3
#define     YCALC_ERROR_ARG2       0x4
#define     YCALC_ERROR_ARG3       0x5
#define     YCALC_ERROR_ARG4       0x6
#define     YCALC_ERROR_INTERN     0x7

#define     YCALC_ERROR_CONF       'C'
#define     YCALC_ERROR_STACK      's'

#define     YCALC_ERROR_BUILD_RPN  'r'
#define     YCALC_ERROR_BUILD_REF  '@'
#define     YCALC_ERROR_BUILD_DEP  'd'
#define     YCALC_ERROR_BUILD_LIK  '~'
#define     YCALC_ERROR_BUILD_PNT  '&'
#define     YCALC_ERROR_BUILD_RNG  ':'
#define     YCALC_ERROR_BUILD_TOK  '?'
#define     YCALC_ERROR_BUILD_DUP  '"'
#define     YCALC_ERROR_BUILD_FNC  'x'
#define     YCALC_ERROR_BUILD_ROO  '['

#define     YCALC_ERROR_EXEC_STEP  '-'
#define     YCALC_ERROR_EXEC_STR   'S'
#define     YCALC_ERROR_EXEC_VAL   'V'
#define     YCALC_ERROR_EXEC_REF   'I'
#define     YCALC_ERROR_EXEC_VAR   'v'
#define     YCALC_ERROR_EXEC_PTR   '*'
#define     YCALC_ERROR_EXEC_IND   '&'
#define     YCALC_ERROR_EXEC_MAL   'M'
#define     YCALC_ERROR_EXEC_PERR  'P'
#define     YCALC_ERROR_EXEC_NULL  'n'
#define     YCALC_ERROR_EXEC_CIR   'O'
#define     YCALC_ERROR_EXEC_ARG   'A'
#define     YCALC_ERROR_EXEC_FMT   'F'
#define     YCALC_ERROR_EXEC_DATE  'D'
#define     YCALC_ERROR_EXEC_BRNG  '<'
#define     YCALC_ERROR_EXEC_ERNG  '>'
#define     YCALC_ERROR_EXEC_MISS  '£'
#define     YCALC_ERROR_EXEC_NADA  'N'
#define     YCALC_ERROR_EXEC_ERR   'E'
#define     YCALC_ERROR_EXEC_HUH   '¢'
#define     YCALC_ERROR_EXEC_OPT   'o'

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
#define       G_SPECIAL_UPOS     'B'
#define       G_SPECIAL_XPOS     'X'
#define       G_SPECIAL_YPOS     'Y'
#define       G_SPECIAL_ZPOS     'Z'
#define       G_SPECIAL_ALLPOS   '@'




typedef   unsigned long long   ullong;
extern long   time_zone;



/*===[[ OBJECT TYPES ]]=======================================================*/
#define   YCALC_MAX_TYPE    25
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

char        ycalc_error_clear       (void);
char        ycalc_error_set         (int a_error, tDEP_ROOT *a_deproot);
char        ycalc_error_finalize    (char a_error, char *a_type, double *a_value, char **a_string, char *a_note);
char        ycalc_error_true        (void);


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
char        ycalc_deps_delvar       (tDEP_ROOT **a_deproot);
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


char        ycalc_range_init        (void);
char        ycalc_range_wrap        (void);
char*       ycalc_range_label       (int n);
int         ycalc_range_nonrange    (tDEP_ROOT *a_deproot);
char        ycalc_range_delete      (tDEP_ROOT *a_deproot, tDEP_ROOT *a_range);
char        ycalc_range_unhook      (void **a_owner, tDEP_ROOT **a_deproot);
char        ycalc_range_deproot     (char *a_name, tDEP_ROOT **a_deproot);
char        ycalc_range_use         (tDEP_ROOT  *a_src, int bb, int eb, int bx, int ex, int by, int ey, int bz, int ez, tDEP_ROOT **a_range);
char        ycalc_range_include     (tDEP_ROOT **a_src, int b, int x, int y, int z);


/*===[ BUILD ]============================================*/
/*---(program)------------------------*/
char        ycalc_build_init        (void);

char        ycalc_build_trusted     (tDEP_ROOT *a_deproot, char **a_source, char *a_type, double *a_value, char **a_string);
char        ycalc_build_detail      (void *a_owner, tDEP_ROOT *a_deproot, char **a_source, char *a_type, double *a_value, char **a_string);
char        ycalc_build_owner       (void *a_owner, tDEP_ROOT *a_deproot);
char        ycalc_build_label       (char *a_label);
char        ycalc_build_ripple      (tDEP_ROOT *a_deproot, char a_kind);
char        ycalc_build_variable    (tDEP_ROOT *a_deproot, char *a_label, short b, short x, short y, short z, char **a_source, char *a_type, double *a_value, char **a_string);
char        ycalc_build_findfunc    (char *a_token);



char        ycalc_calc_wipe         (tDEP_ROOT *a_deproot);



/*===[ EXEC ]=============================================*/
/*---(program)------------------------*/
char        ycalc_exec_init         (void);
char        ycalc_exec_wrap         (void);
/*---(pushing)------------------------*/
char        ycalc_pushstr           (char *a_func, char *a_string);
char        ycalc_pushval           (char *a_func, double a_value);
char        ycalc_pushref           (char *a_func, void *a_thing, char *a_label);
/*---(popping)------------------------*/
double      ycalc_popval            (char *a_func);
char*       ycalc_popstr            (char *a_func);
tDEP_ROOT*  ycalc_popref            (char *a_func);
int         ycalc_popval_plus       (char *a_func, char a_what);
char*       ycalc_popstr_plus       (char *a_func, char a_what);
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
char        ycalc__mock_lister      (void);
char        ycalc__mock_list        (void);
char        ycalc__mock_named       (char *a_label      , char a_force, void **a_owner, void **a_deproot);
char        ycalc__mock_whos_at     (int b, int x, int y, int z, char a_force, void **a_owner, void **a_deproot);
char*       ycalc__mock_labeler     (void *a_owner);
/*---(value)--------------------------*/
char        ycalc__mock_valuer      (void *a_thing, char *a_type, double *a_value, char **a_string);
char        ycalc__mock_address     (void *a_thing, int  *b, int  *x, int *y, int *z);
char        ycalc__mock_special     (void *a_owner, char a_what, double *a_value, char **a_string);
char        ycalc__mock_printer     (void *a_owner);
/*---(unit testing)-------------------*/
char        ycalc__mock_special_set (char *a_label, char *a_source, double a_value, char *a_print);
char        ycalc__mock_source      (char *a_label, char *a_source);
char        ycalc__mock_whole       (char *a_label, char *a_source, char a_format, char a_decs, char a_aign, char a_width);
char*       ycalc__unit_mock        (char *a_question, char *a_label);



/*===[ SEQ ]==============================================*/
char        ycalc__seq_clear        (void);
char        ycalc__seq_add          (short a_level, tDEP_ROOT *a_deproot);
char        ycalc__seq_del          (tDEP_ROOT *a_deproot);
char        ycalc__seq_recursion    (int a_level, tDEP_LINK *a_dep, char a_dir, long a_stamp);
char        ycalc__seq_driver       (tDEP_ROOT *a_deproot, char a_dir_rec, char a_dir_act, long a_stamp, void *a_consumer);
char        ycalc__seq_list         (char *a_list);
char*       ycalc__unit_seq         (char *a_question, void *a_point);


char        ycalc_vars_new          (char a_kind, char *a_name, char *a_label, tDEP_ROOT *a_deproot, char *a_type, double *a_value, char **a_string);
char        yCALC_variable          (char *a_name, char *a_real);
char        ycalc_vars__ripple      (char *a_label, tDEP_ROOT *a_deproot);
char        ycalc_vars__rem         (tVARS *a_cur);
char        ycalc_vars_del          (char *a_loc, tDEP_ROOT **a_deproot);
char        ycalc_vars_wrap         (void);

/*===[ CALLS ]============================================*/
char        ycalc_call_reaper       (void **a_owner, tDEP_ROOT **a_deproot);
char        ycalc_call_who_named    (char *a_label,       char a_force, void **a_owner, void **a_deproot);
char        ycalc_call_who_at       (int b, int x, int y, int z, char a_force, void **a_owner, void **a_deproot);
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
void        ycalc_scipos            (void);
void        ycalc_scineg            (void);
void        ycalc_power             (void);
void        ycalc_power_of_2        (void);
void        ycalc_power_of_3        (void);
void        ycalc_power_of_4        (void);
void        ycalc_power_of_x        (void);
void        ycalc_power_of_y        (void);
void        ycalc_abs               (void);
void        ycalc_sign              (void);
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
void        ycalc_nxor              (void);
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
void        ycalc_beg               (void);
void        ycalc_at                (void);
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
void        ycalc_ditto             (void);
/*---(conversion functions)------------*/
void        ycalc_lower             (void);
void        ycalc_upper             (void);
void        ycalc_char              (void);
void        ycalc_code              (void);
void        ycalc_value             (void);
void        ycalc_unhex             (void);
void        ycalc_hex               (void);
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
void        ycalc_upos              (void);
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
void        ycalc_fsag              (void);
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
void        ycalc_arc               (void);
void        ycalc_arcr              (void);
void        ycalc_los               (void);
void        ycalc_los2              (void);
void        ycalc_xsag              (void);
void        ycalc_xsag2             (void);
void        ycalc_asin              (void);
void        ycalc_asinr             (void);
void        ycalc_acos              (void);
void        ycalc_acosr             (void);
void        ycalc_atan              (void);
void        ycalc_atanr             (void);
void        ycalc_atan2             (void);
void        ycalc_atanr2            (void);
void        ycalc_acrd2             (void);

void        ycalc_dist              (void);
void        ycalc_tabs              (void);
void        ycalc_cols              (void);
void        ycalc_rows              (void);
void        ycalc_levels            (void);
void        ycalc_sum               (void);

void        ycalc_every             (void);
void        ycalc_used              (void);
void        ycalc_empties           (void);

void        ycalc_numbers           (void);
void        ycalc_nlit              (void);
void        ycalc_nform             (void);
void        ycalc_nlike             (void);

void        ycalc_strings           (void);
void        ycalc_slit              (void);
void        ycalc_sform             (void);
void        ycalc_slike             (void);

void        ycalc_pointers          (void);
void        ycalc_merges            (void);
void        ycalc_blanks            (void);
void        ycalc_errors            (void);
void        ycalc_unknowns          (void);

void        ycalc_literals          (void);
void        ycalc_forms             (void);
void        ycalc_likes             (void);
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

void        ycalc_ru                (void);
void        ycalc_rx                (void);
void        ycalc_ry                (void);
void        ycalc_rxy               (void);
void        ycalc_ruxy              (void);

void        ycalc_au                (void);
void        ycalc_ax                (void);
void        ycalc_ay                (void);
void        ycalc_axy               (void);
void        ycalc_auxy              (void);

void        ycalc_rel_b             (void);
void        ycalc_rel_x             (void);
void        ycalc_rel_y             (void);
void        ycalc_rel_xy            (void);
void        ycalc_rel_bxy           (void);

void        ycalc_abs_b             (void);
void        ycalc_abs_x             (void);
void        ycalc_abs_y             (void);
void        ycalc_abs_xy            (void);
void        ycalc_abs_bxy           (void);

void        ycalc_off_b             (void);
void        ycalc_off_x             (void);
void        ycalc_off_y             (void);
void        ycalc_off_xy            (void);
void        ycalc_off_bxy           (void);
void        ycalc_address           (void);

void        ycalc_vlookup           (void);
void        ycalc_vprefix           (void);
void        ycalc_vmatch            (void);
void        ycalc_vrange            (void);
void        ycalc_vclose            (void);
void        ycalc_vabout            (void);
void        ycalc_vover             (void);
void        ycalc_vunder            (void);

void        ycalc_hlookup           (void);
void        ycalc_hprefix           (void);
void        ycalc_hmatch            (void);
void        ycalc_hrange            (void);
void        ycalc_hclose            (void);
void        ycalc_habout            (void);
void        ycalc_hover             (void);
void        ycalc_hunder            (void);

void        ycalc_rlookup           (void);
void        ycalc_rprefix           (void);
void        ycalc_rmatch            (void);
void        ycalc_rrange            (void);
void        ycalc_rclose            (void);
void        ycalc_rabout            (void);
void        ycalc_rover             (void);
void        ycalc_runder            (void);

void        ycalc_ilookup           (void);
void        ycalc_iprefix           (void);
void        ycalc_imatch            (void);
void        ycalc_irange            (void);
void        ycalc_iabout            (void);

void        ycalc_clookup           (void);
void        ycalc_cprefix           (void);
void        ycalc_cmatch            (void);
void        ycalc_crange            (void);
void        ycalc_cabout            (void);

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
/*---(bitwise)------------------------*/
void        ycalc_bit_not           (void);
void        ycalc_bit_left          (void);
void        ycalc_bit_right         (void);
void        ycalc_bit_and           (void);
void        ycalc_bit_or            (void);
void        ycalc_bit_xor           (void);

char*       ycalc__unit_base        (char *a_question, int a_num);

#endif
