/*!
 * \file   include/TFEL/Config/Internals/InvJacobianType.hxx
 * \brief  This file declares the  class
 * \author Thomas Helfer
 * \date   25 Sep 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_INVJACOBIANTYPE_HXX
#define LIB_TFEL_INVJACOBIANTYPE_HXX

#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/Forward/qt.hxx"
#include "TFEL/Math/Quantity/Unit.hxx"

namespace tfel::config::internals {

  template <unsigned short N, typename T, bool use_qt>
  struct InvJacobianType {
    typedef tfel::math::tmatrix<N, N, tfel::math::qt<tfel::math::NoUnit, T>>
        type;
  };

  template <unsigned short N, typename T>
  struct InvJacobianType<N, T, false> {
    typedef tfel::math::tmatrix<N, N, T> type;
  };

  template <typename T, bool use_qt>
  struct InvJacobianType<1u, T, use_qt> {
    typedef tfel::math::qt<tfel::math::NoUnit, T> type;
  };

  template <typename T>
  struct InvJacobianType<1u, T, false> {
    typedef T type;
  };

}  // end of namespace tfel::config::internals

#endif /* LIB_TFEL_INVJACOBIANTYPE_HXX */