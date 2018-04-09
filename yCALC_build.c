/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"



#define  S_ERROR_RANGE    -70


char    (*g_cleanser )   (void *a_thing);
char    (*g_creater  )   (char  a_type , void *a_origin , void   *a_target);
char    (*g_delcref  )   (char  a_type , void *a_origin , void   *a_target);
char    (*g_ranger   )   (void *a_thing, int x1, int y1, int z1, int x2, int y2, int z2);



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
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(functions)----------------------*/
   DEBUG_PROG   yLOG_note    ("clearing function calls");
   g_cleanser  = NULL;
   g_creater   = NULL;
   g_delcref   = NULL;
   g_ranger    = NULL;
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yCALC_build_config      (void *a_cleanser, void *a_creater, void *a_delcref, void *a_ranger)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_PROG   yLOG_char    ("status"    , myCALC.status);
   --rce;  if (strchr ("IO", myCALC.status) == NULL) {
      DEBUG_PROG   yLOG_note    ("must initialize before configuring");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update cleanser)----------------*/
   DEBUG_PROG   yLOG_point   ("cleanser"   , a_cleanser);
   --rce;  if (a_cleanser   == NULL) {
      DEBUG_PROG   yLOG_error   ("cleanser"   , "without this callback, dependencies can not be cleared");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   g_cleanser   = a_cleanser;
   /*---(update creater)-----------------*/
   DEBUG_PROG   yLOG_point   ("creater"    , a_creater);
   --rce;  if (a_creater    == NULL) {
      DEBUG_PROG   yLOG_error   ("creater"    , "without this callback, dependencies can not be created");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   g_creater    = a_creater;
   /*---(update delcref)----------------*/
   DEBUG_PROG   yLOG_point   ("delcref"  , a_delcref);
   --rce;  if (a_delcref  == NULL) {
      DEBUG_PROG   yLOG_error   ("delcref"  , "without this callback, a few functions can't created calculated dependencies");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   g_delcref  = a_delcref;
   /*---(update ranger)---------------*/
   DEBUG_PROG   yLOG_point   ("ranger" , a_ranger);
   --rce;  if (a_ranger == NULL) {
      DEBUG_PROG   yLOG_error   ("ranger" , "without this callback, range dependencies can not be created");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   g_ranger = a_ranger;
   /*---(update)-------------------------*/
   DEBUG_PROG   yLOG_note    ("updating status");
   myCALC.status_detail [2] = 'b';
   if (myCALC.status_detail [3] == 'e') {
      myCALC.status = 'O';
      myCALC.status_detail [6] = 'o';
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yCALC__build_prepare    (void *a_thing, char *a_rpn, void *a_calc)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tCALC      *x_curr      = NULL;
   tCALC      *x_next      = NULL;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_CALC   yLOG_char    ("status"    , myCALC.status);
   --rce;  if (myCALC.status != 'O') {
      DEBUG_PROG   yLOG_note    ("must initialize and configure before use");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_point   ("a_thing"   , a_thing);
   --rce;  if (a_thing == NULL) {
      DEBUG_CALC   yLOG_note    ("a_thing not set, nothing to do");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CALC   yLOG_point   ("a_rpn"     , a_rpn);
   --rce;  if (a_rpn == NULL) {
      DEBUG_CALC   yLOG_note    ("rpn not available, nothing to do");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(clear the calculation)----------*/
   DEBUG_CALC   yLOG_point   ("a_calc"    , a_calc);
   x_curr = a_calc;
   while (x_curr != NULL) {
      DEBUG_CALC   yLOG_complex ("x_curr"    , "%-10p, %c, v=%8.2lf, s=%-10p, r=%-10p, f=%-10p, next=%-10p", x_curr, x_curr->t, x_curr->s, x_curr->r, x_curr->f, x_curr->next);
      x_next = x_curr->next;
      if (x_curr->s  != NULL)  free (x_curr->s);
      free (x_curr);
      x_curr = x_next;
   }
   a_calc = NULL;
   /*---(clear the dependencies)---------*/
   rc = g_cleanser  (a_thing);
   DEBUG_CALC   yLOG_value   ("cleanser"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_note    ("could not remove existing dependencies");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-------------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char*        /*-> parse a calculation string ---------[ leaf   [gs.D80.166.A0]*/ /*-[03.0000.018.I]-*/ /*-[--.---.---.--]-*/
yCALC__build_strtok     (char *a_str)
{
   /*---(locals)-----------+-----------+-*/
   static char *x_str       = NULL;
   static int  x_pos       = 0;
   static int  x_next      = 0;
   static int  x_len       = 0;
   static char x_lit       = '-';
   int         i           = 0;
   /*---(defense)------------------------*/
   if (a_str == NULL && x_str == NULL) return NULL;
   /*---(start new string)---------------*/
   if (a_str != NULL) {
      /*> printf ("CALC_strtok : new string <<%s>>\n", a_str);                        <*/
      x_str   = a_str;
      x_pos   = 0;
      x_next  = 0;
      x_len   = strllen (a_str, LEN_RECD);
      x_lit   = '-';
      /*> printf ("   -- a_str = %p\n", a_str);                                       <*/
      /*> printf ("   -- x_str = %p <<%s>>\n", x_str, x_str);                         <*/
   }
   /*---(or, continue)-------------------*/
   if (a_str == NULL) {
      x_pos  = x_next;
      /*> printf ("CALC_strtok : existing string <<%s>>\n", x_str + x_pos);           <*/
   }
   /*---(defense at end)-----------------*/
   if (x_pos >= x_len)                 return NULL;
   if (x_str + x_pos == '\0')          return NULL;
   /*---(search for delimiter)-----------*/
   for (i = x_pos; i < x_len; ++i) {
      /*> printf ("   -- looking at %3d %c : ", i, x_str[i]);                         <*/
      /*---(quoted strings)--------------*/
      if (x_str [i] == '"' )  {
         if (x_lit == 'y') {
            /*> printf ("exiting quote,  ");                                          <*/
            x_lit = '-';
         }
         else {
            /*> printf ("entering quote, ");                                          <*/
            x_lit = 'y';
         }
      }
      if (x_lit == 'y') {
         /*> printf ("next\n");                                                       <*/
         continue;
      }
      if (x_str [i] == ',') {
         x_str [i] = '\0';
         x_next = i + 1;
         /*> printf ("COMMA, x_next = %3d, return %p\n", x_str + x_pos);              <*/
         return x_str + x_pos;
      }
      /*> printf ("next\n");                                                          <*/
   }
   /*---(complete)-----------------------*/
   x_next = x_len;
   return x_str + x_pos;
}

tCALC*       /*-> create a new calculation entry -----[ leaf   [gp.740.222.40]*/ /*-[11.0000.018.!]-*/ /*-[--.---.---.--]-*/
yCALC__build_new        (void *a_thing, tCALC *a_head, tCALC *a_tail)
{
   /*---(locals)-----------+-----+-----+-*/
   tCALC      *x_new       = NULL;          /* current calculation element    */
   char        x_tries     =    0;
   /*---(allocate)-----------------------*/
   while (x_new == NULL && x_tries < 10)  {
      ++x_tries;
      x_new = (tCALC *) malloc (sizeof (tCALC));
   }
   if (x_new == NULL)   return  NULL;
   /*---(set to defaults)----------------*/
   x_new->t     = '-';
   x_new->v     = 0.0;
   x_new->s     = NULL;
   x_new->r     = NULL;
   x_new->f     = NULL;
   /*---(prepare for integration)--------*/
   x_new->owner = a_thing;
   x_new->next  = NULL;
   x_new->prev  = NULL;
   /*---(tie into linked list)-----------*/
   if (a_head == NULL) {
      a_head        = x_new;
   } else {
      a_tail->next  = x_new;
      x_new->prev   = a_tail;
   }
   /*---(update tail pointer)------------*/
   a_tail        = x_new;
   /*---(complete)-----------------------*/
   return x_new;
}

char
yCALC__build_string     (void *a_thing, tCALC *a_calc, char *a_token)
{
   /*---(locals)-----------+-----+-----+-*/
   char        x_len       =    0;
   /*---(check for string)---------------*/
   if (a_token [0] != '"')  return  0;
   /*---(literal string)------------------*/
   DEBUG_CALC   yLOG_note    ("found literal string");
   x_len = strllen (a_token, LEN_RECD);
   a_token [x_len - 1] = NULL;
   a_calc->t = 's';
   a_calc->s = strndup (a_token + 1, LEN_RECD);
   /*---(complete)-----------------------*/
   return 1;
}

char
yCALC__build_range      (void *a_thing, tCALC *a_calc, char *a_token)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   char        x_len       =    0;
   char        x_good      =  '-';
   tCALC      *x_calc      = NULL;
   void       *x_thing     = NULL;
   int         x_beg, y_beg, z_beg;
   int         x_end, y_end, z_end;
   /*---(check for range operator)-------*/
   if      (a_token [0] != ':')            x_good = 'y';
   else if (a_token [0] != ';')            x_good = 'y';
   else if (strcmp (a_token, "..") == 0)   x_good = 'y';
   if (x_good != 'y')  return  0;
   DEBUG_CALC   yLOG_note    ("found range operator");
   /*---(get starting point)-------------*/
   x_calc = a_calc->prev;
   DEBUG_CALC   yLOG_point   ("x_calc"    , x_calc);
   if (x_calc    == NULL) {     
      DEBUG_CALC   yLOG_note    ("can not find prev calculation");
      return G_ERROR_RANGE;
   }
   DEBUG_CALC   yLOG_char    ("type"      , x_calc->t);
   if (x_calc->t != G_TYPE_REF) {
      DEBUG_CALC   yLOG_note    ("prev calc not a reference");
      return G_ERROR_RANGE;
   }
   x_thing = x_calc->r;
   DEBUG_CALC   yLOG_point   ("x_thing"   , x_thing);
   if (x_thing   == NULL) {     
      DEBUG_CALC   yLOG_note    ("beginning reference can not be null");
      return G_ERROR_RANGE;
   }
   rc = g_addresser (a_thing, &x_beg, &y_beg, &z_beg);
   DEBUG_CALC   yLOG_point   ("rc"        , rc);
   if (rc  < 0) {     
      DEBUG_CALC   yLOG_note    ("not a valid address");
      return G_ERROR_RANGE;
   }
   DEBUG_CALC   yLOG_complex ("beginning" , "%4dx, %4dy, %4dz", x_beg, y_beg, z_beg);
   /*---(get ending point)---------------*/
   x_calc = x_calc->prev;
   DEBUG_CALC   yLOG_point   ("x_calc"    , x_calc);
   if (x_calc    == NULL) {     
      DEBUG_CALC   yLOG_note    ("can not find prev-prev calculation");
      return G_ERROR_RANGE;
   }
   DEBUG_CALC   yLOG_char    ("type"      , x_calc->t);
   if (x_calc->t != G_TYPE_REF) {
      DEBUG_CALC   yLOG_note    ("prev-prev calc not a reference");
      return G_ERROR_RANGE;
   }
   x_thing = x_calc->r;
   DEBUG_CALC   yLOG_point   ("x_thing"   , x_thing);
   if (x_thing   == NULL) {     
      DEBUG_CALC   yLOG_note    ("ending reference can not be null");
      return G_ERROR_RANGE;
   }
   rc = g_addresser (a_thing, &x_end, &y_end, &z_end);
   DEBUG_CALC   yLOG_point   ("rc"        , rc);
   if (rc  < 0) {     
      DEBUG_CALC   yLOG_note    ("not a valid address");
      return G_ERROR_RANGE;
   }
   DEBUG_CALC   yLOG_complex ("ending"    , "%4dx, %4dy, %4dz", x_end, y_end, z_end);
   /*---(set dependencies)---------------*/
   rc = g_ranger (a_thing, z_beg, x_beg, y_beg, z_end, x_end, y_end);
   /*---(set calc type)------------------*/
   a_calc->t = 'x';
   /*---(complete)-----------------------*/
   return 1;
}

char
yCALC__build_pointer    (void *a_thing, tCALC *a_calc, char *a_token)
{
   return 0;
}

char
yCALC__build_function   (void *a_thing, tCALC *a_calc, char *a_token)
{
   return 0;
}

char
yCALC__build_reference  (void *a_thing, tCALC *a_calc, char *a_token)
{
   return 0;
}

char
yCALC__build_value      (void *a_thing, tCALC *a_calc, char *a_token)
{
   return 0;
}

char
yCALC__build_step       (void *a_thing, tCALC *a_head, tCALC *a_tail, char *a_token)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   tCALC      *x_calc      = NULL;          /* current calculation element    */
   tCALC      *x_tail      = NULL;          /* last calculation element       */
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(create and null)-----------------*/
   DEBUG_CALC   yLOG_note    ("allocate calc entry");
   x_calc   = yCALC__build_new (a_thing, a_head, a_tail);
   /*---(try al types)--------------------*/
   if (rc == 0)  yCALC__build_string    (a_thing, x_calc, a_token);
   if (rc == 0)  yCALC__build_range     (a_thing, x_calc, a_token);
   if (rc == 0)  yCALC__build_pointer   (a_thing, x_calc, a_token);
   if (rc == 0)  yCALC__build_function  (a_thing, x_calc, a_token);
   if (rc == 0)  yCALC__build_reference (a_thing, x_calc, a_token);
   if (rc == 0)  yCALC__build_value     (a_thing, x_calc, a_token);
   /*
    *   handle nothing found
    */
   /*
    *   handle explicit errors
    */
   /*---(report out)-----------------------*/
   DEBUG_CALC   yLOG_complex ("element"   , "typ=%c, val=%F, str=%-9p, ref=%-9p, fnc=%-9p", x_calc->t, x_calc->v, x_calc->s, x_calc->r, x_calc->f);
   /*---(complete)-------------------------*/
   DEBUG_CALC   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
yCALC_build             (void *a_thing, char *a_rpn, void *a_head, char *a_notice)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_work      [LEN_RECD];       /* working copy of source string  */
   char       *p;                           /* strtok current pointer         */
   int         x_ntoken    =    0;
   tCALC      *x_tail      = NULL;
   /*---(header)-------------------------*/
   DEBUG_CALC   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   rc = yCALC__build_prepare (a_thing, a_rpn, a_head);
   DEBUG_CALC   yLOG_value   ("prepare"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(initialize)-------------------------*/
   strncpy (x_work, a_rpn + 2, LEN_RECD);
   DEBUG_CALC   yLOG_info    ("x_work"    , x_work);
   /*---(read first tokens)------------------*/
   p = yCALC__build_strtok (x_work);
   --rce;  if (p == NULL) {
      DEBUG_CALC   yLOG_note    ("could not tokenize rpn");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   ++x_ntoken;
   /*---(process tokens)---------------------*/
   while (p != NULL) {
      DEBUG_CALC   yLOG_info    ("token"     , p);
      rc = yCALC__build_step (a_thing, a_head, x_tail, p);
      if (rc != 1)   break;
      p = yCALC__build_strtok (NULL);
      ++x_ntoken;
   }
   /*---(check for failure)----------------*/
   --rce;  if (rc != 1) {
      DEBUG_CALC   yLOG_note    ("could not build calculation chain");
      DEBUG_CALC   yLOG_exitr   (__FUNCTION__, rce);
      switch (rc) {
      case S_ERROR_RANGE :
         strlcpy (a_notice, "#.badrng", LEN_RECD);
         break;
      }
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
