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

char        yCALC_build_config      (void *a_cleanser, void *a_creater, void *a_delcref , void *a_ranger   );
char        yCALC_exec_config       (void *a_thinger , void *a_valuer , void *a_detailer, void *a_addresser);
char        yCALC_debug_config      (void *a_lister  );



#endif
/*===[[ END ]]================================================================*/
