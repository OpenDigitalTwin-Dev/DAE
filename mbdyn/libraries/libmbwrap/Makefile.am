include $(top_srcdir)/build/top.mk
## $Header$
## Process this file with automake to produce Makefile.in

# Build libmbwrap.a library
noinst_LTLIBRARIES = libmbwrap.la

libmbwrap_la_SOURCES = \
harwlib.h \
harwout.c \
harwout.h \
harwout_.cc \
harwrap.cc \
harwrap.h \
kluwrap.h \
kluwrap.cc \
lapackwrap.cc \
lapackwrap.h \
linsol.cc \
linsol.h \
naivewrap.cc \
naivewrap.h \
parnaivewrap.cc \
parnaivewrap.h \
parsuperluwrap.cc \
parsuperluwrap.h \
superluwrap.cc \
superluwrap.h \
umfpackwrap.cc \
umfpackwrap.h \
pardisowrap.h \
pardisowrap.cc \
pastixwrap.cc \
pastixwrap.h \
taucswrap.cc \
taucswrap.h \
y12wrap.cc \
y12wrap.h \
wsmpwrap.cc \
wsmpwrap.h \
qrwrap.h \
qrwrap.cc \
strumpackwrap.h \
strumpackwrap.cc \
aztecoowrap.h \
aztecoowrap.cc \
siconoswrap.h \
siconoswrap.cc

if USE_CHACO
libmbwrap_la_SOURCES += \
chacowrap.cc \
chacowrap.h
endif

if USE_METIS
libmbwrap_la_SOURCES += \
metiswrap.cc \
metiswrap.h
endif

libmbwrap_la_LIBADD = \
../libmbmath/libmbmath.la \
../libmbutil/libmbutil.la \
../libobjs/libobjs.la \
../libcolamd/libmbdyncolamd.la \
../libnaive/libnaive.la \
@UMFPACK_LIBS@ \
@KLU_LIBS@ \
@HARWELL_LIBS@ \
@SUPERLU_LIBS@ \
@TAUCS_LIBS@ \
@PARDISO_LIBS@ \
@WSMP_LIBS@ \
@PASTIX_LIBS@ \
@QRUPDATE_LIBS@ \
@SUITESPARSEQR_LIBS@ \
@STRUMPACK_LIBS@ \
@LAPACK_LIBS@ \
@Y12_LIBS@ \
@METIS_LIBS@ \
@BLAS_LIBS@ \
@TRILINOS_LIBS@ \
@SICONOS_LIBS@ \
@LIBS@ \
@FCLIBS@

libmbwrap_la_LDFLAGS =

noinst_PROGRAMS = wraptest cctest

if USE_ARPACK
noinst_PROGRAMS += arptest
endif

wraptest_SOURCES = wraptest.cc
wraptest_LDADD = \
libmbwrap.la \
@UMFPACK_LIBS@ \
@HARWELL_LIBS@ \
@SUPERLU_LIBS@ \
@TAUCS_LIBS@ \
@LAPACK_LIBS@ \
@Y12_LIBS@ \
@METIS_LIBS@ \
@BLAS_LIBS@ \
@FCLIBS@ \
@LIBS@ \
@GTEST_LIBS@

cctest_SOURCES = cctest.cc
cctest_LDADD = \
libmbwrap.la \
@UMFPACK_LIBS@ \
@HARWELL_LIBS@ \
@SUPERLU_LIBS@ \
@Y12_LIBS@ \
@BLAS_LIBS@ \
@LIBS@ \
@FCLIBS@ \
@GTEST_LIBS@

if USE_ARPACK
arptest_SOURCES = arptest.cc
arptest_LDADD = \
libmbwrap.la \
@ARPACK_LIBS@ \
@LAPACK_LIBS@ \
@UMFPACK_LIBS@ \
@HARWELL_LIBS@ \
@SUPERLU_LIBS@ \
@Y12_LIBS@ \
@BLAS_LIBS@ \
@LIBS@ \
@FCLIBS@ \
@GTEST_LIBS@

endif

if INSTALL_TEST_PROGRAMS
bin_PROGRAMS = $(noinst_PROGRAMS)
endif

AM_CPPFLAGS = \
-I../../include \
-I$(srcdir)/../../include \
-I$(srcdir)/../../mbdyn \
-I$(srcdir)/../libmbutil \
-I$(srcdir)/../libmbmath \
-I$(srcdir)/../liby12 \
-I$(srcdir)/../libcolamd \
-I$(srcdir)/../libnaive \
@PASTIX_INC@ \
@GTEST_INCLUDE@

include $(top_srcdir)/build/bot.mk
