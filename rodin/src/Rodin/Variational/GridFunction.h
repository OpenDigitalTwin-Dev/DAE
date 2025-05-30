/*
 *          Copyright Carlos BRITO PACHECO 2021 - 2022.
 * Distributed under the Boost Software License, Version 1.0.
 *       (See accompanying file LICENSE or copy at
 *          https://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef RODIN_VARIATIONAL_GRIDFUNCTION_H
#define RODIN_VARIATIONAL_GRIDFUNCTION_H

#include <cmath>
#include <utility>
#include <fstream>
#include <functional>
#include <boost/filesystem.hpp>
#include <type_traits>

#include "Rodin/Configure.h"
#include "Rodin/Math.h"
#include "Rodin/Alert.h"
#include "Rodin/Geometry/Point.h"
#include "Rodin/Geometry/SubMesh.h"
#include "Rodin/IO/ForwardDecls.h"
#include "Rodin/IO/MFEM.h"
#include "Rodin/IO/MEDIT.h"
#include "Rodin/QF/GenericPolytopeQuadrature.h"

#include "Rodin/Threads/ThreadPool.h"

#include "ForwardDecls.h"

#include "Function.h"
#include "Component.h"
#include "Restriction.h"
#include "LazyEvaluator.h"
#include "ScalarFunction.h"
#include "VectorFunction.h"
#include "MatrixFunction.h"
#include "FiniteElementSpace.h"

namespace Rodin::FormLanguage
{
  template <class FES, class Derived>
  struct Traits<Variational::GridFunctionBase<FES, Derived>>
  {
    using FESType = FES;
    using MeshType = typename Traits<FESType>::MeshType;
    using RangeType = typename Traits<FESType>::RangeType;
    using ElementType = typename Traits<FESType>::ElementType;
    using ContextType = typename Traits<FESType>::ContextType;
    using ScalarType = typename FormLanguage::Traits<FESType>::ScalarType;
  };
}

namespace Rodin::Variational
{
  /**
   * @defgroup GridFunctionSpecializations GridFunction Template Specializations
   * @brief Template specializations of the GridFunction class.
   * @see GridFunction
   */

  /**
   * @brief Abstract base class for GridFunction objects.
   *
   * This class contains the common routines for the behaviour of a
   * GridFunction object. It provides a common interface for the manipulation
   * of its data and weights, as well as projection utilities and convenience
   * functions.
   *
   * @section gridfunction-data-layout Data layout
   *
   * The data of the GridFunctionBase object can be accessed via a call to @ref
   * getData(). The i-th column of the returned Math::Matrix object corresponds
   * to the value of the grid function at the global i-th degree of freedom in
   * the finite element space. Furthermore, the following conditions are
   * satisfied:
   * ```
   *  const auto& data = gf.getData();
   *  assert(data.rows() == gf.getFiniteElementSpace().getVectorDimension());
   *  assert(data.cols() == gf.getFiniteElementSpace().getSize());
   * ```
   */
  template <class FES, class Derived>
  class GridFunctionBase : public LazyEvaluator<GridFunctionBase<FES, Derived>>
  {
    public:
      using FESType = FES;

      using ScalarType = typename FormLanguage::Traits<FESType>::ScalarType;

      /// Range type of value
      using RangeType = typename FormLanguage::Traits<FESType>::RangeType;

      /// Type of mesh on which the finite element space is built
      using MeshType = Geometry::Mesh<Context::Sequential>;

      /// Represents the Context of the P1 space
      using ContextType = Context::Sequential;

      /// Type of finite element
      using ElementType = typename FormLanguage::Traits<FESType>::ElementType;

      using DataType = Math::Matrix<ScalarType>;

      using WeightVectorType = Math::Vector<ScalarType>;

      /// Parent class
      using Parent = LazyEvaluator<GridFunctionBase<FESType, Derived>>;

      static_assert(std::is_same_v<RangeType, ScalarType> ||
                    std::is_same_v<RangeType, Math::Vector<ScalarType>>);

      GridFunctionBase(const FES& fes)
        : Parent(std::cref(*this)),
          m_fes(fes),
          m_data(fes.getVectorDimension(), fes.getSize())
      {
        m_data.setZero();
      }

      GridFunctionBase(const GridFunctionBase& other)
        : Parent(std::cref(*this)),
          m_fes(other.m_fes),
          m_data(other.m_data),
          m_weights(other.m_weights)
      {}

      GridFunctionBase(GridFunctionBase&& other)
        : Parent(std::cref(*this)),
          m_fes(std::move(other.m_fes)),
          m_data(std::move(other.m_data)),
          m_weights(std::move(other.m_weights))
      {}

      GridFunctionBase& operator=(GridFunctionBase&& other)
      {
        m_fes = std::move(other.m_fes);
        m_data = std::move(other.m_data);
        m_weights = std::move(other.m_weights);
        return *this;
      }

      GridFunctionBase& operator=(const GridFunctionBase&) = delete;

      /**
       * @brief Searches for the maximum value in the grid function data.
       * @returns Maximum value in grid function.
       *
       * This function will compute the maximum value in the grid function
       * data array.
       *
       * @section Complexity
       * The operation is linear in the size of the number of entries in the
       * underlying matrix.
       */
      inline
      constexpr
      ScalarType max() const
      {
        return m_data.maxCoeff();
      }

      inline
      constexpr
      ScalarType max(Index& idx) const
      {
        return m_data.maxCoeff(&idx);
      }

      /**
       * @brief Searches the minimum value in the grid function data.
       * @returns Minimum value in grid function.
       *
       * This function will compute the minimum value in the grid function
       * data array.
       *
       * @section Complexity
       * The operation is linear in the size of the number of entries in the
       * underlying matrix.
       */
      inline
      constexpr
      ScalarType min() const
      {
        return m_data.minCoeff();
      }

      inline
      constexpr
      ScalarType min(Index& idx) const
      {
        return m_data.minCoeff(&idx);
      }

      inline
      constexpr
      Index argmax() const
      {
        Index idx = 0;
        m_data.maxCoeff(&idx);
        return idx;
      }

      inline
      constexpr
      Index argmin() const
      {
        Index idx = 0;
        m_data.minCoeff(&idx);
        return idx;
      }

      inline
      Derived& normalize()
      {
        static_assert(std::is_same_v<RangeType, Math::Vector<ScalarType>>,
            "GridFunction must be vector valued.");
        for (size_t i = 0; i < getSize(); i++)
          getData().col(i).normalize();
        return static_cast<Derived&>(*this);
      }

      inline
      Derived& stableNormalize()
      {
        static_assert(std::is_same_v<RangeType, Math::Vector<ScalarType>>,
            "GridFunction must be vector valued.");
        for (size_t i = 0; i < getSize(); i++)
          getData().col(i).stableNormalize();
        return static_cast<Derived&>(*this);
      }

      inline
      constexpr
      size_t getDimension() const
      {
        return getFiniteElementSpace().getVectorDimension();
      }

      inline
      constexpr
      auto x() const
      {
        static_assert(std::is_same_v<RangeType, Math::Vector<ScalarType>>);
        assert(getFiniteElementSpace().getVectorDimension() >= 1);
        return Component(*this, 0);
      }

      inline
      constexpr
      auto y() const
      {
        static_assert(std::is_same_v<RangeType, Math::Vector<ScalarType>>);
        assert(getFiniteElementSpace().getVectorDimension() >= 2);
        return Component(*this, 1);
      }

      inline
      constexpr
      auto z() const
      {
        static_assert(std::is_same_v<RangeType, Math::Vector<ScalarType>>);
        assert(getFiniteElementSpace().getVectorDimension() >= 3);
        return Component(*this, 2);
      }

      inline
      constexpr
      size_t getSize() const
      {
        return getFiniteElementSpace().getSize();
      }


      inline
      Derived& setZero()
      {
        m_data.setZero();
        if (m_weights)
          m_weights->setZero();
        return static_cast<Derived&>(*this);
      }

      /**
       * @brief Bulk assigns the value to the whole data array.
       */
      inline
      Derived& operator=(const RangeType& v)
      {
        if constexpr (std::is_same_v<RangeType, ScalarType>)
        {
          m_data.setConstant(v);
        }
        else if constexpr (std::is_same_v<RangeType, Math::Vector<ScalarType>>)
        {
          assert(m_data.cols() >= 0);
          for (size_t i = 0; i < static_cast<size_t>(m_data.cols()); i++)
            m_data.col(i) = v;
        }
        else
        {
          assert(false);
        }
        return static_cast<Derived&>(*this);
      }

      inline
      Derived& operator=(std::function<RangeType(const Geometry::Point&)> fn)
      {
        return project(fn);
      }

      inline
      Derived& operator=(std::function<void(RangeType&, const Geometry::Point&)> fn)
      {
        return project(fn);
      }

      /**
       * @brief Projection of a function.
       */
      template <class NestedDerived>
      inline
      Derived& operator=(const FunctionBase<NestedDerived>& fn)
      {
        return project(fn);
      }

      /**
       * @brief Addition of a scalar value.
       */
      inline
      Derived& operator+=(const ScalarType& rhs)
      {
        static_assert(std::is_same_v<RangeType, ScalarType>);
        m_data = m_data.array() + rhs;
        return static_cast<Derived&>(*this);
      }

      /**
       * @brief Substraction of a scalar value.
       */
      inline
      Derived& operator-=(const ScalarType& rhs)
      {
        static_assert(std::is_same_v<RangeType, ScalarType>);
        m_data = m_data.array() - rhs;
        return static_cast<Derived&>(*this);
      }

      /**
       * @brief Multiplication by a scalar value.
       */
      inline
      Derived& operator*=(const ScalarType& rhs)
      {
        m_data = m_data.array() * rhs;
        return static_cast<Derived&>(*this);
      }

      /**
       * @brief Division by a scalar value.
       */
      inline
      Derived& operator/=(const ScalarType& rhs)
      {
        m_data = m_data.array() / rhs;
        return static_cast<Derived&>(*this);
      }

      inline
      Derived& operator+=(const GridFunctionBase& rhs)
      {
        if (this == &rhs)
        {
          operator*=(2);
        }
        else
        {
          assert(&getFiniteElementSpace() == &rhs.getFiniteElementSpace());
          m_data = m_data.array() + rhs.m_data.array();
        }
        return static_cast<Derived&>(*this);
      }

      inline
      Derived& operator-=(const GridFunctionBase& rhs)
      {
        if (this == &rhs)
        {
          m_data.setZero();
        }
        else
        {
          assert(&getFiniteElementSpace() == &rhs.getFiniteElementSpace());
          m_data = m_data.array() - rhs.m_data.array();
        }
        return static_cast<Derived&>(*this);
      }

      inline
      Derived& operator*=(const GridFunctionBase& rhs)
      {
        if (this == &rhs)
        {
          m_data = m_data.array() * m_data.array();
        }
        else
        {
          assert(&getFiniteElementSpace() == &rhs.getFiniteElementSpace());
          m_data = m_data.array() * rhs.m_data.array();
        }
        return static_cast<Derived&>(*this);
      }

      inline
      Derived& operator/=(const GridFunctionBase& rhs)
      {
        if (this == &rhs)
        {
          m_data.setOnes();
        }
        else
        {
          assert(&getFiniteElementSpace() == &rhs.getFiniteElementSpace());
          m_data = m_data.array() / rhs.m_data.array();
        }
        return static_cast<Derived&>(*this);
      }

      /**
       * @brief Projects a scalar valued function on the region of the mesh
       * with the given attribute.
       * @param[in] fn Scalar valued function
       * @param[in] attr Attribute
       */
      inline
      auto& project(
          std::function<RangeType(const Geometry::Point&)> fn, Geometry::Attribute attr)
      {
        return project(fn, FlatSet<Geometry::Attribute>{ attr });
      }

      inline
      auto& project(
          std::function<void(RangeType&, const Geometry::Point&)> fn, Geometry::Attribute attr)
      {
        return project(fn, FlatSet<Geometry::Attribute>{ attr });
      }

      inline
      auto& project(
          std::function<RangeType(const Geometry::Point&)> fn,
          const FlatSet<Geometry::Attribute>& attrs = {})
      {
        if constexpr (std::is_same_v<RangeType, ScalarType>)
        {
          assert(getFiniteElementSpace().getVectorDimension() == 1);
          return project(ScalarFunction(fn));
        }
        else if constexpr (std::is_same_v<RangeType, Math::Vector<ScalarType>>)
        {
          return project(VectorFunction(getFiniteElementSpace().getVectorDimension(), fn));
        }
        else
        {
          assert(false);
          return static_cast<Derived&>(*this);
        }
      }

      inline
      auto& project(
          std::function<void(RangeType&, const Geometry::Point&)> fn,
          const FlatSet<Geometry::Attribute>& attrs = {})
      {
        if constexpr (std::is_same_v<RangeType, ScalarType>)
        {
          assert(getFiniteElementSpace().getVectorDimension() == 1);
          return project(ScalarFunction(fn));
        }
        else if constexpr (std::is_same_v<RangeType, Math::Vector<ScalarType>>)
        {
          return project(VectorFunction(getFiniteElementSpace().getVectorDimension(), fn));
        }
        else
        {
          assert(false);
          return static_cast<Derived&>(*this);
        }
      }

      template <class NestedDerived>
      inline
      Derived& project(const FunctionBase<NestedDerived>& fn)
      {
        return project(fn, FlatSet<Geometry::Attribute>{});
      }

      /**
       * @brief Projects a FunctionBase instance
       *
       * This function will project a FunctionBase instance on the
       * domain elements with the given attribute.
       *
       * It is a convenience function to call
       * project(const FunctionBase&, const FlatSet<Geometry::Atribute>&) with one
       * attribute.
       */
      template <class NestedDerived>
      inline
      Derived& project(const FunctionBase<NestedDerived>& fn, Geometry::Attribute attr)
      {
        return project(fn, FlatSet<Geometry::Attribute>{attr});
      }

      /**
       * @brief Projects a FunctionBase instance on the grid function.
       *
       * This function will project a FunctionBase instance on the
       * domain elements with the given attributes. If the attribute set is
       * empty, this function will project over all elements in the mesh.
       */
      template <class NestedDerived>
      Derived& project(const FunctionBase<NestedDerived>& fn, const FlatSet<Geometry::Attribute>& attrs)
      {
        const auto& fes = getFiniteElementSpace();
        const auto& mesh = fes.getMesh();
        const size_t d = mesh.getDimension();
        std::vector<Real> ns(fes.getSize(), 0);
        if constexpr (std::is_same_v<RangeType, ScalarType>)
        {
#ifdef RODIN_MULTITHREADED
          auto& threadPool = Threads::getGlobalThreadPool();
          auto loop =
            [&](const Index start, const Index end)
            {
              const size_t capacity = fes.getSize() / threadPool.getThreadCount();
              std::vector<Index> is;
              is.reserve(capacity);
              std::vector<ScalarType> vs;
              vs.reserve(capacity);
              std::unique_ptr<FunctionBase<NestedDerived>> fnt(fn.copy());
              for (Index i = start; i < end; ++i)
              {
                const auto it = mesh.getCell(i);
                const auto& polytope = *it;
                if (attrs.size() == 0 || attrs.count(polytope.getAttribute()))
                {
                  const auto& i = polytope.getIndex();
                  const auto& fe = fes.getFiniteElement(d, i);
                  const auto& trans = mesh.getPolytopeTransformation(d, i);
                  for (size_t local = 0; local < fe.getCount(); local++)
                  {
                    const Geometry::Point p(polytope, trans, fe.getNode(local));
                    assert(m_data.rows() == 1);
                    is.push_back(fes.getGlobalIndex({ d, i }, local));
                    vs.push_back(fnt->getValue(p));
                  }
                }
              }
              assert(is.size() == vs.size());
              m_mutex.lock();
              for (Index i = 0; i < is.size(); i++)
              {
                const Index global = is[i];
                m_data(global) =
                  (vs[i] + ns[global] * m_data(global)) / (ns[global] + 1);
                ns[global] += 1;
              }
              m_mutex.unlock();
            };
          threadPool.pushLoop(0, mesh.getCellCount(), loop);
          threadPool.waitForTasks();
#else
          for (auto it = mesh.getCell(); !it.end(); ++it)
          {
            const auto& polytope = *it;
            if (attrs.size() == 0 || attrs.count(polytope.getAttribute()))
            {
              const auto& i = polytope.getIndex();
              const auto& fe = fes.getFiniteElement(d, i);
              const auto& trans = mesh.getPolytopeTransformation(d, i);
              for (size_t local = 0; local < fe.getCount(); local++)
              {
                const Geometry::Point p(polytope, trans, fe.getNode(local));
                const Index global = fes.getGlobalIndex({ d, i }, local);
                assert(m_data.rows() == 1);
                m_data(global) =
                  (fn.getValue(p) + ns[global] * m_data(global)) / (ns[global] + 1);
                ns[global] += 1;
              }
            }
          }
#endif
        }
        else if constexpr (std::is_same_v<RangeType, Math::Vector<ScalarType>>)
        {
          Math::Vector<ScalarType> value;
          for (auto it = mesh.getCell(); !it.end(); ++it)
          {
            const auto& polytope = *it;
            if (attrs.size() == 0 || attrs.count(polytope.getAttribute()))
            {
              const auto& i = polytope.getIndex();
              const auto& fe = fes.getFiniteElement(d, i);
              const auto& trans = mesh.getPolytopeTransformation(d, i);
              for (size_t local = 0; local < fe.getCount(); local++)
              {
                const Geometry::Point p(polytope, trans, fe.getNode(local));
                const Index global = fes.getGlobalIndex({ d, i }, local);
                fn.getDerived().getValue(value, p);
                m_data.col(global) =
                  (value + ns[global] * m_data.col(global)) / (ns[global] + 1);
                ns[global] += 1;
              }
            }
          }
        }
        else
        {
          assert(false);
        }
        return static_cast<Derived&>(*this);
      }

      inline
      auto& projectOnBoundary(
          std::function<RangeType(const Geometry::Point&)> fn, Geometry::Attribute attr)
      {
        return projectOnBoundary(fn, FlatSet<Geometry::Attribute>{attr});
      }

      inline
      auto& projectOnBoundary(
          std::function<void(RangeType&, const Geometry::Point&)> fn, Geometry::Attribute attr)
      {
        return projectOnBoundary(fn, FlatSet<Geometry::Attribute>{attr});
      }

      inline
      auto& projectOnBoundary(
          std::function<RangeType(const Geometry::Point&)> fn,
          const FlatSet<Geometry::Attribute>& attrs = {})
      {
        if constexpr (std::is_same_v<RangeType, ScalarType>)
        {
          assert(getFiniteElementSpace().getVectorDimension() == 1);
          return projectOnBoundary(ScalarFunction(fn));
        }
        else if constexpr (std::is_same_v<RangeType, Math::Vector<ScalarType>>)
        {
          return projectOnBoundary(VectorFunction(getFiniteElementSpace().getVectorDimension(), fn));
        }
        else
        {
          assert(false);
          return static_cast<Derived&>(*this);
        }
      }

      inline
      auto& projectOnBoundary(
          std::function<void(RangeType&, const Geometry::Point&)> fn,
          const FlatSet<Geometry::Attribute>& attrs = {})
      {
        if constexpr (std::is_same_v<RangeType, ScalarType>)
        {
          assert(getFiniteElementSpace().getVectorDimension() == 1);
          return projectOnBoundary(ScalarFunction(fn));
        }
        else if constexpr (std::is_same_v<RangeType, Math::Vector<ScalarType>>)
        {
          return projectOnBoundary(VectorFunction(getFiniteElementSpace().getVectorDimension(), fn));
        }
        else
        {
          assert(false);
          return static_cast<Derived&>(*this);
        }
      }

      template <class NestedDerived>
      inline
      Derived& projectOnBoundary(const FunctionBase<NestedDerived>& fn)
      {
        return static_cast<Derived&>(*this).projectOnBoundary(fn, FlatSet<Geometry::Attribute>{});
      }

      template <class NestedDerived>
      inline
      Derived& projectOnBoundary(const FunctionBase<NestedDerived>& fn, Geometry::Attribute attr)
      {
        return projectOnBoundary(fn, FlatSet<Geometry::Attribute>{attr});
      }

      template <class NestedDerived>
      Derived& projectOnBoundary(const FunctionBase<NestedDerived>& fn, const FlatSet<Geometry::Attribute>& attrs)
      {
        const auto& fes = getFiniteElementSpace();
        const auto& mesh = fes.getMesh();
        const size_t d = mesh.getDimension() - 1;
        std::vector<Real> ns(fes.getSize(), 0);
        for (auto it = mesh.getBoundary(); !it.end(); ++it)
        {
          const auto& polytope = *it;
          if (attrs.size() == 0 || attrs.count(polytope.getAttribute()))
          {
            const auto& i = polytope.getIndex();
            const auto& fe = fes.getFiniteElement(d, i);
            const auto& trans = mesh.getPolytopeTransformation(d, i);
            for (size_t local = 0; local < fe.getCount(); local++)
            {
              const Geometry::Point p(polytope, trans, fe.getNode(local));
              const Index global = fes.getGlobalIndex({ d, i }, local);
              if constexpr (std::is_same_v<RangeType, ScalarType>)
              {
                assert(m_data.rows() == 1);
                m_data(global) =
                  (fn.getValue(p) + ns[global] * m_data(global)) / (ns[global] + 1);
              }
              else if constexpr (std::is_same_v<RangeType, Math::Vector<ScalarType>>)
              {
                m_data.col(global) =
                  (fn.getValue(p) + ns[global] * m_data.col(global)) / (ns[global] + 1);
              }
              else
              {
                assert(false);
              }
              ns[global] += 1;
            }
          }
        }
        return static_cast<Derived&>(*this);
      }

      inline
      auto& projectOnFaces(
          std::function<RangeType(const Geometry::Point&)> fn, Geometry::Attribute attr)
      {
        return projectOnFaces(fn, FlatSet<Geometry::Attribute>{attr});
      }

      inline
      auto& projectOnFaces(
          std::function<void(RangeType&, const Geometry::Point&)> fn, Geometry::Attribute attr)
      {
        return projectOnFaces(fn, FlatSet<Geometry::Attribute>{attr});
      }

      inline
      auto& projectOnFaces(
          std::function<RangeType(const Geometry::Point&)> fn, const FlatSet<Geometry::Attribute>& attrs = {})
      {
        if constexpr (std::is_same_v<RangeType, ScalarType>)
        {
          assert(getFiniteElementSpace().getVectorDimension() == 1);
          return projectOnFaces(ScalarFunction(fn));
        }
        else if constexpr (std::is_same_v<RangeType, Math::Vector<ScalarType>>)
        {
          return projectOnFaces(VectorFunction(getFiniteElementSpace().getVectorDimension(), fn));
        }
        else
        {
          assert(false);
          return static_cast<Derived&>(*this);
        }
      }

      inline
      auto& projectOnFaces(
          std::function<void(RangeType&, const Geometry::Point&)> fn, const FlatSet<Geometry::Attribute>& attrs = {})
      {
        if constexpr (std::is_same_v<RangeType, ScalarType>)
        {
          assert(getFiniteElementSpace().getVectorDimension() == 1);
          return projectOnFaces(ScalarFunction(fn));
        }
        else if constexpr (std::is_same_v<RangeType, Math::Vector<ScalarType>>)
        {
          return projectOnFaces(VectorFunction(getFiniteElementSpace().getVectorDimension(), fn));
        }
        else
        {
          assert(false);
          return static_cast<Derived&>(*this);
        }
      }

      template <class NestedDerived>
      inline
      Derived& projectOnFaces(const FunctionBase<NestedDerived>& fn)
      {
        return static_cast<Derived&>(*this).projectOnFaces(fn, FlatSet<Geometry::Attribute>{});
      }

      template <class NestedDerived>
      inline
      Derived& projectOnFaces(const FunctionBase<NestedDerived>& fn, Geometry::Attribute attr)
      {
        return projectOnFaces(fn, FlatSet<Geometry::Attribute>{attr});
      }

      template <class NestedDerived>
      Derived& projectOnFaces(const FunctionBase<NestedDerived>& fn, const FlatSet<Geometry::Attribute>& attrs)
      {
        const auto& fes = getFiniteElementSpace();
        const auto& mesh = fes.getMesh();
        const size_t d = mesh.getDimension() - 1;
        std::vector<Real> ns(fes.getSize(), 0);
        for (auto it = mesh.getFace(); !it.end(); ++it)
        {
          const auto& polytope = *it;
          if (attrs.size() == 0 || attrs.count(polytope.getAttribute()))
          {
            const auto& i = polytope.getIndex();
            const auto& fe = fes.getFiniteElement(d, i);
            const auto& trans = mesh.getPolytopeTransformation(d, i);
            for (size_t local = 0; local < fe.getCount(); local++)
            {
              const Geometry::Point p(polytope, trans, fe.getNode(local));
              const Index global = fes.getGlobalIndex({ d, i }, local);
              if constexpr (std::is_same_v<RangeType, ScalarType>)
              {
                assert(m_data.rows() == 1);
                m_data(global) =
                  (fn.getValue(p) + ns[global] * m_data(global)) / (ns[global] + 1);
              }
              else if constexpr (std::is_same_v<RangeType, Math::Vector<ScalarType>>)
              {
                m_data.col(global) =
                  (fn.getValue(p) + ns[global] * m_data.col(global)) / (ns[global] + 1);
              }
              else
              {
                assert(false);
              }
              ns[global] += 1;
            }
          }
        }
        return static_cast<Derived&>(*this);
      }

      inline
      auto& projectOnInterfaces(
          std::function<RangeType(const Geometry::Point&)> fn, Geometry::Attribute attr)
      {
        return projectOnInterfaces(fn, FlatSet<Geometry::Attribute>{attr});
      }

      inline
      auto& projectOnInterfaces(
          std::function<void(RangeType&, const Geometry::Point&)> fn, Geometry::Attribute attr)
      {
        return projectOnInterfaces(fn, FlatSet<Geometry::Attribute>{ attr });
      }

      inline
      auto& projectOnInterfaces(
          std::function<RangeType(const Geometry::Point&)> fn, const FlatSet<Geometry::Attribute>& attrs = {})
      {
        if constexpr (std::is_same_v<RangeType, ScalarType>)
        {
          assert(getFiniteElementSpace().getVectorDimension() == 1);
          return projectOnInterfaces(ScalarFunction(fn));
        }
        else if constexpr (std::is_same_v<RangeType, Math::Vector<ScalarType>>)
        {
          return projectOnInterfaces(VectorFunction(getFiniteElementSpace().getVectorDimension(), fn));
        }
        else
        {
          assert(false);
          return static_cast<Derived&>(*this);
        }
      }

      inline
      auto& projectOnInterfaces(
          std::function<void(RangeType&, const Geometry::Point&)> fn, const FlatSet<Geometry::Attribute>& attrs = {})
      {
        if constexpr (std::is_same_v<RangeType, ScalarType>)
        {
          assert(getFiniteElementSpace().getVectorDimension() == 1);
          return projectOnInterfaces(ScalarFunction(fn));
        }
        else if constexpr (std::is_same_v<RangeType, Math::Vector<ScalarType>>)
        {
          return projectOnInterfaces(VectorFunction(getFiniteElementSpace().getVectorDimension(), fn));
        }
        else
        {
          assert(false);
          return static_cast<Derived&>(*this);
        }
      }

      template <class NestedDerived>
      inline
      Derived& projectOnInterfaces(const FunctionBase<NestedDerived>& fn)
      {
        return static_cast<Derived&>(*this).projectOnInterfaces(fn, FlatSet<Geometry::Attribute>{});
      }

      template <class NestedDerived>
      inline
      Derived& projectOnInterfaces(const FunctionBase<NestedDerived>& fn, Geometry::Attribute attr)
      {
        return projectOnInterfaces(fn, FlatSet<Geometry::Attribute>{attr});
      }

      template <class NestedDerived>
      Derived& projectOnInterfaces(
          const FunctionBase<NestedDerived>& fn, const FlatSet<Geometry::Attribute>& attrs)
      {
        const auto& fes = getFiniteElementSpace();
        const auto& mesh = fes.getMesh();
        const size_t d = mesh.getDimension() - 1;
        for (auto it = mesh.getInterface(); !it.end(); ++it)
        {
          const auto& polytope = *it;
          if (attrs.size() == 0 || attrs.count(polytope.getAttribute()))
          {
            const auto& i = polytope.getIndex();
            const auto& fe = fes.getFiniteElement(d, i);
            const auto& trans = mesh.getPolytopeTransformation(d, i);
            for (size_t local = 0; local < fe.getCount(); local++)
            {
              const Geometry::Point p(polytope, trans, fe.getNode(local));
              if constexpr (std::is_same_v<RangeType, ScalarType>)
              {
                assert(m_data.rows() == 1);
                m_data(fes.getGlobalIndex({ d, i }, local)) = fn.getValue(p);
              }
              else if constexpr (std::is_same_v<RangeType, Math::Vector<ScalarType>>)
              {
                m_data.col(fes.getGlobalIndex({ d, i }, local)) = fn.getValue(p);
              }
              else
              {
                assert(false);
              }
            }
          }
        }
        return static_cast<Derived&>(*this);
      }

      Derived& load(
          const boost::filesystem::path& filename, IO::FileFormat fmt = IO::FileFormat::MFEM)
      {
        std::ifstream input(filename.c_str());
        if (!input)
        {
          Alert::Exception()
            << "Failed to open " << filename << " for reading."
            << Alert::Raise;
        }

        switch (fmt)
        {
          case IO::FileFormat::MFEM:
          {
            IO::GridFunctionLoader<IO::FileFormat::MFEM, FES> loader(static_cast<Derived&>(*this));
            loader.load(input);
            break;
          }
          case IO::FileFormat::MEDIT:
          {
            IO::GridFunctionLoader<IO::FileFormat::MEDIT, FES> loader(static_cast<Derived&>(*this));
            loader.load(input);
            break;
          }
          default:
          {
            Alert::Exception()
              << "Loading from \"" << fmt << "\" format unsupported."
              << Alert::Raise;
          }
        }
        return static_cast<Derived&>(*this);
      }

      void save(
          const boost::filesystem::path& filename, IO::FileFormat fmt = IO::FileFormat::MFEM,
          size_t precision = RODIN_DEFAULT_GRIDFUNCTION_SAVE_PRECISION) const
      {
        std::ofstream output(filename.c_str());
        if (!output)
        {
          Alert::Exception()
            << "Failed to open " << filename << " for writing."
            << Alert::Raise;
        }

        output.precision(precision);
        switch (fmt)
        {
          case IO::FileFormat::MFEM:
          {
            IO::GridFunctionPrinter<IO::FileFormat::MFEM, FES> printer(static_cast<const Derived&>(*this));
            printer.print(output);
            break;
          }
          case IO::FileFormat::MEDIT:
          {
            IO::GridFunctionPrinter<IO::FileFormat::MEDIT, FES> printer(static_cast<const Derived&>(*this));
            printer.print(output);
            break;
          }
          default:
          {
            Alert::Exception()
              << "Saving to \"" << fmt << "\" format unsupported."
              << Alert::Raise;
          }
        }
        output.close();
      }

      inline
      constexpr
      const FES& getFiniteElementSpace() const
      {
        return m_fes.get();
      }

      /**
       * @brief Returns a constant reference to the GridFunction data.
       */
      template <class Matrix>
      inline
      constexpr
      Derived& setData(Matrix&& data) const
      {
        m_data = std::forward<Matrix>(data);
        return static_cast<Derived&>(*this).setData();
      }

      /**
       * @brief Returns a constant reference to the GridFunction data.
       */
      inline
      constexpr
      auto& getData()
      {
        return m_data;
      }

      /**
       * @brief Returns a constant reference to the GridFunction data.
       */
      inline
      constexpr
      const DataType& getData() const
      {
        return m_data;
      }

      inline
      constexpr
      std::optional<WeightVectorType>& getWeights()
      {
        return m_weights;
      }

      inline
      constexpr
      const std::optional<WeightVectorType>& getWeights() const
      {
        return m_weights;
      }

      /**
       * @brief Computes the weights from the data.
       * @note CRTP function to be overriden in Derived class.
       */
      inline
      Derived& setWeights()
      {
        return static_cast<Derived&>(*this).setWeights();
      }

      /**
       * @brief Sets the weights in the GridFunction object and computes the
       * values at all the degrees of freedom.
       * @note CRTP function to be overriden in Derived class.
       */
      template <class Vector>
      inline
      Derived& setWeights(Vector&& weights)
      {
        return static_cast<Derived&>(*this).setWeights(std::forward<Vector>(weights));
      }

      /**
       * @brief Sets the weights and data in the GridFunction object. No
       * computation is performed.
       */
      template <class Vector, class Matrix>
      inline
      Derived& setWeightsAndData(Vector&& weights, Matrix&& data)
      {
        m_weights = std::forward<Vector>(weights);
        m_data = std::forward<Matrix>(data);
        return static_cast<Derived&>(*this);
      }

      inline
      constexpr
      RangeShape getRangeShape() const
      {
        return { getFiniteElementSpace().getVectorDimension(), 1 };
      }

      template <class Value>
      inline
      Derived& setValue(const std::pair<size_t, Index>& p, size_t local, Value&& v)
      {
        return setValue(getFiniteElementSpace().getGlobalIndex(p, local), std::forward<Value>(v));
      }

      template <class Value>
      inline
      Derived& setValue(Index global, Value&& v)
      {
        if constexpr (std::is_same_v<RangeType, ScalarType>)
        {
          assert(m_data.size() >= 0);
          assert(global < static_cast<size_t>(m_data.size()));
          m_data.coeffRef(global) = std::forward<Value>(v);
        }
        else if constexpr (std::is_same_v<RangeType, Math::Vector<ScalarType>>)
        {
          assert(m_data.cols() >= 0);
          assert(global < static_cast<size_t>(m_data.cols()));
          m_data.col(global) = std::forward<Value>(v);
        }
        else
        {
          assert(false);
        }
        return static_cast<Derived&>(*this);
      }

      /**
       * @brief Gets the value at the given polytope on the local degree of
       * freedom.
       */
      inline
      auto getValue(const std::pair<size_t, Index>& p, size_t local) const
      {
        return getValue(getFiniteElementSpace().getGlobalIndex(p, local));
      }


      /**
       * @brief Gets the value of the GridFunction at the global degree of
       * freedom index.
       */
      inline
      auto getValue(Index global) const
      {
        if constexpr (std::is_same_v<RangeType, ScalarType>)
        {
          assert(m_data.size() >= 0);
          assert(global < static_cast<size_t>(m_data.size()));
          return m_data.coeff(global);
        }
        else if constexpr (std::is_same_v<RangeType, Math::Vector<ScalarType>>)
        {
          assert(m_data.cols() >= 0);
          assert(global < static_cast<size_t>(m_data.cols()));
          return m_data.col(global);
        }
        else
        {
          assert(false);
          return void();
        }
      }

      inline
      constexpr
      RangeType getValue(const Geometry::Point& p) const
      {
        RangeType res;
        getValue(res, p);
        return res;
      }

      /**
       * @brief Gets the interpolated value at the point.
       */
      inline
      void getValue(RangeType& res, const Geometry::Point& p) const
      {
        const auto& polytope = p.getPolytope();
        const auto& polytopeMesh = polytope.getMesh();
        const auto& fes = m_fes.get();
        const auto& fesMesh = fes.getMesh();
        if (polytopeMesh == fesMesh)
        {
          interpolate(res, p);
        }
        else if (const auto inclusion = fesMesh.inclusion(p))
        {
          interpolate(res, *inclusion);
        }
        else if (fesMesh.isSubMesh())
        {
          const auto& submesh = fesMesh.asSubMesh();
          const auto restriction = submesh.restriction(p);
          if (restriction)
          {
            interpolate(res, *restriction);
          }
          else
          {
            assert(false);
          }
        }
        else
        {
          assert(false);
        }
      }

      /**
       * @brief Interpolates the GridFunction at the given point.
       * @note CRTP function to be overriden in Derived class.
       */
      inline
      constexpr
      void interpolate(RangeType& res, const Geometry::Point& p) const
      {
        static_cast<const Derived&>(*this).interpolate(res, p);
      }

    private:
      std::reference_wrapper<const FESType> m_fes;
      DataType m_data;
      std::optional<WeightVectorType> m_weights;
      mutable Threads::Mutex m_mutex;
  };
}

#endif
