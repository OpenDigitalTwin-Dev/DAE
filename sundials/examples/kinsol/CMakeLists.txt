# ---------------------------------------------------------------
# Programmer(s): David J. Gardner and Slaven Peles @ LLNL
#                Shelby Lockhart @ LLNL
# ---------------------------------------------------------------
# SUNDIALS Copyright Start
# Copyright (c) 2002-2024, Lawrence Livermore National Security
# and Southern Methodist University.
# All rights reserved.
#
# See the top-level LICENSE and NOTICE files for details.
#
# SPDX-License-Identifier: BSD-3-Clause
# SUNDIALS Copyright End
# ---------------------------------------------------------------
# examples/kinsol level CMakeLists.txt for SUNDIALS
# ---------------------------------------------------------------

# C examples
if(EXAMPLES_ENABLE_C)
  add_subdirectory(serial)
  if(ENABLE_OPENMP AND OPENMP_FOUND)
    # the only example here need special handling from testrunner (not yet implemented)
    add_subdirectory(C_openmp)
  endif()
  if(ENABLE_MPI AND MPI_C_FOUND)
    add_subdirectory(parallel)
  endif()
endif()

# C++ examples
if(EXAMPLES_ENABLE_CXX)
  if(ENABLE_MPI AND MPI_CXX_FOUND)
    add_subdirectory(CXX_parallel)
  endif()
  if(ENABLE_MPI AND MPI_CXX_FOUND AND ENABLE_HYPRE AND HYPRE_FOUND)
    add_subdirectory(CXX_parhyp)
  endif()
endif()

# CUDA examples
if(EXAMPLES_ENABLE_CUDA)
  if(ENABLE_MPI AND MPI_CXX_FOUND)
    add_subdirectory(CUDA_mpi)
  endif()
endif()

# Fortran examples
if(BUILD_FORTRAN_MODULE_INTERFACE AND EXAMPLES_ENABLE_F2003)
  add_subdirectory(F2003_serial)
  add_subdirectory(F2003_parallel)
endif()
