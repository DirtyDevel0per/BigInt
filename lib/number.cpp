#include "number.h"

const int kNumbers = 252;

bool ZeroBuff(const char* buff, const unsigned long buff_length);

int2023_t from_int(int32_t i) {
  int amount = 0;
  bool flag = false;
  int2023_t res;

  if (i < 0) {
    i *= -1;
    flag = true;
  }

  while (i > 0) {
    res.bytes[kNumbers - amount / 8] += (i & 1) << amount % 8;
    i >>= 1;
    ++amount;
  }

  if (flag)
    return ~res + from_int(1);

  return res;
}

int2023_t operator~(const int2023_t& operand) {
  int2023_t res;
  int amount = 0;

  for (int i = kNumbers; i >= 0; --i) {
    amount = 0;
    res.bytes[i] = ~operand.bytes[i];
  }

  return res;
}

bool ZeroBuff(const char* buff, const unsigned long buff_length) {
  unsigned long i = 0;

  while (i < buff_length) {
    if (buff[i] != '0')
      return false;
    ++i;
  }

  return true;
}

int2023_t from_string(const char* buff) {
  const int k_Remainder_Of_Ten_Division = 10;
  int2023_t res;
  int remainder_of_two_division = 0;
  int amount = 0;
  bool flag = false;
  unsigned long buff_length = std::strlen(buff);

  if (buff[0] == '-') {
    flag = true;
    --buff_length;
  }

  char* tmp_buf = new char[buff_length];

  for (unsigned long it = 0; it < buff_length; it++)
    tmp_buf[it] = buff[it + flag];

  while (!ZeroBuff(tmp_buf, buff_length)) {
    remainder_of_two_division = 0;
    for (unsigned long i = 0; i < buff_length; i++) {
      if (remainder_of_two_division) {
        remainder_of_two_division = (tmp_buf[i] - '0') % 2;
        tmp_buf[i] = ((tmp_buf[i] - '0') + k_Remainder_Of_Ten_Division) / 2 + '0';
      }
      else {
        remainder_of_two_division = (tmp_buf[i] - '0') % 2;
        tmp_buf[i] = (tmp_buf[i] - '0') / 2 + '0';
      }
    }
    res.bytes[kNumbers - amount / 8] += remainder_of_two_division << amount % 8;
    ++amount;
  }

  if (flag)
    res = -res;

  return res;
}

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs) {
  int amount = 0;
  int remainder = 0;
  int2023_t res;

  for (int i = kNumbers; i >= 0; --i) {
    res.bytes[i] = (lhs.bytes[i] + rhs.bytes[i] + remainder) % 256;
    remainder = (lhs.bytes[i] + rhs.bytes[i] + remainder) / 256;
  }

  return res;
}

int2023_t operator<<(const int2023_t& lhs, const int rhs) {
  int2023_t res = lhs;
  int it = rhs;
  int tmp_next = 0;
  int tmp_cur = 0;

  while (it > 0) {
    tmp_next = 0;
    for (int i = kNumbers; i >= 0; --i) {
      tmp_cur = tmp_next;
      tmp_next = (res.bytes[i] >> 7) & 1;
      res.bytes[i] <<= 1;
      res.bytes[i] += tmp_cur;
    }
    --it;
  }

  return res;
}

int2023_t operator>>(const int2023_t& lhs, const int rhs) {
  int2023_t res = lhs;
  int it = rhs;
  int tmp_next = 0;
  int tmp_cur = 0;

  while (it > 0) {
    tmp_next = 0;
    for (int i = 0; i <= kNumbers; ++i) {
      tmp_cur = tmp_next;
      tmp_next = res.bytes[i] & 1;
      res.bytes[i] >>= 1;
      res.bytes[i] += tmp_cur << 7;
    }
    --it;
  }

  return res;
}

