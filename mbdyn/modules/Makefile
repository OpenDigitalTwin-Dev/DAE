# Makefile.in generated by automake 1.16.5 from Makefile.am.
# modules/Makefile.  Generated from Makefile.in by configure.

# Copyright (C) 1994-2021 Free Software Foundation, Inc.

# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.



# $Header$
# MBDyn (C) is a multibody analysis code.
# http://www.mbdyn.org
#
# Copyright (C) 1996-2023
#
# Pierangelo Masarati     <pierangelo.masarati@polimi.it>
# Paolo Mantegazza        <paolo.mantegazza@polimi.it>
#
# Dipartimento di Ingegneria Aerospaziale - Politecnico di Milano
# via La Masa, 34 - 20156 Milano, Italy
# http://www.aero.polimi.it
#
# Changing this copyright notice is forbidden.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation (version 2 of the License).
# 
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

am__is_gnu_make = { \
  if test -z '$(MAKELEVEL)'; then \
    false; \
  elif test -n '$(MAKE_HOST)'; then \
    true; \
  elif test -n '$(MAKE_VERSION)' && test -n '$(CURDIR)'; then \
    true; \
  else \
    false; \
  fi; \
}
am__make_running_with_option = \
  case $${target_option-} in \
      ?) ;; \
      *) echo "am__make_running_with_option: internal error: invalid" \
              "target option '$${target_option-}' specified" >&2; \
         exit 1;; \
  esac; \
  has_opt=no; \
  sane_makeflags=$$MAKEFLAGS; \
  if $(am__is_gnu_make); then \
    sane_makeflags=$$MFLAGS; \
  else \
    case $$MAKEFLAGS in \
      *\\[\ \	]*) \
        bs=\\; \
        sane_makeflags=`printf '%s\n' "$$MAKEFLAGS" \
          | sed "s/$$bs$$bs[$$bs $$bs	]*//g"`;; \
    esac; \
  fi; \
  skip_next=no; \
  strip_trailopt () \
  { \
    flg=`printf '%s\n' "$$flg" | sed "s/$$1.*$$//"`; \
  }; \
  for flg in $$sane_makeflags; do \
    test $$skip_next = yes && { skip_next=no; continue; }; \
    case $$flg in \
      *=*|--*) continue;; \
        -*I) strip_trailopt 'I'; skip_next=yes;; \
      -*I?*) strip_trailopt 'I';; \
        -*O) strip_trailopt 'O'; skip_next=yes;; \
      -*O?*) strip_trailopt 'O';; \
        -*l) strip_trailopt 'l'; skip_next=yes;; \
      -*l?*) strip_trailopt 'l';; \
      -[dEDm]) skip_next=yes;; \
      -[JT]) skip_next=yes;; \
    esac; \
    case $$flg in \
      *$$target_option*) has_opt=yes; break;; \
    esac; \
  done; \
  test $$has_opt = yes
am__make_dryrun = (target_option=n; $(am__make_running_with_option))
am__make_keepgoing = (target_option=k; $(am__make_running_with_option))
pkgdatadir = $(datadir)/mbdyn
pkgincludedir = $(includedir)/mbdyn
pkglibdir = $(libdir)/mbdyn
pkglibexecdir = $(libexecdir)/mbdyn
am__cd = CDPATH="$${ZSH_VERSION+.}$(PATH_SEPARATOR)" && cd
install_sh_DATA = $(install_sh) -c -m 644
install_sh_PROGRAM = $(install_sh) -c
install_sh_SCRIPT = $(install_sh) -c
INSTALL_HEADER = $(INSTALL_DATA)
transform = $(program_transform_name)
NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
build_triplet = x86_64-pc-linux-gnu
host_triplet = x86_64-pc-linux-gnu
target_triplet = x86_64-pc-linux-gnu
subdir = modules
ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
am__aclocal_m4_deps = $(top_srcdir)/acinclude.m4 \
	$(top_srcdir)/configure.ac
am__configure_deps = $(am__aclocal_m4_deps) $(CONFIGURE_DEPENDENCIES) \
	$(ACLOCAL_M4)
DIST_COMMON = $(srcdir)/Makefile.am $(am__DIST_COMMON)
mkinstalldirs = $(install_sh) -d
CONFIG_HEADER = $(top_builddir)/include/mbconfig.h
CONFIG_CLEAN_FILES =
CONFIG_CLEAN_VPATH_FILES =
AM_V_P = $(am__v_P_$(V))
am__v_P_ = $(am__v_P_$(AM_DEFAULT_VERBOSITY))
am__v_P_0 = false
am__v_P_1 = :
AM_V_GEN = $(am__v_GEN_$(V))
am__v_GEN_ = $(am__v_GEN_$(AM_DEFAULT_VERBOSITY))
am__v_GEN_0 = @echo "  GEN     " $@;
am__v_GEN_1 = 
AM_V_at = $(am__v_at_$(V))
am__v_at_ = $(am__v_at_$(AM_DEFAULT_VERBOSITY))
am__v_at_0 = @
am__v_at_1 = 
SOURCES =
DIST_SOURCES =
am__can_run_installinfo = \
  case $$AM_UPDATE_INFO_DIR in \
    n|no|NO) false;; \
    *) (install-info --version) >/dev/null 2>&1;; \
  esac
am__tagged_files = $(HEADERS) $(SOURCES) $(TAGS_FILES) $(LISP)
am__DIST_COMMON = $(srcdir)/Makefile.in $(top_srcdir)/build/bot.mk \
	$(top_srcdir)/build/top.mk README
DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)
ACLOCAL = ${SHELL} '/test/toolkit/DAE/mbdyn/build/missing' aclocal-1.16
AMTAR = $${TAR-tar}
AM_DEFAULT_VERBOSITY = 0
ANN_LIBS = 
AR = ar
ARPACK_LIBS = 
ATOMIC_OPS_LIBS = 
AUTOCONF = ${SHELL} '/test/toolkit/DAE/mbdyn/build/missing' autoconf
AUTOHEADER = ${SHELL} '/test/toolkit/DAE/mbdyn/build/missing' autoheader
AUTOMAKE = ${SHELL} '/test/toolkit/DAE/mbdyn/build/missing' automake-1.16
AWK = mawk
AX_SWIG_PYTHON_CPPFLAGS = 
AX_SWIG_PYTHON_OPT = 
BLAS_LIBS = -lblas  -L/usr/lib/gcc/x86_64-linux-gnu/11 -L/usr/lib/gcc/x86_64-linux-gnu/11/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/11/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/11/../../.. -lgfortran -lm -lquadmath  -L/usr/lib/gcc/x86_64-linux-gnu/11 -L/usr/lib/gcc/x86_64-linux-gnu/11/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/11/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/11/../../.. -lgfortran -lm -lquadmath -lm
BULLET_LIBS = 
CC = gcc
CCDEPMODE = depmode=gcc3
CFLAGS = -g -O2  -Werror -Wno-error=cpp -Wno-error=format-truncation
CHACO_LIBS = 
CPPFLAGS = 
CSCOPE = cscope
CTAGS = ctags
CXX = g++
CXXCPP = g++ -E
CXXDEPMODE = depmode=gcc3
CXXFLAGS = -g -O2  -Werror -Wno-error=cpp -Wno-error=format-truncation -fopenmp
CYGPATH_W = echo
DATE = 
DEFS = -DHAVE_CONFIG_H
DEPDIR = .deps
DLLTOOL = false
DSYMUTIL = 
DUMPBIN = 
DVIPS = 
ECHO_C = 
ECHO_N = -n
ECHO_T = 
EGREP = /usr/bin/grep -E
ETAGS = etags
EXEEXT = 
F77 = gfortran
FC = gfortran
FCFLAGS = -g -O2
FCLIBS =  -L/usr/lib/gcc/x86_64-linux-gnu/11 -L/usr/lib/gcc/x86_64-linux-gnu/11/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/11/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/11/../../.. -lgfortran -lm -lquadmath
FFLAGS = -g -O2
FGREP = /usr/bin/grep -F
FLIBS = 
GINACLIB_CPPFLAGS = 
GINACLIB_LIBS = 
GREP = /usr/bin/grep
GTEST_INCLUDE = 
GTEST_LIBS = 
HARWELL_LIBS = 
INSTALL = /usr/bin/install -c
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_PROGRAM = ${INSTALL}
INSTALL_SCRIPT = ${INSTALL}
INSTALL_STRIP_PROGRAM = $(install_sh) -c -s
JDQZ_LIBS = 
KLU_LIBS = 
LAPACK_LIBS = -llapack
LATEX = 
LD = /usr/bin/ld -m elf_x86_64
LDFLAGS =    -Wl,--export-dynamic
LIBOBJS = 
LIBS =   -L/usr/lib/gcc/x86_64-linux-gnu/11 -L/usr/lib/gcc/x86_64-linux-gnu/11/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/11/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/11/../../.. -lgfortran -lm -lquadmath  -lltdl
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LIPO = 
LN_S = ln -s
LTLIBOBJS = 
LT_SYS_LIBRARY_PATH = 
MAKEINFO = ${SHELL} '/test/toolkit/DAE/mbdyn/build/missing' makeinfo
MANIFEST_TOOL = :
METIS_LIBS = 
MFRONT_LIBS = 
MKDIR_P = /usr/bin/mkdir -p
MODULES_DIRS = 
MODULE_LOADPATH = ${exec_prefix}/libexec
MPI_LIBS = 
NETCDF_LIBS = -lnetcdf_c++4 -lnetcdf
NM = /usr/bin/nm -B
NMEDIT = 
OBJDUMP = objdump
OBJEXT = o
OCTAVE_CLI = 
OCTAVE_INCLUDE = 
OCTAVE_LIBS = 
OCTAVE_MKOCTFILE = 
OCTAVE_PKG_PREFIX = 
OTOOL = 
OTOOL64 = 
PACKAGE = mbdyn
PACKAGE_BUGREPORT = 
PACKAGE_NAME = mbdyn
PACKAGE_STRING = mbdyn develop
PACKAGE_TARNAME = mbdyn
PACKAGE_URL = 
PACKAGE_VERSION = develop
PARDISO_LIBS = 
PASTIX_INC = 
PASTIX_LIBS = 
PATH_SEPARATOR = :
PKG_CONFIG = /usr/bin/pkg-config
PKG_CONFIG_LIBDIR = 
PKG_CONFIG_PATH = 
PS2PDF = 
PTHREAD_CC = gcc
PTHREAD_CFLAGS = 
PTHREAD_LIBS = 
PYTHON = 
PYTHON_CPPFLAGS = 
PYTHON_EXTRA_LDFLAGS = 
PYTHON_EXTRA_LIBS = 
PYTHON_LIBS = 
PYTHON_NUMPY_HEADERS = 
PYTHON_PLATFORM_SITE_PKG = 
PYTHON_SITE_PKG = 
PYTHON_VERSION = 
QRUPDATE_LIBS = 
RANLIB = ranlib
RTAI_LDFLAGS = 
RT_LIBS = -lrt
SECURITY_LIBS = 
SED = /usr/bin/sed
SET_MAKE = 
SHELL = /bin/bash
SICONOS_LIBS = 
STRIP = strip
STRUMPACK_LIBS = 
SUITESPARSEQR_LIBS = 
SUPERLU_LIBS = 
SWIG = 
SWIG_LIB = 
TAUCS_LIBS = 
THREAD_LIBS = 
TRILINOS_LIBS = 
UDUNITS_LIBS = 
UMFPACK_LIBS = 
VERSION = develop
WSMP_LIBS = 
Y12_LIBS = /test/toolkit/DAE/mbdyn/libraries/liby12/liby12.la -lm
abs_builddir = /test/toolkit/DAE/mbdyn/modules
abs_srcdir = /test/toolkit/DAE/mbdyn/modules
abs_top_builddir = /test/toolkit/DAE/mbdyn
abs_top_srcdir = /test/toolkit/DAE/mbdyn
ac_ct_AR = ar
ac_ct_CC = gcc
ac_ct_CXX = g++
ac_ct_DUMPBIN = 
ac_ct_F77 = f77
ac_ct_FC = gfortran
am__include = include
am__leading_dot = .
am__quote = 
am__tar = $${TAR-tar} chof - "$$tardir"
am__untar = $${TAR-tar} xf -
ax_pthread_config = 
bindir = ${exec_prefix}/bin
build = x86_64-pc-linux-gnu
build_alias = 
build_cpu = x86_64
build_os = linux-gnu
build_vendor = pc
builddir = .
datadir = ${datarootdir}
datarootdir = ${prefix}/share
docdir = ${datarootdir}/doc/${PACKAGE_TARNAME}
dvidir = ${docdir}
exec_prefix = ${prefix}
host = x86_64-pc-linux-gnu
host_alias = 
host_cpu = x86_64
host_os = linux-gnu
host_vendor = pc
htmldir = ${docdir}
includedir = ${prefix}/include
infodir = ${datarootdir}/info
install_sh = ${SHELL} /test/toolkit/DAE/mbdyn/build/install-sh
libdir = ${exec_prefix}/lib
libexecdir = ${exec_prefix}/libexec
localedir = ${datarootdir}/locale
localstatedir = ${prefix}/var
mandir = ${datarootdir}/man
mbdyn_subdir = /mbdyn
mkdir_p = $(MKDIR_P)
oldincludedir = /usr/include
pdfdir = ${docdir}
prefix = /test/toolkit/DAE/mbdyn/../install/mbdyn_install
program_transform_name = s,x,x,
psdir = ${docdir}
runstatedir = ${localstatedir}/run
sbindir = ${exec_prefix}/sbin
sharedstatedir = ${prefix}/com
srcdir = .
sysconfdir = ${prefix}/etc
target = x86_64-pc-linux-gnu
target_alias = 
target_cpu = x86_64
target_os = linux-gnu
target_vendor = pc
top_build_prefix = ../
top_builddir = ..
top_srcdir = ..
wl = 
CXXLD = g++

