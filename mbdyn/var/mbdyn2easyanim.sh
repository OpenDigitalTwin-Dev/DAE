#!/bin/sh
# $Header$
# MBDyn (C) is a multibody analysis code. 
# http://www.mbdyn.org
# 
# Copyright (C) 1996-2023
# 
# Pierangelo Masarati	<pierangelo.masarati@polimi.it>
# Paolo Mantegazza	<paolo.mantegazza@polimi.it>
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

# Prepare MBDyn ASCII output (.log & .mov files) for loading within EasyAnim
#
# EasyAnim is a simple package for the visualization of mechanical systems
# it can be downloaded from http://mecara.fpms.ac.be/EasyDyn/
# For information, contact
#
# Prof. Olivier VERLINDEN
# Faculte Polytechnique de Mons
# Department of Theoretical Mechanics, Dynamics and Vibrations
# 31, Bd Dolez; 7000 MONS (BELGIUM)
# E-mail: Olivier.Verlinden@fpms.ac.be
#
# Usage:
#
# mbdyn2easyanim.sh <filename>

# Location of the awk script
export AWKPATH="$AWKPATH:/test/toolkit/DAE/mbdyn/../install/mbdyn_install/share/awk"
SOURCES="-f mbdyn2easyanim.awk"

# Check args
FILENAME=
EXTRALOGFILES=
while test "x$1" != "x" ; do
	case $1 in
	-f)
		shift
		if test "x$1" = "x" ; then
			echo "usage:  mbdyn2easyanim.sh -f <awkfile> <filename>"	
			exit 1
		fi
		SOURCES="$SOURCES -f $1"
		;;

	-l)
		shift
		if test "x$1" = "x" ; then
			echo "usage:  mbdyn2easyanim.sh -l <(extra)logfile> <filename>"	
			exit 1
		fi
		if test ! -f $1 ; then
			echo "missing extra logfile \"$1\""
			exit 1
		fi
		EXTRALOGFILES="$EXTRALOGFILES $1"
		;;

	-v)
		shift
		if test "x$1" = "x" ; then
			echo "usage:  mbdyn2easyanim.sh -v <name=value> <filename>"	
			exit 1
		fi
		SOURCES="$SOURCES -v $1"
		;;

	-*)
		echo "unknown option $1"
		exit 1
		;;
	*)
		FILENAME=$1
		;;
	esac
	shift
done

if test "x$FILENAME" = "x" ; then
	echo "usage:  mbdyn2easyanim.sh [OPTIONS] <filename>"
	exit 1
fi

# Check log file
if test "x${FILENAME}.log" = "x" ; then
	echo "missing log file \"${FILENAME}.log\""
	exit 1
fi

# TODO: if exists, handle .rfm

# Check mov file
MOVFILE=yes
if test "x${FILENAME}.mov" = "x" ; then
	echo "warning: missing mov file \"${FILENAME}.mov\""
	NOMOVFILE=1
	MOVFILE=`mktemp` 
	# FIXME: check if Euler angles are used in output?
	awk '/^structural node:/ {print $3, $4,$5,$6, $8,$9,$10}' \
		"${FILENAME}.log" > "${MOVFILE}"
else
	NOMOVFILE=0
	MOVFILE="${FILENAME}.mov"
fi

# Make sure files do not exist
rm -f "${FILENAME}.vol" "${FILENAME}.van"

# Generate files
echo "####" | \
	cat "${FILENAME}.log" $EXTRALOGFILES - "${MOVFILE}" | \
	awk ${SOURCES} -vfile="${FILENAME}"
RC=$?
if test $NOMOVFILE = 1 ; then
	rm -f "${MOVFILE}"
fi

if test $RC != 0 ; then
	echo "conversion failed ($RC)"
	exit $RC
fi

# Check for file existence
if test ! -f "${FILENAME}.vol" -o ! -f "${FILENAME}.van" ; then
	echo "conversion failed (no files)"
	exit 1
fi

exit 0
