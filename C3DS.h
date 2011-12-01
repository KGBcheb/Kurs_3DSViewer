#pragma once
#include "main.h"
#include "Shader.h"
#include "C3DSObject.h"
#include <fstream>

using namespace glm;

class C3DS
{
private:
	vector<C3DSObject*> objects;
public:
	void SetFilterMode(char);
	// ������� ��������
	void ClearTexture();
	// ��������� ��������
	void SetTexture(string);
	// �������� ����� 3ds
	bool Load (const char *FileName, Shader*);
	// ����� ������� �� �����
	void Render (void);
	C3DS();
	~C3DS(void);
};
