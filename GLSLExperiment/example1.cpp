#include "Angel.h"
typedef vec4 point4;
typedef vec4 color4;
using namespace std;

GLuint program;

GLuint model_loc;
GLuint projection_loc;
GLuint view_loc;
mat4 projection;
mat4 view;

color4 _ambient;
color4 _diffuse;
color4 _specular;

const float PI = 3.141592654;
const int dinhHinhThang = 36;
const int dinhNon = 24;
const int dinhLapPhuong = 36;
const int dinhTru = 228;
const int dinhMatCong = 300;
const int dinhBanNguyet = 120;

// diem bat dau de ve hinh
const int numHinhThang = dinhLapPhuong;
const int numTru = numHinhThang + dinhHinhThang;
const int numBanNguyet = numTru + dinhTru;
const int numNon = numBanNguyet + dinhBanNguyet;
const int numMatCong = numNon + dinhNon;

float x = 0;

point4 dinhCacHinh[dinhHinhThang + dinhNon + dinhLapPhuong + dinhTru + dinhMatCong + dinhBanNguyet];
color4 colors[dinhHinhThang + dinhNon + dinhLapPhuong + dinhTru + dinhMatCong + dinhBanNguyet];
vec3 normals[dinhHinhThang + dinhNon + dinhLapPhuong + dinhTru + dinhMatCong + dinhBanNguyet];

point4 dinhLapPhuongAvailable[8];
point4 dinhHinhThangAvailable[8];
point4 dinhNonAvailable[8];
point4 dinhTruAvailable[40];
point4 dinhMatCongAvailable[60];

point4 light_position(0.0, 1.87, 0, 1.0);
color4 light_ambient(0.4, 0.4, 0.4, 1.0);
color4 light_diffuse(252. / 255, 251. / 255, 232. / 255, 1.0);
color4 light_specular(1.0, 1.0, 1.0, 1.0);

color4 material_ambient;
color4 material_diffuse;
color4 material_specular;
float material_shininess = 1000.0;

color4 ambient_product;
color4 diffuse_product;
color4 specular_product;

float cam_Rotation[3] = { 90, 0, -180 };
vec4 eye(0, 1.5, 15, 1);
vec4 at(0, 0, 0, 1);
vec4 up(0, 1, 0, 1);
bool slow = false;

void initCube()
{
	dinhLapPhuongAvailable[0] = point4(-0.5, -0.5, 0.5, 1.0);
	dinhLapPhuongAvailable[1] = point4(-0.5, 0.5, 0.5, 1.0);
	dinhLapPhuongAvailable[2] = point4(0.5, 0.5, 0.5, 1.0);
	dinhLapPhuongAvailable[3] = point4(0.5, -0.5, 0.5, 1.0);
	dinhLapPhuongAvailable[4] = point4(-0.5, -0.5, -0.5, 1.0);
	dinhLapPhuongAvailable[5] = point4(-0.5, 0.5, -0.5, 1.0);
	dinhLapPhuongAvailable[6] = point4(0.5, 0.5, -0.5, 1.0);
	dinhLapPhuongAvailable[7] = point4(0.5, -0.5, -0.5, 1.0);
}
int Index = 0;

void quad(int a, int b, int c, int d)
{
	vec4 u = dinhLapPhuongAvailable[b] - dinhLapPhuongAvailable[a];
	vec4 v = dinhLapPhuongAvailable[c] - dinhLapPhuongAvailable[b];
	vec3 normal = normalize(cross(u, v));
	normals[Index] = normal; dinhCacHinh[Index] = dinhLapPhuongAvailable[a]; Index++;
	normals[Index] = normal; dinhCacHinh[Index] = dinhLapPhuongAvailable[b]; Index++;
	normals[Index] = normal; dinhCacHinh[Index] = dinhLapPhuongAvailable[c]; Index++;
	normals[Index] = normal; dinhCacHinh[Index] = dinhLapPhuongAvailable[a]; Index++;
	normals[Index] = normal; dinhCacHinh[Index] = dinhLapPhuongAvailable[c]; Index++;
	normals[Index] = normal; dinhCacHinh[Index] = dinhLapPhuongAvailable[d]; Index++;
}
void makeColorCube(void)

{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}
void generateGeometry(void)
{
	initCube();
	makeColorCube();
}

// Hinh thanh can
void initCanh()
{
	dinhHinhThangAvailable[0] = point4(-0.25, -0.5, 0.5, 1.0);
	dinhHinhThangAvailable[1] = point4(-0.5, 0.5, 0.5, 1.0);
	dinhHinhThangAvailable[2] = point4(0.5, 0.5, 0.5, 1.0);
	dinhHinhThangAvailable[3] = point4(0.25, -0.5, 0.5, 1.0);
	dinhHinhThangAvailable[4] = point4(-0.25, -0.5, -0.5, 1.0);
	dinhHinhThangAvailable[5] = point4(-0.5, 0.5, -0.5, 1.0);
	dinhHinhThangAvailable[6] = point4(0.5, 0.5, -0.5, 1.0);
	dinhHinhThangAvailable[7] = point4(0.25, -0.5, -0.5, 1.0);
}

void quadCanh(int a, int b, int c, int d)
{
	vec4 u = dinhHinhThangAvailable[b] - dinhHinhThangAvailable[a];
	vec4 v = dinhHinhThangAvailable[c] - dinhHinhThangAvailable[b];
	vec3 normal = normalize(cross(u, v));
	normals[Index] = normal; dinhCacHinh[Index++] = dinhHinhThangAvailable[a];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhHinhThangAvailable[b];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhHinhThangAvailable[c];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhHinhThangAvailable[a];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhHinhThangAvailable[c];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhHinhThangAvailable[d];
}
void makeColorCanh(void)

{
	quadCanh(1, 0, 3, 2);
	quadCanh(2, 3, 7, 6);
	quadCanh(3, 0, 4, 7);
	quadCanh(6, 5, 1, 2);
	quadCanh(4, 5, 6, 7);
	quadCanh(5, 4, 0, 1);
}
void generateCanh(void)
{
	initCanh();
	makeColorCanh();
}

//
void initTru()
{
	int j = 0;
	for (int i = 0; i < 360; i += 18)
	{
		float x = 0.5 * cos(PI / 180 * i);
		float y = 0.5 * sin(PI / 180 * i);
		dinhTruAvailable[j] = point4(x, y, -0.5, 1); ++j;
		dinhTruAvailable[j] = point4(x, y, +0.5, 1); ++j;
	}
}

