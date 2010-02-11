/*
 * estimate.c
 */

#include <string.h>
#include "dpstr.h"

#define NIL_STR    "*"
#define	BLANK_CHAR ' '
#define	SAME_CHAR  'H'
#define	BAD_CHAR   'S'
#define	INS_CHAR   'I'
#define	DEL_CHAR   'D'

// extern MAP Dp_map[MAX_ELM_NUM][MAX_ELM_NUM];

/*
 * trace route and get estimate strings
 */
void estimate(ELM *a_list, ELM *b_list, char **a_ans, char **b_ans, 
              char *esti_ans)
{
  int   ca, cb, a_new, b_new, pos;
  bool  a_done, b_done, a_filled, b_filled;

  ca = cb = 0;
  a_done = b_done = false;
  pos = 0;
  while (!((ca == GOAL) && (cb == GOAL))) {
    a_ans[pos] = NIL_STR;
    b_ans[pos] = NIL_STR;
    esti_ans[pos] = BLANK_CHAR;
    a_new = get_a_at(ca,cb); // Dp_map[ca][cb].a;
    b_new = get_b_at(ca,cb); // Dp_map[ca][cb].b;
    if (distance_at(ca, cb) == 0) {
      a_ans[pos] = a_list[ca];
      b_ans[pos] = b_list[cb];
      esti_ans[pos] = SAME_CHAR;
      a_done = b_done = true;
    } else {
      a_filled = b_filled = false;
      if ((a_new != ca) && (!a_done)) {
        a_ans[pos] = a_list[ca];
        a_done = true;
        a_filled = true;
      }
      if ((b_new != cb) && (!b_done)) {
        b_ans[pos] = b_list[cb];
        b_done = true;
        b_filled = true;
      }
      if ((!a_filled) && b_filled) {
        esti_ans[pos] = DEL_CHAR;
      } else if (a_filled && (!b_filled)) {
        esti_ans[pos] = INS_CHAR;
      } else {
        esti_ans[pos] = BAD_CHAR;
      }
    }
    if (a_new != ca) {
      a_done = false;
    }
    if (b_new != cb) {
      b_done = false;
    }
    ca = a_new;
    cb = b_new;
    pos++;
  }
  a_ans[pos] = NIL_STR;
  b_ans[pos] = NIL_STR;
  esti_ans[pos] = '\0';
} /* end of estimate() */

/* end of file */

