/*
 *          Copyright Carlos BRITO PACHECO 2021 - 2022.
 * Distributed under the Boost Software License, Version 1.0.
 *       (See accompanying file LICENSE or copy at
 *          https://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef RODIN_EXTERNAL_MMG_MESHPRINTER_H
#define RODIN_EXTERNAL_MMG_MESHPRINTER_H

#include "Rodin/IO/MEDIT.h"

#include "Mesh.h"

namespace Rodin::External::MMG
{
  /**
   * @brief Class used to print objects of type MMG::Mesh.
   */
  class MeshPrinter : public IO::Printer<MMG::Mesh>
  {
    public:
      MeshPrinter(const MMG::Mesh& mesh)
        : m_mesh(mesh),
          m_printer(mesh)
      {}

      void print(std::ostream& os) override;

      void printCorners(std::ostream& os);
      void printRidges(std::ostream& os);
      void printRequiredVertices(std::ostream& os);
      void printRequiredEdges(std::ostream& os);

      const MMG::Mesh& getObject() const override
      {
        return m_mesh.get();
      }

    private:
      std::reference_wrapper<const MMG::Mesh> m_mesh;
      IO::MeshPrinter<IO::FileFormat::MEDIT, Context::Sequential> m_printer;
  };
}

#endif

