      SUBROUTINE SHIFTT(NTRAN,FSIG,DF,TSHIFT)
C     -
      COMPLEX FSIG(NTRAN)
      TWOPI = 8.0*ATAN(1.0)
      DO J=1,NTRAN
         WT = TWOPI*(J-1)*DF*TSHIFT
         FSIG(J) = FSIG(J)*CMPLX(COS(WT),SIN(WT))
      END DO
      RETURN
      END
      SUBROUTINE FORWFT(N,B,DTIME,SIGN)
C
C     USED FOR FORWARD FOURIER TRANSFORM.  CALLS PAD AND FASTFT
C     ON INPUT B IS A REAL ARRAY OF N VALUES
C     PAD INCREASES THE DIMENSION OF B - IF NECESSARY TO THE NEXT
C     POWER OF 2.  N RETURNED BY PAD IS THIS DIMENSION
C     N IS THEN REDEFINED TO BE (N+2)/2
C     FASTFT RETURNS - IN PLACE - THE HERMITIAN TRANSFORM OF B
C     STORED IN THE ORDER (R=REAL, I=IMAGINARY) ...
C     R(0) R(N) R(1) I(1) ... R(N-1) I(N-1)
C     THIS IS THEN REORDERED WITH I(0) = I(N) = 0.0
C     SIGN IS USUALLY +1.0 FOR THE FORWARD TRANSFORM
C
      DIMENSION B(*)
C     V    VIRTUAL B(*)
      DIMENSION W1(2),W2(2),CJ(2),DJ(2)
      CALL PAD(N,B,DTIME)
      NM1=N-1
      CALL FASTFT (NM1,B,SIGN)
      NN = 2**(NM1)
      W3 = 3.141592*SIGN/FLOAT(NN)
      NN = NN + 2
      BR = B(1) + B(2)
      BI = B(1) - B(2)
      B(1) = BR
      B(2) = BI
      DO 10 JI = 4,NN,2
         KI = 2*NN - JI
         W1(1) = B(KI-1)
         W1(2) =-B(KI)
         CJ(1) = 0.5*(B(JI-1) + W1(1))
         CJ(2) = 0.5*(B(JI) + W1(2))
         DJ(1) = 0.5*(B(JI-1) - W1(1))
         DJ(2) = 0.5*(B(JI) - W1(2))
         ARG = FLOAT(JI/2 - 1)*W3 - 1.570796
         W2(1) = DJ(1)*COS(ARG) - DJ(2)*SIN(ARG)
         W2(2) = DJ(1)*SIN(ARG) + DJ(2)*COS(ARG)
         B(KI-1) = CJ(1) - W2(1)
         B(KI) = W2(2) - CJ(2)
         B(JI-1) = CJ(1) + W2(1)
         B(JI) = W2(2) + CJ(2)
 10   CONTINUE
      N = 2**(N-1) + 1
      B(2*N) = 0.0
      B(2*N-1) = B(2)
      B(2) = 0.0
      RETURN
      END
      SUBROUTINE PAD(N,B,DNORM)
C
C     PAD IS CALLED BY FORWFT AND INVRFT.
C     ON INPUT, N IS THE DIMENSION OF THE (REAL) ARRAY B
C     IN PAD THE DIMENSION OF B IS INCREASED (IF NECESSARY) TO THE
C     NEXT POWER OF 2 - BY PADDING WITH ZEROS.
C     THE FINAL DIMENSION OF B IS 2**N - HENCE N IS REDEFINED
C     TO GIVE RESULTS APPROXIMATING AN INTEGRAL TRANSFORM - AT LEAST IN
C     NORMALIZATION - THE NONZERO TERMS IN B ARE MULTIPLED BY DNORM,
C     WHICH WOULD BE DT FOR THE FORWARD TRANSFORM AND DF FOR INVERSE
C
      DIMENSION B(*)
C     V    VIRTUAL B(*)
      NTRANS = 2
      DO 1  III = 1,25
         IF (NTRANS - N)  1,4,2
 1       NTRANS = NTRANS*2
 2       NPL1 = N + 1
         DO 3 J = NPL1,NTRANS
 3          B(J) = 0.0
 4          DO 5 J=1,N
 5             B(J) = DNORM*B(J)
               N = III
               RETURN
               END
      SUBROUTINE FASTFT(K,DATA,SIGN)
C
C     DATA IS A ONE-DIMENSIONAL COMPLEX ARRAY OF SIZE NN = 2**K.
C     THE CONVENTION USED BY CALLING ROUTINES FORWFT AND INVRFT IS
C     THAT SIGN = -1.0 FOR INVERSE TRANSFORMS AND +1 FOR FORWARD
C     TRANSFORMS.
C     REFERENCE--APPLIED OPTICS, 10, 388, 1971.
C
      DIMENSION DATA(*)
