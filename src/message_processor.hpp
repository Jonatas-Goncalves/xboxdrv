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

#ifndef HEADER_XBOXDRV_MESSAGE_PROCESSOR_HPP
#define HEADER_XBOXDRV_MESSAGE_PROCESSOR_HPP

#include <cstdint>
#include <functional>

struct XboxGenericMsg;
class Controller;

class MessageProcessor {
 public:
  MessageProcessor() {}
  virtual ~MessageProcessor() {}

  virtual void send(const XboxGenericMsg& msg, int msec_delta) = 0;
  virtual void set_controller(Controller* controller) = 0;

 private:
  MessageProcessor(const MessageProcessor&);
  MessageProcessor& operator=(const MessageProcessor&);
};

#endif

/* EOF */
