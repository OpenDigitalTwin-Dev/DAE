/*
 * MBDyn (C) is a multibody analysis code.
 * http://www.mbdyn.org
 *
 * Copyright (C) 2011-2023
 *
 * Marco Morandini	<morandini@aero.polimi.it>
 * Riccardo Vescovini	<vescovini@aero.polimi.it>
 * Tommaso Solcia	<solcia@aero.polimi.it>
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

/*
 * Inspired by
 * Wojciech Witkowski
 * "4-Node combined shell element with semi-EAS-ANS strain interpolations
 * in 6-parameter shell theories with drilling degrees of freedom"
 * Comput Mech (2009) 43:307­319 DOI 10.1007/s00466-008-0307-x
 */

#ifndef MEMBRANE_H
#define MEMBRANE_H

#include "myassert.h"
#include "except.h"

#include "strnode.h"
#include "elem.h"

#include "constltp.h"

/* da spostare in .cc */
#include "Rot.hh"
#include "joint.h"

extern const char* psMembraneNames[];

// Forward declaration
class DataManager;
class MBDynParser;

// Membrane - begin


class Membrane
: public InitialAssemblyElem,
public GravityOwner
{
public:
	// Membrane types
	enum Type {
		UNKNOWN = -1,
		ELASTIC = 0,
		VISCOELASTIC,

		LASTMEMBRANETYPE
	};

	// Da appunti Vescovini
	typedef MyVectorHandler vh;
	typedef std::vector<vh> vvh;
	typedef FullMatrixHandler fmh;
	typedef std::vector<fmh> vfmh;
	typedef ConstitutiveLaw<vh, fmh> ConstitutiveLawType;

	Membrane(unsigned uLabel, const DofOwner* pDO, flag fOut);
	virtual ~Membrane(void);

	// Element type
	virtual Elem::Type GetElemType(void) const {
		return Elem::PLATE;
	};
};

// Membrane - end

extern Elem*
ReadMembrane4EAS(DataManager* pDM,
	MBDynParser& HP,
	const DofOwner* pDO,
	unsigned int uLabel);

extern int
ReadMembraneConstLaw(MBDynParser& HP, Membrane::fmh& pD, Membrane::vh& PreStress);

#endif // MEMBRANE_H


