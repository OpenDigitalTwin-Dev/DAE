C $Header$
C MBDyn (C) is a multibody analysis code. 
C http://www.mbdyn.org
C 
C Copyright (C) 1996-2023
C 
C Pierangelo Masarati        <pierangelo.masarati@polimi.it>
C Paolo Mantegazza        <paolo.mantegazza@polimi.it>
C 
C Dipartimento di Ingegneria Aerospaziale - Politecnico di Milano
C via La Masa, 34 - 20156 Milano, Italy
C http://www.aero.polimi.it
C 
C Changing this copyright notice is forbidden.
C 
C This program is free software; you can redistribute it and/or modify
C it under the terms of the GNU General Public License as published by
C the Free Software Foundation (version 2 of the License).
C 
C 
C This program is distributed in the hope that it will be useful,
C but WITHOUT ANY WARRANTY; without even the implied warranty of
C MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
C GNU General Public License for more details.
C 
C You should have received a copy of the GNU General Public License
C along with this program; if not, write to the Free Software
C Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
C
        SUBROUTINE KD01A ( IV, MAXKEY, ITABLE, KEY )

        IMPLICIT NONE
        
        INTEGER          MAXKEY
        INTEGER          IV( 3 ), ITABLE( * ), KEY( * )
        INTEGER          LENGTH, IPRIME, IEMPTY
C          COMMON /KD01CM/  LENGTH, IPRIME, IEMPTY
C
C  SET UP INITIAL SCATTER TABLE (WILLIAMS, CACM 2, 21-24, 1959).
C
C  ITABLE( I ) GIVES THE STATUS OF TABLE ENTRY I.
C  IF STATUS = - ( LENGTH + 1 ), THE ENTRY IS UNUSED.
C
        INTEGER  I
        EXTERNAL KD01H
        LOGICAL  KD01H

        LENGTH = MAXKEY
        IEMPTY = LENGTH + 1

C
C  FIND AN APPROPRIATE PRIME NUMBER FOR THE HASH FUNCTION.
C  COMPUTE THE LARGEST PRIME SMALLER THAN LENGTH.
C
        IPRIME = 2 * ( ( LENGTH + 1 ) / 2 ) - 1
   10 CONTINUE
C
C  IS IPRIME PRIME ?
C
        IF ( .NOT. KD01H ( IPRIME ) ) THEN
           IPRIME = IPRIME - 2
           GO TO 10
        END IF
C
C  INITIALIZE EACH TABLE ENTRY AS UNFILLED.
C
        DO 30 I        = 1, LENGTH
           ITABLE( I ) = - IEMPTY
C
C  FILL EACH KEY WITH BLANKS.
C
           KEY( I ) = 0
   30 CONTINUE
C
C set static data to structure rather than to common
C
        IV( 1 ) = LENGTH
        IV( 2 ) = IPRIME
        IV( 3 ) = IEMPTY
C
        RETURN
C
C  END OF KD01A.
C
        END



        SUBROUTINE KD01B ( IV, ITABLE, KEY, FIELD, IFREE )
        INTEGER          FIELD, IFREE
        INTEGER          IV( 3 ), ITABLE( * ), KEY( * )
        INTEGER          LENGTH, IPRIME, IEMPTY
C          COMMON /KD01CM/  LENGTH, IPRIME, IEMPTY
C
C  INSERT IN CHAINED SCATTER TABLE (WILLIAMS, CACM 2, 21-24, 1959).
C
C  ITABLE( I ) GIVES THE STATUS OF TABLE ENTRY I.
C  IF STATUS = - ( LENGTH + 1 ), THE ENTRY IS UNUSED.
C  IF STATUS = - K, THE ENTRY WAS USED BUT HAS BEEN DELETED. K GIVES
C              THE INDEX OF THE NEXT ENTRY IN THE CHAIN.
C  IF STATUS = 0, THE ENTRY IS USED AND LIES AT THE END OF A CHAIN.
C  IF STATUS = K, THE ENTRY IS USED. K GIVES THE INDEX OF THE NEXT
C              ENTRY IN THE CHAIN.
C
        INTEGER   KD01F
        INTRINSIC IABS, MOD
C
C  HASH THE FIELD IN KD01F AND ENSURE THAT IFREE LIES WITHIN THE ALLOWED RANGE.
C
        KD01F ( M, N ) = MOD( IABS( MOD( N, M ) ) + 1, M ) + 1
C
C get static data from structure rather than from common
C
        LENGTH = IV( 1 )
        IPRIME = IV( 2 )
        IEMPTY = IV( 3 )
C
C  FIND A STARTING POSITION, IFREE, FOR THE INSERTION BY HASHING FIELD.
C
        IFREE = KD01F( IPRIME, FIELD )
C
C  IS THERE A LIST ?
C
        IF ( ITABLE( IFREE ) .GE. 0 ) THEN
C
C  COMPARE TO SEE IF THE KEY HAS BEEN FOUND.
C
   30    CONTINUE
                IF ( FIELD .NE. KEY( IFREE ) ) GO TO 50
C
C  THE KEY ALREADY EXISTS AND THEREFORE CANNOT BE INSERTED.
C
           IF ( ITABLE( IFREE ) .GE. 0 ) THEN
                IFREE = - IFREE
                GOTO 9999
           END IF
