#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include <cstdint>
#include <iostream>

class Date {
  uint16_t year;
  uint8_t month;
  uint8_t day;

 public:
  Date(uint16_t year, uint8_t month, uint8_t day)
      : year(year), month(month), day(day) {}

  Date(const std::string &date) {
    if (date.length() != 10 || date[2] != '/' || date[5] != '/') {
      throw std::invalid_argument("Invalid date format");
    }
    int d, m, y;
    if (sscanf(date.c_str(), "%d/%d/%d", &d, &m, &y) != 3) {
      throw std::invalid_argument("Failed to parse date");
    }
    if (d < 1 || d > 31 || m < 1 || m > 12 || y < 0 || y > 9999) {
      throw std::invalid_argument("Invalid date values");
    }
    day = static_cast<uint8_t>(d);
    month = static_cast<uint8_t>(m);
    year = static_cast<uint16_t>(y);
  }

  uint16_t getYear() const { return year; }
  uint8_t getMonth() const { return month; }
  uint8_t getDay() const { return day; }

  // friend std::istream &operator>>(std::istream &is, Date &date);
  // friend std::ostream &operator<<(std::ostream &os, const Date &date);
};

class DataLoader {
  std::string file_name;

 public:
  DataLoader(const std::string &file_name) : file_name(file_name) {}
};

#endif
