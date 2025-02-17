/*
 *          Copyright Carlos BRITO PACHECO 2021 - 2022.
 * Distributed under the Boost Software License, Version 1.0.
 *       (See accompanying file LICENSE or copy at
 *          https://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef RODIN_MATH_SPARSEMATRIX_H
#define RODIN_MATH_SPARSEMATRIX_H

#include <Eigen/Sparse>

#include "Rodin/Types.h"

#include "ForwardDecls.h"

namespace Rodin::Math
{
  /**
   * @brief Sparse matrix type
   */
  template <class ScalarType>
  using SparseMatrix = Eigen::SparseMatrix<ScalarType>;
}

namespace Rodin::FormLanguage
{
  template <class Number>
  struct Traits<Math::SparseMatrix<Number>>
  {
    using ScalarType = Number;
  };
}

#endif

