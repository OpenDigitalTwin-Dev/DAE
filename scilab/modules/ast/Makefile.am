##
##  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
##  Copyright (C) 2007-2008 - DIGITEO - Bruno JOFRET
##  Copyright (C) 2007-2008 - DIGITEO - Antoine ELIAS
##
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.
##
##

pkglib_LTLIBRARIES 	= libsciast.la

libsciast_la_SOURCES = \
src/c/operations/doublecomplex.c \
src/c/operations/invert_matrix.c \
src/c/operations/matrix_addition.c \
src/c/operations/matrix_division.c \
src/c/operations/matrix_finite.c \
src/c/operations/matrix_kronecker.c \
src/c/operations/matrix_multiplication.c \
src/c/operations/matrix_power.c \
src/c/operations/matrix_transpose.c \
src/c/operations/operations_tools.c \
src/c/types/dtoa.c \
src/cpp/ast/expHistory.cpp \
src/cpp/ast/macrovarvisitor.cpp \
src/cpp/ast/printvisitor.cpp \
src/cpp/ast/runvisitor.cpp \
src/cpp/ast/shortcutvisitor.cpp \
src/cpp/ast/treevisitor.cpp \
src/cpp/ast/visitor_common.cpp \
src/cpp/ast/consoledebugger.cpp \
src/cpp/ast/debuggervisitor.cpp \
src/cpp/ast/debugmanager.cpp \
src/cpp/ast/prettyprintvisitor.cpp \
src/cpp/operations/interface_addition.cpp \
src/cpp/operations/operations.cpp \
src/cpp/operations/types_addition.cpp \
src/cpp/operations/types_and.cpp \
src/cpp/operations/types_comparison_eq.cpp \
src/cpp/operations/types_comparison_lt_le_gt_ge.cpp \
src/cpp/operations/types_comparison_ne.cpp \
src/cpp/operations/types_divide.cpp \
src/cpp/operations/types_dotdivide.cpp \
src/cpp/operations/types_dotmultiplication.cpp \
src/cpp/operations/types_finite.cpp \
src/cpp/operations/types_kronecker.cpp \
src/cpp/operations/types_ldivide.cpp \
src/cpp/operations/types_multiplication.cpp \
src/cpp/operations/types_opposite.cpp \
src/cpp/operations/types_or.cpp \
src/cpp/operations/types_power.cpp \
src/cpp/operations/types_subtraction.cpp \
src/cpp/parse/parser.cpp \
src/cpp/parse/parsescilab.cpp \
src/cpp/parse/printerror.cpp \
src/cpp/parse/scanscilab.cpp \
src/cpp/symbol/context.cpp \
src/cpp/symbol/libraries.cpp \
src/cpp/symbol/symbol.cpp \
src/cpp/symbol/variables.cpp \
src/cpp/system_env/configvariable.cpp \
src/cpp/system_env/configvariable_interface.cpp \
src/cpp/system_env/context_get.cpp \
src/cpp/system_env/dynamic_module.cpp \
src/cpp/system_env/home.cpp \
src/cpp/system_env/lasterror.cpp \
src/cpp/system_env/sci_home.cpp \
src/cpp/system_env/sci_path.cpp \
src/cpp/system_env/sci_tmpdir.cpp \
src/cpp/system_env/setenvvar.cpp \
src/cpp/system_env/threadmanagement.cpp \
src/cpp/system_env/numericconstants.cpp \
src/cpp/system_env/numericconstants_interface.cpp \
src/cpp/system_env/coverage_instance.cpp \
src/cpp/types/addfunction.cpp \
src/cpp/types/internal.cpp \
src/cpp/types/arrayof.cpp \
src/cpp/types/bool.cpp \
src/cpp/types/callable.cpp \
src/cpp/types/cell.cpp \
src/cpp/types/double.cpp \
src/cpp/types/file.cpp \
src/cpp/types/function.cpp \
src/cpp/types/graphichandle.cpp \
src/cpp/types/implicitlist.cpp \
src/cpp/types/int.cpp \
src/cpp/types/library.cpp \
src/cpp/types/list.cpp \
src/cpp/types/listdelete.cpp \
src/cpp/types/listinsert.cpp \
src/cpp/types/listoperation.cpp \
src/cpp/types/macro.cpp \
src/cpp/types/macrofile.cpp \
src/cpp/types/mlist.cpp \
src/cpp/types/overload.cpp \
src/cpp/types/polynom.cpp \
src/cpp/types/singlepoly.cpp \
src/cpp/types/singlestruct.cpp \
src/cpp/types/sparse.cpp \
src/cpp/types/string.cpp \
src/cpp/types/struct.cpp \
src/cpp/types/test-types.cpp \
src/cpp/types/threadId.cpp \
src/cpp/types/tlist.cpp \
src/cpp/types/tostring_common.cpp \
src/cpp/types/types.cpp \
src/cpp/types/types_tools.cpp \
src/cpp/types/void.cpp

