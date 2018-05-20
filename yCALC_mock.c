/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"




/*====================------------------------------------====================*/
/*===----                         mock system                          ----===*/
/*====================------------------------------------====================*/
static void      o___MOCK____________________o (void) {;}

/*> tMOCK   s_mocks     [500] = {                                                     <* 
 *>    /+---(root)--------------------------------------+/                            <* 
 *>    { "ROOT"   , '-', NULL,     0.00, NULL, NULL,  -1,  -1,  -1,   NULL },         <* 
 *>    /+---(done)--------------------------------------+/                            <* 
 *>    { "0a1"    , '-', NULL,     0.00, NULL, NULL,   0,   0,   0,   NULL },         <* 
 *>    { "0a2"    , '-', NULL,     0.00, NULL, NULL,   0,   1,   0,   NULL },         <* 
 *>    { "0a3"    , '-', NULL,     0.00, NULL, NULL,   0,   2,   0,   NULL },         <* 
 *>    { "0a4"    , '-', NULL,     0.00, NULL, NULL,   0,   3,   0,   NULL },         <* 
 *>    { "0a5"    , '-', NULL,     0.00, NULL, NULL,   0,   4,   0,   NULL },         <* 
 *>    { "0a6"    , '-', NULL,     0.00, NULL, NULL,   0,   5,   0,   NULL },         <* 
 *>    { "0a7"    , '-', NULL,     0.00, NULL, NULL,   0,   6,   0,   NULL },         <* 
 *>    { "0a8"    , '-', NULL,     0.00, NULL, NULL,   0,   7,   0,   NULL },         <* 
 *>    { "0a9"    , '-', NULL,     0.00, NULL, NULL,   0,   8,   0,   NULL },         <* 
 *>    { "0a10"   , '-', NULL,     0.00, NULL, NULL,   0,   9,   0,   NULL },         <* 
 *>    { "0a11"   , '-', NULL,     0.00, NULL, NULL,   0,  10,   0,   NULL },         <* 
 *>    { "0a12"   , '-', NULL,     0.00, NULL, NULL,   0,  11,   0,   NULL },         <* 
 *>    { "0a30"   , '-', NULL,     0.00, NULL, NULL,   0,  29,   0,   NULL },         <* 
 *>    { "0a42"   , '-', NULL,     0.00, NULL, NULL,   0,  41,   0,   NULL },         <* 
 *>    { "1a1"    , '-', NULL,     0.00, NULL, NULL,   0,   0,   1,   NULL },         <* 
 *>    { "1a2"    , '-', NULL,     0.00, NULL, NULL,   0,   1,   1,   NULL },         <* 
 *>    { "1a3"    , '-', NULL,     0.00, NULL, NULL,   0,   2,   1,   NULL },         <* 
 *>    { "1a4"    , '-', NULL,     0.00, NULL, NULL,   0,   3,   1,   NULL },         <* 
 *>    { "0b1"    , '-', NULL,     0.00, NULL, NULL,   1,   0,   0,   NULL },         <* 
 *>    { "0b2"    , '-', NULL,     0.00, NULL, NULL,   1,   1,   0,   NULL },         <* 
 *>    { "0b3"    , '-', NULL,     0.00, NULL, NULL,   1,   2,   0,   NULL },         <* 
 *>    { "0b4"    , '-', NULL,     0.00, NULL, NULL,   1,   3,   0,   NULL },         <* 
 *>    { "0b5"    , '-', NULL,     0.00, NULL, NULL,   1,   4,   0,   NULL },         <* 
 *>    { "0b6"    , '-', NULL,     0.00, NULL, NULL,   1,   5,   0,   NULL },         <* 
 *>    { "0b7"    , '-', NULL,     0.00, NULL, NULL,   1,   6,   0,   NULL },         <* 
 *>    { "0b8"    , '-', NULL,     0.00, NULL, NULL,   1,   7,   0,   NULL },         <* 
 *>    { "0b9"    , '-', NULL,     0.00, NULL, NULL,   1,   8,   0,   NULL },         <* 
 *>    { "0b10"   , '-', NULL,     0.00, NULL, NULL,   1,   9,   0,   NULL },         <* 
 *>    { "0b11"   , '-', NULL,     0.00, NULL, NULL,   1,  10,   0,   NULL },         <* 
 *>    { "0b12"   , '-', NULL,     0.00, NULL, NULL,   1,  11,   0,   NULL },         <* 
 *>    { "0b13"   , '-', NULL,     0.00, NULL, NULL,   1,  12,   0,   NULL },         <* 
 *>    { "0b14"   , '-', NULL,     0.00, NULL, NULL,   1,  13,   0,   NULL },         <* 
 *>    { "0b15"   , '-', NULL,     0.00, NULL, NULL,   1,  14,   0,   NULL },         <* 
 *>    { "0b16"   , '-', NULL,     0.00, NULL, NULL,   1,  15,   0,   NULL },         <* 
 *>    { "0b17"   , '-', NULL,     0.00, NULL, NULL,   1,  17,   0,   NULL },         <* 
 *>    { "0c1"    , '-', NULL,     0.00, NULL, NULL,   2,   0,   0,   NULL },         <* 
 *>    { "0c2"    , '-', NULL,     0.00, NULL, NULL,   2,   1,   0,   NULL },         <* 
 *>    { "0c3"    , '-', NULL,     0.00, NULL, NULL,   2,   2,   0,   NULL },         <* 
 *>    { "0c4"    , '-', NULL,     0.00, NULL, NULL,   2,   3,   0,   NULL },         <* 
 *>    { "0c5"    , '-', NULL,     0.00, NULL, NULL,   2,   4,   0,   NULL },         <* 
 *>    { "0c6"    , '-', NULL,     0.00, NULL, NULL,   2,   5,   0,   NULL },         <* 
 *>    { "0c7"    , '-', NULL,     0.00, NULL, NULL,   2,   6,   0,   NULL },         <* 
 *>    { "0c8"    , '-', NULL,     0.00, NULL, NULL,   2,   7,   0,   NULL },         <* 
 *>    { "0c9"    , '-', NULL,     0.00, NULL, NULL,   2,   8,   0,   NULL },         <* 
 *>    { "0c10"   , '-', NULL,     0.00, NULL, NULL,   2,   9,   0,   NULL },         <* 
 *>    { "0c11"   , '-', NULL,     0.00, NULL, NULL,   2,  10,   0,   NULL },         <* 
 *>    { "0c12"   , '-', NULL,     0.00, NULL, NULL,   2,  11,   0,   NULL },         <* 
 *>    { "0c13"   , '-', NULL,     0.00, NULL, NULL,   2,  12,   0,   NULL },         <* 
 *>    { "0c14"   , '-', NULL,     0.00, NULL, NULL,   2,  13,   0,   NULL },         <* 
 *>    { "0c15"   , '-', NULL,     0.00, NULL, NULL,   2,  14,   0,   NULL },         <* 
 *>    { "0c16"   , '-', NULL,     0.00, NULL, NULL,   2,  15,   0,   NULL },         <* 
 *>    { "0c17"   , '-', NULL,     0.00, NULL, NULL,   2,  16,   0,   NULL },         <* 
 *>    { "0d1"    , '-', NULL,     0.00, NULL, NULL,   3,   0,   0,   NULL },         <* 
 *>    { "0d2"    , '-', NULL,     0.00, NULL, NULL,   3,   1,   0,   NULL },         <* 
 *>    { "0d3"    , '-', NULL,     0.00, NULL, NULL,   3,   2,   0,   NULL },         <* 
 *>    { "0d4"    , '-', NULL,     0.00, NULL, NULL,   3,   3,   0,   NULL },         <* 
 *>    { "0d5"    , '-', NULL,     0.00, NULL, NULL,   3,   4,   0,   NULL },         <* 
 *>    { "0d6"    , '-', NULL,     0.00, NULL, NULL,   3,   5,   0,   NULL },         <* 
 *>    { "0d7"    , '-', NULL,     0.00, NULL, NULL,   3,   6,   0,   NULL },         <* 
 *>    { "0d8"    , '-', NULL,     0.00, NULL, NULL,   3,   7,   0,   NULL },         <* 
 *>    { "0d9"    , '-', NULL,     0.00, NULL, NULL,   3,   8,   0,   NULL },         <* 
 *>    { "0d10"   , '-', NULL,     0.00, NULL, NULL,   3,   9,   0,   NULL },         <* 
 *>    { "0d11"   , '-', NULL,     0.00, NULL, NULL,   3,  10,   0,   NULL },         <* 
 *>    { "0d12"   , '-', NULL,     0.00, NULL, NULL,   3,  11,   0,   NULL },         <* 
 *>    { "0d13"   , '-', NULL,     0.00, NULL, NULL,   3,  12,   0,   NULL },         <* 
 *>    { "0d14"   , '-', NULL,     0.00, NULL, NULL,   3,  13,   0,   NULL },         <* 
 *>    { "0d15"   , '-', NULL,     0.00, NULL, NULL,   3,  14,   0,   NULL },         <* 
*>    { "0d16"   , '-', NULL,     0.00, NULL, NULL,   3,  15,   0,   NULL },         <* 
*>    { "0d17"   , '-', NULL,     0.00, NULL, NULL,   3,  16,   0,   NULL },         <* 
*>    { "0d18"   , '-', NULL,     0.00, NULL, NULL,   3,  17,   0,   NULL },         <* 
*>    /+---(two-way dependency testing)----------------+/                            <* 
*>    { "0f6"    , '-', NULL,     0.00, NULL, NULL,   5,   5,   0,   NULL },         <* 
*>    { "0f7"    , '-', NULL,     0.00, NULL, NULL,   5,   6,   0,   NULL },         <* 
*>    { "0g6"    , '-', NULL,     0.00, NULL, NULL,   6,   5,   0,   NULL },         <* 
*>    { "0g7"    , '-', NULL,     0.00, NULL, NULL,   6,   6,   0,   NULL },         <* 
*>    { "0h8"    , '-', NULL,     0.00, NULL, NULL,   7,   7,   0,   NULL },         <* 
*>    { "0k11"   , '-', NULL,     0.00, NULL, NULL,  10,  10,   0,   NULL },         <* 
*>    { "1k11"   , '-', NULL,     0.00, NULL, NULL,  10,  10,   1,   NULL },         <* 
*>    { "2k11"   , '-', NULL,     0.00, NULL, NULL,  10,  10,   2,   NULL },         <* 
*>    { "3k11"   , '-', NULL,     0.00, NULL, NULL,  10,  10,   3,   NULL },         <* 
*>    { "4k11"   , '-', NULL,     0.00, NULL, NULL,  10,  10,   4,   NULL },         <* 
*>    { "5d50"   , '-', NULL,     0.00, NULL, NULL,   3,  49,   5,   NULL },         <* 
*>    { "Aa1"    , '-', NULL,     0.00, NULL, NULL,   0,   0,  10,   NULL },         <* 
*>    /+---(done)--------------------------------------+/                            <* 
*>    { ""       , '-', NULL,     0.00, NULL, NULL,   0,   0,   0,   NULL },         <* 
*> };                                                                                <*/

