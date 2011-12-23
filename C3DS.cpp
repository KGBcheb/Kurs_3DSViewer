#include "C3DS.h"
#include "Occluder.h"

extern GLuint occluderBufferVBOIndex;
// �������� ����� 3ds
extern bool Load3DSFile(const char *FileName, C3DS* scene);

// ��������� ������ ����������
void C3DS::SetFilterMode(char mode)
{
	for (uint i = 0; i < objects.size(); i++)
		objects[i]->SetFilterMode(mode);
}

// ��������� ������� ������
CCamera* C3DS::GetCurrentCamera()
{
	return cameras[cameraIndex];
}

// ��������� ��������� ������
CCamera* C3DS::GetNextCamera()
{
	return cameras[(++cameraIndex)%cameras.size()];
}

// ��������� �������� ��������� �����
CLight* C3DS::GetCurrentLight()
{
	if (lightIndex >= 0)
		return lights[lightIndex];
	else
		return NULL;
}

// ��������� ���������� ��������� �����
CLight* C3DS::GetNextLight()
{
	if (lightIndex >= 0)
		return lights[(++lightIndex)%lights.size()];
	else
		return NULL;
}

// �������� ����� 3ds
bool C3DS::Load(const char *FileName, Shader* shader)
{
	bool _retBool = false;
	UseDestructors=false;
	cameraIndex = 0;
	lightIndex = -1;

	WriteLogF("Loading scene \"%s\"...", FileName);

	// ���� ����������
	if (Load3DSFile(FileName, this)!=NULL)
	{
		this->shader = shader;
		if (cameras.size() == 0)
		{
			cameras.push_back(new CCamera());
			WriteLogF("Cameras not found. Created default camera.");
		}

		if (lights.size() > 0)
			lightIndex = 0;

		if (objects.size() > 0 && objects.back()->VertexCount == 0)
			objects.pop_back();

		for (uint _objNum = 0; _objNum < objects.size(); _objNum++)
		{
			C3DSObject * currentObject = objects[_objNum];
			// �������������� ������ ���������
			//for(int i=0;i<currentObject->VertexCount;++i)
			//{
			//		//���� ������� �������� ��� �����, �� ���� �� ������
			//		//-offset (�� �� offset),
			//		// x
			//		currentObject->VertexList[3*i]-=currentObject->LocalMatrix[9];
			//		// y
			//		currentObject->VertexList[3*i+1]-=currentObject->LocalMatrix[11];
			//		// z
			//		currentObject->VertexList[3*i+2]-=currentObject->LocalMatrix[10];
			//		//� ����� ��������� ������� �������� rotmatr
			//		//������� �������� ���������
			//		//� ��� y � z ���� ����� �������� �������!
			//		GLfloat x=-currentObject->VertexList[3*i];
			//		GLfloat y=-currentObject->VertexList[3*i+1];
			//		GLfloat z=-currentObject->VertexList[3*i+2];
			//		currentObject->VertexList[3*i]=currentObject->LocalMatrix[0]*x+currentObject->LocalMatrix[2]*z+currentObject->LocalMatrix[1]*y;
			//		currentObject->VertexList[3*i+2]=currentObject->LocalMatrix[3]*x+currentObject->LocalMatrix[5]*z+currentObject->LocalMatrix[4]*y;
			//		currentObject->VertexList[3*i+1]=currentObject->LocalMatrix[6]*x+currentObject->LocalMatrix[8]*z+currentObject->LocalMatrix[7]*y;
			//}

			GLfloat _min[3], _max[3];
			for (int i = 0; i < 3; ++i)
			{
				_min[0] = 1E+37;
				_max[0] = -1E+37;
			}

			// ������������ ������
			GLfloat *VertexListCopy=new GLfloat[currentObject->IndexCount*4*2];//+�������+���������� ����������
			for(unsigned int i=0;i<currentObject->IndexCount;++i)
			{
				for(int k=0;k<3;++k)	
				{
					VertexListCopy[3*i+k]=currentObject->VertexList[3*currentObject->IndexList[i]+k];
					if (_min[k] > VertexListCopy[3*i+k])
						_min[k] = VertexListCopy[3*i+k];
					else if (_max[k] < VertexListCopy[3*i+k])
						_max[k] = VertexListCopy[3*i+k];
				}
				if (currentObject->TexVertList != NULL)
				{
					for(int k=0;k<2;++k)	
						// ���������� ���������� ���������
						VertexListCopy[currentObject->IndexCount*3*2+2*i+k]=currentObject->TexVertList[2*currentObject->IndexList[i]+k];
				}
			}

			// ���������� ��������
			for(unsigned int i=0;i<currentObject->IndexCount/3;++i)
			{
				//A2A1
				vec3 a1=vec3(VertexListCopy[3*(3*i+2)]-VertexListCopy[3*(3*i+1)], VertexListCopy[3*(3*i+2)+1]-VertexListCopy[3*(3*i+1)+1], VertexListCopy[3*(3*i+2)+2]-VertexListCopy[3*(3*i+1)+2]);
				//A0A1
				vec3 a2=vec3(VertexListCopy[3*3*i]-VertexListCopy[3*(3*i+1)], VertexListCopy[3*3*i+1]-VertexListCopy[3*(3*i+1)+1], VertexListCopy[3*3*i+2]-VertexListCopy[3*(3*i+1)+2]);
				vec3 normal=normalize(-cross(a1, a2));
				//������������� ������� ��� ������ �� 3 ������
				for(int j=0;j<3;++j)
					for(int k=0;k<3;++k)
						VertexListCopy[currentObject->IndexCount*3+i*9+j*3+k]=normal[k];
			}
			// VBO
			// �������� ��������� �������������
			glGenBuffersARB(1, &currentObject->Buffer);

			// �������� ������ ��� �������
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, currentObject->Buffer);
			//glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, Buffers[1]);

			// ���������� ������� VBO
			glBufferDataARB(GL_ARRAY_BUFFER_ARB,sizeof(GLfloat)*6*currentObject->IndexCount+sizeof(GLfloat)*2*currentObject->IndexCount,VertexListCopy,GL_STATIC_DRAW_ARB);
			//glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,sizeof(unsigned short)*IndexCount,IndexList,GL_STATIC_DRAW_ARB);

			// ������������� ������� ������ VBO = 0
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

			//printf("  Loading successful. Using buffers %u and %u\n", Buffers[0], Buffers[1]);
			currentObject->occluder = new COccluder(_min, _max);
			WriteLogF("  Object '%s': loading successful. Using buffer %u", currentObject->name.c_str(), currentObject->Buffer);
			delete[] VertexListCopy;
		}
		UseDestructors=true;
		_retBool = true;
	}
	else
	{
		WriteLogF("  Error: File not found.\n");
		_retBool = false;
	}
	return _retBool;
}

