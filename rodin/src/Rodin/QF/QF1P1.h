#ifndef RODIN_VARIATIONAL_QF_QF1P1_H
#define RODIN_VARIATIONAL_QF_QF1P1_H

#include "Rodin/Geometry/GeometryIndexed.h"

#include "QuadratureFormula.h"

namespace Rodin::QF
{
  /**
   * @ingroup RodinQuadrature
   */
  class QF1P1 final : public QuadratureFormulaBase
  {
    public:
      using Parent = QuadratureFormulaBase;

      constexpr
      QF1P1(Geometry::Polytope::Type g)
        : Parent(g)
      {}

      size_t getSize() const override
      {
        return 1;
      }

      const Math::SpatialVector<Real>& getPoint(size_t i) const override
      {
        assert(i == 0);
        return s_points[getGeometry()];
      }

      Real getWeight(size_t i) const override
      {
        assert(i == 0);
        return s_weights[getGeometry()];
      }

      QF1P1* copy() const noexcept override
      {
        return new QF1P1(*this);
      }

    private:
      static const Geometry::GeometryIndexed<Math::SpatialVector<Real>> s_points;
      static const Geometry::GeometryIndexed<Real> s_weights;
  };
}

#endif
