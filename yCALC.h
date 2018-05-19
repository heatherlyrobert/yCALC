/*===[[ START HDOC ]]=========================================================*/
#ifndef YCALC_HGUARD
#define YCALC_HGUARD loaded



#define       YCALC_GYGES        's'



/*---(object types)-------------------*/
#define       YCALC_DATA_BLANK   '-'
#define       YCALC_DATA_STR     's'
#define       YCALC_DATA_SFORM   '#'
#define       YCALC_DATA_SLIKE   '5'
#define       YCALC_DATA_NUM     'n'
#define       YCALC_DATA_NFORM   '='
#define       YCALC_DATA_NLIKE   '9'
#define       YCALC_DATA_ADDR    '&'
#define       YCALC_DATA_RANGE   ':'
#define       YCALC_DATA_MERGED  '+'
#define       YCALC_DATA_ERROR   'E'



/*---(dependency types)---------------*/

#define     G_DEP_BLANK          '-'

#define     G_DEP_REQUIRE        'R'
#define     G_DEP_PROVIDE        'p'

#define     G_DEP_POINTER        'P'
#define     G_DEP_TARGET         't'

#define     G_DEP_RANGE          ':'
#define     G_DEP_ENTRY          '.'

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

char        yCALC_exist_config      (void *a_enabler  , void *a_pionter  , void *a_reaper );
char        yCALC_label_config      (void *a_who_named, void *a_who_at   , void *a_labeler);
char        yCALC_value_config      (void *a_valuer   , void *a_addresser, void *a_special);

char        yCALC_enable            (void *a_owner);
char        yCALC_disable           (void **a_deproot);

char        yCALC_create            (char a_type, char *a_source, char *a_target);
char        yCALC_delete            (char a_type, char *a_source, char *a_target);
char        yCALC_range             (void *a_source, int x1, int y1, int z1, int x2, int y2, int z2);

char        yCALC_handle            (char *a_label);

char        yCALC_seq_up            (void *a_deproot, void *a_consumer);
char        yCALC_seq_down          (void *a_deproot, void *a_consumer);
char        yCALC_seq_full          (void *a_consumer);
char        yCALC_seq_downdown      (long a_stamp   , void *a_consumer);
char        yCALC_seq_downup        (long a_stamp   , void *a_consumer);

char        yCALC_calculate         (void);


#endif
/*===[[ END ]]================================================================*/