void generateHinhTru(void)
{
	initTru();
	for (int i = 0; i < 37; i += 2)
	{
		vec4 u = dinhTruAvailable[i + 2] - dinhTruAvailable[i];
		vec4 v = dinhTruAvailable[i + 1] - dinhTruAvailable[i + 2];
		vec3 normal = normalize(cross(u, v));
		normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[i];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[i + 2];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[i + 1];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[i + 1];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[i + 2];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[i + 3];
	}
	vec4 u = dinhTruAvailable[0] - dinhTruAvailable[38];
	vec4 v = dinhTruAvailable[39] - dinhTruAvailable[0];
	vec3 normal = normalize(cross(u, v));
	normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[39];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[0];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[38];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[1];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[0];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[39];
	for (int i = 0; i < 35; i += 2)
	{
		u = dinhTruAvailable[0] - dinhTruAvailable[i + 2];
		v = dinhTruAvailable[i + 4] - dinhTruAvailable[i + 2];
		normal = normalize(cross(u, v));
		normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[0];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[i + 2];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[i + 4];
	}
	for (int i = 1; i < 36; i += 2)
	{
		u = dinhTruAvailable[i + 2] - dinhTruAvailable[1];
		v = dinhTruAvailable[i + 4] - dinhTruAvailable[i + 2];
		normal = normalize(cross(u, v));
		normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[1];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[i + 2];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[i + 4];
	}
}

//
void generateBanNguyet(void)
{
	initTru();
	for (int i = 0; i < 19; i += 2)
	{
		vec4 u = dinhTruAvailable[i + 2] - dinhTruAvailable[i];
		vec4 v = dinhTruAvailable[i + 1] - dinhTruAvailable[i + 2];
		vec3 normal = normalize(cross(u, v));
		normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[i];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[i + 2];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[i + 1];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[i + 1];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[i + 2];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[i + 3];
	}
	vec4 u = dinhTruAvailable[0] - dinhTruAvailable[20];
	vec4 v = dinhTruAvailable[21] - dinhTruAvailable[0];
	vec3 normal = normalize(cross(u, v));
	normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[21];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[0];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[20];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[21];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[0];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[1];
	for (int i = 0; i < 17; i += 2)
	{
		u = dinhTruAvailable[0] - dinhTruAvailable[i + 2];
		v = dinhTruAvailable[i + 4] - dinhTruAvailable[i + 2];
		normal = normalize(cross(u, v));
		normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[0];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[i + 2];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[i + 4];
	}
	for (int i = 1; i < 18; i += 2)
	{
		u = dinhTruAvailable[i + 2] - dinhTruAvailable[1];
		v = dinhTruAvailable[i + 4] - dinhTruAvailable[i + 2];
		normal = normalize(cross(u, v));
		normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[1];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[i + 2];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhTruAvailable[i + 4];
	}
}

//
void initNon()
{
	int j = 0;
	for (int i = 0; i < 360; i += 45)
	{
		float x = 0.5 * cos(PI / 180 * i);
		float y = 0.5 * sin(PI / 180 * i);
		dinhNonAvailable[j++] = point4(x, y, -0.5, 1);
	}
}

void generateNon(void)
{
	initNon();

	for (int i = 0; i < 7; ++i)
	{
		vec4 u = dinhNonAvailable[i] - point4(0, 0, 0.5, 1);
		vec4 v = dinhNonAvailable[i + 1] - dinhNonAvailable[i];
		vec3 normal = normalize(cross(u, v));
		normals[Index] = normal; dinhCacHinh[Index++] = point4(0, 0, 0.5, 1);
		normals[Index] = normal; dinhCacHinh[Index++] = dinhNonAvailable[i];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhNonAvailable[i + 1];
	}

	vec4 u = dinhNonAvailable[7] - point4(0, 0, 0.5, 1);
	vec4 v = dinhNonAvailable[0] - dinhNonAvailable[7];
	vec3 normal = normalize(cross(u, v));
	normals[Index] = normal; dinhCacHinh[Index++] = point4(0, 0, 0.5, 1);
	normals[Index] = normal; dinhCacHinh[Index++] = dinhNonAvailable[0];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhNonAvailable[7];
}

//
void initMatCong()
{
	int j = 0;
	for (int i = 0; i < 360; i += 18)
	{
		float x = 0.5 * cos(PI / 180 * i);
		float y = 0.5 * sin(PI / 180 * i);
		dinhMatCongAvailable[j] = point4(x, y, 0, 1); ++j;
		x = 0.4 * cos(PI / 180 * i);
		y = 0.4 * sin(PI / 180 * i);
		dinhMatCongAvailable[j] = point4(x, y, 0.3, 1); ++j;
		x = 0.2 * cos(PI / 180 * i);
		y = 0.2 * sin(PI / 180 * i);
		dinhMatCongAvailable[j] = point4(x, y, 0.46, 1); ++j;
	}
}

void generateMatCong(void)
{
	initMatCong();
	for (int i = 0; i < 57; i += 3)
	{
		vec4 u = dinhMatCongAvailable[i] - dinhMatCongAvailable[i + 1];
		vec4 v = dinhMatCongAvailable[i + 3] - dinhMatCongAvailable[i];
		vec3 normal = normalize(cross(u, v));
		normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[i + 1];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[i];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[i + 3];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[i + 1];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[i + 3];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[i + 4];
		u = dinhMatCongAvailable[i + 1] - dinhMatCongAvailable[i + 2];
		v = dinhMatCongAvailable[i + 4] - dinhMatCongAvailable[i + 1];
		normal = normalize(cross(u, v));
		normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[i + 2];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[i + 1];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[i + 4];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[i + 2];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[i + 4];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[i + 5];
		u = dinhMatCongAvailable[i + 2] - point4(0, 0, 0.5, 1);
		v = dinhMatCongAvailable[i + 5] - dinhMatCongAvailable[i + 2];
		normal = normalize(cross(u, v));
		normals[Index] = normal; dinhCacHinh[Index++] = point4(0, 0, 0.5, 1);
		normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[i + 2];
		normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[i + 5];
	}

	vec4 u = dinhMatCongAvailable[57] - dinhMatCongAvailable[58];
	vec4 v = dinhMatCongAvailable[0] - dinhMatCongAvailable[57];
	vec3 normal = normalize(cross(u, v));
	normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[58];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[57];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[0];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[58];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[0];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[1];
	u = dinhMatCongAvailable[58] - dinhMatCongAvailable[59];
	v = dinhMatCongAvailable[1] - dinhMatCongAvailable[58];
	normal = normalize(cross(u, v));
	normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[59];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[58];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[1];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[59];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[1];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[2];
	u = dinhMatCongAvailable[59] - point4(0, 0, 0.5, 1);
	v = dinhMatCongAvailable[2] - dinhMatCongAvailable[59];
	normal = normalize(cross(u, v));
	normals[Index] = normal; dinhCacHinh[Index++] = point4(0, 0, 0.5, 1);
	normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[59];
	normals[Index] = normal; dinhCacHinh[Index++] = dinhMatCongAvailable[2];
}

