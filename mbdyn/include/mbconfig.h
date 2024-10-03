/* include/mbconfig.h.  Generated from mbconfig.h.in by configure.  */
/* include/mbconfig.h.in.  Generated from configure.ac by autoheader.  */


/*
 * MBDyn (C) is a multibody analysis code.
 * http://www.mbdyn.org
 *
 * Copyright (C) 1996-2023
 *
 * Pierangelo Masarati  <pierangelo.masarati@polimi.it>
 * Paolo Mantegazza     <paolo.mantegazza@polimi.it>
 *
 * Dipartimento di Ingegneria Aerospaziale - Politecnico di Milano
 * via La Masa, 34 - 20156 Milano, Italy
 * http://www.aero.polimi.it
 *
 * Changing this copyright notice is forbidden.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation (version 2 of the License).
 *
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


/* define this to add debugging code */
/* #undef DEBUG */

/* define this to add memory debugging code */
/* #undef DEBUG_MEMMANAGER */

/* define this to add MPI debugging code */
/* #undef DEBUG_MPI */

/* define if variable environ needs to be declared */
/* #undef ENVIRON_NEEDS_DECLARATION */

/* define if variable environ needs _GNU_SOURCE and unistd.h */
#define ENVIRON_NEEDS_GNU_SOURCE_AND_UNISTD_H 1

/* define if variable environ needs unistd.h */
/* #undef ENVIRON_NEEDS_UNISTD_H */

/* Define to dummy `main' function (if any) required to link to the Fortran
   libraries. */
/* #undef FC_DUMMY_MAIN */

/* Define if F77 and FC dummy `main' functions are identical. */
/* #undef FC_DUMMY_MAIN_EQ_F77 */

/* define if aligned_alloc can be used */
#define HAVE_ALIGNED_ALLOC 1

/* define if _aligned_malloc and _aligned_free can be used */
/* #undef HAVE_ALIGNED_MALLOC */

/* define if alloca can be used */
#define HAVE_ALLOCA 1

/* Define to 1 if you have the <alloca.h> header file. */
#define HAVE_ALLOCA_H 1

/* Define to 1 if you have the <Amesos_BaseSolver.h> header file. */
/* #undef HAVE_AMESOS_BASESOLVER_H */

/* Define to 1 if you have the <Amesos.h> header file. */
/* #undef HAVE_AMESOS_H */

/* define to use ANN */
/* #undef HAVE_ANN */

/* Define to 1 if you have the <ANN/ANN.h> header file. */
/* #undef HAVE_ANN_ANN_H */

/* Define to 1 if you have the <ANN.h> header file. */
/* #undef HAVE_ANN_H */

/* Define to 1 if you have the <arpa/inet.h> header file. */
#define HAVE_ARPA_INET_H 1

/* Define to 1 if you have the `atoi' function. */
#define HAVE_ATOI 1

/* Define to 1 if you have the `atol' function. */
#define HAVE_ATOL 1

/* define to use HP's atomic_ops library */
/* #undef HAVE_ATOMIC_OPS */

/* Define to 1 if you have the <atomic_ops.h> header file. */
/* #undef HAVE_ATOMIC_OPS_H */

/* Define to 1 if you have the <AztecOO.h> header file. */
/* #undef HAVE_AZTECOO_H */

/* define if BLAS are available */
#define HAVE_BLAS 1

/* define if bool is defined */
#define HAVE_BOOL 1

/* Define to 1 if you have the <boost/config.hpp> header file. */
/* #undef HAVE_BOOST_CONFIG_HPP */

/* Define to 1 if you have the <boost/graph/adjacency_list.hpp> header file.
   */
/* #undef HAVE_BOOST_GRAPH_ADJACENCY_LIST_HPP */

/* Define to 1 if you have the <boost/graph/bandwidth.hpp> header file. */
/* #undef HAVE_BOOST_GRAPH_BANDWIDTH_HPP */