if ENABLE_DEBUG
libsciast_la_SOURCES += src/cpp/types/inspector.cpp
endif

if IS_MACOSX
if !IS_SYS_FMEMOPEN
libsciast_la_SOURCES += src/cpp/parse/fmemopen.c
endif
endif


libsciast_la_CPPFLAGS = \
	-I$(srcdir)/includes/ast \
	-I$(srcdir)/includes/exps \
	-I$(srcdir)/includes/operations \
	-I$(srcdir)/includes/parse \
	-I$(srcdir)/includes/symbol \
	-I$(srcdir)/includes/system_env \
	-I$(srcdir)/includes/types \
	-I$(srcdir)/src/cpp \
	-I$(top_srcdir)/modules/dynamic_link/includes \
	-I$(top_srcdir)/modules/functions_manager/includes \
	-I$(top_srcdir)/modules/console/includes \
	-I$(top_srcdir)/modules/elementary_functions/includes \
	-I$(top_srcdir)/modules/output_stream/includes \
	-I$(top_srcdir)/modules/api_scilab/includes \
	-I$(top_srcdir)/modules/string/includes \
	-I$(top_srcdir)/modules/threads/includes \
	-I$(top_srcdir)/modules/localization/includes \
	-I$(top_srcdir)/modules/io/includes \
	-I$(top_srcdir)/modules/fileio/includes \
	-I$(top_srcdir)/modules/fileio/src/c \
	-I$(top_srcdir)/modules/coverage/includes \
	-I$(top_srcdir)/modules/graphic_objects/includes \
	$(EIGEN_CPPFLAGS) \
	$(AM_CPPFLAGS)

if IS_MACOSX
if IS_SYS_FMEMOPEN
libsciast_la_CPPFLAGS += -DHAVE_SYS_FMEMOPEN
endif
endif


libsciast_la_includedir=$(pkgincludedir)
libsciast_la_include_HEADERS =


#### Target ######
modulename=ast

####io : init scripts ####
libsciast_la_etcdir = $(mydatadir)/etc
libsciast_la_etc_DATA = \
    etc/ast.quit \
    etc/ast.start