// ����� �� �����
void C3DS::Render(void)
{
	uint N = objects.size();
	GLuint* queries = new GLuint[N];

	glGenQueriesARB(N, queries);
	for (uint i = 0; i < objects.size(); i++)
	{
		C3DSObject* _obj = objects[i];
		glBeginQueryARB(GL_SAMPLES_PASSED_ARB, queries[i]);
		if (_obj->wasDrawn)
		{
			_obj->Render();
		}
		else
		{
			_obj->occluder->Render();
		}
		glEndQueryARB(GL_SAMPLES_PASSED_ARB);
	}
	delete[] queries;
}

C3DS::C3DS()
{
	GLushort _indicies[24] = {0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 5, 4, 3, 2, 6, 7, 1, 5, 6, 2, 0, 4, 7, 3};
	glGenBuffersARB(1, &occluderBufferVBOIndex);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, occluderBufferVBOIndex);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(GLushort)*24,_indicies,GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
}

C3DS::~C3DS(void)
{
	glDeleteBuffersARB(1, &occluderBufferVBOIndex);

	for (uint i = 0; i < objects.size(); i++)
		delete objects[i];
	for (uint i = 0; i < cameras.size(); i++)
		delete cameras[i];
	for (uint i = 0; i < lights.size(); i++)
		delete lights[i];
	for (uint i = 0; i < materials.size(); i++)
		delete materials[i];

	objects.clear();
	cameras.clear();
	lights.clear();
	materials.clear();
}
