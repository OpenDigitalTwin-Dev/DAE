/*
 *          Copyright Carlos BRITO PACHECO 2021 - 2022.
 * Distributed under the Boost Software License, Version 1.0.
 *       (See accompanying file LICENSE or copy at
 *          https://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef RODIN_VARIATIONAL_P0_FORWARDDECLS_H
#define RODIN_VARIATIONAL_P0_FORWARDDECLS_H

#include "Rodin/Types.h"
#include "Rodin/Math/Vector.h"
#include "Rodin/Variational/ForwardDecls.h"

namespace Rodin::Variational
{

  /**
   * @brief Degree 0 Lagrange element
   * @tparam Range Range value type
   *
   * @note For an overview of all the possible specializations of the
   * P0 class, please see @ref P0Specializations.
   *
   * @see P0ElementSpecializations
   */
  template <class Range>
  class P0Element;

  /**
   * @brief Degree 0 Lagrange finite element space
   * @tparam Range Range value type
   * @tparam Context Context type
   * @tparam Args Additional arguments
   *
   * Represents the finite element space composed of discontinuous, piecewise
   * constant functions:
   * @f[
   *  \mathbb{P}_0 (\mathcal{T}_h)^d = \{ v : \mathcal{T}_h \rightarrow \mathbb{R}^d : v|_{\tau} \in \mathbb{P}_0(\tau), \ \tau \in \mathcal{T}_h \} \ ,
   * @f]
   * for a given vector dimension @f$ d \in \mathbb{N} @f$.
   *
   * @note For an overview of all the possible specializations of the
   * P0 class, please see @ref P0Specializations.
   *
   * @see P0Specializations
   */
  template <class Range, class Mesh>
  class P0;

  /**
   * @ingroup GridFunctionSpecializations
   * @brief GridFunction on the P0 finite element space.
   */
  template <class Range, class Mesh>
  class GridFunction<P0<Range, Mesh>>;

  /**
   * @brief Alias for P0Element<Real>
   */
  using RealP0Element = P0Element<Real>;

  /**
   * @brief Alias for P0Element<Complex>
   */
  using ComplexP0Element = P0Element<Complex>;

  /**
   * @brief Alias for P0Element<Math::Vector<Real>>
   */
  using VectorP0Element = P0Element<Math::Vector<Real>>;
}

#endif

