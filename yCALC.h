/*===[[ START HDOC ]]=========================================================*/
#ifndef YCALC_HGUARD
#define YCALC_HGUARD loaded


/*---(dependency types)---------------*/

#define     G_DEP_BLANK          '-'

#define     G_DEP_REQUIRE        'R'
#define     G_DEP_PROVIDE        'p'

#define     G_DEP_RANGE          'P'
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

char        yCALC_build_config      (void *a_deproot , void *a_whois  , void *a_labeler  , void *a_reaper  );
char        yCALC_exec_config       (void *a_valuer  , void *a_addresser);

char        yCALC_enable            (void *a_owner, void **a_ycalc);

char        yCALC_create            (char a_type, void *a_source, void *a_target);
char        yCALC_range             (void *a_source, int x1, int y1, int z1, int x2, int y2, int z2);
char        yCALC_delete            (char a_type, void *a_source, void *a_target);

char        yCALC_build             (void *a_deproot, char *a_rpn, char *a_notice);



#endif
/*===[[ END ]]================================================================*/
