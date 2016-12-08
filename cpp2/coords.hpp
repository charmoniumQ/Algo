#ifndef COORDS_HPP_
#define COORDS_HPP_

#include <cmath>
#include <stdexcept>

template <typename Real>
class Coord2 {
public:
  Real x, y;

  const Real& operator[](uint8_t i) const {
	if (i == 0) { return x; }
	if (i == 1) { return y; }
	throw std::runtime_error("Index out of bounds");
  }
  Real& operator[](uint8_t i) {
	if (i == 0) { return x; }
	if (i == 1) { return y; }
	throw std::runtime_error("Index out of bounds");
  }

  Coord2() = default;
  Coord2(Real x_, Real y_) { x = x_; y = y_; }

  Coord2 operator+() const {
	return Coord2(+this->x, +this->y);
  }
  Coord2 operator-() const {
	return Coord2(-this->x, -this->y);
  }
  Coord2 operator+(const Coord2& that) const {
	return Coord2(this->x + that.x, this->y + that.y);
  }
  Coord2 operator-(const Coord2& that) const {
	return Coord2(this->x - that.x, this->y - that.y);
  }
  Coord2& operator+=(const Coord2& that) {
	this->x += that.x; this->y += that.y;
  }
  Coord2& operator-=(const Coord2& that) {
	this->x -= that.x; this->y -= that.y;
  }

  double d() {
	return std::sqrt(x*x + y*y);
  }

  friend std::ostream& operator<<(std::ostream& os, const Coord2<Real>& array) {
	return os << "(" << array.x << ", " << array.y << ")";
  }

};

template <typename Real>
double d(const Coord2<Real>& p1, const Coord2<Real>& p2) { return (p1 - p2).d(); }

#endif
