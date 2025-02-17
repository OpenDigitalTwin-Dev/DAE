// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SE - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

nx    = 10;

nev   = 3;
ncv   = 6;
bmat  = 'I';
which = 'LM';

// Local Arrays

iparam  = zeros(11, 1);
ipntr   = zeros(14, 1);
_select = zeros(ncv, 1);
d       = zeros(nev, 1);
z       = zeros(nx, nev);
resid   = zeros(nx, 1);
v       = zeros(nx, ncv);
workd   = zeros(3 * nx, 1);
workl   = zeros(ncv * ncv + 8 * ncv, 1);

// Build the symmetric test matrix

A            = diag(10 * ones(nx,1));
A(1:$-1,2:$) = A(1:$-1,2:$) + diag(6 * ones(nx-1,1));
A(2:$,1:$-1) = A(2:$,1:$-1) + diag(6 * ones(nx-1,1));

tol    = 0;
ido    = 0;

ishfts = 1;
maxitr = 300;
mode1  = 1;

iparam(1) = ishfts;
iparam(3) = maxitr;
iparam(7) = mode1;

sigma = 0; // the real part of the shift
info_dsaupd = 0;

// M A I N   L O O P (Reverse communication)

while(ido <> 99)
  // Repeatedly call the routine DSAUPD and take actions indicated by parameter IDO until
  // either convergence is indicated or maxitr has been exceeded.

  [ido, resid, v, iparam, ipntr, workd, workl, info_dsaupd] = %_dsaupd(ido, bmat, nx, which, nev, tol, resid, ncv, v, iparam, ipntr, workd, workl, info_dsaupd);

  if(info_dsaupd < 0)
    printf('\nError with dsaupd, info = %d\n',info_dsaupd);
    printf('Check the documentation of dsaupd\n\n');
  end

  if(ido == -1 | ido == 1)
    // Perform matrix vector multiplication
    workd(ipntr(2):ipntr(2) + nx - 1) = A * workd(ipntr(1):ipntr(1) + nx - 1);
  end
end

// Post-Process using DSEUPD.
rvec    = 1;
howmany = 'A';
info_dseupd = 0;

[d, z, resid, v, iparam, ipntr, workd, workl, info_dseupd] = %_dseupd(rvec, howmany, _select, d, z, sigma, bmat, nx, which, nev, tol, resid, ncv, v, ...
                                                                    iparam, ipntr, workd, workl, info_dseupd);

if(info_dseupd < 0)
  printf('\nError with dseupd, info = %d\n', info_dseupd);
  printf('Check the documentation of dseupd.\n\n');
end


assert_checkalmostequal(A * z, z * diag(d), sqrt(%eps), 1.e-10);
