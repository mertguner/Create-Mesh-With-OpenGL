#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <math.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Vector3f.h"

using namespace std;

// Color Map Renklendirme için
float colormap[7][5] = {
    {0.00000,0.00000,0.00000,1.00000,1.00000},
    {0.10000,0.00000,0.00000,1.00000,1.00000},
    {0.25000,0.00000,1.00000,1.00000,1.00000},
    {0.50000,0.00000,0.50000,0.00000,1.00000},
    {0.75000,1.00000,1.00000,0.00000,1.00000},
    {0.90000,1.00000,0.00000,0.00000,1.00000},
    {1.00000,1.00000,0.00000,0.00000,1.00000}
    };

// Renklendirme için gerekli fonksiyonlar
float InterpolateComponentSystem(float endPoint1,float endPoint2,float lambda)
{
    return (endPoint1 + (endPoint2 - endPoint1) * lambda);
}

Vector3F InterpolateBetween(float lambda)
{
    Vector3F endPoint1,endPoint2;
    if (lambda <= 0) return Vector3F((colormap[0][1]),                  (colormap[0][2]),                  (colormap[0][3]));
    if (lambda >= 1) return Vector3F((colormap[7 - 1][1]), (colormap[7 - 1][2]), (colormap[7 - 1][3]));

    for (int i = 0; i < 7-1; i++)
    {

        if (lambda == colormap[i][0]) return Vector3F((colormap[i][1]), (colormap[i][2]), (colormap[i][3]));

        if (lambda <= colormap[0][0])
        {
            return Vector3F((colormap[0][1]), (colormap[0][2]), (colormap[0][3]));
        }
        else if ((lambda > colormap[i][0]) && (lambda < colormap[i + 1][0]))
        {
            endPoint1 = Vector3F((colormap[i][1]), (colormap[i][2]), (colormap[i][3]));
            endPoint2 = Vector3F((colormap[i + 1][1]), (colormap[i + 1][2]), (colormap[i + 1][3]));
            lambda = (lambda - colormap[i][0]) / (colormap[i + 1][0] - colormap[i][0]);
            float R = InterpolateComponentSystem(endPoint1.Value(0), endPoint2.Value(0), lambda);
            float G = InterpolateComponentSystem(endPoint1.Value(1), endPoint2.Value(1), lambda);
            float B = InterpolateComponentSystem(endPoint1.Value(2), endPoint2.Value(2), lambda);
            return Vector3F(R, G, B);
        }
        else if (lambda >= colormap[7 - 1][0])
        {
            return Vector3F((colormap[7 - 1][1]), (colormap[7 - 1][2]), (colormap[7 - 1][3]));
        }
    }
    return Vector3F(0,0,0);
}
//Represents a terrain, by storing a set of heights and normals at 2D locations
class TerrianHeighMap {
	private:

		int w; //Genişliği
		int l; //Uzunluğu
		float Max = -9999, Min = 9999;
		Vector3F** color;
		float** hs; //Yükseklik bilgileri yani Alınan dataların değerleri
		Vector3F** normals;
		bool computedNormals; //Noarmaller ışık yansımaları için gerekli
	public:
		TerrianHeighMap(int w2, int l2) {
			w = w2;
			l = l2;

			hs = new float*[l];
			for(int i = 0; i < l; i++) {
				hs[i] = new float[w];
			}

			normals = new Vector3F*[l];
			color = new Vector3F*[l];
			for(int i = 0; i < l; i++) {
				normals[i] = new Vector3F[w];
				color[i] = new Vector3F[w];
			}

			computedNormals = false;
		}

		~TerrianHeighMap() {
			for(int i = 0; i < l; i++) {
				delete[] hs[i];
			}
			delete[] hs;

			for(int i = 0; i < l; i++) {
				delete[] normals[i];
			}
			delete[] normals;
		}

		int width() {
			return w;
		}

		int length() {
			return l;
		}

		//yükseklik değerlerini set ederken aynı zamanda maksimum ve minimum değerleride takip eder.
		void setHeight(int x, int z, float y) {
			hs[z][x] = y;
			Max = max(Max,y);
			Min = min(Min,y);

			computedNormals = false;
		}

