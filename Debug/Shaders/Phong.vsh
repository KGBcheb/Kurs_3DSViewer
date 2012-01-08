//������� ���������
varying vec3 position;

//�������
varying vec3 normal;

void main (void)
{
	//������� �����
	position=vec3(gl_ModelViewMatrix*gl_Vertex);

	//�������
	normal=vec3(gl_NormalMatrix*gl_Normal);
	
	// ���������� ����������
	gl_TexCoord[0]=gl_MultiTexCoord0;
	
	//��������� � ������������ ����������
	gl_Position=ftransform();
}
