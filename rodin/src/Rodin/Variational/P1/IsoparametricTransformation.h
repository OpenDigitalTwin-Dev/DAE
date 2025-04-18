/*
 *          Copyright Carlos BRITO PACHECO 2021 - 2022.
 * Distributed under the Boost Software License, Version 1.0.
 *       (See accompanying file LICENSE or copy at
 *          https://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef RODIN_VARIATIONAL_P1_ISOPARAMETRICTRANSFORMATION_H
#define RODIN_VARIATIONAL_P1_ISOPARAMETRICTRANSFORMATION_H

#include <Eigen/QR>

#include "Rodin/Geometry/Polytope.h"
#include "Rodin/Geometry/PolytopeTransformation.h"

#include "P1Element.h"

#include "ForwardDecls.h"

// namespace Rodin::Geometry
// {
//   /**
//    * @brief Polytope isoparametric transformation.
//    */
//   template <>
//   class IsoparametricTransformation<Variational::RealP1Element> final
//     : public PolytopeTransformation
//   {
//     public:
//       using Parent = PolytopeTransformation;
// 
//       IsoparametricTransformation(Math::PointMatrix&& pm, Variational::RealP1Element&& fe)
//         : m_pm(std::move(pm)),
//           m_sdim(m_pm.rows()),
//           m_fe(std::move(fe)),
//           m_rdim(Polytope::getGeometryDimension(m_fe.getGeometry()))
//       {
//         assert(m_pm.cols() >= 0);
//         assert(static_cast<size_t>(m_pm.cols()) == m_fe.getCount());
//       }
// 
//       /**
//        * pm : sdim x dof
//        */
//       IsoparametricTransformation(const Math::Matrix<Real>& pm, const Variational::RealP1Element& fe)
//         : m_pm(pm),
//           m_sdim(m_pm.rows()),
//           m_fe(fe),
//           m_rdim(Polytope::getGeometryDimension(m_fe.getGeometry()))
//       {
//         assert(m_pm.cols() >= 0);
//         assert(static_cast<size_t>(m_pm.cols()) == m_fe.getCount());
//       }
// 
//       IsoparametricTransformation(Math::Matrix<Real>&& pm, const Variational::RealP1Element& fe)
//         : m_pm(std::move(pm)),
//           m_sdim(m_pm.rows()),
//           m_fe(fe),
//           m_rdim(Polytope::getGeometryDimension(m_fe.getGeometry()))
//       {
//         assert(m_pm.cols() >= 0);
//         assert(static_cast<size_t>(m_pm.cols()) == m_fe.getCount());
//       }
// 
//       IsoparametricTransformation(const Math::Matrix<Real>& pm, Variational::RealP1Element&& fe)
//         : m_pm(pm),
//           m_sdim(m_pm.rows()),
//           m_fe(std::move(fe)),
//           m_rdim(Polytope::getGeometryDimension(m_fe.getGeometry()))
//       {
//         assert(m_pm.cols() >= 0);
//         assert(static_cast<size_t>(m_pm.cols()) == m_fe.getCount());
//       }
// 
//       IsoparametricTransformation(const IsoparametricTransformation& other)
//         : Parent(other),
//           m_pm(other.m_pm),
//           m_sdim(m_pm.rows()),
//           m_fe(other.m_fe),
//           m_rdim(Polytope::getGeometryDimension(m_fe.getGeometry()))
//       {
//         assert(m_pm.cols() >= 0);
//         assert(static_cast<size_t>(m_pm.cols()) == m_fe.getCount());
//       }
// 
//       IsoparametricTransformation(IsoparametricTransformation&& other)
//         : Parent(std::move(other)),
//           m_pm(std::move(other.m_pm)),
//           m_sdim(m_pm.rows()),
//           m_fe(std::move(other.m_fe)),
//           m_rdim(Polytope::getGeometryDimension(m_fe.getGeometry()))
//       {
//         assert(m_pm.cols() >= 0);
//         assert(static_cast<size_t>(m_pm.cols()) == m_fe.getCount());
//       }
// 
//       inline
//       Math::SpatialVector<Real> transform(const Math::SpatialVector<Real>& rc) const override
//       {
//         Math::Vector<Real> res = Math::Vector<Real>::Zero(m_sdim);
//         for (size_t local = 0; local < m_fe.getCount(); local++)
//         {
//           assert(res.size() == m_pm.col(local).size());
//           res.noalias() += m_pm.col(local) * m_fe.getBasis(local)(rc);
//         }
//         return res;
//       }
// 
//       inline
//       void jacobian(const Math::SpatialVector<Real>& rc, Math::SpatialMatrix<Real>& res) override
//       {
//         res = Math::SpatialMatrix<Real>::Zero(m_sdim, m_rdim);
//         Math::SpatialVector<Real> gradient;
//         for (size_t local = 0; local < m_fe.getCount(); local++)
//         {
//           m_fe.getGradient(local)(gradient, rc);
//           for (size_t i = 0; i < m_rdim; i++)
//           {
//             assert(res.col(i).size() == m_pm.col(local).size());
//             res.col(i).noalias() += m_pm.col(local) * gradient.coeff(i);
//           }
//         }
//         return res;
//       }
// 
//       inline
//       const Math::PointMatrix& getPointMatrix() const
//       {
//         return m_pm;
//       }
// 
//     private:
//       Math::PointMatrix m_pm;
//       const size_t m_sdim;
//       Variational::RealP1Element m_fe;
//       const size_t m_rdim;
//   };
// }

#endif

