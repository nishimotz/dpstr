/* 
 * dpmatch.c
 */

#include <float.h>
#include <assert.h>
#include <stdio.h>
#include "dpstr.h"

typedef struct {
  int da1, db1; 
  score_t w2; int da2, db2;
  score_t w3; int da3, db3;
} SCAN_TBL;

#define	SCAN_TABLE_NUM 	3

static SCAN_TBL m_scan[SCAN_TABLE_NUM] = {
#if 0
  {1, 0, 1.0, 0, 0, 0.0, 0, 0}, 
  {1, 1, 2.0, 0, 0, 0.0, 0, 0}, 
  {0, 1, 1.0, 0, 0, 0.0, 0, 0}
#else
  {2, 1, 2.0, 1, 0, 1.0, 0, 0}, 
  {1, 1, 2.0, 0, 0, 0.0, 0, 0}, 
  {1, 2, 2.0, 0, 1, 1.0, 0, 0}
#endif
};

static MAP m_dp_map[MAX_ELM_NUM][MAX_ELM_NUM];
static int m_alen = 0;
static int m_blen = 0;

void set_size(int alen, int blen)
{
  m_alen = alen;
  m_blen = blen;
}

int get_size_a()
{
  return m_alen;
}

int get_size_b()
{
  return m_blen;
}

void show_map()
{
  int i, j;
  printf("dp_map:\n");
  for (j = m_blen - 1; 0 <= j; j--) {
    printf("j=%d: ", j);
    for (i = 0; i < m_alen; i++) {
      MAP m = m_dp_map[i][j];
      double f = m.future;
      if (f == FLT_MAX) {
        printf("[%d,%d,MAX] ", m.a, m.b);
      } else if (m.a == GOAL) {
        printf("[G,G,%3.1f] ", f);
      } else if (m.a == NOWHERE) {
        printf("[N,N,MAX] ");
      } else {
        printf("[%d,%d,%3.1f] ", m.a, m.b, f);
      }
    }
    printf("\n");
  }
}

score_t dp_match(int a_len, int b_len)
{
  int  dest_a, dest_b, min_a, min_b, ta, tb, k;
  score_t min_score, sc, dist;

  for (ta = (a_len - 1); 0 <= ta; ta--) {
    for (tb = (b_len - 1); 0 <= tb; tb--) {
      dist = distance_at(ta, tb);
      if ((ta == (a_len - 1)) && (tb == (b_len - 1))) {
        m_dp_map[ta][tb].future = dist;
        m_dp_map[ta][tb].a = GOAL;
        m_dp_map[ta][tb].b = GOAL;
      } else {
        min_score = MAX_SCORE;
        min_a = NOWHERE;
        min_b = NOWHERE;
        for (k = 0; k < SCAN_TABLE_NUM; k++) {
          dest_a = ta + m_scan[k].da1;
          dest_b = tb + m_scan[k].db1;
          sc = m_dp_map[dest_a][dest_b].future;
          if ((dest_a < a_len) && (dest_b < b_len)) {
            sc += distance_at(ta + m_scan[k].da2, 
                              tb + m_scan[k].db2 ) 
              * m_scan[k].w2;
            if (m_scan[k].w3 > 0.0) {
              sc += distance_at(ta + m_scan[k].da3, 
                                tb + m_scan[k].db3 ) 
                * m_scan[k].w3;
            }
            if (sc < min_score) {
              min_score = sc;
              min_a = dest_a;
              min_b = dest_b;
            }	
          }	
        }
        m_dp_map[ta][tb].future = min_score;
        m_dp_map[ta][tb].a = min_a;
        m_dp_map[ta][tb].b = min_b;
      }
    }	
  }
  return (m_dp_map[0][0].future);
}

int get_a_at(int a, int b)
{
  return m_dp_map[a][b].a;
}

int get_b_at(int a, int b)
{
  return m_dp_map[a][b].b;
}

score_t get_future_at(int a, int b)
{
  return m_dp_map[a][b].future;
}

/* end of file */

