#include "3DSLoader.h"

// ��������� ID �����
unsigned short GetChunkId(ifstream &ModelF)
{
	unsigned short chunk_id;
	int gp = ModelF.tellg();
	ModelF.read((char *)&chunk_id,sizeof(chunk_id));
	ModelF.seekg(gp, ios::beg);
	return chunk_id;
}

// ������ �����
void ReadChunk(ifstream &ModelF, vector<C3DSObject*> &objects, vector<CCamera*> &cameras, vector<CLight*> &lights)
{
	GLfloat _tempFloat;
	GLbyte _tempByte;
	GLfloat _posX, _posY, _posZ, _tX, _tY, _tZ;

	C3DSObject* currentObject = NULL;
	if (objects.size() > 0)
		currentObject = objects.back();

	// �������������, ���������� ��� ����� (2 �����)
	unsigned short chunk_id;

	// ����� ����� � ������ (4 �����)
	unsigned int chunk_len;

	int gp;
	// ������ ��������� �����
	ModelF.read((char *)&chunk_id,sizeof(chunk_id));
	ModelF.read((char *)&chunk_len,sizeof(chunk_len));
	WriteLogF("  Chunk ID = 0x%hX Length = %u",chunk_id,chunk_len);
	switch (chunk_id)
	{
	// �����, ������� �� ����������
	case 0x0010: //RGB_COLOR
		// �������� �����
		ModelF.read((char *)&_tempFloat, sizeof(GLfloat));
		color.r = _tempFloat;
		ModelF.read((char *)&_tempFloat, sizeof(GLfloat));
		color.g = _tempFloat;
		ModelF.read((char *)&_tempFloat, sizeof(GLfloat));
		color.b = _tempFloat;
		isColor24Bit = false;
		WriteLogF("   Loaded color: (%f; %f; %f)", color.r, color.g, color.b);
		break;
	case 0x0011: //RGB_COLOR_24BIT
		// �������� �����
		ModelF.read((char *)&_tempByte, sizeof(GLbyte));
		colorByte.r = _tempByte;
		ModelF.read((char *)&_tempByte, sizeof(GLbyte));
		colorByte.g = _tempByte;
		ModelF.read((char *)&_tempByte, sizeof(GLbyte));
		colorByte.b = _tempByte;
		isColor24Bit = true;
		//WriteLogF(" Loaded color: (%f; %f; %f)", color.r, color.g, color.b);
		break;
	case 0x4D4D: //MAIN3DS
	case 0x3D3D: //EDIT3DS
		break;
	case 0x4000: //EDIT_OBJECT
		meshFirst = false;
		gp = ModelF.tellg();
		// ������ ��� �������
		ModelF.read(objectName, 255);
		// ���� ��������� �������, ������������
		ModelF.seekg(gp+strlen(objectName)+1, ios::beg);
		break;
	case 0x4100: //OBJ_TRIMESH
		meshFirst=!meshFirst;
		if (!meshFirst)
		{
			// ���������� ����, ������ ������� �����
			ModelF.ignore(chunk_len-6);
			WriteLogF("   IGNORING, not first OBJ_TRIMESH");
		}
		else
		{
			// ���������� �������� ������ �������
			currentObject = new C3DSObject();
			strcpy_s(currentObject->name, objectName);
			objects.push_back(currentObject);
		}
		break;
	case 0x4110: //POINT_ARRAY
		// ������ ������ ������
		ModelF.read((char *)&(currentObject->VertexCount),sizeof(unsigned short));
		currentObject->VertexList = new GLfloat[currentObject->VertexCount*6];
		//ModelF.read((char *)currentObject->VertexList,currentObject->VertexCount*sizeof(GLfloat)*3);
		for (int i=0;i<currentObject->VertexCount;++i)
		{
			ModelF.read((char *)&(currentObject->VertexList[3*i]),sizeof(GLfloat));
			ModelF.read((char *)&(currentObject->VertexList[3*i+2]),sizeof(GLfloat));
			ModelF.read((char *)&(currentObject->VertexList[3*i+1]),sizeof(GLfloat));
			currentObject->VertexList[3*i+2] = -currentObject->VertexList[3*i+2];
		}
		// ��������� � ������ ����!
		for (int i=currentObject->VertexCount; i< currentObject->VertexCount*2; ++i)
		{
			currentObject->VertexList[3*i]=currentObject->color.r;
			currentObject->VertexList[3*i+1]=currentObject->color.g;
			currentObject->VertexList[3*i+2]=currentObject->color.b;
		}
		WriteLogF(" VertexCount = %hu",currentObject->VertexCount);
		break;
	case 0x4120: //FACE_ARRAY
		// ������ ������ ��������
		ModelF.read((char *)&(currentObject->IndexCount),sizeof(unsigned short));
		currentObject->IndexList = new unsigned short[currentObject->IndexCount*3];
		for (int i=0;i<currentObject->IndexCount;++i)
		{
			ModelF.read((char *)&(currentObject->IndexList[3*i]),sizeof(unsigned short));
			ModelF.read((char *)&(currentObject->IndexList[3*i+1]),sizeof(unsigned short));
			ModelF.read((char *)&(currentObject->IndexList[3*i+2]),sizeof(unsigned short));
			// ���������� �������� ��������
			ModelF.ignore(sizeof(unsigned short));
		}
		currentObject->IndexCount*=3;
		WriteLogF(" IndexCount = %hu",currentObject->IndexCount);
		break;
	case 0x4140: //TEX_VERTS
		// ������ ������ �����. ���������
		ModelF.read((char *)&(currentObject->TexVertCount),2);
		currentObject->TexVertList = new GLfloat[currentObject->TexVertCount*2];
		ModelF.read((char *)currentObject->TexVertList,currentObject->TexVertCount*sizeof(GLfloat)*2);
		WriteLogF(" TextureVertexCount = %hu",currentObject->TexVertCount);
		break;
	case 0x4160: //MESH_MATRIX
		// ���������� �������
		ModelF.read((char *)currentObject->LocalMatrix,sizeof(currentObject->LocalMatrix));
		break;			
	case 0x4600: //OBJ_LIGHT
		// �������� ��������� �����
		ModelF.read((char *)&_posX, sizeof(GLfloat));
		ModelF.read((char *)&_posZ, sizeof(GLfloat));
		ModelF.read((char *)&_posY, sizeof(GLfloat));
		lights.push_back(new CLight(isLightEnabled, vec3(_posX, _posY, -_posZ)));
		WriteLogF(" Loaded light: (%f; %f; %f)", _posX, _posY, -_posZ);
		unsigned short temp_chunk_id;
		temp_chunk_id = GetChunkId(ModelF);
		color = vec3(1.0f, 1.0f, 1.0f);
		isColor24Bit = false;
		while (temp_chunk_id == 0x0010 || temp_chunk_id == 0x0011 || (temp_chunk_id >= 0x4600 && temp_chunk_id < 0x4700))
		{
			ReadChunk(ModelF, objects, cameras, lights);
			temp_chunk_id = GetChunkId(ModelF);
		}
		if (isColor24Bit)
		{
			lights.back()->SetColor(colorByte);
		}
		else
		{
			lights.back()->SetColor(color);
		}
		break;
	case 0x4610: //LIGHT_SPOTLIGHT
		// �������� ������������� ��������� �����
		ModelF.read((char *)&_posX, sizeof(GLfloat));
		ModelF.read((char *)&_posZ, sizeof(GLfloat));
		ModelF.read((char *)&_posY, sizeof(GLfloat));
		ModelF.ignore(sizeof(GLfloat)*2);
		lights.back()->SetTarget(vec3(_posX, _posY, -_posZ));
		WriteLogF("   Spotlight target: (%f; %f; %f)", _posX, _posY, -_posZ);
		break;
	case 0x4620: //LIGHT_ENABLED
		ModelF.read((char *)&isLightEnabled, sizeof(GLboolean));
		break;
	case 0x4700: //OBJ_CAMERA
		// �������� ����� ������
		// ������ ��������� ������
		ModelF.read((char *)&_posX, sizeof(GLfloat));
		ModelF.read((char *)&_posZ, sizeof(GLfloat));
		ModelF.read((char *)&_posY, sizeof(GLfloat));
		ModelF.read((char *)&_tX,	 sizeof(GLfloat));
		ModelF.read((char *)&_tZ,	 sizeof(GLfloat));
		ModelF.read((char *)&_tY,	 sizeof(GLfloat));
		ModelF.ignore(sizeof(GLfloat)*2);
		// �������� ������
		cameras.push_back(new CCamera(_posX, _posY, -_posZ, _tX, _tY, -_tZ));
		// ������� ��������
		//ModelF.ignore(chunk_len - 6 - 32);
		WriteLogF(" Loaded camera: (%f; %f; %f) -> (%f; %f; %f)", _posX, _posY, -_posZ, _tX, _tY, -_tZ);
		break;
	default:
		// ���������� ����
		ModelF.ignore(chunk_len-6);
		WriteLogF("   IGNORING");
		break;
	}
}

// �������� ����� 3ds
bool Load3DSFile(const char *FileName, vector<C3DSObject*> &objects, vector<CCamera*> &cameras, vector<CLight*> &lights)
{
	// ���� ������
	ifstream ModelF(FileName, ios::in|ios::binary);

		// ���� ����������
	if (ModelF!=NULL)
	{
		ModelF.seekg(0);
		while(ModelF.good())
		{
			ReadChunk(ModelF, objects, cameras, lights);
		}
		ModelF.close();
		return true;
	}
	return false;
}
