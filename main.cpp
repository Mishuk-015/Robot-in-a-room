#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>
#endif
#include "RGBpixmap.cpp"
#define PI 3.1415927

#include <stdlib.h>
#include <stdio.h>
#include <math.h>


RGBpixmap pix[10];

int w,h;

float degree1=0;
float degree2=0;
float degree3=0;
float rx=0,ry=0,rz=0;
float zoom=1;
float dx=3.5+2,dy=2,dz=-3+8;//3.5,0,-3
static GLfloat l0x=10.0,loy=10.0,loz=10.0,l1x=20.0,l1y=30.0,l1z=30.0;


GLdouble eyex=2,eyey=60,eyez=6,cx=2,cy=-7,cz=3,ux=0,uy=4,uz=6;//2,-7,0,2,7,0,0,-4,6

GLdouble camtheta=0;
GLdouble camroll=90;
GLdouble camrad=-7;
GLdouble camheight=-7;

static GLdouble walk_phase_angle = 0;
static const GLdouble del_rotation = 5;
/* GLUT callback Handlers */

static GLfloat l0_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat l0_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat l0_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat l0_position[] = { 10, 10, 10, 1.0f };

static GLfloat l2_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat l2_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat l2_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat l2_position[] = { -10, -10, 20, 1.0f };

static GLfloat l1_ambient[] = { 0.05f, 0.05f, 0.05f, 1 };
static GLfloat l1_diffuse[] = { 1, 1, 1, 1 };
static GLfloat l1_specular[] = { 0, 0, 0, 1 };
static GLfloat l1_position[] = { 20, 30, 30, 1 };
static GLfloat l1_spot_dir[] = { -10,-10,-10};//-2.5,-2.5,-5
static GLfloat l1_spot_cutoff[] = { 10 };

//static GLfloat tabletop_color[] = { 1, 1, 0, 1 };
static GLfloat table_coloram[] = { 1.0f, 0.9f, 0.0f, 1.0f };
static GLfloat table_colordf[] = { 1.0f, 0.9f, 0.0f, 1.0f };
static GLfloat table_spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat table_shine[] = { 60.0f };

static GLfloat wall_coloram[] = { 0.6f, 0.6f, 0.6f, 1.0f };
static GLfloat wall_colordf[] = { 0.6f, 0.6f, 0.6f, 1.0f };
static GLfloat wall_spec[] = { 1.0f, 0.5f, 1.0f, 1.0f };
static GLfloat wall_shine[] = { 20.0f };

static GLfloat jack_coloram[] = { 0.0f, 1.0f, 1.0f, 1.0f };
static GLfloat jack_colordf[] = { 0.0f, 1.0f, 0.0f, 1.0f };
static GLfloat jack_spec[] = { 0.0f, 1.0f, 1.0f, 1.0f };
static GLfloat jack_shine[] = { 120 };

static GLfloat fireman_coloram[] = { 1.0f, 0.6f, 1.0f, 1.0f };
static GLfloat fireman_colordf[] = { 1.0f, 0.6f, 1.0f, 1.0f };
static GLfloat fireman_spec[] = { 1.0f, 0.5f, 1.0f, 1.0f };
static GLfloat fireman_shine[] = { 120.0f };

static void lightset()
{
    //static GLfloat l0_position[] = { l0x, l0y, l0z, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, l0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, l0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, l0_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, l0_position);

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.005);

    glLightfv(GL_LIGHT2, GL_AMBIENT, l2_ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, l2_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, l2_specular);
    glLightfv(GL_LIGHT2, GL_POSITION, l2_position);

    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0);
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.005);

    glLightfv(GL_LIGHT1, GL_AMBIENT, l1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, l1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, l1_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, l1_position);

    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, l1_spot_dir);
    glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, l1_spot_cutoff);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.01);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.001f);
}

static void matColor(GLfloat am[],GLfloat df[], GLfloat s[], GLfloat sh[])
{
    //GLfloat a[4] = { c[0]*0.1f, c[1]*0.1f, c[2]*0.1f, c[3] };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, am);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, df);
    glMaterialfv(GL_FRONT, GL_SPECULAR, s);
    glMaterialfv(GL_FRONT, GL_SHININESS, sh);
}


