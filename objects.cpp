#include <fstream>
#include "C3DS.h"
#include "Shader.h"
	
C3DS scene;

extern void DrawFps();

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
void LoadObjects(char* filename)
{
	string _fnf = ProgPath+"Models\\"+string(filename);
	if (!FileExists(_fnf.c_str()))
	{
		WriteLogF("Scene file does not exists, exiting...");
		exit(1);
	}
	TextureFilter = TEXTURE_FILTER_NEAREST;

	// �������� �����
	scene.Load(_fnf.c_str(), &ModelShader);

	// �������� ��������
	ModelShader.LoadShader("Phong");
}

void RenderObjects()
{
	// ���������� ��������� �����
	CLight* _light = scene.GetCurrentLight();
	if (_light != NULL)
		_light->ApplyLight();

	// ���������� ������
	ModelShader.Activate();
	// ����� ��������
	scene.Render();

	ModelShader.Deactivate();
	DrawFps();
}