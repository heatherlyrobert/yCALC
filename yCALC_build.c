/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"

tTERMS      s_terms [MAX_TERM] = {
   { 'v' , "val"    , "numeric or cell address"        },
   { 's' , "str"    , "string or cell address"         },
   { '?' , "v/s"    , "string or numeric (runtime)"    },
   { 'a' , "adr"    , "cell address or pointer"        },
   { 'r' , "rng"    , "range of cells or pointer"      },
   { 't' , "T/F"    , "true or false"                  },
   { '-' , "---"    , "-----"                          },
};

char
yCALC__build_init       (void)
{
}


char
yCALC__build_purge      (void *a_calc)
{
   tCALC      *x_next;
   x_next = a_calc;
   while (x_next != NULL) {
   }
}

char
yCALC__build_step       (char *a_rpn)
{
}

char
yCALC_build             (char *a_rpn, void **a_head)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        work        [LEN_RECD];       /* working copy of source string  */
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CALC   yLOG_point   ("a_rpn"     , a_rpn);
   --rce;  if (a_rpn == NULL) {
      DEBUG_CALC   yLOG_note    ("rpn not available, nothing to do");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(clear)--------------------------*/
   DEBUG_CALC   yLOG_point   ("a_head"    , a_head);
   --rce;  if (a_head != NULL) {
      DEBUG_CALC   yLOG_note    ("rpn not available, nothing to do");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }




   /*---(complete)-------------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

/*> char         /+-> build a new calculation ------------[ leaf   [ge.#QA.1Q#.XA]+/ /+-[04.0000.107.!]-+/ /+-[--.---.---.--]-+/                                           <* 
 *> yCALC_build_OLD         (tCELL *a_cell)                                                                                                                                <* 
 *> {                                                                                                                                                                      <* 
 *>    DEBUG_CALC   yLOG_enter   (__FUNCTION__);                                                                                                                           <* 
 *>    /+---(locals)-----------+-----------+-+/                                                                                                                            <* 
 *>    char        work        [LEN_RECD];       /+ working copy of source string  +/                                                                                      <* 
 *>    int         x_ntoken    = -1;                                                                                                                                       <* 
 *>    char       *p;                           /+ strtok current pointer         +/                                                                                       <* 
 *>    char       *q           = ",";           /+ strtok delimiter               +/                                                                                       <* 
 *>    tCALC      *x_calc      = NULL;          /+ current calculation element    +/                                                                                       <* 
 *>    tCALC      *x_tail      = NULL;          /+ last calculation element       +/                                                                                       <* 
 *>    int         t1, t2, c1, c2, r1, r2;                                                                                                                                 <* 
 *>    int         range       = 0;                                                                                                                                        <* 
 *>    char        mode        = '-';                                                                                                                                      <* 
 *>    int         x_tab       = a_cell->tab;                                                                                                                              <* 
 *>    int         x_col       = 0;                                                                                                                                        <* 
 *>    int         x_row       = 0;                                                                                                                                        <* 
 *>    tCELL      *dest        = NULL;                                                                                                                                     <* 
 *>    char        rc          = 0;                                                                                                                                        <* 
 *>    char        rce         = -10;                                                                                                                                      <* 
 *>    char        msg         [1000];                                                                                                                                     <* 
 *>    int         i           = 0;                                                                                                                                        <* 
 *>    int         len         = 0;                                                                                                                                        <* 
 *>    char       *valid       = "0123456789.-+";    /+ only digits               +/                                                                                       <* 
 *>    char        label       [20]        = "";                                                                                                                           <* 
 *>    /+---(defense: starting conditions)---+/                                                                                                                            <* 
 *>    DEBUG_CALC   yLOG_point   ("a_cell"    , a_cell);                                                                                                                   <* 
 *>    --rce;  if (a_cell       == NULL)  {                                                                                                                                <* 
 *>       DEBUG_CALC   yLOG_note    ("can not calculate a null cell");                                                                                                     <* 
 *>       DEBUG_CALC   yLOG_exit    (__FUNCTION__);                                                                                                                        <* 
 *>       return rce;                                                                                                                                                      <* 
 *>    }                                                                                                                                                                   <* 
 *>    DEBUG_CALC   yLOG_info    ("label"     , a_cell->label);                                                                                                            <* 
 *>    DEBUG_CALC   yLOG_char    ("type"      , a_cell->t);                                                                                                                <* 
 *>    --rce;  if (strchr (G_CELL_CALC, a_cell->t) == 0) {                                                                                                                 <* 
 *>       DEBUG_CALC   yLOG_note    ("not a calculated type");                                                                                                             <* 
 *>       DEBUG_CALC   yLOG_exit    (__FUNCTION__);                                                                                                                        <* 
 *>       return rce;                                                                                                                                                      <* 
 *>    }                                                                                                                                                                   <* 
 *>    DEBUG_CALC   yLOG_info    ("rpn"       , a_cell->rpn);                                                                                                              <* 
 *>    --rce;  if (a_cell->rpn  == NULL) {                                                                                                                                 <* 
 *>       DEBUG_CALC   yLOG_note    ("rpn is null, so can not build");                                                                                                     <* 
 *>       DEBUG_CALC   yLOG_exit    (__FUNCTION__);                                                                                                                        <* 
 *>       return rce;                                                                                                                                                      <* 
 *>    }                                                                                                                                                                   <* 
 *>    /+---(clear calc and deps)------------+/                                                                                                                            <* 
 *>    CALC_cleanse (a_cell);                                                                                                                                              <* 
 *>    /+---(initialize)-------------------------+/                                                                                                                        <* 
 *>    strncpy (work, a_cell->rpn + 2, LEN_RECD);                                                                                                                          <* 
 *>    DEBUG_CALC   yLOG_info    ("source"    , work);                                                                                                                     <* 
 *>    if (a_cell->s [0] == '&')  strcpy (label, "");                                                                                                                      <* 
 *>    /+---(read first tokens)------------------+/                                                                                                                        <* 
 *>    --rce;                                                                                                                                                              <* 
 *>    /+> p = strtok (work, q);                                                          <+/                                                                              <* 
 *>    p = CALC_strtok (work);                                                                                                                                             <* 
 *>    ++x_ntoken;                                                                                                                                                         <* 
 *>    if (p == NULL) {                                                                                                                                                    <* 
 *>       a_cell->t = 'E';                 /+ turn formula type off and error on  +/                                                                                       <* 
 *>       DEBUG_CALC   yLOG_exit    (__FUNCTION__);                                                                                                                        <* 
 *>       return rce;                                                                                                                                                      <* 
 *>    }                                                                                                                                                                   <* 
 *>    --rce;                                                                                                                                                              <* 
 *>    /+---(process tokens)---------------------+/                                                                                                                        <* 
 *>    while (p != NULL) {                                                                                                                                                 <* 
 *>       /+---(debug token header)--------------+/                                                                                                                        <* 
 *>       DEBUG_CALC   yLOG_info    ("token"     , p);                                                                                                                     <* 
 *>       /+---(create and null)-----------------+/                                                                                                                        <* 
 *>       DEBUG_CALC   yLOG_note    ("allocate calc entry");                                                                                                               <* 
 *>       x_calc   = CALC_new (a_cell, x_tail);                                                                                                                            <* 
 *>       x_tail   = x_calc;                                                                                                                                               <* 
 *>       /+---(literal string)------------------+/                                                                                                                        <* 
 *>       DEBUG_CALC   yLOG_note    ("check for literal string");                                                                                                          <* 
 *>       if (p[0] == '"') {                                                                                                                                               <* 
 *>          int   len = strlen(p);                                                                                                                                        <* 
 *>          p[len - 1] = '\0';                                                                                                                                            <* 
 *>          x_calc->t = 's';                                                                                                                                              <* 
 *>          x_calc->s = strndup(p + 1, LEN_RECD);                                                                                                                         <* 
 *>          /+---(read next)---------------------+/                                                                                                                       <* 
 *>          DEBUG_CALC   yLOG_complex ("element"   , "typ=%c, val=%F, str=%-9p, ref=%-9p, fnc=%-9p", x_calc->t, x_calc->v, x_calc->s, x_calc->r, x_calc->f);              <* 
 *>          /+> p = strtok (NULL, q);                                                    <+/                                                                              <* 
 *>          p = CALC_strtok (NULL);                                                                                                                                       <* 
 *>          ++x_ntoken;                                                                                                                                                   <* 
 *>          continue;                                                                                                                                                     <* 
 *>       }                                                                                                                                                                <* 
 *>       /+---(range operators)-----------------+/                                                                                                                        <* 
 *>       DEBUG_CALC   yLOG_note    ("check for range operator");                                                                                                          <* 
 *>       if ((strncmp(p, ":"          ,  2) == 0)                                                                                                                         <* 
 *>             || (strncmp(p, ";"          ,  2) == 0)                                                                                                                    <* 
 *>             || (strncmp(p, ".."         ,  2) == 0)) {                                                                                                                 <* 
 *>          DEBUG_CALC   yLOG_note    ("found a range");                                                                                                                  <* 
 *>          rc = CALC_range (a_cell, x_calc);                                                                                                                             <* 
 *>          if (rc < 0) {                                                                                                                                                 <* 
 *>             a_cell->t = 'E';                                                                                                                                           <* 
 *>             a_cell->v_str = strndup ("#.badrng", LEN_RECD);                                                                                                            <* 
 *>             DEBUG_CALC   yLOG_exit    (__FUNCTION__);                                                                                                                  <* 
 *>             return rce;                                                                                                                                                <* 
 *>          }                                                                                                                                                             <* 
 *>          x_calc->t = 'x';                                                                                                                                              <* 
 *>          /+---(read next)---------------------+/                                                                                                                       <* 
 *>          DEBUG_CALC   yLOG_complex ("element"   , "typ=%c, val=%F, str=%-9p, ref=%-9p, fnc=%-9p", x_calc->t, x_calc->v, x_calc->s, x_calc->r, x_calc->f);              <* 
 *>          /+> p = strtok (NULL, q);                                                    <+/                                                                              <* 
 *>          p = CALC_strtok (NULL);                                                                                                                                       <* 
 *>          ++x_ntoken;                                                                                                                                                   <* 
 *>          continue;                                                                                                                                                     <* 
 *>       }                                                                                                                                                                <* 
 *>       /+---(pointer operator)----------------+/                                                                                                                        <* 
 *>       DEBUG_CALC   yLOG_note    ("check for pointer operator");                                                                                                        <* 
 *>       if (strncmp(p, "*:"         ,  2) == 0) {                                                                                                                        <* 
 *>          DEBUG_CALC   yLOG_note    ("found a pointer");                                                                                                                <* 
 *>          rc = CALC_pointer (a_cell, x_calc);                                                                                                                           <* 
 *>          if (rc < 0) {                                                                                                                                                 <* 
 *>             a_cell->t = 'E';                                                                                                                                           <* 
 *>             a_cell->v_str = strndup ("#.badptr", LEN_RECD);                                                                                                            <* 
 *>             DEBUG_CALC   yLOG_exit    (__FUNCTION__);                                                                                                                  <* 
 *>             return rce;                                                                                                                                                <* 
 *>          }                                                                                                                                                             <* 
 *>          /+---(read next)---------------------+/                                                                                                                       <* 
 *>          DEBUG_CALC   yLOG_complex ("element"   , "typ=%c, val=%F, str=%-9p, ref=%-9p, fnc=%-9p", x_calc->t, x_calc->v, x_calc->s, x_calc->r, x_calc->f);              <* 
 *>          /+> p = strtok (NULL, q);                                                    <+/                                                                              <* 
 *>          p = CALC_strtok (NULL);                                                                                                                                       <* 
 *>          ++x_ntoken;                                                                                                                                                   <* 
 *>          continue;                                                                                                                                                     <* 
 *>       }                                                                                                                                                                <* 
 *>       /+---(function table)------------------+/                                                                                                                        <* 
 *>       DEBUG_CALC   yLOG_note    ("check for function");                                                                                                                <* 
 *>       len = strlen (p);                                                                                                                                                <* 
 *>       for (i = 0; i < MAX_FUNCS; ++i) {                                                                                                                                <* 
 *>          /+> DEBUG_CALC   yLOG_complex ("check"     , "%3d, %-12.12s, %3d, %-10.10p   vs %-12.12s, %3d", i, s_funcs[i].n, s_funcs[i].l, s_funcs[i].f, p, len);   <+/   <* 
 *>          if (s_funcs [i].f == NULL)              break;                                                                                                                <* 
 *>          if (strcmp (s_funcs[i].n, "END") == 0)  break;                                                                                                                <* 
 *>          if (len  != s_funcs [i].l  )            continue;                                                                                                             <* 
 *>          if (p[0] != s_funcs[i].n[0])            continue;                                                                                                             <* 
 *>          if (strcmp (s_funcs[i].n, p) != 0)      continue;                                                                                                             <* 
 *>          DEBUG_CALC   yLOG_complex ("found it"  , "%3d, %-12.12s, %3d, %-10.10p   vs %-12.12s, %3d", i, s_funcs[i].n, s_funcs[i].l, s_funcs[i].f, p, len);             <* 
 *>          x_calc->t = 'f';                                                                                                                                              <* 
 *>          x_calc->f = s_funcs[i].f;                                                                                                                                     <* 
 *>          /+> if (label [0] == '\0')  strncpy (label, s_funcs[i].h, 19);                 <+/                                                                            <* 
 *>          /+---(read next)---------------------+/                                                                                                                       <* 
 *>          DEBUG_CALC   yLOG_complex ("element"   , "typ=%c, val=%F, str=%-9p, ref=%-9p, fnc=%-9p", x_calc->t, x_calc->v, x_calc->s, x_calc->r, x_calc->f);              <* 
 *>          /+> p = strtok (NULL, q);                                                    <+/                                                                              <* 
 *>          p = CALC_strtok (NULL);                                                                                                                                       <* 
 *>          ++x_ntoken;                                                                                                                                                   <* 
 *>          break;                                                                                                                                                        <* 
 *>       }                                                                                                                                                                <* 
 *>       if (x_calc->t != '-')  continue;                                                                                                                                 <* 
 *>       /+---(look for tab names)--------------+/                                                                                                                        <* 
 *>       DEBUG_CALC   yLOG_note    ("look for reference");                                                                                                                <* 
 *>       rc = LOC_parse (p, &x_tab, &x_col, &x_row, NULL);                                                                                                                <* 
 *>       DEBUG_CALC   yLOG_value   ("rc"        , rc);                                                                                                                    <* 
 *>       /+---(handle cell reference)--------+/                                                                                                                           <* 
 *>       if (rc == 0) {                                                                                                                                                   <* 
 *>          DEBUG_CALC   yLOG_note    ("check if legal");                                                                                                                 <* 
 *>          rc = LOC_legal (x_tab, x_col, x_row, CELL_FIXED);                                                                                                             <* 
 *>          DEBUG_CALC   yLOG_value   ("rc"        , rc);                                                                                                                 <* 
 *>          if (rc == 0) {                                                                                                                                                <* 
 *>             dest = LOC_cell_at_loc (x_tab, x_col, x_row);                                                                                                              <* 
 *>             DEBUG_CALC   yLOG_point   ("dest"      , dest);                                                                                                            <* 
 *>             if (dest == NULL)  CELL_change (&dest, CHG_NOHIST, x_tab, x_col, x_row, "");                                                                               <* 
 *>             DEBUG_CALC   yLOG_point   ("dest"      , dest);                                                                                                            <* 
 *>             if (dest == NULL)  {                                                                                                                                       <* 
 *>                a_cell->t = 'E';                                                                                                                                        <* 
 *>                a_cell->v_str = strndup ("#.badmal", LEN_RECD);                                                                                                         <* 
 *>                DEBUG_CALC   yLOG_exit    (__FUNCTION__);                                                                                                               <* 
 *>                return rce - 1;                                                                                                                                         <* 
 *>             }                                                                                                                                                          <* 
 *>             rc = DEP_create (G_DEP_REQUIRE, a_cell, dest);                                                                                                             <* 
 *>             DEBUG_CALC   yLOG_value   ("rc"        , rc);                                                                                                              <* 
 *>             if (rc < 0) {                                                                                                                                              <* 
 *>                DEBUG_CALC   yLOG_info    ("error"     , "dependency can not be created");                                                                              <* 
 *>                a_cell->t = 'E';                                                                                                                                        <* 
 *>                a_cell->v_str = strndup ("#.baddep", LEN_RECD);                                                                                                         <* 
 *>                DEBUG_CALC   yLOG_exit    (__FUNCTION__);                                                                                                               <* 
 *>                return rce - 2;                                                                                                                                         <* 
 *>             }                                                                                                                                                          <* 
 *>             x_calc->t = 'r';                                                                                                                                           <* 
 *>             x_calc->r = dest;                                                                                                                                          <* 
 *>          } else {                                                                                                                                                      <* 
 *>             a_cell->t = 'E';                                                                                                                                           <* 
 *>             a_cell->v_str = strndup ("#.badref", LEN_RECD);                                                                                                            <* 
 *>             DEBUG_CALC   yLOG_exit    (__FUNCTION__);                                                                                                                  <* 
 *>             return rce - 3;                                                                                                                                            <* 
 *>          }                                                                                                                                                             <* 
 *>          /+---(read next)---------------------+/                                                                                                                       <* 
 *>          DEBUG_CALC   yLOG_complex ("element"   , "typ=%c, val=%F, str=%-9p, ref=%-9p, fnc=%-9p", x_calc->t, x_calc->v, x_calc->s, x_calc->r, x_calc->f);              <* 
 *>          /+> p = strtok (NULL, q);                                                    <+/                                                                              <* 
 *>          p = CALC_strtok (NULL);                                                                                                                                       <* 
 *>          ++x_ntoken;                                                                                                                                                   <* 
 *>          continue;                                                                                                                                                     <* 
 *>       }                                                                                                                                                                <* 
 *>       if (rc > 0) {                                                                                                                                                    <* 
 *>          a_cell->t = 'E';                                                                                                                                              <* 
 *>          a_cell->v_str = strndup ("#.bigref", LEN_RECD);                                                                                                               <* 
 *>          DEBUG_CALC   yLOG_exit    (__FUNCTION__);                                                                                                                     <* 
 *>          return rce - 4;                                                                                                                                               <* 
 *>       }                                                                                                                                                                <* 
 *>       /+---(handle cell reference)--------+/                                                                                                                           <* 
 *>       x_calc->t = 'v';                                                                                                                                                 <* 
 *>       DEBUG_CALC   yLOG_note    ("check for numeric value");                                                                                                           <* 
 *>       for (i = 0; i < len; ++i) {                                                                                                                                      <* 
 *>          if (strchr (valid, p[i])  != 0) continue;                                                                                                                     <* 
 *>          DEBUG_CALC   yLOG_char    ("non-value" , p[i]);                                                                                                               <* 
 *>          x_calc->t = '-';                                                                                                                                              <* 
 *>       }                                                                                                                                                                <* 
 *>       if (x_calc->t == 'v') {                                                                                                                                          <* 
 *>          x_calc->v = atof (p);                                                                                                                                         <* 
 *>          /+---(read next)---------------------+/                                                                                                                       <* 
 *>          DEBUG_CALC   yLOG_complex ("element"   , "typ=%c, val=%F, str=%-9p, ref=%-9p, fnc=%-9p", x_calc->t, x_calc->v, x_calc->s, x_calc->r, x_calc->f);              <* 
 *>          /+> p = strtok (NULL, q);                                                    <+/                                                                              <* 
 *>          p = CALC_strtok (NULL);                                                                                                                                       <* 
 *>          ++x_ntoken;                                                                                                                                                   <* 
 *>          continue;                                                                                                                                                     <* 
 *>       }                                                                                                                                                                <* 
 *>       /+---(fall through)-----------------+/                                                                                                                           <* 
 *>       a_cell->t = 'E';                                                                                                                                                 <* 
 *>       a_cell->v_str = strndup ("#.unknown", LEN_RECD);                                                                                                                 <* 
 *>       DEBUG_CALC   yLOG_exit    (__FUNCTION__);                                                                                                                        <* 
 *>       return rce - 4;                                                                                                                                                  <* 
 *>    }                                                                                                                                                                   <* 
 *>    /+---(auto-labeling)--------------------+/                                                                                                                          <* 
 *>    /+> rc = CALC_checkpointer (a_cell);                                                <*                                                                              <* 
 *>     *> if (rc == 1)   strcpy (label, "pointer");                                       <*                                                                              <* 
 *>     *> if (rc == 3)   strcpy (label, "dataset");                                       <*                                                                              <* 
 *>     *> if (label [0] == '\0' || label [0] == '-')  {                                   <*                                                                              <* 
 *>     *>    strcpy (label, a_cell->s);                                                   <*                                                                              <* 
 *>     *>    label [0] = ':';                                                             <*                                                                              <* 
 *>     *> }                                                                               <*                                                                              <* 
 *>     *> if (label [0] != '-') {                                                         <*                                                                              <* 
 *>     *>    rc = LOC_legal (a_cell->tab, a_cell->col + 1, a_cell->row, CELL_FIXED);      <*                                                                              <* 
 *>     *>    if (rc == 0) {                                                               <*                                                                              <* 
 *>     *>       dest = LOC_cell_at_loc (a_cell->tab, a_cell->col + 1, a_cell->row);       <*                                                                              <* 
 *>     *>       if (dest == NULL || dest->t == 'l') {                                     <*                                                                              <* 
 *>     *>          CELL_change (&dest, a_cell->tab, a_cell->col + 1, a_cell->row, label);        <*                                                                       <* 
 *>     *>          dest->t = 'l';                                                         <*                                                                              <* 
 *>     *>       }                                                                         <*                                                                              <* 
 *>     *>    }                                                                            <*                                                                              <* 
 *>     *> }                                                                               <+/                                                                             <* 
 *>    /+---(complete)-------------------------+/                                                                                                                          <* 
 *>    DEBUG_CALC   yLOG_exit    (__FUNCTION__);                                                                                                                           <* 
 *>    return 0;                                                                                                                                                           <* 
 *> }                                                                                                                                                                      <*/