static void resize(int width, int height)
{
    const float ar = (float) (width/2) / (float) height;
    w=width;
    h=height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

void draw_cylinder(GLfloat radius,
                   GLfloat height,
                   GLubyte R,
                   GLubyte G,
                   GLubyte B)
{
    GLfloat x              = 0.0;
    GLfloat y              = 0.0;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;

    /** Draw the tube */
    glColor3ub(R,G,B);
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            glVertex3f(x, y , 0.0);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
        glVertex3f(radius, 0.0, 0.0);
    glEnd();

    /** Draw the circle on top of cylinder */
    glColor3ub(R,G,B);
    glBegin(GL_POLYGON);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
    glEnd();
}




void cube()
{
    glBegin(GL_QUADS);
        //glColor3d(0,0,1);
        glVertex3f(0,2,2);
        glVertex3f(0,0,2);
        glVertex3f(2,0,2);
        glVertex3f(2,2,2);
    glEnd();
    glBegin(GL_QUADS);
        //glColor3d(0,0,1);
        glVertex3f(0,2,0);
        glVertex3f(2,2,0);
        glVertex3f(2,0,0);
        glVertex3f(0,0,0);
    glEnd();
    glBegin(GL_QUADS);
        //glColor3d(0,0,1);
        glVertex3f(0,2,0);
        glVertex3f(0,0,0);
        glVertex3f(0,0,2);
        glVertex3f(0,2,2);
    glEnd();
    glBegin(GL_QUADS);
        //glColor3d(0,0,1);
        glVertex3f(2,2,2);
        glVertex3f(2,0,2);
        glVertex3f(2,0,0);
        glVertex3f(2,2,0);
    glEnd();
    glBegin(GL_QUADS);
        //glColor3d(0,0,1);
        glVertex3f(0,2,0);
        glVertex3f(0,2,2);
        glVertex3f(2,2,2);
        glVertex3f(2,2,0);
    glEnd();
    glBegin(GL_QUADS);
        //glColor3d(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(2,0,0);
        glVertex3f(2,0,2);
        glVertex3f(0,0,2);
    glEnd();
}

//GLUquadricObj* quadratic;
//quadratic = gluNewQuadric();

void trapizoid()
{
    glBegin(GL_QUADS);
        //glColor3d(1,0,0);
        glVertex3f(-5,1,9);
        glVertex3f(-5.5,.5,6);
        glVertex3f(-1.5,.5,6);
        glVertex3f(-2,1,9);
    glEnd();
    glBegin(GL_QUADS);
        //glColor3d(1,0,0);
        glVertex3f(-5,2,9);
        glVertex3f(-2,2,9);
        glVertex3f(-1.5,2.5,6);
        glVertex3f(-5.5,2.5,6);
    glEnd();
    glBegin(GL_QUADS);
        //glColor3d(1,0,0);
        glVertex3f(-5,1,9);
        glVertex3f(-5,2,9);
        glVertex3f(-5.5,2.5,6);
        glVertex3f(-5.5,.5,6);
    glEnd();
    glBegin(GL_QUADS);
        //glColor3d(1,0,0);
        glVertex3f(-1.5,2.5,6);
        glVertex3f(-2,2,9);
        glVertex3f(-2,1,9);
        glVertex3f(-1.5,.5,6);
    glEnd();
    glBegin(GL_QUADS);
        //glColor3d(1,0,0);
        glVertex3f(-2,1,9);
        glVertex3f(-2,2,9);
        glVertex3f(-5,2,9);
        glVertex3f(-5,1,9);
    glEnd();
    glBegin(GL_QUADS);
        //glColor3d(1,0,0);
        glVertex3f(-1.5,2.5,6);
        glVertex3f(-1.5,.5,6);
        glVertex3f(-5.5,.5,6);
        glVertex3f(-5.5,2.5,6);
    glEnd();
}

void fireman()
{
    const GLdouble max_hand_angle = 30;
    const GLdouble max_leg_angle = 20;
    matColor(fireman_coloram,fireman_colordf,fireman_spec,fireman_shine);
    glPushMatrix();

        //matColor(fireman_color,fireman_spec,fireman_shine);
        //right foot
        glPushMatrix();
            glTranslated(-3, 0,0);
            glScaled(.5, 1, .5);
            cube();
        glPopMatrix();
        //right leg
        glPushMatrix();
            glTranslated(-3,1, 1);
            glScaled(.5, .5, 2);
            cube();
        glPopMatrix();
        //left foot
        glPushMatrix();
            glTranslated(-5, 0, 0);
            glScaled(.5, 1, .5);
            cube();
        glPopMatrix();
        //left leg
        glPushMatrix();
            glTranslated(-5,1, 1);
            glScaled(.5, .5, 2);
            cube();
        glPopMatrix();
        //torso
        glPushMatrix();
            glColor3d(0,1,1);
            glTranslated(-5, 1, 5);
            glScaled(1.5, .5, .5);
            cube();
        glPopMatrix();
        /*/upperbody
        glPushMatrix();
            glTranslated(-5, 1, 6);
            glScaled(1.5, .5, 1.5);
            cube();
        glPopMatrix();*/

        //right arm from my view
        glPushMatrix();
            glTranslated(-1.5, 2, 8);
            glRotated(-max_hand_angle * sin(walk_phase_angle * PI / 180.0) + 10, 1, 0, 0);
            glTranslated(1.5, -2, -8);
            glPushMatrix();
                //glTranslated(-5, 0, 0);
                //glRotated(30,0,1,0);
                glTranslated(-2, 1, 8);
                glScaled(.75, .5, .5);
                cube();
            glPopMatrix();
            //right hand from my view
            glPushMatrix();
                //glTranslated(-5, 0, 0);
                //glRotated(30,0,1,0);
                glTranslated(-1.5, 1, 5);
                glScaled(.5, .5, 1.5);
                cube();
            glPopMatrix();
        glPopMatrix();
        glPushMatrix();
            glTranslated(-1.5, 2, 8);
            glRotated(max_hand_angle * sin(walk_phase_angle * PI / 180.0) + 10, 1, 0, 0);
            glTranslated(1.5, -2, -8);
            //left arm from my view
            glPushMatrix();
                //glTranslated(-5, 0, 0);
                //glRotated(210,0,1,0);
                glTranslated(-6.5, 1, 8);
                glScaled(.75, .5, .5);
                cube();
            glPopMatrix();
            //left hand from my view
            glPushMatrix();
                //glTranslated(-5, 0, 0);
                //glRotated(210,0,1,0);
                glTranslated(-6.5, 1, 5);
                glScaled(.5, .5, 1.5);
                cube();
            glPopMatrix();
        glPopMatrix();
        //neck
        glPushMatrix();
            //glTranslated(-5, 0, 0);
            //glRotated(210,0,1,0);
            glTranslated(-3.5, 1.5, 9);
            //glScaled(1.5, .5, .5);
            draw_cylinder(0.35, 2, 255, 255, 0);
        glPopMatrix();
        //head
        glPushMatrix();
            //glTranslated(-5, 0, 0);
            //glRotated(210,0,1,0);
            glTranslated(-3.5, 1.5, 9.5);
            //glScaled(1.5, .5, .5);
            draw_cylinder(0.8, 1.3, 255, 255, 0);
        glPopMatrix();
        //upperbody
        glPushMatrix();
            trapizoid();
        glPopMatrix();

    glPopMatrix();

}

void myscalef(GLdouble sx,GLdouble sy,GLdouble sz)
{
    GLdouble sm[16];

    sm[0]=sx;sm[1]=0;sm[2]=0;sm[3]=0;
    sm[4]=0,sm[5]=sy,sm[6]=0,sm[7]=0;
    sm[8]=0,sm[9]=0,sm[10]=sz,sm[11]=0;
    sm[12]=0,sm[13]=0,sm[14]=0,sm[15]=1;

    glMatrixMode(GL_MODELVIEW);
    glMultMatrixd(sm);
}

static void cameraChanges()
{
    GLdouble anglerad=camtheta*3.1416;
    GLdouble cosangle=cos(anglerad);
    GLdouble sinangle=sin(anglerad);

    GLdouble camz=camrad*cosangle;
    GLdouble camx=camrad*sinangle;
    GLdouble camy=camheight;

    GLdouble tx,ty,tz;
    GLdouble rollrad=camroll*3.1416;
    tx=cos(rollrad)*ux-sin(rollrad)*uy;

    ux=tx,uy=ty;
    tx=cosangle*ux+sinangle*uz;
    tz=-sinangle*ux+cosangle*uz;
    uz=tx,uz=tz;

    gluLookAt(camx,camy,camz,cx,cy,cz,ux,uy,uz);
}

void sphere()
{
    //glColor3b(1,1,0);
    glutSolidSphere(.3,30,30);
}

void jackarm()
{
    glPushMatrix();
        glPushMatrix();
            glTranslatef(0,0,2.33);
            sphere();
        glPopMatrix();
        glPushMatrix();
            glScalef(1,1,4);
            glTranslatef(0,0,.3);
            sphere();
        glPopMatrix();
        glPushMatrix();
            sphere();
        glPopMatrix();
    glPopMatrix();
}

void jack()
{
    matColor(jack_coloram,jack_colordf,jack_spec,jack_shine);
    glPushMatrix();
        //matColor(jack_color,jack_spec,jack_shine);
            glPushMatrix();
                //glTranslated(2,-1,0);
                glRotated(240,0,0,1);
                glRotated(45,0,1,0);
                jackarm();
            glPopMatrix();
            glPushMatrix();
                //glTranslated(2,-1,0);
                glTranslatef(0,-1.25,0);
                glRotated(120,0,0,1);
                glRotated(45,0,1,0);
                jackarm();
            glPopMatrix();
            glPushMatrix();
                //glTranslated(2,-1,0);
                glTranslatef(-1.15,-.7,0);
                glRotated(45,0,1,0);
                jackarm();
            glPopMatrix();
    glPopMatrix();
}

void wallsout()
{
    matColor(wall_coloram,wall_colordf,wall_spec,wall_shine);
    glPushMatrix();
                glTranslatef(-10,0,0);
                myscalef(.25,10,10);
                cube();
    glPopMatrix();
}

void wallsz()
{
    matColor(wall_coloram,wall_colordf,wall_spec,wall_shine);
    glPushMatrix();
                myscalef(10,10,.25);
                cube();
    glPopMatrix();
}
void wallsy()
{
    matColor(wall_coloram,wall_colordf,wall_spec,wall_shine);
    glPushMatrix();
                glTranslated(0,0,20);
                myscalef(10,.25,10);
                glRotated(270,1,0,0);
                cube();
            glPopMatrix();
}
void wallsup()
{
    matColor(wall_coloram,wall_colordf,wall_spec,wall_shine);
    glPushMatrix();
                glTranslated(0,0,20);
                myscalef(10,10,.25);
                cube();
            glPopMatrix();
}
void wallsx()
{
    matColor(wall_coloram,wall_colordf,wall_spec,wall_shine);

        //matColor(wall_color,wall_spec,wall_shine);
            /*glPushMatrix();// View From the window
                glTranslatef(-10,0,0);
                myscalef(.25,10,10);
                cube();
            glPopMatrix();*/
            glPushMatrix();// window above column
                glTranslated(0,8,12);
                myscalef(.25,4,4);
                glRotated(270,0,1,0);
                cube();
            glPopMatrix();
            glPushMatrix();// Window below column
                glTranslated(0,8,0);
                myscalef(.25,4,4);
                glRotated(270,0,1,0);
                cube();
            glPopMatrix();
            glPushMatrix();// Right side of windowed wall
                glTranslated(0,12,0);
                myscalef(.25,4,10);
                glRotated(270,0,1,0);
                cube();
            glPopMatrix();
            glPushMatrix();
                myscalef(.25,4,10);
                glRotated(270,0,1,0);
                cube();
            glPopMatrix();
            /*glPushMatrix();
                myscalef(10,.25,10);
                cube();
            glPopMatrix();
            glPushMatrix();
                myscalef(10,10,.25);
                cube();
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0,0,20);
                myscalef(10,10,.25);
                cube();
            glPopMatrix();*/
}

void table()
{
    matColor(table_coloram,table_colordf,table_spec,table_shine);
    glPushMatrix();
        myscalef(.25,.25,2);
        cube();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(4.5,0,0);
        myscalef(.25,.25,2);
        cube();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0,4.5,0);
        myscalef(.25,.25,2);
        cube();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(4.5,4.5,0);
        myscalef(.25,.25,2);
        cube();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0,0,4);
        myscalef(2.5,2.5,.5);
        cube();
    glPopMatrix();
}

