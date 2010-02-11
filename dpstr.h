/*
 * dpstr.h
 */

typedef int bool;
#define true  (1)
#define false (0)

#define	uint unsigned int
typedef enum { ERROR = -1, OK = 0 }  error_t;

#define	ELM         char *
#define	score_t	    double  /* type of score */
#define	MAX_SCORE   FLT_MAX
#define	MAX_ELM_NUM 100	    /* max number of elements */
#define	GOAL        (-1)
#define	NOWHERE     (-2)

typedef struct {
  int     a;      /* fittest route */
  int     b;	        
  score_t future; /* score of future */
} MAP;

typedef struct {
  int m_num;
  int m_hit;
  int m_del;
  int m_sub;
  int m_ins;
  score_t m_score;
} DPSTR_RESULT;

void get_distance ( ELM *x_list, int x_len, ELM *y_list, int y_len );
score_t	distance_at ( int a, int b );

void estimate(ELM * x_list, ELM * y_list, char **x_str, char **y_str, 
              char *esti_str);
score_t dp_match(int x_len, int y_len);
void compare_strings( char **x_str, char **y_str, DPSTR_RESULT *res );

int get_a_at(int a, int b);
int get_b_at(int a, int b);
score_t get_future_at(int a, int b);

void set_size(int alen, int blen);
int get_size_a();
int get_size_b();
void show_map();

/* end of file */

