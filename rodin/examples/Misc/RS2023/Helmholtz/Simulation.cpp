/*
 *          Copyright Carlos BRITO PACHECO 2021 - 2022.
 * Distributed under the Boost Software License, Version 1.0.
 *       (See accompanying file LICENSE or copy at
 *          https://www.boost.org/LICENSE_1_0.txt)
 */
#include <thread>
#include <chrono>
#include <fstream>
#include <Rodin/Solver.h>
#include <Rodin/Geometry.h>
#include <Rodin/Variational.h>

using namespace Rodin;
using namespace Rodin::Math;
using namespace Rodin::Geometry;
using namespace Rodin::Variational;

static constexpr Real hmax = 0.01; // Maximal size of a triangle's edge
static constexpr Real hmin = 0.1 * hmax;

static constexpr Attribute dQ = 2; // Attribute of box boundary
static constexpr Attribute dCurrent = 4; // Attribute of box boundary
static constexpr Attribute dGround = 5; // Attribute of box boundary

static const Math::Vector<Real> x0{{0.5, 0.5}}; // Center of domain

static constexpr Real pi = Math::Constants::pi();

static constexpr Real R0 = 0.2; // Radius of B_R(x_0)
static constexpr Real R1 = R0 + 10 * hmax; // Radius of B_R(x_0)

static std::mutex mutex_io;

struct Data
{
  const Real m;
  const Real epsilon;
  const Real waveNumber;
  const Real conductivity;
  const Real angle;
};

template <class T>
std::vector<std::vector<T>> splitVector(const std::vector<T>& vec, int n) {
    std::vector<std::vector<T>> result;
    int size = vec.size();
    int partSize = size / n;
    int remainder = size % n;
    int startIndex = 0;

    for (int i = 0; i < n; i++) {
        int currentPartSize = partSize + (i < remainder ? 1 : 0);
        std::vector<T> currentPart(vec.begin() + startIndex, vec.begin() + startIndex + currentPartSize);
        result.push_back(currentPart);
        startIndex += currentPartSize;
    }

    return result;
}

void run(int i, const std::vector<Data>& grid);

int main(int, char**)
{
  // Define evaluation grid
  Math::Vector<Real> m_r = Math::Vector<Real>::LinSpaced(161, 0, 202);
  Math::Vector<Real> epsilon_r = Math::Vector<Real>::LinSpaced(50, hmax, 0.2);
  // Math::Vector<Real> waveNumber_r = Math::Vector<Real>::LinSpaced(1.0 / hmax, 1, 1.0 / hmax);
  // Math::Vector<Real> conductivity_r{{ 1e-12, 0.5, 1.0, 2.0, 1e12 }};
  Math::Vector<Real> waveNumber_r = Math::Vector<Real>::LinSpaced(211, 0, 210);
  // Math::Vector<Real> waveNumber_r{{ 1, 2, 3, 4, 5, 10, 11, 12, 13, 14, 15, 20 }};
  Math::Vector<Real> conductivity_r{{ 2.0 }};
  Math::Vector<Real> angle_r{{ M_PI / 4 }};

  std::vector<Data> grid;
  grid.reserve(epsilon_r.size() * waveNumber_r.size() *  conductivity_r.size());
  for (const Real waveNumber : waveNumber_r)
    for (const Real m : m_r)
      for (const Real epsilon : epsilon_r)
        for (const Real conductivity : conductivity_r)
          for (const Real angle : angle_r)
            grid.push_back({ m, epsilon, waveNumber, conductivity, angle });

  const size_t hwc = std::thread::hardware_concurrency();
  const size_t n = hwc - 2;
  Alert::Info() << "Total grid size: " << grid.size()
                << Alert::NewLine
                << "Hardware concurrency: " << hwc
                << Alert::NewLine
                << "Launching " << n << " threads"
                << Alert::Raise;
  std::vector<std::thread> ts(n);
  auto split = splitVector(grid, ts.size());

  for (size_t i = 0; i < ts.size(); i++)
    ts[i] = std::thread(run, i, split[i]);

  for (auto& t : ts)
    t.join();
}

