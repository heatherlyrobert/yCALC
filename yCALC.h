/*===[[ START HDOC ]]=========================================================*/
#ifndef YCALC_HGUARD
#define YCALC_HGUARD loaded



/*---(object types)-------------------*/
#define       YCALC_DATA_BLANK   '-'
#define       YCALC_DATA_STR     's'
#define       YCALC_DATA_SFORM   '#'
#define       YCALC_DATA_SLIKE   '5'
#define       YCALC_DATA_NUM     'n'
#define       YCALC_DATA_NFORM   '='
#define       YCALC_DATA_NLIKE   '9'
#define       YCALC_DATA_RANGE   'p'
#define       YCALC_DATA_ADDR    'a'
#define       YCALC_DATA_MERGED  '+'
#define       YCALC_DATA_ERROR   'E'



/*---(dependency types)---------------*/

#define     G_DEP_BLANK          '-'

#define     G_DEP_REQUIRE        'R'
#define     G_DEP_PROVIDE        'p'

#define     G_DEP_POINTER        'P'
#define     G_DEP_CELL           'c'

#define     G_DEP_FORMAT         'F'
#define     G_DEP_COPY           'f'

#define     G_DEP_SOURCE         'S'
#define     G_DEP_LIKE           'l'

#define     G_DEP_MERGED         'M'
#define     G_DEP_BLEED          'b'

#define     G_DEP_CALCREF        'A'
#define     G_DEP_ADDRESS        'a'

#define     G_DEP_WATCHER        'W'
#define     G_DEP_STALKED        's'


char*       yCALC_version           (void);

char        yCALC_init              (char  a_style);
char        yCALC_wrap              (void);

char        yCALC_build_config      (void *a_enabler, void *a_deproot, void *a_whois, void *a_labeler, void *a_reaper);
char        yCALC_exec_config       (void *a_valuer, void *a_addresser, void *a_special);

char        yCALC_enable            (void *a_owner);
char        yCALC_disable           (void *a_deproot);

char        yCALC_create            (char a_type, char *a_source, char *a_target);
char        yCALC_delete            (char a_type, char *a_source, char *a_target);
char        yCALC_range             (void *a_source, int x1, int y1, int z1, int x2, int y2, int z2);

char        yCALC_build             (char *a_label, char *a_rpn, char **a_notice);

char        yCALC_exec              (void *a_deproot, char a_type, double *a_value, char **a_string, char **a_notice);
char        yCALC_exec_label        (char *a_label  , char a_type, double *a_value, char **a_string, char **a_notice);

char        yCALC_seq_up            (void *a_deproot, void *a_consumer);
char        yCALC_seq_down          (void *a_deproot, void *a_consumer);
char        yCALC_seq_full          (void *a_consumer);
char        yCALC_seq_downdown      (long a_stamp   , void *a_consumer);
char        yCALC_seq_downup        (long a_stamp   , void *a_consumer);
char        yCALC_seq_downup        (long a_stamp   , void *a_consumer);



#endif
/*===[[ END ]]================================================================*/
