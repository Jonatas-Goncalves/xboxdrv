/*
**  Xbox360 USB Gamepad Userspace Driver
**  Copyright (C) 2011 Ingo Ruhnke <grumbel@gmail.com>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "axisfilter/relative_axis_filter.hpp"

#include <algorithm>
#include <sstream>
#include <stdexcept>

#include "helper.hpp"

RelativeAxisFilter* RelativeAxisFilter::from_string(const std::string& str) {
  int speed = 20000;

  std::vector<std::string> tokens = string_split(str, ":");
  int idx = 0;
  for (auto& t : tokens) {
    switch (idx) {
      case 0:
        speed = std::stoi(t);
        break;
      default:
        throw std::runtime_error("to many arguments");
        break;
    }
    ++idx;
  }

  return new RelativeAxisFilter(speed);
}

RelativeAxisFilter::RelativeAxisFilter(int speed)
    : m_speed(speed), m_float_speed(0.0f), m_value(0), m_state(0) {}

void RelativeAxisFilter::update(int msec_delta) {
  m_state += m_float_speed * m_value * msec_delta / 1000.0f;
  m_state = std::clamp(m_state, -1.0f, 1.0f);
}

int RelativeAxisFilter::filter(int value, int min, int max) {
  m_value = to_float(value, min, max);

  m_float_speed = to_float_no_range_check(m_speed, min, max);

  return from_float(m_state, min, max);
}

std::string RelativeAxisFilter::str() const {
  std::ostringstream out;
  out << "relativeaxis:" << m_speed;
  return out.str();
}

/* EOF */
