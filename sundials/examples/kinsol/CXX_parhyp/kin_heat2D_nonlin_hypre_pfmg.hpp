/* -----------------------------------------------------------------------------
 * Programmer(s): Shelby Lockhart @ LLNL
 *                David Gardner @ LLNL
 * -----------------------------------------------------------------------------
 * SUNDIALS Copyright Start
 * Copyright (c) 2002-2024, Lawrence Livermore National Security
 * and Southern Methodist University.
 * All rights reserved.
 *
 * See the top-level LICENSE and NOTICE files for details.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * SUNDIALS Copyright End
 * ---------------------------------------------------------------------------*/

#ifndef KIN_HEAT2D_NONLIN_HYPRE_PFMG_HPP
#define KIN_HEAT2D_NONLIN_HYPRE_PFMG_HPP

#include <cmath>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

#include "HYPRE_struct_ls.h"          // HYPRE structured grid solver interface
#include "kinsol/kinsol.h"            // access to KINSOL
#include "mpi.h"                      // MPI header file
#include "nvector/nvector_parallel.h" // access to the MPI N_Vector
#include "sundials/sundials_core.hpp"
#include "sunlinsol/sunlinsol_pcg.h" // access to PCG SUNLinearSolver

// Macros for problem constants
#define PI    SUN_RCONST(3.141592653589793238462643383279502884197169)
#define ZERO  SUN_RCONST(0.0)
#define ONE   SUN_RCONST(1.0)
#define TWO   SUN_RCONST(2.0)
#define EIGHT SUN_RCONST(8.0)

// Macro to access (x,y) location in 1D NVector array
#define IDX(x, y, n) ((n) * (y) + (x))

// Define c function type
typedef sunrealtype (*cFn)(sunrealtype u_val);

using namespace std;

// -----------------------------------------------------------------------------
// User data structure
// -----------------------------------------------------------------------------

struct UserData
{
  // Diffusion coefficients in the x and y directions
  sunrealtype kx;
  sunrealtype ky;

  // Enable/disable forcing
  bool forcing;

  // Upper bounds in x and y directions
  sunrealtype xu;
  sunrealtype yu;

  // Global number of nodes in the x and y directions
  sunindextype nx;
  sunindextype ny;

  // Global total number of nodes
  sunindextype nodes;

  // Mesh spacing in the x and y directions
  sunrealtype dx;
  sunrealtype dy;

  // Local number of nodes in the x and y directions
  sunindextype nx_loc;
  sunindextype ny_loc;

  // Overall number of local nodes
  sunindextype nodes_loc;

  // Global x and y indices of this subdomain
  sunindextype is; // x starting index
  sunindextype ie; // x ending index
  sunindextype js; // y starting index
  sunindextype je; // y ending index

  // MPI variables
  MPI_Comm comm_c; // Cartesian communicator in space

  int nprocs_w; // total number of MPI processes in Comm world
  int npx;      // number of MPI processes in the x-direction
  int npy;      // number of MPI processes in the y-direction

  int myid_c; // process ID in Cartesian communicator

  // Flags denoting if this process has a neighbor
  bool HaveNbrW;
  bool HaveNbrE;
  bool HaveNbrS;
  bool HaveNbrN;

  // Neighbor IDs for exchange
  int ipW;
  int ipE;
  int ipS;
  int ipN;

  // Receive buffers for neighbor exchange
  sunrealtype* Wrecv;
  sunrealtype* Erecv;
  sunrealtype* Srecv;
  sunrealtype* Nrecv;

  // Receive requests for neighbor exchange
  MPI_Request reqRW;
  MPI_Request reqRE;
  MPI_Request reqRS;
  MPI_Request reqRN;

  // Send buffers for neighbor exchange
  sunrealtype* Wsend;
  sunrealtype* Esend;
  sunrealtype* Ssend;
  sunrealtype* Nsend;

  // Send requests for neighor exchange
  MPI_Request reqSW;
  MPI_Request reqSE;
  MPI_Request reqSS;
  MPI_Request reqSN;

  // Fixed Point Solver settings
  sunrealtype rtol;    // relative tolerance
  int maa;             // m for Anderson Acceleration
  sunrealtype damping; // daming for Anderson Acceleration
  int orthaa;          // orthogonalization routine for AA
  int maxits;          // max number of fixed point iterations

