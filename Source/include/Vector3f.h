#include <iostream>

class Vector3F {
	private:
		float v[3];
	public:
		Vector3F();
		Vector3F(float x, float y, float z);
		float Value(int index) const;

		float &operator[](int index);
		float operator[](int index) const;

		Vector3F operator*(float scale) const;
		Vector3F operator/(float scale) const;
		Vector3F operator+(const Vector3F &other) const;
		Vector3F operator-(const Vector3F &other) const;
		Vector3F operator-() const;

		const Vector3F &operator*=(float scale);
		const Vector3F &operator/=(float scale);
		const Vector3F &operator+=(const Vector3F &other);
		const Vector3F &operator-=(const Vector3F &other);

		float magnitude() const;
		float magnitudeSquared() const;
		Vector3F normalize() const;
		float dot(const Vector3F &other) const;
		Vector3F cross(const Vector3F &other) const;
};

Vector3F operator*(float scale, const Vector3F &v);
std::ostream &operator<<(std::ostream &output, const Vector3F &v);

