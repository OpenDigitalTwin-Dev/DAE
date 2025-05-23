//
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// Examples of Standard plants (mostly from Kwakernak)
// Define the polynomial s by typing "s=poly(0,'s')"
// Most examples are in transfer representation but computations
// are done in state space form

//2.4.1 (Kwakernaak)
s = poly(0, "s");
H=[1 (s-1)/((s-2)*(s-3));
   1 (s-1)/((s-2)*(s-3))];
r=[1 1];

//Transform to get d12 full rank

H(:,2)=H(:,2)*(s+1);

H = syslin("c", H);
[sk,mu]=h_inf(H,r,0,1,20);

//transform back

sk=sk*(s+1);

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//3.9.1 
c=0.1;r=0;

H=[(1+sqrt(2)*s+s**2)/(s*s) 1/(s*s);
0 c+c*r*s;
(1+sqrt(2)*s+s**2)/(s*s) 1/(s*s)];

r=[1 1];

//[sk,mu]=h_inf(H,r,0,1,20);

//compare (185) in como notes

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

c=0.1;r=1;

H=[(1+sqrt(2)*s+s**2)/(s*s) 1/(s*s);...
   0 c+c*r*s;..
   (1+sqrt(2)*s+s**2)/(s*s) 1/(s*s)];

r=[1 1];

// divide 2nd column of h by (s+2) for properness
H(:,2)=H(:,2)/(s+2);

//[sk,mu]=h_inf(H,r,0,1,30);

// Remove parasitic root introduced above :
//vv=roots(sk(2))   //vv(2) = -2 approximately 

//sk=sk/real((s-vv(2)))

//compare (186) como notes

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//3.12.1

c=0.1;
H=[(s+1)/s 1/s;
  0  c;
 (s+1)/s 1/s];

r=[1 1];

//[sk,mu]=h_inf(H,r,0,1,20);

//sk=clean(sk,0,0.00001);

//compare (228)
H = syslin("c", H);

[sk,mu]=h_inf(H,r,0,10,20);

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//4.12.1

H=[(s*s+sqrt(2)*s+1)/(s*s) 1/(s*s);
 0 1;
(s*s+sqrt(2)*s+1)/(s*s) 1/(s*s)];

r=[1 1];

//[sk,mu]=h_inf(H,r,0,10,20);

//compare (422) which is misprinted !

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//1990 IFAC (Kwakernaak)

H=[-(1+s/4)/(1+2*s) (s+1)/(s-2) 0 1/(s-2) 0;
   0 0 0 1 0;
   0 0 0 0 1;
(1+s/4)/(1+2*s) 0 0 0 0;
 0 (s+1)/(s-2) 1 1/(s-2) 0];

r=[2 2];

//[Sk,mu]=h_inf(H,r,0,1,30);

//compare (33) in Matlab report 

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//Mc-Farlane Glover for 1/s^2

G=[(1+s*sqrt(2)+s*s)/(s*s) 1/(s*s);
   0 1;
   (1+s*sqrt(2)+s*s)/(s*s) 1/(s*s)]

r=[1,1]

//[sk,mu]=h_inf(G,r,0,1,20)

//compare (25) in Matlab report

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//Mc Farlane Glover for 1/s^3



G=[(1+2*s+2*s*s+s**3)/(s**3) 1/(s**3);
  0 1;
  (1+2*s+2*s*s+s**3)/(s**3) 1/(s**3)]

r=[1 1];
//[sk,mu]=h_inf(G,r,0,1,20)

//compare (27) in Matlab report

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//SISO Model matching

G=[6*(s-1)/((s-2)*(s-3)) 1;
   -1 0];
r=[1 1];

//multiply first row by all-pass transfer to have stabilizability
//and detectability

k=((s-2)*(s-3))/((s+2)*(s+3))

G(1,:)=k*G(1,:)

G = syslin("c", G);

[sk,mu]=h_inf(G,r,0,5,20)

//compare (42) in Matlab report

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//Mimo Model matching

G=[0.5/(s-1) 0 1 0;
   1/(s*s-s+1) 2/(s-1) 0 1;
    -1 0 0 0;
     0 -1 0 0];