		//istediğimiz bir noktanın datasını verir
		float getHeight(int x, int z) {
			return hs[z][x];
		}
        //Min Max değerlerini Okur
        float getMin()
        {
            return Min;
        }
        //Min Max değerlerini Okur
        float getMax()
        {
            return Max;
        }
		//Normalleri hesapla
		void computeNormals() {
			if (computedNormals) {
				return;
			}

			Vector3F** normals2 = new Vector3F*[l];
			for(int i = 0; i < l; i++) {
				normals2[i] = new Vector3F[w];
			}

			for(int z = 0; z < l; z++) {
				for(int x = 0; x < w; x++) {
					Vector3F sum(0.0f, 0.0f, 0.0f);

					Vector3F out;
					if (z > 0) {
						out = Vector3F(0.0f, hs[z - 1][x] - hs[z][x], -1.0f);
					}
					Vector3F in;
					if (z < l - 1) {
						in = Vector3F(0.0f, hs[z + 1][x] - hs[z][x], 1.0f);
					}
					Vector3F left;
					if (x > 0) {
						left = Vector3F(-1.0f, hs[z][x - 1] - hs[z][x], 0.0f);
					}
					Vector3F right;
					if (x < w - 1) {
						right = Vector3F(1.0f, hs[z][x + 1] - hs[z][x], 0.0f);
					}

					if (x > 0 && z > 0) {
						sum += out.cross(left).normalize();
					}
					if (x > 0 && z < l - 1) {
						sum += left.cross(in).normalize();
					}
					if (x < w - 1 && z < l - 1) {
						sum += in.cross(right).normalize();
					}
					if (x < w - 1 && z > 0) {
						sum += right.cross(out).normalize();
					}

					normals2[z][x] = sum;
					color[z][x] = Vector3F(0.0f, 0.0f, 0.0f);
				}
			}

			//Işık yansılamarındaki keskinlikleri önlemek için normalleri yumuşatıyorum. Bir nevi Smooth efekti
			const float FALLOUT_RATIO = 0.5f;
			for(int z = 0; z < l; z++) {
				for(int x = 0; x < w; x++) {
					Vector3F sum = normals2[z][x];

					if (x > 0) {
						sum += normals2[z][x - 1] * FALLOUT_RATIO;
					}
					if (x < w - 1) {
						sum += normals2[z][x + 1] * FALLOUT_RATIO;
					}
					if (z > 0) {
						sum += normals2[z - 1][x] * FALLOUT_RATIO;
					}
					if (z < l - 1) {
						sum += normals2[z + 1][x] * FALLOUT_RATIO;
					}

					if (sum.magnitude() == 0) {
						sum = Vector3F(0.0f, 1.0f, 0.0f);
					}
					normals[z][x] = sum;
				}
			}

			for(int i = 0; i < l; i++) {
				delete[] normals2[i];
			}
			delete[] normals2;

			computedNormals = true;
		}

		Vector3F getNormal(int x, int z) {
			if (!computedNormals) {
				computeNormals();
			}
			return normals[z][x];
		}
};

//Dosyadan açılıyor x==20 bilgisi tek boyutlu dizi halinde gelen verilerin nereden bölünmesi gerektiği bilgisidir.
TerrianHeighMap* loadTerrain(const char* filename)
{
    float x=0,y=0;
	TerrianHeighMap* t = new TerrianHeighMap(20, 20);

    ifstream file(filename);
    string str;
    while (getline(file, str))
    {
        t->setHeight(x, y, atof(str.c_str()));
        x++;
        if(x==20){x=0;y++;}
    }
	t->computeNormals();
	return t;
}

//Ekrandaki görüntüyü dödürme hızını belirtir.
float _angle = 60.0f;

TerrianHeighMap* _terrain;

void cleanup() {
	delete _terrain;
}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27: //Escape key
			cleanup();
			exit(0);
	}
}

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -10.0f);
	glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(-_angle, 0.0f, 1.0f, 0.0f);

	GLfloat ambientColor[] = {0.4f, 0.4f, 0.4f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	GLfloat lightColor0[] = {0.6f, 0.6f, 0.6f, 1.0f};
	GLfloat lightPos0[] = {-0.5f, 0.8f, 0.1f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	float scale = 5.0f / max(_terrain->width() - 1, _terrain->length() - 1);
	glScalef(scale, scale, scale);
	glTranslatef(-(float)(_terrain->width() - 1) / 2,
				 0.0f,
				 -(float)(_terrain->length() - 1) / 2);


	for(int z = 0; z < _terrain->length() - 1; z++) {
		glBegin(GL_TRIANGLE_STRIP);
		for(int x = 0; x < _terrain->width(); x++)
        {
            Vector3F sonuc = InterpolateBetween((_terrain->getHeight(x, z) - _terrain->getMin())/(_terrain->getMax() - _terrain->getMin()));
			glColor3f(sonuc[0],sonuc[1],sonuc[2]);
			Vector3F normal = _terrain->getNormal(x, z);
			glNormal3f(normal[0], normal[1], normal[2]);
			glVertex3f(x, _terrain->getHeight(x, z)/50, z);

			sonuc = InterpolateBetween((_terrain->getHeight(x, z +1) - _terrain->getMin())/(_terrain->getMax() - _terrain->getMin()));
			glColor3f(sonuc[0],sonuc[1],sonuc[2]);
			normal = _terrain->getNormal(x, z + 1);
			glNormal3f(normal[0], normal[1], normal[2]);
			glVertex3f(x, _terrain->getHeight(x, z + 1)/50, z + 1);

		}
		glEnd();
	}

	glutSwapBuffers();
}

void update(int value) {
	_angle += 1.0f;
	if (_angle > 360) {
		_angle -= 360;
	}

	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);

	glutCreateWindow("Huawei Icin Deneme Yazilimi");
	initRendering();

    //Exeyi çalıştırmadan önce datalar.txt dosyası Exe'nin yanında olmalıdır.
	_terrain = loadTerrain("datas.txt");

	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutTimerFunc(25, update, 0);

	glutMainLoop();
	return 0;
}