#### ast : include files ####
libsciast_scilab_la_includedir=$(pkgincludedir)
libsciast_scilab_la_include_HEADERS = \
includes/ast/deserializervisitor.hxx \
includes/ast/dummyvisitor.hxx \
includes/ast/dynlib_ast.h \
includes/ast/execvisitor.hxx \
includes/ast/expHistory.hxx \
includes/ast/macrovarvisitor.hxx \
includes/ast/printvisitor.hxx \
includes/ast/runvisitor.hxx \
includes/ast/scilabexception.hxx \
includes/ast/serializervisitor.hxx \
includes/ast/shortcutvisitor.hxx \
includes/ast/stepvisitor.hxx \
includes/ast/timedvisitor.hxx \
includes/ast/visitor_common.hxx \
includes/ast/visitor.hxx \
includes/ast/abstractdebugger.hxx \
includes/ast/breakpoint.hxx \
includes/ast/consoledebugger.hxx \
includes/ast/debuggervisitor.hxx \
includes/ast/debugmanager.hxx \
includes/ast/prettyprintvisitor.hxx \
includes/exps/alldec.hxx \
includes/exps/allexp.hxx \
includes/exps/all.hxx \
includes/exps/allvar.hxx \
includes/exps/argumentsexp.hxx \
includes/exps/argumentdec.hxx \
includes/exps/arraylistexp.hxx \
includes/exps/arraylistvar.hxx \
includes/exps/assignexp.hxx \
includes/exps/assignlistexp.hxx \
includes/exps/ast.hxx \
includes/exps/boolexp.hxx \
includes/exps/breakexp.hxx \
includes/exps/callexp.hxx \
includes/exps/caseexp.hxx \
includes/exps/cellcallexp.hxx \
includes/exps/cellexp.hxx \
includes/exps/colonvar.hxx \
includes/exps/commentexp.hxx \
includes/exps/constexp.hxx \
includes/exps/continueexp.hxx \
includes/exps/controlexp.hxx \
includes/exps/dec.hxx \
includes/exps/decls.hxx \
includes/exps/dollarvar.hxx \
includes/exps/doubleexp.hxx \
includes/exps/exp.hxx \
includes/exps/fieldexp.hxx \
includes/exps/forexp.hxx \
includes/exps/functiondec.hxx \
includes/exps/ifexp.hxx \
includes/exps/listexp.hxx \
includes/exps/location.hxx \
includes/exps/logicalopexp.hxx \
includes/exps/mathexp.hxx \
includes/exps/matrixexp.hxx \
includes/exps/matrixlineexp.hxx \
includes/exps/nilexp.hxx \
includes/exps/notexp.hxx \
includes/exps/opexp.hxx \
includes/exps/returnexp.hxx \
includes/exps/selectexp.hxx \
includes/exps/seqexp.hxx \
includes/exps/simplevar.hxx \
includes/exps/stepvisitor.hxx \
includes/exps/stringexp.hxx \
includes/exps/token.hxx \
includes/exps/transposeexp.hxx \
includes/exps/trycatchexp.hxx \
includes/exps/vardec.hxx \
includes/exps/var.hxx \
includes/exps/whileexp.hxx \
includes/operations/doublecomplex.h \
includes/operations/generic_operations.hxx \
includes/operations/interface_addition.h \
includes/operations/invert_matrix.h \
includes/operations/matrix_addition.h \
includes/operations/matrix_division.h \
includes/operations/matrix_finite.h \
includes/operations/matrix_kronecker.h \
includes/operations/matrix_left_division.h \
includes/operations/matrix_multiplication.h \
includes/operations/matrix_power.h \
includes/operations/matrix_right_division.h \
includes/operations/matrix_transpose.h \
includes/operations/operation_f.h \
includes/operations/operations.hxx \
includes/operations/operations_tools.h \
includes/operations/types_addition.hxx \
includes/operations/types_and.hxx \
includes/operations/types_comparison_eq.hxx \
includes/operations/types_comparison_lt_le_gt_ge.hxx \
includes/operations/types_comparison_ne.hxx \
includes/operations/types_divide.hxx \
includes/operations/types_dotdivide.hxx \
includes/operations/types_dotmultiplication.hxx \
includes/operations/types_finite.hxx \
includes/operations/types_kronecker.hxx \
includes/operations/types_ldivide.hxx \
includes/operations/types_multiplication.hxx \
includes/operations/types_opposite.hxx \
includes/operations/types_or.hxx \
includes/operations/types_power.hxx \
includes/operations/types_subtraction.hxx \
includes/operations/types_transposition.hxx \
includes/parse/isatty.hxx \
includes/parse/parse.hxx \
includes/parse/parser.hxx \
includes/parse/parser_private.hxx \
includes/parse/parsescilab.hxx \
includes/symbol/context.hxx \
includes/symbol/libraries.hxx \
includes/symbol/symbol.hxx \
includes/symbol/variables.hxx \
includes/system_env/configvariable.hxx \
includes/system_env/configvariable_interface.h \
includes/system_env/context_get.h \
includes/system_env/dynamic_module.h \
includes/system_env/home.h \
includes/system_env/lasterror.h \
includes/system_env/sci_home.h \
includes/system_env/sci_path.h \
includes/system_env/sci_tmpdir.h \
includes/system_env/setenvvar.h \
includes/types/addfunction.h \
includes/types/alltypes.hxx \
includes/types/arrayof.hxx \
includes/types/bool.hxx \
includes/types/callable.hxx \
includes/types/cell.hxx \
includes/types/c_gateway_prototype.h \
includes/types/colon.hxx \
includes/types/container.hxx \
includes/types/cpp_gateway_prototype.hxx \
includes/types/double.hxx \
includes/types/dtoa.h \
includes/types/file.hxx \
includes/types/function.hxx \
includes/types/gatewaystruct.hxx \
includes/types/graphichandle.hxx \
includes/types/implicitlist.hxx \
includes/types/inspector.hxx \
includes/types/internal.hxx \
includes/types/int.hxx \
includes/types/keepForSparse.hxx \
includes/types/library.hxx \
includes/types/listdelete.hxx \
includes/types/list.hxx \
includes/types/listinsert.hxx \
includes/types/listoperation.hxx \
includes/types/macrofile.hxx \
includes/types/macro.hxx \
includes/types/matrixiterator.hxx \
includes/types/mlist.hxx \
includes/types/overload.hxx \
includes/types/pointer.hxx \
includes/types/polynom.hxx \
includes/types/singlepoly.hxx \
includes/types/singlestruct.hxx \
includes/types/sparse.hxx \
includes/types/sparseOp.hxx \
includes/types/string.hxx \
includes/types/struct.hxx \
includes/types/threadId.hxx \
includes/types/tlist.hxx \
includes/types/tostring_common.hxx \
includes/types/typesdecl.hxx \
includes/types/types.hxx \
includes/types/types_tools.hxx \
includes/types/type_traits.hxx \
includes/types/user.hxx \
includes/types/void.hxx \
src/cpp/ast/run_AssignExp.hpp \
src/cpp/ast/run_CallExp.hpp \
src/cpp/ast/run_MatrixExp.hpp \
src/cpp/ast/run_OpExp.hpp