/* Define to 1 if you have the <boost/graph/cuthill_mckee_ordering.hpp> header
   file. */
/* #undef HAVE_BOOST_GRAPH_CUTHILL_MCKEE_ORDERING_HPP */

/* Define to 1 if you have the <boost/graph/king_ordering.hpp> header file. */
/* #undef HAVE_BOOST_GRAPH_KING_ORDERING_HPP */

/* Define to 1 if you have the <boost/graph/minimum_degree_ordering.hpp>
   header file. */
/* #undef HAVE_BOOST_GRAPH_MINIMUM_DEGREE_ORDERING_HPP */

/* Define to 1 if you have the <boost/graph/properties.hpp> header file. */
/* #undef HAVE_BOOST_GRAPH_PROPERTIES_HPP */

/* Define to 1 if you have the <boost/graph/sloan_ordering.hpp> header file.
   */
/* #undef HAVE_BOOST_GRAPH_SLOAN_ORDERING_HPP */

/* Define to 1 if you have the <boost/graph/wavefront.hpp> header file. */
/* #undef HAVE_BOOST_GRAPH_WAVEFRONT_HPP */

/* define if CBLAS are available */
/* #undef HAVE_CBLAS */

/* define to build CHARM WP module */
/* #undef HAVE_CHARM */

/* Define to 1 if you have the `chdir' function. */
#define HAVE_CHDIR 1

/* define if clean_up_and_exit can be used */
/* #undef HAVE_CLEAN_UP_AND_EXIT */

/* define if clock_gettime can be used */
#define HAVE_CLOCK_GETTIME 1

/* Define to 1 if you have the `copysign' function. */
#define HAVE_COPYSIGN 1

/* define if crypt(3) can be linked */
/* #undef HAVE_CRYPT */

/* Define to 1 if you have the <cxxabi.h> header file. */
/* #undef HAVE_CXXABI_H */

/* define to use LAPACK's dgecon in newton raphson solver */
#define HAVE_DGECON 1

/* define to use LAPACK's dgegv in Eig */
#define HAVE_DGEGV 1

/* define to use LAPACK's dgelsd function */
#define HAVE_DGELSD 1

/* define to use LAPACK's dgesvd in GPC */
#define HAVE_DGESVD 1

/* define to use LAPACK's dgetrf in newton raphson solver */
#define HAVE_DGETRF 1

/* define to use LAPACK's dgetri function */
#define HAVE_DGETRI 1

/* define to use LAPACK's dgetrs function */
#define HAVE_DGETRS 1

/* define to use LAPACK's dggev in Eig */
#define HAVE_DGGEV 1

/* define to use LAPACK's dlamch in dgeequ */
#define HAVE_DLAMCH 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* define if do_octave_atexit can be used */
/* #undef HAVE_DO_OCTAVE_ATEXIT */

/* Define to 1 if you have the <dsp_defs.h> header file. */
/* #undef HAVE_DSP_DEFS_H */

/* define to use LAPACK's dsyev function */
#define HAVE_DSYEV 1

/* define to use LAPACK's dtrtrs in SPQR */
#define HAVE_DTRTRS 1

/* define if variable environ is declared */
#define HAVE_ENVIRON 1

/* Define to 1 if you have the <Epetra_Comm.h> header file. */
/* #undef HAVE_EPETRA_COMM_H */

/* Define to 1 if you have the <Epetra_CrsMatrix.h> header file. */
/* #undef HAVE_EPETRA_CRSMATRIX_H */

/* Define to 1 if you have the <Epetra_Operator.h> header file. */
/* #undef HAVE_EPETRA_OPERATOR_H */

/* Define to 1 if you have the <Epetra_SerialComm.h> header file. */
/* #undef HAVE_EPETRA_SERIALCOMM_H */

/* Define to 1 if you have the <Epetra_Vector.h> header file. */
/* #undef HAVE_EPETRA_VECTOR_H */

/* Define to 1 if you have the <exception> header file. */
#define HAVE_EXCEPTION 1

