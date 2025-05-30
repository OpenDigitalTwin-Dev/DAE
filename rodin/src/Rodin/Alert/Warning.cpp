/*
 *          Copyright Carlos BRITO PACHECO 2021 - 2022.
 * Distributed under the Boost Software License, Version 1.0.
 *       (See accompanying file LICENSE or copy at
 *          https://www.boost.org/LICENSE_1_0.txt)
 */
#include <iostream>

#include "Rodin/Configure.h"

#include "Warning.h"

namespace Rodin::Alert
{
  Warning::Warning()
    : Warning(std::cerr)
  {}

  Warning::Warning(std::ostream& os)
    : Parent(os, WarningPrefix())
  {}

  Warning::Warning(const Warning& other)
    : Parent(other)
  {}

  Warning::Warning(Warning&& other)
    : Parent(std::move(other))
  {}
}