if BUILD_PARSER

BUILT_SOURCES = $(FLEX_SCANSCILAB_CXX) $(BISON_PARSESCILAB_CXX)

LFLAGS = --noline --nounistd -o $(FLEX_SCANSCILAB_CXX)
FLEX_SCANSCILAB_LL	= src/cpp/parse/flex/scanscilab.ll
FLEX_SCANSCILAB_CXX	= src/cpp/parse/scanscilab.cpp

BISON_FLAGS = --no-lines -dvy -ra -Werror=conflicts-sr -Werror=conflicts-rr
BISON_PARSESCILAB_YY	= src/cpp/parse/bison/parsescilab.yy
BISON_PARSESCILAB_OUTPUT= src/cpp/parse/bison/parsescilab.output
BISON_PARSESCILAB_HXX	= includes/parse/parsescilab.hxx
BISON_PARSESCILAB_CXX = src/cpp/parse/parsescilab.cpp

$(FLEX_SCANSCILAB_CXX): $(FLEX_SCANSCILAB_LL)
	$(LEX) $(AM_FLAGS) $(LFLAGS) $(FLEX_SCANSCILAB_LL)

$(BISON_PARSESCILAB_CXX): $(BISON_PARSESCILAB_YY)
	$(YACC) $(AM_FLAGS) $(BISON_FLAGS) $(YFLAGS) $(BISON_PARSESCILAB_YY) \
	&& mv y.tab.c $(BISON_PARSESCILAB_CXX) \
	&& mv y.tab.h $(BISON_PARSESCILAB_HXX) \
	&& mv y.output $(BISON_PARSESCILAB_OUTPUT)
endif


include $(top_srcdir)/Makefile.incl.am
