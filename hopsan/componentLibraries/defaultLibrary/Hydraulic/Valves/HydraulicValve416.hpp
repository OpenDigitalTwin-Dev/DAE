/*-----------------------------------------------------------------------------

 Copyright 2017 Hopsan Group

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.


 The full license is available in the file LICENSE.
 For details about the 'Hopsan Group' or information about Authors and
 Contributors see the HOPSANGROUP and AUTHORS files that are located in
 the Hopsan source code root directory.

-----------------------------------------------------------------------------*/

#ifndef HYDRAULICVALVE416_HPP_INCLUDED
#define HYDRAULICVALVE416_HPP_INCLUDED

#include <iostream>
#include "ComponentEssentials.h"
#include "ComponentUtilities.h"
#include "math.h"

//!
//! @file HydraulicValve416.hpp
//! @author Petter Krus <petter.krus@liu.se>
//! @date Wed 21 May 2014 14:22:47
//! @brief A hydraulic valve with separate orifices
//! @ingroup HydraulicComponents
//!
//==This code has been autogenerated using Compgen==
//from 
/*{, C:, HopsanTrunk, ComponentLibraries, defaultLibrary, Hydraulic, \
Valves}/HydraulicValve416.nb*/

using namespace hopsan;

class HydraulicValve416 : public ComponentQ
{
private:
     double rho;
     double Cq;
     double Sd;
     double Frpa;
     double Frta;
     double Frpb;
     double Frtb;
     double Xpa0;
     double Xta0;
     double Xpb0;
     double Xtb0;
     double Xvmax;
     double plam;
     Port *mpPp;
     Port *mpPt;
     Port *mpPa;
     Port *mpPb;
     double delayParts1[9];
     double delayParts2[9];
     double delayParts3[9];
     double delayParts4[9];
     double delayParts5[9];
     double delayParts6[9];
     double delayParts7[9];
     double delayParts8[9];
     Matrix jacobianMatrix;
     Vec systemEquations;
     Matrix delayedPart;
     int i;
     int iter;
     int mNoiter;
     double jsyseqnweight[4];
     int order[8];
     int mNstep;
     //Port Pp variable
     double pp;
     double qp;
     double Tp;
     double dEp;
     double cp;
     double Zcp;
     //Port Pt variable
     double pt;
     double qt;
     double Tt;
     double dEt;
     double ct;
     double Zct;
     //Port Pa variable
     double pa;
     double qa;
     double Ta;
     double dEa;
     double ca;
     double Zca;
     //Port Pb variable
     double pb;
     double qb;
     double Tb;
     double dEb;
     double cb;
     double Zcb;
//==This code has been autogenerated using Compgen==
     //inputVariables
     double xvpa;
     double xvta;
     double xvpb;
     double xvtb;
     //outputVariables
     //LocalExpressions variables
     double Ks;
     double Kspa;
     double Ksta;
     double Kspb;
     double Kstb;
     //Port Pp pointer
     double *mpND_pp;
     double *mpND_qp;
     double *mpND_Tp;
     double *mpND_dEp;
     double *mpND_cp;
     double *mpND_Zcp;
     //Port Pt pointer
     double *mpND_pt;
     double *mpND_qt;
     double *mpND_Tt;
     double *mpND_dEt;
     double *mpND_ct;
     double *mpND_Zct;
     //Port Pa pointer
     double *mpND_pa;
     double *mpND_qa;
     double *mpND_Ta;
     double *mpND_dEa;
     double *mpND_ca;
     double *mpND_Zca;
     //Port Pb pointer
     double *mpND_pb;
     double *mpND_qb;
     double *mpND_Tb;
     double *mpND_dEb;
     double *mpND_cb;
     double *mpND_Zcb;
     //Delay declarations
//==This code has been autogenerated using Compgen==
     //inputVariables pointers
     double *mpxvpa;
     double *mpxvta;
     double *mpxvpb;
     double *mpxvtb;
     //inputParameters pointers
     double *mprho;
     double *mpCq;
     double *mpSd;
     double *mpFrpa;
     double *mpFrta;
     double *mpFrpb;
     double *mpFrtb;
     double *mpXpa0;
     double *mpXta0;
     double *mpXpb0;
     double *mpXtb0;
     double *mpXvmax;
     double *mpplam;
     //outputVariables pointers
     Delay mDelayedPart10;
     Delay mDelayedPart20;
     Delay mDelayedPart30;
     Delay mDelayedPart40;
     EquationSystemSolver *mpSolver = nullptr;

public:
     static Component *Creator()
     {
        return new HydraulicValve416();
     }

