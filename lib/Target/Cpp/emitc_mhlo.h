// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// This file defines functions used by EmitC

#ifndef EMITC_MHLO_H
#define EMITC_MHLO_H

#include <algorithm>
#include <cmath>
#include <complex>
#include <cstdint>
#include <functional>
#include <random>
#include <vector>

namespace mhlo {
/// See
/// https://github.com/tensorflow/tensorflow/blob/6f59650012f8904745dffaba540afc794c6613be/tensorflow/compiler/xla/service/hlo_evaluator.cc
/// for the XLA implementation

/// Functions for MHLO unary elementwise ops
// AbsOp
template <typename T1, typename T2>
inline T1 abs(T2 x) {
  return std::abs(x);
}

template <typename T>
inline std::vector<T> abs(std::vector<T> x) {
  std::vector<T> z(x);
  for (size_t i = 0; i < z.size(); i++) {
    z[i] = std::abs(x[i]);
  }
  return z;
}

// AbsOp supports complex to real.
template <typename T>
inline std::vector<T> abs(std::vector<std::complex<T>> x) {
  std::vector<T> z;
  z.reserve(x.size());
  for (size_t i = 0; i < z.size(); i++) {
    z[i] = std::abs(x[i]);
  }
  return z;
}

// BitcastConvertOp
template <typename T1, typename T2>
inline T1 bitcast_convert(T2 x) {
  return reinterpret_cast<T1>(x);
}

template <typename T1, typename T2>
inline std::vector<T1> bitcast_convert(std::vector<T2> x) {
  std::vector<T1> z(x.size());
  for (size_t i = 0; i < z.size(); i++) {
    z[i] = reinterpret_cast<T1>(x[i]);
  }
  return z;
}

// CompareOp
template <typename T, template <typename> class Compare>
std::vector<bool> compare(std::vector<T> x, std::vector<T> y) {
  std::vector<bool> z(x.size());
  std::transform(x.begin(), x.end(), y.begin(), z.begin(), Compare<T>());
  return z;
}

// ConvertOp
template <typename T1, typename T2>
inline T1 convert(T2 x) {
  return static_cast<T1>(x);
}

template <typename T1, typename T2>
inline std::vector<T1> convert(std::vector<T2> x) {
  std::vector<T1> z(x.size());
  for (size_t i = 0; i < z.size(); i++) {
    z[i] = static_cast<T1>(x[i]);
  }
  return z;
}

// CosOp
template <typename T>
inline T cos(T x) {
  return std::cos(x);
}

template <typename T>
inline std::vector<T> cos(std::vector<T> x) {
  std::vector<T> z(x);
  for (size_t i = 0; i < z.size(); i++) {
    z[i] = std::cos(x[i]);
  }
  return z;
}

// ExpOp
// TODO: Implement!
// `e^(operand)` element-wise

// IsFiniteOp
// TODO: Implement!

// LogOp
// TODO: Implement!

// NegOp
// TODO: Implement!

// SinOp
template <typename T>
inline T sin(T x) {
  return std::sin(x);
}

template <typename T>
inline std::vector<T> sin(std::vector<T> x) {
  std::vector<T> z(x);
  for (size_t i = 0; i < z.size(); i++) {
    z[i] = std::sin(x[i]);
  }
  return z;
}

// SqrtOp
template <typename T>
inline T sqrt(T x) {
  return std::sqrt(x);
}

template <typename T>
inline std::vector<T> sqrt(std::vector<T> x) {
  std::vector<T> z(x);
  for (size_t i = 0; i < z.size(); i++) {
    z[i] = std::sqrt(x[i]);
  }
  return z;
}

/// Functions for MHLO binary elementwise ops.
// AddOp
template <typename T>
inline T add(T x, T y) {
  return std::plus<>{}(x, y);
}

template <typename T>
inline std::vector<T> add(std::vector<T> x, std::vector<T> y) {
  std::vector<T> z(x);
  std::transform(x.begin(), x.end(), y.begin(), z.begin(), std::plus<>());
  return z;
}

// DivOp
template <typename T>
inline T div(T x, T y) {
  return std::divides<>{}(x, y);
}

template <typename T>
inline std::vector<T> div(std::vector<T> x, std::vector<T> y) {
  std::vector<T> z(x);
  std::transform(x.begin(), x.end(), y.begin(), z.begin(), std::divides<>());
  return z;
}

// MaxOp
template <typename T>
inline T max(T x, T y) {
  return std::max(x, y);
}

template <typename T>
inline std::vector<T> max(std::vector<T> x, std::vector<T> y) {
  std::vector<T> z(x);
  std::transform(x.begin(), x.end(), y.begin(), z.begin(),
                 [](auto a, auto b) { return std::max(a, b); });
  return z;
}

// MinOp
template <typename T>
inline T min(T x, T y) {
  return std::min(x, y);
}

template <typename T>
inline std::vector<T> min(std::vector<T> x, std::vector<T> y) {
  std::vector<T> z(x);
  std::transform(x.begin(), x.end(), y.begin(), z.begin(),
                 [](auto a, auto b) { return std::min(a, b); });
  return z;
}

// MulOp
template <typename T>
inline T mul(T x, T y) {
  return std::multiplies<>{}(x, y);
}

template <typename T>
inline std::vector<T> mul(std::vector<T> x, std::vector<T> y) {
  std::vector<T> z(x);
  std::transform(x.begin(), x.end(), y.begin(), z.begin(), std::multiplies<>());
  return z;
}

// PowOp
template <typename T>
inline T pow(T x, T y) {
  return std::pow(x, y);
}

template <typename T>
inline std::vector<T> pow(std::vector<T> x, std::vector<T> y) {
  std::vector<T> z(x);
  std::transform(x.begin(), x.end(), y.begin(), z.begin(),
                 [](auto a, auto b) { return std::pow(a, b); });
  return z;
}

// ShiftLeftOp
// TODO: Implement!

// ShiftRightLogicalOp
// TODO: Implement!

// SubOp
template <typename T>
inline T sub(T x, T y) {
  return std::minus<>{}(x, y);
}

template <typename T>
inline std::vector<T> sub(std::vector<T> x, std::vector<T> y) {
  std::vector<T> z(x);
  std::transform(x.begin(), x.end(), y.begin(), z.begin(), std::minus<>());
  return z;
}

/// Functions for MHLO binary logical elementwise ops.
// OrOp
// TODO: Implement!

// XorOp
// TODO: Implement!

/// Functions for other MHLO ops.
// BroadcastInDimOp
template <typename T>
inline std::vector<T> broadcast_in_dim(std::vector<T> x, size_t n) {
  std::vector<T> z;

  for (size_t i = 0; i < n; i++) {
    z.insert(z.end(), x.begin(), x.end());
  }

  return z;
}

// ConcatenateOp
template <typename T>
inline std::vector<T> concatenate(std::vector<T> x, std::vector<T> y) {
  std::vector<T> z(x);
  z.insert(z.end(), y.begin(), y.end());
  return z;
}

// ReshapeOp
// This needs to be changed if tensor rank/shape get modelled in the translation
template <typename T>
inline std::vector<T> reshape(std::vector<T> x) {
  return std::vector(x);
}

// SelectOp
template <typename T>
inline std::vector<T> select(std::vector<bool> s, std::vector<T> x,
                             std::vector<T> y) {
  std::vector<T> z(x.size());
  for (size_t i = 0; i < z.size(); i++) {
    z[i] = s[i] ? x[i] : y[i];
  }
  return z;
}

// RngUniformOp
template <typename T>
using IsIntegral =
    typename std::enable_if<std::is_integral<T>::value, bool>::type;
template <typename T>
using IsFloatingPoint =
    typename std::enable_if<std::is_floating_point<T>::value, bool>::type;

// integer types
template <typename T, IsIntegral<T> = true>
std::vector<T> rng_uniform(T low, T high, std::vector<int64_t> shape) {
  int64_t n = std::accumulate(shape.begin(), shape.end(), 1,
                              std::multiplies<int64_t>());

  std::random_device rd;
  std::mt19937 gen(rd());
  // high value is exclusive in xla but incluseive in cpp
  // see https://www.tensorflow.org/xla/operation_semantics?hl=en#rnguniform and
  // https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
  std::uniform_int_distribution<T> distribution(low, high - 1);
  std::vector<T> result(n);
  for (size_t i = 0; i < n; i++) {
    result[i] = distribution(gen);
  }
  return result;
}

// floating point types
template <typename T, IsFloatingPoint<T> = true>
std::vector<T> rng_uniform(T low, T high, std::vector<int64_t> shape) {
  int64_t n = std::accumulate(shape.begin(), shape.end(), 1,
                              std::multiplies<int64_t>());

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<T> distribution(low, high);
  std::vector<T> result(n);
  for (size_t i = 0; i < n; i++) {
    result[i] = distribution(gen);
  }
  return result;
}

// RngBitGeneratorOp
template <typename T, int32_t Algorithm, int64_t N>
std::tuple<std::vector<uint64_t>, std::vector<T>>
rng_bit_generator(std::vector<uint64_t> state) {
  // TODO implement correct algorithm; starting point would be
  // https://github.com/tensorflow/tensorflow/blob/6f59650012f8904745dffaba540afc794c6613be/tensorflow/compiler/xla/service/rng_bit_generator_expander.cc#L56
  std::vector<uint64_t> newState(state);
  std::vector<int64_t> shape{N};

  T min = std::numeric_limits<T>::min();
  T max = std::numeric_limits<T>::max();
  std::vector<T> resultVector = rng_uniform<T>(min, max, shape);

  return std::make_tuple(newState, resultVector);
}

} // namespace mhlo

#endif // EMITC_MHLO_H
