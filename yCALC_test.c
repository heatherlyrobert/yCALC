/*===[[ START ]]==============================================================*/
#include    "yCALC.h"
#include    "yCALC_priv.h"


char*
yCALC__unit           (char *a_question, char *a_label)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   tDEP_ROOT  *x_deproot   = NULL;
   char        t           [LEN_RECD]  = "";
   /*---(defense)------------------------*/
   sprintf (ycalc__unit_answer, "yCALC unit       : question not understood");
   if (a_label == NULL) {
      sprintf (ycalc__unit_answer, "yCALC unit       : no label provided");
      return ycalc__unit_answer;
   }
   /*---(prepare)------------------------*/
   rc = ycalc_call_who_named (a_label, YCALC_LOOK, NULL, &x_deproot);
   if (x_deproot == NULL) {
      sprintf (ycalc__unit_answer, "yCALC unit       : label å%sæ not found", a_label);
      return ycalc__unit_answer;
   }
   /*---(questions)----------------------*/
   if      (strcmp (a_question, "rpn"      )      == 0) {
      if  (x_deproot->rpn == NULL || x_deproot->nrpn <= 0) {
         sprintf (ycalc__unit_answer, "yCALC rpn        : %-8.8s  ··  ··dåæ", a_label);
      } else {
         strlcpy  (t, x_deproot->rpn, LEN_RECD);
         strldchg (t, 0x0F, '´', LEN_RECD);
         sprintf (ycalc__unit_answer, "yCALC rpn        : %-8.8s %2d  %2då%sæ", a_label, x_deproot->nrpn, strlen (t), t);
      }
   }
   /*---(complete)-----------------------*/
   return ycalc__unit_answer;
}