     void configure()
     {
//==This code has been autogenerated using Compgen==

        mNstep=9;
        jacobianMatrix.create(8,8);
        systemEquations.create(8);
        delayedPart.create(9,6);
        mNoiter=2;
        jsyseqnweight[0]=1;
        jsyseqnweight[1]=0.67;
        jsyseqnweight[2]=0.5;
        jsyseqnweight[3]=0.5;


        //Add ports to the component
        mpPp=addPowerPort("Pp","NodeHydraulic");
        mpPt=addPowerPort("Pt","NodeHydraulic");
        mpPa=addPowerPort("Pa","NodeHydraulic");
        mpPb=addPowerPort("Pb","NodeHydraulic");
        //Add inputVariables to the component
            addInputVariable("xvpa","Spool position","m",0.,&mpxvpa);
            addInputVariable("xvta","Spool position","m",0.,&mpxvta);
            addInputVariable("xvpb","Spool position","m",0.,&mpxvpb);
            addInputVariable("xvtb","Spool position","m",0.,&mpxvtb);

        //Add inputParammeters to the component
            addInputVariable("rho", "Oil density", "kg/m3", 870, &mprho);
            addInputVariable("Cq", "Flow coefficient.", "", 0.67,&mpCq);
            addInputVariable("Sd", "spool diameter", "m", 0.001,&mpSd);
            addInputVariable("Frpa", "Spool cricle fraction(P-A)", "", \
1.,&mpFrpa);
            addInputVariable("Frta", "Spool cricle fraction(A-T)", "", \
1.,&mpFrta);
            addInputVariable("Frpb", "Spool cricle fraction(P-B)", "", \
1.,&mpFrpb);
            addInputVariable("Frtb", "Spool cricle fraction(B-T)", "", \
1.,&mpFrtb);
            addInputVariable("Xpa0", "Underlap", "m", 0.,&mpXpa0);
            addInputVariable("Xta0", "Underlap", "m", 0.,&mpXta0);
            addInputVariable("Xpb0", "Underlap", "m", 0.,&mpXpb0);
            addInputVariable("Xtb0", "Underlap", "m", 0.,&mpXtb0);
            addInputVariable("Xvmax", "Max opening", "m", 0.01,&mpXvmax);
            addInputVariable("plam", "Turbulence onset pressure", "Pa", \
10000.,&mpplam);
        //Add outputVariables to the component

//==This code has been autogenerated using Compgen==
        //Add constantParameters
        mpSolver = new EquationSystemSolver(this,8);
     }

