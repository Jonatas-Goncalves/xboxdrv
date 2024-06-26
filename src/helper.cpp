/*
**  Xbox/Xbox360 USB Gamepad Userspace Driver
**  Copyright (C) 2008 Ingo Ruhnke <grumbel@gmail.com>
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

#include "helper.hpp"

#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>

#include <algorithm>
#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <format>
#include <iostream>
#include <stdexcept>

#include "helper.hpp"
#include "raise_exception.hpp"

int hexstr2int(const std::string& str) {
  unsigned int value = 0;
  if (sscanf(str.c_str(), "%x", &value) == 1) {
    return value;
  } else if (sscanf(str.c_str(), "0x%x", &value) == 1) {
    return value;
  } else {
    raise_exception(std::runtime_error,
                    "couldn't convert '" << str << "' to int");
  }
}

bool str2bool(std::string const& str) {
  bool b;
  std::istringstream(str) >> std::boolalpha >> b;
  return b;
}

std::string raw2str(uint8_t* data, int len) {
  std::ostringstream out;
  out << "len: " << len << " data: ";

  for (int i = 0; i < len; ++i) {
    out << std::format("{:#02x} ", int(data[i]));
  }

  return out.str();
}

std::string to_lower(const std::string& str) {
  std::string lower_impl = str;

  for (std::string::iterator i = lower_impl.begin(); i != lower_impl.end();
       ++i) {
    *i = static_cast<char>(tolower(*i));
  }

  return lower_impl;
}

void split_string_at(const std::string& str, char c, std::string* lhs,
                     std::string* rhs) {
  std::string::size_type p = str.find(c);
  if (p == std::string::npos) {
    *lhs = str;
  } else {
    *lhs = str.substr(0, p);
    *rhs = str.substr(p + 1);
  }
}

std::vector<std::string> string_split(std::string_view text,
                                      std::string_view delimiter) {
  std::vector<std::string> result;

  std::string::size_type pos = 0;
  std::string::size_type prev = 0;
  while ((pos = text.find(delimiter, prev)) != std::string::npos) {
    result.emplace_back(text.substr(prev, pos - prev));
    prev = pos + delimiter.length();
  }

  // to get the last substring
  // (or entire string if delimiter is not found)
  result.emplace_back(text.substr(prev));

  return result;
}

void process_name_value_string(
    const std::string& str,
    const std::function<void(const std::string&, const std::string&)>& func) {
  std::vector<std::string> tokens = string_split(str, ",");

  for (auto& i : tokens) {
    std::string lhs, rhs;
    split_string_at(i, '=', &lhs, &rhs);
    func(lhs, rhs);
  }
}

bool is_number(const std::string& str) {
  for (std::string::const_iterator i = str.begin(); i != str.end(); ++i) {
    if (!isdigit(*i)) {
      return false;
    }
  }
  return true;
}

int to_number(int range, const std::string& str) {
  if (str.empty()) {
    return 0;
  } else {
    if (str[str.size() - 1] == '%') {
      int percent = std::stoi(str.substr(0, str.size() - 1));
      return range * percent / 100;
    } else {
      return std::stoi(str);
    }
  }
}

uint32_t get_time() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

float to_float_no_range_check(int value, int min, int max) {
  // FIXME: '+1' is kind of a hack to
  // get the center at 0 for the
  // [-32768, 32767] case
  int center = (max + min + 1) / 2;

  if (value < center) {
    return static_cast<float>(value - center) /
           static_cast<float>(center - min);
  } else  // (value >= center)
  {
    return static_cast<float>(value - center) /
           static_cast<float>(max - center);
  }
}

float to_float(int value, int min, int max) {
  return std::clamp(to_float_no_range_check(value, min, max), -1.0f, 1.0f);
}

int from_float(float value, int min, int max) {
  return (value + 1.0f) / 2.0f * static_cast<float>(max - min) + min;
}

int get_terminal_width() {
  struct winsize w;
  if (ioctl(0, TIOCGWINSZ, &w) < 0) {
    return 80;
  } else {
    return w.ws_col;
  }
}

pid_t spawn_exe(const std::string& arg0) {
  std::vector<std::string> args;
  args.push_back(arg0);
  return spawn_exe(args);
}

pid_t spawn_exe(const std::vector<std::string>& args) {
  assert(!args.empty());

  pid_t pid = fork();
  if (pid == 0) {
    char** argv =
        static_cast<char**>(malloc(sizeof(char*) * (args.size() + 1)));
    for (size_t i = 0; i < args.size(); ++i) {
      argv[i] = strdup(args[i].c_str());
    }
    argv[args.size()] = NULL;

    if (execvp(args[0].c_str(), argv) == -1) {
      log_error(args[0] << ": exec failed: " << strerror(errno));
      _exit(EXIT_FAILURE);
    }
  }

  return pid;
}

/* EOF */
