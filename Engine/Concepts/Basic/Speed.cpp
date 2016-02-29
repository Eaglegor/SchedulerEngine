#include "Speed.h"

namespace Scheduler
{

	Speed::Speed() : value(Distance(0)) {}

	Speed::Speed(const Distance &value) : value(value) {}

	Speed::Speed(const Speed& rhs) : value(rhs.value) {}

	const Distance &Speed::getValue() const {
		return value;
	}

	void Speed::setValue(const Distance &value) {
		this->value = value;
	}

	Speed& Speed::operator=(const Speed& rhs) {
		this->value = rhs.value;
		return *this;
	}

	Speed Speed::operator+(const Speed& rhs) const {
		return Speed(value + rhs.value);
	}

	Speed Speed::operator-(const Speed& rhs) const {
		return Speed(value - rhs.value);
	}

	Speed& Speed::operator+=(const Speed& rhs) {
		this->value += rhs.value;
		return *this;
	}

	Speed& Speed::operator-=(const Speed& rhs) {
		this->value -= rhs.value;
		return *this;
	}

	bool Speed::operator==(const Speed& rhs) const {
		return this->value == rhs.value;
	}

	bool Speed::operator!=(const Speed& rhs) const {
		return !(*this == rhs);
	}

	bool Speed::operator>(const Speed& rhs) const {
		return this->value > rhs.getValue();
	}

	bool Speed::operator<(const Speed& rhs) const {
		return this->value < rhs.getValue();
	}

	bool Speed::operator>=(const Speed& rhs) const {
		return *this > rhs || *this == rhs;
	}

	bool Speed::operator<=(const Speed& rhs) const {
		return *this < rhs || *this == rhs;
	}

	Speed Speed::operator-() const {
		return Speed(-value);
	}

	Speed Speed::operator*(float multiplier) const {
		return Speed(this->value * multiplier);
	}

	Speed& Speed::operator*=(float multiplier) {
		this->value *= multiplier;
		return *this;
	}

	Speed Speed::operator/(float divider) const {
		return Speed(this->value / divider);
	}

	Speed& Speed::operator/=(float divider) {
		this->value /= divider;
		return *this;
	}
}