void GenerateAllShape(void)
{
	generateGeometry();
	generateCanh();
	generateHinhTru();
	generateBanNguyet();
	generateNon();
	generateMatCong();
}


color4 RGBtoColor(int _R, int _G, int _B, float _A)
{
	return color4(_R * 1. / 255, _G * 1. / 255, _B * 1. / 255, _A);
}

void shuffColor()
{
	ambient_product = light_ambient * material_ambient; // Mau vat lieu
	diffuse_product = light_diffuse * material_diffuse; // Mau nhin thay
	specular_product = light_specular * material_specular; // Mau bong 
	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
}
void createMaterial(color4 ma, color4 md, color4 ms, float sd)
{
	material_ambient = ma;
	material_specular = ms;
	material_diffuse = md;
	material_shininess = sd;
	shuffColor();
}

// Ham tao hinh
void taoHinhThang(mat4 model)
{
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model);
	glDrawArrays(GL_TRIANGLES, 36, dinhHinhThang);
}

void taoBanNguyet(mat4 model)
{
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model);
	glDrawArrays(GL_TRIANGLES, numBanNguyet, dinhBanNguyet);
}

void taoLapPhuong(mat4 model)
{
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model);
	glDrawArrays(GL_TRIANGLES, 0, dinhLapPhuong);
}

void taoHinhTru(mat4 model)
{
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model);
	glDrawArrays(GL_TRIANGLES, numTru, dinhTru);
}

void taoMatCong(mat4 model)
{
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model);
	glDrawArrays(GL_TRIANGLES, numMatCong, dinhMatCong);
}

void taoHinhNon(mat4 model)
{
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model);
	glDrawArrays(GL_TRIANGLES, numNon, dinhNon);
}

void initGPUBuffers(void)
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(dinhCacHinh) + sizeof(colors) + sizeof(normals), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(dinhCacHinh), dinhCacHinh);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(dinhCacHinh), sizeof(colors), colors);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(dinhCacHinh) + sizeof(colors), sizeof(normals), normals);
}

void shaderSetup(void)
{
	program = InitShader("vshader1.glsl", "fshader1.glsl");
	glUseProgram(program);
	GLuint loc_vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc_vPosition);
	glVertexAttribPointer(loc_vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	GLuint loc_vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc_vColor);
	glVertexAttribPointer(loc_vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(dinhCacHinh)));
	GLuint loc_vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(loc_vNormal);
	glVertexAttribPointer(loc_vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(dinhCacHinh) + sizeof(colors)));
	model_loc = glGetUniformLocation(program, "Model");
	projection_loc = glGetUniformLocation(program, "Projection");
	view_loc = glGetUniformLocation(program, "View");
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glClearColor(1, 1, 1, 1.0); // mau man hinh
}


