!     +
      SUBROUTINE INTERP_f(n_in,x_in,y_in,n_out,dx_out,y_out,a,b,c)
!
!     Interpolates a real*4 array y_in of dimension n_in
!     x_in is the array of the independent variable, also of dimension n_in.
!     dx_out is the (input) new digitizing interval.  n_out is the output
!     dimension,of the output array y_out.  n_out and y_out are calculated.
!     -
      REAL*4 x_in(*), y_in(*), y_out(*),a(*), b(*),c(*)
!
      xtotal = x_in(n_in) - x_in(1)
      dx_out = xtotal/(n_out-1)
      CALL SPSPLN(N_in,X_in,Y_in,A,B,C)
      DO J=1,N_out
         y_out(J) = SPEVAL(N_in,X_in(1)+DX_out*(J-1),X_in,Y_in,A,B,C)
      enddo
      RETURN
      END
!     +
!     FUNCTION SPEVAL(N,U,X,Y,B,C,D)
!
!     EVALUATES THE CUBIC SPLINE FUNCTION
!
!     SPEVAL = Y(I) + B(I)*(U-X(I)) + C(I)*(U-X(I)**2 + D(I)*(U-X(I))**3
!
!     WHERE X(I).LT.U.LT.X(I+1),  USING HORNER'S RULE
!
!     IF U.LT.X(1) THEN I=1 IS USED
!     IF U.GE.X(N) THEN I=N IS USED
!
!     INPUT
!     N=NUMBER OF DATA POINTS
!     U=ABSCISSA AT WHICH SPLINE IS TO BE EVALUATED
!     X,Y ARE ARRAYS OF DATA ABSCISSAS AND ORDINATES
!     B,C,D ARE ARRAYS OF SPLINE COEFFICIENTS COMPUTED BY SPLINE
!
!     IF U IS NOT IN THE SAME INTERVAL AS THE PREVIOUS CALL, THEN A
!     BINARY SEARCH IS PERFORMED TO DETERMINE PROPER INTERVAL
!     This is the single-precision version of the program
!     Reference:  Forsythe et al., 1977 Ch. 4
!     -
      FUNCTION SPEVAL(N,U,X,Y,B,C,D)
      DIMENSION X(N),Y(N),B(N),C(N),D(N)
      DATA I/1/
      IF(I.GE.N) I=1
      IF(U.LT.X(I)) GO TO 10
      IF(U.LT.X(I+1)) GO TO 30
 10   I=1
      J=N+1
 20   K=(I+J)/2
      IF(U.LT.X(K)) J=K
      IF(U.GE.X(K)) I=K
      IF(J.GT.I+1) GO TO 20
 30   DX=U-X(I)
      SPEVAL=Y(I)+DX*(B(I)+DX*(C(I)+DX*D(I)))
      RETURN
      END
!     +
!     SUBROUTINE SPSPLN (N,X,Y,B,C,D)
!
!     THE COEFFICIENTS B(I), C(I) AND D(I) , I=1,2..N ARE COMPUTED
!     FOR A CUBIC INTERPOLATING SPLINE
!
!     S(X) = X(I) + B(I)*(X-X(I)) + C(I)*(X-X(I))**2 + D(I)*(X-X(I)**3
!
!     FOR X(I) .LE. X .LE. X(I+1)
!
!     INPUT
!
!     N = THE NUMBER OF DATA POINTS  (N.GE.2)
!     X = ABSCISSAS OF DATA POINTS IN STRICTLY INCREASING ORDER
!     Y = ORDINATES OF DATA POINTS
!
!     OUTPUT
!
!     B, C, D ARE THE ARRAYS OF THE SPLINE COEFFICIENTS
!
!     THE FUNCTION SUBPROGRAM SPEVAL CAN BE USED TO EVALUATE THE SPLINE
!     This is the single precision version
!     Reference: Forsythe et al., 1977 Ch. 4
!     -
      SUBROUTINE SPSPLN (N,X,Y,B,C,D)
      DIMENSION X(N),Y(N),B(N),C(N),D(N)
      NM1=N-1
      IF(N.LT.2) RETURN
      IF(N.LT.3) GO TO 50
      D(1)=X(2)-X(1)
      C(2)=(Y(2)-Y(1))/D(1)
      DO 10 I=2,NM1
         D(I)=X(I+1)-X(I)
         B(I)=2.0*(D(I-1)+D(I))
         C(I+1)=(Y(I+1)-Y(I))/D(I)
         C(I)=C(I+1)-C(I)
 10   CONTINUE
      B(1)=-D(1)
      B(N)=-D(N-1)
      C(1)=0.0D0
      C(N)=0.0D0
      IF(N.EQ.3) GO TO 15
      C(1)=C(3)/(X(4)-X(2))-C(2)/(X(3)-X(1))
      C(N)=C(N-1)/(X(N)-X(N-2))-C(N-2)/(X(N-1)-X(N-3))
      C(1)=C(1)*D(1)**2/(X(4)-X(1))
      C(N)=-C(N)*D(N-1)**2/(X(N)-X(N-3))
 15   DO 20 I=2,N
         T=D(I-1)/B(I-1)
         B(I)=B(I)-T*D(I-1)
         C(I)=C(I)-T*C(I-1)
 20   CONTINUE
      C(N)=C(N)/B(N)
      DO 30 IB=1,NM1
         I=N-IB
         C(I)=(C(I)-D(I)*C(I+1))/B(I)
 30   CONTINUE
      B(N)=(Y(N)-Y(NM1))/D(NM1)+D(NM1)*(C(NM1)+2.0*C(N))
      DO 40 I=1,NM1
         B(I)=(Y(I+1)-Y(I))/D(I)-D(I)*(C(I+1)+2.0*C(I))
         D(I)=(C(I+1)-C(I))/D(I)
         C(I)=3.0*C(I)
 40   CONTINUE
      C(N)=3.0*C(N)
      D(N)=D(N-1)
      RETURN
!
 50   B(1)=(Y(2)-Y(1))/(X(2)-X(1))
      C(1)=0.0D0
      D(1)=0.0D0
      B(2)=B(1)
      C(2)=0.0D0
      D(2)=0.0D0
      RETURN
      END
!
      function lenc(string)
!     +
!     function lenc(string)
!
!     Returns length of character variable STRING excluding right-hand-
!     most blanks or nulls
!     -
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
