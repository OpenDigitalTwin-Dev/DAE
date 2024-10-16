/* $Header$ */
/*
 * MBDyn (C) is a multibody analysis code.
 * http://www.mbdyn.org
 *
 * Copyright (C) 1996-2023
 *
 * Pierangelo Masarati	<pierangelo.masarati@polimi.it>
 * Paolo Mantegazza	<paolo.mantegazza@polimi.it>
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

/* Elemento aerodinamico modale */

/* Aerodynamic Modal Element by Giuseppe Quaranta (C) - March 2002
 * <quaranta@aero.polimi.it>
 *
 * This element is connected to a modal joint element.
 * It needs a state-space representation of the instationary
 * generalized aerodynamic forces of the following type:
 *
 *  xa' = A*xa + B*q
 *  Fa  = (1/2*rho*V^2) * ((c/2*V)^2*D2*q'' + (c/2*V)*D1*q'+ D0*q + C*xa)
 * where
 *  xa are the aerodynamic states
 *  q  are the modal coordinates (structural)
 *
 * The matrices A, B, C, D0, D1, D2 can be computed starting from the
 * knowledge of the generalized aerodynamic forces frequency response
 * H(jw,M).
 * The current model can only deal with data relative to a specific Mach number.
 * Future development will include:
 *   - aerodinamic forces generated by wind gust (FIXME: DONE?);
 *   - the capability to handle data at various Mach numbers;
 *   - the effect unsteady aerodynamic forces related to rigid-body motion (FIXME: DONE?).
 */

#ifndef AerodynamicModal_hh
#define AerodynamicModal_hh

#include "myassert.h"
#include "mynewmem.h"
#include "elem.h"
#include "aerodyn.h"
#include "modal.h"
#include "spmapmh.h"

/* AerodynamicModal - begin */