  // Linear solver and preconditioner settings
  int liniters;       // number of linear iterations
  int msbp;           // max number of steps between preconditioner setups
  sunrealtype epslin; // linear solver tolerance factor

  // Linear solver object
  SUNLinearSolver LS; // linear solver memory structure

  // c(u) Function and integer for help setting
  cFn c;
  int c_int;

  // Vectors to help with FPFunction definition and execution
  N_Vector b;     // defined using c(u_exact)
  N_Vector vtemp; // temporary vector for function evaluation

  // hypre objects
  HYPRE_StructGrid grid;
  HYPRE_StructStencil stencil;
  HYPRE_StructMatrix Jmatrix;
  HYPRE_StructVector bvec;
  HYPRE_StructVector xvec;
  HYPRE_StructVector vvec;
  HYPRE_StructVector Jvvec;
  HYPRE_StructSolver precond;

  // hypre grid extents
  HYPRE_Int ilower[2];
  HYPRE_Int iupper[2];

  // hypre workspace
  HYPRE_Int nwork;
  HYPRE_Real* work;

  // hypre counters
  HYPRE_Int pfmg_its;

  // hypre PFMG settings (hypre defaults)
  HYPRE_Int pfmg_relax;  // type of relaxation:
                         //   0 - Jacobi
                         //   1 - Weighted Jacobi
                         //   2 - symmetric R/B Gauss-Seidel (*)
                         //   3 - nonsymmetric R/B Gauss-Seidel
  HYPRE_Int pfmg_nrelax; // number of pre and post relaxation sweeps (2)

  // Ouput variables
  int output;    // output level
  ofstream uout; // output file stream
  ofstream rout; // output residual file stream
  ofstream eout; // output error file stream
  N_Vector e;    // error vector

  // Timing variables
  bool timing; // print timings
  double totaltime;
  double fevaltime;
  double matfilltime;
  double jvtime;
  double psetuptime;
  double psolvetime;
};

// -----------------------------------------------------------------------------
// Functions provided to the SUNDIALS iterator
// -----------------------------------------------------------------------------

// Nonlinear fixed point function
static int FPFunction(N_Vector u, N_Vector f, void* user_data);

// Nonlinear function c(u)
static int c(N_Vector u, N_Vector z, void* user_data);

// Jacobian-vector product function
static int JTimes(void* user_data, N_Vector v, N_Vector Jv);

// Preconditioner setup and solve functions
static int PSetup(void* user_data);

static int PSolve(void* user_data, N_Vector r, N_Vector z, sunrealtype tol,
                  int lr);

// -----------------------------------------------------------------------------
// Helper functions
// -----------------------------------------------------------------------------

// Setup the parallel decomposition
static int SetupDecomp(MPI_Comm comm_w, UserData* udata);

// Create rhs = b - c(u) for FPFunction
static int SetupRHS(void* user_data);

// Set nonlinear function c(u)
static int SetC(UserData* udata);

// Create hypre objects
static int SetupHypre(UserData* udata);

// Create PCG Linear Solver and attach hypre
static int SetupLS(N_Vector u, void* user_data, SUNContext sunctx);

// Fill Jacobian and A = I - gamma * J
static int Jac(UserData* udata);

// -----------------------------------------------------------------------------
// UserData and input functions
// -----------------------------------------------------------------------------

// Set the default values in the UserData structure
static int InitUserData(UserData* udata);

// Free memory allocated within UserData
static int FreeUserData(UserData* udata);

// Read the command line inputs and set UserData values
static int ReadInputs(int* argc, char*** argv, UserData* udata, bool outproc);

// -----------------------------------------------------------------------------
// Output and utility functions
// -----------------------------------------------------------------------------

// Compute the true solution
static int Solution(N_Vector u, UserData* udata);

// Compute the solution error solution
static int SolutionError(N_Vector u, N_Vector e, UserData* udata);

// Print the command line options
static void InputHelp();

// Print some UserData information
static int PrintUserData(UserData* udata);

// Print solver statistics
static int OutputStats(void* kinsol_mem, UserData* udata);

// Print solver timing
static int OutputTiming(UserData* udata);

// Write solution to a file
static int WriteSolution(N_Vector u, UserData* udata);

// Functions for outputting residual history to file
static int OpenOutput(UserData* udata);
static int WriteOutput(N_Vector u, N_Vector f, UserData* udata);
static int CloseOutput(UserData* udata);

