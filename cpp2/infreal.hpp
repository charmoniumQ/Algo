#ifndef INFREAL_HPP_
#define INFREAL_HPP_

#include <iostream>

template <typename Real>
class InfReal {
private:
  Real r;
  bool inf;
public:
  InfReal(Real r_, bool inf_) : r(r_), inf(inf_) {}
  InfReal(Real r_) : InfReal<Real>(r_, false) {}
  InfReal() : InfReal<Real>(0, true) {}
  InfReal(const InfReal<Real>& other) : InfReal<Real>(other.r, other.inf) {}
  InfReal& operator=(const InfReal<Real>& other) {
	this->r = other.r; this->inf = other.inf;
	return *this;
  }
  // InfReal() = default;

  InfReal operator+(const InfReal<Real>& other) const {
	return InfReal(this->r + other.r, this->inf || other.inf);
  }
  InfReal operator-(const InfReal<Real>& other) const {
	return InfReal(this->r - other.r, this->inf || other.inf);
  }
  InfReal operator*(const InfReal<Real>& other) const {
	return InfReal(this->r * other.r, this->inf || other.inf);
  }
  InfReal operator/(const InfReal<Real>& other) const {
	return InfReal(this->r / other.r, this->inf || other.inf || other.r == 0);
  }
  InfReal& operator++() { ++r; return *this; }
  InfReal& operator--() { --r; return *this; }
  InfReal& operator+=(const InfReal<Real>& other) {
	this->r += other.r;
	this->inf |= other.inf;
	return *this;
  }
  InfReal& operator-=(const InfReal<Real>& other) {
	this->r -= other.r;
	this->inf |= other.inf;
	return *this;
  }
  InfReal& operator*=(const InfReal<Real>& other) {
	this->r *= other.r;
	this->inf |= other.inf;
	return *this;
  }
  InfReal& operator/=(const InfReal<Real>& other) {
	this->r /= other.r;
	this->inf |= other.inf || other.r == 0;
	return *this;
  }
  InfReal operator++(int) { InfReal<Real> other (*this); ++r; return other; }
  InfReal operator--(int) { InfReal<Real> other (*this); --r; return other; }
  InfReal operator-() const { InfReal<Real>(-r, inf); }
  InfReal operator+() const { InfReal<Real>(+r, inf); }
  bool operator<(const InfReal<Real>& other) const {
	return not this->inf and (other.inf or this->r < other.r);
  }
  bool operator>(const InfReal<Real>& other) const {
	return not other.inf and (this->inf or this->r > other.r);
  }
  bool operator<=(const InfReal<Real>& other) const {
	return not this->inf and (other.inf or this->r < other.r);
  }
  bool operator>=(const InfReal<Real>& other) const {
	return not other.inf and (this->inf or this->r > other.r);
  }
  bool operator==(const InfReal<Real>& other) const {
	return (this->inf and other.inf) or (not this->inf and not other.inf and this->r == other.r);
  }
  bool operator!=(const InfReal<Real>& other) const {
	return !(*this == other);
  }

  bool finite() const { return not inf; }

  friend std::ostream& operator<<(std::ostream& out, const InfReal& ir) {
	if (ir.finite()) {
	  out << static_cast<int>(ir.r);
	} else {
	  out << "inf";
	}
	return out;
  }
};

#endif
