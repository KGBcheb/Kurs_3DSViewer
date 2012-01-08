#include <windows.h>
#include <fstream>
#include "Camera.h"
#include "C3DS.h"

extern C3DS scene;

// ����������� �� ���������
CCamera::CCamera(void)
{
	positionDefault=vec3((GLfloat)15,15,15);
	ViewPoint=vec3((GLfloat)0,0,0);
	Reset();
}

CCamera::~CCamera(void)
{
	//SaveToFile();
}

// ����������� �� ������� ����������
CCamera::CCamera(GLfloat x, GLfloat y, GLfloat z, GLfloat lx, GLfloat ly, GLfloat lz)
{
	positionDefault=vec3(x,y,z);
	ViewPoint=vec3(lx,ly,lz);
	Reset();
}

// ��������� �������
vec3 CCamera::GetPos()
{
	return position;
}

// ����� ������
void CCamera::Reset(void)
{
	position=positionDefault;
	//ViewPoint.x=0;
	//ViewPoint.y=0;
	//ViewPoint.z=0;
	//SaveToFile();
	Recalc();
}

// �������� ������� ������
void CCamera::Recalc(void)
{
	// �������� ��������
	forward = ViewPoint - position;
	forward = normalize(forward);

	right = cross(forward, vec3(0,1,0));
	right = normalize(right);

	up = cross(right, forward);
	up = normalize(up);

	// ������ ������
	matrix[0]=right.x;
	matrix[4]=right.y;
	matrix[8]=right.z;
	matrix[12]=0;
	// ������ �����
	matrix[1]=up.x;
	matrix[5]=up.y;
	matrix[9]=up.z;
	matrix[13]=0;
	// ������ -������
	matrix[2]=-forward.x;
	matrix[6]=-forward.y;
	matrix[10]=-forward.z;
	matrix[14]=0;
	// ��������� ������
	matrix[3]=0;
	matrix[7]=0;
	matrix[11]=0;
	matrix[15]=1;

	// ���������� ������� �������� � ���������� ��������
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(25.0,((float)windowWidth)/windowHeight,1.0,sqrt(position.x * position.x + position.y * position.y + position.z * position.z)*4);

	scene.SortObjects();
}

// �������� ������� ������ � �� ����������
void CCamera::ApplyCamera(void)
{
	GLdouble m[16];

	// ���������� ������
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixd(matrix);
	// ������� ������
	m[0]=1;	m[1]=0;	m[2]=0;	m[3]=0;
	m[4]=0;	m[5]=1;	m[6]=0;	m[7]=0;
	m[8]=0;	m[9]=0;	m[10]=1;m[11]=0;
	m[12]=-position.x; 
	m[13]=-position.y; 
	m[14]=-position.z; 
	m[15]=1;
	glMultMatrixd(m);
}

// ����������� ������ ������
void CCamera::Forward(void)
{
	double r=length(ViewPoint - position);
	if (r>4)
	{
		position=position+forward*r/20;
	}
	Recalc();
}

// ����������� ������ �����
void CCamera::Back(void)
{
	double r=length(ViewPoint - position);
	//if (r<60)
	{
		position=position-forward*r/20;
	}
	Recalc();
}

// ����������� ������ �����
void CCamera::Left(void)
{
	double r=length(ViewPoint - position);
	position=position+(-right+forward*(sqrt(r*r+1)-r))*r/20;
	Recalc();
}

// ����������� ������ ������
void CCamera::Right(void)
{
	double r=length(ViewPoint - position);
	position=position+(right+forward*(sqrt(r*r+1)-r))*r/20;
	Recalc();
}

// ����������� ������ �����
void CCamera::Up(void)
{
	double r=length(ViewPoint - position);
	if (asin((position.y - ViewPoint.y)/r)<1.4)
	{
		position=position+(up+forward*(sqrt(r*r+1)-r))*r/20;
	}
	Recalc();
}

// ����������� ������ ����
void CCamera::Down(void)
{
	double r=length(ViewPoint - position);
	if (asin((position.y - ViewPoint.y)/r)>-1.4)
	{
		position=position+(-up+forward*(sqrt(r*r+1)-r))*r/20;
	}
	Recalc();
}