/////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////DUCLE/////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////
float xnn=0, ynn=0, znn=0;
// code mau
void GauMuc(mat4 model)
{
	_ambient = RGBtoColor(0, 0, 0, 1); // Mau khi khong co anh sang
	_diffuse = RGBtoColor(100, 100, 100, 1); // Mau nhin thay khi co anh sang
	_specular = RGBtoColor(128, 128, 128, 1); // Mau bong
	createMaterial(_ambient, _diffuse, _specular, 1000);//xamm

	//model = model * RotateX(xnn) * RotateY(ynn) * RotateZ(znn);
	//model = Scale(1, 1, 1);
	taoBanNguyet(model * Translate(0, 0, 1) * Scale(1, 1.5, 0.05));//mat phai
	taoBanNguyet(model *  Scale(1, 1.5, 0.05));//mat trái
	taoLapPhuong(model *  Translate(-0.45, 0.15, 0.5) * RotateZ(-10) *  Scale(0.05, 0.3, 1));//mat sau gau muc
	taoLapPhuong(model * Translate(-0.36, 0.43, 0.5) * RotateZ(-27) * Scale(0.05, 0.3, 1));//mat truoc gau muc
	taoLapPhuong(model * Translate(-0.21, 0.62, 0.5) * RotateZ(-50) * Scale(0.05, 0.25, 1));//mat truoc gau muc
	taoLapPhuong(model * Translate(0, 0.7, 0.5) * Scale(0.3, 0.03, 1));//mat tren gau muc
	taoLapPhuong(model * Translate(0.45, 0.15, 0.5) * RotateZ(10) * Scale(0.05, 0.3, 1));//mat truoc gau muc
	taoLapPhuong(model * Translate(0.36, 0.43, 0.5) * RotateZ(27) * Scale(0.05, 0.3, 1));//mat truoc gau muc
	taoLapPhuong(model * Translate(0.21, 0.62, 0.5) * RotateZ(50) * Scale(0.05, 0.25, 1));//mat truoc gau muc
	taoBanNguyet(model * Translate(0.47, 0.03, 0.1) * RotateX(180) * RotateY(90) * Scale(0.1, 0.5, 0.05));//rang cua gau muc
	taoBanNguyet(model * Translate(0.47, 0.03, 0.3) * RotateX(180) * RotateY(90) * Scale(0.1, 0.5, 0.05));//rang cua gau muc
	taoBanNguyet(model * Translate(0.47, 0.03, 0.5) * RotateX(180) * RotateY(90) * Scale(0.1, 0.5, 0.05));//rang cua gau muc
	taoBanNguyet(model * Translate(0.47, 0.03, 0.7) * RotateX(180) * RotateY(90) * Scale(0.1, 0.5, 0.05));//rang cua gau muc
	taoBanNguyet(model * Translate(0.47, 0.03, 0.9) * RotateX(180) * RotateY(90) * Scale(0.1, 0.5, 0.05));//rang cua gau muc
	taoHinhTru(model * Translate(-0.6, 0.2, 0.5) * Scale(0.3, 0.3, 0.6));// phan noi
}
void ThanhXuc1(mat4 model)
{
	_ambient = RGBtoColor(0, 0, 0, 1); // Mau khi khong co anh sang
	_diffuse = RGBtoColor(255, 85, 0, 1); // Mau nhin thay khi co anh sang
	_specular = RGBtoColor(128, 128, 128, 1); // Mau bong
	createMaterial(_ambient, _diffuse, _specular, 1000);//cammm

	//model = model * RotateX(xnn) * RotateY(ynn) * RotateZ(znn);
	taoLapPhuong(model * Scale(2, 0.2, 0.3));//thanh muc 1 duoi
	taoLapPhuong(model * Translate(0,0.1,0) * RotateZ(-3) * Scale(2, 0.2, 0.3));//thanh muc 1 tren
	taoLapPhuong(model * Translate(1.1, 0, 0) * Scale(0.2, 0.1, 0.2));//thanh noi truoc
	taoHinhTru(model * Translate(1.2, 0, 0) * Scale(0.2, 0.2, 0.62));// phan noi thanh muc 1
	taoHinhTru(model * Translate(-1, 0.1, 0) * Scale(0.3, 0.3, 0.3));//phan duoi sau
	_ambient = RGBtoColor(0, 0, 0, 1); // Mau khi khong co anh sang
	_diffuse = RGBtoColor(255, 170, 25, 1); // Mau nhin thay khi co anh sang
	_specular = RGBtoColor(128, 128, 128, 1); // Mau bong
	createMaterial(_ambient, _diffuse, _specular, 1000);//vanggg
	taoHinhTru(model * Translate(-1, 0.1, 0) * Scale(0.2, 0.2, 0.4));// phan noi thanh muc 1
}
void ThanhXuc2(mat4 model)
{
	_ambient = RGBtoColor(0, 0, 0, 1); // Mau khi khong co anh sang
	_diffuse = RGBtoColor(255, 85, 0, 1); // Mau nhin thay khi co anh sang
	_specular = RGBtoColor(128, 128, 128, 1); // Mau bong
	createMaterial(_ambient, _diffuse, _specular, 1000);//cammm

	//model = model * RotateX(xnn) * RotateY(ynn) * RotateZ(znn);
	taoLapPhuong(model * Scale(1.5, 0.2, 0.3));//thanh muc 2 duoi
	taoLapPhuong(model * Translate(0, 0.1, 0) * RotateZ(-3) * Scale(1.5, 0.2, 0.3));//thanh muc 2 tren

	taoLapPhuong(model * Translate(-1.5, -0.41, 0) * RotateZ(35) * Scale(1.7, 0.4, 0.3));//thanh noi truoc
	taoHinhTru(model * Translate(-0.79, 0.1, 0) * Scale(0.6, 0.3, 0.3));// noi giua 2 thanh

	taoHinhTru(model * Translate(0.7, 0.04, 0.14) * Scale(1, 0.25, 0.03));//phan noi trái
	taoHinhTru(model * Translate(0.7, 0.04, -0.14) * Scale(1, 0.25, 0.03));//phan noi phai

	taoLapPhuong(model * Translate(-2, -1, 0) * Scale(0.8, 0.5, 0.4)); //phan de thanh xuc

	taoLapPhuong(model * Translate(-1.7, -0.6, -0.17) * RotateZ(35) * Scale(0.8, 0.1, 0.1)); //thanh nep de 
	taoLapPhuong(model * Translate(-1.7, -0.6, 0.17) * RotateZ(35) * Scale(0.8, 0.1, 0.1)); //thanh nep de phai
	taoHinhTru(model * Translate(-1.38, -0.38, 0) * Scale(0.1, 0.1, 0.5));//nep noi tron
}
void ThanDuoi(mat4 model)
{
	_ambient = RGBtoColor(0, 0, 0, 1); // Mau khi khong co anh sang
	_diffuse = RGBtoColor(100, 100, 100, 1); // Mau nhin thay khi co anh sang
	_specular = RGBtoColor(128, 128, 128, 1); // Mau bong
	createMaterial(_ambient, _diffuse, _specular, 1000);

	//model = model * RotateX(xnn) * RotateY(ynn) * RotateZ(znn);
	taoLapPhuong(model * Scale(1.5, 0.3, 1));//than duoi giua

	taoLapPhuong(model * Translate(1, -0.1, 0) * Scale(1, 0.1, 0.4));//thanh noi thanh-banh truoc
	taoHinhTru(model * Translate(1.5, -0.1, 0) * Scale(0.3, 0.3, 1.2));//phan noi truoc banh-than 

	taoLapPhuong(model * Translate(-1, -0.1, 0) * Scale(1, 0.1, 0.4));//thanh noi thanh-banh sau
	taoHinhTru(model * Translate(-1.5, -0.1, 0) * Scale(0.3, 0.3, 1.2));//phan noi sau banh-than 

	taoLapPhuong(model * Translate(0.85, 0.2, 0.35) * RotateZ(45) * Scale(0.5, 0.03, 0.3));//chan bun truoc thanh 1 phai
	taoLapPhuong(model * Translate(1.27, 0.38, 0.35) * RotateZ(0) * Scale(0.5, 0.03, 0.3));//chan bun truoc thanh 2 phai

	taoLapPhuong(model * Translate(0.85, 0.2, -0.35) * RotateZ(45) * Scale(0.5, 0.03, 0.3));//chan bun truoc thanh 1 trai
	taoLapPhuong(model * Translate(1.27, 0.38, -0.35) * RotateZ(0) * Scale(0.5, 0.03, 0.3));//chan bun truoc thanh 2 trai

	taoLapPhuong(model * Translate(-0.85, 0.2, 0.35) * RotateZ(-45) * Scale(0.5, 0.03, 0.3));//chan bun sau thanh 1 phai
	taoLapPhuong(model * Translate(-1.27, 0.38, 0.35) * RotateZ(0) * Scale(0.5, 0.03, 0.3));//chan bun sau thanh 2 phai

	taoLapPhuong(model * Translate(-0.85, 0.2, -0.35) * RotateZ(-45) * Scale(0.5, 0.03, 0.3));//chan bun sau thanh 1 trai
	taoLapPhuong(model * Translate(-1.27, 0.38, -0.35) * RotateZ(0) * Scale(0.5, 0.03, 0.3));//chan bun sau thanh 2 trai
}

