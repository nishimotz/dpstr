/*
 * dpstr.c
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dpstr.h"

void compare_strings( char **a_str, char **b_str, DPSTR_RESULT *res )
{
  char esti_ans[MAX_ELM_NUM * 2 + 1];
  char *a_ans[MAX_ELM_NUM * 2 + 1];
  char *b_ans[MAX_ELM_NUM * 2 + 1];
  int a_len = get_size_a();
  int b_len = get_size_b();
  int i;

  res->m_num = 0;
  res->m_hit = 0;
  res->m_del = 0;
  res->m_sub = 0;
  res->m_ins = 0;
  res->m_score = (score_t)0;

  if ( b_len == 0 ) {
    return;
  } else if ( a_len == 0 ) {
    for ( i = 0; i < b_len; i++ ) {
      res->m_num++;
      res->m_del++;
    }
  } else {
    get_distance((ELM *) a_str, a_len, (ELM *) b_str, b_len);
    res->m_score = dp_match(a_len, b_len);
    estimate((ELM *)a_str, (ELM *)b_str, a_ans, b_ans, esti_ans);
    int n = strlen(esti_ans);
    for ( i = 0; i < n ; i++ ) {
      // display alignment by word
      printf("%c %s %s\n", esti_ans[i], b_ans[i], a_ans[i] );
      switch ( esti_ans[i] ) {
      case 'H':
        res->m_num++;
        res->m_hit++; break;
      case 'D':
        res->m_num++;
        res->m_del++; break;
      case 'S':
        res->m_num++;
        res->m_sub++; break;
      case 'I':
        res->m_ins++; break;
      default:
        break;
      }
    }
  }
}

/* end of file */