void left_viewport()
{
    gluLookAt(eyex,eyey,eyez,cx,cy,cz,ux,uy,uz);
    glPushMatrix();
        lightset();
        glRotated(45,0,0,1);
        glPushMatrix();
            glTranslatef(dx,dy,dz);
        //glTranslatef(1,-2.2,-10);
            //glScalef(zoom,zoom,zoom);
            myscalef(zoom,zoom,zoom);
            glRotated(degree2,1,0,0);
            glRotated(degree3,0,1,0);
            glRotated(degree1,0,0,1);
            //rx+=degree2;
            //ry+=degree3;
            //rz+=degree1;
            glScalef(.5,.5,.5);
            fireman();
        glPopMatrix();
        glPushMatrix();
            glTranslated(0,0,5.25);
            glTranslated(2,2,0);
            jack();
        glPopMatrix();
        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D,2);
            glEnable(GL_TEXTURE_2D);
            glTranslated(-8,-8,-.5);
            wallsout();
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D,2);
            glEnable(GL_TEXTURE_2D);
            glTranslated(-8,-8,-.5);
            wallsx();
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D,2);
            glEnable(GL_TEXTURE_2D);
            glTranslated(-8,-8,-.5);
            wallsy();
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D,2);
            glEnable(GL_TEXTURE_2D);
            glTranslated(-8,-8,-.5);
            wallsz();
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D,2);
            glEnable(GL_TEXTURE_2D);
            glTranslated(-8,-8,-.5);
            wallsup();
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D,4);
            glEnable(GL_TEXTURE_2D);
            table();
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    glPopMatrix();
}