r=[2 2]

p=(s*s-s+1)*(s-1);

q=p/horner(p,-s);

H=[q 0;
   0 q]

//multiplication from the left by inner H

G(1:2,:)=H*G(1:2,:);

//Stabilizability and detectability assumptions are now verified
G = syslin("c", G);
[sk,mu]=h_inf(G,r,0,1,30);

// the parasitic (?) root -83.60 (Kwakernaak) or -84.53 (Francis) is not
// in the poles of sk :
// one finds the roots -0.6920857 + or -0.7932064i and -1.1030557

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//Mixed sensitivity (ex 11 in matlab notes)

G=[(s+1)/s 0 1/s 0;
   0 (s+0.5)/(s-1) 0 1/(s-1);
   0 0 1 0;
   0 0 0 1;
(s+1)/s 0 1/s 0;
0 (s+0.5)/(s-1) 0 1/(s-1)];
G = syslin("c", G);

r=[2 2]

[sk,mu]=h_inf(G,r,0,1,30);

sk=clean(sk,1.d-6)

//compare (40) in Matlab notes

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//Laub's ex (CDC 90)

A=[0.0904 0.8465;
   0.6888 0.7152];

B=[0.7092 0.3017 0.7001;
   0.1814 0.9525 0.1593];

C=[0.3088 0.7350;
   0.5735 0.9820;
   0.6644 0.5627];

D=[0.7357 0.4156 0.0;
   0.2588 0.1544 1.0;
    0.0    1.0   0.0];

r=[1 1];

p=syslin('c', A, B, C, D);

//[sk,mu]=h_inf(P,r,0,10,20);

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//Mustafa's ex

A=[20 -100;1 0];
B=[1;0];
C=[1,-0.1];
Gss=syslin('c',A,B,C);
[Pss, r] = lqg2stan(Gss, eye(3,3), eye(3,3)); //The standard plant

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//Maciejowski's example

At=[-0.001 0 1.1320 0 -1;
 0 -0.0538 -0.1712 0 0.0705;
 0 0 0 1 0;
 0 0.0485 0 -0.8556 -1.013;
 0 -0.2909 0 1.0532 -0.6859];

B=[0 0 0;
   -0.120 1 0;
 0  0 0;
4.4190 0 -1.665;
1.575 0 -0.0732];

C=[1 0 0 0 0;
   0 1 0 0 0;
   0 0 1 0 0];

Dt=1.d-5*eye(3,3);
D=0*Dt;

Gss=syslin('c',At,B,C,D);

//gtf=ss2tf(Gss);gtf=clean(gtf,1.d-10);

w1=syslin('c',((s+6)**2)/((s+0.00006)*(s+0.6)));

w2=syslin('c',2000*(s+10)*(s+50)/((s+1000)**2));

W1ss=tf2ss(w1);
W2ss=tf2ss(w2);

W1=[W1ss,0,0;0,W1ss,0;0,0,W1ss];
W2=[W2ss,0,0;0,W2ss,0;0,0,W2ss];

A1=W1(2);B1=W1(3);C1=W1(4);D1=W1(5);
A2=W2(2);B2=W2(3);C2=W2(4);D2=W2(5);

//Ptf=[W1, W1*Gtf;
//     0*eye(3,3), W2*Gtf;
//     eye(3,3), -Gtf];

//Pss=tf2ss(Ptf);

r=[3,3];

Ap=[At,0*eye(5,6),0*eye(5,6);
   B1*C,A1,0*eye(6,6);
   B2*C,0*eye(6,6),A2];

Bp=[0*eye(5,3),B;
   -B1,B1*D;
    0*eye(6,3),B2*D];

Cp=[-D1*C,-C1,0*eye(3,6);
    D2*C,0*eye(3,6),C2;
    -C,0*eye(3,6),0*eye(3,6)];

Dp=[D1,-D*Dt;
    0*eye(3,3),D2*Dt;
   eye(3,3),-D];

P=syslin('c',Ap,Bp,Cp,Dp);

[sk,mu]=h_inf(P,r,0,1,30);

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//Safonov-Chiang