C     V    VIRTUAL DATA(*)
      NN = 2**K
      N = 2*NN
      J = 1
      DO 5 I = 1,N,2
         IF(I-J) 1,2,2
 1       TEMPR = DATA(J)
         TEMPI = DATA(J+1)
         DATA(J) = DATA(I)
         DATA(J+1) = DATA(I+1)
         DATA(I) = TEMPR
         DATA(I+1) = TEMPI
 2       M = N/2
 3       IF(J-M) 5,5,4
 4       J = J - M
         M = M/2
         IF(M-2) 5,3,3
 5       J = J + M
         MMAX = 2
 6       IF(MMAX - N) 7,10,10
 7       ISTEP = 2*MMAX
         THETA = 6.28318530717959*SIGN/FLOAT(MMAX)
         SINTH = SIN(THETA/2.0)
         WSTPR = -2.0*SINTH*SINTH
         WSTPI = SIN(THETA)
         WR = 1.0
         WI = 0.0
         DO 9 M = 1,MMAX,2
            DO 8 I = M,N,ISTEP
               J = I + MMAX
               TEMPR = WR*DATA(J) - WI*DATA(J+1)
               TEMPI = WR*DATA(J+1) + WI*DATA(J)
               DATA(J) = DATA(I) - TEMPR
               DATA(J+1) = DATA(I+1) - TEMPI
               DATA(I) = DATA(I) + TEMPR
 8             DATA(I+1) = DATA(I+1) + TEMPI
               TEMPR = WR
               WR = WR*WSTPR - WI*WSTPI + WR
 9             WI = WI*WSTPR + TEMPR*WSTPI + WI
               MMAX = ISTEP
               GO TO 6
 10            RETURN
               END
      SUBROUTINE INVRFT(N,B,DFREQ,SIGN)
C
C     INVRFT PERFORMS THE INVERSE TRANSFORM OF A COMPLEX ARRAY B OF
C     DIMENSION N ASSUMING THE TRANSFORM TO BE REAL.
C     THE DIMESNION OF B IS INCREASED - IF NECESSARY - TO THE NEXT
C     POWER OF 2 PLUS 1 BY PAD.
C     PAD ALSO NORMALIZES B BY MULTIPLYING ITS NONZERO ELEMENTS BY DFREQ
C     THEN B IS TRANSFORMED BY FASTFT AND IS RETURNED AS A REAL ARRAY
C     OF DIMENSION N = 2*N - 1 -- THUS REDEFINING N AS THE DIMENSION
C     SIGN IS GENERALLY -1 FOR THIS INVERSE TRANSFORM
C
      DIMENSION B(*)
C     V    VIRTUAL B(*)
      DIMENSION W1(2),W2(2),CJ(2),DJ(2)
      B(2) = B(2*N-1)
      N = 2*(N-1)
      CALL PAD(N,B,DFREQ)
      NM1=N-1
      NN = 2**(NM1)
      W3 = 3.141592*SIGN/FLOAT(NN)
      NN = NN + 2
      BR = B(1) + B(2)
      BI = B(1) - B(2)
      B(1) = BR
      B(2) = BI
      DO 10 JI = 4,NN,2
         KI = 2*NN - JI
         W1(1) = B(KI-1)
         W1(2) =-B(KI)
         CJ(1) = B(JI-1) + W1(1)
         CJ(2) = B(JI) + W1(2)
         DJ(1) = B(JI-1) - W1(1)
         DJ(2) = B(JI) - W1(2)
         ARG = FLOAT(JI/2 - 1)*W3 + 1.570796
         W2(1) = DJ(1)*COS(ARG) - DJ(2)*SIN(ARG)
         W2(2) = DJ(1)*SIN(ARG) + DJ(2)*COS(ARG)
         B(KI-1) = CJ(1) - W2(1)
         B(KI) = W2(2) - CJ(2)
         B(JI-1) = CJ(1) + W2(1)
         B(JI) = W2(2) + CJ(2)
 10   CONTINUE
      CALL FASTFT(NM1,B,SIGN)
      N = 2**N
      RETURN
      END


      function lenc(string)
C     +
C     function lenc(string)
C
C     Returns length of character variable STRING excluding right-hand
C     most blanks or nulls
C     -
      character*(*) string
      length = len(string)      ! total length
      if (length .eq. 0) then
         lenc = 0
         return
      end if
      if(ichar(string(length:length)).eq.0)string(length:length) = ' '
      do j=length,1,-1
         lenc = j
         if (string(j:j).ne.' ' .and. ichar(string(j:j)).ne.0) return
      end do
      lenc = 0
      return
      end
C     +
