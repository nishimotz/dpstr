/*
 * disp.c
 */

#include <GL/glut.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <float.h>
#include "dpstr.h"
#include "disp.h"

static int m_screen_width  = SCREEN_WIDTH;
static int m_screen_height = SCREEN_HEIGHT;

void XpSetBlack()
{
  glColor3d(0.0, 0.0, 0.0);
}

void XpSetGray()
{
  glColor3d(0.8, 0.8, 0.8);
}

void XpSetWhite()
{
  glColor3d(1.0, 1.0, 1.0);
}

void XpSetGreen()
{
  glColor3d(0, 1.0, 0);
}

void XpSetRed()
{
  glColor3d(1.0, 0, 0);
}

void XpFillBox( double x1, double y1, double x2, double y2 )
{
  glBegin(GL_POLYGON);
  glVertex2d(x1, y1);
  glVertex2d(x2, y1);
  glVertex2d(x2, y2);
  glVertex2d(x1, y2);
  glEnd();
}

void XpDrawBox( double x1, double y1, double x2, double y2 )
{
  glBegin(GL_LINE_LOOP);
  glVertex2d(x1, y1);
  glVertex2d(x2, y1);
  glVertex2d(x2, y2);
  glVertex2d(x1, y2);
  glEnd();
}

void XpDrawLine( double x1, double y1, double x2, double y2 )
{
  glBegin(GL_LINES);
  glVertex2d(x1, y1);
  glVertex2d(x2, y2);
  glEnd();
}

void XpDrawPoint( double x1, double y1 )
{
  double x2, y2;
  x2 = x1 + 1;
  y2 = y1 + 1;
  XpFillBox(x1, y1, x2, y2);
}


void disp_clear()
{
  XpSetWhite();
  XpFillBox( 0, 0, m_screen_width, m_screen_height );
}


void disp_distance(int left, int top, int width, int height)
{
  int i, j;
  int a, b, a2, b2;
  double apos0, apos1, apos2, ahalf;
  double bpos0, bpos1, bpos2, bhalf;
  int alen = get_size_a();
  int blen = get_size_b();

  XpSetWhite();
  XpFillBox(left, top, left+width, top+height); 

  ahalf = 0.5 * width  / alen;
  bhalf = 0.5 * height / blen;

  // back-trace path
  a = b = 0;
  while ( ! (a == alen-1 && b == blen-1) ) {
    apos0 = (double) a * height / alen;
    bpos0 = (double) b * width  / blen;
    apos2 = (double) get_a_at(a,b) * height / alen;
    bpos2 = (double) get_b_at(a,b) * width  / blen;
    
    XpSetRed();
    XpDrawLine(left + apos0  + ahalf + 2, 
               top  + height - bpos0 - bhalf + 2, 
               left + apos2  + ahalf + 2, 
               top  + height - bpos2 - bhalf + 2);
    a2 = get_a_at(a,b);
    b2 = get_b_at(a,b);
    a = a2;
    b = b2;
  } 

  for (i = 0; i < alen; i++) {
    apos0 = (double)  i    * height / alen;
    apos1 = (double) (i+1) * height / alen;

    for (j = 0; j < blen; j++) {
      bpos0 = (double)  j    * width / blen;
      bpos1 = (double) (j+1) * width / blen;

      // frames
      XpSetGray();
      XpDrawBox(left + apos0 + 2, top + height - bpos0 - 2, 
                left + apos1 - 2, top + height - bpos1 + 2);

      // draw back-pointer
      if ( get_b_at(i,j) != NOWHERE 
           && get_b_at(i,j) != GOAL 
           && get_future_at(i,j) != MAX_SCORE ) {
        apos2 = (double) get_a_at(i,j) * width  / alen;
        bpos2 = (double) get_b_at(i,j) * height / blen;
	
        XpSetGreen();
        XpDrawLine(left + apos0 + ahalf, 
                   top + height - bpos0 - bhalf, 
                   left + apos2 + ahalf, 
                   top + height - bpos2 - bhalf);
      }

      // paint distance
      if ( distance_at(i,j) == 0 ) {
        XpSetBlack();
        XpFillBox(left + apos0 + 20, top + height - bpos0 - 20, 
                  left + apos1 - 20, top + height - bpos1 + 20);
      }
    }
  }
}

void draw()
{
  disp_clear();
  disp_distance(0, 0, m_screen_width, m_screen_height);
  glFlush();
  glutSwapBuffers();
}

void reshape(int w, int h)
{
  glViewport(0, 0, w, h);
  glLoadIdentity();
  glOrtho(0.0, (GLdouble)w, (GLdouble)h, 0.0, -1.0, 1.0);

  m_screen_width  = w;
  m_screen_height = h;

  //draw();
}


void display(void)
{
  draw();
}


void keyboard(unsigned char key, int x, int y)
{
  switch (tolower(key)) {
  case 'q':
  case '\033': // Esc
    exit(0);
  default:
    break;
  }
}


void mouse(int button, int state, int x, int y)
{
  // state: 0=GLUT_DOWN, 1=GLUT_UP
  // x,y: upper-left: (0,0) lower-right: (width-1,height-1)
  switch (button) {
  case GLUT_LEFT_BUTTON:
    printf("left_button %d %d %d %d\n", button, state, x, y);
    break;
  case GLUT_MIDDLE_BUTTON:
    printf("middle_button %d %d %d %d\n", button, state, x, y);
    break;
  case GLUT_RIGHT_BUTTON:
    printf("right_button %d %d %d %d\n", button, state, x, y);
    break;
  default:
    break;
  }
}
 

void motion(int x, int y)
{
  printf("motion %d %d\n", x, y);
}


void passivemotion(int x, int y)
{
  printf("passive motion %d %d\n", x, y);
}


void disp_init(int argc, char **argv)
{
  glutInitWindowSize( m_screen_width, m_screen_height );

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutCreateWindow( WINDOW_NAME );

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
}


void disp_main_loop()
{
  glutMainLoop();
}

/* end of file */