void run(int id, const std::vector<Data>& grid)
{
  mutex_io.lock();
  Alert::Info() << "Running thread: " << id
                << Alert::NewLine
                << "Grid size: " << grid.size()
                << Alert::Raise;
  mutex_io.unlock();

  // Load mesh
  Mesh mesh;
  mesh.load("Q1.medit.mesh", IO::FileFormat::MEDIT);
  mesh.getConnectivity().compute(1, 2);
  // mesh.save("out/Q.mesh");

  // Math::Vector m_r{{  }};
  // Math::Vector epsilon_r{{ 0.1 }};
  // Math::Vector waveNumber_r{{ 5 }};
  // Math::Vector conductivity_r{{ 2 }};

  Alert::Info() << "Grid size: " << grid.size() << Alert::Raise;

  std::stringstream filename;
  filename << "grid/T" << id << "_L2_Grid_HMax" << std::setw(4) << hmax << ".live.csv";
  std::ofstream out(filename.str());
  out << "m,epsilon,waveNumber,conductivity,angle,error\n";

  size_t i = 0;
  auto t0 = std::chrono::high_resolution_clock::now();
  for (const auto& data : grid)
  {
    if (i % 25 == 0)
    {
      auto t1 = std::chrono::high_resolution_clock::now();
      const auto delta = std::chrono::duration_cast<std::chrono::seconds>(t1 - t0);
      mutex_io.lock();
      Alert::Info() << "Thread " << id << " progress."
                    << Alert::NewLine
                    << "----------------------------------"
                    << Alert::NewLine
                    << i << " / " << grid.size() << " ---- "
                    << (100 * Real(i) / Real(grid.size())) << "%"
                    << Alert::NewLine
                    << "Total elapsed time: " << delta.count() << "s"
                    << Alert::Raise;
      mutex_io.unlock();
    }

    // Alert::Info() << ">>>>>>>>>>>>>>>>>>>>"
    //               << Alert::NewLine
    //               << i << " / " << grid.size()
    //               << Alert::NewLine
    //               << "Epsilon: " << data.epsilon
    //               << Alert::NewLine
    //               << "m: " << data.m
    //               << Alert::NewLine
    //               << "waveNumber: " << data.waveNumber
    //               << Alert::NewLine
    //               << "Inhomogeinity constant: " << data.conductivity
    //               << Alert::Raise;

    // Define finite element spaces
    P1 vh(mesh);

    // Define oscillatory screen
    RealFunction h =
      [&](const Point& p)
      {
        return 2 + sin(2 * pi * data.m * p.x()) * sin(2 * pi * data.m * p.y());
      };

    // Define conductivity
    RealFunction gamma = 1;

    RealFunction gamma_e =
      [&](const Point& p)
      {
        const Real r = (p.getCoordinates() - x0).norm();
        if (r > data.epsilon)
          return gamma(p);
        else
          return data.conductivity;
      };

    // Define boundary data
    VectorFunction xi = { std::cos(data.angle), std::sin(data.angle) };
    RealFunction phi =
      [&](const Point& p)
      { return cos(std::sqrt(2) * data.waveNumber * p.getCoordinates().dot(xi(p))); };

    // Define variational problems
    TrialFunction u(vh);
    TestFunction  v(vh);

    Problem helmholtz(u, v);
    helmholtz = Integral(gamma * Grad(u), Grad(v))
              - Integral(data.waveNumber * data.waveNumber * h * u, v)
              + DirichletBC(u, phi);

    Problem perturbed(u, v);
    perturbed = Integral(gamma_e * Grad(u), Grad(v))
              - Integral(data.waveNumber * data.waveNumber * h * u, v)
              + DirichletBC(u, phi);

    // Solve the background problem
    // Alert::Info() << "Solving background equation." << Alert::Raise;
    Solver::SparseLU(helmholtz).solve();
    const auto u0 = std::move(u.getSolution());
    // Alert::Success() << "Done." << Alert::Raise;

    // Alert::Info() << "Solving perturbed equation." << Alert::Raise;
    Solver::SparseLU(perturbed).solve();
    const auto ue = std::move(u.getSolution());
    // Alert::Success() << "Done." << Alert::Raise;

  RealFunction chi =
    [&](const Point& p)
    { return Real((p.getCoordinates() - x0).norm() > 0.25); };

    GridFunction diff(vh);
    diff = chi * Pow(u0 - ue, 2);
    diff.setWeights();
    const Real error = sqrt(Integral(diff).compute());

    // Alert::Success() << "L2 Error: " << error
    //                  << Alert::NewLine
    //                  << "<<<<<<<<<<<<<<<<<<<<"
    //                  << Alert::Raise;
    out << data.m << ','
        << data.epsilon << ','
        << data.waveNumber << ','
        << data.conductivity << ','
        << data.angle << ','
        << error << '\n';
    out.flush();

    // auto t1 = std::chrono::high_resolution_clock::now();
    // auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
    // Alert::Info() << "Time taken: " << delta.count() << "ms" << Alert::Raise;

    // std::stringstream ss;
    // ss << std::setfill('0') << std::setw(4) << i;
    // u0.save("out/Background_" + ss.str() + ".gf");
    // ue.save("out/Perturbed_" + ss.str() + ".gf");

    i++;
  }

  auto t1 = std::chrono::high_resolution_clock::now();
  const auto delta = std::chrono::duration_cast<std::chrono::seconds>(t1 - t0);

  mutex_io.lock();
  Alert::Success() << "Finished thread " << id << " in " << delta.count() << "s" << Alert::Raise;
  mutex_io.unlock();
}

