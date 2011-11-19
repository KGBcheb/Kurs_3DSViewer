#pragma once
#include "main.h"
#include "fstream"

class Shader
{
	// �������� �� ������
	bool Loaded;
	// �������������� ��
	GLuint VShader;
	GLuint FShader;
public:
	// ������������� ��
	GLuint PShader;
private:
	// �������� ������� �� ����� � ����������
	bool Load(unsigned int Shader, string FileName);
public:
	// �������� �������
	void LoadShader(string Name);
	// ��������� �������
	void Activate();
	Shader(void);
	~Shader(void);
};
