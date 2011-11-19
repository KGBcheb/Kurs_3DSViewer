#include <windows.h>
#include <fstream>
#include "Camera.h"

// ����������� �� ���������
CCamera::CCamera(void)
{
	// ������� ������
	position=vec3((GLfloat)15,15,15);
	// ����� ����������
	ViewPoint=vec3((GLfloat)0,0,0);
	// ������ ������
	forward = vec3((GLfloat)0,0,0);
	// ������ ������
	right = vec3((GLfloat)0,0,0);
	// ������ �����
	up = vec3((GLfloat)0,0,0);
	Recalc();
}

CCamera::~CCamera(void)
{
	SaveToFile();
}

// ����������� �� ������� ����������
CCamera::CCamera(GLfloat x, GLfloat y, GLfloat z, GLfloat lx, GLfloat ly, GLfloat lz)
{
	position=vec3(x,y,z);
	ViewPoint=vec3(lx,ly,lz);
	forward = vec3((GLfloat)0,0,0);
	right = vec3((GLfloat)0,0,0);
	up = vec3((GLfloat)0,0,0);
	Recalc();
}

// �������� ������� ������
void CCamera::Reset(void)
{
	position.x=15;
	position.y=15;
	position.z=15;
	ViewPoint.x=0;
	ViewPoint.y=0;
	ViewPoint.z=0;
	SaveToFile();
	Recalc();
}

// ����� ������
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
	if (length(ViewPoint - position)>4)
	{
		position=position+normalize(ViewPoint - position);
		//SaveToFile();
	}
	Recalc();
}

// ����������� ������ �����
void CCamera::Back(void)
{
	if (length(ViewPoint - position)<60)
	{
		position=position-normalize(ViewPoint - position);
		//SaveToFile();
	}
	Recalc();
}

// ����������� ������ �����
void CCamera::Left(void)
{
	double r=length(ViewPoint - position);
	position=position-right-normalize(ViewPoint - position)*(sqrt(r*r-1)-r);
	//SaveToFile();
	Recalc();
}

// ����������� ������ ������
void CCamera::Right(void)
{
	double r=length(ViewPoint - position);
	position=position+right+normalize(ViewPoint - position)*(sqrt(r*r-1)-r);
	//SaveToFile();
	Recalc();
}

// ����������� ������ �����
void CCamera::Up(void)
{
	double r=length(ViewPoint - position);
	if (asin(position.y/r)<1.5)
	{
		position=position+up+normalize(ViewPoint - position)*(sqrt(r*r-1)-r);
		//SaveToFile();
	}
	Recalc();
}

// ����������� ������ ����
void CCamera::Down(void)
{
	double r=length(ViewPoint - position);
	if (asin(position.y/r)>0.1)
	{
		position=position-up-forward*(sqrt(r*r-1)-r);
		//SaveToFile();
	}
	Recalc();
}

// �������� ������� �� �����
void CCamera::LoadFromFile(void)
{
	ifstream ConfigF(ConfigFile.c_str(), ios::in);
	// ��������� �����
	char str[MAX_CONFIG_LINE_LENGTH];
	string str2;

	int pos;

	vector<string> v;

	while (ConfigF.good())
	{
		ConfigF.getline(str,MAX_CONFIG_LINE_LENGTH-1);
		str2=str;
		str2=StrToLower(remove_spaces(str2));
		// ��� ��������� ������������� ����������
		pos=str2.find("campos=(");
		if (pos != string::npos)
		{
			split(str2.substr(pos+strlen("campos=(")),",;)", v);
			if (v.size()>2)
			{
				for (int i=0;i<3;++i)
					position[i]=atof(v[i].c_str());
			}
			WriteLogF("Loaded camera position: %f %f %f\n",position.x, position.y, position.z);
		}
	}

	if (length(ViewPoint - position)>120 || length(ViewPoint - position)<2)
		Reset();
	Recalc();
	ConfigF.close();
}

// ���������� ������� � ����
void CCamera::SaveToFile(void)
{
	char buf[1024];
	SetFileAttributes(ConfigFile.c_str(), FILE_ATTRIBUTE_NORMAL); 
	
	sprintf_s(buf, "(%f; %f; %f)", position.x, position.y, position.z); 

	WritePrivateProfileString("Config","campos",buf,ConfigFile.c_str());
	//printf("Saved camera position: %f %f %f\n",position.x, position.y, position.z);
}