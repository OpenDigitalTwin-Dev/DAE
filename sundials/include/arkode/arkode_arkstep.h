/* -----------------------------------------------------------------
 * Programmer(s): Daniel R. Reynolds @ SMU
 * -----------------------------------------------------------------
 * SUNDIALS Copyright Start
 * Copyright (c) 2002-2024, Lawrence Livermore National Security
 * and Southern Methodist University.
 * All rights reserved.
 *
 * See the top-level LICENSE and NOTICE files for details.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * SUNDIALS Copyright End
 * -----------------------------------------------------------------
 * This is the header file for the ARKode ARKStep module.
 * -----------------------------------------------------------------*/

#ifndef _ARKSTEP_H
#define _ARKSTEP_H

#include <arkode/arkode.h>
#include <arkode/arkode_butcher_dirk.h>
#include <arkode/arkode_butcher_erk.h>
#include <arkode/arkode_ls.h>
#include <sunadaptcontroller/sunadaptcontroller_imexgus.h>
#include <sunadaptcontroller/sunadaptcontroller_soderlind.h>

#ifdef __cplusplus /* wrapper to enable C++ usage */
extern "C" {
#endif

/* -----------------
 * ARKStep Constants
 * ----------------- */

/* Default Butcher tables for each method/order */

/*    explicit */
static const int ARKSTEP_DEFAULT_ERK_2 = ARKODE_HEUN_EULER_2_1_2;
static const int ARKSTEP_DEFAULT_ERK_3 = ARKODE_BOGACKI_SHAMPINE_4_2_3;
static const int ARKSTEP_DEFAULT_ERK_4 = ARKODE_ZONNEVELD_5_3_4;
static const int ARKSTEP_DEFAULT_ERK_5 = ARKODE_CASH_KARP_6_4_5;
static const int ARKSTEP_DEFAULT_ERK_6 = ARKODE_VERNER_8_5_6;
static const int ARKSTEP_DEFAULT_ERK_7 = ARKODE_VERNER_10_6_7;
static const int ARKSTEP_DEFAULT_ERK_8 = ARKODE_FEHLBERG_13_7_8;
static const int ARKSTEP_DEFAULT_ERK_9 = ARKODE_VERNER_16_8_9;

/*    implicit */
static const int ARKSTEP_DEFAULT_DIRK_2 = ARKODE_SDIRK_2_1_2;
static const int ARKSTEP_DEFAULT_DIRK_3 = ARKODE_ARK324L2SA_DIRK_4_2_3;
static const int ARKSTEP_DEFAULT_DIRK_4 = ARKODE_SDIRK_5_3_4;
static const int ARKSTEP_DEFAULT_DIRK_5 = ARKODE_ARK548L2SA_DIRK_8_4_5;

/*    ImEx */
static const int ARKSTEP_DEFAULT_ARK_ETABLE_2 = ARKODE_ARK2_ERK_3_1_2;
static const int ARKSTEP_DEFAULT_ARK_ETABLE_3 = ARKODE_ARK324L2SA_ERK_4_2_3;
static const int ARKSTEP_DEFAULT_ARK_ETABLE_4 = ARKODE_ARK436L2SA_ERK_6_3_4;
static const int ARKSTEP_DEFAULT_ARK_ETABLE_5 = ARKODE_ARK548L2SA_ERK_8_4_5;
static const int ARKSTEP_DEFAULT_ARK_ITABLE_2 = ARKODE_ARK2_DIRK_3_1_2;
static const int ARKSTEP_DEFAULT_ARK_ITABLE_3 = ARKODE_ARK324L2SA_DIRK_4_2_3;
static const int ARKSTEP_DEFAULT_ARK_ITABLE_4 = ARKODE_ARK436L2SA_DIRK_6_3_4;
static const int ARKSTEP_DEFAULT_ARK_ITABLE_5 = ARKODE_ARK548L2SA_DIRK_8_4_5;

/* -------------------
 * Exported Functions
 * ------------------- */

/* Create, Resize, and Reinitialization functions */
SUNDIALS_EXPORT void* ARKStepCreate(ARKRhsFn fe, ARKRhsFn fi, sunrealtype t0,
                                    N_Vector y0, SUNContext sunctx);

SUNDIALS_EXPORT int ARKStepResize(void* arkode_mem, N_Vector ynew,
                                  sunrealtype hscale, sunrealtype t0,
                                  ARKVecResizeFn resize, void* resize_data);

SUNDIALS_EXPORT int ARKStepReInit(void* arkode_mem, ARKRhsFn fe, ARKRhsFn fi,
                                  sunrealtype t0, N_Vector y0);

SUNDIALS_EXPORT int ARKStepReset(void* arkode_mem, sunrealtype tR, N_Vector yR);

/* Tolerance input functions */
SUNDIALS_EXPORT int ARKStepSStolerances(void* arkode_mem, sunrealtype reltol,
                                        sunrealtype abstol);
SUNDIALS_EXPORT int ARKStepSVtolerances(void* arkode_mem, sunrealtype reltol,
                                        N_Vector abstol);
SUNDIALS_EXPORT int ARKStepWFtolerances(void* arkode_mem, ARKEwtFn efun);

/* Residual tolerance input functions */
SUNDIALS_EXPORT int ARKStepResStolerance(void* arkode_mem, sunrealtype rabstol);
SUNDIALS_EXPORT int ARKStepResVtolerance(void* arkode_mem, N_Vector rabstol);
SUNDIALS_EXPORT int ARKStepResFtolerance(void* arkode_mem, ARKRwtFn rfun);

/* Linear solver set functions */
SUNDIALS_EXPORT int ARKStepSetLinearSolver(void* arkode_mem, SUNLinearSolver LS,
                                           SUNMatrix A);
SUNDIALS_EXPORT int ARKStepSetMassLinearSolver(void* arkode_mem,
                                               SUNLinearSolver LS, SUNMatrix M,
                                               sunbooleantype time_dep);

/* Rootfinding initialization */
SUNDIALS_EXPORT int ARKStepRootInit(void* arkode_mem, int nrtfn, ARKRootFn g);

/* Optional input functions -- must be called AFTER ARKStepCreate */
SUNDIALS_EXPORT int ARKStepSetDefaults(void* arkode_mem);
SUNDIALS_EXPORT int ARKStepSetOptimalParams(void* arkode_mem);
SUNDIALS_EXPORT int ARKStepSetOrder(void* arkode_mem, int maxord);
SUNDIALS_EXPORT int ARKStepSetInterpolantType(void* arkode_mem, int itype);
SUNDIALS_EXPORT int ARKStepSetInterpolantDegree(void* arkode_mem, int degree);
SUNDIALS_EXPORT int ARKStepSetDenseOrder(void* arkode_mem, int dord);
SUNDIALS_EXPORT int ARKStepSetNonlinearSolver(void* arkode_mem,
                                              SUNNonlinearSolver NLS);
SUNDIALS_EXPORT int ARKStepSetNlsRhsFn(void* arkode_mem, ARKRhsFn nls_fi);
SUNDIALS_EXPORT int ARKStepSetLinear(void* arkode_mem, int timedepend);
SUNDIALS_EXPORT int ARKStepSetNonlinear(void* arkode_mem);
SUNDIALS_EXPORT int ARKStepSetExplicit(void* arkode_mem);
SUNDIALS_EXPORT int ARKStepSetImplicit(void* arkode_mem);
SUNDIALS_EXPORT int ARKStepSetImEx(void* arkode_mem);
SUNDIALS_EXPORT int ARKStepSetDeduceImplicitRhs(void* arkode_mem,
                                                sunbooleantype deduce);
SUNDIALS_EXPORT int ARKStepSetTables(void* arkode_mem, int q, int p,
                                     ARKodeButcherTable Bi,
                                     ARKodeButcherTable Be);
SUNDIALS_EXPORT int ARKStepSetTableNum(void* arkode_mem,
                                       ARKODE_DIRKTableID itable,
                                       ARKODE_ERKTableID etable);
SUNDIALS_EXPORT int ARKStepSetTableName(void* arkode_mem, const char* itable,
                                        const char* etable);
SUNDIALS_EXPORT int ARKStepSetAdaptController(void* arkode_mem,
                                              SUNAdaptController C);
SUNDIALS_EXPORT int ARKStepSetAdaptivityAdjustment(void* arkode_mem, int adjust);
SUNDIALS_EXPORT int ARKStepSetCFLFraction(void* arkode_mem, sunrealtype cfl_frac);
SUNDIALS_EXPORT int ARKStepSetSafetyFactor(void* arkode_mem, sunrealtype safety);
SUNDIALS_DEPRECATED_EXPORT_MSG("use SUNAdaptController instead")
int ARKStepSetErrorBias(void* arkode_mem, sunrealtype bias);
SUNDIALS_EXPORT int ARKStepSetMaxGrowth(void* arkode_mem, sunrealtype mx_growth);
SUNDIALS_EXPORT int ARKStepSetMinReduction(void* arkode_mem, sunrealtype eta_min);
SUNDIALS_EXPORT int ARKStepSetFixedStepBounds(void* arkode_mem, sunrealtype lb,
                                              sunrealtype ub);
SUNDIALS_DEPRECATED_EXPORT_MSG("use SUNAdaptController instead")
int ARKStepSetAdaptivityMethod(void* arkode_mem, int imethod, int idefault,
                               int pq, sunrealtype adapt_params[3]);
SUNDIALS_DEPRECATED_EXPORT_MSG("use SUNAdaptController instead")
int ARKStepSetAdaptivityFn(void* arkode_mem, ARKAdaptFn hfun, void* h_data);
SUNDIALS_EXPORT int ARKStepSetMaxFirstGrowth(void* arkode_mem,
                                             sunrealtype etamx1);
SUNDIALS_EXPORT int ARKStepSetMaxEFailGrowth(void* arkode_mem,
                                             sunrealtype etamxf);
SUNDIALS_EXPORT int ARKStepSetSmallNumEFails(void* arkode_mem, int small_nef);
SUNDIALS_EXPORT int ARKStepSetMaxCFailGrowth(void* arkode_mem, sunrealtype etacf);
SUNDIALS_EXPORT int ARKStepSetNonlinCRDown(void* arkode_mem, sunrealtype crdown);
SUNDIALS_EXPORT int ARKStepSetNonlinRDiv(void* arkode_mem, sunrealtype rdiv);
SUNDIALS_EXPORT int ARKStepSetDeltaGammaMax(void* arkode_mem, sunrealtype dgmax);
SUNDIALS_EXPORT int ARKStepSetLSetupFrequency(void* arkode_mem, int msbp);
SUNDIALS_EXPORT int ARKStepSetPredictorMethod(void* arkode_mem, int method);
SUNDIALS_EXPORT int ARKStepSetStabilityFn(void* arkode_mem, ARKExpStabFn EStab,
                                          void* estab_data);
SUNDIALS_EXPORT int ARKStepSetMaxErrTestFails(void* arkode_mem, int maxnef);
SUNDIALS_EXPORT int ARKStepSetMaxNonlinIters(void* arkode_mem, int maxcor);
SUNDIALS_EXPORT int ARKStepSetMaxConvFails(void* arkode_mem, int maxncf);
SUNDIALS_EXPORT int ARKStepSetNonlinConvCoef(void* arkode_mem,
                                             sunrealtype nlscoef);
SUNDIALS_EXPORT int ARKStepSetConstraints(void* arkode_mem, N_Vector constraints);
SUNDIALS_EXPORT int ARKStepSetMaxNumSteps(void* arkode_mem, long int mxsteps);
SUNDIALS_EXPORT int ARKStepSetMaxHnilWarns(void* arkode_mem, int mxhnil);
SUNDIALS_EXPORT int ARKStepSetInitStep(void* arkode_mem, sunrealtype hin);
SUNDIALS_EXPORT int ARKStepSetMinStep(void* arkode_mem, sunrealtype hmin);
SUNDIALS_EXPORT int ARKStepSetMaxStep(void* arkode_mem, sunrealtype hmax);
SUNDIALS_EXPORT int ARKStepSetInterpolateStopTime(void* arkode_mem,
                                                  sunbooleantype interp);
SUNDIALS_EXPORT int ARKStepSetStopTime(void* arkode_mem, sunrealtype tstop);
SUNDIALS_EXPORT int ARKStepClearStopTime(void* arkode_mem);
SUNDIALS_EXPORT int ARKStepSetFixedStep(void* arkode_mem, sunrealtype hfixed);
SUNDIALS_EXPORT int ARKStepSetMaxNumConstrFails(void* arkode_mem, int maxfails);

SUNDIALS_EXPORT int ARKStepSetRootDirection(void* arkode_mem, int* rootdir);
SUNDIALS_EXPORT int ARKStepSetNoInactiveRootWarn(void* arkode_mem);

SUNDIALS_EXPORT int ARKStepSetUserData(void* arkode_mem, void* user_data);

SUNDIALS_EXPORT int ARKStepSetPostprocessStepFn(void* arkode_mem,
                                                ARKPostProcessFn ProcessStep);
SUNDIALS_EXPORT int ARKStepSetPostprocessStageFn(void* arkode_mem,
                                                 ARKPostProcessFn ProcessStage);
SUNDIALS_EXPORT int ARKStepSetStagePredictFn(void* arkode_mem,
                                             ARKStagePredictFn PredictStage);

/* Linear solver interface optional input functions -- must be called
   AFTER ARKStepSetLinearSolver and/or ARKStepSetMassLinearSolver */
SUNDIALS_EXPORT int ARKStepSetJacFn(void* arkode_mem, ARKLsJacFn jac);
SUNDIALS_EXPORT int ARKStepSetMassFn(void* arkode_mem, ARKLsMassFn mass);
SUNDIALS_EXPORT int ARKStepSetJacEvalFrequency(void* arkode_mem, long int msbj);
SUNDIALS_EXPORT int ARKStepSetLinearSolutionScaling(void* arkode_mem,
                                                    sunbooleantype onoff);
SUNDIALS_EXPORT int ARKStepSetEpsLin(void* arkode_mem, sunrealtype eplifac);
SUNDIALS_EXPORT int ARKStepSetMassEpsLin(void* arkode_mem, sunrealtype eplifac);
SUNDIALS_EXPORT int ARKStepSetLSNormFactor(void* arkode_mem, sunrealtype nrmfac);
SUNDIALS_EXPORT int ARKStepSetMassLSNormFactor(void* arkode_mem,
                                               sunrealtype nrmfac);
SUNDIALS_EXPORT int ARKStepSetPreconditioner(void* arkode_mem,
                                             ARKLsPrecSetupFn psetup,
                                             ARKLsPrecSolveFn psolve);
SUNDIALS_EXPORT int ARKStepSetMassPreconditioner(void* arkode_mem,
                                                 ARKLsMassPrecSetupFn psetup,
                                                 ARKLsMassPrecSolveFn psolve);
SUNDIALS_EXPORT int ARKStepSetJacTimes(void* arkode_mem,
                                       ARKLsJacTimesSetupFn jtsetup,
                                       ARKLsJacTimesVecFn jtimes);
SUNDIALS_EXPORT int ARKStepSetJacTimesRhsFn(void* arkode_mem,
                                            ARKRhsFn jtimesRhsFn);
SUNDIALS_EXPORT int ARKStepSetMassTimes(void* arkode_mem,
                                        ARKLsMassTimesSetupFn msetup,
                                        ARKLsMassTimesVecFn mtimes,
                                        void* mtimes_data);
SUNDIALS_EXPORT int ARKStepSetLinSysFn(void* arkode_mem, ARKLsLinSysFn linsys);

/* Integrate the ODE over an interval in t */
SUNDIALS_EXPORT int ARKStepEvolve(void* arkode_mem, sunrealtype tout,
                                  N_Vector yout, sunrealtype* tret, int itask);

/* Computes the kth derivative of the y function at time t */
SUNDIALS_EXPORT int ARKStepGetDky(void* arkode_mem, sunrealtype t, int k,
                                  N_Vector dky);

/* Utility function to update/compute y based on zcor */
SUNDIALS_EXPORT int ARKStepComputeState(void* arkode_mem, N_Vector zcor,
                                        N_Vector z);

/* Optional output functions */
SUNDIALS_EXPORT int ARKStepGetNumExpSteps(void* arkode_mem, long int* expsteps);
SUNDIALS_EXPORT int ARKStepGetNumAccSteps(void* arkode_mem, long int* accsteps);
SUNDIALS_EXPORT int ARKStepGetNumStepAttempts(void* arkode_mem,
                                              long int* step_attempts);
SUNDIALS_EXPORT int ARKStepGetNumRhsEvals(void* arkode_mem, long int* nfe_evals,
                                          long int* nfi_evals);
SUNDIALS_EXPORT int ARKStepGetNumLinSolvSetups(void* arkode_mem,
                                               long int* nlinsetups);
SUNDIALS_EXPORT int ARKStepGetNumErrTestFails(void* arkode_mem,
                                              long int* netfails);
SUNDIALS_EXPORT int ARKStepGetCurrentButcherTables(void* arkode_mem,
                                                   ARKodeButcherTable* Bi,
                                                   ARKodeButcherTable* Be);
SUNDIALS_EXPORT int ARKStepGetEstLocalErrors(void* arkode_mem, N_Vector ele);
SUNDIALS_EXPORT int ARKStepGetWorkSpace(void* arkode_mem, long int* lenrw,
                                        long int* leniw);
SUNDIALS_EXPORT int ARKStepGetNumSteps(void* arkode_mem, long int* nsteps);
SUNDIALS_EXPORT int ARKStepGetActualInitStep(void* arkode_mem,
                                             sunrealtype* hinused);
SUNDIALS_EXPORT int ARKStepGetLastStep(void* arkode_mem, sunrealtype* hlast);
SUNDIALS_EXPORT int ARKStepGetCurrentStep(void* arkode_mem, sunrealtype* hcur);
SUNDIALS_EXPORT int ARKStepGetCurrentTime(void* arkode_mem, sunrealtype* tcur);
SUNDIALS_EXPORT int ARKStepGetCurrentState(void* arkode_mem, N_Vector* state);
SUNDIALS_EXPORT int ARKStepGetCurrentGamma(void* arkode_mem, sunrealtype* gamma);
SUNDIALS_EXPORT int ARKStepGetCurrentMassMatrix(void* arkode_mem, SUNMatrix* M);
SUNDIALS_EXPORT int ARKStepGetTolScaleFactor(void* arkode_mem,
                                             sunrealtype* tolsfac);
SUNDIALS_EXPORT int ARKStepGetErrWeights(void* arkode_mem, N_Vector eweight);
SUNDIALS_EXPORT int ARKStepGetResWeights(void* arkode_mem, N_Vector rweight);
SUNDIALS_EXPORT int ARKStepGetNumGEvals(void* arkode_mem, long int* ngevals);
SUNDIALS_EXPORT int ARKStepGetRootInfo(void* arkode_mem, int* rootsfound);
SUNDIALS_EXPORT int ARKStepGetNumConstrFails(void* arkode_mem,
                                             long int* nconstrfails);
SUNDIALS_EXPORT int ARKStepGetUserData(void* arkode_mem, void** user_data);
SUNDIALS_EXPORT int ARKStepPrintAllStats(void* arkode_mem, FILE* outfile,
                                         SUNOutputFormat fmt);
SUNDIALS_EXPORT char* ARKStepGetReturnFlagName(long int flag);

SUNDIALS_EXPORT int ARKStepWriteParameters(void* arkode_mem, FILE* fp);

SUNDIALS_EXPORT int ARKStepWriteButcher(void* arkode_mem, FILE* fp);

/* Grouped optional output functions */
SUNDIALS_EXPORT int ARKStepGetTimestepperStats(
  void* arkode_mem, long int* expsteps, long int* accsteps,
  long int* step_attempts, long int* nfe_evals, long int* nfi_evals,
  long int* nlinsetups, long int* netfails);
SUNDIALS_EXPORT int ARKStepGetStepStats(void* arkode_mem, long int* nsteps,
                                        sunrealtype* hinused, sunrealtype* hlast,
                                        sunrealtype* hcur, sunrealtype* tcur);

/* Nonlinear solver optional output functions */
SUNDIALS_EXPORT int ARKStepGetNonlinearSystemData(
  void* arkode_mem, sunrealtype* tcur, N_Vector* zpred, N_Vector* z,
  N_Vector* Fi, sunrealtype* gamma, N_Vector* sdata, void** user_data);

SUNDIALS_EXPORT int ARKStepGetNumNonlinSolvIters(void* arkode_mem,
                                                 long int* nniters);
SUNDIALS_EXPORT int ARKStepGetNumNonlinSolvConvFails(void* arkode_mem,
                                                     long int* nnfails);
SUNDIALS_EXPORT int ARKStepGetNonlinSolvStats(void* arkode_mem, long int* nniters,
                                              long int* nnfails);
SUNDIALS_EXPORT int ARKStepGetNumStepSolveFails(void* arkode_mem,
                                                long int* nncfails);

/* Linear solver optional output functions */
SUNDIALS_EXPORT int ARKStepGetJac(void* arkode_mem, SUNMatrix* J);
SUNDIALS_EXPORT int ARKStepGetJacTime(void* arkode_mem, sunrealtype* t_J);
SUNDIALS_EXPORT int ARKStepGetJacNumSteps(void* arkode_mem, long int* nst_J);
SUNDIALS_EXPORT int ARKStepGetLinWorkSpace(void* arkode_mem, long int* lenrwLS,
                                           long int* leniwLS);
SUNDIALS_EXPORT int ARKStepGetNumJacEvals(void* arkode_mem, long int* njevals);
SUNDIALS_EXPORT int ARKStepGetNumPrecEvals(void* arkode_mem, long int* npevals);
SUNDIALS_EXPORT int ARKStepGetNumPrecSolves(void* arkode_mem, long int* npsolves);
SUNDIALS_EXPORT int ARKStepGetNumLinIters(void* arkode_mem, long int* nliters);
SUNDIALS_EXPORT int ARKStepGetNumLinConvFails(void* arkode_mem,
                                              long int* nlcfails);
SUNDIALS_EXPORT int ARKStepGetNumJTSetupEvals(void* arkode_mem,
                                              long int* njtsetups);
SUNDIALS_EXPORT int ARKStepGetNumJtimesEvals(void* arkode_mem,
                                             long int* njvevals);
SUNDIALS_EXPORT int ARKStepGetNumLinRhsEvals(void* arkode_mem,
                                             long int* nfevalsLS);
SUNDIALS_EXPORT int ARKStepGetLastLinFlag(void* arkode_mem, long int* flag);

SUNDIALS_EXPORT int ARKStepGetMassWorkSpace(void* arkode_mem, long int* lenrwMLS,
                                            long int* leniwMLS);
SUNDIALS_EXPORT int ARKStepGetNumMassSetups(void* arkode_mem, long int* nmsetups);
SUNDIALS_EXPORT int ARKStepGetNumMassMultSetups(void* arkode_mem,
                                                long int* nmvsetups);
SUNDIALS_EXPORT int ARKStepGetNumMassMult(void* arkode_mem, long int* nmvevals);
SUNDIALS_EXPORT int ARKStepGetNumMassSolves(void* arkode_mem, long int* nmsolves);
SUNDIALS_EXPORT int ARKStepGetNumMassPrecEvals(void* arkode_mem,
                                               long int* nmpevals);
SUNDIALS_EXPORT int ARKStepGetNumMassPrecSolves(void* arkode_mem,
                                                long int* nmpsolves);
SUNDIALS_EXPORT int ARKStepGetNumMassIters(void* arkode_mem, long int* nmiters);
SUNDIALS_EXPORT int ARKStepGetNumMassConvFails(void* arkode_mem,
                                               long int* nmcfails);
SUNDIALS_EXPORT int ARKStepGetNumMTSetups(void* arkode_mem, long int* nmtsetups);
SUNDIALS_EXPORT int ARKStepGetLastMassFlag(void* arkode_mem, long int* flag);

SUNDIALS_EXPORT char* ARKStepGetLinReturnFlagName(long int flag);

/* Free function */
SUNDIALS_EXPORT void ARKStepFree(void** arkode_mem);

/* Output the ARKStep memory structure (useful when debugging) */
SUNDIALS_EXPORT void ARKStepPrintMem(void* arkode_mem, FILE* outfile);

/* MRIStep interface functions */
SUNDIALS_EXPORT int ARKStepCreateMRIStepInnerStepper(void* arkode_mem,
                                                     MRIStepInnerStepper* stepper);

/* Relaxation functions */
SUNDIALS_EXPORT int ARKStepSetRelaxFn(void* arkode_mem, ARKRelaxFn rfn,
                                      ARKRelaxJacFn rjac);
SUNDIALS_EXPORT int ARKStepSetRelaxEtaFail(void* arkode_mem, sunrealtype eta_rf);
SUNDIALS_EXPORT int ARKStepSetRelaxLowerBound(void* arkode_mem,
                                              sunrealtype lower);
SUNDIALS_EXPORT int ARKStepSetRelaxMaxFails(void* arkode_mem, int max_fails);
SUNDIALS_EXPORT int ARKStepSetRelaxMaxIters(void* arkode_mem, int max_iters);
SUNDIALS_EXPORT int ARKStepSetRelaxSolver(void* arkode_mem,
                                          ARKRelaxSolver solver);
SUNDIALS_EXPORT int ARKStepSetRelaxResTol(void* arkode_mem, sunrealtype res_tol);
SUNDIALS_EXPORT int ARKStepSetRelaxTol(void* arkode_mem, sunrealtype rel_tol,
                                       sunrealtype abs_tol);
SUNDIALS_EXPORT int ARKStepSetRelaxUpperBound(void* arkode_mem,
                                              sunrealtype upper);
SUNDIALS_EXPORT int ARKStepGetNumRelaxFnEvals(void* arkode_mem,
                                              long int* r_evals);
SUNDIALS_EXPORT int ARKStepGetNumRelaxJacEvals(void* arkode_mem,
                                               long int* J_evals);
SUNDIALS_EXPORT int ARKStepGetNumRelaxFails(void* arkode_mem,
                                            long int* relax_fails);
SUNDIALS_EXPORT int ARKStepGetNumRelaxBoundFails(void* arkode_mem,
                                                 long int* fails);
SUNDIALS_EXPORT int ARKStepGetNumRelaxSolveFails(void* arkode_mem,
                                                 long int* fails);
SUNDIALS_EXPORT int ARKStepGetNumRelaxSolveIters(void* arkode_mem,
                                                 long int* iters);

#ifdef __cplusplus
}
#endif

#endif