// Check function return values
static int check_retval(void* flagvalue, const string funcname, int opt);

// -----------------------------------------------------------------------------
// Multiple nonlinear functions for testing
// -----------------------------------------------------------------------------

// c(u) = u
sunrealtype c1(sunrealtype u_val) { return u_val; }

// c(u) = u^3 - u
sunrealtype c2(sunrealtype u_val) { return u_val * u_val * u_val - u_val; }

// c(u) = u - u^2
sunrealtype c3(sunrealtype u_val) { return u_val - u_val * u_val; }

// c(u) = e^u
sunrealtype c4(sunrealtype u_val) { return exp(u_val); }

// c(u) = u^4
sunrealtype c5(sunrealtype u_val) { return u_val * u_val * u_val * u_val; }

// c(u) = cos^2(u) - sin^2(u)
sunrealtype c6(sunrealtype u_val)
{
  return (cos(u_val) * cos(u_val)) - (sin(u_val) * sin(u_val));
}

// c(u) = cos^2(u) - sin^2(u) - e^u
sunrealtype c7(sunrealtype u_val)
{
  return (cos(u_val) * cos(u_val)) - (sin(u_val) * sin(u_val)) - exp(u_val);
}

// c(u) = e^u * u^4 - u * e^{cos(u)}
sunrealtype c8(sunrealtype u_val)
{
  sunrealtype u2 = u_val * u_val;
  return exp(u_val) * u2 * u2 - u_val * exp(cos(u_val));
}

// c(u) = e^(cos^2(u))
sunrealtype c9(sunrealtype u_val)
{
  sunrealtype cos2u = cos(u_val) * cos(u_val);
  return exp(cos2u);
}

// c(u) = 10(u - u^2)
sunrealtype c10(sunrealtype u_val)
{
  sunrealtype u2 = u_val * u_val;
  return 10.0 * (u_val - u2);
}

// c(u) = -13 + u + ((5-u)u - 2)u
sunrealtype c11(sunrealtype u_val)
{
  sunrealtype temp = ((5.0 - u_val) * u_val) - 2.0;
  return -13.0 + u_val + temp * u_val;
}

// c(u) = sqrt(5) * (u - u^2)
sunrealtype c12(sunrealtype u_val)
{
  sunrealtype temp = sqrt(5);
  sunrealtype u2   = u_val * u_val;
  return temp * (u_val - u2);
}

// c(u) = (u - e^u)^2 + (u + u * sin(u) - cos(u))^2
sunrealtype c13(sunrealtype u_val)
{
  sunrealtype eu   = u_val - exp(u_val);
  sunrealtype usin = u_val * sin(u_val);
  sunrealtype temp = (u_val + usin - cos(u_val));
  return eu * eu + temp * temp;
}

// c(u) = u + ue^u + ue^{-u}
sunrealtype c14(sunrealtype u_val)
{
  sunrealtype ueu  = u_val * exp(u_val);
  sunrealtype ue_u = u_val * exp(-u_val);
  return u_val + ueu + ue_u;
}

// c(u) = u + ue^u + ue^{-u} + (u - e^u)^2
sunrealtype c15(sunrealtype u_val)
{
  sunrealtype ueu  = u_val * exp(u_val);
  sunrealtype ue_u = u_val * exp(-u_val);
  sunrealtype temp = u_val - exp(u_val);
  return u_val + ueu + ue_u + (temp * temp);
}

// c(u) = u + ue^u + ue^{-u} + (u - e^u)^2 + (u + usin(u) - cos(u))^2
sunrealtype c16(sunrealtype u_val)
{
  sunrealtype ueu   = u_val * exp(u_val);
  sunrealtype ue_u  = u_val * exp(-u_val);
  sunrealtype temp  = u_val - exp(u_val);
  sunrealtype temp2 = u_val + (u_val * sin(u_val)) - cos(u_val);
  return u_val + ueu + ue_u + (temp * temp) + (temp2 * temp2);
}

// c(u) = u + ue^{-u} + e^u*(u + sin(u) - cos(u))^3
sunrealtype c17(sunrealtype u_val)
{
  sunrealtype ue_u = u_val * exp(-u_val);
  sunrealtype eu   = exp(u_val);
  sunrealtype temp = u_val + sin(u_val) - cos(u_val);
  return u_val + ue_u + eu * (temp * temp * temp);
}

#endif