/* define if a Fortran 77 compiler is available */
#define HAVE_FC_F77 1

/* Define to 1 if you have the `feenableexcept' function. */
#define HAVE_FEENABLEEXCEPT 1

/* Define to 1 if you have the <fenv.h> header file. */
#define HAVE_FENV_H 1

/* Define to 1 if you have the `finite' function. */
#define HAVE_FINITE 1

/* define if class ios has 'fmtflags' type */
#define HAVE_FMTFLAGS_IN_IOS 1

/* Define to 1 if you have the `getaddrinfo' function. */
#define HAVE_GETADDRINFO 1

/* Define to 1 if you have the `getcwd' function. */
#define HAVE_GETCWD 1

/* Define to 1 if you have the `gethostbyname' function. */
#define HAVE_GETHOSTBYNAME 1

/* Define to 1 if you have the `gethostbyname2' function. */
#define HAVE_GETHOSTBYNAME2 1

/* Define to 1 if you have the `getopt' function. */
#define HAVE_GETOPT 1

/* Define to 1 if you have the <getopt.h> header file. */
#define HAVE_GETOPT_H 1

/* Define to 1 if you have the `getopt_long' function. */
#define HAVE_GETOPT_LONG 1

/* Define to 1 if you have the `getpid' function. */
#define HAVE_GETPID 1

/* Define to 1 if you have the `get_ncpus' function. */
/* #undef HAVE_GET_NCPUS */

/* define if get_nprocs is available */
#define HAVE_GET_NPROCS 1

/* Define to 1 if you have the `get_nprocs_conf' function. */
#define HAVE_GET_NPROCS_CONF 1

/* Define to 1 if you have the <gtest/gtest.h> header file. */
/* #undef HAVE_GTEST_GTEST_H */

/* Define to 1 if you have the `inet_aton' function. */
#define HAVE_INET_ATON 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <klu.h> header file. */
/* #undef HAVE_KLU_H */

/* Define to 1 if you have the `ws2_32' library (-lws2_32). */
/* #undef HAVE_LIBWS2_32 */

/* Define to 1 if you have the `localtime' function. */
#define HAVE_LOCALTIME 1

/* define if you have <ltdl.h> */
#define HAVE_LTDL_H 1

/* define if memalign can be used */
#define HAVE_MEMALIGN 1

/* Define to 1 if you have the `memmove' function. */
#define HAVE_MEMMOVE 1

/* Define to 1 if you have the `memset' function. */
#define HAVE_MEMSET 1

/* Define to 1 if you have the <metis.h> header file. */
/* #undef HAVE_METIS_H */

/* Define to 1 if you have the <MGIS/Behaviour/BehaviourData.hxx> header file.
   */
/* #undef HAVE_MGIS_BEHAVIOUR_BEHAVIOURDATA_HXX */

/* Define to 1 if you have the <MGIS/Behaviour/Behaviour.hxx> header file. */
/* #undef HAVE_MGIS_BEHAVIOUR_BEHAVIOUR_HXX */

/* Define to 1 if you have the <MGIS/Behaviour/Integrate.hxx> header file. */
/* #undef HAVE_MGIS_BEHAVIOUR_INTEGRATE_HXX */

/* Define to 1 if you have the <MGIS/Behaviour/State.hxx> header file. */
/* #undef HAVE_MGIS_BEHAVIOUR_STATE_HXX */

/* Define to 1 if you have the `mkstemp' function. */
#define HAVE_MKSTEMP 1

/* Define to 1 if you have the `mlockall' function. */
#define HAVE_MLOCKALL 1

/* Define to 1 if you have the <mpicxx.h> header file. */
/* #undef HAVE_MPICXX_H */

/* Define to 1 if you have the <mpi.h> header file. */
/* #undef HAVE_MPI_H */

/* Define to 1 if you have the <mpi++.h> header file. */
/* #undef HAVE_MPI___H */

