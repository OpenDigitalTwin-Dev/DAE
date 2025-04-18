/*
 *          Copyright Carlos BRITO PACHECO 2021 - 2022.
 * Distributed under the Boost Software License, Version 1.0.
 *       (See accompanying file LICENSE or copy at
 *          https://www.boost.org/LICENSE_1_0.txt)
 */
#include <Rodin/Geometry.h>
#include <Rodin/Variational.h>

using namespace Rodin;
using namespace Rodin::Geometry;
using namespace Rodin::Variational;

int main(int, char**)
{
  const size_t vdim = 2;
  const size_t n = 32;

  Mesh mesh;
  mesh = mesh.UniformGrid(Polytope::Type::Triangle, { n, n });
  mesh.scale(1.0 / (n - 1.0));
  mesh.getConnectivity().compute(1, 2);
  for (auto it = mesh.getCell(); it; ++it)
  {
    bool b = true;
    for (auto vit = it->getVertex(); vit; ++vit)
    {
      if (vit->x() >= 0.4)
      {
        b = false;
        break;
      }
    }
    if (b)
      mesh.setAttribute({2, it->getIndex()}, 2);
  }

  P1 fes(mesh);
  GridFunction gf(fes);
  gf = [](const Point& p){ return 1 - (p.x() - 0.5) * (p.x() - 0.5) - (p.y() - 0.5) * (p.y() - 0.5); };

  P1 vfes(mesh, vdim);

  GridFunction grad(vfes);
  grad = Grad(gf);

  GridFunction trace(vfes);
  trace.projectOnFaces(Grad(gf).traceOf(1));

  mesh.save("Grid.mesh");
  gf.save("Function.gf");
  grad.save("Gradient.gf");
  trace.save("Trace.gf");

}