class AerodynamicModal :
	public AerodynamicElem,
	public InitialAssemblyElem
{
protected:
	const StructNode* pModalNode; /* Nodo modale per il moto rigido */
	const Modal* pModalJoint;     /* puntatore all'elemento modale di riferimento */
	Vec3 P0;                      /* Posizione iniziale nodo modale */
	Mat3x3 R0;                    /* Rotazione iniziale nel sistema aerodinamico del nodo modale */
	const Mat3x3 Ra;              /* Rotaz. del sistema aerodinamico al nodo */

	doublereal Chord;             /* Reference Cord */
	unsigned int NStModes;        /* Numero di modi strutturali */
	unsigned int NAeroStates;     /* Numero stati aerodinamici */
	unsigned int NGust;           /* Numero ingressi raffica */

	SpMapMatrixHandler* pA;	      /* Vettore degli autovalori del sistema aerodinamco */
	FullMatrixHandler* pB;        /* Matrici del modello agli stati dell'aerodinamica */
	FullMatrixHandler* pC;
	FullMatrixHandler* pD0;
	FullMatrixHandler* pD1;
	FullMatrixHandler* pD2;

	MyVectorHandler* pq;          /* coordinate modali */
	MyVectorHandler* pqPrime;     /* velocita' modali */
	MyVectorHandler* pqSec;	      /* accelerazioni modali */

	MyVectorHandler* pxa;         /* coordinate modali aerodinamiche*/
	MyVectorHandler* pxaPrime;    /* velocita' modali aerodinamiche*/

	MyVectorHandler* pgs;         /* coordinate modali raffica*/
	MyVectorHandler* pgsPrime;    /* derivate prime modali raffica*/

	const doublereal gustVff;     /* frequenza di taglio filtro passa basso raffica */
	const doublereal gustXi;      /* smorzamento filtro del secondo ordine raffica */

public:
	enum RigidF_t {
		NO_RIGID = 0,
		RIGID = 6
	};

protected:
	RigidF_t RigidF;              /* Numero di gdl del corpo rigido */

	/* Assemblaggio residuo */
	void AssVec(SubVectorHandler& WorkVec);

public:
	AerodynamicModal(unsigned int uLabel,
		const StructNode* pN,
		const Modal* pMJ,
		const Mat3x3& RaTmp,
		const DofOwner* pDO,
		doublereal Cd,
		const int NModal,
		const int NAero,
		RigidF_t rgF,
		const int Gust,
		const doublereal Vff,
		SpMapMatrixHandler* pAMat,
		FullMatrixHandler* pBMat,
		FullMatrixHandler* pCMat,
		FullMatrixHandler* pD0Mat,
		FullMatrixHandler* pD1Mat,
		FullMatrixHandler* pD2Mat,
		flag fout);

	~AerodynamicModal(void);

	/* Scrive il contributo dell'elemento al file di restart */
	std::ostream& Restart(std::ostream& out) const;

	/* ritorna il numero di Dofs per gli elementi che sono anche DofOwners */
	unsigned int iGetNumDof(void) const {
		return NAeroStates + NGust*2;
	};

	/* esegue operazioni sui dof di proprieta' dell'elemento */
	DofOrder::Order GetDofType(unsigned int i) const {
		/* gradi di liberta' differenziali (eq. modali) */
		ASSERT(i < iGetNumDof());
		return DofOrder::DIFFERENTIAL;
	};

	/* Tipo dell'elemento (usato per debug ecc.) */
	Elem::Type GetElemType(void) const {
		return Elem::AEROMODAL;
	};

	/* funzioni proprie */

	/* Dimensioni del workspace */
	void WorkSpaceDim(integer* piNumRows, integer* piNumCols) const {
		*piNumRows = NAeroStates + NStModes + NGust*2;
		*piNumCols = NAeroStates + 2*NStModes + NGust*2;
	};

	/* assemblaggio jacobiano */
	VariableSubMatrixHandler&
	AssJac(VariableSubMatrixHandler& WorkMat,
		doublereal dCoef ,
		const VectorHandler& /* XCurr */ ,
		const VectorHandler& /* XPrimeCurr */);

	/* assemblaggio residuo */
	SubVectorHandler&
	AssRes(SubVectorHandler& WorkVec,
		doublereal dCoef,
		const VectorHandler& XCurr,
		const VectorHandler& XPrimeCurr);

	/* output; si assume che ogni tipo di elemento sappia, attraverso
	 * l'OutputHandler, dove scrivere il proprio output */
	void Output(OutputHandler& OH) const;

	/* Numero di GDL iniziali */
	unsigned int iGetInitialNumDof(void) const {
		return NAeroStates + NGust*2;
	};

	/* Dimensioni del workspace */
	void
	InitialWorkSpaceDim(integer* piNumRows, integer* piNumCols) const {
		*piNumRows = NAeroStates + NStModes + NGust*2;
		*piNumCols = NAeroStates + 2*NStModes + NGust*2;
	};

	/* assemblaggio jacobiano */
	VariableSubMatrixHandler&
	InitialAssJac(VariableSubMatrixHandler& WorkMat,
		const VectorHandler& XCurr)
	{
		DEBUGCOUTFNAME("AerodynamicModal::InitialAssJac");
		AssJac(WorkMat, 0, XCurr, XCurr);
		return WorkMat;
	};

	/* assemblaggio residuo */
	SubVectorHandler&
	InitialAssRes(SubVectorHandler& WorkVec, const VectorHandler& XCurr);

	/* Tipo di elemento aerodinamico */
	AerodynamicElemBase::Type GetAerodynamicElemType(void) const {
		return AerodynamicElemBase::AEROMODAL;
	};

	/* *******PER IL SOLUTORE PARALLELO******** */
	/* Fornisce il tipo e la label dei nodi che sono connessi all'elemento
	 * utile per l'assemblaggio della matrice di connessione fra i dofs */
	virtual void
	GetConnectedNodes(std::vector<const Node *>& connectedNodes) const {
		connectedNodes.resize(1);
		connectedNodes[0] = pModalNode;
	};
	/* ************************************************ */

	/* returns the dimension of the component */
	const virtual OutputHandler::Dimensions GetEquationDimension(integer index) const;

	/* describes the dimension of components of equation */
    virtual std::ostream& DescribeEq(std::ostream& out,
		  const char *prefix = "",
		  bool bInitial = false) const;
};

/* AerodynamicModal - end */

class DataManager;
class MBDynParser;

extern Elem*
ReadAerodynamicModal(DataManager* pDM, MBDynParser& HP,
	const DofOwner* pDO, unsigned int uLabel);

#endif /* AerodynamicModal_hh */