/* Define to 1 if you have the `nanosleep' function. */
#define HAVE_NANOSLEEP 1

/* Define to 1 if you have the <netdb.h> header file. */
#define HAVE_NETDB_H 1

/* Define to 1 if you have the <netinet/in.h> header file. */
#define HAVE_NETINET_IN_H 1

/* Define to 1 if you have the <net_rpc.h> header file. */
/* #undef HAVE_NET_RPC_H */

/* define if the compiler accept noexcept */
#define HAVE_NOEXCEPT 1

/* Define to 1 if you have the <NOX_Epetra.H> header file. */
/* #undef HAVE_NOX_EPETRA_H */

/* Define to 1 if you have the <NOX_Epetra_Interface_Jacobian.H> header file.
   */
/* #undef HAVE_NOX_EPETRA_INTERFACE_JACOBIAN_H */

/* Define to 1 if you have the <NOX_Epetra_Interface_Preconditioner.H> header
   file. */
/* #undef HAVE_NOX_EPETRA_INTERFACE_PRECONDITIONER_H */

/* Define to 1 if you have the <NOX_Epetra_Interface_Required.H> header file.
   */
/* #undef HAVE_NOX_EPETRA_INTERFACE_REQUIRED_H */

/* Define to 1 if you have the <NOX_Epetra_LinearSystem_AztecOO.H> header
   file. */
/* #undef HAVE_NOX_EPETRA_LINEARSYSTEM_AZTECOO_H */

/* Define to 1 if you have the <NOX_Epetra_Vector.H> header file. */
/* #undef HAVE_NOX_EPETRA_VECTOR_H */

/* Define to 1 if you have the <NOX.H> header file. */
/* #undef HAVE_NOX_H */

/* Define to 1 if you have the <NOX_Solver_Generic.H> header file. */
/* #undef HAVE_NOX_SOLVER_GENERIC_H */

/* Define to 1 if you have the <numerics/MCP_cst.h> header file. */
/* #undef HAVE_NUMERICS_MCP_CST_H */

/* Define to 1 if you have the <numerics/MixedComplementarityProblem.h> header
   file. */
/* #undef HAVE_NUMERICS_MIXEDCOMPLEMENTARITYPROBLEM_H */

/* Define to 1 if you have the <numerics/Newton_methods.h> header file. */
/* #undef HAVE_NUMERICS_NEWTON_METHODS_H */

/* Define to 1 if you have the <numerics/NonSmoothDrivers.h> header file. */
/* #undef HAVE_NUMERICS_NONSMOOTHDRIVERS_H */

/* Define to 1 if you have the <numerics/NumericsMatrix.h> header file. */
/* #undef HAVE_NUMERICS_NUMERICSMATRIX_H */

/* Define to 1 if you have the <numerics/NumericsVerbose.h> header file. */
/* #undef HAVE_NUMERICS_NUMERICSVERBOSE_H */

/* Define to 1 if you have the <numerics/SolverOptions.h> header file. */
/* #undef HAVE_NUMERICS_SOLVEROPTIONS_H */

/* define if offsetof() works */
#define HAVE_OFFSETOF 1

/* Define to 1 if you have the <pam_misc.h> header file. */
/* #undef HAVE_PAM_MISC_H */

/* Define to 1 if you have the `pastixFinalize' function. */
/* #undef HAVE_PASTIXFINALIZE */

/* Define to 1 if you have the `pastixInit' function. */
/* #undef HAVE_PASTIXINIT */

/* Define to 1 if you have the `pastixInitParam' function. */
/* #undef HAVE_PASTIXINITPARAM */

/* Define to 1 if you have the `pastix_task_analyze' function. */
/* #undef HAVE_PASTIX_TASK_ANALYZE */

/* Define to 1 if you have the `pastix_task_numfact' function. */
/* #undef HAVE_PASTIX_TASK_NUMFACT */

/* Define to 1 if you have the `pastix_task_refine' function. */
/* #undef HAVE_PASTIX_TASK_REFINE */