void ThanTren(mat4 model)
{
	//model = model * RotateX(xnn) * RotateY(ynn) * RotateZ(znn);

	_ambient = RGBtoColor(0, 0, 0, 1); // Mau khi khong co anh sang
	_diffuse = RGBtoColor(255, 85, 0, 1); // Mau nhin thay khi co anh sang
	_specular = RGBtoColor(128, 128, 128, 1); // Mau bong
	createMaterial(_ambient, _diffuse, _specular, 1000);//cammm
	taoLapPhuong(model * Scale(2, 0.1, 1.2));//phan duoi than tren
	taoLapPhuong(model * Translate(1.25, 0, 0.35) * Scale(0.5, 0.1, 0.5));//than duoi than tren phai
	taoLapPhuong(model * Translate(1.25, 0, -0.35) * Scale(0.5, 0.1, 0.5));//than duoi than tren trai

	_ambient = RGBtoColor(0, 0, 0, 1); // Mau khi khong co anh sang
	_diffuse = RGBtoColor(100, 100, 100, 1); // Mau nhin thay khi co anh sang
	_specular = RGBtoColor(128, 128, 128, 1); // Mau bong
	createMaterial(_ambient, _diffuse, _specular, 1000);
	taoHinhTru(model * Translate(0, -0.2, 0) * RotateX(90) * Scale(0.8, 0.8, 0.4));//phan noi than tren than duoi
	
	//Phan lai dau lai xe
	_ambient = RGBtoColor(0, 0, 0, 1); // Mau khi khong co anh sang
	_diffuse = RGBtoColor(255, 85, 0, 1); // Mau nhin thay khi co anh sang
	_specular = RGBtoColor(128, 128, 128, 1); // Mau bong
	createMaterial(_ambient, _diffuse, _specular, 1000);//cammm
	taoLapPhuong(model * Translate(1.48, 0.5, 0.35) * Scale(0.03, 1, 0.5));//Dang truoc

	_ambient = RGBtoColor(0, 0, 0, 1); // Mau khi khong co anh sang
	_diffuse = RGBtoColor(255, 230, 230, 1); // Mau nhin thay khi co anh sang
	_specular = RGBtoColor(128, 128, 128, 1); // Mau bong
	createMaterial(_ambient, _diffuse, _specular, 1000);//kinhh
	taoLapPhuong(model * Translate(1.48, 0.5, 0.35) * Scale(0.033, 0.8, 0.4));//kinh truoc
	taoLapPhuong(model * Translate(0.35, 0.5, 0.35) * Scale(0.03, 1, 0.5));//Dang sau

	_ambient = RGBtoColor(0, 0, 0, 1); // Mau khi khong co anh sang
	_diffuse = RGBtoColor(255, 85, 0, 1); // Mau nhin thay khi co anh sang
	_specular = RGBtoColor(128, 128, 128, 1); // Mau bong
	createMaterial(_ambient, _diffuse, _specular, 1000);//cammm
	taoLapPhuong(model * Translate(0.9, 1, 0.35) * Scale(1.15, 0.03, 0.5));//noc xe
	taoBanNguyet(model * Translate(0.9, 1, 0.35) * Scale(1.15, 0.7, 0.5));//noc xe ban nguyet
	taoLapPhuong(model * Translate(0.47, 0.5, 0.59) * Scale(0.3, 1, 0.03));//phan le cua
	taoLapPhuong(model * Translate(0.9, 0.5, 0.1) * Scale(1.15, 1, 0.03));//phan than trai 

	_ambient = RGBtoColor(0, 0, 0, 1); // Mau khi khong co anh sang
	_diffuse = RGBtoColor(255, 230, 230, 1); // Mau nhin thay khi co anh sang
	_specular = RGBtoColor(128, 128, 128, 1); // Mau bong
	createMaterial(_ambient, _diffuse, _specular, 1000);
	taoLapPhuong(model * Translate(0.9, 0.55, 0.1) * Scale(1, 0.8, 0.033));//phan kinh than trai 

	_ambient = RGBtoColor(0, 0, 0, 1); // Mau khi khong co anh sang
	_diffuse = RGBtoColor(255, 85, 0, 1); // Mau nhin thay khi co anh sang
	_specular = RGBtoColor(128, 128, 128, 1); // Mau bong
	createMaterial(_ambient, _diffuse, _specular, 1000);//cammm
	taoLapPhuong(model * Translate(-0.325, 0.25, 0) * Scale(1.35, 0.5, 1.2));// phan hop may moc
	taoBanNguyet(model * Translate(-0.325, 0.5, 0) * Scale(1.35, 0.5, 1.2));//

	/////////////Phan trong xe
	_ambient = RGBtoColor(0, 0, 0, 1); // Mau khi khong co anh sang
	_diffuse = RGBtoColor(102, 51, 0, 1); // Mau nhin thay khi co anh sang
	_specular = RGBtoColor(128, 128, 128, 1); // Mau bong
	createMaterial(_ambient, _diffuse, _specular, 1000);
	taoLapPhuong(model * Translate(0.9, 0.1, 0.3) * Scale(0.4, 0.2, 0.3));
	taoBanNguyet(model * Translate(0.7, 0.2, 0.3) * RotateY(90) * Scale(0.3, 1, 0.05));

	//////////Vo lang 
	_ambient = RGBtoColor(0, 0, 0, 1); // Mau khi khong co anh sang
	_diffuse = RGBtoColor(51, 0, 51, 1); // Mau nhin thay khi co anh sang
	_specular = RGBtoColor(128, 128, 128, 1); // Mau bong
	createMaterial(_ambient, _diffuse, _specular, 1000);
	taoHinhTru(model * Translate(1.3, 0.2, 0.3) * RotateZ(-70) * RotateY(90) * Scale(0.07, 0.07, 0.4));
	taoHinhTru(model * Translate(1.25, 0.4, 0.3) * RotateZ(-70) * RotateY(90) * Scale(0.3, 0.3, 0.03));

	///////Den phai
	_ambient = RGBtoColor(0, 0, 0, 1); // Mau khi khong co anh sang
	_diffuse = RGBtoColor(51, 0, 51, 1); // Mau nhin thay khi co anh sang
	_specular = RGBtoColor(128, 128, 128, 1); // Mau bong
	createMaterial(_ambient, _diffuse, _specular, 1000);//dennn
	taoHinhTru(model * Translate(1.5, 0, 0.35) * RotateZ(0) * RotateY(90) * Scale(0.25, 0.25, 0.05));///vong ngoai den phai
	_ambient = RGBtoColor(0, 0, 0, 1); // Mau khi khong co anh sang
	_diffuse = RGBtoColor(255, 255, 0, 1); // Mau nhin thay khi co anh sang
	_specular = RGBtoColor(128, 128, 128, 1); // Mau bong
	createMaterial(_ambient, _diffuse, _specular, 1000);//vangg
	taoHinhTru(model * Translate(1.5+0.03, 0, 0.35) * RotateZ(0) * RotateY(90) * Scale(0.2, 0.2, 0.05));///vong trong den phai

	///////Den trai
	_ambient = RGBtoColor(0, 0, 0, 1); // Mau khi khong co anh sang
	_diffuse = RGBtoColor(51, 0, 51, 1); // Mau nhin thay khi co anh sang
	_specular = RGBtoColor(128, 128, 128, 1); // Mau bong
	createMaterial(_ambient, _diffuse, _specular, 1000);//denn
	taoHinhTru(model * Translate(1.5, 0, -0.35) * RotateZ(0) * RotateY(90) * Scale(0.25, 0.25, 0.05));///vong ngoai den trai
	_ambient = RGBtoColor(0, 0, 0, 1); // Mau khi khong co anh sang
	_diffuse = RGBtoColor(255, 255, 0, 1); // Mau nhin thay khi co anh sang
	_specular = RGBtoColor(128, 128, 128, 1); // Mau bong
	createMaterial(_ambient, _diffuse, _specular, 1000);//vanggg
	taoHinhTru(model * Translate(1.5 + 0.03, 0, -0.35) * RotateZ(0) * RotateY(90) * Scale(0.2, 0.2, 0.05));///vong trong den trai

	//////Phan ong xa
	///////Den trai
	_ambient = RGBtoColor(0, 0, 0, 1); // Mau khi khong co anh sang
	_diffuse = RGBtoColor(51, 0, 51, 1); // Mau nhin thay khi co anh sang
	_specular = RGBtoColor(128, 128, 128, 1); // Mau bong
	createMaterial(_ambient, _diffuse, _specular, 1000);//denn
	taoHinhTru(model * Translate(-0.5, 0.9, 0.3) * RotateX(90) * Scale(0.2, 0.2, 0.5));///vong ngoai den trai
	taoHinhTru(model* Translate(-0.15-0.5, 1.06, 0.3) * RotateY(90)* Scale(0.2, 0.2, 0.4));///vong ngoai den trai

	/////Bien so
	_ambient = RGBtoColor(0, 0, 0, 1); // Mau khi khong co anh sang
	_diffuse = RGBtoColor(51, 0, 51, 1); // Mau nhin thay khi co anh sang
	_specular = RGBtoColor(128, 128, 128, 1); // Mau bong
	createMaterial(_ambient, _diffuse, _specular, 1000);//denn
	taoLapPhuong(model* Translate(-1, 0.1, 0) * Scale(0.03, 0.5, 0.6));//khung bien
	_ambient = RGBtoColor(0, 0, 0, 1); // Mau khi khong co anh sang
	_diffuse = RGBtoColor(255, 255, 255, 1); // Mau nhin thay khi co anh sang
	_specular = RGBtoColor(128, 128, 128, 1); // Mau bong
	createMaterial(_ambient, _diffuse, _specular, 1000);//denn
	taoLapPhuong(model* Translate(-1, 0.1, 0)* Scale(0.033, 0.4, 0.5));//long bien
	//chu
	_ambient = RGBtoColor(0, 0, 0, 1); // Mau khi khong co anh sang
	_diffuse = RGBtoColor(0, 0, 0, 1); // Mau nhin thay khi co anh sang
	_specular = RGBtoColor(128, 128, 128, 1); // Mau bong
	createMaterial(_ambient, _diffuse, _specular, 1000);//denn
	taoBanNguyet(model* Translate(-1, 0.1, -0.22) * RotateY(90) * RotateZ(90)* Scale(0.17, 0.17, 0.034));
	taoLapPhuong(model* Translate(-1, 0.1, -0.11)* Scale(0.034, 0.15, 0.03));
	taoLapPhuong(model* Translate(-1, 0.1, -0.03)* Scale(0.034, 0.15, 0.03));
	taoLapPhuong(model* Translate(-1, 0.04, -0.06)* Scale(0.034, 0.03, 0.08));
	taoLapPhuong(model* Translate(-1, 0.1, 0.01)* Scale(0.034, 0.15, 0.03));
	taoLapPhuong(model* Translate(-1, 0.16, 0.05)* Scale(0.034, 0.03, 0.05));
	taoLapPhuong(model* Translate(-1, 0.04, 0.05)* Scale(0.034, 0.03, 0.05));
	taoLapPhuong(model* Translate(-1, 0.1, 0.1)* Scale(0.034, 0.15, 0.03));
	taoLapPhuong(model* Translate(-1, 0.04, 0.12)* Scale(0.034, 0.03, 0.05));
	taoLapPhuong(model* Translate(-1, 0.1, 0.17)* Scale(0.034, 0.15, 0.03));
	taoLapPhuong(model* Translate(-1, 0.04, 0.2)* Scale(0.034, 0.03, 0.05));
	taoLapPhuong(model* Translate(-1, 0.1, 0.2)* Scale(0.034, 0.03, 0.05));
	taoLapPhuong(model* Translate(-1, 0.16, 0.2)* Scale(0.034, 0.03, 0.05));
}

