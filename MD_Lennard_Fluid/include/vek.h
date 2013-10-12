#ifndef VEK_H
#define VEK_H

#include <iostream>


class vek
{
	public:
		vek();
		vek(double x, double y) : m_x(x), m_y(y) {};
		double GetX() const { return m_x; }
		double GetY() const { return m_y; }
		void SetX(double val) { m_x = val; }
		void SetY(double val) { m_y = val; }

		double get_len();
		vek get_dir();

		void print();

		vek& operator=(const double& rhs);
		vek operator+(const vek& rhs);
		vek& operator+=(const vek& rhs);
		vek operator-(const vek& rhs);
		vek& operator-=(const vek& rhs);
		double operator*(const vek& rhs);
		vek operator*(const double& rhs);
		vek& operator*=(const double& rhs);
		vek operator/(const double& rhs);
		vek& operator/=(const double& rhs);

		~vek();
	protected:
	private:
		double m_x;
		double m_y;
};

#endif // VEK_H
