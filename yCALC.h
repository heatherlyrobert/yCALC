/*===============================[[ beg-code ]]===============================*/


#ifndef YCALC_HGUARD
#define YCALC_HGUARD loaded



#define       YCALC_GYGES        's'



/*---(object types)-------------------*/
#define     YCALC_DATA_BLANK      '-'
#define     YCALC_DATA_STR        's'
#define     YCALC_DATA_SFORM      '#'
#define     YCALC_DATA_SLIKE      '5'
#define     YCALC_DATA_NUM        'n'
#define     YCALC_DATA_NFORM      '='
#define     YCALC_DATA_NLIKE      '9'
#define     YCALC_DATA_ADDR       '&'
#define     YCALC_DATA_CADDR      '!'
#define     YCALC_DATA_RANGE      ':'
#define     YCALC_DATA_VAR        'Ö'
#define     YCALC_DATA_INTERN     '®'
#define     YCALC_DATA_MERGED     '<'
#define     YCALC_DATA_ERROR      'E'
#define     YCALC_DATA_PMERGE     'p'
#define     YCALC_DATA_GARBAGE    'g'

extern char YCALC_GROUP_ALL    [20];
extern char YCALC_GROUP_RPN    [20];
extern char YCALC_GROUP_CALC   [20];
extern char YCALC_GROUP_DEPS   [20];
extern char YCALC_GROUP_NUM    [20];
extern char YCALC_GROUP_STR    [20];
extern char YCALC_GROUP_POINT  [20];
extern char YCALC_GROUP_ERR    [20];
extern char YCALC_GROUP_FPRE   [20];




#define     YCALC_FULL            'f'    /* create owner && deproot            */
#define     YCALC_OWNR            'y'    /* create owner only                  */
#define     YCALC_LOOK            '-'    /* do not create, just look           */

#define     G_SPECIAL_TYPE        'T'
#define     G_SPECIAL_LABEL       'L'
#define     G_SPECIAL_SOURCE      'S'
#define     G_SPECIAL_PRINT       'P'


/*---(dependency types)---------------*/

#define     G_DEP_BLANK           '-'

#define     G_DEP_REQUIRE         'R'
#define     G_DEP_PROVIDE         'p'

#define     G_DEP_POINTER         '&'
#define     G_DEP_TARGET          'T'

#define     G_DEP_RANGE           ':'
#define     G_DEP_ENTRY           '.'

#define     G_DEP_FORMAT          'F'
#define     G_DEP_COPY            'f'

#define     G_DEP_SOURCE          'S'
#define     G_DEP_LIKE            'l'

#define     G_DEP_MERGED          'M'
#define     G_DEP_BLEED           'b'

#define     G_DEP_CALCREF         'A'
#define     G_DEP_ADDRESS         'a'

#define     G_DEP_CONTENT         'C'
#define     G_DEP_TITLE           't'

#define     G_DEP_WATCHER         'W'
#define     G_DEP_STALKED         's'

#define     G_DEP_CALL            ' '
#define     G_DEP_DEST            '´'


char*       yCALC_version           (void);

char        yCALC_init              (char  a_style);
char        yCALC_set_state         (char a_state);
char        yCALC_cleanse           (void);
char        yCALC_wrap              (void);

char        yCALC_exist_config      (void *a_enabler  , void *a_pointer  , void *a_reaper );
char        yCALC_label_config      (void *a_who_named, void *a_who_at   , void *a_labeler);
char        yCALC_value_config      (void *a_valuer   , void *a_addresser, void *a_special, void *a_printer);

char        yCALC_enable            (void *a_owner);
char        yCALC_disable           (void **a_owner, void **a_deproot);

char        yCALC_create            (char a_type, char *a_source, char *a_target);
char        yCALC_delete            (char a_type, char *a_source, char *a_target);
char        yCALC_range             (void *a_source, int b1, int x1, int y1, int b2, int x2, int y2);

char        yCALC_variable          (char *a_name, char *a_real);
char        yCALC_handle            (char *a_label);
char        yCALC_exec              (char *a_label);

long        yCALC_getstamp          (void *a_deproot);
char        yCALC_seq_up            (void *a_deproot, void *a_consumer);
char        yCALC_seq_down          (void *a_deproot, void *a_consumer);
char        yCALC_seq_full          (void *a_consumer);
char        yCALC_seq_downdown      (long a_stamp   , void *a_consumer);
char        yCALC_seq_downup        (long a_stamp   , void *a_consumer);
char        yCALC_seq_dump          (void *a_file);
char        yCALC_garbage_collect   (void);

char        yCALC_calculate         (void);
char        yCALC_calc_from         (void *a_deproot);


int         yCALC_type              (void *a_deproot);
int         yCALC_nreq              (void *a_deproot);
int         yCALC_npro              (void *a_deproot);
int         yCALC_ncalc             (void *a_deproot);
char        yCALC_stamp_cmp         (void *a_deproot, long a_stamp);
char        yCALC_stamp_set         (void *a_deproot, long a_stamp);

/*--type--- ---name---------------- ---params---------------------------------*/
char        yCALC_disp_reqs         (void *a_me, char *a_list);
char        yCALC_disp_pros         (void *a_me, char *a_list);
char        yCALC_disp_like         (void *a_me, char *a_list);
char        yCALC_disp_copy         (void *a_me, char *a_list);

/*--type--- ---accessors----------- ---params---------------------------------*/
char        yCALC_show_rpn          (void *a_deproot, int *a_nrpn, char *a_rpn);
char        yCALC_show_reqs         (void *a_deproot, int *a_nreq, char *a_reqs);
char        yCALC_show_pros         (void *a_deproot, int *a_npro, char *a_pros);

char        yCALC_vars_dump         (void *f);
char        yCALC_deps_dump         (void *f);

char        yCALC_merge_source      (void *a_deproot, void **a_owner);

#endif
/*===[[ END ]]================================================================*/