void CuaXe(mat4 model)
{
	//phan cua xe
	_ambient = RGBtoColor(0, 0, 0, 1); // Mau khi khong co anh sang
	_diffuse = RGBtoColor(26, 0, 0, 1); // Mau nhin thay khi co anh sang
	_specular = RGBtoColor(128, 128, 128, 1); // Mau bong
	createMaterial(_ambient, _diffuse, _specular, 1000);//cammm
	taoLapPhuong(model * Scale(0.85, 1, 0.03));// canh cua xe
	_ambient = RGBtoColor(0, 0, 0, 1); // Mau khi khong co anh sang
	_diffuse = RGBtoColor(255, 230, 230, 1); // Mau nhin thay khi co anh sang
	_specular = RGBtoColor(128, 128, 128, 1); // Mau bong
	createMaterial(_ambient, _diffuse, _specular, 1000);//kinh
	taoLapPhuong(model * Translate(0, 0.2, 0) * Scale(0.7, 0.5, 0.033));// kinh cua xe tren
	taoLapPhuong(model * Translate(0, -0.27, 0) * Scale(0.7, 0.3, 0.033));// kinh cua xe tren duoi
}

void BanhXe(mat4 model)
{
	_ambient = RGBtoColor(0, 0, 0, 1); // Mau khi khong co anh sang
	_diffuse = RGBtoColor(51, 17, 0, 1); // Mau nhin thay khi co anh sang
	_specular = RGBtoColor(128, 128, 128, 1); // Mau bong
	createMaterial(_ambient, _diffuse, _specular, 1000);//denn

	//model = model * RotateX(xnn) * RotateY(ynn) * RotateZ(znn);
	taoHinhTru(model * Translate(0, 0, 0) * Scale(1, 1, 0.3));
	for (float i = 0; i <= 180; i += 30)
	{
		taoLapPhuong(model * Translate(0, 0, 0.06) * RotateZ(i) * Scale(0.15, 1.05, 0.1));// ngoai
		taoLapPhuong(model * Translate(0, 0, -0.06) * RotateZ(i) * Scale(0.15, 1.05, 0.1));// trong
	}

	taoHinhTru(model * Translate(0, 0, -0.35) * Scale(1, 1, 0.3));
	for (float i = 0; i <= 180; i += 30)
	{
		taoLapPhuong(model * Translate(0, 0, -0.29) * RotateZ(i) * Scale(0.15, 1.05, 0.1));// ngoai
		taoLapPhuong(model * Translate(0, 0, -0.41) * RotateZ(i) * Scale(0.15, 1.05, 0.1));// trong
	}
	
}