# Add distributed modules below
EXTRA_DIST = Makefile.module Makefile.template \
	module-aerodyn/NREL_AeroDyn.h module-aerodyn/Makefile.inc \
	module-aerodyn/mbdyn_ad.f90 module-aerodyn/module-aerodyn.cc \
	module-aerodyn/README \
	module-asynchronous_machine/asynchronous_machine.mbdyn \
	module-asynchronous_machine/module-asynchronous_machine.cc \
	module-asynchronous_machine/module-asynchronous_machine.h \
	module-asynchronous_machine/doc/Doxyfile \
	module-asynchronous_machine/doc/Makefile \
	module-autodiff_test/autodiff_test.f \
	module-autodiff_test/Makefile.inc \
	module-autodiff_test/module-autodiff_test.cc \
	module-autodiff_test/module-autodiff_test.h \
	module-autodiff_test/examples/deformable_joint_ad.cod \
	module-autodiff_test/examples/deformable_joint_ad.elm \
	module-autodiff_test/examples/deformable_joint_ad.inv \
	module-autodiff_test/examples/deformable_joint_ad.nod \
	module-autodiff_test/examples/deformable_joint_ad.ref \
	module-autodiff_test/examples/deformable_joint_ad.set \
	module-autodiff_test/examples/deformable_joint_ad_f77.mbdyn \
	module-autodiff_test/examples/deformable_joint_ad_trad.mbdyn \
	module-autodiff_test/examples/inline_joint_ad.mbdyn \
	module-autodiff_test/examples/inline_joint_ad_2.mbdyn \
	module-autodiff_test/examples/inline_joint_ad_2_elements.elm \
	module-autodiff_test/examples/inline_joint_ad_2_parameters.set \
	module-autodiff_test/examples/inline_joint_ad_2_tmp.elm \
	module-autodiff_test/examples/inline_joint_ad_2_trad.elm \
	module-ballbearing_contact/module-ballbearing_contact.cc \
	module-ballbearing_contact/module-ballbearing_contact.h \
	module-ballbearing_contact/@BallBearingContact/AssJac.m \
	module-ballbearing_contact/@BallBearingContact/AssRes.m \
	module-ballbearing_contact/@BallBearingContact/BallBearingContact.m \
	module-ballbearing_contact/@BallBearingContact/ComputeResidual.m \
	module-ballbearing_contact/@BallBearingContact/DescribeDof.m \
	module-ballbearing_contact/@BallBearingContact/DescribeEq.m \
	module-ballbearing_contact/@BallBearingContact/dGetPrivData.m \
	module-ballbearing_contact/@BallBearingContact/display.m \
	module-ballbearing_contact/@BallBearingContact/GetConnectedNodes.m \
	module-ballbearing_contact/@BallBearingContact/GetDofType.m \
	module-ballbearing_contact/@BallBearingContact/GetEqType.m \
	module-ballbearing_contact/@BallBearingContact/GetStateVector.m \
	module-ballbearing_contact/@BallBearingContact/iGetNumConnectedNodes.m \
	module-ballbearing_contact/@BallBearingContact/iGetNumDof.m \
	module-ballbearing_contact/@BallBearingContact/iGetNumPrivData.m \
	module-ballbearing_contact/@BallBearingContact/iGetPrivDataIdx.m \
	module-ballbearing_contact/@BallBearingContact/SetValue.m \
	module-ballbearing_contact/@BallBearingContact/Update.m \
	module-ballbearing_contact/@BallBearingContact/WorkSpaceDim.m \
	module-constlaw/module-constlaw.cc module-constlaw/README \
	module-constlaw-f90/Makefile.inc \
	module-constlaw-f90/module-constlaw-f90.cc \
	module-constlaw-f90/README module-constlaw-f90/usrsub.f90 \
	module-constlaw-f90/usrsub.h module-constlaw-f95/Makefile.inc \
	module-constlaw-f95/module-constlaw-f95.cc \
	module-constlaw-f95/README module-constlaw-f95/usrsub.f95 \
	module-constlaw-f95/usrsub.h \
	module-cont-contact/module-cont-contact.cc \
	module-cont-contact/cont_contact_flap \
	module-cont-contact/freefall \
	module-cyclocopter/module-cyclocopter.cc \
	module-cyclocopter/module-cyclocopter.h \
	module-damper-graall/damper_graall.f \
	module-damper-graall/Makefile.inc \
	module-damper-graall/module-damper-graall.cc \
	module-damper-hydraulic/module-damper-hydraulic.cc \
	module-drive/module-drive.cc module-drive/README \
	module-eu2phi/module-eu2phi.cc module-FMU/FF.mbdyn \
	module-FMU/fmi_zip_unzip.h module-FMU/Makefile.inc \
	module-FMU/mbdynFMI.cc module-FMU/mbdynFMI_config.cc \
	module-FMU/mbdynFMI_config.h module-FMU/mbdynFMI.h \
	module-FMU/module-FMU.cc module-FMU/module-FMU.h \
	module-FMU/README.md module-hfelem/harmonicExcitationElem2.mbd \
	module-hfelem/harmonicExcitationElem.mbd \
	module-hfelem/module-hfelem.cc module-hfelem/module-hfelem.h \
	module-hid/README module-hid/Makefile.inc \
	module-hid/module-hid.cc module-hid/hid_detect.c \
	module-hid/hid.set module-hid/hid_ms_sidewinder.set \
	module-hid/hid_thrustmaster.set module-hid/hid_saitek_x45.set \
	module-hid/hid_aircraft module-hid/hid_test \
	module-hydrodynamic_plain_bearing/module-hydrodynamic_plain_bearing.cc \
	module-hydrodynamic_plain_bearing/module-hydrodynamic_plain_bearing.h \
	module-hydrodynamic_plain_bearing/hydrodynamic_plain_bearing.mbdyn \
	module-hunt-crossley/module-hunt-crossley.cc \
	module-hunt-crossley/hunt_crossley module-imu/module-imu.cc \
	module-imu/imu module-imu/imu_constraint module-imu/imu.tex \
	module-imu/README \
	module-inline_friction/module-inline_friction.cc \
	module-inline_friction/module-inline_friction.h \
	module-inline_friction/@InLineFriction/AssJac.m \
	module-inline_friction/@InLineFriction/AssRes.m \
	module-inline_friction/@InLineFriction/ComputeDeltaXP.m \
	module-inline_friction/@InLineFriction/ComputeInitialResidual.m \
	module-inline_friction/@InLineFriction/ComputeResidual.m \
	module-inline_friction/@InLineFriction/ComputeTau.m \
	module-inline_friction/@InLineFriction/DescribeDof.m \
	module-inline_friction/@InLineFriction/DescribeEq.m \
	module-inline_friction/@InLineFriction/dGetPrivData.m \
	module-inline_friction/@InLineFriction/display.m \
	module-inline_friction/@InLineFriction/GetConnectedNodes.m \
	module-inline_friction/@InLineFriction/GetDofType.m \
	module-inline_friction/@InLineFriction/GetEqType.m \
	module-inline_friction/@InLineFriction/GetInitialStateVector.m \
	module-inline_friction/@InLineFriction/GetStateVector.m \
	module-inline_friction/@InLineFriction/iGetInitialNumDof.m \
	module-inline_friction/@InLineFriction/iGetNumConnectedNodes.m \
	module-inline_friction/@InLineFriction/iGetNumDof.m \
	module-inline_friction/@InLineFriction/iGetNumPrivData.m \
	module-inline_friction/@InLineFriction/iGetPrivDataIdx.m \
	module-inline_friction/@InLineFriction/InitialAssJac.m \
	module-inline_friction/@InLineFriction/InitialAssRes.m \
	module-inline_friction/@InLineFriction/InitialWorkSpaceDim.m \
	module-inline_friction/@InLineFriction/InLineFriction.m \
	module-inline_friction/@InLineFriction/SetInitialValue.m \
	module-inline_friction/@InLineFriction/SetValue.m \
	module-inline_friction/@InLineFriction/Update.m \
	module-inline_friction/@InLineFriction/WorkSpaceDim.m \
	module-inline_friction/examples/inline_friction2_bench.elm \
	module-inline_friction/examples/inline_friction2.elm \
	module-inline_friction/examples/inline_friction2.mbdyn \
	module-inline_friction/examples/inline_friction2.nod \
	module-inline_friction/examples/inline_friction3.mbdyn \
	module-inline_friction/examples/inline_friction.mbdyn \
	module-inplane_friction/@InPlaneFriction/AssJac.m \
	module-inplane_friction/@InPlaneFriction/AssRes.m \
	module-inplane_friction/@InPlaneFriction/ComputeInitialResidual.m \
	module-inplane_friction/@InPlaneFriction/ComputeResidual.m \
	module-inplane_friction/@InPlaneFriction/DescribeDof.m \
	module-inplane_friction/@InPlaneFriction/DescribeEq.m \
	module-inplane_friction/@InPlaneFriction/dGetPrivData.m \
	module-inplane_friction/@InPlaneFriction/display.m \
	module-inplane_friction/@InPlaneFriction/GetConnectedNodes.m \
	module-inplane_friction/@InPlaneFriction/GetDofType.m \
	module-inplane_friction/@InPlaneFriction/GetEqType.m \
	module-inplane_friction/@InPlaneFriction/GetInitialStateVector.m \
	module-inplane_friction/@InPlaneFriction/GetStateVector.m \
	module-inplane_friction/@InPlaneFriction/iGetInitialNumDof.m \
	module-inplane_friction/@InPlaneFriction/iGetNumConnectedNodes.m \
	module-inplane_friction/@InPlaneFriction/iGetNumDof.m \
	module-inplane_friction/@InPlaneFriction/iGetNumPrivData.m \
	module-inplane_friction/@InPlaneFriction/iGetPrivDataIdx.m \
	module-inplane_friction/@InPlaneFriction/InitialAssJac.m \
	module-inplane_friction/@InPlaneFriction/InitialAssRes.m \
	module-inplane_friction/@InPlaneFriction/InitialWorkSpaceDim.m \
	module-inplane_friction/@InPlaneFriction/InPlaneFriction.m \
	module-inplane_friction/@InPlaneFriction/SetInitialValue.m \
	module-inplane_friction/@InPlaneFriction/SetValue.m \
	module-inplane_friction/@InPlaneFriction/Update.m \
	module-inplane_friction/@InPlaneFriction/WorkSpaceDim.m \
	module-inplane_friction/examples/inplane_friction2.mbdyn \
	module-inplane_friction/examples/inplane_friction.mbdyn \
	module-journal_bearing/module-journal_bearing.cc \
	module-journal_bearing/module-journal_bearing.h \
	module-journal_bearing/examples/journal_bearing1.mbdyn \
	module-journal_bearing/examples/journal_bearing2.mbdyn \
	module-loadinc/module-loadinc.cc module-mds/mds \
	module-mds/module-mds.cc module-minmaxdrive/minmax.mbdyn \
	module-minmaxdrive/module-minmaxdrive.cc \
	module-minmaxdrive/module-minmaxdrive.h \
	module-multi_step_drive/module-multi_step_drive.cc \
	module-multi_step_drive/module-multi_step_drive.h \
	module-multi_step_drive/multi_step.mbdyn \
	module-muscles/module-muscles.cc \
	module-namespace/module-namespace.cc module-namespace/tns \
	module-nodedistdrive/module-nodedistdrive.cc \
	module-nodedistdrive/module-nodedistdrive.h \
	module-nonsmooth-node/Makefile.inc \
	module-nonsmooth-node/mbdyn_siconos.cc \
	module-nonsmooth-node/mbdyn_siconos.h \
	module-nonsmooth-node/module-nonsmooth-node.cc \
	module-octave/Makefile.inc module-octave/module-octave.cc \
	module-octave/module-octave.h module-octave/octave_object.cc \
	module-octave/octave_object.h module-octave/README \
	module-octave/demo/my_func2.m module-octave/demo/my_func2.m \
	module-octave/demo/my_func3.m module-octave/demo/my_func4.m \
	module-octave/demo/my_func5.m module-octave/demo/my_func.m \
	module-octave/demo/my_tpl_drive1D.m \
	module-octave/demo/my_tpl_drive3x3D.m \
	module-octave/demo/octave_const_law.mbdyn \
	module-octave/demo/octave_drive_caller.mbdyn \
	module-octave/demo/octave_element.mbdyn \
	module-octave/demo/@MyConstLaw1/GetConstLawType.m \
	module-octave/demo/@MyConstLaw1/MyConstLaw1.m \
	module-octave/demo/@MyConstLaw1/Update.m \
	module-octave/demo/@MyElem1/AfterConvergence.m \
	module-octave/demo/@MyElem1/AssJac.m \
	module-octave/demo/@MyElem1/AssRes.m \
	module-octave/demo/@MyElem1/DescribeDof.m \
	module-octave/demo/@MyElem1/DescribeEq.m \
	module-octave/demo/@MyElem1/dGetPrivData.m \
	module-octave/demo/@MyElem1/display.m \
	module-octave/demo/@MyElem1/GetConnectedNodes.m \
	module-octave/demo/@MyElem1/GetDofType.m \
	module-octave/demo/@MyElem1/GetEqType.m \
	module-octave/demo/@MyElem1/iGetInitialNumDof.m \
	module-octave/demo/@MyElem1/iGetNumConnectedNodes.m \
	module-octave/demo/@MyElem1/iGetNumDof.m \
	module-octave/demo/@MyElem1/iGetNumPrivData.m \
	module-octave/demo/@MyElem1/iGetPrivDataIdx.m \
	module-octave/demo/@MyElem1/InitialAssJac.m \
	module-octave/demo/@MyElem1/InitialAssRes.m \
	module-octave/demo/@MyElem1/InitialWorkSpaceDim.m \
	module-octave/demo/@MyElem1/MyElem1.m \
	module-octave/demo/@MyElem1/Output.m \
	module-octave/demo/@MyElem1/SetInitialValue.m \
	module-octave/demo/@MyElem1/SetValue.m \
	module-octave/demo/@MyElem1/Update.m \
	module-octave/demo/@MyElem1/WorkSpaceDim.m \
	module-scalarfunc/module-scalarfunc.cc \
	module-scalarfunc/README \
	module-switch_drive/module-switch_drive.cc \
	module-switch_drive/module-switch_drive.h \
	module-switch_drive/switch.mbdyn module-tclpgin/Makefile.inc \
	module-tclpgin/module-tclpgin.cc \
	module-template/module-template.cc \
	module-template2/module-template2.cc \
	module-udunits/Makefile.inc module-udunits/module-udunits.cc \
	module-udunits/README \
	module-uni_in_plane/module-uni_in_plane.cc \
	module-uni_in_plane/module-uni_in_plane.h \
	module-uni_in_plane/uni_in_plane.mbdyn \
	module-wheel2/module-wheel2.cc module-wheel2/module-wheel2.h \
	module-wheel4/module-wheel4.cc module-wheel4/module-wheel4.h \
	module-wheel4/axleExampleNoData module-flightgear/Makefile.inc \
	module-flightgear/module-flightgear.cc \
	module-flightgear/module-flightgear.h \
	module-flightgear/sendToFlightgear.cc \
	module-flightgear/recFromFlightgear.cc \
	module-chrono-interface/Makefile.inc \
	module-chrono-interface/module-chrono-interface.cc \
	module-chrono-interface/module-chrono-interface.h