A=[-2.2567e-02  -36.617  -18.897  -32.090  3.2509  -7.6257e-1;
  9.2572e-05  -1.8997  9.8312e-01  -7.2562e-04  -1.7080e-01  -4.9652e-03;
    1.2338e-02  11.720  -2.6316  8.7582e-04  -31.604  22.396;
    0           0        1       0            0       0  ;
    0           0        0       0           -30      0  ;
    0           0        0       0            0      -30  ];

B=0*ones(6,2);B(5,1)=30;B(6,2)=30;
C=0*ones(2,6);C(1,2)=1;C(2,4)=1;
D=0*ones(2,2);
G=syslin('c',A,B,C);r=syssize(G);
W1=[(s+0.01)/(1+0.01*s)  0;0    (s+0.01)/(0.01*s+1)], 
tau=0.0005;
W2=[1000/(s*s)   0;0    1000/(s*s*(s*tau+1))],


//

s=poly(0,'s');
G=(s+1)/(s*s+1);
w1=1;w3=(s+2);

P=[w1, -w1*G;
   0 , w3*G
   1 , -G];

r=[1,1];
Pss=tf2ss(P);
P12=Pss(1:2,2);P21=Pss(3,1);P22=Pss(3,2);
sl=P21;
sm=[s*eye(sl(2))-sl(2),sl(3);sl(4),-sl(5)];
detr(sm)       //Poles of G --> Transmission zeros of P21
P = syslin("c", P);
[sk,mu]=h_inf(P,r,0,1,20);   //Fails

Ptmp=P;fil=(1+s*s)/(1+s)/(1+s);Ptmp(:,2)=Ptmp(:,2)*fil;
[sk,mu]=h_inf(Ptmp,r,0,1,10);    //---> sk=0 mu=1 OK

// Random example...

nx=5;
nu=2;ny=3;
nw=4;nz=3;
A =[1,2,3,0,1;4,5,6,1,0;7,8,9,1,1;0,1,0,1,1;5,4,3,2,1];
B2=[1,2;2,1;1,1;1,0;2,0];  //(nx,nu)
C2=[1,1,0,1,0;0,1,1,2,1;2,3,1,0,1];   //(ny,nx)
r=[ny,nu];

//%Q=rand(nx+nu,3)*rand(3,nx+nu);Q=Q*Q';
//%R=rand(nx+ny,2)*rand(2,nx+ny);R=R*R';


D12=[0,1;2,1;3,0];    //%(nz,nu)
D21=[1,2,1,1;2,1,1,2;0,2,1,1];    //%(ny,nw);
C1=[1,1,0,1,1;2,1,1,0,1;1,0,0,1,1];   //%(nz,nx)
B1=[1,0,1,0;2,3,3,1;3,1,1,0;1,2,1,0;0,1,1,0];  //%(nx,nw)
//%Q#=[C1 D12]'*[C1 D12];
//%R#=[B1;D21]*[B1;D21]';
D11=2*[1,2,1,0;2,2,1,1;0,0,2,1]; //%(nz,nw)
D11=D11;
D22=0*[1,2;2,1;3,1];       //%(ny,nu)
gama=64;
B1=B1/sqrt(gama);
D11=D11/sqrt(gama);
D21=D21/sqrt(gama);
B2=B2*sqrt(gama);
D12=D12*sqrt(gama);
C1=C1/sqrt(gama);
D11=D11/sqrt(gama);
D12=D12/sqrt(gama);
C2=C2*sqrt(gama);
D21=D21*sqrt(gama);
B=[B1,B2];C=[C1;C2];
D=[D11,D12;D21,D22];

P=syslin('c',A,B,C,D);

[K,mu]=h_inf(P,r,0,10,20);