void NhaDangXay(mat4 model)
{
	_ambient = RGBtoColor(0, 0, 0, 0); // Mau khi khong co anh sang
	_diffuse = RGBtoColor(222, 111, 111, 0); // Mau nhin thay khi co anh sang
	_specular = RGBtoColor(128, 128, 128, 0); // Mau bong
	createMaterial(_ambient, _diffuse, _specular, 1000);//denn

	model = model * RotateX(xnn) * RotateY(ynn) * RotateZ(znn);
	taoLapPhuong(model * Scale(10, 1, 10));//san
	taoLapPhuong(model * Translate(0, 2, -4.75) * Scale(10, 3, 0.5));
	taoLapPhuong(model * Translate(0, 2, 4.75) * Scale(10, 3, 0.5));
	taoLapPhuong(model * Translate(0, 2, 4.75) * Scale(10, 3, 0.5));
	taoLapPhuong(model * Translate(4.75, 2, 0) * Scale(0.5, 3, 10));
	taoLapPhuong(model * Translate(-4.75, 2, 0) * Scale(0.5, 3, 10));
	taoLapPhuong(model * Translate(2.5, 4, 0) * Scale(5, 8, 0.5));
	taoLapPhuong(model * Translate(-2.5, 3, 0) * Scale(5, 1, 10));//san
	////Thanh be tong
	taoLapPhuong(model * Translate(0, 5, 0) * Scale(0.5, 10, 0.5));
	taoLapPhuong(model * Translate(4.75, 5, 0) * Scale(0.5, 10, 0.5));
	taoLapPhuong(model * Translate(-4.75, 5, 0) * Scale(0.5, 10, 0.5));
	taoLapPhuong(model * Translate(0, 5, 4.75) * Scale(0.5, 10, 0.5));
	taoLapPhuong(model * Translate(0, 5, -4.75) * Scale(0.5, 10, 0.5));
	taoLapPhuong(model * Translate(4.75, 5, -4.75) * Scale(0.5, 10, 0.5));
	taoLapPhuong(model * Translate(4.75, 5, 4.75) * Scale(0.5, 10, 0.5));
	taoLapPhuong(model * Translate(-4.75, 5, 4.75) * Scale(0.5, 10, 0.5));
	taoLapPhuong(model * Translate(-4.75, 5, -4.75) * Scale(0.5, 10, 0.5));
	taoLapPhuong(model * Translate(0, 7, 0) * Scale(10, 1, 10));//san

	/////////cua
	taoLapPhuong(model * Translate(-0.5, 1, 5) * Scale(1, 2, 0.1));
	taoLapPhuong(model * Translate(-0.5, 1.3, 5) * Scale(0.8, 1, 0.11));
	taoLapPhuong(model * Translate(0.51, 1, 5) * Scale(1, 2, 0.1));
	taoLapPhuong(model * Translate(0.51, 1.3, 5) * Scale(0.8, 1, 0.11));
	taoLapPhuong(model * Translate(0, -0.25, 5) * Scale(2, 0.5, 1));
	taoLapPhuong(model * Translate(0, -0.5, 6.5) * Scale(2, 0.01, 3));
}

mat4 m;
float xt = 0, yt = 0, zt = 0, xoaybanh=0, xoaythan =0, xoaygau = 0, xoaythanhmuc=0, xoaycua=0;
void MayXuc(mat4 m)
{
	m = Translate(xt, yt, zt) * RotateX(xnn) * RotateY(ynn) * RotateZ(znn);
	ThanDuoi(m * Translate(0.25, -0.45, 0) * Scale(1, 1, 1));
	ThanTren(m * Translate(0, 0, 0) * RotateY(xoaythan) * Scale(1, 1, 1));
	mat4 cua = m * Translate(0, 0, 0) * RotateY(xoaythan);
	CuaXe(cua * Translate(1.04, 0.5, 0.6) * Translate(0.425, 0, 0) * RotateY(xoaycua) * Translate(-0.425, 0, 0) * Scale(1, 1, 1));
	ThanhXuc2(m * RotateY(xoaythan) * Translate(3, 1.3, -0.3) * Scale(1, 1, 1));
	ThanhXuc1(m * RotateY(xoaythan) * Translate(4.5, 0.9, -0.3) * RotateZ(-30) * Translate(-0.67, 0.1, 0) * RotateZ(xoaythanhmuc) * Translate(0.67, -0.1, 0) * Scale(0.7, 1, 1));
	mat4 gau = m * RotateY(xoaythan) * Translate(4.5, 0.9, -0.3) * RotateZ(-30) * Translate(-0.67, 0.1, 0) * RotateZ(xoaythanhmuc) * Translate(0.67, -0.1, 0) * Scale(0.7, 1, 1);
	GauMuc(gau * Translate(1.63, -0.47, -0.5) * RotateZ(-30) * Translate(-0.6, 0.2, 0) * RotateZ(xoaygau) * Translate(0.6, -0.2, 0) * Scale(1, 1, 1));
	BanhXe(m * Translate(1.75, -0.5, 0.5) * RotateZ(xoaybanh) * Scale(0.7, 0.7, 0.7));//banh xe phai truoc
	BanhXe(m * Translate(1.75, -0.5, -0.3) * RotateZ(xoaybanh) * Scale(0.7, 0.7, 0.7));//banh xe trai truoc
	BanhXe(m * Translate(-1.35, -0.5, 0.5) * RotateZ(xoaybanh) * Scale(0.7, 0.7, 0.7));//banh xe phai sau
	BanhXe(m * Translate(-1.35, -0.5, -0.3) * RotateZ(xoaybanh) * Scale(0.7, 0.7, 0.7));//banh xe trai sau
	//ThanTren(Scale(1, 1, 1));*/
	//NhaDangXay(Scale(1, 1, 1));

}
/////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////DUCLE/////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////
// ham tu dong goi khi khong lam gi ca

