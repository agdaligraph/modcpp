#ifndef org_modcpp_math_linear
#define org_modcpp_math_linear

#include "org/eigen/Core"
#include "org/modcpp/math/numbers.h"

namespace org::modcpp::math {

  template<typename FieldT>
  using Vector = Eigen::Matrix<FieldT, Eigen::Dynamic, 1>;

  template<typename FieldT>
  using Matrix = Eigen::Matrix<FieldT, Eigen::Dynamic, Eigen::Dynamic>;
  
  typedef Vector<ℤ> VectorZ;
  typedef Matrix<ℤ> MatrixZ;
  typedef Vector<ℚ> VectorQ;
  typedef Matrix<ℚ> MatrixQ;
  
}

#endif
