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

function [facpr, comprinc, lambda, tsquare, explainedvar, mu]=princomp(x,eco)
    //
    //This  function performs  several  computations known  as
    //"principal component  analysis".
    //The  idea  behind this  method  is  to  represent in  an
    //approximative  manner a  cluster of  n individuals  in a
    //smaller  dimensional subspace.  In order  to do  that it
    //projects the cluster onto a subspace.  The choice of the
    //k-dimensional projection subspace is  made in such a way
    //that  the distances  in  the projection  have a  minimal
    //deformation: we are looking for a k-dimensional subspace
    //such that the squares of the distances in the projection
    //is  as  big  as  possible  (in  fact  in  a  projection,
    //distances can only stretch).  In other words, inertia of
    //the projection  onto the k dimensional  subspace must be
    //maximal.
    //

    //References: Saporta, Gilbert, Probabilites,  Analyse des
    //Donnees et Statistique, Editions Technip, Paris, 1990.
    //
    //derived from pca,
    //  author: carlos klimann
    //
    arguments
        x
        eco {mustBeA(eco, "boolean")} = %f
    end

    warnobsolete("pca", "2026.0.0");

    if x==[] then
        facpr=[];
        comprinc=[];
        lambda=[];
        tsquare=[];
        return;
    end

    [rowx,colx]=size(x)

    //center data
    y=center(x,1)

    //compute eigenvectors of  y'*y using svd
    if eco then
        [U,lambda,facpr]=svd(y,"e");
    else
        [U,lambda,facpr]=svd(y);
    end
    lambda=diag(lambda).^2/(rowx-1);//normalize
    comprinc=y*facpr
    if rowx <= colx then
        if eco then
            lambda(rowx)=[]
            facpr(:,rowx)=[];
            comprinc(:,rowx) = [];
        else
            lambda(rowx:$)=[]
            comprinc(:,rowx:$) = [];
        end
    end
    q=find(lambda<=max(rowx,colx)*%eps*lambda(1),1)
    if q==[] then q=size(lambda,"*"),end
    tsquare=(rowx-1)*sum(U(:,1:q).^2,2)

    if nargout > 4 then
        // percentage of variance explained by each principal component
        explainedvar = 100 * lambda / sum(lambda);
    end

    if nargout > 5 then
        mu = mean(x, 1);
    end
endfunction
