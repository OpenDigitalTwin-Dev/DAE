/*
 *          Copyright Carlos BRITO PACHECO 2021 - 2022.
 * Distributed under the Boost Software License, Version 1.0.
 *       (See accompanying file LICENSE or copy at
 *          https://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef RODIN_IO_LOADER_H
#define RODIN_IO_LOADER_H

#include <iostream>
#include <istream>
#include <fstream>

#include <boost/filesystem/path.hpp>

#include "ForwardDecls.h"

namespace Rodin::IO
{
  template <class T>
  class Loader
  {
    public:
      using ObjectType = T;

      virtual void load(std::istream& is) = 0;

      virtual void load(const boost::filesystem::path& is)
      {
        std::ifstream in(is.c_str());
        load(in);
      }

    protected:
      virtual ObjectType& getObject() = 0;
  };
}

#endif