all: all-am

.SUFFIXES:
$(srcdir)/Makefile.in:  $(srcdir)/Makefile.am $(top_srcdir)/build/top.mk $(top_srcdir)/build/bot.mk $(am__configure_deps)
	@for dep in $?; do \
	  case '$(am__configure_deps)' in \
	    *$$dep*) \
	      ( cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh ) \
	        && { if test -f $@; then exit 0; else break; fi; }; \
	      exit 1;; \
	  esac; \
	done; \
	echo ' cd $(top_srcdir) && $(AUTOMAKE) --foreign modules/Makefile'; \
	$(am__cd) $(top_srcdir) && \
	  $(AUTOMAKE) --foreign modules/Makefile
Makefile: $(srcdir)/Makefile.in $(top_builddir)/config.status
	@case '$?' in \
	  *config.status*) \
	    cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh;; \
	  *) \
	    echo ' cd $(top_builddir) && $(SHELL) ./config.status $(subdir)/$@ $(am__maybe_remake_depfiles)'; \
	    cd $(top_builddir) && $(SHELL) ./config.status $(subdir)/$@ $(am__maybe_remake_depfiles);; \
	esac;
$(top_srcdir)/build/top.mk $(top_srcdir)/build/bot.mk $(am__empty):

$(top_builddir)/config.status: $(top_srcdir)/configure $(CONFIG_STATUS_DEPENDENCIES)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh

$(top_srcdir)/configure:  $(am__configure_deps)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh
$(ACLOCAL_M4):  $(am__aclocal_m4_deps)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh
$(am__aclocal_m4_deps):

mostlyclean-libtool:
	-rm -f *.lo

clean-libtool:
	-rm -rf .libs _libs
tags TAGS:

ctags CTAGS:

cscope cscopelist:

distdir: $(BUILT_SOURCES)
	$(MAKE) $(AM_MAKEFLAGS) distdir-am

distdir-am: $(DISTFILES)
	@srcdirstrip=`echo "$(srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`; \
	topsrcdirstrip=`echo "$(top_srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`; \
	list='$(DISTFILES)'; \
	  dist_files=`for file in $$list; do echo $$file; done | \
	  sed -e "s|^$$srcdirstrip/||;t" \
	      -e "s|^$$topsrcdirstrip/|$(top_builddir)/|;t"`; \
	case $$dist_files in \
	  */*) $(MKDIR_P) `echo "$$dist_files" | \
			   sed '/\//!d;s|^|$(distdir)/|;s,/[^/]*$$,,' | \
			   sort -u` ;; \
	esac; \
	for file in $$dist_files; do \
	  if test -f $$file || test -d $$file; then d=.; else d=$(srcdir); fi; \
	  if test -d $$d/$$file; then \
	    dir=`echo "/$$file" | sed -e 's,/[^/]*$$,,'`; \
	    if test -d "$(distdir)/$$file"; then \
	      find "$(distdir)/$$file" -type d ! -perm -700 -exec chmod u+rwx {} \;; \
	    fi; \
	    if test -d $(srcdir)/$$file && test $$d != $(srcdir); then \
	      cp -fpR $(srcdir)/$$file "$(distdir)$$dir" || exit 1; \
	      find "$(distdir)/$$file" -type d ! -perm -700 -exec chmod u+rwx {} \;; \
	    fi; \
	    cp -fpR $$d/$$file "$(distdir)$$dir" || exit 1; \
	  else \
	    test -f "$(distdir)/$$file" \
	    || cp -p $$d/$$file "$(distdir)/$$file" \
	    || exit 1; \
	  fi; \
	done
check-am: all-am
check: check-am
all-am: Makefile
installdirs:
install-exec: install-exec-am
install-data: install-data-am
uninstall: uninstall-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am

installcheck: installcheck-am
install-strip:
	if test -z '$(STRIP)'; then \
	  $(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	    install_sh_PROGRAM="$(INSTALL_STRIP_PROGRAM)" INSTALL_STRIP_FLAG=-s \
	      install; \
	else \
	  $(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	    install_sh_PROGRAM="$(INSTALL_STRIP_PROGRAM)" INSTALL_STRIP_FLAG=-s \
	    "INSTALL_PROGRAM_ENV=STRIPPROG='$(STRIP)'" install; \
	fi
mostlyclean-generic:

clean-generic:

distclean-generic:
	-test -z "$(CONFIG_CLEAN_FILES)" || rm -f $(CONFIG_CLEAN_FILES)
	-test . = "$(srcdir)" || test -z "$(CONFIG_CLEAN_VPATH_FILES)" || rm -f $(CONFIG_CLEAN_VPATH_FILES)

maintainer-clean-generic:
	@echo "This command is intended for maintainers to use"
	@echo "it deletes files that may require special tools to rebuild."
clean: clean-am

clean-am: clean-generic clean-libtool mostlyclean-am

distclean-am: clean-am distclean-generic

dvi: dvi-am

dvi-am:

html: html-am

html-am:

info: info-am

info-am:

install-data-am:

install-dvi: install-dvi-am

install-dvi-am:

install-exec-am:

install-html: install-html-am

install-html-am:

install-info: install-info-am

install-info-am:

install-man:

install-pdf: install-pdf-am

install-pdf-am:

install-ps: install-ps-am

install-ps-am:

installcheck-am:

maintainer-clean: maintainer-clean-am
	-rm -f Makefile
maintainer-clean-am: distclean-am maintainer-clean-generic

mostlyclean: mostlyclean-am

mostlyclean-am: mostlyclean-generic mostlyclean-libtool

pdf: pdf-am

pdf-am:

ps: ps-am

ps-am:

uninstall-am:

.MAKE: install-am install-strip

.PHONY: all all-am check check-am clean clean-generic clean-libtool \
	cscopelist-am ctags-am distclean distclean-generic \
	distclean-libtool distdir dvi dvi-am html html-am info info-am \
	install install-am install-data install-data-am install-dvi \
	install-dvi-am install-exec install-exec-am install-html \
	install-html-am install-info install-info-am install-man \
	install-pdf install-pdf-am install-ps install-ps-am \
	install-strip installcheck installcheck-am installdirs \
	maintainer-clean maintainer-clean-generic mostlyclean \
	mostlyclean-generic mostlyclean-libtool pdf pdf-am ps ps-am \
	tags-am uninstall uninstall-am

.PRECIOUS: Makefile


export LTCXXCOMPILE = $(SHELL) $(top_builddir)/libtool --tag=CXX --mode=compile g++ -DHAVE_CONFIG_H \
	-I. -I. \
	-I../../include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/libraries/libmbutil \
	-I$(top_srcdir)/libraries/libmbmath \
	-I$(top_srcdir)/libraries/libmbwrap \
	-I$(top_srcdir)/libraries/libobjs \
	-I$(top_srcdir)/libraries/libmbc \
	-I$(top_srcdir)/mbdyn \
	-I$(top_srcdir)/mbdyn/base \
	-I$(top_srcdir)/mbdyn/struct \
	-I$(top_srcdir)/mbdyn/aero \
	-I$(top_srcdir)/mbdyn/elec \
	-I$(top_srcdir)/mbdyn/hydr \
	-I../$(top_srcdir)/include \
	-I../$(top_srcdir)/libraries/libmbutil \
	-I../$(top_srcdir)/libraries/libmbmath \
	-I../$(top_srcdir)/libraries/libmbwrap \
	-I../$(top_srcdir)/libraries/libobjs \
	-I../$(top_srcdir)/libraries/libmbc \
	-I../$(top_srcdir)/mbdyn \
	-I../$(top_srcdir)/mbdyn/base \
	-I../$(top_srcdir)/mbdyn/struct \
	-I../$(top_srcdir)/mbdyn/aero \
	-I../$(top_srcdir)/mbdyn/elec \
	-I../$(top_srcdir)/mbdyn/hydr \
	 \
	-prefer-pic \
	$(AM_CPPFLAGS)  $(AM_CXXFLAGS) -g -O2  -Werror -Wno-error=cpp -Wno-error=format-truncation -fopenmp
export CXXLINK = $(SHELL) $(top_builddir)/libtool --mode=link $(CXXLD) $(AM_CXXFLAGS) \
	-g -O2  -Werror -Wno-error=cpp -Wno-error=format-truncation -fopenmp    -Wl,--export-dynamic -shared  -L/usr/lib/gcc/x86_64-linux-gnu/11 -L/usr/lib/gcc/x86_64-linux-gnu/11/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/11/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/11/../../.. -lgfortran -lm -lquadmath
export LTCOMPILE = $(SHELL) $(top_builddir)/libtool --mode=compile gcc -DHAVE_CONFIG_H \
	-I. -I. \
	-I../../include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/libraries/libmbc \
	-I$(top_srcdir)/libraries/libmbutil \
	-I$(top_srcdir)/libraries/libmbmath \
	-I$(top_srcdir)/libraries/libmbwrap \
	-I$(top_srcdir)/libraries/libobjs \
	-I$(top_srcdir)/mbdyn \
	-I$(top_srcdir)/mbdyn/base \
	-I$(top_srcdir)/mbdyn/struct \
	-I$(top_srcdir)/mbdyn/aero \
	-I$(top_srcdir)/mbdyn/elec \
	-I$(top_srcdir)/mbdyn/hydr \
	-I../$(top_srcdir)/include \
	-I../$(top_srcdir)/libraries/libmbc \
	-I../$(top_srcdir)/libraries/libmbutil \
	-I../$(top_srcdir)/libraries/libmbmath \
	-I../$(top_srcdir)/libraries/libmbwrap \
	-I../$(top_srcdir)/libraries/libobjs \
	-I../$(top_srcdir)/mbdyn \
	-I../$(top_srcdir)/mbdyn/base \
	-I../$(top_srcdir)/mbdyn/struct \
	-I../$(top_srcdir)/mbdyn/aero \
	-I../$(top_srcdir)/mbdyn/elec \
	-I../$(top_srcdir)/mbdyn/hydr \
	-prefer-pic \
	$(AM_CPPFLAGS)  $(AM_CFLAGS) -g -O2  -Werror -Wno-error=cpp -Wno-error=format-truncation
export LTF77COMPILE = $(SHELL) $(top_builddir)/libtool --tag=F77 --mode=compile gfortran $(AM_FFLAGS) -g -O2 \
	-prefer-pic
export LTFCCOMPILE = $(SHELL) $(top_builddir)/libtool --tag=FC --mode=compile gfortran $(AM_FFLAGS) -g -O2 \
	-prefer-pic
export LIBTOOL = $(SHELL) $(top_builddir)/libtool
export MODULES_DIRS=
export INSTALL_PROGRAM=${INSTALL}
export LIBEXEC_DIR=$(libexecdir)

.PHONY: all all__ clean distclean

# Note: this hack is required because macro "LIBTOOL" may expand
# to sh ../libtool, so, when we change directory to the modules,
# the effective libtool is not found.

all: libtool all__

libtool:
	-@ln -sf ../libtool .

all__:
	@echo "Configured modules: $(MODULES_DIRS)"
	-@if test x"$(MODULES_DIRS)" != x ; then \
		for i in $(MODULES_DIRS) ; do \
			echo "Playing with $$i ..."; \
			export MBDYN_MODULE=$$i ; \
			isrc=$(srcdir)/$$i ; \
			if test -d $$isrc ; then mkdir -p $$i ; fi ; \
			case $$isrc in /*) isrca=$$isrc ;; *) isrca=../$$isrc ;; esac ; \
			for fsrc in $$isrc/*.{h,c,cc,f,f90,f95} ; do \
				f=`basename $$fsrc` ; \
				case $$f in \*.h|\*.c|\*.cc|\*.f|\*.f90|\*.f95) ;; *) if test ! -f $$i/$$f && test -f $$isrc/$$f ; then ( cd $$i && ln -sf $$isrca/$$f $$f ) ; fi ; ;; esac ; \
			done ; \
			cp $(srcdir)/Makefile.module $$i/Makefile ; \
			( cd $$i && $(MAKE) OCTAVE_INCLUDE="$(OCTAVE_INCLUDE)" \
					    OCTAVE_LIBS="$(OCTAVE_LIBS)" \
					    MBDYN_MODULE_SRCDIR=$$isrca ) ; \
			rm -f $$i/Makefile ; \
		done \
	fi

install: all__
	$(mkinstalldirs) $(DESTDIR)$(libexecdir)
	-@if test x"$(MODULES_DIRS)" != x ; then \
		for i in $(MODULES_DIRS) ; do \
			echo "Installing $$i ..."; \
			export MBDYN_MODULE=$$i ; \
			isrc=$(srcdir)/$$i ; \
			if test -d $$isrc ; then mkdir -p $$i ; fi ; \
			case $$isrc in /*) isrca=$$isrc ;; *) isrca=../$$isrc ;; esac ; \
			for fsrc in $$isrc/*.{h,c,cc,f,f90,f95} ; do \
				f=`basename $$fsrc` ; \
				case $$f in \*.h|\*.c|\*.cc|\*.f|\*.f90|\*.f95) ;; *) if test ! -f $$i/$$f && test -f $$isrc/$$f ; then ( cd $$i && ln -sf $$isrca/$$f $$f ) ; fi ; ;; esac ; \
			done ; \
			cp $(srcdir)/Makefile.module $$i/Makefile ; \
			( cd $$i && $(MAKE) install ) ; \
			rm -f $$i/Makefile ; \
		done \
	fi

clean clean-so:
	-@if test x"$(MODULES_DIRS)" != x ; then \
		for i in $(MODULES_DIRS) ; do \
			echo "Cleaning $$i ..."; \
			cp $(srcdir)/Makefile.module $$i/Makefile ; \
			cd $$i && $(MAKE) $@ ; \
			cd .. ; \
			rm -f $$i/Makefile ; \
		done \
	fi

distclean:
	-@if test x"$(MODULES_DIRS)" != x ; then \
		for i in $(MODULES_DIRS) ; do \
			echo "Cleaning $$i ..."; \
			rm -rf $$i/*.so $$i/*.lo $$i/*.o ; \
		done \
	fi
	rm -f Makefile libtool

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
