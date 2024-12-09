// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2024 - Dassault Systèmes - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
//<-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//

// load the reference SSP file from https://www.easy-ssp.com/app/
scs_m = scicosDiagramToScilab("SCI/modules/xcos/tests/unit_tests/DC Motor.ssp")

// chec various properties
assert_checkequal(scs_m.props.title(1), "DC-Motor")
assert_checkequal(length(scs_m.objs), 14);

assert_checkequal(scs_m.objs(1).gui, "SSPOutputConnector")
assert_checkequal(scs_m.objs(1).graphics.exprs, ["M_mot";"256"])

assert_checkequal(scs_m.objs(7).gui, "SimpleFMU")
assert_checkequal(scs_m.objs(7).graphics.exprs, ["stimuli_model.fmu";"TMPDIR/resources/stimuli_model"])

assert_checkequal(scs_m.objs(6).gui, "SUPER_f")

inner = scs_m.objs(6).model.rpar;
assert_checkequal(inner.props.title(1), "SuT")
assert_checkequal(length(inner.objs), 17);

assert_checkequal(inner.objs(7).gui, "SimpleFMU")
assert_checkequal(inner.objs(7).graphics.exprs, ["edrive_mass.fmu";"TMPDIR/resources/edrive_mass"])

assert_checkequal(inner.objs(8).gui, "SimpleFMU")
assert_checkequal(inner.objs(8).graphics.exprs, ["emachine_model.fmu";"TMPDIR/resources/emachine_model"])