int2023_t operator-(const int2023_t& op) {
  int2023_t res = op;
  if (((res.bytes[0] >> 7) & 1) == 0)
    res = ~res + from_int(1);
  else
    res = ~(res - from_int(1));
  return res;
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t res;

  res = lhs + (-rhs);

  return res;
}

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t res, lhs_tmp = lhs, rhs_tmp = rhs, tmp;
  int amount = 0;
  bool flag_lhs = false;
  bool flag_rhs = false;

  if ((rhs.bytes[0] & (1 << 7)) != 0) {
    rhs_tmp = -rhs_tmp;
    flag_rhs = true;
  }
  if ((lhs.bytes[0] & (1 << 7)) != 0) {
    lhs_tmp = -lhs_tmp;
    flag_lhs = true;
  }

  if (lhs == from_int(0) || rhs == from_int(0))
    return from_int(0);
  if (rhs == from_int(1))
    return lhs;
  if (lhs == from_int(1))
    return rhs;

  if (lhs_tmp < rhs_tmp) {
    tmp = lhs_tmp;
    lhs_tmp = rhs_tmp;
    rhs_tmp = lhs_tmp;
  }

  while (amount <= kNumbers * 8) {
    if (((rhs_tmp.bytes[kNumbers - amount / 8] >> (amount % 8)) & 1))
      res = res + (lhs_tmp << amount);
    ++amount;
  }

  if (flag_lhs != flag_rhs)
    res = -res;

  return res;
}

bool operator>=(const int2023_t& lhs, const int2023_t& rhs) {

  for (int i = 0; i <= kNumbers; ++i) {
    if (lhs.bytes[i] > rhs.bytes[i])
      return true;
    else if (lhs.bytes[i] < rhs.bytes[i])
      return false;
  }

  return true;
}

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t res, lhs_tmp = lhs, rhs_tmp = rhs, div_tmp, remainder;
  int cnt = (kNumbers + 1) * 8;
  bool flag_rhs = false;
  bool flag_lhs = false;

  if (rhs == from_int(0)) {
    std::cerr << "Zero division";
    exit(EXIT_FAILURE);
  }

  if (rhs == from_int(1))
    return lhs;
  if (rhs == from_int(-1))
    return -lhs;

  if ((rhs.bytes[0] & (1 << 7)) != 0) {
    rhs_tmp = -rhs_tmp;
    flag_rhs = true;
  }
  if ((lhs.bytes[0] & (1 << 7)) != 0) {
    lhs_tmp = -lhs_tmp;
    flag_lhs = true;
  }
  if (((lhs_tmp - rhs_tmp).bytes[0] >> 7 == 1))
    return from_int(0);
  if (((lhs_tmp - rhs_tmp - rhs_tmp).bytes[0] >> 7) == 1)
    return (flag_lhs == flag_rhs ? from_int(1) : from_int(-1));

  while (cnt > 0) {
    res = res << 1;
    remainder = remainder << 1;
    remainder.bytes[kNumbers] |= lhs_tmp.bytes[0] >> 7;
    lhs_tmp = lhs_tmp << 1;
    if (remainder >= rhs_tmp) {
      res.bytes[kNumbers] |= 1;
      remainder = remainder - rhs_tmp;
    } else {
      res.bytes[kNumbers] |= 0;
    }
    --cnt;
  }
  bool k = remainder >= rhs_tmp;
  if (flag_rhs != flag_lhs)
    res = -res;

  return res;
}

bool operator<(const int2023_t& lhs, const int2023_t& rhs) {
  for (int i = 0; i <= kNumbers; ++i) {
    if (lhs.bytes[i] >= rhs.bytes[i])
      return false;
  }

  if (lhs == rhs)
    return false;

  return true;
}

bool operator==(const int2023_t& lhs, const int2023_t& rhs) {
  for (int i = kNumbers; i >= 0; i--)
    if (lhs.bytes[i] != rhs.bytes[i])
      return false;

  return true;
}

bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {
  if (lhs == rhs)
    return false;

  return true;
}

std::ostream& operator<<(std::ostream& stream, const int2023_t& value) {
  int2023_t res = value;
  int amount = 0;

  if (value.bytes[0] & (1 << 7)) {
    res = -res;
    res.bytes[0] += 1 << 7;
  }

  for (int i = 0; i <= kNumbers; ++i) {
    amount = 7;
    while (amount >= 0) {
      stream << ((res.bytes[i] >> amount) & 1) ;
      --amount;
    }
  }

  return stream;
}