/* Define to 1 if you have the `pastix_task_solve' function. */
/* #undef HAVE_PASTIX_TASK_SOLVE */

/* Define to 1 if you have the <pdsp_defs.h> header file. */
/* #undef HAVE_PDSP_DEFS_H */

/* define if posix_memalign can be used */
#define HAVE_POSIX_MEMALIGN 1

/* Define to 1 if you have the `pstat_getdynamic' function. */
/* #undef HAVE_PSTAT_GETDYNAMIC */

/* Define to 1 if you have the `pstat_getprocessor' function. */
/* #undef HAVE_PSTAT_GETPROCESSOR */

/* Define if you have POSIX threads libraries and header files. */
/* #undef HAVE_PTHREAD */

/* Define to 1 if you have the <pthread.h> header file. */
#define HAVE_PTHREAD_H 1

/* Have PTHREAD_PRIO_INHERIT. */
#define HAVE_PTHREAD_PRIO_INHERIT 1

/* define to use pthread_sigmask */
#define HAVE_PTHREAD_SIGMASK 1

/* Define to 1 if you have the `putenv' function. */
#define HAVE_PUTENV 1

/* Define to 1 if you have the <pwd.h> header file. */
#define HAVE_PWD_H 1

/* If available, contains the Python version number currently in use. */
/* #undef HAVE_PYTHON */

/* Define to 1 if you have the <rtai_lxrt.h> header file. */
/* #undef HAVE_RTAI_LXRT_H */

/* Define to 1 if you have the <rtai_lxrt_user.h> header file. */
/* #undef HAVE_RTAI_LXRT_USER_H */

/* Define to 1 if you have the <rtai_netrpc.h> header file. */
/* #undef HAVE_RTAI_NETRPC_H */

/* define to use Cyrus SASL2 */
/* #undef HAVE_SASL2 */

/* Define to 1 if you have the <sasl.h> header file. */
/* #undef HAVE_SASL_H */

/* Define to 1 if you have the <sasl/sasl.h> header file. */
/* #undef HAVE_SASL_SASL_H */

/* Define to 1 if you have the <sched.h> header file. */
#define HAVE_SCHED_H 1

/* Define to 1 if you have the `sched_setaffinity' function. */
#define HAVE_SCHED_SETAFFINITY 1

/* Define to 1 if you have the <security/pam_appl.h> header file. */
/* #undef HAVE_SECURITY_PAM_APPL_H */

/* Define to 1 if you have the <security/pam_misc.h> header file. */
/* #undef HAVE_SECURITY_PAM_MISC_H */

/* Define to 1 if you have the <semaphore.h> header file. */
#define HAVE_SEMAPHORE_H 1

/* Define to 1 if you have the `setenv' function. */
#define HAVE_SETENV 1

/* Define to 1 if you have the `shmget' function. */
#define HAVE_SHMGET 1

/* Define to 1 if you have the `signal' function. */
#define HAVE_SIGNAL 1

/* Define to 1 if you have the <signal.h> header file. */
#define HAVE_SIGNAL_H 1

/* define if sig_atomic_t is available */
#define HAVE_SIG_ATOMIC_T 1

/* Define to 1 if you have the `sleep' function. */
#define HAVE_SLEEP 1

/* define if socket() is available */
#define HAVE_SOCKET 1

/* define if typedef socklen_t is defined */
#define HAVE_SOCKLEN_T 1

/* Define to 1 if you have the `spmCheckAxb' function. */
/* #undef HAVE_SPMCHECKAXB */

/* Define to 1 if you have the `spmExit' function. */
/* #undef HAVE_SPMEXIT */

/* Define to 1 if you have the `spmNorm' function. */
/* #undef HAVE_SPMNORM */

/* Define to 1 if you have the `spmScalMatrix' function. */
/* #undef HAVE_SPMSCALMATRIX */

/* Define to 1 if you have the `spmScalVector' function. */
/* #undef HAVE_SPMSCALVECTOR */