void right_viewport()
{
    gluLookAt(eyex,eyey,eyez,cx,cy,cz,ux,uy,uz);
    glPushMatrix();
        lightset();
        glRotated(45,0,0,1);
        glPushMatrix();
            glTranslatef(dx,dy,dz);
        //glTranslatef(1,-2.2,-10);
            //glScalef(zoom,zoom,zoom);
            myscalef(zoom,zoom,zoom);
            glRotated(degree2,1,0,0);
            glRotated(degree3,0,1,0);
            glRotated(degree1,0,0,1);
            //rx+=degree2;
            //ry+=degree3;
            //rz+=degree1;
            glScalef(.5,.5,.5);
            fireman();
        glPopMatrix();
        glPushMatrix();
            glTranslated(0,0,5.25);
            glTranslated(2,2,0);
            jack();
        glPopMatrix();
        glPushMatrix();
            //glBindTexture(GL_TEXTURE_2D,2);
            //glEnable(GL_TEXTURE_2D);
            glTranslated(-8,-8,-.5);
            wallsx();
            //glDisable(GL_TEXTURE_2D);
        glPopMatrix();
        glPushMatrix();
            glTranslated(-8,-8,-.5);
            wallsout();
        glPopMatrix();
        glPushMatrix();
            //glBindTexture(GL_TEXTURE_2D,2);
            //glEnable(GL_TEXTURE_2D);
            glTranslated(-8,-8,-.5);
            wallsy();
            //glDisable(GL_TEXTURE_2D);
        glPopMatrix();
        glPushMatrix();
            //glBindTexture(GL_TEXTURE_2D,2);
            //glEnable(GL_TEXTURE_2D);
            glTranslated(-8,-8,-.5);
            wallsz();
            //lDisable(GL_TEXTURE_2D);
        glPopMatrix();
        glPushMatrix();
            //glBindTexture(GL_TEXTURE_2D,2);
            //glEnable(GL_TEXTURE_2D);
            glTranslated(-8,-8,-.5);
            wallsup();
            //glDisable(GL_TEXTURE_2D);
        glPopMatrix();
        glPushMatrix();
            table();
        glPopMatrix();
    glPopMatrix();
}

