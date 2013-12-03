#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#include <GL\glut.h>
#include "clParticles.h"
#include "camera.h"
using namespace std;

CLParticles *particlesSystem;
Camera *camera;

int window_width=1024,
    window_height=768,
    window_x=320,
    window_y=0;

void draw_particle(vector3 *p){
	glPushMatrix();
	glTranslatef(p->x,p->y,p->z);
	glutSolidSphere(1,5,5);
	glPopMatrix();
}
void draw_particles(){
	for(int i = 0; i < MAX_PARTICLES; i++){
		draw_particle(&(particlesSystem->P[i]));
	}
}
void keyboard(unsigned char key, int x, int y){
	switch(key){
	case 'q':
		camera->distance+=10.;
		break;
	case 'e':
		camera->distance-=10.;
		break;
	}
	glutPostRedisplay();
}
void display(){
    glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	camera->update();
	
	particlesSystem->step();
    
	glColor3f(0,1,0);


	draw_particles();
		//drawFloor
	/*
	glPushMatrix();
	glTranslatef(0,-4,0);
	glScalef(100,0.3,100);
	glColor3f(1,0,0);
	glutSolidCube(1);
	glPopMatrix();
	*/
    glFlush();
	Sleep(0);
	glutPostRedisplay();
	Sleep(3);
}
void reshape(int width, int height){
    window_width=width;
    window_height=height;
}
float lastX=0;
float lastY=0;
void mouse(int button, int state, int x, int y){
	lastX=x;
    lastY=y;
}


void motion(int x, int y){
	camera->rotation[1]+=x-lastX;
    camera->rotation[0]-=y-lastY;
    lastX=x;
    lastY=y;
	
	glutPostRedisplay();
}
void special(int key, int x, int y){
    cout<<"special key: "<<key<<endl;
}
void glut_init(int argc, char** argv){
	
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowPosition(window_x,window_y);
    glutInitWindowSize(window_width,window_height);
    glutCreateWindow("CL-Sand Particles");
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
	
	
}
void gl_init(){
    glClearColor(0,0,0,0);
	glLoadIdentity();
	camera=new Camera();
	camera->viewport(0,0,window_width,window_height);
	camera->distance=200;
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,window_width/window_height,0.1,200000);
    glMatrixMode (GL_MODELVIEW);
    camera->update();
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}
int n_particles=100;
int main(int argc, char**argv){
	particlesSystem = new CLParticles();
	particlesSystem->init();

	/**
	vector3 **particles = (vector3**) malloc (n_particles*sizeof(vector3*));
	for (int i=0;i<n_particles;i++){
		particles[i]= new vector3();
		particles[i]->x=(rand()%100)/10;
		particles[i]->y=(rand()%100)/10;
		particles[i]->z=(rand()%100)/10;
		//particles[i]->z=(1*i)-(n_particles/2);
	}

	particlesSystem->setParticles(particles,n_particles);
	vector3 **gravity =(vector3**) malloc (2*sizeof(vector3*));
	gravity[0]=new vector3();
	gravity[0]->x=0;
	gravity[0]->y=-1;
	gravity[0]->z=0;
	gravity[1]=new vector3();
	
	gravity[1]->x=0.3;
	gravity[1]->y=0.3;
	gravity[1]->z=0;
	particlesSystem->setForces(gravity,2);
	/**/

	glut_init(argc, argv);
    gl_init();
    glutMainLoop();
	system("pause");
}