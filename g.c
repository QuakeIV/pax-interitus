#include <GL/glut.h>
#include <math.h>

void render()
{
  glClear( GL_COLOR_BUFFER_BIT );

  glBegin(GL_LINES);
  for( double i = 0 ; i < 2*3.14159; i+=0.01 )
  {
    double value1 = cos(i) ;
    double value2 = sin(i) ;
    glColor3d( fabs(value1), fabs(value1)*fabs(value2), fabs(value2) );
    glVertex2d( .2*value1*value2,.4*value2 ) ;
    glVertex2d( value1, value2 ) ;
  }
  glEnd() ;

  glutSwapBuffers() ;
}

int main( int argc, char** argv )
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(120,260);
  glutInitWindowSize(600,600); 
  glutCreateWindow("GLUT Window");

  glutDisplayFunc( render ); 
  glutMainLoop();
}
