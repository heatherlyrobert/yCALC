/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"




/*====================------------------------------------====================*/
/*===----                         mock system                          ----===*/
/*====================------------------------------------====================*/
static void      o___MOCK____________________o (void) {;}

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
      /*> printf ("malloc () ycalc__mock_new\n");                                     <*/
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
   x_owner->width  =    8;
   /*---(printing)-----------------------*/
   x_owner->align  =  '<';
   x_owner->format =  'i';
   x_owner->decs   =    0;
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
   rc = str6gyges  (a_label, 0, x_label, YSTR_LEGAL);
   DEBUG_CELL   yLOG_value   ("str6gyges" , rc);
   --rce;  if (rc < 0) {
      DEBUG_CELL   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_CELL   yLOG_info    ("a_label"   , a_label);
   DEBUG_CELL   yLOG_info    ("x_label"   , x_label);
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
ycalc__mock_new_at       (int b, int x, int y, int z, tMOCK **a_owner)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_label     [LEN_LABEL];
   tMOCK      *x_owner     = NULL;
   --rce;  if (a_owner  == NULL)  return rce;
   --rce;  if (*a_owner != NULL)  return rce;
   rc = str4gyges  (b, x, y, z, 0, x_label, YSTR_LEGAL);
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
   DEBUG_CELL   yLOG_snote   ("label");
   if (a_owner->label     != NULL)  free (a_owner->label);
   a_owner->label          = NULL;
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
ycalc__mock_insider     (int b, int x, int y, int z)
{
   return 0;
}

char
ycalc__mock_ystr_check  (int b, int x, int y, int z, char a_check)
{
   if (b  <    0)        return -1;
   if (b  >   37)        return -1;
   if (x  <    0)        return -1;
   if (x  >   20)        return -1;
   if (y  <    0)        return -1;
   if (y  >   30)        return -1;
   return 0;
}

char
ycalc__mock_prepare     (void)
{
   char        rc          =    0;
   myCALC.mroot = NULL;
   if (rc == 0)  rc = yCALC_exist_config (ycalc__mock_enabler, ycalc__mock_pointer, ycalc__mock_reaper);
   if (rc == 0)  rc = yCALC_label_config (ycalc__mock_named  , ycalc__mock_whos_at, ycalc__mock_labeler);
   if (rc == 0)  rc = yCALC_value_config (ycalc__mock_valuer , ycalc__mock_address, ycalc__mock_special, ycalc__mock_printer);
   str0gyges  (ycalc__mock_ystr_check);
   yRPN_init (YRPN_GYGES);
   rc = yRPN_addr_config   (str2gyges, str4gyges, str6gyges, str8gyges, ycalc__mock_insider);
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
   DEBUG_DEPS   yLOG_note    ("BEFORE");
   DEBUG_DEPS   yLOG_point   ("mroot"     , myCALC.mroot);
   DEBUG_DEPS   yLOG_point   ("mhead"     , myCALC.mhead);
   DEBUG_DEPS   yLOG_point   ("mtail"     , myCALC.mtail);
   DEBUG_DEPS   yLOG_value   ("mcount"    , myCALC.mcount);
   x_owner = myCALC.mroot;
   while (x_owner != NULL) {
      x_save  = x_owner->next;
      DEBUG_DEPS   yLOG_point   ("before"     , x_owner);
      ycalc_call_reaper (&x_owner, &(x_owner->ycalc));
      ycalc__mock_free  (&x_owner);
      DEBUG_DEPS   yLOG_point   ("after"      , x_owner);
      x_owner = x_save;
   }
   myCALC.mroot = NULL;
   DEBUG_DEPS   yLOG_note    ("AFTER");
   DEBUG_DEPS   yLOG_point   ("mroot"     , myCALC.mroot);
   DEBUG_DEPS   yLOG_point   ("mhead"     , myCALC.mhead);
   DEBUG_DEPS   yLOG_point   ("mtail"     , myCALC.mtail);
   DEBUG_DEPS   yLOG_value   ("mcount"    , myCALC.mcount);
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> tbd --------------------------------[ leaf   [fe.532.233.20]*/ /*-[01.0000.10#.!]-*/ /*-[--.---.---.--]-*/
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
   DEBUG_DEPS   yLOG_snote   (x_mock->label);
   DEBUG_DEPS   yLOG_spoint  (x_mock->ycalc);
   --rce;  if (a_deproot != NULL && x_mock->ycalc != NULL) {
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_spoint  (a_deproot);
   x_mock->ycalc = a_deproot;
   DEBUG_DEPS   yLOG_spoint  (x_mock->ycalc);
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ycalc__mock_lister      (void)
{
   /*---(locals)-----------+-----+-----+-*/
   tMOCK      *x_owner     = NULL;
   int         c           =    0;
   /*---(walk)---------------------------*/
   x_owner = myCALC.mroot;
   /*> printf ("mroot   = %p\n", myCALC.mroot);                                       <*/
   /*> printf ("mcount  = %d\n", myCALC.mcount);                                      <*/
   while (x_owner != NULL) {
      /*> printf ("next    = %p\n", x_owner);                                         <*/
      /*> if (x_owner->label != NULL) {                                               <* 
       *>    printf ("  label = %s\n", x_owner->label);                               <* 
       *> } else {                                                                    <* 
       *>    printf ("  label = %s\n", "NULL");                                       <* 
       *> }                                                                           <*/
      x_owner = x_owner->next;
      ++c;
   }
   /*> printf ("c       = %d\n", c);                                                  <*/
   /*---(complete)-----------------------*/
   return 0;
}

char
ycalc__mock_list        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   tMOCK      *x_owner     = NULL;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   /*---(walk)---------------------------*/
   x_owner = myCALC.mroot;
   DEBUG_DEPS   yLOG_point   ("mroot"     , myCALC.mroot);
   DEBUG_DEPS   yLOG_value   ("mcount"    , myCALC.mcount);
   while (x_owner != NULL) {
      DEBUG_DEPS   yLOG_point   ("next"      , x_owner);
      if (x_owner->label != NULL) {
         DEBUG_DEPS   yLOG_info    ("checking"  , x_owner->label);
      } else {
         DEBUG_DEPS   yLOG_info    ("checking"  , "label null");
      }
      x_owner = x_owner->next;
      ++c;
   }
   DEBUG_DEPS   yLOG_value   ("c"         , c);
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc__mock_named       (char *a_label, char a_force, void **a_owner, void **a_deproot)
{
   /*---(notes)--------------------------*/
   /*
    *  can not use short-cutting as reaper may kill owners between calls!!!
    *
    */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tMOCK      *x_owner     = NULL;
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
   DEBUG_DEPS   yLOG_char    ("a_force"   , a_force);
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
   /*---(handle failure)-----------------*/
   DEBUG_DEPS   yLOG_point   ("x_owner"    , x_owner);
   --rce;  if (x_owner == NULL && a_force == YCALC_LOOK) {
      DEBUG_DEPS   yLOG_note    ("owner does not exist and only in look mode");
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (x_owner == NULL) {
      DEBUG_DEPS   yLOG_note    ("create a new one");
      rc = ycalc__mock_new_labeled (a_label, &x_owner);
      if (rc < 0) {
         DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_DEPS   yLOG_point   ("x_owner"    , x_owner);
      /*---(check for root)--------------*/
      if (myCALC.mroot == NULL && strcmp (a_label, "ROOT") == 0) {
         DEBUG_DEPS   yLOG_note    ("saved as root");
         myCALC.mroot = x_owner;
      }
   }
   --rce;  if (x_owner == NULL) {
      DEBUG_DEPS   yLOG_note    ("FAILURE");
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
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
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc__mock_whos_at     (int b, int x, int y, int z, char a_force, void **a_owner, void **a_deproot)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   char        x_label     [LEN_LABEL];
   /*---(legal)--------------------------*/
   rc = str4gyges (b, x, y, z, 0, x_label, YSTR_LEGAL);
   if (rc == 0)  rc = ycalc__mock_named (x_label, a_force, a_owner, a_deproot);
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
ycalc__mock_reaper      (void **a_owner)
{
   char        rce         =  -10;
   char        rc          =    0;
   tMOCK      *x_mock      = NULL;
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   DEBUG_DEPS   yLOG_point   ("a_owner"   , a_owner);
   --rce;  if (a_owner  == NULL) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_point   ("*a_owner"  , *a_owner);
   --rce;  if (*a_owner  == NULL) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, -rce);
      return -rce;
   }
   x_mock = (tMOCK *) *a_owner;
   DEBUG_DEPS   yLOG_point   ("ycalc"     , x_mock->ycalc);
   --rce;  if (x_mock->ycalc  != NULL) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_char    ("type"      , x_mock->type);
   --rce;  if (x_mock->type  != YCALC_DATA_BLANK) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = ycalc__mock_free (a_owner);
   --rce;  if (rc < 0) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_point   ("*a_owner"  , *a_owner);
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc__mock_pointer     (void *a_owner, char **a_source, char **a_type, double **a_value, char **a_string)
{
   char        rce         =  -10;
   tMOCK      *x_mock      = NULL;
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   DEBUG_DEPS   yLOG_point   ("a_owner"   , a_owner);
   --rce;  if (a_owner  == NULL) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_mock    = (tMOCK     *) a_owner;
   if (a_source != NULL) {
      DEBUG_DEPS   yLOG_point   ("*a_source" , *a_source);
      *a_source = &x_mock->source;
      DEBUG_DEPS   yLOG_point   ("*a_source" , *a_source);
      DEBUG_DEPS   yLOG_point   ("->source"  , x_mock->source);
      if (x_mock->source != NULL)  DEBUG_DEPS   yLOG_info    ("->source"  , x_mock->source);
   }
   if (a_type  != NULL) {
      DEBUG_DEPS   yLOG_point   ("*a_type"   , *a_type);
      *a_type   = &x_mock->type;
      DEBUG_DEPS   yLOG_point   ("*a_type"   , *a_type);
   }
   if (a_value  != NULL) {
      DEBUG_DEPS   yLOG_point   ("*a_value"  , *a_value);
      *a_value  = &x_mock->value;
      DEBUG_DEPS   yLOG_point   ("*a_value"  , *a_value);
   }
   if (a_string != NULL) {
      DEBUG_DEPS   yLOG_point   ("*a_string" , *a_string);
      *a_string = &x_mock->string;
      DEBUG_DEPS   yLOG_point   ("*a_string" , *a_string);
      DEBUG_DEPS   yLOG_point   ("->string"  , x_mock->string);
      if (x_mock->string != NULL)  DEBUG_DEPS   yLOG_info    ("->string"  , x_mock->string);
   }
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
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
   if (a_string != NULL)  *a_string = NULL;
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
      if      (x_mock->type == 'E')     *a_string = x_mock->string;
      else if (x_mock->type == 's' && x_mock->source != NULL)  *a_string = x_mock->source;
      else if (x_mock->string != NULL)  *a_string = x_mock->string;
      DEBUG_DEPS   yLOG_spoint  (*a_string);
      DEBUG_DEPS   yLOG_snote   (*a_string);
   }
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ycalc__mock_address     (void *a_owner, int *b, int *x, int *y, int *z)
{
   tMOCK      *x_mock      = NULL;
   DEBUG_DEPS   yLOG_senter  (__FUNCTION__);
   DEBUG_DEPS   yLOG_spoint  (a_owner);
   if (b != NULL)  *b   = 0;
   if (x != NULL)  *x   = 0;
   if (y != NULL)  *y   = 0;
   if (z != NULL)  *z   = 0;
   if (a_owner == NULL) {
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, -1);
      return -1;
   }
   x_mock    = (tMOCK     *) a_owner;
   DEBUG_DEPS   yLOG_spoint  (x_mock);
   DEBUG_DEPS   yLOG_snote   (x_mock->label);
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
   str2gyges (x_mock->label, b, x, y, z, NULL, 0, YSTR_LEGAL);
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
   if (a_string != NULL)  *a_string = NULL;
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
      DEBUG_DEPS   yLOG_snote   ("source");
      DEBUG_DEPS   yLOG_snote   (x_owner->source);
      if (a_string != NULL)  *a_string = x_owner->source;
      break;
   case G_SPECIAL_PRINT  :
      DEBUG_DEPS   yLOG_snote   ("print");
      DEBUG_DEPS   yLOG_snote   (x_owner->print);
      if (a_string != NULL)  *a_string = x_owner->print;
      break;
   case G_SPECIAL_TYPE   :
      DEBUG_DEPS   yLOG_snote   ("type");
      DEBUG_DEPS   yLOG_schar   (x_owner->type);
      if (a_value  != NULL)  *a_value  = x_owner->type;
      break;
   }
   DEBUG_DEPS   yLOG_spoint  (*a_string);
   if (*a_string != NULL)  DEBUG_DEPS   yLOG_snote   (*a_string);
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
   return 0;
}


static int   s_merges =    0;
static void *s_owners [20];
static int   s_widths [20];

char         /*-> determine full print width ---------[ ------ [gz.530.321.23]*/ /*-[01.0000.015.!]-*/ /*-[--.---.---.--]-*/
ycalc__mock_width    (void *a_owner, int *a_width, int *a_merge)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   tMOCK      *x_owner     = NULL;
   char        x_label     [LEN_LABEL];
   int         b, x, y, z;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   /*---(legal)--------------------------*/
   DEBUG_DEPS   yLOG_point   ("a_owner"   , a_owner);
   if (a_owner == NULL) {
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   DEBUG_DEPS   yLOG_point   ("a_width"   , a_width);
   if (a_width == NULL) {
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   DEBUG_DEPS   yLOG_point   ("a_merge"   , a_merge);
   if (a_merge == NULL) {
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(initialize)---------------------*/
   x_owner  = (tMOCK *) a_owner;
   *a_width = x_owner->width;
   *a_merge = 0;
   s_owners [*a_merge] = x_owner;
   s_widths [*a_merge] = x_owner->width;
   /*---(look for mergse)----------------*/
   rc = str2gyges (x_owner->label, &b, &x, &y, &b, NULL, 0, YSTR_LEGAL);
   DEBUG_DEPS   yLOG_value   ("str2gyges" , rc);
   if (rc < 0)  {
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   DEBUG_DEPS   yLOG_complex ("owner"     , "%-10p, %-5s, %3db, %3dx, %3dy, %3dz, %3dw", x_owner, x_owner->label, b, x, y, z, x_owner->width);
   for (i = x + 1; i < x + 20; ++i) {
      rc = str4gyges (b, i, y, z, 0, x_label, YSTR_LEGAL);
      if (rc < 0)  break;
      rc = ycalc__mock_whos_at (b, i, y, z, YCALC_LOOK, &x_owner, NULL);
      if (rc < 0)                              break;
      if (x_owner == NULL)                     break;
      if (x_owner->type != YCALC_DATA_MERGED)  break;
      DEBUG_DEPS   yLOG_complex ("owner"     , "%-10p, %-5s, %3db, %3dx, %3dy, %3dz, %3dw", x_owner, x_label, b, i, y, z, x_owner->width);
      *a_width += x_owner->width;
      ++(*a_merge);
      s_owners [*a_merge] = x_owner;
      s_widths [*a_merge] = x_owner->width;
   }
   DEBUG_DEPS   yLOG_value   ("*a_width"  , *a_width);
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> parse print into merged cells ------[ ------ [gz.641.351.22]*/ /*-[22.0000.015.!]-*/ /*-[--.---.---.--]-*/
ycalc__mock_parse    (char *a_full, int a_merge)
{
   /*---(locals)-----------+-----+-----+-*/
   tMOCK      *x_owner     = NULL;
   int         i           = 0;
   int         w           = 0;             /* available printing width       */
   int         wa          = 0;             /* adjusted width                 */
   char       *p           = NULL;
   /*---(header)-------------------------*/
   DEBUG_DEPS   yLOG_senter  (__FUNCTION__);
   DEBUG_DEPS   yLOG_spoint  (a_full);
   if (a_full == NULL) {
      DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
      return 0;
   }
   DEBUG_DEPS   yLOG_sint    (a_merge);
   for (i = 0; i <= a_merge; ++i) {
      x_owner = s_owners [i];
      if (x_owner == NULL)  break;
      w       = s_widths [i];
      DEBUG_DEPS  yLOG_sint    (w);
      /*> printf ("malloc () ycalc__mock_parse\n");                                   <*/
      while (p == NULL)  p = (char*) malloc (w + 1);
      sprintf (p, "%-*.*s", w, w, a_full + wa);
      DEBUG_DEPS  yLOG_snote   (p);
      if (x_owner->print != NULL) {
         free (x_owner->print);
         x_owner->print = NULL;
      }
      x_owner->print = p;
      p    = NULL;
      wa   += w;
   }
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ycalc__mock_printer     (void *a_owner)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tMOCK      *x_owner     = NULL;
   char        s           [LEN_RECD];
   char        t           [LEN_RECD];
   char        x_out       [LEN_RECD];
   char        x_len       =    0;
   int         w           =    0;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   DEBUG_DEPS   yLOG_point   ("a_owner"   , a_owner);
   /*---(defense)------------------------*/
   --rce;  if (a_owner   == NULL) {
      DEBUG_DEPS   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_owner   = (tMOCK     *) a_owner;
   /*---(contents)-----------------------*/
   DEBUG_DEPS   yLOG_char    ("type"      , x_owner->type);
   if (strchr ("=n", x_owner->type) != NULL) {
      strl4main (x_owner->value, s, x_owner->decs, x_owner->format, '-', LEN_RECD);
   } else {
      if      (x_owner->string != NULL)  strlcpy (s, x_owner->string, LEN_RECD);
      else if (x_owner->source != NULL)  strlcpy (s, x_owner->source, LEN_RECD);
      else                               strlcpy (s, ""             , LEN_RECD);
   }
   /*---(pad/trim)-----------------------*/
   ycalc__mock_width (x_owner, &w, &c);
   DEBUG_DEPS   yLOG_value   ("w"         , w);
   DEBUG_DEPS   yLOG_value   ("c"         , c);
   strlpad (s, t, '!', x_owner->align, w - 1);
   sprintf (x_out, "%s ", t);
   /*---(parse)--------------------------*/
   ycalc__mock_parse (x_out, c);
   /*> x_owner->print = strdup (x_out);                                               <*/
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
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
   DEBUG_DEPS   yLOG_point   ("existing"  , x_owner->source);
   DEBUG_DEPS   yLOG_info    ("existing"  , x_owner->source);
   if (x_owner->source != NULL) {
      DEBUG_DEPS   yLOG_note    ("must free");
      free (x_owner->source);
      x_owner->source = NULL;
      DEBUG_DEPS   yLOG_point   ("freed"     , x_owner->source);
   }
   if (a_source != NULL) {
      x_owner->source = strdup (a_source);
      DEBUG_DEPS   yLOG_point   ("new"       , x_owner->source);
      DEBUG_DEPS   yLOG_info    ("new"       , x_owner->source);
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
   tDEP_ROOT  *x_deproot   = NULL;
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
   rc = ycalc_call_who_named (a_label, YCALC_OWNR, &x_owner, &x_deproot);
   if (x_owner->print  != NULL) {
      free (x_owner->print);
      x_owner->print  = NULL;
   }
   x_owner->decs   = a_decs;
   x_owner->format = a_format;
   x_owner->align  = a_align;
   x_owner->width  = a_width;
   /*---(classify)-----------------------*/
   rc = yCALC_handle (a_label);
   rc = ycalc_call_who_named (a_label, YCALC_LOOK, &x_owner, &x_deproot);
   if (x_owner == NULL) {
      DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(handle results)-----------------*/
   /*> if (strchr ("=n", x_owner->type) != NULL) {                                    <* 
    *>    strl4main (x_owner->value, t, a_decs, a_format, LEN_RECD);                  <* 
    *> } else {                                                                       <* 
    *>    if (x_owner->string != NULL)  strlcpy (t, x_owner->string, LEN_RECD);       <* 
    *>    else                          strlcpy (t, x_owner->source, LEN_RECD);       <* 
    *> }                                                                              <* 
    *> strlpad (t, x_out, ' ', a_align, a_width - 1);                                 <* 
    *> x_owner->print = strdup (x_out);                                               <*/
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ycalc__mock_wrap      (void)
{
   tMOCK      *x_curr      = NULL;
   DEBUG_DEPS   yLOG_enter   (__FUNCTION__);
   DEBUG_DEPS   yLOG_note    ("BEFORE");
   DEBUG_DEPS   yLOG_point   ("mroot"     , myCALC.mroot);
   DEBUG_DEPS   yLOG_point   ("mhead"     , myCALC.mhead);
   DEBUG_DEPS   yLOG_point   ("mtail"     , myCALC.mtail);
   DEBUG_DEPS   yLOG_value   ("mcount"    , myCALC.mcount);
   x_curr = myCALC.mtail;
   while (x_curr != NULL) {
      ycalc__mock_free (x_curr);
      x_curr = myCALC.mtail;
   }
   DEBUG_DEPS   yLOG_note    ("AFTER");
   DEBUG_DEPS   yLOG_point   ("mroot"     , myCALC.mroot);
   DEBUG_DEPS   yLOG_point   ("mhead"     , myCALC.mhead);
   DEBUG_DEPS   yLOG_point   ("mtail"     , myCALC.mtail);
   DEBUG_DEPS   yLOG_value   ("mcount"    , myCALC.mcount);
   DEBUG_DEPS   yLOG_exit    (__FUNCTION__);
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
   char        x_print     [LEN_RECD ];
   char        x_rpn       [LEN_RECD ];
   char        x_type      =  '?';
   double      x_value     =  0.0;
   /*---(preprare)-----------------------*/
   strcpy (ycalc__unit_answer, "yCALC            : question not understood");
   /*---(label)--------------------------*/
   if (a_label == NULL)   strlcpy (x_label, "---"  , LEN_LABEL);
   else                   strlcpy (x_label, a_label, LEN_LABEL);
   /*---(owner/deproot)------------------*/
   rc = ycalc__mock_named (x_label, YCALC_LOOK, &x_owner, &x_deproot);
   /*> printf ("mock_unit :: %s, rc %d, owner %p, deproot %p\n", x_label, rc, x_owner, x_deproot);   <*/
   if (rc        <  0   )  strlcpy (x_rnote , "-----"   , LEN_LABEL);
   else                    strlcpy (x_rnote , "good"    , LEN_LABEL);
   if (x_owner   == NULL)  strlcpy (x_onote , "-----"   , LEN_LABEL);
   else                    strlcpy (x_onote , "exists"  , LEN_LABEL);
   if (x_deproot == NULL)  strlcpy (x_dnote , "-----"   , LEN_LABEL);
   else                    strlcpy (x_dnote , "enabled" , LEN_LABEL);
   /*---(string)-------------------------*/
   if      (x_owner == NULL)                                  strlcpy (x_string, "", LEN_RECD );
   else if (strchr ("&:®", x_owner->type)  != NULL)           strlcpy (x_string, x_owner->source, LEN_RECD );
   else if (strchr ("s#5E", x_owner->type) == NULL)           strlcpy (x_string, "", LEN_RECD );
   else if (x_owner->string != NULL)                          strlcpy (x_string, x_owner->string, LEN_RECD );
   else if (x_owner->type == 's' && x_owner->source != NULL)  strlcpy (x_string, x_owner->source, LEN_RECD );
   else                                                       strlcpy (x_string, "", LEN_RECD );
   /*---(value)--------------------------*/
   if      (x_owner == NULL)              x_value = 0.0;
   else                                   x_value = x_owner->value;
   /*---(type)---------------------------*/
   if      (x_owner == NULL)              x_type  = '?';
   else                                   x_type  = x_owner->type;
   /*---(print)--------------------------*/
   if      (x_owner == NULL)              strlcpy (x_print , "", LEN_RECD );
   else if (x_owner->print  != NULL)      strlcpy (x_print , x_owner->print, LEN_RECD );
   else                                   strlcpy (x_print , "", LEN_RECD );
   /*---(rpn)----------------------------*/
   if      (x_deproot == NULL)            strlcpy (x_rpn   , "", LEN_RECD );
   else if (x_deproot->rpn  != NULL)      strlcpy (x_rpn   , x_deproot->rpn, LEN_RECD );
   else                                   strlcpy (x_rpn   , "", LEN_RECD );
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
   else if (strcmp (a_question, "rpn"      )      == 0) {
      snprintf (ycalc__unit_answer, LEN_RECD, "yCALC mock rpn   : %2d:%s:", strlen (x_rpn), x_rpn);
   }
   else if (strcmp (a_question, "value"    )      == 0) {
      snprintf (ycalc__unit_answer, LEN_RECD, "yCALC mock value : %-5s %c %8.2lf :%s:", x_label, x_type, x_value, x_string);
   }
   else if (strcmp (a_question, "double"   )      == 0) {
      snprintf (ycalc__unit_answer, LEN_RECD, "yCALC mock double: %-5s %c %22.6lf", x_label, x_type, x_value);
   }
   else if (strcmp (a_question, "print"    )      == 0) {
      snprintf (ycalc__unit_answer, LEN_RECD, "yCALC mock print : %-5s %3d:%-.45s:", x_label, strlen (x_print), x_print);
   }
   /*---(complete)-----------------------*/
   return ycalc__unit_answer;
}



