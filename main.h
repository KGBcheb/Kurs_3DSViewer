#pragma once
#include <windows.h>
#include "glee.h"
#include "glm/glm.hpp"
#include "glut.h"
#include "texture.h" 

#include <string>
#include <vector>
#include <algorithm>
#include "macro.h"

using namespace std;

// ������
typedef vector<GLfloat> vect;

const char TEXTURE_FILTER_NEAREST = 0;
const char TEXTURE_FILTER_LINEAR = 1;
const char TEXTURE_FILTER_MIPMAP_LINEAR = 2;
const char TEXTURE_FILTER_ANISOTROPY = 3;

const int MAX_CONFIG_LINE_LENGTH=500;

extern bool UseDestructors;
extern bool CullFaceFlag;

extern string ProgPath;
extern string ConfigFile;
extern string LogFile;

extern int windowWidth, windowHeight;

// �������
extern void split(const string& str, const string& delim, vector<string>& parts);
extern string remove_spaces(const string& source);
extern string StrToLower(string s);
extern inline string StrReplace(string text, string s, string d);
extern void WriteLog(char *text);
extern bool FileExists (const char *fname);
extern string getNowToString();

extern void ChangeTextureFilter();
extern void LoadObjects();
extern void RenderObjects();
extern void DrawPlane(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b);
extern void PreCalcObjects();

extern bool LoadTGA(Texture * texture, char * filename);