/* Define to 1 if you have the <stdexcept> header file. */
#define HAVE_STDEXCEPT 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdio.h> header file. */
#define HAVE_STDIO_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strftime' function. */
#define HAVE_STRFTIME 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strtod' function. */
#define HAVE_STRTOD 1

/* Define to 1 if you have the `strtol' function. */
#define HAVE_STRTOL 1

/* Define to 1 if you have the `strtoul' function. */
#define HAVE_STRTOUL 1

/* Define to 1 if you have the <StrumpackSparseSolver.hpp> header file. */
/* #undef HAVE_STRUMPACKSPARSESOLVER_HPP */

/* Define to 1 if you have the <SuiteSparseQR.hpp> header file. */
/* #undef HAVE_SUITESPARSEQR_HPP */

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#define HAVE_SYS_IOCTL_H 1

/* Define to 1 if you have the <sys/ipc.h> header file. */
#define HAVE_SYS_IPC_H 1

/* Define to 1 if you have the <sys/mman.h> header file. */
#define HAVE_SYS_MMAN_H 1

/* Define to 1 if you have the <sys/param.h> header file. */
#define HAVE_SYS_PARAM_H 1

/* Define to 1 if you have the <sys/poll.h> header file. */
#define HAVE_SYS_POLL_H 1

/* Define to 1 if you have the <sys/pstat.h> header file. */
/* #undef HAVE_SYS_PSTAT_H */

/* Define to 1 if you have the <sys/select.h> header file. */
#define HAVE_SYS_SELECT_H 1

/* Define to 1 if you have the <sys/shm.h> header file. */
#define HAVE_SYS_SHM_H 1

/* Define to 1 if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/sysinfo.h> header file. */
#define HAVE_SYS_SYSINFO_H 1

/* Define to 1 if you have the <sys/times.h> header file. */
#define HAVE_SYS_TIMES_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* define if usable threads are available */
#define HAVE_THREADS 1

/* Define to 1 if you have the `time' function. */
#define HAVE_TIME 1

/* Define to 1 if you have the `times' function. */
#define HAVE_TIMES 1

/* Define to 1 if you have the <time.h> header file. */
#define HAVE_TIME_H 1

/* Define to 1 if you have the <umfpack.h> header file. */
/* #undef HAVE_UMFPACK_H */

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `unsetenv' function. */
#define HAVE_UNSETENV 1

/* Define to 1 if you have the `usleep' function. */
#define HAVE_USLEEP 1

/* Define to 1 if you have the <util.h> header file. */
/* #undef HAVE_UTIL_H */

/* Define to 1 if you have the <values.h> header file. */
#define HAVE_VALUES_H 1

/* Define to 1 if you have the <winsock2.h> header file. */
/* #undef HAVE_WINSOCK2_H */

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* define to enable features under development */
#define MBDYN_DEVEL 1

/* define if BLAS/ATLAS are not available to KLU */
/* #undef NBLAS */

/* define if bool needs bool.h */
/* #undef NEED_BOOL_H */

/* define if CHARM WP module needs shipwake */
/* #undef NEED_CHARM_SHIPWAKE */

/* define if you want to use new Metis API */
/* #undef NEW_METIS_INTERFACE */

/* if defined, operator new will return memory initilized to 0xFF */
/* #undef OVERRIDE_OPERATOR_NEW */

/* Name of package */
#define PACKAGE "mbdyn"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME "mbdyn"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "mbdyn develop"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "mbdyn"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "develop"

/* Define to necessary symbol if this constant uses a non-standard name on
   your system. */
/* #undef PTHREAD_CREATE_JOINABLE */

/* define to statically build modules */
/* #undef STATIC_MODULES */

/* Define to 1 if all of the C90 standard headers exist (not just the ones
   required in a freestanding environment). This macro is provided for
   backward compatibility; new code need not use it. */
#define STDC_HEADERS 1