    void initialize()
     {
        //Read port variable pointers from nodes
        //Port Pp
        mpND_pp=getSafeNodeDataPtr(mpPp, NodeHydraulic::Pressure);
        mpND_qp=getSafeNodeDataPtr(mpPp, NodeHydraulic::Flow);
        mpND_Tp=getSafeNodeDataPtr(mpPp, NodeHydraulic::Temperature);
        mpND_dEp=getSafeNodeDataPtr(mpPp, NodeHydraulic::HeatFlow);
        mpND_cp=getSafeNodeDataPtr(mpPp, NodeHydraulic::WaveVariable);
        mpND_Zcp=getSafeNodeDataPtr(mpPp, NodeHydraulic::CharImpedance);
        //Port Pt
        mpND_pt=getSafeNodeDataPtr(mpPt, NodeHydraulic::Pressure);
        mpND_qt=getSafeNodeDataPtr(mpPt, NodeHydraulic::Flow);
        mpND_Tt=getSafeNodeDataPtr(mpPt, NodeHydraulic::Temperature);
        mpND_dEt=getSafeNodeDataPtr(mpPt, NodeHydraulic::HeatFlow);
        mpND_ct=getSafeNodeDataPtr(mpPt, NodeHydraulic::WaveVariable);
        mpND_Zct=getSafeNodeDataPtr(mpPt, NodeHydraulic::CharImpedance);
        //Port Pa
        mpND_pa=getSafeNodeDataPtr(mpPa, NodeHydraulic::Pressure);
        mpND_qa=getSafeNodeDataPtr(mpPa, NodeHydraulic::Flow);
        mpND_Ta=getSafeNodeDataPtr(mpPa, NodeHydraulic::Temperature);
        mpND_dEa=getSafeNodeDataPtr(mpPa, NodeHydraulic::HeatFlow);
        mpND_ca=getSafeNodeDataPtr(mpPa, NodeHydraulic::WaveVariable);
        mpND_Zca=getSafeNodeDataPtr(mpPa, NodeHydraulic::CharImpedance);
        //Port Pb
        mpND_pb=getSafeNodeDataPtr(mpPb, NodeHydraulic::Pressure);
        mpND_qb=getSafeNodeDataPtr(mpPb, NodeHydraulic::Flow);
        mpND_Tb=getSafeNodeDataPtr(mpPb, NodeHydraulic::Temperature);
        mpND_dEb=getSafeNodeDataPtr(mpPb, NodeHydraulic::HeatFlow);
        mpND_cb=getSafeNodeDataPtr(mpPb, NodeHydraulic::WaveVariable);
        mpND_Zcb=getSafeNodeDataPtr(mpPb, NodeHydraulic::CharImpedance);

        //Read variables from nodes
        //Port Pp
        pp = (*mpND_pp);
        qp = (*mpND_qp);
        Tp = (*mpND_Tp);
        dEp = (*mpND_dEp);
        cp = (*mpND_cp);
        Zcp = (*mpND_Zcp);
        //Port Pt
        pt = (*mpND_pt);
        qt = (*mpND_qt);
        Tt = (*mpND_Tt);
        dEt = (*mpND_dEt);
        ct = (*mpND_ct);
        Zct = (*mpND_Zct);
        //Port Pa
        pa = (*mpND_pa);
        qa = (*mpND_qa);
        Ta = (*mpND_Ta);
        dEa = (*mpND_dEa);
        ca = (*mpND_ca);
        Zca = (*mpND_Zca);
        //Port Pb
        pb = (*mpND_pb);
        qb = (*mpND_qb);
        Tb = (*mpND_Tb);
        dEb = (*mpND_dEb);
        cb = (*mpND_cb);
        Zcb = (*mpND_Zcb);

        //Read inputVariables from nodes
        xvpa = (*mpxvpa);
        xvta = (*mpxvta);
        xvpb = (*mpxvpb);
        xvtb = (*mpxvtb);

        //Read inputParameters from nodes
        rho = (*mprho);
        Cq = (*mpCq);
        Sd = (*mpSd);
        Frpa = (*mpFrpa);
        Frta = (*mpFrta);
        Frpb = (*mpFrpb);
        Frtb = (*mpFrtb);
        Xpa0 = (*mpXpa0);
        Xta0 = (*mpXta0);
        Xpb0 = (*mpXpb0);
        Xtb0 = (*mpXtb0);
        Xvmax = (*mpXvmax);
        plam = (*mpplam);

        //Read outputVariables from nodes

//==This code has been autogenerated using Compgen==

        //LocalExpressions
        Ks = (1.4142135623730951*Cq)/Sqrt(rho);
        Kspa = 3.14159*Frpa*Ks*Sd*limit(Xpa0 + xvpa,0.,Xpa0 + Xvmax);
        Ksta = 3.14159*Frta*Ks*Sd*limit(Xta0 + xvta,0.,Xta0 + Xvmax);
        Kspb = 3.14159*Frpb*Ks*Sd*limit(Xpb0 + xvpb,0.,Xpb0 + Xvmax);
        Kstb = 3.14159*Frtb*Ks*Sd*limit(Xtb0 + xvtb,0.,Xtb0 + Xvmax);

        //Initialize delays

        delayedPart[1][1] = delayParts1[1];
        delayedPart[2][1] = delayParts2[1];
        delayedPart[3][1] = delayParts3[1];
        delayedPart[4][1] = delayParts4[1];
        delayedPart[5][1] = delayParts5[1];
        delayedPart[6][1] = delayParts6[1];
        delayedPart[7][1] = delayParts7[1];
        delayedPart[8][1] = delayParts8[1];
     }
    void simulateOneTimestep()
     {
        Vec stateVar(8);
        Vec stateVark(8);
        Vec deltaStateVar(8);

        //Read variables from nodes
        //Port Pp
        Tp = (*mpND_Tp);
        cp = (*mpND_cp);
        Zcp = (*mpND_Zcp);
        //Port Pt
        Tt = (*mpND_Tt);
        ct = (*mpND_ct);
        Zct = (*mpND_Zct);
        //Port Pa
        Ta = (*mpND_Ta);
        ca = (*mpND_ca);
        Zca = (*mpND_Zca);
        //Port Pb
        Tb = (*mpND_Tb);
        cb = (*mpND_cb);
        Zcb = (*mpND_Zcb);

        //Read inputVariables from nodes
        xvpa = (*mpxvpa);
        xvta = (*mpxvta);
        xvpb = (*mpxvpb);
        xvtb = (*mpxvtb);

        //LocalExpressions
        Ks = (1.4142135623730951*Cq)/Sqrt(rho);
        Kspa = 3.14159*Frpa*Ks*Sd*limit(Xpa0 + xvpa,0.,Xpa0 + Xvmax);
        Ksta = 3.14159*Frta*Ks*Sd*limit(Xta0 + xvta,0.,Xta0 + Xvmax);
        Kspb = 3.14159*Frpb*Ks*Sd*limit(Xpb0 + xvpb,0.,Xpb0 + Xvmax);
        Kstb = 3.14159*Frtb*Ks*Sd*limit(Xtb0 + xvtb,0.,Xtb0 + Xvmax);

        //Initializing variable vector for Newton-Raphson
        stateVark[0] = qp;
        stateVark[1] = qt;
        stateVark[2] = qa;
        stateVark[3] = qb;
        stateVark[4] = pp;
        stateVark[5] = pt;
        stateVark[6] = pa;
        stateVark[7] = pb;

        //Iterative solution using Newton-Rapshson
        for(iter=1;iter<=mNoiter;iter++)
        {
         //Valve416
         //Differential-algebraic system of equation parts

          //Assemble differential-algebraic equations
          systemEquations[0] =qp + Kspa*signedSquareL(-pa + pp,plam) + \
Kspb*signedSquareL(-pb + pp,plam);
          systemEquations[1] =qt + Ksta*signedSquareL(-pa + pt,plam) + \
Kstb*signedSquareL(-pb + pt,plam);
          systemEquations[2] =qa - Kspa*signedSquareL(-pa + pp,plam) - \
Ksta*signedSquareL(-pa + pt,plam);
          systemEquations[3] =qb - Kspb*signedSquareL(-pb + pp,plam) - \
Kstb*signedSquareL(-pb + pt,plam);
          systemEquations[4] =pp - lowLimit(cp + qp*Zcp*onPositive(pp),0);
          systemEquations[5] =pt - lowLimit(ct + qt*Zct*onPositive(pt),0);
          systemEquations[6] =pa - lowLimit(ca + qa*Zca*onPositive(pa),0);
          systemEquations[7] =pb - lowLimit(cb + qb*Zcb*onPositive(pb),0);

          //Jacobian matrix
          jacobianMatrix[0][0] = 1;
          jacobianMatrix[0][1] = 0;
          jacobianMatrix[0][2] = 0;
          jacobianMatrix[0][3] = 0;
          jacobianMatrix[0][4] = Kspa*dxSignedSquareL(-pa + pp,plam) + \
Kspb*dxSignedSquareL(-pb + pp,plam);
          jacobianMatrix[0][5] = 0;
          jacobianMatrix[0][6] = -(Kspa*dxSignedSquareL(-pa + pp,plam));
          jacobianMatrix[0][7] = -(Kspb*dxSignedSquareL(-pb + pp,plam));
          jacobianMatrix[1][0] = 0;
          jacobianMatrix[1][1] = 1;
          jacobianMatrix[1][2] = 0;
          jacobianMatrix[1][3] = 0;
          jacobianMatrix[1][4] = 0;
          jacobianMatrix[1][5] = Ksta*dxSignedSquareL(-pa + pt,plam) + \
Kstb*dxSignedSquareL(-pb + pt,plam);
          jacobianMatrix[1][6] = -(Ksta*dxSignedSquareL(-pa + pt,plam));
          jacobianMatrix[1][7] = -(Kstb*dxSignedSquareL(-pb + pt,plam));
          jacobianMatrix[2][0] = 0;
          jacobianMatrix[2][1] = 0;
          jacobianMatrix[2][2] = 1;
          jacobianMatrix[2][3] = 0;
          jacobianMatrix[2][4] = -(Kspa*dxSignedSquareL(-pa + pp,plam));
          jacobianMatrix[2][5] = -(Ksta*dxSignedSquareL(-pa + pt,plam));
          jacobianMatrix[2][6] = Kspa*dxSignedSquareL(-pa + pp,plam) + \
Ksta*dxSignedSquareL(-pa + pt,plam);
          jacobianMatrix[2][7] = 0;
          jacobianMatrix[3][0] = 0;
          jacobianMatrix[3][1] = 0;
          jacobianMatrix[3][2] = 0;
          jacobianMatrix[3][3] = 1;
          jacobianMatrix[3][4] = -(Kspb*dxSignedSquareL(-pb + pp,plam));
          jacobianMatrix[3][5] = -(Kstb*dxSignedSquareL(-pb + pt,plam));
          jacobianMatrix[3][6] = 0;
          jacobianMatrix[3][7] = Kspb*dxSignedSquareL(-pb + pp,plam) + \
Kstb*dxSignedSquareL(-pb + pt,plam);
          jacobianMatrix[4][0] = -(Zcp*dxLowLimit(cp + \
qp*Zcp*onPositive(pp),0)*onPositive(pp));
          jacobianMatrix[4][1] = 0;
          jacobianMatrix[4][2] = 0;
          jacobianMatrix[4][3] = 0;
          jacobianMatrix[4][4] = 1;
          jacobianMatrix[4][5] = 0;
          jacobianMatrix[4][6] = 0;
          jacobianMatrix[4][7] = 0;
          jacobianMatrix[5][0] = 0;
          jacobianMatrix[5][1] = -(Zct*dxLowLimit(ct + \
qt*Zct*onPositive(pt),0)*onPositive(pt));
          jacobianMatrix[5][2] = 0;
          jacobianMatrix[5][3] = 0;
          jacobianMatrix[5][4] = 0;
          jacobianMatrix[5][5] = 1;
          jacobianMatrix[5][6] = 0;
          jacobianMatrix[5][7] = 0;
          jacobianMatrix[6][0] = 0;
          jacobianMatrix[6][1] = 0;
          jacobianMatrix[6][2] = -(Zca*dxLowLimit(ca + \
qa*Zca*onPositive(pa),0)*onPositive(pa));
          jacobianMatrix[6][3] = 0;
          jacobianMatrix[6][4] = 0;
          jacobianMatrix[6][5] = 0;
          jacobianMatrix[6][6] = 1;
          jacobianMatrix[6][7] = 0;
          jacobianMatrix[7][0] = 0;
          jacobianMatrix[7][1] = 0;
          jacobianMatrix[7][2] = 0;
          jacobianMatrix[7][3] = -(Zcb*dxLowLimit(cb + \
qb*Zcb*onPositive(pb),0)*onPositive(pb));
          jacobianMatrix[7][4] = 0;
          jacobianMatrix[7][5] = 0;
          jacobianMatrix[7][6] = 0;
          jacobianMatrix[7][7] = 1;
//==This code has been autogenerated using Compgen==

          //Solving equation using LU-faktorisation
          mpSolver->solve(jacobianMatrix, systemEquations, stateVark, iter);
          qp=stateVark[0];
          qt=stateVark[1];
          qa=stateVark[2];
          qb=stateVark[3];
          pp=stateVark[4];
          pt=stateVark[5];
          pa=stateVark[6];
          pb=stateVark[7];
        }

        //Calculate the delayed parts

        delayedPart[1][1] = delayParts1[1];
        delayedPart[2][1] = delayParts2[1];
        delayedPart[3][1] = delayParts3[1];
        delayedPart[4][1] = delayParts4[1];
        delayedPart[5][1] = delayParts5[1];
        delayedPart[6][1] = delayParts6[1];
        delayedPart[7][1] = delayParts7[1];
        delayedPart[8][1] = delayParts8[1];

        //Write new values to nodes
        //Port Pp
        (*mpND_pp)=pp;
        (*mpND_qp)=qp;
        (*mpND_dEp)=dEp;
        //Port Pt
        (*mpND_pt)=pt;
        (*mpND_qt)=qt;
        (*mpND_dEt)=dEt;
        //Port Pa
        (*mpND_pa)=pa;
        (*mpND_qa)=qa;
        (*mpND_dEa)=dEa;
        //Port Pb
        (*mpND_pb)=pb;
        (*mpND_qb)=qb;
        (*mpND_dEb)=dEb;
        //outputVariables

        //Update the delayed variabels

     }
    void deconfigure()
    {
        delete mpSolver;
    }
};
#endif // HYDRAULICVALVE416_HPP_INCLUDED