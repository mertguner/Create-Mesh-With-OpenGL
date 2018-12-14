#include <math.h>

#include "Vector3f.h"

using namespace std;

Vector3F::Vector3F() {

}

Vector3F::Vector3F(float x, float y, float z) {
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

float &Vector3F::operator[](int index) {
	return v[index];
}

float Vector3F::operator[](int index) const {
	return v[index];
}

Vector3F Vector3F::operator*(float scale) const {
	return Vector3F(v[0] * scale, v[1] * scale, v[2] * scale);
}

Vector3F Vector3F::operator/(float scale) const {
	return Vector3F(v[0] / scale, v[1] / scale, v[2] / scale);
}

Vector3F Vector3F::operator+(const Vector3F &other) const {
	return Vector3F(v[0] + other.v[0], v[1] + other.v[1], v[2] + other.v[2]);
}

Vector3F Vector3F::operator-(const Vector3F &other) const {
	return Vector3F(v[0] - other.v[0], v[1] - other.v[1], v[2] - other.v[2]);
}

Vector3F Vector3F::operator-() const {
	return Vector3F(-v[0], -v[1], -v[2]);
}

const Vector3F &Vector3F::operator*=(float scale) {
	v[0] *= scale;
	v[1] *= scale;
	v[2] *= scale;
	return *this;
}

const Vector3F &Vector3F::operator/=(float scale) {
	v[0] /= scale;
	v[1] /= scale;
	v[2] /= scale;
	return *this;
}

const Vector3F &Vector3F::operator+=(const Vector3F &other) {
	v[0] += other.v[0];
	v[1] += other.v[1];
	v[2] += other.v[2];
	return *this;
}

const Vector3F &Vector3F::operator-=(const Vector3F &other) {
	v[0] -= other.v[0];
	v[1] -= other.v[1];
	v[2] -= other.v[2];
	return *this;
}

float Vector3F::Value(int index) const {
	return v[index];
}
float Vector3F::magnitude() const {
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

float Vector3F::magnitudeSquared() const {
	return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

Vector3F Vector3F::normalize() const {
	float m = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	return Vector3F(v[0] / m, v[1] / m, v[2] / m);
}

float Vector3F::dot(const Vector3F &other) const {
	return v[0] * other.v[0] + v[1] * other.v[1] + v[2] * other.v[2];
}

Vector3F Vector3F::cross(const Vector3F &other) const {
	return Vector3F(v[1] * other.v[2] - v[2] * other.v[1],
				 v[2] * other.v[0] - v[0] * other.v[2],
				 v[0] * other.v[1] - v[1] * other.v[0]);
}

Vector3F operator*(float scale, const Vector3F &v) {
	return v * scale;
}

ostream &operator<<(ostream &output, const Vector3F &v) {
	cout << '(' << v[0] << ", " << v[1] << ", " << v[2] << ')';
	return output;
}