char         /*-> create a new data entry ------------[ ------ [fc.B52.112.94]*/ /*-[40.0000.144.M]-*/ /*-[--.---.---.--]-*/
ycalc__mock_new          (tMOCK **a_owner)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   void       *x_new       = NULL;
   tMOCK      *x_owner     = NULL;
   int         x_tries     =    0;
   /*---(header)-------------------------*/
   DEBUG_DEPS   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_DEPS   yLOG_spoint  (a_owner);
   --rce;  if (a_owner == NULL) {
      DEBUG_DEPS   yLOG_snote   ("no return entry pointer");
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_spoint  (*a_owner);
   --rce;  if (*a_owner != NULL) {
      DEBUG_DEPS   yLOG_snote   ("entry pointer already assigned");
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(create)-------------------------*/
   DEBUG_DEPS   yLOG_snote   ("malloc");
   while (x_new == NULL && x_tries < 10) {
      x_new = malloc (sizeof (tMOCK));
      ++x_tries;
   }
   DEBUG_DEPS   yLOG_sint    (x_tries);
   --rce;  if (x_new == NULL) {
      DEBUG_DEPS   yLOG_snote   ("FAILED");
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_owner = (tMOCK *) x_new;
   /*---(data)---------------------------*/
   DEBUG_DEPS   yLOG_snote   ("data");
   x_owner->label  = NULL;
   x_owner->type   = G_DEP_BLANK;
   x_owner->source = NULL;
   x_owner->value  =  0.0;
   x_owner->string = NULL;
   x_owner->print  = NULL;
   /*---(list)---------------------------*/
   DEBUG_DEPS   yLOG_snote   ("list");
   x_owner->prev   = NULL;
   x_owner->next   = NULL;
   if (myCALC.mtail == NULL) {
      myCALC.mhead       = x_owner;
   } else {
      x_owner->prev      = myCALC.mtail;
      x_owner->next      = NULL;
      myCALC.mtail->next = x_owner;
   }
   myCALC.mtail    = x_owner;
   ++myCALC.mcount;
   DEBUG_DEPS   yLOG_sint    (myCALC.mcount);
   /*---(cell pointers)------------------*/
   DEBUG_DEPS   yLOG_snote   ("deproot");
   x_owner->ycalc  = NULL;
   /*---(save)---------------------------*/
   DEBUG_DEPS   yLOG_snote   ("save");
   *a_owner        = x_owner;
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char         /*-> create a new data entry ------------[ ------ [fc.B52.112.94]*/ /*-[40.0000.144.M]-*/ /*-[--.---.---.--]-*/
ycalc__mock_new_labeled  (char *a_label, void **a_owner)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_label     [LEN_LABEL];
   tMOCK      *x_owner     = NULL;
   /*---(header)-------------------------*/
   DEBUG_CELL   yLOG_enter   (__FUNCTION__);
   DEBUG_CELL   yLOG_point   ("a_owner"   , a_owner);
   --rce;  if (a_owner  == NULL) {
      DEBUG_CELL   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CELL   yLOG_point   ("*a_owner"  , *a_owner);
   --rce;  if (*a_owner != NULL) {
      DEBUG_CELL   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = str6gyges  (a_label, 0, x_label);
   DEBUG_CELL   yLOG_value   ("str6gyges" , rc);
   --rce;  if (rc < 0) {
      DEBUG_CELL   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = ycalc__mock_new (&x_owner);
   DEBUG_CELL   yLOG_value   ("new"       , rc);
   --rce;  if (rc < 0) {
      DEBUG_CELL   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_owner->label = strdup (x_label);
   *a_owner = x_owner;
   DEBUG_CELL   yLOG_point   ("*a_owner"  , *a_owner);
   DEBUG_CELL   yLOG_info    ("label"     , x_owner->label);
   DEBUG_CELL   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> create a new data entry ------------[ ------ [fc.B52.112.94]*/ /*-[40.0000.144.M]-*/ /*-[--.---.---.--]-*/
ycalc__mock_new_at       (int x, int y, int z, tMOCK **a_owner)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_label     [LEN_LABEL];
   tMOCK      *x_owner     = NULL;
   --rce;  if (a_owner  == NULL)  return rce;
   --rce;  if (*a_owner != NULL)  return rce;
   rc = str4gyges  (x, y, z, 0, x_label);
   --rce;  if (rc < 0) return rce;
   x_owner = (tMOCK *) *a_owner;
   rc = ycalc__mock_new (&x_owner);
   --rce;  if (rc < 0) return rce;
   x_owner->label = strdup (x_label);
   *a_owner = x_owner;
   return 0;
}

char         /*-> clear values from a data entry -----[ ------ [fc.B52.112.94]*/ /*-[40.0000.144.M]-*/ /*-[--.---.---.--]-*/
ycalc__mock_wipe     (tMOCK *a_owner)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(beginning)----------------------*/
   DEBUG_CELL   yLOG_senter  (__FUNCTION__);
   /*---(defenses)-----------------------*/
   DEBUG_DEPS   yLOG_spoint  (a_owner);
   --rce;  if (a_owner == NULL) {
      DEBUG_DEPS   yLOG_snote   ("no entry pointer");
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(data)---------------------------*/
   DEBUG_CELL   yLOG_snote   ("type");
   a_owner->type   = G_DEP_BLANK;
   DEBUG_CELL   yLOG_snote   ("source");
   if (a_owner->source    != NULL)  free (a_owner->source);
   a_owner->source         = NULL;
   DEBUG_CELL   yLOG_snote   ("value");
   a_owner->value          =  0.0;
   DEBUG_CELL   yLOG_snote   ("string");
   if (a_owner->string    != NULL)  free (a_owner->string);
   a_owner->string         = NULL;
   DEBUG_CELL   yLOG_snote   ("print");
   if (a_owner->print     != NULL)  free (a_owner->print);
   a_owner->print          = NULL;
   /*---(complete)-----------------------*/
   DEBUG_CELL   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char         /*-> destroy an existing data entry -----[ ------ [fc.B52.112.94]*/ /*-[40.0000.144.M]-*/ /*-[--.---.---.--]-*/
ycalc__mock_free         (tMOCK **a_owner)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tMOCK      *x_owner     = NULL;
   /*---(clear data)---------------------*/
   rc = ycalc__mock_wipe (*a_owner);
   if (rc < 0)  return rc;
   /*---(header)-------------------------*/
   DEBUG_CELL   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_DEPS   yLOG_spoint  (a_owner);
   --rce;  if (a_owner == NULL) {
      DEBUG_DEPS   yLOG_snote   ("no return entry pointer");
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_spoint  (*a_owner);
   --rce;  if (*a_owner == NULL) {
      DEBUG_DEPS   yLOG_snote   ("entry pointer not assigned");
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_owner = *a_owner;
   /*---(remove from list)---------------*/
   DEBUG_DEPS   yLOG_snote   ("list");
   if (x_owner->next != NULL) x_owner->next->prev = x_owner->prev;
   else                       myCALC.mtail        = x_owner->prev;
   if (x_owner->prev != NULL) x_owner->prev->next = x_owner->next;
   else                       myCALC.mhead        = x_owner->next;
   --myCALC.mcount;
   DEBUG_DEPS   yLOG_sint    (myCALC.mcount);
   /*---(save)---------------------------*/
   DEBUG_DEPS   yLOG_snote   ("free");
   free (x_owner);
   DEBUG_DEPS   yLOG_snote   ("save");
   *a_owner        = NULL;
   /*---(complete)-----------------------*/
   DEBUG_CELL   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      deproot functions                       ----===*/
/*====================------------------------------------====================*/
static void      o___DEPROOT_________________o (void) {;}

char
ycalc__mock_prepare     (void)
{
   char        rc          =    0;
   myCALC.mroot = NULL;
   if (rc == 0)  rc = ycalc__mock_new_labeled ("ROOT", &myCALC.mroot);
   if (rc == 0)  rc = yCALC_exist_config (ycalc__mock_enabler, ycalc__mock_pointer, ycalc__mock_reaper);
   if (rc == 0)  rc = yCALC_label_config (ycalc__mock_named  , ycalc__mock_whos_at, ycalc__mock_labeler);
   if (rc == 0)  rc = yCALC_value_config (ycalc__mock_valuer , ycalc__mock_address, ycalc__mock_special);
   return rc;
}

char
ycalc__mock_cleanup     (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tMOCK      *x_owner     = NULL;
   tMOCK      *x_save      = NULL;
   /*---(search)-------------------------*/
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   x_owner = myCALC.mroot;
   while (x_owner != NULL) {
      x_save  = x_owner->next;
      DEBUG_DEPS   yLOG_point   ("before"     , x_owner);
      ycalc_call_reaper (&(x_owner->ycalc));
      ycalc__mock_free (&x_owner);
      DEBUG_DEPS   yLOG_point   ("after"      , x_owner);
      x_owner = x_save;
   }
   myCALC.mroot = NULL;
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc__mock_enabler     (void *a_owner, void *a_deproot)
{
   char        rce         =  -10;
   int         i           =    0;
   tMOCK      *x_mock      = NULL;
   DEBUG_DEPS   yLOG_senter  (__FUNCTION__);
   DEBUG_DEPS   yLOG_spoint  (a_owner);
   --rce;  if (a_owner == NULL) {
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_mock = (tMOCK *) a_owner;
   DEBUG_DEPS   yLOG_spoint  (x_mock->ycalc);
   --rce;  if (a_deproot != NULL && x_mock->ycalc != NULL) {
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_spoint  (a_deproot);
   x_mock->ycalc = a_deproot;
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ycalc__mock_named       (char *a_label, char a_force, void **a_owner, void **a_deproot)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tMOCK      *x_owner     = NULL;
   static char    x_sforce =  '?';
   static char   *x_label  [LEN_LABEL];
   static tMOCK  *x_saved  = NULL;
   /*---(header)-------------------------*/
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   if (a_owner   != NULL)  *a_owner   = NULL;
   if (a_deproot != NULL)  *a_deproot = NULL;
   /*---(defense)------------------------*/
   DEBUG_DEPS   yLOG_point   ("a_label"   , a_label);
   --rce;  if (a_label == NULL) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_info    ("a_label"   , a_label);
   /*---(shortcut)-----------------------*/
   if (strcmp (x_label, a_label) == 0 && a_force == x_sforce) {
      DEBUG_DEPS   yLOG_note    ("short-cut");
      if (a_owner   != NULL) {
         *a_owner   = x_saved;
         DEBUG_DEPS   yLOG_point   ("*a_owner"  , *a_owner);
      }
      if (a_deproot != NULL) {
         *a_deproot = x_saved->ycalc;
         DEBUG_DEPS   yLOG_point   ("*a_deproot", *a_deproot);
      }
      DEBUG_PROG   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(search)-------------------------*/
   x_owner = myCALC.mroot;
   DEBUG_DEPS   yLOG_point   ("mroot"     , myCALC.mroot);
   while (x_owner != NULL) {
      DEBUG_DEPS   yLOG_point   ("next"      , x_owner);
      if (x_owner->label != NULL) {
         DEBUG_DEPS   yLOG_info    ("checking"  , x_owner->label);
         if (strcmp (x_owner->label, a_label) == 0) {
            DEBUG_DEPS   yLOG_note    ("FOUND");
            break; 
         }
      }
      x_owner = x_owner->next;
   }
   DEBUG_DEPS   yLOG_point   ("x_owner"    , x_owner);
   /*---(handle failure)-----------------*/
   --rce;  if (x_owner == NULL && a_force != YCALC_LOOK) {
      DEBUG_DEPS   yLOG_note    ("create a new one");
      rc = ycalc__mock_new_labeled (a_label, &x_owner);
      if (rc < 0) {
         DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   DEBUG_DEPS   yLOG_point   ("x_owner"    , x_owner);
   --rce;  if (x_owner == NULL) {
      DEBUG_DEPS   yLOG_note    ("FAILURE");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save)---------------------------*/
   x_sforce = a_force;
   x_saved  = x_owner;
   strlcpy (x_label, a_label, LEN_LABEL);
   DEBUG_DEPS   yLOG_point   ("x_saved"   , x_saved);
   /*---(handle normal)------------------*/
   DEBUG_DEPS   yLOG_note    ("success");
   if (a_owner   != NULL) {
      *a_owner   = x_owner;
      DEBUG_DEPS   yLOG_point   ("*a_owner"  , *a_owner);
   }
   if (a_deproot != NULL) {
      *a_deproot = x_owner->ycalc;
      DEBUG_DEPS   yLOG_point   ("*a_deproot", *a_deproot);
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc__mock_whos_at     (int x, int y, int z, char a_force, void **a_owner, void **a_deproot)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   char        x_label     [LEN_LABEL];
   /*---(legal)--------------------------*/
   rc = str4gyges (x, y, z, 0, x_label);
   if (rc == 0)  rc = ycalc__mock_named (x_label, YCALC_LOOK, a_owner, a_deproot);
   /*---(complete)-----------------------*/
   return rc;
}



/*====================------------------------------------====================*/
/*===----                       owner functions                        ----===*/
/*====================------------------------------------====================*/
static void      o___OWNER___________________o (void) {;}

char*
ycalc__mock_labeler     (void *a_owner)
{
   tMOCK      *x_mock      = NULL;
   DEBUG_DEPS   yLOG_senter  (__FUNCTION__);
   DEBUG_DEPS   yLOG_spoint  (a_owner);
   if (a_owner == NULL) {
      DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
      return g_nada;
   }
   x_mock = (tMOCK *) a_owner;
   DEBUG_DEPS   yLOG_snote   (x_mock->label);
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
   return x_mock->label;
}

char
ycalc__mock_reaper      (void *a_owner)
{
   return 0;
}

char
ycalc__mock_pointer     (void *a_owner, char **a_source, char **a_type, double **a_value, char **a_string)
{
   char        rce         =  -10;
   tMOCK      *x_mock      = NULL;
   DEBUG_DEPS   yLOG_senter  (__FUNCTION__);
   DEBUG_DEPS   yLOG_spoint  (a_owner);
   --rce;  if (a_owner  == NULL) {
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_mock    = (tMOCK     *) a_owner;
   if (a_source != NULL) {
      DEBUG_DEPS   yLOG_spoint  (*a_source);
      *a_source = &x_mock->source;
      DEBUG_DEPS   yLOG_spoint  (*a_source);
   }
   if (a_type  != NULL) {
      DEBUG_DEPS   yLOG_spoint  (*a_type);
      *a_type   = &x_mock->type;
      DEBUG_DEPS   yLOG_spoint  (*a_type);
   }
   if (a_value  != NULL) {
      DEBUG_DEPS   yLOG_spoint  (*a_value);
      *a_value  = &x_mock->value;
      DEBUG_DEPS   yLOG_spoint  (*a_value);
   }
   if (a_string != NULL) {
      DEBUG_DEPS   yLOG_spoint  (*a_string);
      *a_string = &x_mock->string;
      DEBUG_DEPS   yLOG_spoint  (*a_string);
   }
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ycalc__mock_valuer      (void *a_owner, char *a_type, double *a_value, char **a_string)
{
   char        rce         =  -10;
   tMOCK      *x_mock      = NULL;
   DEBUG_DEPS   yLOG_senter  (__FUNCTION__);
   DEBUG_DEPS   yLOG_spoint  (a_owner);
   if (a_type   != NULL)  *a_type   = '-';
   if (a_value  != NULL)  *a_value  = 0.0;
   if (a_string != NULL)  *a_string = g_nada;
   --rce;  if (a_owner  == NULL) {
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_mock    = (tMOCK     *) a_owner;
   if (a_type   != NULL) {
      *a_type   = x_mock->type;
      DEBUG_DEPS   yLOG_schar   (*a_type);
   }
   if (a_value  != NULL) {
      *a_value  = x_mock->value;
      DEBUG_DEPS   yLOG_sdouble (*a_value);
   }
   if (a_string != NULL) {
      if      (x_mock->type == 'E')     *a_string   = g_nada;
      else if (x_mock->type == 's' && x_mock->source != NULL)  *a_string = x_mock->source;
      else if (x_mock->string != NULL)  *a_string = x_mock->string;
      else                              *a_string = g_nada;
      DEBUG_DEPS   yLOG_snote   (*a_string);
   }
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ycalc__mock_address     (void *a_owner, int *x, int *y, int *z)
{
   tMOCK      *x_mock      = NULL;
   if (x != NULL)  *x   = 0;
   if (y != NULL)  *y   = 0;
   if (z != NULL)  *z   = 0;
   if (a_owner == NULL)  return -1;
   x_mock    = (tMOCK     *) a_owner;
   str2gyges (x_mock->label, x, y, z, NULL, 0);
   return 0;
}

char
ycalc__mock_special     (void *a_owner, char a_what, double *a_value, char **a_string)
{
   char        rce         =  -10;
   tMOCK      *x_owner     = NULL;
   tDEP_ROOT  *x_deproot   = NULL;
   DEBUG_DEPS   yLOG_senter  (__FUNCTION__);
   DEBUG_DEPS   yLOG_spoint  (a_owner);
   DEBUG_DEPS   yLOG_schar   (a_what);
   if (a_value  != NULL)  *a_value  = 0.0;
   if (a_string != NULL)  *a_string = g_nada;
   --rce;  if (a_owner   == NULL) {
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_owner   = (tMOCK     *) a_owner;
   x_deproot = (tDEP_ROOT *) x_owner->ycalc;
   --rce;  if (x_deproot == NULL) {
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  switch (a_what) {
   case G_SPECIAL_SOURCE :
      if (a_string != NULL)  *a_string = x_owner->source;
      break;
   case G_SPECIAL_PRINT  :
      if (a_string != NULL)  *a_string = x_owner->print;
      break;
   case G_SPECIAL_TYPE   :
      if (a_value  != NULL)  *a_value  = x_owner->type;
      break;
   }
   DEBUG_DEPS   yLOG_snote   (*a_string);
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ycalc__mock_source      (char *a_label, char *a_source)
{
   char        rce         =  -10;
   char        rc          =    0;
   tMOCK      *x_owner     = NULL;
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   rc = ycalc_call_who_named (a_label, YCALC_OWNR, &x_owner, NULL);
   DEBUG_DEPS   yLOG_value   ("rc"        , rc);
   --rce;  if (rc < 0) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_point   ("source"    , x_owner->source);
   DEBUG_DEPS   yLOG_info    ("source"    , x_owner->source);
   if (x_owner->source != NULL) {
      DEBUG_DEPS   yLOG_note    ("must free");
      free (x_owner->source);
      x_owner->source = NULL;
   }
   if (a_source != NULL) {
      x_owner->source = strdup (a_source);
      DEBUG_DEPS   yLOG_point   ("source"    , x_owner->source);
      DEBUG_DEPS   yLOG_info    ("source"    , x_owner->source);
   }
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc__mock_special_set (char *a_label, char *a_source, double a_value, char *a_print)
{
   char        rce         =  -10;
   char        rc          =    0;
   tMOCK      *x_owner     = NULL;
   rc = ycalc_call_who_named (a_label, YCALC_OWNR, &x_owner, NULL);
   --rce;  if (rc < 0)  return rce;
   if (a_source != NULL) {
      if (x_owner->source != NULL) {
         free (x_owner->source);
         x_owner->source = NULL;
      }
      x_owner->source = strdup (a_source);
   }
   if (a_value  != -666) {
      x_owner->value  = a_value;
   }
   if (a_print  != NULL) {
      if (x_owner->print != NULL) {
         free (x_owner->print);
         x_owner->print = NULL;
      }
      x_owner->print  = strdup (a_print);
   }
   return 0;
}

char
ycalc__mock_whole       (char *a_label, char *a_source, char a_format, char a_decs, char a_align, char a_width)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tMOCK      *x_owner     = NULL;
   char        t           [LEN_RECD];
   char        x_out       [LEN_RECD];
   char        x_len       =    0;
   FILE       *f           = NULL;
   /*---(header)-------------------------*/
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   /*---(get location)-------------------*/
   rc = ycalc__mock_source (a_label, a_source);
   DEBUG_DEPS   yLOG_value   ("source"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = ycalc_call_who_named (a_label, YCALC_OWNR, &x_owner, NULL);
   if (x_owner->print  != NULL) {
      free (x_owner->print);
      x_owner->print  = NULL;
   }
   /*---(classify)-----------------------*/
   rc = yCALC_handle (a_label);
   /*---(handle results)-----------------*/
   if (strchr ("=n", x_owner->type) != NULL) {
      strl4main (x_owner->value, t, a_decs, a_format, LEN_RECD);
   } else {
      if (x_owner->string != NULL)  strlcpy (t, x_owner->string, LEN_RECD);
      else                          strlcpy (t, x_owner->source, LEN_RECD);
   }
   strlpad (t, x_out, ' ', a_align, a_width - 1);
   x_owner->print = strdup (x_out);
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNIT_TEST_______o () { return; }

char*        /*-> unit testing accessor --------------[ ------ [gs.HA0.1B3.K5]*/ /*-[02.0000.00#.#]-*/ /*-[--.---.---.--]-*/
ycalc__unit_mock        (char *a_question, char *a_label)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         x_fore      =    0;
   int         x_back      =    0;
   tMOCK      *x_owner     = NULL;
   tDEP_ROOT  *x_deproot   = NULL;
   char        x_label     [LEN_LABEL];
   char        x_rnote     [LEN_LABEL];
   char        x_onote     [LEN_LABEL];
   char        x_dnote     [LEN_LABEL];
   char        x_string    [LEN_RECD ];
   double      x_value     =  0.0;
   /*---(preprare)-----------------------*/
   strcpy (ycalc__unit_answer, "yCALC            : question not understood");
   /*---(label)--------------------------*/
   if (a_label == NULL)   strlcpy (x_label, "---"  , LEN_LABEL);
   else                   strlcpy (x_label, a_label, LEN_LABEL);
   /*---(owner/deproot)------------------*/
   rc = ycalc__mock_named (x_label, YCALC_LOOK, &x_owner, &x_deproot);
   if (rc        <  0   )  strlcpy (x_rnote , "-----"   , LEN_LABEL);
   else                    strlcpy (x_rnote , "good"    , LEN_LABEL);
   if (x_owner   == NULL)  strlcpy (x_onote , "-----"   , LEN_LABEL);
   else                    strlcpy (x_onote , "exists"  , LEN_LABEL);
   if (x_deproot == NULL)  strlcpy (x_dnote , "-----"   , LEN_LABEL);
   else                    strlcpy (x_dnote , "enabled" , LEN_LABEL);
   /*---(string)-------------------------*/
   if      (x_owner == NULL)              strlcpy (x_string, "", LEN_RECD );
   else if (strchr ("s#5E", x_owner->type) == NULL)   strlcpy (x_string, "", LEN_RECD );
   else if (x_owner->string != NULL)      strlcpy (x_string, x_owner->string, LEN_RECD );
   else if (x_owner->type == 's' && x_owner->source != NULL)         strlcpy (x_string, x_owner->source, LEN_RECD );
   else                                   strlcpy (x_string, "", LEN_RECD );
   /*---(value)--------------------------*/
   if      (x_owner == NULL)              x_value = 0.0;
   else                                   x_value = x_owner->value;
   /*---(dependency list)----------------*/
   if      (strcmp (a_question, "count"    )      == 0) {
      x_owner  = myCALC.mhead; while (x_owner  != NULL) { ++x_fore; x_owner  = x_owner ->next; }
      x_owner  = myCALC.mtail; while (x_owner  != NULL) { ++x_back; x_owner  = x_owner ->prev; }
      snprintf (ycalc__unit_answer, LEN_RECD, "yCALC mock count : num=%4d, fore=%4d, back=%4d", myCALC.mcount, x_fore, x_back);
   }
   else if (strcmp (a_question, "root_count"  )   == 0) {
      x_owner  = myCALC.mhead; while (x_owner  != NULL) { if (x_owner->ycalc != NULL) ++x_fore; x_owner  = x_owner ->next; }
      x_owner  = myCALC.mtail; while (x_owner  != NULL) { if (x_owner->ycalc != NULL) ++x_back; x_owner  = x_owner ->prev; }
      snprintf (ycalc__unit_answer, LEN_RECD, "yCALC root count : num=%4d, fore=%4d, back=%4d", myCALC.rcount, x_fore, x_back);
   }
   else if (strcmp (a_question, "linked_list" )   == 0) {
      if (myCALC.mhead == NULL) {
         snprintf (ycalc__unit_answer, LEN_RECD, "yCALC mock list  : num=%4d, head=%-10s, tail=%s", myCALC.mcount, NULL, NULL);
      } else {
         snprintf (ycalc__unit_answer, LEN_RECD, "yCALC mock list  : num=%4d, head=%-10s, tail=%s", myCALC.mcount, myCALC.mhead->label, myCALC.mtail->label);
      }
   }
   else if (strcmp (a_question, "status"   )      == 0) {
      snprintf (ycalc__unit_answer, LEN_RECD, "yCALC mock status: %-5s %-8s %-10s %s", x_label, x_rnote, x_onote, x_dnote);
   }
   else if (strcmp (a_question, "value"    )      == 0) {
      snprintf (ycalc__unit_answer, LEN_RECD, "yCALC mock value : %-5s %c %8.2lf :%-.45s:", x_label, x_owner->type, x_value, x_string);
   }
   else if (strcmp (a_question, "double"   )      == 0) {
      snprintf (ycalc__unit_answer, LEN_RECD, "yCALC mock double: %-5s %c %16.6lf", x_label, x_owner->type, x_value);
   }
   /*---(complete)-----------------------*/
   return ycalc__unit_answer;
}



