
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000 - INRIA - Carlos Klimann
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//

function [s]=wcenter(x,orient)
    //
    //This function computes  s, the centered weighted version
    //of the numerical matrix x.
    //
    //For a  vector or a  matrix x, s=wcenter(x)  returns in
    //the  (i,j)  coefficient  of   the  matrix  s  the  value
    //(x(i,j)-xbar)/sigma,  where  xbar  is  the mean  of  the
    //values of  the coefficients of x and  sigma his standard
    //deviation.
    //
    //s=wcenter(x,'r')  (or, equivalently, s=wcenter(x,1))
    //is the rowwise  centre reduction of the values  of x. It
    //returns    in    the     entry    s(i,j)    the    value
    //(x(i,j)-xbarv(j))/sigmav(j)  with xbarv(j)  the  mean of
    //the values  of the j  column and sigmav(j)  the standard
    //deviation of the j column of x.
    //
    //s=wcenter(x,'c')  (or, equivalently, s=wcenter(x,2))
    //is  the columnwise  centre  reduction of  the values  of
    //x.   It  returns   in   the  entry   s(i,j)  the   value
    //(x(i,j)-xbarh(i))/sigmah(i)  with xbarh(i)  the  mean of
    //the  values of  the  i row  and  sigmah(i) the  standard
    //deviation of the i row of x.
    //
    //

    arguments
        x
        orient (1, 1) {mustBeA(orient, ["double", "string"]), mustBeMember(orient, {1, 2, "r", "c", "*"})} = "*"
    end

    if x==[] then s=%nan, return, end
    [m n]=size(x);
    if orient == "*" then
        xbar=(sum(x)/(m*n))
        s=x-(ones(m,n)*xbar)
        sigma=sqrt(sum(s .^ 2)/(m*n-1))
        s=s/sigma
    elseif orient=="c"|orient==2 then
        xbar=sum(x,"c")/n
        s=x-(xbar*ones(1,n))
        sigma=sqrt(sum((s.^2),"c")/(n-1))
        s=s./(sigma*ones(1,n))
    elseif orient=="r"|orient==1 then
        xbar=sum(x,"r")/m
        s=x-(ones(m,1)*xbar)
        sigma=sqrt(sum((s.^2),"r")/(m-1))
        s=s./(ones(m,1)*sigma)
    end
endfunction
