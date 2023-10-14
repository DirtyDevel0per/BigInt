#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpragma-pack"
#pragma once
#include <cinttypes>
#include <iostream>

#pragma pack(push, 1)
struct int2023_t {
  uint8_t bytes[253]{0};
};
#pragma pack(pop)
static_assert(sizeof(int2023_t) <= 253, "Size of int2023_t must be no higher than 253 bytes");

int2023_t from_int(int32_t i);
bool operator<(const int2023_t& lhs, const int2023_t& rhs);
bool operator>=(const int2023_t& lhs, const int2023_t rhs);
int2023_t from_string(const char* buff);

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs);

bool operator==(const int2023_t& lhs, const int2023_t& rhs);

bool operator!=(const int2023_t& lhs, const int2023_t& rhs);

std::ostream& operator<<(std::ostream& stream, const int2023_t& value);
int2023_t operator<<(const int2023_t& lhs, const int rhs);

#pragma clang diagnostic pop