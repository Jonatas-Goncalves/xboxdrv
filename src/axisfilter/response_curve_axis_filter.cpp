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

#include "response_curve_axis_filter.hpp"

#include <sstream>

#include "helper.hpp"

ResponseCurveAxisFilter* ResponseCurveAxisFilter::from_string(
    const std::string& str) {
  std::vector<int> samples;

  for (auto& i : string_split(str, ":")) {
    samples.push_back(std::stoi(i));
  }

  return new ResponseCurveAxisFilter(samples);
}

ResponseCurveAxisFilter::ResponseCurveAxisFilter(
    const std::vector<int>& samples)
    : m_samples(samples) {}

int ResponseCurveAxisFilter::filter(int value, int min, int max) {
  if (m_samples.empty()) {
    return value;
  } else if (m_samples.size() == 1) {
    return m_samples[0];
  } else {
    // FIXME: should rewrite this to use integer only and make sure
    // that the edge conditions are meet
    int bucket_count = m_samples.size() - 1;
    float bucket_size = (max - min) / static_cast<float>(bucket_count);

    int bucket_index = int((value - min) / bucket_size);

    float t =
        ((value - min) - (static_cast<float>(bucket_index) * bucket_size)) /
        bucket_size;

    return ((1.0f - t) * m_samples[bucket_index]) +
           (t * m_samples[bucket_index + 1]);
  }
}

std::string ResponseCurveAxisFilter::str() const {
  std::ostringstream out;
  out << "responsecurve";
  for (std::vector<int>::const_iterator i = m_samples.begin();
       i != m_samples.end(); ++i) {
    out << ":" << *i;
  }
  return out.str();
}

/* EOF */
