#ifndef __Light_h__
#define __Light_h__

#include "main.h"
#include <windows.h>

using namespace glm;

class CLight
{
private:
	GLfloat position[4];
	vec3 target;
	GLfloat color[4];
	GLfloat directColor[4];
	bool isSpotLight;
	bool enabled;

	void CalculateDirectColor();
public:
	CLight(bool, vec3);
	~CLight(void);
	// ��������� �����������
	void SetTarget(vec3);
	// ��������� �����
	void SetColor(vec3);
	// ��������� �����
	void SetColor(ivec3);
	void ApplyLight(void);
};
#endif