c Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c ...
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c Copyright (C) 2024 - UTC - Stéphane MOTTELET
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
c
c     Examples for daskr 
c     ----------------------
c
c
      subroutine grd1 (neq, t, y, yd, ng, groot, rpar, ipar)
      INTEGER neq, ng,ipar(*)
      DOUBLE PRECISION t, y(*), yd(*), groot(*),rpar(*)
      groot(1) = ((2.0D0*LOG(y(1)) + 8.0D0)/t - 5.0D0)*y(1)
      groot(2) = LOG(y(1)) - 2.2491D0
      RETURN
      END

      subroutine grd2 (neq, t, y, yd, ng, groot, rpar, ipar)
      INTEGER neq, ng, ipar(*)
      DOUBLE PRECISION t, y(*), yd(*), groot(*),rpar(*)
      groot(1) = y(1)
      RETURN
      END

