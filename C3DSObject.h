#pragma once
#include "main.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include <fstream>

using namespace glm;

class C3DSObject
{
	friend class C3DS;
	friend void ReadChunk(ifstream &, vector<C3DSObject*> &, vector<CCamera*> &, vector<CLight*> &);
private:
	// ��� �������
	char name[256];
	// ������� �������
	vec3 position;
	// ���� �������
	vec3 color;
	// ������� �������
	GLfloat scale;
	// ��������
	Texture texture;
	// ������� ������� ARB
	GLuint Buffer;
	// ���������� �����. ���������
	unsigned short TexVertCount;
	// ������ �����. ���������
	GLfloat *TexVertList;
	// ���������� ��������
	unsigned short IndexCount;
	// ������ ��������
	unsigned short *IndexList;
	// ���������� ������
	unsigned short VertexCount;
	// ������ ������
	GLfloat *VertexList;
	// ����� VBO ��� ������
	GLuint VertexVBO;
	// ����� VBO ��� ��������
	GLuint IndexVBO;
	// ��������� �������
	GLfloat LocalMatrix[12];
	// ������
	Shader *shader;
	// �����������
	C3DSObject();
public:
	// ��������� �������
	void SetPos(GLfloat x, GLfloat y, GLfloat z);
	// ��������� �����
	void SetColor(GLfloat r, GLfloat g, GLfloat b);
	// ��������� ��������
	void SetScale(GLfloat);
	// ��������� ������ ����������
	void SetFilterMode(char);
	// ������� ��������
	void ClearTexture();
	// ��������� ��������
	void SetTexture(string);
	//// �������� ����� 3ds � ������ � ������ VBO
	//bool Load (const char *FileName, Shader*);
	// ����� ������� �� �����
	void Render (void);
	~C3DSObject(void);
};