float kt = 0, bien=0;
void idle(void)
{
	if (bien==0.5)
	{
		xoaythan += bien;
		kt += 0.5;
	}
	if (xoaythan == 360) xoaythan = 0;
	if (kt == 180)
	{
		kt = 0;
		bien = 0;
	}
	glutPostRedisplay();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	MayXuc(Scale(1,1,1));
	////////////////////////////////////////////////////////////
	at = vec4(cosf(cam_Rotation[0] * DegreesToRadians), 0, cosf(cam_Rotation[2] * DegreesToRadians), 0) + eye;
	view = RotateX(cam_Rotation[1]) * LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, view);
	projection = Frustum(-0.025, 0.025, -0.025, 0.025, 0.02, 50);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection);
	glViewport(0, 0, 1000, 1000);
	glutSwapBuffers();
}

void KeyboardControl(unsigned char key, int x, int y)
{
	vec3 cam_forward = normalize(vec3(cosf(cam_Rotation[0] * DegreesToRadians) * abs(cosf(cam_Rotation[1] * DegreesToRadians)),
		-sinf(cam_Rotation[1] * DegreesToRadians),
		cosf(cam_Rotation[2] * DegreesToRadians) * abs(cosf(cam_Rotation[1] * DegreesToRadians))));
	vec3 cam_right = normalize(cross(cam_forward, up));
	vec4 cam_up = normalize(cross(cam_right, cam_forward));

	switch (key) {
	case 033:
		exit(1);
		break;
	case 'f':
		cam_Rotation[0] += 10 * (slow ? 0.1 : 1);
		cam_Rotation[2] += 10 * (slow ? 0.1 : 1);
		break;
	case 'h':
		cam_Rotation[0] -= 10 * (slow ? 0.1 : 1);
		cam_Rotation[2] -= 10 * (slow ? 0.1 : 1);
		cout << cam_Rotation[0] << ": " << cam_Rotation[2] << endl;
		break;
	case 't':
		cam_Rotation[1] -= 10 * (slow ? 0.1 : 1);
		if (cam_Rotation[1] <= -90) cam_Rotation[1] = -90;
		break;
	case 'g':
		cam_Rotation[1] += 10 * (slow ? 0.1 : 1);
		if (cam_Rotation[1] >= 90) cam_Rotation[1] = 90;
		break;
	case 'w':
		eye = eye + cam_forward * (slow ? 0.1 : 1);
		break;
	case 's':
		eye = eye - cam_forward * (slow ? 0.1 : 1);
		cout << eye.x << ": " << eye.y << ": " << eye.z << ":::" << endl;
		break;
	case 'a':
		eye = eye - cam_right * (slow ? 0.1 : 1);
		break;
	case 'd':
		eye = eye + cam_right * (slow ? 0.1 : 1);
		break;
	case 'q':
		eye = eye - cam_up * (slow ? 0.1 : 1);
		break;
	case 'e':
		eye = eye + cam_up * (slow ? 0.1 : 1);
		break;
	case 'r':
		slow = !slow;
		break;
	//////////////////////////////////////////////////////////////////
		////////////////////////////DUCLE//////////////////
	/////////////////////////////////////////////////////////////////
	case 'x':
		xnn += 10;
		if (xnn == 360) xnn = -360;
			break;
	case 'X':
		xnn -= 10;
		if (xnn == -360) xnn = 360;
			break;
	case 'y':
		ynn += 10;
		if (ynn == 360) ynn = -360;
		break;
	case 'Y':
		ynn -= 10;
		if (ynn == -360) ynn = 360;
		break;
	case 'z':
		znn += 10;
		if (znn == 360) znn = -360;
		break;
	case 'Z':
		znn -= 10;
		if (znn == -360) znn = 360;
		break;
	case '.'://tien
		xt += 0.1;
		xoaybanh -= 10;
		if (xoaybanh == -360) xoaybanh = 0;
		break;
	case ','://lui
		xt -= 0.1;
		xoaybanh += 10;
		if (xoaybanh == 360) xoaybanh = 0;
		break;
	case '/'://quay than tren 
		xoaythan += 10;
		if (xoaythan == 360) xoaythan = 0;
		break;
	case '?'://quay than tren 
		xoaythan -= 10;
		if (xoaythan == -360) xoaythan = 0;
		break;
	case 'm'://quay thanh muc
		if (xoaythanhmuc > -60) xoaythanhmuc -= 10;
		break;
	case 'M'://quay thanh muc
		if (xoaythanhmuc < 0) xoaythanhmuc += 10;
		break;
	case 'n'://quay gau muc
		if (xoaygau > -60) xoaygau -= 10 ;
		break;
	case 'N'://quay gau muc
		if (xoaygau < 0) xoaygau += 10;
		break;
	case 'c'://mo cua
		if (xoaycua < 90) xoaycua += 10;
		break;
	case 'C'://dong cua
		if (xoaycua > 0) xoaycua -= 10;
		break;
	case ' ':
		bien = 0.5;
		break;
	//////////////////////////////////////////////////////////////////
		////////////////////////////DUCLE//////////////////
	/////////////////////////////////////////////////////////////////



	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glEnable(GL_MULTISAMPLE);
	glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
	glutSetOption(GL_MULTISAMPLE, 8);

	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(50, 0);
	glutCreateWindow("A Cube is rotated by KeyboardControl and shaded");

	glewInit();

	GenerateAllShape();
	initGPUBuffers();
	shaderSetup();

	glutDisplayFunc(display);
	glutKeyboardFunc(KeyboardControl);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}
/////////////////////////////
// ===== Key control ===== //
/////////////////////////////
/*
a: Di chuyen sang trai
w: Di chuyen len
s: Di chuyen lui
d: Di chuyen sang phai
q: Di chuyen len cao
e: Di chuyen xuong thap
r: Tang do nhanh/ cham
f: Xoay camera sang trai
h: Xoay camera sang phai
t: Xoay camera len troi
g: Xoay camera xuong dat
space: show quat cay
shift + up: chinh co quat len cao
shift + down: chinh co quat xuong thap
ctrl + up: tang chieu cao quat
ctrl + down: ha thap quat
ctrl + left: xoay dau quat
1: Che do xoay canh 1
2: Che do xoay canh 2
3: Che do xoay canh 3
0: Dung xoay/ tat quat
*/