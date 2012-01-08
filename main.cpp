#include <conio.h>
#include <stdio.h>
#include "main.h"
#include "C3DS.h"
#include "Camera.h"
#include "Shader.h"
#include "vkscancodes.h"

extern C3DS scene;
CCamera* camera;

inline string GetPathFromFilename( const string& filename )
{
	return filename.substr( 0, filename.rfind("\\")+1 );
}


void ToggleCullFace()
{
	if (CullFaceFlag)
		glDisable(GL_CULL_FACE);
	else
		glEnable(GL_CULL_FACE);
	CullFaceFlag = !CullFaceFlag;
}


// ������� ���������� ��� ����������� ����
// � ��� ����� � �������������, �� �������� glutPostRedisplay
void Display (void)
{
	// ������� ����� ����� � ����� �������
	glClearColor(0.3,0.3,0.3,1.0);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ����� ������ ��������� (������� � ���� �����)
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	// ������������� ������
	camera->ApplyCamera();

	// ��������� ��������
	RenderObjects();

	// ����� ��������� � ������� �������
	glutSwapBuffers();
}

// �������, ���������� ��� ��������� �������� ����
void Reshape (int w,int h)
{
	windowWidth = w;
	windowHeight = h;
	// ���������� ����� ������� ���������, ������ ���� ������� ����
	glViewport(0,0,(GLsizei)w, (GLsizei)h);

	// ���������� ������� �������� � ���������� ��������
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (camera != NULL)
	{
		vec3 _camPos = camera->GetPos();
		gluPerspective(25.0,((float)w)/h,1.0,sqrt(_camPos.x * _camPos.x + _camPos.y * _camPos.y + _camPos.z * _camPos.z)*15);
	}
	else
		gluPerspective(25.0,((float)w)/h,1.0,1000.0);

	// ������������� ������������ ����
	glutPostRedisplay();
}

// ������� ���������� �� �������
void Simulation(void)
{
	// ������������� ������������ ����
	glutPostRedisplay();
}

void KeyPressed(unsigned char c, int, int)
{
	// ��������� ������������ ��������� ��������� ������ ���������
	HKL _keyboardLayout = GetKeyboardLayout(0);
	// ��������� ������������ ���� ������� �������
	int _key = VkKeyScanExA(c,_keyboardLayout);

	switch (c)
	{
		case '+':
			camera->Forward();
			break;
		case '-':
			camera->Back();
			break;
	}

	switch (_key)
	{
		case VK_ADD:
			camera->Forward();
			break;
		case VK_SUBTRACT:
			camera->Back();
			break;
		case VK_W:
			camera->Up();
			break;
		case VK_S:
			camera->Down();
			break;
		case VK_A:
			camera->Left();
			break;
		case VK_D:
			camera->Right();
			break;
		case VK_R:
			camera->Reset();
			break;	
		case VK_TAB:
			camera = scene.GetNextCamera();
			break;	
		case VK_L:
			ToggleCullFace();
			break;	
		case VK_O:
			UseOcclusionCulling = !UseOcclusionCulling && GLEE_ARB_occlusion_query;
			WriteLogF("Occlusion culling: %s", UseOcclusionCulling?"Enabled":"Disabled");
			break;
		case VK_T:
			ChangeTextureFilter();
			break;
	}
}

void main(int argc,char **argv)
{
	setlocale(LC_CTYPE, "RUS");

	ProgPath = GetPathFromFilename(argv[0]);
	LogFile = ProgPath + "Logs\\" + getNowToString() + ".log";
	if (!FileExists((ProgPath + "Logs").c_str()))
	{
		CreateDirectory((ProgPath + "Logs").c_str(), NULL);
	}

	// ������������� ���������� GLUT
	glutInit(&argc,argv);
	// ������������� ������� (������ ������)
	glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	// �������� ����:
	// 1. ������������� ������� ����� ���� ����
	glutInitWindowPosition(200,200);
	// 2. ������������� ������ ����
	glutInitWindowSize(600,600);
	// 3. ������� ����
	glutCreateWindow("�������� ������");
	// 4. ������������� �������, ������� ����� ���������� ��� ����������� ����
	glutDisplayFunc(Display);
	// 5. ������������� �������, ������� ����� ���������� ��� ��������� �������� ����
	glutReshapeFunc(Reshape);
	// 6. ������������� �������, ������� ����� ���������� ��� ������� �������
	glutKeyboardFunc(KeyPressed);
	// 7. ������������� ������� ������� ���������� ������ ���, ����� ��������� �����������
	glutIdleFunc(Simulation);

	// ����� ����� ����������� OpenGL
	// ����� �������������
	WriteLogF("Vendor: %s", glGetString(GL_VENDOR));
	WriteLogF("Render: %s", glGetString(GL_RENDER));
	WriteLogF("Version: %s", glGetString(GL_VERSION));
	// ����� ������ ���������� �����
	WriteLogF("GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
	// ����� ���� ��������� ����������
	char *_ext=(char*)glGetString(GL_EXTENSIONS);
	char *_extForPrint = new char[strlen(_ext) * 2];
	int i = 0, j = 0;
	do 
	{
		if (_ext[i] ==' ')
		{
			_extForPrint[j] = '\n';
			j++;
			_extForPrint[j] = '\t';
		}
		else
			_extForPrint[j] = _ext[i];
		j++;
	} while (_ext[i++] != NULL);

	WriteLogF("%s\n\t%s", "Supported extensions:", _extForPrint);
	delete[] _extForPrint;

	// �������� ��������� VBO
	if (GLEE_ARB_vertex_buffer_object)
	{
		WriteLogF("supporting VBO ......OK");
	}
	else
	{
		WriteLogF("supporting VBO ......fail");
		printf("Press any key...");
		_getch();
		exit(1);
	}

	// �������� ��������� ��������
	if (GLEE_ARB_shader_objects && GLEE_ARB_fragment_shader && GLEE_ARB_vertex_shader)
	{
		WriteLogF("supporting shaders ......OK");
		if (GLEE_ARB_occlusion_query)
		{
			WriteLogF("supporting occlusion query ......OK");
		}
		else
			WriteLogF("supporting occlusion query ......fail");
	}
	else
	{
		WriteLogF("supporting shaders ......fail");
		printf("Press any key...");
		_getch();
		exit(1);
	}

	// �������� ��������
	if (argc>1)
		LoadObjects(argv[1]);
	else
		LoadObjects("scene.3ds");

	printf("\n");
	camera = scene.GetCurrentCamera();
	UseOcclusionCulling = GLEE_ARB_occlusion_query;
	
	// �������� ���� ��������� ��������� ��
	glutMainLoop();
	return;
}