/* define Fortran 77 aerodynamic routines can be used */
#define USE_AEROD2_F 1

/* define to use ANN */
/* #undef USE_ANN */

/* define to use ARPACK libraries */
/* #undef USE_ARPACK */

/* define to use BOOST */
/* #undef USE_BOOST */

/* define this to enable bullet <http://bulletphysics.com> */
/* #undef USE_BULLET */

/* define if you want to use Chaco */
/* #undef USE_CHACO */

/* define if you want to use GPC/DeadBeat control */
#define USE_DBC 1

/* define if you want to enable direct eigensolution */
#define USE_EIG 1

/* define if GiNaC is available */
/* #undef USE_GINAC */

/* define if you want to use GoogleTest */
/* #undef USE_GTEST */

/* define to use Harwell sparse LU library */
/* #undef USE_HARWELL */

/* define to use JDQZ library */
/* #undef USE_JDQZ */

/* define if you want to use KLU */
/* #undef USE_KLU */

/* define to use LAPACK libraries */
#define USE_LAPACK 1

/* define if you want to use Metis */
/* #undef USE_METIS */

/* define if you want to use MFrontGenericInterfaceSupport */
/* #undef USE_MFRONT */

/* define to use mkl_set_num_threads_local */
/* #undef USE_MKL_SET_NUM_THREADS_LOCAL */

/* define if you want to use MPI */
/* #undef USE_MPI */

/* define to enable multithreaded solution */
/* #undef USE_MULTITHREAD */

/* define to enable multithread naive solver */
/* #undef USE_NAIVE_MULTITHREAD */

/* define this to enable NetCDF4 based output */
#define USE_NETCDF 1

/* define if octave can be used */
/* #undef USE_OCTAVE */

/* define if you want to use PAM */
/* #undef USE_PAM */

/* define to use Intel MKL Pardiso library */
/* #undef USE_PARDISO */

/* Define if PaStiX can be used */
/* #undef USE_PASTIX */

/* define to use pthread_setaffinity_np */
#define USE_PTHREAD_SETAFFINITY_NP 1

/* define if you want to use QRUPDATE */
/* #undef USE_QRUPDATE */

/* define to use POSIX realtime */
#define USE_RT 1

/* define to use RTAI */
/* #undef USE_RTAI */

/* define if you want to load runtime */
#define USE_RUNTIME_LOADING 1

/* define if you want to use Schur parallel solver */
/* #undef USE_SCHUR */

/* define if you want to use Siconos */
/* #undef USE_SICONOS */

/* define if sleep is usable with C++ (issue with MinGW) */
#define USE_SLEEP 1

/* define if socket() can be used */
#define USE_SOCKET 1

/* define if you want to use STRUMPACK */
/* #undef USE_STRUMPACK */

/* define if you want to use SuiteSparseQR */
/* #undef USE_SUITESPARSE_QR */

/* define if you want to use SuperLU */
/* #undef USE_SUPERLU */

/* define if you want to use multithreaded SuperLU */
/* #undef USE_SUPERLU_MT */

/* define this to use external symbol table (default) */
#define USE_TABLE 1

/* define to use Taucs libraries */
/* #undef USE_TAUCS */

/* define if you want to use Trilinos */
/* #undef USE_TRILINOS */

/* define this to enable udunits in NetCDF based output */
/* #undef USE_UDUNITS */

/* define if you want to use Umfpack */
/* #undef USE_UMFPACK */

/* define if need to use umfpack_dl_* calls */
/* #undef USE_UMFPACK_LONG */

/* append underscore to fortran calls */
#define USE_UNDERSCORE 1

/* define to use IBM Watson wsmp library */
/* #undef USE_WSMP */

/* define to use Y12 sparse LU library */
#define USE_Y12 1

/* define if C++ compiler allows namespace std */
#define USING_STD 1

/* Version number of package */
#define VERSION "develop"


/* this file must be included by every .c/.cc file */
#include "mbdyn.h"

