#include <gtk/gtk.h>
#include <GL/glut.h>

static gboolean render (GtkGLArea *area, GdkGLContext *context)
{
  // inside this function it's safe to use GL; the given
  // #GdkGLContext has been made current to the drawable
  // surface used by the #GtkGLArea and the viewport has
  // already been set to be the size of the allocation
  // we can start by clearing the buffer
  static float FUCK = 0.0;
  FUCK += 0.01;
  glClearColor (0, FUCK, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GtkAllocation all;
  gtk_widget_get_allocation(GTK_WIDGET(area), &all);
  
  
    glDisable(GL_DEPTH_TEST);
     glDepthMask(GL_FALSE);
    glMatrixMode(GL_PROJECTION);
    glDisable(GL_CULL_FACE);
    glLoadIdentity();
    gluOrtho2D(0,all.width ,all.height, 0);
    glMatrixMode(GL_MODELVIEW);
//  GtkAllocation all;
//  gtk_widget_get_allocation(GTK_WIDGET(area), &all);
//  printf("%d, %d\n", all.width, all.height);
//glOrtho(0,all.width, all.height, 0, -1,1);
//glEnable(GL_LINE_SMOOTH);
//glHint(GL_LINE_SMOOTH_HINT,  GL_NICEST);
//glColor3f(1.0, 0.0, 0.0);
  // draw your object
//  draw_an_object ();
//glColor3f(1,1,1);
glColor3f(1.0f, 1.0f, 0.0f);

glBegin(GL_LINES);
glVertex2f(-1.0, 0.0);
glVertex2f(-1.0, -1.0);
glVertex2f(0.0, 1.0);
glVertex2f(1.0, 2.0);
glVertex2f(2.0, 3.0);
glVertex2f(20, 30);
glVertex2f(20, -30);
glEnd();

    glFlush();

  // we completed our drawing; the draw commands will be
  // flushed at the end of the signal emission chain, and
  // the buffers will be drawn on the window
  
  return TRUE;
}

// callback function which is called when button is clicked
static void on_button_clicked(GtkButton *btn, gpointer data) {
    // change button label when it's clicked
    gtk_button_set_label(btn, "Hello World");
}

// callback function which is called when application is first started
static void on_app_activate(GApplication *app, gpointer data)
{
    // create a new application window for the application
    // GtkApplication is sub-class of GApplication
    // downcast GApplication* to GtkApplication* with GTK_APPLICATION() macro
    GtkWidget *window = gtk_application_window_new(GTK_APPLICATION(app));
    // a simple push button
//    GtkWidget *btn = gtk_button_new_with_label("Click Me!");
    // connect the event-handler for "clicked" signal of button
  //  g_signal_connect(btn, "clicked", G_CALLBACK(on_button_clicked), NULL);
    // add the button to the window
    //gtk_container_add(GTK_CONTAINER(window), btn);
    
    // create a GtkGLArea instance
    GtkWidget *gl_area = gtk_gl_area_new ();
    // connect to the "render" signal
    g_signal_connect(gl_area, "render", G_CALLBACK(render), NULL);
    // add the renderer to the window
    gtk_container_add(GTK_CONTAINER(window), gl_area);
        
    // display the window
    gtk_widget_show_all(GTK_WIDGET(window));
}

int main(int argc, char *argv[]) {
    // create new GtkApplication with an unique application ID
    GtkApplication *app = gtk_application_new(
        NULL, 
        G_APPLICATION_FLAGS_NONE
    );
    // connect the event-handler for "activate" signal of GApplication
    // G_CALLBACK() macro is used to cast the callback function pointer
    // to generic void pointer
    g_signal_connect(app, "activate", G_CALLBACK(on_app_activate), NULL);
    // start the application, terminate by closing the window
    // GtkApplication* is upcast to GApplication* with G_APPLICATION() macro
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    // deallocate the application object
    g_object_unref(app);
    return status;
}

