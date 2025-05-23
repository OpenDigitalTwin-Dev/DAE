/*
 *          Copyright Carlos BRITO PACHECO 2021 - 2022.
 * Distributed under the Boost Software License, Version 1.0.
 *       (See accompanying file LICENSE or copy at
 *          https://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef RODIN_VARIATIONAL_ZERO_H
#define RODIN_VARIATIONAL_ZERO_H

#include <cmath>

#include <Rodin/Math/Common.h>

#include "ForwardDecls.h"

#include "RangeShape.h"
#include "Function.h"
#include "RealFunction.h"
#include "VectorFunction.h"

namespace Rodin::Variational
{
  /**
   * @defgroup ZeroSpecializations Zero Template Specializations
   * @brief Template specializations of the Zero class.
   * @see Zero
   */

  template <class Scalar>
  class Zero<Scalar> final
    : public ScalarFunctionBase<Scalar, Zero<Scalar>>
  {
    public:
      using ScalarType = Scalar;

      using Parent = ScalarFunctionBase<Scalar, Zero<Scalar>>;

      Zero() {}

      Zero(const Zero& other)
        : Parent(other)
      {}

      Zero(Zero&& other)
        : Parent(std::move(other))
      {}

      inline
      constexpr
      Zero& traceOf(Geometry::Attribute attrs)
      {
        return *this;
      }

      inline
      constexpr
      auto getValue(const Geometry::Point&) const
      {
        return ScalarType(0);
      }

      inline Zero* copy() const noexcept override
      {
        return new Zero(*this);
      }
  };

  /**
   * @brief CTAD for Zero.
   */
  Zero() -> Zero<Real>;

  template <class Scalar>
  class Zero<Math::Vector<Scalar>> final
    : public VectorFunctionBase<Scalar, Zero<Math::Vector<Scalar>>>
  {
    public:
      using VectorType = Math::Vector<Scalar>;

      using Parent = VectorFunctionBase<Scalar, Zero<VectorType>>;

      Zero(size_t d)
        : m_d(d)
      {}

      Zero(const Zero& other)
        : Parent(other),
          m_d(other.m_d)
      {}

      Zero(Zero&& other)
        : Parent(std::move(other)),
          m_d(std::move(other.m_d))
      {}

      inline
      constexpr
      Zero& traceOf(Geometry::Attribute attrs)
      {
        return *this;
      }

      inline
      auto getValue(const Geometry::Point&) const
      {
        return VectorType::Zero(m_d);
      }

      inline
      void getValue(VectorType& out, const Geometry::Point&) const
      {
        out.resize(m_d);
        out.setZero();
      }

      inline Zero* copy() const noexcept override
      {
        return new Zero(*this);
      }

    private:
      const size_t m_d;
  };

  Zero(size_t) -> Zero<Math::Vector<Real>>;

  template <class Scalar>
  using VectorZero = Zero<Math::Vector<Scalar>>;
}

#endif

