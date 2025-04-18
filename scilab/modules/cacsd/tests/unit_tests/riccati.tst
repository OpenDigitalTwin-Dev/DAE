// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// ============================================================================
// Tests for riccati() function
// Added : 25/06/2007
// Thanks to Sabine GAUZERE
// ============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
// <-- NO CHECK REF -->

rand("seed", 2);

n = 10;
A = rand(n,n);
B = rand(n,n);
C = rand(n,n);
C = C*C';
R = rand(n,n);
R = R*R' + eye();
B = B * inv(R) * B';

// Test de l'équation en temps continu
// -----------------------------------
// (Eq) A'*X + X*A - X*B*X + C = 0
// From the matricial coefficients of the equation
X = riccati(A,B,C,'c','eigen');

C_cont = A'*X+X*A-X*B*X;
assert_checktrue(norm(C_cont+C,1) < (10000*%eps));

[X1,X2,residual] = riccati(A,B,C,'c','eigen');
assert_checkequal(X, X1/X2);
assert_checktrue(residual < 1e-12);

// From the Hamiltonian of the equation
H = [A, -B ; -C, -A'];
x = riccati(H);
assert_checkequal(x,X);
[x1, x2, residual] = riccati(H);
assert_checkequal(x, x1/x2);
assert_checktrue(residual < 1e-12);

// No solution
a = zeros(3,3);
b = a;
c = eye(3,3);
X = riccati(a, b, c, 'c','eigen');
assert_checkequal(X, []);
[X1, X2, residual] = riccati(a, b, c, 'c', 'eigen');
assert_checkequal(X1, []);
assert_checkequal(residual, []);


// Test de l'équation en temps discret
// -----------------------------------
// From the matricial coefficients of the equation 
F      = A;
B      = rand(n,n);
G1     = B;
G2     = R;
G      = G1/G2*G1';

X = riccati(F,G,C,'d','schur');
C_disc = F'*X*F-(F'*X*G1/(G2+G1'*X*G1))*(G1'*X*F)-X;
assert_checktrue(norm(C_disc+C,1) < 10000*%eps);

[X1, X2, residual] = riccati(F,G,C,'d','schur');
assert_checkequal(X, X1/X2);
assert_checktrue(residual < 1e-12);

// From the Hamiltonian pencil (E,H) of the equation
H = [F, zeros(n,n) ; -C, eye(n,n)];
E = [eye(n,n), G ; zeros(n,n), F'];
x = riccati(H, E);
assert_checkequal(x, X);
[x1, x2, residual] = riccati(H, E);
assert_checkequal(x, x1/x2);
assert_checktrue(residual < 1e-12);
