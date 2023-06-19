#include <stdio.h>
#include <stdlib.h>
#include <GL\glut.h>
#include "glui.h"
#define PAXOS 				4.0
#define WINDOW_WIDTH 		480
#define WINDOW_HEIGHT		360
#define RESET_COORDINATES	100
#define CREATE 				101

int xs, xe, ys, ye, main_window, xxs, xxe, yys, yye;	
GLUI_EditText *x0, *y0, *x1, *y1;
GLUI_Button *reset,*ok;
GLUI *glui;

int min(int a, int b);

int max(int a, int b);

void init_glut(int argc, char** argv);

void init_glui();

void myGlutIdle();

void button_cb( GLUI_Control *control);

void display();

void draw_pixel(int x, int y);

void bresenham();

int main(int argc, char** argv)
{
	init_glut(argc,argv);
	glutDisplayFunc(display);
	init_glui();

	glutMainLoop();
	
	return 0;
}

void init_glut(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	main_window = glutCreateWindow("Bresenham's Line Drawing Algorithm");
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-WINDOW_WIDTH/2, WINDOW_WIDTH/2, -WINDOW_HEIGHT/2, WINDOW_HEIGHT/2);
}

void init_glui()
{
	glui = GLUI_Master.create_glui( "Bresenham's GUI" );
    x0 = glui->add_edittext("Starting x coordinate",GLUI_EDITTEXT_INT,&xxs);
    x0->set_int_limits(-WINDOW_WIDTH/2,WINDOW_WIDTH/2);
	y0 = glui->add_edittext("Starting y coordinate",GLUI_EDITTEXT_INT,&yys);
	y0->set_int_limits(-WINDOW_HEIGHT/2,WINDOW_HEIGHT/2);
	x1 = glui->add_edittext("Ending x coordinate",GLUI_EDITTEXT_INT,&xxe);
	x1->set_int_limits(-WINDOW_WIDTH/2,WINDOW_WIDTH/2);
	y1 = glui->add_edittext("Ending y coordinate",GLUI_EDITTEXT_INT,&yye);
	y1->set_int_limits(-WINDOW_HEIGHT/2,WINDOW_HEIGHT/2);
	reset = glui->add_button("Reset",RESET_COORDINATES,button_cb);
	ok = glui->add_button("OK",CREATE,button_cb);
	
	glui->set_main_gfx_window(main_window);
	
	GLUI_Master.set_glutIdleFunc(myGlutIdle);
}

void myGlutIdle(){
	if ( glutGetWindow() != main_window ) glutSetWindow(main_window);  

	glutPostRedisplay();
}

void button_cb( GLUI_Control *control)
{
	if(control->get_id() == RESET_COORDINATES)
	{
		xs = 0;
		xe = 0;
		ys = 0;
		ye = 0;
		x0->set_int_val(0);
		y0->set_int_val(0);
		x1->set_int_val(0);
		y1->set_int_val(0);
	}
	else if(control->get_id() == CREATE)
	{
		xs = xxs;
		xe = xxe;
		ys = yys;
		ye = yye;
	}
}

void display()
{
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 0, 0);
	glPointSize(PAXOS);

 	bresenham();
 
	glFlush();
}

void draw_pixel(int x, int y)
{
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

int min(int a, int b)
{
	return (abs(a) < abs(b)) ? a : b;
}

int max(int a, int b)
{
	return (abs(a) > abs(b)) ? a : b;
}

void bresenham()
{
    //major_axis: a pointer for the major axis.
    //minor_axis: a pointer for the minor axis.
    int x, y, end, e, dx, dy, decreament, increament, *major_axis, *minor_axis,major_add,minor_add;

    dx = abs(xe - xs);
    dy = abs(ye - ys);

	//Checks if the two points coincide.
	if(!dx && !dy)
	{
		draw_pixel(xs,ys);
	}
	else
	{
	    //Checks the major and minor axis.
	    if(dx >= dy)
	    {
	        e = -(dx >> 1);
	        decreament = dx;
	        increament = dy;
	        
	        major_axis = &x;
	        minor_axis = &y;
	        
	        *major_axis = min(xs,xe);
	        *minor_axis = min(ys,ye);
	        
	        major_add = (xe - xs)/dx;    
	        if(!dy) minor_add = 0;
	        else minor_add = (ye - ys)/dy;
	        
	        end = abs(max(xe,xs));
	    }
	    else
	    {
	        e = -(dy >> 1);
	        decreament = dy;
	        increament = dx;
	        
			major_axis = &y;
	        minor_axis = &x;
	        
	        *major_axis = min(ys,ye);
	        *minor_axis = min(xs,xe);
	
			major_add = (ye - ys)/dy;
			if(!dx) minor_add = 0;
			else minor_add = (xe-xs)/dx;
	
	        end = abs(max(ye,ys));
	    }
	
		while(abs(*major_axis) <= end)
	    {
	 		draw_pixel(x,y);
	        (*major_axis) += major_add;
	        e += increament;
	        if(e >= 0)
	        {
	        	(*minor_axis) += minor_add;
	        	e -= decreament;
			}
	    }
	}

}
