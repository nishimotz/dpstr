/* 
 * distance.c
 */

#include <stdio.h>
#include <string.h>
#include "dpstr.h"

static score_t Distance[MAX_ELM_NUM][MAX_ELM_NUM]; // [a_i][b_j]

void get_distance ( ELM *a_list, int a_len, ELM *b_list, int b_len )
{
  int i, j;
    
  for ( i = 0; i < MAX_ELM_NUM; i++ ) {
    for ( j = 0; j < MAX_ELM_NUM; j++ ) {
      Distance[i][j] = 0;
    }
  }
  for ( i = 0; i < a_len; i++ ) {
    for ( j = 0; j < b_len; j++ ) {
      if ( strcmp(a_list[i], b_list[j]) == 0 )
        Distance[i][j] = 0;
      else
        Distance[i][j] = 1;
    }
  }
} /* end of get_distance() */

score_t	distance_at ( int a, int b )
{
  // printf("distance at %d, %d\n", a, b);
  return ( Distance [ a ][ b ] );
}


/* end of file */

