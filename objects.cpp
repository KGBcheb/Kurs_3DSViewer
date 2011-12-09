#include <fstream>
#include "C3DS.h"
#include "Shader.h"
	
C3DS scene;

extern void DrawFps();

struct Vertex {
	GLfloat Coord[3];
	GLfloat TexCoord[2];
	GLfloat Color[3];
	GLfloat Normal[3];
};

char TextureFilter;

Shader ModelShader;

// ��������� ������ ����������
void ChangeTextureFilter() 
{
	++TextureFilter;
	TextureFilter%=4;
	scene.SetFilterMode(TextureFilter);
}

// �������� ��������.
void LoadObjects()
{
	TextureFilter = TEXTURE_FILTER_NEAREST;

	scene.Load((ProgPath+"Models\\scene2.3ds").c_str(), &ModelShader);

	// �������� ��������
	ModelShader.LoadShader("Phong");
}

// �������� �������
void SetModelMaterial()
{
	GLfloat ambient[4]={1,1,1,1}, diffuse[4]={0.8,0.8,0.8,1}, specular[4]={0.0,0.0,0.0,1}, shininess=32.0;

	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS, &shininess);
}

void RenderObjects()
{
	// ���������� ��������� �����
	CLight* _light = scene.GetCurrentLight();
	if (_light != NULL)
		_light->ApplyLight();

	SetModelMaterial();

	// ���������� ������
	ModelShader.Activate();
	// ����� ��������
	scene.Render();

	ModelShader.Deactivate();
	DrawFps();
}