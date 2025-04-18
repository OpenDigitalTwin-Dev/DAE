/*
 *          Copyright Carlos BRITO PACHECO 2021 - 2022.
 * Distributed under the Boost Software License, Version 1.0.
 *       (See accompanying file LICENSE or copy at
 *          https://www.boost.org/LICENSE_1_0.txt)
 */
#include <Rodin/Geometry.h>

using namespace Rodin;
using namespace Geometry;

int main(int, char**)
{
  Mesh mesh;
  mesh.load("../resources/gmsh/inductor.msh", IO::FileFormat::GMSH);

  std::cout << "Saved Gmsh file to mfem.mesh in Mfem format" << std::endl;

  mesh.save("mfem.mesh", IO::FileFormat::MFEM);

  return 0;
}
