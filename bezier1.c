#include<stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <math.h>
#include <stdlib.h>
#include<string.h>

int i,n,number,count=1000,count1=1,i1,rccount =0,count2=0,valid=0;
float t;

typedef struct									// Structure for storing the points
{
	int x,y;
}Point;					
	
Point p[1000],temp[1000],points[101],temp1[1000],temp2[1000];

Point lerp(Point a,Point b, float t)						// Method to calculate the Linear Interpolation
{										// of two points at a given t value
	Point c;
	c.x = (1-t)*a.x + t*b.x;
	c.y = (1-t)*a.y + t*b.y;
	return c;
}

Point final(Point p[],int n,float t)						// Method to find the last interpolated point for a given t
{
	if(n>1)
	{
		for(i=0;i<n-1;i++)
		{
			p[i]=lerp(p[i],p[i+1],t);
		}
		final(p,n-1,t);
	}
	else
	{
		return p[0];
	}

}

void plotPoint(int a,int b)							// Method to plot points on the screen
{
	glColor3f(1,0,0);
	glPointSize(5);
	glBegin(GL_POINTS);
     	glVertex2i(a,b);
    	glEnd();
    	glFlush();
}

void printPoint(Point a)							// Method to print the co-ordinates of a point
{
	printf("(%d,%d)\n",a.x,a.y);
}

void joinPoints(Point a, Point b)						// Method to join two points using a line
{
	glBegin(GL_LINES);
	glLineWidth(2);
	glColor3f(0,0,1);
      	glVertex2i(a.x, a.y);
      	glVertex2i(b.x, b.y);
    	glEnd();
    	glFlush();
}
	
void drawCurve()								// Method to draw the Bezier curve
{
	glColor3f(0,0,0);
	glLineWidth(2);
	glBegin(GL_LINE_STRIP);
	for(i=0;i<101;i++)
	{
		glVertex2f(points[i].x,points[i].y);
	}
	glEnd();
}

void equate(Point p[],Point temp[],int n)					// Method to equate two point arrays
{
	for(i=0;i<n;i++)
	{
		temp[i].x = p[i].x ;
		temp[i].y = p[i].y ;
	}
	count = n;
}

void reset(Point p[],Point temp[],int n)					// Method to restore the original values in an array
{
	equate(temp,p,n);
	n = count;
}

void draw()									// Draw function
{
	equate(p,temp,n);
	equate(p,temp1,n);

	points[0].x=p[0].x;
	points[0].y=p[0].y;
	points[100].x=p[n-1].x;
	points[100].y=p[n-1].y;

	t=0.01;
	while(t<=0.99)
	{
		points[count1]=final(p,n,t);
		count1++;
		reset(p,temp1,n);
		t+=0.01;
	}

	drawCurve();

	for(i=0;i<n;i++)
	{
		plotPoint(p[i].x,p[i].y);
	}

	for(i=0;i<n-1;i++)
	{
		joinPoints(p[i],p[i+1]);		
	}
}

void drawText(char *string, float x, float y, float z)				// Method to write text on screen
{
    	char *c;
    	glRasterPos3f(x, y, z);

    	for (c=string; *c != '\0'; c++)
    	{
        	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    	}
}

void myInit() 									// Method to initialize the screen
{
    	glClearColor(1,1,1,1);
	glColor3f(0,0,0);
	glLoadIdentity();
	gluOrtho2D(0,640,0,480);
	glMatrixMode(GL_MODELVIEW);
}

void myMouse(int button, int state, int a, int b) 
{
	int i=0;

  	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
		p[number].x = a;
		p[number].y = 480-b; 
    		number++;
			
		if(number<=n)
		{
			plotPoint(a,480-b);
		}

		if(number == n)
		{
			equate(p,temp2,n);

			printf("\n The following are the control points choosen :\n\n");

			for(i=0;i<n;i++)					// Print the control points choosen
			{
				printf(" Point %d : ",i+1);
				printPoint(p[i]);
			}

			printf("\n");	
				
			printf(" Right click on a control point and drag to move it\n\n");

			glClear(GL_COLOR_BUFFER_BIT);
			
			glColor3d(0,0,0);
			drawText("Right click on a control point and drag",170,460,0);

			draw();							// draw the curve for the control points given
				
			count = 0;		
		}
	}

	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) 			// Right click on any of control points
	{
		if(count == 0) 
		{
			for(i=0;i<n;i++)
			{
				if(temp1[i].x-5 <= a && a <= temp1[i].x+5 && temp1[i].y-5 <= 480-b && 480-b <= temp1[i].y+5)
				{
					printf(" Selected point is control point %d\n",i+1);
					i1=i;
					rccount = 1;
					valid = state == GLUT_DOWN;						
					break;
				}				
			}
		}
	}

	if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP && rccount == 1) 	// Once the right click button is released move the
	{									// point and print it's location 							
		rccount = 0;
		count = 0;
		printf(" It has been moved to : (%d,%d)\n\n",a,480-b);
	}

}

void myMotion(int a, int b)							// Method to determine cursor motion
{
	if(rccount == 1)
	{
		temp2[i1].x = a;
		temp2[i1].y = 480-b; 
		
		equate(temp2,p,n);

		equate(p,temp,n);
		equate(p,temp1,n);

		points[0].x=p[0].x;
		points[0].y=p[0].y;
		points[100].x=p[n-1].x;
		points[100].y=p[n-1].y;
						
		t=0.01;
		count1=1;
		while(t<=0.99)
		{
			points[count1]=final(p,n,t);
			count1++;
			reset(p,temp1,n);
			t+=0.01;
		}

		glClear(GL_COLOR_BUFFER_BIT);
		
		glColor3d(0,0,0);
		drawText("Right click on a control point and drag",170,460,0);

		drawCurve();

		for(i=0;i<n;i++)
		{
			plotPoint(p[i].x,p[i].y);		
		}

		for(i=0;i<n-1;i++)
		{
			joinPoints(p[i],p[i+1]);		
		}
	}
}

void myDisplay() 								// Display function
{
    	glClear(GL_COLOR_BUFFER_BIT);
	glColor3d(0,0,0);
	drawText("Plot the points by clicking the left mouse button",130,460,0);
    	glFlush();
}

int main(int argc, char** argv)
{
	printf("\n Enter the no.of control points : ");
	scanf("%d",&n);

	while(n<=1)
	{
		printf("\n Number of control points should be greater than 1. Enter a new value : ");
		scanf("%d",&n);
	}

	printf("\n Plot the %d points on the screen \n",n);

	glutInit(&argc, argv);   
  	glutInitDisplayMode(GLUT_SINGLE| GLUT_RGB);
  	glutInitWindowSize(640,480);
	glutInitWindowPosition(700,0);
	glutCreateWindow("Bezier Curve implementation using De Casteljau Algorithm");  
	glutDisplayFunc(myDisplay);	
	glutMouseFunc(myMouse);
	glutMotionFunc(myMotion);
	myInit(); 
	glutMainLoop(); 

	return 0;
}
