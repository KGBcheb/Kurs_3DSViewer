#pragma once
#include "main.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "VBOInfo.h"
#include <fstream>

using namespace glm;

class C3DSObject
{
	friend class C3DS;
	friend void ReadChunk(ifstream &, C3DS*);
private:
	// ��� �������
	string name;
	// ��������
	Texture texture;
	// ������� ������� ARB
	GLuint Buffer;
	// ���������� �����. ���������
	unsigned short TexVertCount;
	// ������ �����. ���������
	GLfloat *TexVertList;
	// ���������� ��������
	unsigned int IndexCount;
	// ������ ��������
	unsigned short *IndexList;
	// ���������� ������
	unsigned short VertexCount;
	// ������ ������
	GLfloat *VertexList;
	// ����� VBO ��� ������
	GLuint VertexVBO;
	// ����� VBO ��� ��������
	vector<CVBOInfo> IndexVBO;
	// ��������� �������
	GLfloat LocalMatrix[12];
	// ������
	//Shader *shader;
	// �����������
	C3DSObject();
	~C3DSObject(void);
public:
	// ��������� �����
	void SetName(char* name);
	// ��������� ������ ����������
	void SetFilterMode(char);
	// ������� ��������
	void ClearTexture();
	// ��������� ��������
	void SetTexture(string);
	// ����� ������� �� �����
	void Render (void);
};
