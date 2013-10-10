#include <math.h>

#include "../include/vek.h"

vek::vek()
{
	//ctor
	m_x = m_y = 0;
}

double vek::get_len() {
	return sqrt(*this * *this);
}

vek vek::get_dir() {
	vek tmp(*this);
	return tmp / tmp.get_len();
}

vek::~vek()
{
	//dtor
}

vek& vek::operator=(const double& rhs) {
	m_x = m_y = rhs;
	return *this;
}

vek vek::operator+(const vek& rhs)
{
  /* Erzeugen eines neuen Objektes, dessen Attribute gezielt einzeln gesetzt werden. Oder: */
  vek tmp(*this); //Kopie des linken Operanden
  tmp += rhs; //Implementierung mittels des +=-Operators
  return tmp;
}

vek& vek::operator+=(const vek& rhs)
{
	m_x += rhs.GetX();
	m_y += rhs.GetY();
	return *this;
}


vek vek::operator-(const vek& rhs)
{
	vek tmp(*this);
	tmp -= rhs;
	return tmp;
}

vek& vek::operator-=(const vek& rhs) {
	m_x -= rhs.GetX();
	m_y -= rhs.GetY();
	return *this;
}

double vek::operator*(const vek& rhs)
{
	double tmp = m_x*rhs.GetX() + m_y*rhs.GetY();
	return tmp;
}

vek vek::operator*(const double& rhs)
{
	vek tmp(*this); //Kopie des linken Operanden
	tmp *= rhs;
	return tmp;
}

vek& vek::operator*=(const double& rhs) {
	m_x *= rhs;
	m_y *= rhs;
	return *this;
}

vek vek::operator/(const double& rhs)
{
	vek tmp(*this); //Kopie des linken Operanden
	tmp /= rhs;
	return tmp;
}

vek& vek::operator/=(const double& rhs) {
	m_x /= rhs;
	m_y /= rhs;
	return *this;
}

void vek::print(){
	std::cout << '(' << m_x << ',' << m_y << ')';
}
