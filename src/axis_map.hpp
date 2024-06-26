/*
**  Xbox360 USB Gamepad Userspace Driver
**  Copyright (C) 2010 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_XBOXDRV_AXIS_MAP_HPP
#define HEADER_XBOXDRV_AXIS_MAP_HPP

#include "axis_event.hpp"
#include "xboxmsg.hpp"

class AxisMap {
 private:
  AxisEventPtr m_axis_map[XBOX_BTN_MAX][XBOX_AXIS_MAX];

 public:
  AxisMap();

  void bind(XboxAxis code, AxisEventPtr event);
  void bind(XboxButton shift_code, XboxAxis code, AxisEventPtr event);

  AxisEventPtr lookup(XboxAxis code) const;
  AxisEventPtr lookup(XboxButton shift_code, XboxAxis code) const;

  void clear();

  void init(UInput& uinput, int slot, bool extra_devices) const;
  void update(UInput& uinput, int msec_delta);
};

#endif

/* EOF */