C
C  THE KEY USED TO EXIST BUT HAS BEEN DELETED AND MUST BE RESTORED.
C
           GO TO 100
C
C  ADVANCE ALONG THE CHAIN TO THE NEXT ENTRY.
C
   50    CONTINUE
           IF ( ITABLE( IFREE ) .NE. 0 ) THEN
                IFREE = IABS( ITABLE( IFREE ) )
                GO TO 30
           END IF
C
C  THE END OF THE CHAIN HAS BEEN REACHED. FIND EMPTY ENTRY IN THE TABLE.
C
   60    CONTINUE
           IEMPTY = IEMPTY - 1
           IF ( IEMPTY .EQ. 0 ) THEN
                IFREE = 0
                GOTO 9999
           END IF
           IF ( ITABLE( IEMPTY ) .GE. - LENGTH ) GO TO 60
           ITABLE( IFREE ) = IEMPTY
           IFREE           = IEMPTY
        ELSE
C
C  THE STARTING ENTRY FOR THE CHAIN IS UNUSED.
C
           IF ( ITABLE( IFREE ) .GE. - LENGTH ) THEN
                ITABLE( IFREE ) = - ITABLE ( IFREE )
                GO TO 100
           END IF
        END IF
C
C  THERE IS NO LINK FROM THE NEWLY INSERTED FIELD.
C
        ITABLE( IFREE ) = 0
C
C  INSERT NEW KEY.
C
  100 CONTINUE
        KEY( IFREE ) = FIELD
C
C assigned
C
 9999 CONTINUE
        IV( 3 ) = IEMPTY
        RETURN
C
C  END OF KD01B.
C
        END



        SUBROUTINE KD01C ( IV, ITABLE, KEY, FIELD, IFREE )
        INTEGER          FIELD, IFREE
        INTEGER          IV( 3 ), ITABLE( * ), KEY( * )
        INTEGER          LENGTH, IPRIME, IEMPTY
C          COMMON /KD01CM/  LENGTH, IPRIME, IEMPTY
C
C  SEARCH WITHIN CHAINED SCATTER TABLE (WILLIAMS, CACM 2, 21-24, 1959).
C
C  ITABLE( I ) GIVES THE STATUS OF TABLE ENTRY I.
C  IF STATUS = - ( LENGTH + 1 ), THE ENTRY IS UNUSED.
C  IF STATUS = - K, THE ENTRY WAS USED BUT HAS BEEN DELETED. K GIVES
C              THE INDEX OF THE NEXT ENTRY IN THE CHAIN.
C  IF STATUS = 0, THE ENTRY IS USED AND LIES AT THE END OF A CHAIN.
C  IF STATUS = K, THE ENTRY IS USED. K GIVES THE INDEX OF THE NEXT
C              ENTRY IN THE CHAIN.
C
        INTEGER   KD01F
        INTRINSIC IABS, MOD
C
C  HASH THE FIELD IN KD01F AND ENSURE THAT IFREE LIES WITHIN THE ALLOWED RANGE.
C
        KD01F ( M, N ) = MOD( IABS( MOD( N, M ) ) + 1, M ) + 1
C
C get static data from structure rather than from common
C
        LENGTH = IV( 1 )
        IPRIME = IV( 2 )
C unused
C        IEMPTY = IV( 3 )
C
C  FIND A STARTING POSITION, IFREE, FOR THE INSERTION BY HASHING FIELD.
C
        IFREE = KD01F( IPRIME, FIELD )
C
C  IS THERE A LIST ?
C
        IF ( ITABLE( IFREE ) .LT. - LENGTH ) THEN
           IFREE = 0
           RETURN
        END IF
C
C  COMPARE TO SEE IF THE KEY HAS BEEN FOUND.
C
   30 CONTINUE
        IF ( FIELD .NE. KEY( IFREE ) ) GO TO 50
C
C  CHECK THAT THE TABLE ITEM HAS NOT BEEN REMOVED.
C
           IF ( ITABLE( IFREE ) .LT. 0 ) THEN
                IFREE = - IFREE
           END IF
           RETURN
C
C  ADVANCE TO NEXT.
C
   50    CONTINUE
           IF ( ITABLE( IFREE ) .EQ. 0 ) THEN
                IFREE = 0
                RETURN
           END IF
           IFREE = IABS( ITABLE( IFREE ) )
        GO TO 30
C
C  END OF KD01C.
C
        END



        LOGICAL FUNCTION KD01H ( IPRIME )
        INTEGER IPRIME
C
C  RETURNS THE VALUE .TRUE. IF IPRIME IS PRIME.
C
        INTEGER   I
        INTRINSIC MOD, INT, SQRT, FLOAT
        KD01H  = .FALSE.
        IF ( MOD( IPRIME, 2 ) .EQ. 0 ) RETURN
        DO 10 I = 3, INT( SQRT( FLOAT( IPRIME ) ) ), 2
           IF ( MOD( IPRIME, I ) .EQ. 0 ) RETURN
   10 CONTINUE
        KD01H  = .TRUE.
        RETURN
C
C  END OF KD01H.
C
        END