static void display(void)
{
    lightset();
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glColor3d(1,0,0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //GLdouble eyex=5,eyey=5,eyez=5,cx=-4,cy=3,cz=4,ux=4,uy=5,uz=5;
    //cameraChanges();
    //gluLookAt(eyex,eyey,eyez,cx,cy,cz,ux,uy,uz);//3,4,0,3,5
    //printf("x=%f\ty=%f\tz=%f\n",eyex,eyey,eyez);
    //gluLookAt(GLdouble eyex,GLdouble eyey,GLdouble eyez,GLdouble cX,GLdouble cY,GLdouble cZ,GLdouble ux,GLdouble uy,GLdouble uz);

    glPushMatrix();
        glViewport(0,0,w/2,h);
        left_viewport();
    glPopMatrix();
    glPushMatrix();
        glViewport(w/2,0,w/2,h);
        right_viewport();
    glPopMatrix();
        glutSwapBuffers();
}

void texture_init()
{
    pix[0].makeCheckImage();
	pix[0].setTexture(1);

	pix[1].readBMPFile("wall1.bmp");//Put Path of the project folder before image name, use double slashes
	pix[1].setTexture(2);

	pix[2].readBMPFile("wall2.bmp");//Put Path of the project folder before image name, use double slashes
	pix[2].setTexture(3);

	pix[3].readBMPFile("wood.bmp");//Put Path of the project folder before image name, use double slashes
	pix[3].setTexture(4);

}
static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case '[':
            glDisable(GL_LIGHT1);
            break;
        case ']':
            glEnable(GL_LIGHT1);
            break;
         case '{':
            glDisable(GL_LIGHT2);
            break;
        case '}':
            glEnable(GL_LIGHT2);
            break;
        case '5':
            glDisable(GL_LIGHT0);
            break;
        case '6':
            glEnable(GL_LIGHT0);
            break;
        case 'r':
            camroll+=5;
            break;
        case 'R':
            camroll-=5;
            break;
        case 'a':
            eyex++;
            break;
        case 'z':
            eyex--;
            break;
        case 's':
            eyey++;
            break;
        case 'x':
            eyey--;
            break;
        case 'd':
            eyez++;
            break;
        case 'c':
            eyez--;
            break;
        case 'f'://yaw
            cx+=.05;
            break;
        case 'v'://yaw
            cx-=.05;
            break;
        case 'g':
            cy++;
            break;
        case 'b':
            cy--;
            break;
        case 'h'://pitch
            cz+=.1;
            break;
        case 'n'://pitch
            cz-=.1;
            break;
        case 'j'://roll
            ux+=.2;
            break;
        case 'm'://roll
            ux-=.2;
            break;
        case 'k':
            uy++;
            break;
        case ',':
            uy--;
            break;
        case 'l':
            uz++;
            break;
        case '.':
            uz--;
            break;

        case 'q':
            exit(0);
            break;
        case 't':
            degree1+=10;
            //rz+=10;
            break;
        case 'T':
            degree1-=10;
            //rz-=degree1;
            break;
        case 'u':
            degree2+=10;
            //rx+=degree2;
            break;
        case 'U':
            degree2-=10;
            //rx-=degree2;
            break;
        case 'y':
            degree3+=10;
            //ry+=degree3;
            break;
        case 'Y':
            degree3-=10;
            //ry-=degree3;
            break;
        case '+':
            zoom=zoom+.1;
            break;

        case '-':
            zoom=zoom-.1;
            break;

        case 'w':
            walk_phase_angle+=del_rotation;
            break;
        case 'W':
            walk_phase_angle-=del_rotation;
            break;

        case '1':
            dx++;
            break;
        case '2':
            dy++;
            break;
        case '3':
            dz++;
            break;
        case '7':
            dx--;
            break;
        case '8':
            dy--;
            break;
        case '9':
            dz--;
            break;

        case '`':
            //dx=1,dy=-2.2,dz=-10;
            dx-=dx,dy-=dy,dz-=dz;
            degree3=-0;
            degree2=-0;
            degree1=-0;
            zoom=.5;
            break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 40.0f, 4.0f, 10.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT2);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    texture_init();
    //glEnable(GL_COLOR_MATERIAL);


    /*glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);*/

    glutMainLoop();

    return EXIT_SUCCESS;
}
