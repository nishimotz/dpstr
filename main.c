/* 
 * main.c
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dpstr.h"
#include "disp.h"

char *strtok_r(char *SOURCE, const char *DELIMITERS, char **LASTS)
{
    return strtok(SOURCE, DELIMITERS);
}


static bool show_graph_mode = false;
static bool use_list_mode = false;
static bool show_accuracy_mode = true;
static bool use_word_mode = true;
static bool show_map_mode = false;

#define BUFSIZE 1000

static void compare_word( char *adata, char *bdata, DPSTR_RESULT *res )
{
  // a: test data
  // b: reference data
  char *astr[MAX_ELM_NUM + 1];
  char *bstr[MAX_ELM_NUM + 1];
  int alen, blen, i;

  alen = strlen(adata);
  for (i = 0; i < alen; i++) {
    astr[i] = calloc(sizeof(char), 2);
    astr[i][0] = adata[i];
  }
  blen = strlen(bdata);
  for (i = 0; i < blen; i++) {
    bstr[i] = calloc(sizeof(char), 2);
    bstr[i][0] = bdata[i];
  }
  set_size(alen, blen);
  compare_strings( astr, bstr, res, false );
} /* compare_word() */


static void compare_file( char *afile, char *bfile, DPSTR_RESULT *res )
{
  // a: test data
  // b: reference data
  char *astr[MAX_ELM_NUM + 1];
  char *bstr[MAX_ELM_NUM + 1];
  char  tstr[BUFSIZE];
  int   alen, blen;
  FILE *fp;
  char *ptr = NULL;

  if ( (fp = fopen( afile, "rt" )) == NULL ) {
    perror("afile"); exit(1);
  }
  alen = 0;
  while ( fgets( tstr, BUFSIZE, fp ) ) {
    strtok_r(tstr, " \n\r\t", &ptr);
    if ( strlen(tstr) > 0 ) {
      astr[alen] = strdup(tstr);
      alen++;
    }
  }
  fclose(fp);
  if ( (fp = fopen( bfile, "rt" )) == NULL ) {
    perror("bfile"); exit(1);
  }
  blen = 0;
  while ( fgets( tstr, BUFSIZE, fp ) ) {
    strtok_r(tstr, " \n\r\t", &ptr);
    if ( strlen(tstr) > 0 ) {
      bstr[blen] = strdup(tstr);
      blen++;
    }
  }
  fclose(fp);
  set_size(alen, blen);
  compare_strings( astr, bstr, res, false );
} /* compare_file() */


static void show_help()
{
  printf("Usage: dpstr [-word] [options] reference-word test-word\n");
  printf("Usage: dpstr -file   [options] refecence.lbl test.lbl\n");
  printf("options:\n" );
  printf(" -list listfile (do not use args)\n" );
  printf(" -acc   : display accuracy\n" );
  printf(" -graph : show graphics using GLUT\n" );
  printf(" -map   : show dp_map\n" );
  printf("dpstr (version:2010-02-11) by nishimoz\n" );
  exit(0);
}


int main( int argc, char **argv )
{
  char tstr[BUFSIZE];
  char a_data[BUFSIZE] = {'\0'}; // a_i test data
  char b_data[BUFSIZE] = {'\0'}; // b_j reference data
  FILE *fp;
  DPSTR_RESULT result;
  int ct;
  char *ptr = NULL;
  char *listfile = NULL;

  if (argc < 3) {
    show_help();
  }
  for ( ct = 1; ct < argc; ct++ ) {
    if ( strcmp(argv[ct], "-help") == 0 ) {
      show_help();
    } else if ( strcmp(argv[ct], "-list") == 0 ) {
      assert(ct+1 < argc);
      use_list_mode = true;
      listfile = argv[++ct];
    } else if ( strcmp(argv[ct], "-acc") == 0 ) {
      show_accuracy_mode = true;
    } else if ( strcmp(argv[ct], "-graph") == 0 ) {
      show_graph_mode = true;
    } else if ( strcmp(argv[ct], "-word") == 0 ) {
      use_word_mode = true;
    } else if ( strcmp(argv[ct], "-file") == 0 ) {
      use_word_mode = false;
    } else if ( strcmp(argv[ct], "-map") == 0 ) {
      show_map_mode = false;
    } else {
      assert(ct+1 < argc);
      strcpy( b_data, argv[ct] );
      ct++;
      strcpy( a_data, argv[ct] );
    }
  }

#ifdef USE_GLUT
  if ( show_graph_mode ) {
    disp_init(argc, argv);
  }
#endif
  if ( use_list_mode ) {
    if ( (fp = fopen( argv[2], "rt" )) == NULL ) {
      perror("listfile"); exit(1);
    }
    while ( fgets( tstr, BUFSIZE, fp ) ) {
      strcpy( b_data, strtok_r(tstr, " \n\r\t", &ptr) );
      strcpy( a_data, strtok_r(ptr, " \n\r\t", &ptr) );
      printf( "Comparing %s : %s\n", b_data, a_data );
      compare_file( a_data, b_data, &result );
    }
  } else if ( use_word_mode ) {
    compare_word( a_data, b_data, &result );
    printf("score: %f\n", result.m_score / (get_size_a() + get_size_b()));
    if (show_map_mode) show_map();
  } else {
    compare_file( a_data, b_data, &result );
#if 0
    printf("score: %f\n", result.m_score / (get_size_a() + get_size_b()));
#endif
    if (show_map_mode) show_map();
  }

  if ( show_accuracy_mode ) {
	printf("acc1: H=%d  D=%d  S=%d  I=%d  N=%d\n", 
           result.m_hit, result.m_del, result.m_sub, 
           result.m_ins, result.m_num );
    
#if 0
	printf("acc2: H/N[Correct]=%6.2f  (H-I)/N[Accuracy]=%6.2f\n", 
           100.0 * result.m_hit / result.m_num, 
	   100.0 * (result.m_hit-result.m_ins) / result.m_num );
#endif
    }

#ifdef USE_GLUT
  if ( show_graph_mode ) {
    disp_main_loop();
  }
#endif
  
  return 0;
} 

/* end of file */
