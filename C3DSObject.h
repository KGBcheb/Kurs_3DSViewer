#pragma once
#include "main.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "VBOInfo.h"
#include "Occluder.h"
#include <fstream>

using namespace glm;

class C3DSObject
{
	friend class C3DS;
	friend bool ObjectsComparer(C3DSObject *obj1, C3DSObject *obj2);
	friend void ReadChunk(ifstream &, C3DS*);
private:
	// ��� �������
	string name;
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
	// ������ VBO ��� ��������
	vector<CVBOInfo> IndexVBO;
	// ��������� �������
	GLfloat LocalMatrix[12];
	// Occluder
	COccluder* occluder;
	// ��� �� ��������� �� ��������� �����
	bool wasDrawn;
	// ���������� ��
	bool isTransparent;
	// �����������
	C3DSObject();
	~C3DSObject(void);
public:
	// ��������� �����
	void SetName(char* name);
	// ����� ������� �� �����
	void Render (void);
};