//    Difficult example gamma between 1.6 and 1.8
// h_inf finds 1.671244...  and
// K=[(9.7574+0.2790*s)/(0.023419+s), 0.1346]
// with P1=minreal(P) (state dimension 5 instead of 8 for P)
// get same gamma but a controller of order (4,3)
W=[-1.05033595281261e+00 1 0  -2.72165256144227e+03 0 0 0 0 0 0 0 0 0 0;
   -1.39973553704802e+02 0 0  -2.94468368703638e+06 0 0 0 0 0 0 0 0 0 0;
   0 0  -2.100e+02  -2.250e+04 0 0 0 0 0   3.80748204520352e-01 0 0 0 1;
   0 0 1 0 0 0 0 0 0 0 0 0 0 0;
   0,0,0,0, -4.89284164859002e+03, -7.98004338394794e+06,...
 -4.33839479392625e+09,0 , 4.49639998582548e-02,0,0,0,0,0;
   0 0 0 0 1 0 0 0 0 0 0 0 0 0;
   0 0 0 0 0 1 0 0 0 0 0 0 0 0;
  1.80466256786985e+00 0 0   4.58687395748091e+03 0 0 0,...
  -2.34192037470726e-02 0 0 1  -1 0 0;
   0 0 0 1.25100080458419e+04 0 0 0 0 0 0 0 0 0 0;
   0 0 0 0 0 0 0 0 0   6.000e-01 0 0 0 1.57584459460774e+00;
   6.31631898754447e-01 0 0 1.60540588511832e+03 0 0 0,...
   4.67564402810304e+00 0 0   3.500e-01  -3.500e-01 0 0;
 1.80466256786985e+00 0 0   4.58687395748091e+03 0 0 0 0 0 0 1  -1 0 0;
   0 1 0 0 0 0 0 0 1.79855999433019e+00 0 0 0 1 0];

A=W(1:8,1:8);B=W(1:8,9:14);C=W(9:13,1:8);D=W(9:13,9:14);
P=syslin('c',A,B,C,D);
r=[2,1];

[K,mu]=h_inf(P,r,0,1,40);

/////////////////////////////////////////////////////////////////

//Automotive fuel control
eps=0.001;
s=poly(0,'s');
P=(5.498*s*s+4.007d+2*s-4.444d+5)/((s+eps)*(s**3+93.72*s*s+9.520d+3*s+1.214d+5));
P=P*(18000/5.498);
ro=2;
w2=(s+30)*(s+60)/30/60/10;
w1=20*ro/((s+eps)*(s+20));
G=[w1,-w1*P;
   0 ,w2*P;
   1, -P];
W1ss=tf2ss(w1);
Pss=tf2ss(P);
W2P=w2*P;
//calcul de cw2;
www=clean((s*eye(Pss(2))-Pss(2))**(-1)*Pss(3));
mat=[horner(www,-1),horner(www,1),horner(www,10),horner(www,20)];
ww=clean(W2P-1);
f=[horner(ww,-1),horner(ww,1),horner(ww,10),horner(ww,20)];
cw2=f/mat;
W2pss=syslin('c',Pss(2),Pss(3),cw2,1);

AGss=[W1ss(2),W1ss(3)*Pss(4);0*eye(4,2),Pss(2)];
BGss=[-W1ss(3),0*ones(2,1);0*ones(4,1),Pss(3)];
CGss=[-W1ss(4),0*ones(1,4);0*ones(1,2),W2pss(4);0*ones(1,2),-Pss(4)];
DGss=[0,0;0,1;1,0];
Gss=syslin('c',AGss,BGss,CGss,DGss);
r=[1,1];
[a,b1,b2,c1,c2,d11,d12,d21,d22]=smga(Gss,r);

[K,mu]=h_inf(Gss,r,0,100,20);

////////////SERE
s=poly(0,'s')
g=-4.714841d-3*(s**2+0.0218517*s+1.793965)*(s**2-0.3057194*s+4.752967);
g=g*(s**2+0.4002371*s+4.812033)*(s-4.911808);
g=g/((s**2)*(s**2+0.0221061*s+1.833885)*(s**2+7.392301d-3*s+3.18453));
g=g/(s**2+2.716316d-2*s+4.624171);
w1=0.33333333*s**3+0.3494322*s**2+9.157714d-2*s+0.012;
w1=w1/((s**3)*(0.01*s+1));
w2=2*s/(s+100);
P=[w1,-w1*g;
   0 , w2;
   1, -g];
P = syslin("c", P);
[sk,mu]=h_inf(P,[1,1],1,1,1);
