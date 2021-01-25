c - Program geopot07

c - ========================================
c - geopot97.v0.4g.f   Version 97.0.4g, 04/28/2010
c - National Geodetic Survey
c - A bug in subroutine fixc20 was corrected on 04/28/2010 at NGS.

c - ========================================
c - Version 2007.0.1a, 02/23/2007
C - To compile, use:   f90 -e geopot07.f or f90 -e -O5 geopot07.f
C - The second option is available on SUN Solaris work stations and
C - gives optimized performance faster than the first option.
C - The option -e should be used to support the f90 source form (e.g.,
C - lines that exceed column 72).

C - At the end of a run, a log file called "log_file", which contains
C - all answers is created for use in future runs.
C - Use   a.out < log_file     to avoid questions and answers in any
C - future runs. This file is over-written in each run. If you like
C - to keep an old log_file, you need to rename it.

C - The user needs to fix the part of the program which reads the
C - harmonic coefficients. As it is, the program is adapted to read
C - the PGM07 coefficients which were distributed on the NGA web
C - site for testing their harmonic synthesis program. These test
C - coefficients start at degree zero and not 2 (containing zeros
C - for all coefficients below degree 2).

C - Modified to work up to Nmax = 2190 for latitudes up to the North Pole
C - at the National Geodetic Survey on 02/23/2007.

c - ========================================
c - Version 97.0.4e, 4/03/2006
c - by Dru Smith, National Geodetic Survey
c - original programmer of GPTDR : Goad/Tscherning

c - All versions prior to 0.4e contain known errors

c - ========================================
c - Changes: 0.4e (4/03/2006):
c - Fixed an error in Function 11 where ".b" output
c - was being erroneously created (no header was
c - being written)

c - ========================================
c - Changes: 0.4d (4/14/2004):
c - Fixed a typo in "function 6" where the flattening
c - was being computed incorrectly if the user
c - input a & b values.

c - ========================================
c - Changes: 0.4c:
c - Fixed the error I inadvertantly introduced in version 0.4a
c - in the subroutine "norm".

c - ========================================
c - Changes: 0.4b:
c - Fixed a "goto 34" line to "goto 134" in Function # 3

c - ========================================
c - Changes: 0.4a:
c - Modified subroutine "norm" for the computation of the
c - J2,J4, etc coefficients to a simpler form.  The slight
c - change in code yields a very slight change in the
c - numbers (around the 10th decimal place)

c - ========================================
c - Changes: 0.4
c     Removed a strict definition of gravity anomaly, and instead
c     gave the user the choice:
c     Whenever gravity anomalies are to be calculated, give the
c        user 2 options:
c
c      Spherical approximation (compatable with Stokes' eqn and
c      spherical harmonics)
c        1) delta_g = -dT/dr - 2*T/r - delta_W*2/r
c   Eval Pt:  P-Q        P       P P     P-Q     P
c
c      Direct evaluation to 1st order terms (incompatable with
c      Stokes' eqn and spherical harmonics)
c   2) delta_g =-dT/dh +T*(dgamma/dh)/gamma +delta_W*(dgamma/dh)/gamma
cEvalPt: P-Q       P    P        Q      Q      P-Q          Q     Q
c   The "Eval Pt" is the evaluation point, where P is the point
c   on the geoid and Q the point on the ellipsoid

c - ========================================
c - Changes: 0.3:
c   Removed the following errors found while debugging:
c     1) sin(lat) calls were replaced with sin(lat*d2r) in general
c        where they occurred (dtdr computations mostly)
c     2) In a type (9) run, I was constantly re-setting the
c        evaluation height (h) to zero, rather than just on the
c        first time through.
c     3) In a type (8) run, the gravity anomaly was calculated
c     4) Changed a typo in type (8) run -- units of N changed
c        from saying "m**2/s**2" to "m"
c     5) Changed the gravity anomaly calculation BACK to that
c        of geopot96 (that is, the 2nd term is now -2T/r rather
c        than (dgamma/dh)/gamma ).  This allows for a consistancy
c        between spherical harmonic N and delta-g.
c     6) Added the degree zero terms to gravity anomalies for
c        the non-equivalence of potential (W0<>U0).  (The degree
c        zero term due to non-equivalence of the masses is already
c        accounted for in the degree zero term of disturbing potential).
c     7) In a type #11 run, the total potential call was fixed
c        to use "fomega" rather than the erroneous use of "domega"
c
c   Added the following:
c     1) Added the degree zero component of gravity anomalies that
c        is induced by W0.ne.U0 (2nd half of equation 2-186 in H/M)
c     2) Added a gravity/normal gravity printout to a type #7 run.

c - ========================================
c - Changes: 0.2b co-ordinates the non-variable nature of the
c - call to INITAL and the subroutine header itself.  Also removed
c - references to non-used variables in INITAL.

c - ========================================
c - Changes: 0.2a removes variable "OM2" from subroutine
c - GPTDR.  This variable was a left-over from the days when
c - GPTDR used "omega" and did the centrifugal potential part.
c - It *shouldn't* have been a problem, since OM2 is undefined,
c - and thus *probably* set to zero.  But safest just to remove
c - the code, right?

c - ========================================
c - Changes: 0.2 adds the option of inputting an
c - (ellipsoidal or orthometric) height DTED and getting out surface
c - gravity values on the DTED grid, at the Earth's surface
c - Also, the subroutine "getgeom" was added to remove a redundancy
c - between numerous functions.  Also, subroutine getmsl was added
c - to allow one to modify the DTED based on it's divergence from
c - best global mean sea level.  The bias for NAVD 88 is hard-coded
c - in the parameter statement

c - ========================================
c - Changes: 0.1a added a warning about the mean tide system
c - being incompatable with Laplace's equation.

ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c   Program to use geopotential coefficients for point and      c
c   grid calculation of various gravimetric quantities          c
c                                                               c
c   Dru Smith, 1/13/97                                          c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c   History (geopot97):                                         c
c   -- Program started by Tscherning and Goad to use Clenshaw   c
c      summation for potential, undulation, anomaly, and        c
c      gravity gradient computation.                            c
c   -- Modified by Dennis Milbert into grd360 and geo360        c
c      (grid and point computations)                            c
c   -- Modified by Dru Smith, 1996, into geopot96, a point or   c
c      grid method, for either geoid or ellipsoid quantities.   c
c      Added upward derivatives of height anomaly.              c
c      Added ability to use any reference ellipsoid.            c
c   -- Modified by Dru Smith, 1997, into geopot97 to properly   c
c      account for tides, and clean up the code which was       c
c      cluttered and clunky.                                    c
c   -- geopot97 work begun 1/13/97                              c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c FUNCTIONS PERFORMED (Assuming you've input the coefficients):
c (geopot97)
c
c  (1) Given a spherical lat/lon, and a value of GRAVITY potential,
c      find the radial distance to the surface of that potential.
c  (2) Given a geocentric ellipsoid and an ellipsoidal lat/lon, and
c      a value of GRAVITY potential, find the ellipsoidal height to the
c      surface of that potential.
c  (3) Same as (1), but do the computations for grid points
c  (4) Same as (2), but do the computations for grid points
c
c  (5) Given a spherical lat/lon/distance, compute the potential
c      and gravity at that point.
c  (6) Given a geocentric ellipsoid, and an ellipsoidal lat/lon/height,
c      compute the potential and gravity at that point.
c
c  (7) Given an ellipsoidal normal field, and a lat/lon/height,
c      compute potential, gravity, disturbing potential, gravity
c      anomaly, height anomaly, gravity disturbance, deflection
c      of the vertical, and 3-D gravity gradients at that point.
c
c  (8) Given an ellipsoidal normal field, and either a best fitting
c      geoid or gravity potential value on the geoid, compute the
c      geoid undulation and gravity anomaly at that point.
c
c  (9) Given an ellipsoidal normal field, and lat/lon boundaries,
c      compute the geoid undulation and/or gravity anomaly inside
c      the masses (theoretically incorrect / mathematically consistant)
c
c (10) Given an ellipsoidal normal field, and lat/lon boundaries,
c      compute the 0th, 1st and 2nd order derivatives of the
c      height anomaly on the ellipsoid.
c
c - Added for version 0.2:
c (11) Given a DTED, compute the surface
c      gravity value from the spherical harmonic model, on a grid
c      at the Earth's surface
c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c  INPUT (General) (geopot97):
c    1) The name of a set of fully normalized spherical harmonic
c       coefficients of the Earth's external GRAVITATIONAL potential.
c       (The name leads to a hard-coded identification of the
c       proper scale factors, a and GM, of the coefficient set)
c
c    2) The coefficient set named in (1)
c
c    3) The tide system (mean, zero or non) to which the
c       coefficient set refers (if known).
c
c    4) A standard value of omega (often taken as 7292115d-11 rad/sec)
c
c  INPUT (Functions 2,4,6):
c    1) Geometric parameters of an ellipsoid (a/f or a/b.
c       *NOT* a and J2, as the conversion to "f" requires
c       GM and omega, which are non-geometric).
c
c  INPUT (Functions 7,8,9,10):
c    1) Any four parameters defining the ellipsoidal normal field
c       of the Earth's NORMAL external GRAVITY potential.
c    2) The tide system of the normal field, if known.
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c IMPORTANT RULES (geopot97):
c  1)  Never use:  N(p,q) = T(p)/gamma(q)
c     Always use:  N(p,q) = T(p)/gamma(q) - [W(p) - U(q)]/gamma(q)
c
c  2) The Earth spins; The ellipsoid spins; Their spins cancel
c
c  3) Before using any approximation, ask yourself this: Would I
c     bet my house on the accuracy of that approximation?
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c - Assumptions(geopot96 and geopot97):
c -   1) The program will do calculations INSIDE THE MASSES, but
c -      the results are NOT guaranteed.  Only values outside of
c -      the masses have any grounds in theoretical correctness.
c -   2) The TOTAL CENTRIFUGAL and NORMAL CENTRIFUGAL potentials
c -      are identical, thus the "DISTURBING" potential will have
c -      no dependence on OMEGA (being both the TOTAL SPIN and NORMAL SPIN
c -      rate of the Earth).
c -   3) The summation of the degree 1 through NMAX terms (and
c -      the 1st and 2nd order derivatives of those sums (wrt the
c -      lat, lon and height) are calculated through a function
c -      called "GPTDR", written by Tscherning/Goad, which uses
c -      Clenshaw summations and either un-normalized or
c -      quasi-normalized coefficients (*NOT* FULLY Normalized,
c -      though that is what is assumed to come INTO this
c -      program -- they will be converted to QUASI-normalized
c -      coefficients later).
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c - Generalizations(geopot96 and geopot97):
c -   1) It is generalized that the "GM" and "a" values associated
c -      with the TOTAL field expansion will not necessarily be
c -      the same as the "GM" and "a" values associated with the
c -      NORMAL field.  As such, a DISTURBING potential will have
c -      a NON-ZERO value for the degree zero term.  Subroutine
c -      GPTDR was not set up for such a contingency, and was
c -      therefore modified to allow for such.
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc

      implicit real*8 (a-h,o-z)

      real*8      j2norm,j2bf,glamn,glomn,dla,dlo
      integer*4   nla,nlo,ikind,date_time(8)

      logical     lfile,lscreen            !Functions 3 and 4:
      logical     lgeoid,lganom            !Function 6:
      logical     lt0,lt1,lt2              !Function 7:
      logical     force,iform              !To force the summation in gptdr:
      logical     unnormalize              !True if unnormalize SH coeff
      character   yesno*1,tidcoef*4,tidsum*4,tidbf*4,name*80,fname*80
      character   string*80,date*8,time*10,zone*5

      parameter (max         = 2190,
     &           njmax       = 10,  !set for comparison with the NGA's harmonic synthesis program
     &           maxdum      = 50000,
     &           b88         = -0.314,
     &           Capital_R   = 6371000.d0)

c  max = highest possible degree/order of the coefficients to use in calculations
c  (max+1)*(max+1)-1 = number of coefficients stored, knowing that all even zonal
C  S terms are zero (and thus not stored), and the degree zero term of C is stored
C  in the 0th place.
c  njmax => njmax*2 = highest even zonal computed for the normal field.
C  Thus, njmax = 10 means going up to J(20)
c  b88 = bias in NAVD88 (needed for type 11 run.)  The number is how far the H(88)=0
C  level is above the H(true)=0 level. Thus a negative b88 value puts it below.

      real*8      root((max+1)*2)
      real*8      dmp1mp1(0:max)
      real*8      smlp1(0:max+2),cmlp1(0:max+2)
      real*8      c(0:(max+1)*(max+1)-1)
      real*8      su(10*(max+1))
      real*8      typot1(3),tionpot1(3),cenpot1(3)
      real*8      typot2(3,3),tionpot2(3,3),cenpot2(3,3)
      real*8      jnorm(njmax)
      real*4      raddist(maxdum)                   !Function 3:
      real*4      outht(maxdum)                     !Function 4:
      real*4      geoid(maxdum),ganom(maxdum)       !Function 6:
      real*4      t0(maxdum),t1(maxdum),t2(maxdum)  !Function 7:
      real*4      elev(maxdum),outg(maxdum)         !Function 11:


c - tionpot1(3) = g1(3) in GPTDR
c - tionpot2(3,3) = g2(3,3) in GPTDR

C  Define some geometric constants

      common/block1/pi,d2r
      pi = 2.d0*dasin(1.d0) ; d2r = pi/180.d0

C  Set a logical parameter responsible for derivatives

      force = .false.

c - Before doing ANYTHING, initialze variables that help us keep
c   track of our trips to GPTDR

      call inital

C  open log file

      open (101,file='log_file')

c - Establish which coefficients, and defining parameters

      write(6,*) ' '
c     write(6,*) ' Program geopot97, version 0.4d, 4/14/2004'
c     write(6,*) ' Program geopot07, version 0.1d, 2/23/2007'
      write(6,*) ' Program geopot07, version 0.4g, 4/28/2009'
      write(6,*) ' '

      write(6,*) ' '
      write(6,*) ' '
      write(6,*) ' '
      write(6,*) '   ***************** WARNING ******************* '
      write(6,*) '   Before proceeding, a word of warning is needed'
      write(6,*) '   regarding the mean tide system.  Changing between'
      write(6,*) '   tide systems is often performed by modifying '
      write(6,*) '   the C(2,0) harmonic coefficient and re-inserting'
      write(6,*) '   it into the spherical harmonic expansion. '
      write(6,*) '   The *MEAN* tide system, however, is incompatable'
      write(6,*)'    with Laplaces equation, and therefore can not '
      write(6,*) '   be represented by the simple spherical harmonic'
      write(6,*) '   expansion of the potential.  No current fix of '
      write(6,*) '   this problem exists.  Take heed when using any'
      write(6,*) '   data given in the mean tide system.  Thank you.'
      write(6,*) '   ***************** WARNING ******************* '
      write(6,*) ' '
      write(6,*) ' '
      write(6,*) ' '

    3 write(6,1)
    1 format(1x,'Name of the coefficient set? :',$)
      read(5,'(a)')name
      write (*,'(a)')name
      string = ' '
      write (string,'(a80)') name
      call log (string)
      if(name.eq.'egm96' .or. name.eq.'EGM96')then
        gmcoef = 3.986004415d14 ; acoef  = 6378136.3d0 ; nmaxcoef = 360 ; tidcoef  = 'non-'
      elseif(name.eq.'osu91a' .or. name.eq.'OSU91A')then
        gmcoef = 3.98600436d14 ; acoef  = 6378137.d0 ; nmaxcoef = 360 ; tidcoef = '????'
      else
        write(6,2)
    2   format(1x,'  That coefficient set is unknown. What',
     *         'to do?',/,1x,'1 - Re-enter name',/,
     *         '2 - Proceed and prompt for defining parameters',/,
     *         '3 - Abort program ',/,$)
        read(5,*)ians
        string = ' '
        write (string,*) ians
        call log (string)
        if(ians.le.0 .or. ians.ge.3)stop
        if(ians.eq.1) goto 3
        if(ians.eq.2)then
          write(6,4)
    4     format(1x,'GM of the coefficients? ',$)
          read(5,*)gmcoef
          string = ' '
          write (string,*) gmcoef
          call log (string)
          write(6,5)
    5     format(1x,'a  of the coefficients? ',$)
          read(5,*)acoef
          string = ' '
          write (string,*) acoef
          call log (string)
          write(6,6)
    6     format(1x,'maximum degree of the coefficients? ',$)
          read(5,*)nmaxcoef
          string = ' '
          write (string,*) nmaxcoef
          call log (string)
          write(6,7)
    7     format(1x,'tidal system of the coefficients?',/,
     *       ' 1 - mean',/,'2 - zero',/,'3 - non-tidal',/,$)
          read(5,*)ians
          string = ' '
          write (string,*) ians
          call log (string)
          if(ians.le.0 .or. ians.ge.4)stop
          if(ians.eq.1)tidcoef = 'mean'
          if(ians.eq.2)tidcoef = 'zero'
          if(ians.eq.3)tidcoef = 'non-'
        endif
      endif

c - Establish format and name of file and open it

      write(6,23)
   23 format(1x,'Is the coefficient file formatted? :',$)
      read(5,'(a)')yesno
      string = ' '
      write (string,*) yesno
      call log (string)
      iform = .true.
      if(yesno.eq.'n' .or. yesno.eq.'N')iform = .false.
      write(6,24)
   24 format(1x,'Name of the coefficient file? :',/)
      read(5,'(a)')fname
      string = ' '
      string = fname
      call log (string)
      if(iform)then
        open(1,file=fname,form='formatted',status='old')
      else
        open(1,file=fname,form='unformatted',status='old')
      endif

c - Establish tide system of incoming coefficients

      write(6,10)tidcoef
   10 format(/1x,'Your geopotential model is assumed to be in the ',
     *          'system of: ',a4,'tide',/,' Change this (y/n)? ',$)
      read(5,'(a)')yesno
      string = ' '
      write (string,*) yesno
      call log (string)
      if(yesno.eq.'y' .or. yesno.eq.'Y')then
        write(6,11)
   11   format(1x,'New tide system: ',/,'1 - mean',/,'2 - zero',/,
     *  '3 - non-tidal',/,$)
        read(5,*)ians
        string = ' '
        write (string,*) ians
        call log (string)
        if(ians.le.0 .or. ians.ge.4)stop
        if(ians.eq.1)tidcoef = 'mean'
        if(ians.eq.2)tidcoef = 'zero'
        if(ians.eq.3)tidcoef = 'non-'
      endif

c - Print out defining parameters

      write(6,13)
   13 format(/,1x,60('*'),/)
      write(6,8)
    8 format(1x,'Your coeffiecients will be read in with the'
     *' following defining parameters:')
      write(6,112)gmcoef
  112 format(1x,' GM              = ',d19.13)
      write(6,113)acoef
  113 format(1x,' a               = ',f15.7)
      write(6,114)nmaxcoef
  114 format(1x,' maximum degree  = ',i4)
      write(6,12)tidcoef
   12 format(1x,' tidal system    = ',a4,' tidal')
      write(6,36)
   36 format(1x,' normalization   = fully-normalized')

c - Establish Omega

      write(6,13) ; write(6,14)

c - fomega = "FULL" omega (that is the true rate)
c - domega = "DISTURBED" omega (that is, zero)

      fomega = 7292115d-11 ; domega = 0.d0
   14 format(1x,'Establish spin rate of Earth (which must'/
     *       1x,'also be the same rate as any reference '/
     *       1x,'ellipsoid you use, for this program) :'/
     *       1x,'Omega = 7292115d-11 rad/sec.  Ok (y/n)? ',$)
      read(5,'(a)')yesno
      string = ' '
      write (string,*) yesno
      call log (string)
      if(yesno.eq.'n' .or. yesno.eq.'N')then
        write(6,15)
   15   format(1x,'New value of Omega (rad/sec) : ',$)
        read(5,*) fomega
        string = ' '
        write (string,*) fomega
        call log (string)
      endif

c - Pick which function we'll be doing:

      write(6,13) ; write(6,16) ; write(6,120) ; write(6,17)
   16 format(1x,' Perform which of the following functions?'//
     *' 1 = Given a spherical lat/lon and a value of GRAVITY'/
     *'     POTENTIAL, find the radial distance to the surface'/
     *'     of that potential.'//
     *' 2 = Given a geometric ellipsoid, an ellipsoidal '/
     *'     latitude and longitude and a value of GRAVITY'/
     *'     POTENTIAL, find the ellipsoidal height to the '/
     *'     surface of that potential.'//
     *' 3 = Same as function #1, but done on a lat/lon grid'//
     *' 4 = Same as function #2, but done on a lat/lon grid'//
     *' 5 = Given a spherical lat/lon/distance, compute the'/
     *'     GRAVITY POTENTIAL, and GRAVITATIONAL POTENTIAL at'/
     *'     that location.'//
     *' 6 = Given a geometric ellipsoid, and an ellipsoidal'/
     *'     latitude, longitude and height, find the GRAVITY,'/
     *'     GRAVITY POTENTIAL and GRAVITATIONAL POTENTIAL at'/
     *'     that location.'/)
  120 format(
     *' 7 = Given and ellipsoidal normal gravity field, and an'/
     *'     ellipsoidal latitude, longitude, and height, compute'/
     *'     the gravity potential, gravitational potential, and'/
     *'     gravity *AND ALSO* the gravity anomaly, height anomaly,'/
     *'     gravity disturbance, deflections of the vertical, and'/
     *'     3-D gradients of gravity at that location.'//
     *' 8 = Given an ellipsoidal normal gravity field, and an'/
     *'     ellipsoidal latitude, longitude, and a defined'/
     *'     geoid (either through a best fit ellipsoid or a gravity'/
     *'     potential value), give the geoid undulation and '/
     *'     gravity anomaly.'/)
   17 format(
     *' 9 = Given an ellipsoidal normal gravity field, and'/
     *'     boundaries of ellipsoidal latitude and longitude,'/
     *'     compute the geoid undulations and/or gravity anomalies'/
     *'     on a grid, in the masses.'//
     *' 10= Given an ellipsoidal normal gravity field, and'/
     *'     boundaries of ellipsoidal latitude and longitude,'/
     *'     compute the height anomaly, as well as the 1st and'/
     *'     2nd order upward derivatives of the height anomaly,'/
     *'     all on the ellipsoid.'//
     *' 11= Given a DTED (H or h), compute a grid of gravity values'/
     *'     at the Surface of the Earth'/)

      read(5,*)ifun
      string = ' '
      write (string,*) ifun
      call log (string)
      if(ifun.lt.1 .or. ifun.gt.11)stop

c - Determine maximum degree we'll be using

      write(6,18)nmaxcoef
   18 format(/1x,'The maximum degree in the coefficient set is:',i4,/
     *       1x,'What is the maximum degree you want used? ',$)
      read(5,*)nmax
      write (*,*) nmax
      string = ' '
      write (string,*) nmax
      call log (string)
      write (*,*) string

c - Read in fully-normalized coefficients (assume 2,0 is first one)

c - C(0,0) = c(0)
c - S(0,0) = not stored

c - C(1,0) = c(1)
c - S(1,0) = not stored
c - C(1,1) = c(2)
c - S(1,1) = c(3)

c - C(2,0) = c(4)   <- this is why isto starts at isto=4
c - S(2,0) = not stored
c - C(2,1) = c(5)
c - S(2,1) = c(6)
c - C(2,2) = c(7)
c - S(2,2) = c(8)  etc....

      c(0)   = 1.d0 ! <- may change if we work with disturbing potentials
      c(1:3) = 0.d0

      isto = 4

c - Formatted

      if(iform)then
        do n=0,nmax
          do m = 0,n
            read (1,*) nnn,mmm,ccc,sss
            if (nnn < 2) cycle
            c(isto) = ccc
            if(m.ne.0)then
              isto = isto + 1
              c(isto) = sss
            endif
            isto = isto + 1
            if(nnn.ne.n .or. mmm.ne.m)stop
          enddo
        enddo

c - Unformatted

      else
        do n=2,nmax
          do m = 0,n
            read (1) ccc,sss
            c(isto) = ccc
            if(m.ne.0)then
              isto = isto + 1
              c(isto) = sss
            endif
            isto = isto + 1

          enddo
        enddo
      endif

c - Convert Fully Normalized Coefficients into
c - quasi-normalized coefficients, for efficiency in GPTDR
c - and also compute a table of square roots for quick
c - look-up.


      call setcm (nmax,root,c,su,dmp1mp1,max)

c - Find the tide system of the output

      call gettid(tidsum,tidcoef)

C   Start counting synthesis time

      call DATE_AND_TIME (date,time,zone,date_time)
      time0 = sum (date_time(5:8)*(/3600.d0,60.d0,1.d0,0.001d0/))
























ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c - FUNCTION # 7
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
      elseif(ifun.eq.7) then

c - NOTE: In THIS particular function, there is no need to
c -       define the geoid through "bestfit", as everything
c -       will be relative to the assumption that Wsurf=Utelluroid,
c -       since a dW correction at altitude is both complicated
c -       and undeveloped.

c       call bestfit(gmcoef,fomega,abf,j2bf,finvbf,w0,tidbf,tidcoef)

c - get the 4 parameters and the tide system of the normal field,
c - BUT return the normal field in the tidsum tide system

        call getref (anorm,bnorm,finvnorm,j2norm,gmnorm,fomega,tidsum)

c - Now, anorm,bnorm,finvnorm,j2norm,gmnorm,fomega are in the
c - tidsum system and need no further tinkering :)
c - calculate normal values from the normal field

        call norm (anorm,j2norm,gmnorm,fomega,bnorm,jnorm,njmax,u0norm,gammae,gammap)
        fnorm  = (anorm-bnorm)/anorm ; e2norm = (2.d0 - fnorm)*fnorm

c - The "jnorm" values are NOT fully-normalized here.  They
c - will be fully normalized in subroutine dist.

c - Now, correct the C(2,0) coefficient for tides, if necessary

        if (tidsum /= tidcoef) call fixc20 (tidcoef,tidsum,c(4),gmcoef,acoef)

c - At this point, everything is in the "tidsum" tide system.

c - get the location of the computation point

   91   write(6,92)
   92   format(/1x,'Input ellipsoidal latitude (dec. deg.): ',$)
        read(5,*)geodlat
        string = ' '
        write (string,*) geodlat
        call log (string)
        write(6,93)
   93   format(1x,'Input ellipsoidal longitude (dec. deg.): ',$)
        read(5,*)geodlon
        string = ' '
        write (string,*) geodlon
        call log (string)
        write(6,94)
   94   format(1x,'Input ellipsoidal height (meters): ',$)
        read(5,*)h
        string = ' '
        write (string,*) h
        call log (string)

c - calculate normal gravity at the point
c - as of 1/29/97, the following subroutines are either
c - corrupt or (more likely) the actual formulation
c - is bad.  Bad meaning disagreements to 0.5 mgals or so
c - (function gam(*) is ok, and gamh(*) replaces the need
c - for the other two.

        gamma  = gam(geodlat,anorm,bnorm,gammae,gammap)
        gammah = gamh(anorm,bnorm,gmnorm,fomega,h,geodlat,gamma)

c - Convert to spherical coordinates

        call ell2sph(geodlat,geodlon,h,anorm,finvnorm,xlat,xlon,r)

c------------------------
c - STEP #1:

        isw = 1
        call pot(xlat,xlon,r,fomega,tionpot,tionpot1,tionpot2,
     *   cenpot,cenpot1,cenpot2,typot,typot1,typot2,isw,nmax,
     *   root,dmp1mp1,smlp1,cmlp1,c,su,max,gmcoef,acoef,force)

c - Store the 3 potentials for printing

        xp1 = tionpot ; xp2 = typot ; xp3 = cenpot

c - Store the 3 derivs for computing gamma from disturbings later

        xg1 = typot1(1) ; xg2 = typot1(2) ; xg3 = typot1(3)
        gravityh = dsqrt(xg1**2 + xg2**2 + xg3**2)

c------------------------------
c - STEP #2:
c - Compute the gravity/gravitational/centrifugal disturbing potentials

c - Calculate the disturbing coefficients, set up such
c - that they work with gmcoef and acoef (which means
c - you may need to scale the j2 coefficients from
c - working with gmnorm and anorm)

        call dist(c,max,nmax,gmcoef,acoef,jnorm,njmax,gmnorm,anorm)

c - the "c" vector now contains the fully normalized disturbing coefficients

c - Do the summation. All "*pot" values are DISTURBING potential values!!!

        isw = 2

c - Don't have to "force", as we've changed the order,
c - (that is, isw), and thus gptdr has a built in re-summation

        call pot (xlat,xlon,r,domega,tionpot,tionpot1,tionpot2,
     *   cenpot,cenpot1,cenpot2,typot,typot1,typot2,isw,nmax,
     *   root,dmp1mp1,smlp1,cmlp1,c,su,max,gmcoef,acoef,force)

c - STEP #2a:
c - Compute Normal values by just subtracting the disturbing from total

        xnp1 = xp1 - tionpot ; xnp2 = xp2 - typot ; xnp3 = xp3 - cenpot

c - Compute a gamma value to check against the closed formula

        write (*,*) typot1(1)*1d5,typot1(2)*1d5,typot1(3)*1d5

        gammachk = dsqrt( (xg1-typot1(1))**2+(xg2-typot1(2))**2 + (xg3-typot1(3))**2 )

        dd = (gammah - gammachk)*1d5            !This difference is in m/sec**2, convert to mGal
        if(dabs(dd).gt.0.01)then
          write(6,117)
  117     format(1x,'***WARNING*** The closed formula for gamma',/,
     *              'disagrees with that implied by summing the',/,
     *              'Normal field coefficients by more than ',/,
     *              '0.01 mgals! ***')

          write(6,*) ' gammah  (gamh subrtn) = ',gammah
          write(6,*) ' gammachk(g - Delta-g)  = ',gammachk
          write(6,*) ' dif, mGals            = ',dd
        endif

c--------------------------------------
c - STEP #3:
c - Calculate gravimetric quantities of interest
c - Height Anomaly (Assuming W(surf) = U(telluroid)

        htanom = typot/gammah

c - Deflections of the Vertical (assuming d(gamma)/ds = 0)
c - Convert deflections from radians to arcseconds

        const = 3600.d0/gamma/d2r
        defns = -typot1(1)*const ; defew = -typot1(2)*const

c - Get dr/dh, needed for gravity disturbance:

        s2 = dsin(geodlat*d2r)**2 ; xw = dsqrt(1.d0 - e2norm*s2)
        xn = anorm/xw             ; drdh=((xn + h) - xn*e2norm*s2)/r

c - Gravity Disturbance (two ways; convert both to mgals)

        grdist1 = (gravityh - gammah)*1d5             !This is in m/sec**2, convert to mGals

c - Gravity Anomaly (Assuming W(surf) = U(telluroid)):
c - Modified for version 0.4:

        call getdgtyp(idgtyp)
        if(idgtyp.eq.1)then
          dtdr = typot1(3)
          anom = (-dtdr - 2.d0*typot/r)*1.d5
        else
          if(h.ne.0)then
            write(6,*) ' I cannot calculate dgamma/dh'
            write(6,*) ' at altitudes, so I will give'
            write(6,*) ' the spherical approximation'
            write(6,*) ' to the gravity anomaly'
            dtdr = typot1(3)
            anom = (-dtdr - 2.d0*typot/r)*1.d5
          else
            dtdh = typot1(3)*drdh
            dgamdh = dgdh(geodlat,anorm,finvnorm,gamma,fomega)
            anom = (-dtdh + typot*dgamdh/gamma)*1.d5
          endif
        endif

c - Gravity Gradients in mgals/km:

        typot2 = typot2*1.d8
        x11 = typot2(1,1) ; x12 = typot2(1,2) ; x13 = typot2(1,3)
        x21 = typot2(2,1) ; x22 = typot2(2,2) ; x23 = typot2(2,3)
        x31 = typot2(3,1) ; x32 = typot2(3,2) ; x33 = typot2(3,3)

c - Print it all out neatly:

        write(6,103)name,tidsum
  103   format(1x,60('*'),/,
     *         1x,'Coefficient Set: ',a9,
     *         8x,'Tide System: ',a4,'tide')
        write(6,104)gmnorm,anorm,j2norm,fomega
  104   format(1x,'Normal Field:  GM = ',d20.10,5x,
     *            'a     = ',d20.10,/,
     *         1x,'               J2 = ',d20.10,5x,
     *            'omega = ',d20.10,/)
        write(6,105)geodlat,geodlon,h
  105   format(1x,'Geodetic Latitude(deg) : ',f19.11,/,
     *         1x,'Geodetic Longitude(deg): ',f19.11,/,
     *         1x,'Ellipsoidal Height(m)  : ',f19.11,/)
        write(6,106)xp1,xp3,xp2
  106   format(1x,'Total  Gravitational Potential (m**2/s**2): ',f20.10,/,
     *         1x,'Total  Centrifugal   Potential (m**2/s**2): ',f20.10,/,
     *         1x,'Total  Gravity       Potential (m**2/s**2): ',f20.10,/)

        write(6,115)xnp1,xnp3,xnp2
  115   format(1x,'Normal Gravitational Potential (m**2/s**2): ',f20.10,/,
     *         1x,'Normal Centrifugal   Potential (m**2/s**2): ',f20.10,/,
     *         1x,'Normal Gravity       Potential (m**2/s**2): ',f20.10,/)

        write(6,116)tionpot,cenpot,typot
  116   format(1x,'Dist.  Gravitational Potential (m**2/s**2): ',f20.10,/,
     *         1x,'Dist.  Centrifugal   Potential (m**2/s**2): ',f20.10,/,
     *         1x,'Dist.  Gravity       Potential (m**2/s**2): ',f20.10,//)


        write(6,107)htanom
  107   format(1x,'Height Anomaly (m, between Wsurf=Utell)  : ',f15.5)
        write(6,108)anom,defns,defew
  108   format(1x,'Gravity Anomaly (mGals, btwn Wsurf=Utell): ',f10.5
     *  ,/,
     *         1x,'Deflection N/S (arcsec): ',f10.5,5x,
     *            'Deflection E/W (arcsec): ',f10.5,/)

        write(6,708)gravityh,gammachk
  708   format(1x,'Total  Gravity at h (m/sec**2) = ',f15.10,/,
     *         1x,'Normal Gravity at h (m/sec**2) = ',f15.10)

        write(6,109)grdist1
  109   format(1x,
     * 'Gravity Disturbance (abs(g)-abs(gamma), mGals ): ',f15.5,/)

        write(6,110)x11,x12,x13,x22,x23,x33
  110   format(1x,'Gravity Gradients (mgals/km):',/,
     *        11x,'  dNorth  ',3x,'  dEast   ',3x,'  dUp     ',/,
     *  1x,'dNorth    ',f10.7,3x,f10.7,3x,f10.7,/,
     *  1x,'dEast     ',10x,3x,f10.7,3x,f10.7,/,
     *  1x,'dUp       ',10x,3x,10x,3x,f10.7,//)

        stop

c----------------------------------------------------------------------------

      subroutine getref (a,b,finv,j2,gm,omega,tidsum)

c - Subroutine to input the normal gravity field

      implicit real*8(a-h,o-z)

      real*8      j2
      character   tidsum*4,tidnorm*4,yesno*1
      character   string*80

  101 write(6,1)
    1 format(/1x,'You are now about to input the parameters of the ',
     *'ellipsoidal normal gravity',/,
     *1x,'field.  This is NOT the so-called best-fitting ellipsoid, ',
     *'but is rather the ',/,
     *1x,'actual ellipsoid to which the undulations will refer.  ',
     *'The method to be used is',/,
     *1x,'method #3 (Notebook DRU-3, p. 58), which has NO geometric ',
     *'transformations.',/,
     *1x,'So, let us proceed.....',//)
    3 write(6,2)
    2 format(1x,'How shall the normal field be described?',/,
     *1x,'1 - a,J2,GM,omega',/,1x,'2 - a,f,GM,omega',/,
     *1x,'3 - a,b,GM,omega',/,'4 - a,C20,GM,omega')

      read(5,*)ians
      string = ' '
      write (string,*) ians
      call log (string)
      if(ians.le.0 .or. ians.gt.4)goto 3

      write(6,4)
    4 format(1x,'Input the equatorial radius (a) = ',$)
      read(5,*) a
      string = ' '
      write (string,*) a
      call log (string)
      write(6,5)
    5 format(1x,'Input the GM value = ',$)
      read(5,*) gm
      string = ' '
      write (string,*) gm
      call log (string)
      write(6,6)omega
    6 format(1x,'To remind you, Omega is defined as:',e13.7)

c - Input either j2,finv or b, but compute the other two as well

      if(ians.eq.1)then
        write(6,7)
    7   format(1x,'Input the J2 value = ',$)
        read(5,*)j2
        string = ' '
        write (string,*) j2
        call log (string)
        call getb(a,j2,gm,omega,b)
        finv = a/(a-b)
        c20 = -0.447213595d0*j2
      elseif(ians.eq.2)then
        write(6,8)
    8   format(1x,'Input the INVERSE flattening (1/f) = ',$)
        read(5,*)finv
        string = ' '
        write (string,*) finv
        call log (string)
        b = a - (a/finv)
        call getj2(a,gm,omega,b,j2)
      elseif(ians.eq.3)then
        write(6,9)
    9   format(1x,'Input the semi-minor axis (b) = ',$)
        read(5,*)b
        string = ' '
        write (string,*) b
        call log (string)
        finv = a/(a-b)
        call getj2(a,gm,omega,b,j2)
      elseif(ians.eq.4)then
        write(6,17)
   17   format(1x,'Input the C20 value = ',$)
        read(5,*)C20
        string = ' '
        write (string,*) C20
        call log (string)
        j2 = -C20*sqrt(5.d0)
        call getb(a,j2,gm,omega,b)
        finv = a/(a-b)
      endif

C  output GRS parameters

      flat = 1.d0/finv ; e2norm = flat*(2.d0 - flat)

      write (*,*) 'GRS values in the GRS old tidal system'
      write (*,'(a20,d19.12)') 'anorm             = ',a
      write (*,'(a20,d19.12)') 'C20norm           = ',C20
      write (*,'(a20,d19.12)') 'GMnorm            = ',gm
      write (*,'(a20,d19.12)') 'omega             = ',omega
      write (*,'(a20,d19.12)') 'J2norm            = ',j2
      write (*,'(a20,d19.12)') 'finvnorm          = ',finv
      write (*,'(a20,d19.12)') 'bnorm             = ',b
      write (*,'(a20,d19.12)') 'e2norm            = ',e2norm

c - Get the tide system of the normal field.
c - We will transform the normal field from this system into the
c - tidsum system (which is also what the total coefficients will
c - be transformed into, in another part of the program, to ensure
c - that disturbing potential coefficients are computed from
c - total/normal coefficients in a consistant system)

   11 write(6,10)
   10 format(/1x,'What tide system is this normal field in?',/,
     * 1x,'1: mean',/,1x,'2: zero',/,1x,'3: non-')
      read(5,*)ians
      string = ' '
      write (string,*) ians
      call log (string)
      if(ians.le.0 .or. ians.ge.4) goto 11
      if(ians.eq.1) tidnorm = 'mean'
      if(ians.eq.2) tidnorm = 'zero'
      if(ians.eq.3) tidnorm = 'non-'

c - get the final ref field in the "tidsum" system

      if(tidnorm.eq.tidsum)return
      write(6,12)
   12 format(/1x,'You have input a normal field in a tide',/,
     *1x,'system different than you have requested for output.',/,
     *1x,'In order to have consistancy in computing the ',/,
     *1x,'disturbing coefficients, we will now transform your',/,
     *1x,'normal field to the tide system of the output.'//)
      write(6,13)
   13 format(1x,20('!'),'IMPORTANT',20('!'))
      write(6,14)
   14 format(1x,'Note that this *WILL* change the SHAPE of the ',/,
     *1x,'ellipsoid.  I hope you thought about that.  If not,',/,
     *1x,'you can go back and try entering the normal field',/,
     *1x,'again.')
      write(6,15)
   15 format(1x,'y = YES, please let me re-enter the normal field.',
     */,1x,'n = NO, I know what I am doing.  Proceed, darnit!')
      read(5,'(a)')yesno
      string = ' '
      write (string,*) yesno
      call log (string)
      if(yesno.eq.'y' .or. yesno.eq.'Y')goto 101
      write(6,16)
   16 format(1x,'Ok, you asked for it....')

      call fixab2 (a,b,tidnorm,tidsum,afixed,bfixed)
      a = afixed ; b = bfixed ; finv = a/(a-b)
      call getj2(a,gm,omega,b,j2)

      return

      end

c-----------------------------------------------------------------------------------
      subroutine norm (a,j2,gm,omega,b,j,nmax,u0,gammae,gammap)

c - This subroutine takes, as input, the normal gravity field values
c - of a,j2,gm,omega and b (b is gotten from "getb" first), and
c - computes the values of the normal gravity field:

c -    j(nmax) = coefficients of the spherical harmonic expansion of
c -              the normal gravitational potential
c -              nmax is equal to the highest degree desired divided by 2
c -              (thus if j2 through j20 are desired, set n = 10,
c -              and receive j2 = j(1), j4 = j(2), ..., j20 = j(10)
c -              (see H/M, eqn after 2-91)
c -              (note that j2 is input, not calculated)

c -    u0 = value of normal gravity potential on the ellipsoid

c -    gammae = normal gravity, on the ellipsoid, at the equator
c -    gammap = normal gravity, on the ellipsoid, at the pole

      implicit real*8(a-z)
      integer*4   n,nmax
      real*8      j(nmax)
      parameter (third = 0.333333333333333d0,sixth = 0.166666666666667d0)

      a2 = a*a ; b2 = b*b ; omega2 = omega*omega
      bige = dsqrt(a2 - b2) ; e = bige/a ; ep = bige/b ; ep2 = ep*ep
      e2 = e*e ; arctan = datan(ep) ; epinv = 1.d0/ep ; epinv2 = epinv*epinv

c - H/M (exact formula), 2-61, p. 67

      u0 = (gm/bige)*arctan + third*omega2*a2
      q0 = (0.5d0 + 1.5d0*epinv2)*arctan - 1.5d0*epinv

c - H/M, exact formula, 2-67 (applied at u=b), p. 68

      qp0 = 3.d0*(1.d0 + epinv2)*(1.d0 - epinv*arctan) - 1.d0

c - Change for version 0.4c:
c - Added the definition of j(1)!!!!!!!

      j(1) = j2

c - Change for version 0.4a:
c - Replaced 10/2/1997.  What was I thinking above???

      do n=2,nmax
        twon = 2.d0*n
        j(n) = (-1)**(n+1)*3.d0*e2**n*(1.d0 - n + 5.d0*n*j2/e2)/((twon + 1.d0)*(twon + 3.d0))
      enddo

      do n=1,nmax
        write(6,'(i20,d19.12)') 2*n,-j(n)/sqrt(4.d0*n + 1.d0)
      enddo

c - H/M, exact formula, 2-70, p. 69

      m = omega2*a2*b/gm

c - H/M, exact formula, 2-73, 2-74, p. 69

      gma    = gm/a
      quant  = m*ep*qp0/q0
      gammae = (gma/b)*(1.d0 - m - sixth*quant)
      gammap = (gma/a)*(1.d0     + third*quant)
      write (*,'(a20,d19.12)') 'e2                = ',e2
      write (*,'(a20,d19.12)') 'geqt              = ',gammae
      write (*,'(a20,d19.12)') 'gpol              = ',gammap
      write (*,'(a20,d19.12)') 'm                 = ',m

      return
      end
c----------------------------------------------------------------------------


c-----------------------------------------------------------------------------------
      subroutine pot(xlat,xlon,r,omega,tionpot,tionpot1,tionpot2,
     *    cenpot,cenpot1,cenpot2,typot,typot1,typot2,isw,nmax,
     *    root,dmp1mp1,smlp1,cmlp1,c,su,max,gmsum,asum,force)
c
c - subroutine to compute quantities based on either the FULL
c - potential, or the DISTURBING potential.  For the case of the
c - disturbing potential, it is assumed that scaling of the
c - coefficients has occurred if differing "GM" or "a" values
c - are being sent in, and that the coefficients (and also omega!!)
c - are DISTURBING values, not FULL values!!!
c


c - tionpot = gravitaTIONal POTential (full or disturbing, based on coefs)
c - typot   = graviTY POTential (full or disturbing, based on coefs & omega)
c - cenpot  = CENtrifugal POTential (full or disturbing, based on omega)
c
c - tionpot1,typot1,cenpot1 = 1st order derivatives (N,E,U)
c - tionpot2,typot2,cenpot2 = 2nd order derivatives
c -        (NN,NE,NU ; EN,EE,EU ; UN,UE,UU)
c
c - isw     = iSWitch, to tell the summing routine whether we're
c             looking for:
c             0 = just the potential
c             1 = potential and 1st derirvatives (gravity)
c             2 = potential, 1st and 2nd derivatives (gravity gradients)

      implicit real*8(a-h,o-z)

      logical  force,unnormalize

      real*8   root((max+1)*2)
      real*8   dmp1mp1(0:max)
      real*8   smlp1(0:max+2),cmlp1(0:max+2)
      real*8   c(0:(max+1)*(max+1)-1)
      real*8   su(10*(max+1))
      real*8   tionpot1(3),cenpot1(3),typot1(3)
      real*8   tionpot2(3,3),cenpot2(3,3),typot2(3,3)

      call centpot (xlat,xlon,r,omega,cenpot,cenpot1,cenpot2,isw)

c - cenpot = centrifugal potential
c - cenpot1 = 1st derivatives, 3 directions
c - cenpot2 = 2nd derivatives, 3x3 directions

      call gravitation (xlat,xlon,r,tionpot,tionpot1,tionpot2,isw,nmax,su,
     *           root,dmp1mp1,smlp1,cmlp1,c,max,gmsum,asum,force)

c - tionpot = gravitational potential
c - tionpot1 = 1st derivatives, 3 directions
c - tionpot2 = 2nd derivatives, 3x3 directions

      typot  = cenpot  + tionpot
      typot1 = cenpot1 + tionpot1
      typot2 = cenpot2 + tionpot2

      return
      end
c---------------------------------------------------------------------------------------




























      subroutine centpot(xlat,xlon,r,omega,cp,cp1,cp2,isw)

c - subroutine to compute centrifugal potential (m**2/sec**2), and its
c - 1st and 2nd order derivatives.

      implicit real*8(a-h,o-z)

      real*8 cp1(3),cp2(3,3)

      common/block1/pi,d2r

c - Clear anything out

      cp = 0.d0     ; cp1 = 0.d0 ; cp2 = 0.d0
      fi = xlat*d2r ; c = dcos(fi)
      cp = 0.5d0*omega*omega*r*r*c*c

      if (isw == 0) return

      omega2 = omega*omega
      s      = dsin(fi)

      cp1(1) = -omega2*r*c*s ; cp1(2) = 0.d0 ; cp1(3) = omega2*r*c*c
      if (isw == 1) return

      c2 = dcos(2.d0*fi)

      cp2(1,1) = -omega2*c2
      cp2(1,2) = 0.d0
      cp2(1,3) = -2.d0*omega2*c*s
      cp2(2,1) = 0.d0
      cp2(2,2) = 0.d0
      cp2(2,3) = 0.d0
      cp2(3,1) = cp2(1,3)
      cp2(3,2) = 0.d0
      cp2(3,3) = omega2*c*c

      return
      end
C---------------------------------------------------------------------------
      subroutine gravitation (xlat,xlon,r,tionpot,tionpot1,tionpot2,isw,nmax,su,
     *          root,dmp1mp1,smlp1,cmlp1,c,max,gmsum,asum,force)

c - subroutine to prepare to go into GPTDR, the Clenshaw
c - summation subroutine.
c
c - tionpot = Gravitational Potential
c - tionpot1 = 1st derivs of tionpot
c - tionpot2 = 2nd derivs of tionpot
c
c - isw = switch (0,1 or 2) telling whether you want 0th, 1st
c -       or 2nd order derivatives added up

      implicit real*8(a-h,o-z)

      real*8     lamda
      integer*4  order
      logical    force,unnormalize

      real*8     root((max+1)*2)
      real*8     dmp1mp1(0:max)
      real*8     smlp1(0:max+2),cmlp1(0:max+2)
      real*8     c(0:(max+1)*(max+1)-1)
      real*8     su(10*(max+1))
      real*8     tionpot1(3),tionpot2(3,3)
      real*8     po(6)

      common/block1/pi,d2r

c - Clear anything out

      tionpot  = 0.d0 ; tionpot1 = 0.d0 ; tionpot2 = 0.d0

      po(1) = r*dcos(xlat*d2r) ; po(2) = r
      theta = (90.d0 - xlat)*d2r
      po(3) = dcos(theta) ; po(4) = dsin(theta)
      lamda = xlon*d2r
      po(5) = dsin(lamda) ; po(6) = dcos(lamda)


c - call gptdr with negative nmax to indicate the use of
c - quasi-normalized coefficients

      order = isw
      if (unnormalize) then
        negn = nmax
      else
        negn = -nmax
      endif
      tionpot = gptdr(po,negn,order,su,tionpot1,tionpot2,root,dmp1mp1,smlp1,
     &                cmlp1,c,max,gmsum,asum,force)

      return
      end
c-----------------------------------------------------------------------------------------
      SUBROUTINE SETCM (CAPN,root,c,su,dmp1mp1,max)

*** THIS ROUTINE SETS THE SQUARE ROOT TABLE IN 'root'
*** changes the coefficients from fully normalized to quasi-normalized
c - CAPN = actual maximum degree to be used
c - max  = maximal possible degree read in

c - Fully normalized and quasi-normalized C(0,0) is the same

      IMPLICIT REAL*8(A-H,O-Z),INTEGER*4(I-N)

      INTEGER*4 CAPN,twom

      real*8    root((max+1)*2)
      real*8    dmp1mp1(0:max)
      real*8    c(0:(max+1)*(max+1)-1)
      real*8    su(10*(max+1))

      NN = 2*(max+1)
      DO I=1,NN
        ROOT(I) = SQRT(dfloat(i))
      enddo

      SQ2 = DSQRT(2.D0)  ! <- to change the m=0 conversion to m<>0 conversion

c - Don't loop from 0, since deg 0 fully norm and quasi norm are same

      do n=1,CAPN
        N2=N+N
        S21 = root(n2+1)  ! <- use pre-calculated values!!!
        K=N**2  !  <- neat...c(n**2) = C(n,m=0)

*** D IS THE QUASI-NORMALIZATION FACTOR FOR ZONAL TERMS

        D=S21
        C(K)=C(K)*D

*** GG IS THE QUASI-NORMALIZATION FACTOR FOR NON-ZONAL TERMS

        GG=D*SQ2
        DO J=1,N
          KJ2=J+J+K
          C(KJ2-1)=C(KJ2-1)*GG !  <- the C(n,m<>0) terms
          C(KJ2)=C(KJ2)*GG     !  <- the S(n,m<>0) terms
        enddo
      enddo

      do m=0,max
        twom = 2*m
        dmp1mp1(m) = root(twom + 1)/root(twom + 2)
      enddo

      return
      end
c -------------------------------------------------------------------
      function gptdr (po,nmax,order,su,g1,g2,root,dmp1mp1,smlp1,cmlp1,
     &                c,max,gmsum,asum,force)

c - Version 1/14/97, by Dru Smith, National Geodetic Survey
c - Uses the core of the original function, but cleans it up
c - for readability and moved all non-clenshaw functions
c - OUTSIDE this function.
c - Added logical function "force" which, if true, forces
c - a re-summation (useful when you've switched the coefficients)

c - Rather than have commons/parameters, I just pass all the
c - variable addresses: g1,g2,root,smlp1,cmlp1,c,max
c - I also pass ONLY ONE value of GM (gmsum) and
c - ONLY ONE value of "a" (asum), assuming that all of the
c - coefficients are scaled.  I also do NOT pass omega, as it has
c - no bearing on the gravitational potential (which is this
c - subroutine) and do NOT pass J2 (or other parts of the reference
c - normal field) as they also have nothing to do with the
c - Clenshaw summation of gravitational potential!
c
c - IN short, all I need (variable-wise) is:
c      1)  The "c" array holding C(n,m) and S(n,m) in a set
c            order, and already quasi-normalized.  These coefficients
c            represent either full or disturbing gravitational
c            potential, and are scaled to work with "gmsum" and "asum"
c      2)  The value of GM used in the summation, gmsum
c      3)  The value of a  used in the summation, asum
c      4)  The positional information, in the "po" array
c      5)  The maximum degree to take the summation, nmax
c      6)  The highest order derivatives to compute, order.
c - What comes out:
c      1)  GPTDR itself returns the gravitational potential (full or
c          disturbing.
c      2)  g1(3) returns the N,E and up derivatives of GPTDR
c      3)  g2(3,3) returns the NN,NE,NU,EN,EE,EU,UN,UE,UU derivatives
c          of GPTDR


c - THAT IS ALL THIS SUBROUTINE HAS BEEN MODIFIED TO DO!

c - Original comments:
*** GI REG.NO. 81013 AUTHOR -C.C.TSCHERNING, DANISH GEODETIC INSTITUTE
***                                     JULY 1981 IN ALGOL REF.(2)
***                         -C.C.GOAD, NOAA/NOS/NATIONAL GEODETIC SURVEY
***                                      MAY 1982 TRANSLATED TO FORTRAN

*** REFERENCES:
*** (1) TSCHERNING, C.C.:ON THE CHAIN-RULE METHOD FOR COMPUTING
***     POTENTIAL DERIVATIVES. MANUSCRIPTA GEODAETICA, VOL.1,
***     PP. 125-141, 1976

*** (2) TSCHERNING, C.C., AND PODER, K.: SOME APPLICATIONS OF CLENSHAW
***     SUMMATION, PRESENTED AT VIII SYMPOSIUM ON MATHEMATICAL GEODESY,
***     COMO, ITALY, SEPT 7-9, 1981

***  THE PROCEDURE COMPUTES THE VALUE AND UP TO THE SECOND-ORDER
*** DERIVATIVES OF THE POTENTIAL OF THE EARTH (W) OR OF ITS
*** CORRESPONDING ANOMALOUS POTENTIAL(T).

***  THE POTENTIAL IS REPRESENTED BY A SERIES OF SOLID SPHERICAL
*** HARMONICS, WITH UN-NORMALIZED OR QUASI-NORMALIZED COEFFICIENTS.
*** THE CHAIN-RULE IS USED ALONG WITH THE CLENSHAW ALGORITHM.
*** THE ARRAY C MUST HOLD THE COEFFICIENTS C(1)=C(1,0),C(2)=C(1,1),
*** C(3)=S(1,1), ETC. UP TO C((N+1)**2-1=S(N,N).  C(0,0) IS STORED IN C0
*** WHICH IMPLICITLY ACTS AS C(0) (SEE THE COMMON BLOCK CM).


*** PARAMETERS:

*** (A) INPUT VALUES:

*** NMAX
***    THE ABSOLUTE VALUE OF NMAX IS EQUAL TO THE MAXIMAL DEGREE AND
***    ORDER OF THE SERIES. NEGATIVE NMAX INDICATES THAT THE COEFFICIENTS
***    ARE QUASI-NORMALIZED.

*** ORDER
***    ORDER OF DERIVATIVES
***    0 FOR POTENTIAL ONLY
***    1 FOR POTENTIAL AND FIRST DERIVATIVES
***    2 FOR POTENTIAL, FIRST DERIVATIVES, AND SECOND DERIVATIVES

*** PO
***    ARRAY HOLDING POSITION INFORMATION. PO(6)
***    PO(1)=P, THE DISTANCE FROM THE Z (ROTATION) AXIS,
***    PO(2)=R, THE DISTANCE FROM THE ORIGIN,
***    PO(3),PO(4) COS AND SIN OF GEOCENTRIC POLAR ANGLE(COLATITUDE),
***    PO(5),PO(6) SIN AND COS OF THE LONGITUDE.

*** C
***    C((ABS(NMAX)+1)**2-1)    ARRAY OF C'S AND S'S DESCRIBED ABOVE


c - GEOPOT97:  Ignore the following historical comments.
c - das
***    CM3=GM
***    CM2=A THE SEMI-MAJOR AXIS OF THE REFERENCE ELLIPSOID
***    CM1=THE ANGULAR VELOCITY (=0,WHEN DEALING WITH T)
***    C0=1.D0 FOR W AND =0.D0 FOR T

c - ACTUALLY!!!!! --  CM1,2,3 are truly those values assigned
c -                   to the UNSCALED coeffients.  However,
c -                   the coefficients have been SCALED, so
c -                   that the proper use of the scale factors
c -                   is to use OMEGA, AE, and GM!!!!!!!!!
c -                   These values come in under the "normal"
c -                   common block, and are hard-coded into
c -                   this subroutine, replacing CM1,CM2,CM3

c -                   ALSO - C0 is not necessarily 1 or 0.  It
c -                   is (CM3/GM)-1, for T, and 1 for W.
c -                   This is because of the
c -                   possible difference between CM3 and GM
c - das
c - GEOPOT97:  Til here for ignoring


***    ROOT(K)=SQRT(K), 0.LE.K.LE.2(ABS(N)+1)-1  WHEN NMAX.LT.0


*** (B) RETURN VALUES:

*** G1 AND G2
***    THE RESULT IS STORED IN G1 AND G2 AS FOLLOWS:

***    G1(1)=DW/DX, G1(2)=DW/DY, G1(3)=DW/DZ
***    G2(1,1)=DDW/DXX, G2(1,2)=G2(2,1)=DDW/DXDY,
***    G2(1,3)=G2(3,1)=DDW/DXDZ, G2(2,2)=DDW/DYY,
***    G2(2,3)=G2(3,2)=DDW/DYDZ AND G2(3,3)=DDW/DZZ
***    WHERE W MAY BE INTERCHANGED WITH T AND
***    VARIABLES X, Y, Z ARE THE CARTESIAN COORDINATES
***    IN A LOCAL (FIXED) FRAME WITH ORIGIN IN THE POINT
***    OF EVALUATION, X POSITIVE NORTH, Y POSITIVE EAST,
***    AND Z POSITIVE IN THE DIRECTION OF THE RADIUS
***    VECTOR, (CF. REF.(1),EQ (4) AND (5)).
***    THE VALUES OF W OR T WILL BE RETURNED IN GPTDR.

*** (C)  PASSED AND RETURNED VALUES:

*** SU
***    ARRAY OF DIMENSION K*(N+1), WHERE K=2 FOR NO DERIVATIVES,
***    =6 FOR 0-TH AND FIRST DERIVATIVES, =10 FOR 0-TH, FIRST AND
***    SECOND DERIVATIVES.  HERE ARE STORED THE PARTIAL SUMS, CF.
***    REF.(2), EQ. (29), OF P(N,M)*(A/R)**(N+1-M)/P(M,M)*(C(N,M) OR
***    S(N,M))  FROM N=M TO N=N, AND THE DERIVATIVES OF THESE SUMS.
***    THIS MAKES IT UNNECESSARY TO RECALCULATE THESE QUANTITIES, IF
***    THE PROCEDURE IS CALLED SUBSEQUENTLY WITH THE SAME VALUE OF T
***    AND R, AND THE SAME ORDER.
**********************************************************************

      IMPLICIT REAL*8 (A-H,O-Z),INTEGER*4(I-N)

      INTEGER*4   CAPN,ORDER,CAPN21,OLDORD
      REAL*8      M21,M21T,M21U,M21U0,invscale

      REAL*8      VCS(2),VCS1(2),VCS0(2),VCS2(2)
      REAL*8      VxCS(2),VxCS1(2),VxCS2(2),CKZ(2)
      REAL*8      VzCS(2),VzCS1(2),VzCS2(2)
      REAL*8      VxxCS(2),VxxCS1(2),VxxCS2(2)
      REAL*8      VzzCS(2),VzzCS1(2),VzzCS2(2)
      REAL*8      VxzCS(2),VxzCS1(2),VxzCs2(2)

      parameter (scale = 1d-280, invscale = 1d280)

      LOGICAL     QUASI,DERIV1,DERIV2,POLE,FIRST,NEW,OLD,NPOLE,force

      real*8      root((max+1)*2)
      real*8      dmp1mp1(0:max)
      real*8      smlp1(0:max+2),cmlp1(0:max+2)
      real*8      c(0:(max+1)*(max+1)-1)
      real*8      su(10*(max+1))
      real*8      po(6),g1(3),g2(3,3)

c - These determine if we've been here before (efficiency!)

      COMMON/GPTCM/OLDT,OLDR,IZ,FIRST,OLDORD,I1,I2,I3,I4,I5,I6,I7,I8,I9,NMAXSV

c - convert to the local variable to avoid a lengthy replacement

      CM2 = asum ; CM3 = gmsum

      IF (NMAXSV.NE.NMAX)FIRST=.FALSE.
      NMAXSV=NMAX

      if(force) first = .false. ! <-seems backwards, but logically ok

      IF (FIRST) GO TO 100

      FIRST = .TRUE.
      OLDT  = 2.D0   ! < - picked to force disagreement with -1<=T<=1
      J     = IABS(NMAX)
      I=J+1 ; I1=I+1 ; I2=I1+I ; I3=I2+I ; I4=I3+I ; I5=I4+I
      I6=I5+I ; I7=I6+I ; I8=I7+I ; I9=I8+I

  100 CAPN=NMAX

C  Geometric quantities

      p    = PO(1)           ! DISTANCE FROM ROTATION AXIS
      r    = PO(2)           ! DISTANCE FROM ORIGIN
      T    = PO(3)           ! COSINE OF COLATITUDE
      U    = PO(4)           ! SINE OF COLATITUDE
      SL   = PO(5)           ! SINE OF LONGITUDE
      CL   = PO(6)           ! COSINE OF LONGITUDE
      T2   = T+T

      POLE = DABS(U) <= 1.D-9
      NEW  = DABS(OLDR-R) > 1.D-3 .OR. DABS(OLDT-T) > 1.D-9 .OR. OLDORD /= ORDER .OR. POLE
      OLD  = .NOT.NEW
      NPOLE= .NOT.POLE

      if (new) then
        OLDR=R ; OLDT=T ; OLDORD = ORDER
      endif

C  Quasi-normalization quantities

      QUASI  = .FALSE.
      IF (CAPN < 0) then
        QUASI = .TRUE.
        CAPN  = -CAPN
      endif

*** COMPUTE AE/R

      S = CM2/r ; S2 = S**2

C  Do you need to compute derivatives of the geopotential?

      DERIV1 = .FALSE. ; IF (ORDER > 0) DERIV1 = .TRUE.
      DERIV2 = .FALSE. ; IF (ORDER > 1) DERIV2 = .TRUE.

c  SMLP1(M) = sin(m*longitude) , CMLP1(M) = cos(m*longitude)

      CMLP1(0) = 1.D0 ; SMLP1(0) = 0.D0
      CMLP1(1) = CL   ; SMLP1(1) = SL

      M1=1
      DO M=2,CAPN
        CMLP1(M)=CMLP1(M1)*CL-SMLP1(M1)*SL ; SMLP1(M)=SMLP1(M1)*CL+CMLP1(M1)*SL
        M1=M
      enddo

C  Initialize some more parameters

      CAPN21 = CAPN + CAPN + 1
      SQNM1  = 1.D0 ; SQNPM1 = 1.D0
      KM     = (CAPN+1)**2
      MAX2   = CAPN21
      ITWO   = 2

      VM     = 0.D0
      if (DERIV1) then
        VxM=0.D0 ; VyM=0.D0 ; VzM=0.D0
      endif
      IF(DERIV2) then
        VxxM=0.D0 ; VyyM=0.D0 ; VzzM=0.D0 ; VxyM=0.D0 ; VxzM=0.D0 ; VyzM=0.D0
      endif

*** WE NOW USE THE CLENSHAW ALGORITHM, CF. REF.(2), EQ(8-13),
*** MODIFIED IN AN OBVIOUS WAY FOLLOWING REF.(1).

      DO m=CAPN,0,-1
        if (m == 0) ITWO=1
        KM  = KM-ITWO  ; K = KM      ; N21 = CAPN21
        CM  = CMLP1(M) ; SM = SMLP1(M)
        NM1 = CAPN-M+2 ; N1 = CAPN+1 ; NPM1 = CAPN+M+2

        VCS = 0.D0 ; VCS1 = 0.D0
        if (DERIV1) then
          VxCS = 0.d0 ; VxCS1=0.D0 ; VzCS=0.D0 ; VzCS1=0.D0
        endif
        IF (DERIV2) then
          VxxCS=0.D0  ; VxxCS1=0.D0 ; VzzCS=0.D0 ; VzzCS1=0.D0
          VxzCS=0.D0  ; VxzCS1=0.D0
          M2 = M*M
        endif

C  If the same latitude and r, skip recursion over latitude and r

        if (new) then
          N = CAPN+1
          do in=m,CAPN
            n=n-1 ; NM2=NM1 ; NM1=NM1-1 ; NPM1=NPM1-1
            IF(QUASI) then
              SQNM2=SQNM1 ; SQNM1=ROOT(NM1) ; SQNPM2=SQNPM1
              SQNPM1=ROOT(NPM1) ; SQ1=SQNM1*SQNPM1
              A1=S*N21/SQ1 ; B2=-S2*SQ1/(SQNM2*SQNPM2)
            else
              A1=S*N21/NM1 ; B2=-S2*NPM1/NM2
            endif
            A1T=A1*T
            A1U=A1*U
            N21=N21-2
            VCS0=(/C(K),C(K+1)/)*scale
            K=K-N21
            VCS2=VCS1 ; VCS1=VCS ; VCS=VCS1*A1T+VCS2*B2+VCS0
            IF(.NOT.DERIV1) cycle
            A1U=A1*U
            CKZ=VCS0*N1
            VxCS2=VxCS1 ; VxCS1=VxCS ; VxCS=VxCS1*A1T+VCS1*A1U+VxCS2*B2
            VzCS2=VzCS1 ; VzCS1=VzCS ; VzCS=VzCS1*A1T         +VzCS2*B2-CKZ
            N1=N
            IF(.NOT.DERIV2) cycle
            N2=N+2
            VzzCs2=VzzCS1 ; VzzCS1=VzzCS ; VzzCS=VzzCS1*A1T+VzzCS2*B2+N2*CKZ
            IF(.not. NPOLE) then
              VxxCS2=VxxCS1 ; VxxCS1=VxxCS
              VxxCS=A1T*(VxxCS1-VCS1)+(A1U+A1U)*VxCS1+VxxCS2*B2
            endif
            VxzCS2=VxzCS1 ; VxzCS1=VxzCS ; VxzCS=VxzCS1*A1T+VzCS1*A1U+VxzCS2*B2
          enddo

          SU(M+1)=VCS(1) ; SU(M+I1)=VCS(2)
          IF(DERIV1) then
            SU(M+I2)=VxCS(1) ; SU(M+I3)=VxCS(2)
            SU(M+I4)=VzCS(1) ; SU(M+I5)=VzCS(2)
          endif
          IF(DERIV2) then
            SU(M+I6)=VzzCS(1) ; SU(M+I7)=VzzCS(2)
            SU(M+I8)=VxzCS(1) ; SU(M+I9)=VxzCS(2)
          endif

        else

          VCS(1)=SU(M+1) ; VCS(2)=SU(M+I1)
          NPM1=MAX2 ; MAX2=MAX2-2
          if (DERIV1) then
            VxCS(1)=SU(M+I2) ; VxCS(2)=SU(M+I3)
            VzCS(1)=SU(M+I4) ; VzCS(2)=SU(M+I5)
          endif
          if (DERIV2) then
            VzzCS(1)=SU(M+I6) ; VzzCS(2)=SU(M+I7)
            VxzCS(1)=SU(M+I8) ; VxzCS(2)=SU(M+I9)
          endif

*** THE COMPUTATION OF DERIVATIVES IN DIRECTION OF POS LONGITUDE,Y,
*** INVOLVES THE DIVISION BY U=COS(LATITUDE).  THIS DIVISION IS
*** PERFORMED IMPLICITLY, BY STOPPING THE CLENSHAW SUMMATION AT M=1.
*** THIS IS DONE BY PUTTING U0=1.0.  THIS TRICK PERMITS THE USE OF THE
*** PROCEDURE AT POLES, EXCEPT FOR THE SECOND-ORDER DERIVATIVE WRT
*** LONGITUDE.  BUT HERE WE USE THE VALIDITY OF THE LAPLACE EQUATION
*** AND COMPUTE THE SECOND-ORDER DERIVATIVES WRT X AND Z

        endif
        U0=U ; IF (M == 0) U0=1.D0

        if (QUASI) then
          AUX = dmp1mp1(m)
        else
          AUX = NPM1
        endif
        M21=S*AUX ; M21U=M21*U
        IF(DERIV1) then
          M21T=M21*T ; M21U0=M21*U0                      !REF.(2) EQ.(35)
          if (DERIV2) then
            VzzM=VzzCS(1)*CM+VzzCS(2)*SM+M21U*VzzM
            IF (M.GT.0) VxyM=M*(VxCS(2)*CM-VxCS(1)*SM)+M21U*VxyM-M21T*VyM
            VxzM=VxzCS(1)*CM+VxzCS(2)*SM-M21T*VzM+M21U*VxzM
            VyzM=(VzCS(2)*CM-VzCS(1)*SM)*M+M21U0*VyzM
            IF(POLE) VxxM=VxxCS(1)*CM+VxxCS(2)*SM+M21*(U*(VxxM-VM)-T2*VxM)
            IF(NPOLE)VyyM=-(VCS(1)*CM+VCS(2)*SM)*M2+M21U0*VyyM
          endif

          VxM = VxCS(1)*CM   + VxCS(2)*SM - M21T*VM   + M21U*VxM
          VyM = M*(VCS(2)*CM - VCS(1)*SM) + M21U0*VyM
          VzM = VzCS(1)*CM   + VzCS(2)*SM + M21U*VzM
        endif
        VM = VCS(1)*CM + VCS(2)*SM + M21U*VM         !0th order
      enddo

*** COMPUTE GM/R

      S = CM3/r ; GPTDR = S*VM*invscale
      IF(.NOT.DERIV1) RETURN

*** COMPUTE GM/R**2

      S  = S/r
      G1 = S*(/VxM,VyM,VzM/)
      G1 = G1*invscale

c - changed back for geopot97 (but specifically for v0.2a):

      IF(.NOT.DERIV2) RETURN

*** COMPUTE GM/R**3

      S=S/R

*** HERE THE LAPLACE EQUATION IS USED

      IF (POLE) then
        VxxM = VxxM + VzM ; VyyM=-(VxxM+VzzM)
      else
        VyyM=VzM+(VyyM-T*VxM)/U ; VxxM=-(VzzM+VyyM)
      endif

c - changed for v0.2a of geopot97:

      G2(1,1)=VxxM*S

*** man. geod. patch

      G2(1,2)=S*VxyM*m21 ; G2(2,1)=G2(1,2)

c - changed for v0.2a of geopot97:

      G2(1,3)=S*(VxzM-VxM) ; G2(3,1)=G2(1,3)


c - changed for v0.2a of geopot97:

      G2(2,2)=VyyM*S ; G2(2,3)=S*(VyzM-VyM) ; G2(3,2)=G2(2,3)

c - das (see above, remember that CM3 currently is GM)
c - changed for v0.2a of geopot97:

      G2(3,3)=S*VzzM
      G2 = G2*invscale

      RETURN
      END
c-----------------------------------------------------------------------------
      SUBROUTINE INITAL

*** INITIALIZE COMMON BLOCKS

      IMPLICIT REAL*8 (A-H,O-Z),INTEGER*4(I-N)

      LOGICAL FIRST
      INTEGER*4 OLDORD

      COMMON/GPTCM/OLDT,OLDR,IZ,FIRST,OLDORD,I1,I2,I3,I4,I5,I6,I7,I8,I9,NMAXSV

c - Initialize block "GPTCM"

      IZ=0 ; FIRST=.FALSE.
      OLDT=0.D0 ;  OLDR=0.D0 ; OLDORD=0
      I1=0 ; I2=0 ; I3=0 ; I4=0 ; I5=0 ; I6=0 ; I7=0 ; I8=0 ; I9=0

      RETURN
      END
c----------------------------------------------------------------
      subroutine ell2sph(glat,glon,h,a,finv,slat,slon,r)

c - subroutine to convert ellipsoidal to spherical coordinates

c - glat,glon,h = ellipsoidal (input) coordinates
c - slat,slon,r = spherical (output) coordinates
c - a,finv = semi-major axis and inverse flattening of ellipsoid

      implicit real*8(a-h,o-z)

c - Convert to X,Y,Z first

      call ell2xyz(glat,glon,h,a,finv,x,y,z)

c - Convert X,Y,Z to spherical

      call xyz2sph(x,y,z,slat,slon,r)

c - Added because somehow the longitude got a sign mixed up

      slon = glon

      return
      end
c----------------------------------------------------------------------
      subroutine ell2xyz(glat,glon,h,a,finv,x,y,z)

c - subroutine to convert ellipsoidal to cartesian coordinates

c - glat,glon,h = ellipsoidal (input) coordinates
c - x,y,z = cartesian (output) coordinates
c - a,finv = semi-major axis and inverse flattening of ellipsoid

      implicit real*8(a-h,o-z)
      real*8     lamda

      common/block1/pi,d2r

      f = 1.d0/finv    ; e2    = (2.d0 - f)*f
      phi  = glat*d2r  ; lamda = glon*d2r
      sphi = dsin(phi) ; cphi  = dcos(phi)

      xw = dsqrt(1.d0-e2*sphi*sphi) ; xn = a/xw
      slam = dsin(lamda) ; clam = dcos(lamda)

      x = (xn+h)*cphi*clam ; y = (xn+h)*cphi*slam ; z = (xn*(1.d0-e2)+h)*sphi

      return
      end
c-----------------------------------------------------------------------
      subroutine xyz2sph (x,y,z,slat,slon,r)

c - subroutine to convert cartesian coordinates to spherical

      implicit real*8(a-h,o-z)

      common/block1/pi,d2r

c - x,y,z = cartesian coordinates
c - slat,slon,r = spherical coordinates

      p    = dsqrt(x*x + y*y) ; r    = dsqrt(p*p + z*z)
      slon = datan2(y,x)/d2r  ; slat = datan2(z,p)/d2r

      return
      end
c-------------------------------------------------------------------------
      subroutine bestfit(gmcoef,omega,abf,j2bf,finvbf,w0,tidbf,tidcoef)

c - subroutine to "define" the geoid by 1 of 4 methods:
c   1) Define the best fitting ellipsoid to the geoid, in the
c      same tide system as the coefficients, by giving abf and finvbf
c   2) Define the best fitting ellipsoid to the geoid, in the
c      same tide system as the coefficients, by giving abf and j2bf
c      (j2 can be given, since we have gmcoef and omega)
c   3) Define the best fitting ellipsoid to the geoid, in the
c      same tide system as the coefficients, by giving abf and bbf
c   4) Define the gravity potential value on the geoid, Wo

      implicit real*8(a-h,o-z)

      real*8 j2bf
      character*4 tidbf,tidcoef
      character   string*80

      write(6,1)
    1 format(/1x,'To properly use the coefficients in combination',/,
     *       1x,'with a reference field, you *must* define the',/,
     *       1x,'geoid in one of 4 ways:',/,
     *       1x,' 1) Define a best-fitting ellipsoid to the geoid',/,
     *       1x,'    using a and f, and knowing the tide system.',/,
     *       1x,' 2) Define a best-fitting ellipsoid to the geoid',/,
     *       1x,'    using a and J2, and knowing the tide system.',/,
     *       1x,' 3) Define a best-fitting ellipsoid to the geoid',/,
     *       1x,'    using a and b, and knowing the tide system.',/,
     *       1x,' 4) Define the gravity potential on the geoid, Wo')
      write(6,2)
    2 format(/1x,'Which way do you wish to define the geoid? ',$)
      read(5,*)ians
      string = ' '
      write (string,*) ians
      call log (string)
      if(ians.le.0 .or. ians.ge.5)stop

c - Get a/f, a/b  or a/j2

      if(ians.eq.1 .or. ians.eq.2 .or. ians.eq.3)then
        write(6,3)
    3   format(1x,'Input the best equatorial radius (a): ',$)
        read(5,*)abf
        string = ' '
        write (string,*) abf
        call log (string)

        if(ians.eq.1)then
          write(6,4)
    4     format(1x,'Input the best inverse flattening (1/f): ',$)
          read(5,*)finvbf
          string = ' '
          write (string,*) finvbf
          call log (string)
          bbf = abf*(1.d0 - (1.d0/finvbf))
          call getj2(abf,gmcoef,omega,bbf,j2bf)

        elseif(ians.eq.2)then
          write(6,7)
    7     format(1x,'Input the best J2: ',$)
          read(5,*)j2bf
          string = ' '
          write (string,*) j2bf
          call log (string)
          call getb(abf,j2bf,gmcoef,omega,bbf)
          finvbf = abf / (abf - bbf)

        elseif(ians.eq.3)then
          write(6,9)
    9     format(1x,'Input the best semi-major axis (b): ',$)
          read(5,*)bbf
          string = ' '
          write (string,*) bbf
          call log (string)
          finvbf = abf / (abf - bbf)
          call getj2(abf,gmcoef,omega,bbf,j2bf)

        endif

c - Get tide system of best-fit ellipsoid

        write(6,5)
    5   format(1x,'Which tide system is this best ellipsoid in?',/,
     *       1x,' 1) = Mean',/,
     *       1x,' 2) = Zero',/,
     *       1x,' 3) = Non-')
        read(5,*)ians2
        string = ' '
        write (string,*) ians2
        call log (string)
        if(ians2.le.0 .or. ians2.ge.4)stop
        if(ians2.eq.1)tidbf = 'mean'
        if(ians2.eq.2)tidbf = 'zero'
        if(ians2.eq.3)tidbf = 'non-'

c - Deal with possibly differing tide systems

c - Steps: 1) Transform abf and bbf into the tide system of
c             the coefficients (abft, bbft)
c          2) Calculate Uo from abft,bbft,gmcoef,omega
c          3) Set Wo = Uo
c - Step 1 is only performed if tidbf is not tidcoef

c - Side note to myself...the transformation should NOT
c - be necessary, as Uo is identical for all three tide
c - systems, given the same GM/omega and the properly
c - transformed a/b values!  But, for now, leave it
c - in.

        if(tidbf.ne.tidcoef)then
          write(6,6)
    6     format(1x,
     *'Your best-fit ellipsoid is in a different tide system than'/
     *'the coefficients.  Transforming your ellipsoid to the tide'/
     *'system of the coefficients...')
          call fixab(abf,bbf,tidbf,tidcoef,abft,bbft)
        else
          abft = abf
          bbft = bbf
        endif

        call getu0(abft,gmcoef,omega,bbft,u0)

        W0 = u0
        return

c - Get Wo directly, rather than a/f, a/b or a/j2

      elseif(ians.eq.4)then
        write(6,8)
    8   format(1x,'Input the Wo value for the geoid: ',$)
        read(5,*) W0
        string = ' '
        write (string,*) W0
        call log (string)

c - The "tide system of Wo" is irrelevant.  Consider this:
c - Given GM/omega/a/b in the mean tide system, you get a
c - Uo value.  Then if you transform to the zero or the
c - non-tidal system (changing a and b), you get almost
c - EXACTLY the same Uo value!!! (To within .001 m**s/s**2)
c - As such, the tide system is NOT tied to the potential
c - value, but just to the SHAPE of the ellipsoid/geoid.

      endif

      return
      end
c--------------------------------------------------------------------------------
      subroutine gettid(tidsum,tidcoef)

c - Subroutine to define the tide system for the summation
c - and thus the output (irregardless of what tide system
c - the coefficients/normal field/best fitting ellipsoid are in).

      character*4 tidsum,tidcoef
      character   string*80

      write(6,1)tidcoef
    1 format(/1x,'The tide system for any output quantities can '/
     *       1x,'differ from that of the coefficients and/or the '/
     *       1x,'normal field (if used).  Therefore, what tide'/
     *       1x,'system shall all values be computed in?'/
     *       1x,'1: mean',/,1x'2: zero',/,1x,'3: non-',/,
     *       1x,'(The coefficients are in the ',a4,' tide system)')
      read(5,*)ians
      string = ' '
      write (string,*) ians
      call log (string)

      if(ians.le.0 .or. ians.ge.4)stop
      if(ians.eq.1)tidsum = 'mean'
      if(ians.eq.2)tidsum = 'zero'
      if(ians.eq.3)tidsum = 'non-'
      return
      end
c----------------------------------------------------------------------------------
      subroutine fixc20 (tidcoef,tidsum,c20,gm,a)

c - subroutine to modify the C(2,0) term because the tide
c - system that C(2,0) is in differs from the tide system
c - requested for output

      implicit real*8 (a-h,o-z)

      real*8      kto,kfrom
      character*4 tidcoef,tidsum
      character   string*80

      write(6,3)tidcoef ; write(6,4)tidsum
    3 format(/,1x,50('*'),/,
     *        1x,'The tide system of your coefficients is: ',a4,
     *' tide')
    4 format(1x,'You want the potential computed in the : ',a4,
     *' tide system,',/,1x,'requiring a transformation of C(2,0)')

      if(tidsum.eq.'non-')then
        write(6,1)
    1   format(1x,'To transform *to* the non-tidal system, you'/
     *         1x,'must input a value of the k Love number'/
     *         1x,'(often taken to be k = 0.3) : ',$)
        read(5,*)kto
        string = ' '
        write (string,*) kto
        call log (string)
      endif
      if(tidcoef.eq.'non-')then
        write(6,2)
    2   format(1x,'To transform *from* the non-tidal system, you'/
     *         1x,'must input the value of k which was used to'/
     *         1x,'put the coefficients in the non-tidal system'/
     *         1x,'initially (often k = 0.3, but DO NOT GUESS!): ',$)
        read(5,*)kfrom
        string = ' '
        write (string,*) kfrom
        call log (string)
      endif

      if(tidcoef.ne.'non-' .and. tidsum.ne.'non-')then
        write(6,5)
    5   format(1x,'Since you are neither going to nor from the',/,
     *         1x,'non-tidal system, the transformation is simple',/,
     *         1x,'and does not require the k Love number')
      endif

      if(tidsum.eq.'non-')then
        if(tidcoef.eq.'mean')then
          conv = 1.d0 + kto
        elseif(tidcoef.eq.'zero')then
          conv = kto
        endif
      elseif(tidsum.eq.'zero')then
        if(tidcoef.eq.'non-')then
          conv = -kfrom
        elseif(tidcoef.eq.'mean')then
          conv = 1.d0
        endif
      elseif(tidsum.eq.'mean')then
        if(tidcoef.eq.'non-')then
          conv = -(1.d0 + kfrom)
        elseif(tidcoef.eq.'zero')then
          conv = -1.d0
        endif
      endif

c     s5  = 1.d0/dsqrt(5.d0)
c     tot = -0.198d0*s5*gamma*r*r*r/gm/a/a


      write(6,6)
    6 format(/1x,'For now, the conversion factor is HARD-CODED',/,
     *'as -1.39844d-8 x 1 or 1+k or k.  If this is bad, ',/,
     *'you must get in the code and re-compile.')

      tot = -3.127007d-8               !-1.39844d-8*sqrt(5)
      c20 = c20 - conv*tot

      return
      end
c------------------------------------------------------------------------
      subroutine getu0(a,gm,omega,b,u0)

c - subroutine to compute Uo from a,GM,omega and b

      implicit real*8(a-h,o-z)

      bige = dsqrt(a**2 - b**2)

c - H/M (exact formula), 2-61, p. 67

      u0 = (gm/bige)*datan2(bige,b) + (omega**2*a**2)/3

      return
      end
c------------------------------------------------------------------------------------
      subroutine getb (a,j2,gm,omega,b)

c - This subroutine takes, as input, the normal gravity field
c - variables of a, j2, gm and omega, and iterates to an
c - "exact" solution of "b" (where the iteration stops
c - when the difference between the b-implied "j2" value
c - and the true "j2" value is less than "tol", in absolute
c - value.  "tol" is hard-coded as 1.d-15
c -
      implicit real*8(a-z)

      step = 100.d0 ; tol = 1.d-15 ; idum2 = 0
      fp    = 1.5d0*j2 + 0.5d0*omega**2*a/9.8d0 ; bp = a*(1.d0 - fp)
      c1 = -(1.d0/3/a**2/gm) ; c2 = 2.d0*(omega*a)**2/15.d0 ; c3 = gm
      j2old = j2

    1 bigep = dsqrt(a**2 - bp**2)
      x1=1.d0+3.d0*bp**2/bigep**2 ; x2=x1*datan2(bigep,bp) ; x3=x2-3.d0*bp/bigep
      q0p = 0.5d0*x3
      j2p = c1*(c2*bigep**3/q0p - c3*bigep**2)
      dj2 = j2old - j2p
      if(dabs(dj2).lt.tol)goto 95
      dj2 = j2 - j2p
      if(dj2.lt.0)then
        idum = +1
        if(idum.ne.idum2)step = 0.5d0*step
        bp = bp + step
      else
        idum = -1
        if(idum.ne.idum2)step = 0.5d0*step
        bp = bp - step
      endif

      j2old = j2p
      idum2 = idum

      goto 1

   95 b = bp

      return
      end
c----------------------------------------------------------------------------------
      subroutine getj2(a,gm,omega,b,j2)

      implicit real*8(a-z)

c - This subroutine calcultes, non-iteratively, the value
c - of J2 implied by 4 parameters of the normal field
c - (a,gm,omega and b).

      bigE = dsqrt(a**2 - b**2) ; ep = bigE/b ; epinv = 1.d0/ep ; e2 = bigE**2/a**2
      q0 = 0.5d0*( (1.d0+3*epinv**2)*datan2(bigE,b) - 3*epinv)
      m = omega**2*a**2*b/gm
      j2 = (e2/3.d0)*(1.d0 - (2.d0/15.d0)*m*ep/q0)

      return
      end
c------------------------------------------------------------------------------------
      subroutine fixab(abf,bbf,tidbf,tidcoef,abft,bbft)

c - Subroutine to convert abf/bbf (a and b of the
c - ellipsoid in the "tidbf" tide system, to abft/bbft
c - (a and b of the ellipsoid in the "tidcoef" tide system).
c - That is: transform the best fit ellipsoid to the tide
c -          system of the coefficients

      implicit real*8(a-h,o-z)

      real*8      kto,kfrom
      character*4 tidbf,tidcoef
      character   string*80

      write(6,3)tidcoef ; write(6,4)tidbf
    3 format(1x,'The tide system of your coefficients is: ',a4,' tide')
    4 format(1x,'You gave the ellipsoid in the: ',a4,
     *' tide system,',/,1x,'requiring a transformation of a & b')

      if(tidbf.eq.'non-')then
        write(6,1)
    1   format(1x,'To transform *to* the non-tidal system, you'/
     *         1x,'must input a value of the k Love number'/
     *         1x,'(often taken to be k = 0.3) : ',$)
        read(5,*)kto
        string = ' '
        write (string,*) kto
        call log (string)
      endif
      if(tidcoef.eq.'non-')then
        write(6,2)
    2   format(1x,'To transform *from* the non-tidal system, you'/
     *         1x,'must input the value of k which was used to'/
     *         1x,'put the coefficients in the non-tidal system'/
     *         1x,'initially (often k = 0.3, but DO NOT GUESS!): ',$)
        read(5,*)kfrom
        string = ' '
        write (string,*) kfrom
        call log (string)
      endif

      if(tidcoef.ne.'non-' .and. tidbf.ne.'non-')then
        write(6,5)
    5   format(1x,'Since you are neither going to nor from the',/,
     *         1x,'non-tidal system, the transformation is simple',/,
     *         1x,'and does not require the k Love number')
      endif

      if(tidbf.eq.'non-')then
        if(tidcoef.eq.'mean')then
          conv = -(1.d0 + kto)
        elseif(tidcoef.eq.'zero')then
          conv = -kto
        endif
      elseif(tidbf.eq.'zero')then
        if(tidcoef.eq.'non-')then
          conv = kfrom
        elseif(tidcoef.eq.'mean')then
          conv = -1.d0
        endif
      elseif(tidbf.eq.'mean')then
        if(tidcoef.eq.'non-')then
          conv = 1.d0 + kfrom
        elseif(tidcoef.eq.'zero')then
          conv = 1.d0
        endif
      endif

      abft = abf - (0.099d0)*conv ; bbft = bbf + (0.198d0)*conv

      return
      end
c--------------------------------------------------------------------------------

      subroutine dist(c,max,nmax,gmcoef,acoef,jnorm,njmax,gmnorm,anorm)

c - subroutine to compute disturbing potential coefficients, based
c - on total coefs, normal coefs, and the GM/a values associated
c - with them both.  It is assumed that total/normal coefs are in
c - an identical tide system, whatever that system may be.
c - Also, the TOTAL (c) coefficients, are coming in as
c - quasi-normlized.  The NORMAL (jnorm) coefficients are coming
c - in as unnormalized, and of opposite sign as c.  We need
c - to make the jnorm coefficients of the right sign, and
c - quasinormalized, before subtracting.

      implicit real*8(a-h,o-z)

      real*8 j0

      real*8 c(0:(max+1)*(max+1)-1)
      real*8 jnorm(njmax)

c - Do C(0,0) - scale*J(0) first:
c - J(0) is the same, whether conventional, fully normalized
c - or quasi-normalized (= 1.d0)

      j0   = 1.d0 ; scale = (gmnorm/gmcoef)*1.d0 ! the 1.d0 = (anorm/acoef)**0
      c(0) = c(0) - j0*scale

c     c(0) = 0.d0     !Remove later, was set = 0 to compare non-tidal everything with the NGA program.
c - Now do the even zonals from 2 to 2*njmax, transforming the
c - jnorm values into fully-normalized values to be consistant
c - with the c(*) values.

      do i=1,njmax
        n = i*2
        scale = (gmnorm/gmcoef) * (anorm/acoef)**n

c - Converts from J2 (conventional) to J2(fully normalized)

        xnorm1 = -1.d0/dsqrt(2.d0*n + 1.d0)

c - Converts from J2 (fully normalized) to J2 (quasi-normalized)

        xnorm2 = dsqrt(2.d0*n + 1.d0)

c - Thus, J2(quasi-normalized) = J2(conventional)*(-1.d0) (xnorm1*xnorm2 = -1.d0)

        xnorm   = xnorm1*xnorm2  ! = -1.d0
        isto    = n*n   !  <- Just happens to be this way
        c(isto) = c(isto) - scale*(xnorm)*jnorm(i)
      enddo

      return
      end
c-----------------------------------------------------------------------------------
      subroutine undist(c,max,nmax,gmcoef,acoef,jnorm,njmax,gmnorm,anorm)

c - subroutine to UNDISTURB the coefficients (basically
c - un-does the dist(*) subroutine

      implicit real*8(a-h,o-z)

      real*8 j0
      real*8 c(0:(max+1)*(max+1)-1)
      real*8 jnorm(njmax)

c - Do C(0,0) first:
c - J(0) is the same, whether conventional, fully normalized
c - or quasi-normalized (= 1.d0)

      j0   = 1.d0 ; scale = (gmnorm/gmcoef)*1.d0 ! the 1.d0 = (anorm/acoef)**0
      c(0) = c(0) + j0*scale

c - Now do the even zonals from 2 to 2*njmax, transforming the
c - jnorm values into fully-normalized values to be consistant
c - with the c(*) values.

      do i=1,njmax
        n = i*2
        scale = (gmnorm/gmcoef) * (anorm/acoef)**n

c - Converts J2 (conventional) to J2(fully normalized)

        xnorm1 = -1.d0/dsqrt(2.d0*n + 1.d0)

c - Converts J2 (fully normalized) to J2(quasi-normalized)

        xnorm2 = dsqrt(2.d0*n + 1.d0)

c - Thus, J2(quasi-normalized) = J2(conventional)*(-1.d0) (xnorm1*xnorm2 = -1.d0)

        xnorm   = xnorm1*xnorm2 ! = -1.d0
        isto    = n*n  !  < - Just happens to be this way
        c(isto) = c(isto) + scale*(xnorm)*jnorm(i)
      enddo

      return
      end
c----------------------------------------------------------------------------------
      subroutine getgrid(glamn,glomn,dla,dlo,nla,nlo,ikind)

      implicit real*8(a-h,o-z)
      character   string*80

      write(6,1)
    1 format(1x,
     *'Input south geodetic latitude boundary (degrees): ',$)
      read(5,*)glamn
      string = ' '
      write (string,*) glamn
      call log (string)
      write(6,2)
    2 format(1x,
     *'Input north geodetic latitude boundary (degrees): ',$)
      read(5,*)glamx
      string = ' '
      write (string,*) glamx
      call log (string)
      write(6,3)
    3 format(1x,'Input latitude grid spacing (minutes): ',$)
      read(5,*)dlamin
      string = ' '
      write (string,*) dlamin
      call log (string)

      write(6,4)
    4 format(1x,
     *'Input west geodetic longitude boundary (degrees): ',$)
      read(5,*)glomn
      string = ' '
      write (string,*) glomn
      call log (string)
      write(6,5)
    5 format(1x,
     *'Input east geodetic longitude boundary (degrees): ',$)
      read(5,*)glomx
      string = ' '
      write (string,*) glomx
      call log (string)
      write(6,6)
    6 format(1x,'Input longitude grid spacing (minutes): ',$)
      read(5,*)dlomin
      string = ' '
      write (string,*) dlomin
      call log (string)

      dla = dlamin/60.d0 ; dlo = dlomin/60.d0
      nla = nint((glamx-glamn)/dla + 1.d0) ; nlo = nint((glomx-glomn)/dlo + 1.d0)
      ikind = 1

      return
      end
c---------------------------------------------------------------------------------------
      subroutine getgrid2(glamn,glomn,dla,dlo,nla,nlo,ikind)

      implicit real*8(a-h,o-z)
      character   string*80

      write(6,1)
    1 format(1x,
     *'Input south spherical latitude boundary (degrees): ',$)
      read(5,*)glamn
      string = ' '
      write (string,*) glamn
      call log (string)
      write(6,2)
    2 format(1x,
     *'Input north spherical latitude boundary (degrees): ',$)
      read(5,*)glamx
      string = ' '
      write (string,*) glamx
      call log (string)
      write(6,3)
    3 format(1x,'Input latitude grid spacing (minutes): ',$)
      read(5,*)dlamin
      string = ' '
      write (string,*) dlamin
      call log (string)

      write(6,4)
    4 format(1x,
     *'Input west spherical longitude boundary (degrees): ',$)
      read(5,*)glomn
      string = ' '
      write (string,*) glomn
      call log (string)
      write(6,5)
    5 format(1x,
     *'Input east spherical longitude boundary (degrees): ',$)
      read(5,*)glomx
      string = ' '
      write (string,*) glomx
      call log (string)
      write(6,6)
    6 format(1x,'Input longitude grid spacing (minutes): ',$)
      read(5,*)dlomin
      string = ' '
      write (string,*) dlomin
      call log (string)

      dla = dlamin/60.d0 ; dlo = dlomin/60.d0
      nla = nint((glamx-glamn)/dla + 1.d0) ; nlo = nint((glomx-glomn)/dlo + 1.d0)
      ikind = 1

      return
      end
c-----------------------------------------------------------------------------------
      double precision function gam(glat,a,b,ge,gp)

      implicit real*8(a-h,o-z)

      common/block1/pi,d2r

      phi = glat*d2r
      c = dcos(phi) ; s = dsin(phi)
      c2 = c*c ; s2 = s*s
      gam = (a*ge*c2 + b*gp*s2 )/ dsqrt(a*a*c2 + b*b*s2)

      return
      end
c-------------------------------------------------------------------------------------
      double precision function dgdh(glat,a,finv,gamma,omega)
      implicit real*8(a-h,o-z)

      common/block1/pi,d2r

      f = 1.d0/finv ; e2 = (2.d0 - f)*f

      s2 = dsin(glat*d2r)**2
      xw = dsqrt(1.d0 - e2*s2) ; xm = a*(1.d0 - e2)/xw/xw/xw ; xn = a/xw
      xj = 0.5d0*((1.d0/xm) + (1.d0/xn))

c - H/M, p 70, eqn 2-79

      dgdh = -2.d0*(gamma*xj + omega*omega)

      return
      end
c-----------------------------------------------------------------------------------
      double precision function d2gdh2(gamma,a)

      implicit real*8(a-h,o-z)

      d2gdh2 = 6.d0*gamma/a/a

      return
      end
c-----------------------------------------------------------------------------------
      double precision function pnm (n,m,xlat)

      implicit real*8(a-h,o-z)

      common/block1/pi,d2r

      t = dsin(xlat*d2r)
      xm2 = 0.5d0*dble(m)

      if(mod((n-m),2).eq.0)then
        ir = (n-m)/2
      else
        ir = (n-m-1)/2
      endif

      cons = ((1.d0-t*t)**xm2)/(2.d0**n)

      xsum = 0.d0
      do k=0,ir
        xnum = fact(2*n - 2*k)
        xden = fact(k)*fact(n-k)*fact(n-m-2*k)
        sca  = (-1.d0)**k * t**(n-m-2*k)
        xsum = xsum + sca*xnum/xden
      enddo
      pnm = cons*xsum

      write(6,*) ' n,m,xlat,pnm = ',n,m,xlat,pnm

      return
      end
c----------------------------------------------------------------------------------
      double precision function fact(n)

      fact = 1.d0
      if(n.eq.0)return

      do i=1,n
        fact = fact*i
      enddo

      return
      end
c---------------------------------------------------------------------------------
      double precision function gamh(a,b,gm,omega,h,xlat,gamma)

c - Function to compute, through a closed form, the
c - normal gravity vector magnitude at an elevation h.
c - This closed form was tested 1/29/97 by D. Smith
c - against both Clenshaw summation, and pure Legendre
c - summation and found to agree to about 0.01 mgals at
c - various elevations (up to 3000 meters).  The Clenshaw
c - and pure Legendre summations agreed to 16 decimal places.
c - This function based on equation 2-123 of H/M, p. 79.

      implicit real*8(a-h,o-z)

      real*8 m

      common/block1/pi,d2r

      f = (a-b)/a ; m = omega*omega*a*a*b/gm
      gamh = gamma*(1.d0 - 2.d0/a*(1.d0 + f + m - 2*f*dsin(xlat*d2r)**2)*h
     *                   + 3.d0/a/a*h*h)
      return
      end
c---------------------------------------------------------------------------------
      subroutine fixab2 (anorm,bnorm,tidnorm,tidsum,afixed,bfixed)

c - Subroutine to convert anorm/bnorm (a and b of the
c - ellipsoid in the "tidnorm" tide system, to afixed/bfixed
c - (a and b of the ellipsoid in the "tidsum" tide system).
c - That is: transform the normal field ellipsoid to the tide
c -          system requested for output.

      implicit real*8(a-h,o-z)

      real*8      kto,kfrom
      character*4 tidnorm,tidsum
      character   string*80

      write(6,3)tidsum ; write(6,4)tidnorm
    3 format(1x,'The tide system you requested for output is: ',a4,
     *' tide')
    4 format(1x,'You gave the normal field in the: ',a4,
     *' tide system,',/,1x,'requiring a transformation of a & b')

      if(tidnorm.eq.'non-')then
        write(6,1)
    1   format(1x,'To transform *to* the non-tidal system, you'/
     *         1x,'must input a value of the k Love number'/
     *         1x,'(often taken to be k = 0.3) : ',$)
        read(5,*)kto
        string = ' '
        write (string,*) kto
        call log (string)
      endif
      if(tidsum.eq.'non-')then
        write(6,2)
    2   format(1x,'To transform *from* the non-tidal system, you'/
     *         1x,'must input the value of k which was used to'/
     *         1x,'put the coefficients in the non-tidal system'/
     *         1x,'initially (often k = 0.3, but DO NOT GUESS!): ',$)
        read(5,*)kfrom
        string = ' '
        write (string,*) kfrom
        call log (string)
      endif

      if(tidsum.ne.'non-' .and. tidnorm.ne.'non-')then
        write(6,5)
    5   format(1x,'Since you are neither going to nor from the',/,
     *         1x,'non-tidal system, the transformation is simple',/,
     *         1x,'and does not require the k Love number')
      endif

      if(tidnorm.eq.'non-')then
        if(tidsum.eq.'mean')then
          conv = -(1.d0 + kto)
        elseif(tidsum.eq.'zero')then
          conv = -kto
        endif
      elseif(tidnorm.eq.'zero')then
        if(tidsum.eq.'non-')then
          conv = kfrom
        elseif(tidsum.eq.'mean')then
          conv = -1.d0
        endif
      elseif(tidnorm.eq.'mean')then
        if(tidsum.eq.'non-')then
          conv = 1.d0 + kfrom
        elseif(tidsum.eq.'zero')then
          conv = 1.d0
        endif
      endif

      afixed = anorm - (0.099d0)*conv ; bfixed = bnorm + (0.198d0)*conv

      return
      end
c-------------------------------------------------------------------------------------
      subroutine getgeom (anorm,finvnorm)

c - Subroutine to get a and 1/f of some geometric ellipsoid

      implicit real*8(a-h,o-z)
      character   string*80

      write(6,1)
    1 format(1x,'How is the ellipsoid described? '/
     *       1x,'1 = a and f'/1x,'2 = a and b'/$)
      read(5,*)ians
      string = ' '
      write (string,*) ians
      call log (string)
      if(ians.le.0 .or. ians.ge.3)stop

      write(6,2)
    2 format(1x,'Enter the equatorial radius, a = ',$)
      read(5,*) anorm
      string = ' '
      write (string,*) anorm
      call log (string)
      if(ians.eq.1)then
        write(6,3)
    3   format(1x,'Enter the INVERSE flattenting, 1/f = ',$)
        read(5,*)finvnorm
        string = ' '
        write (string,*) finvnorm
        call log (string)
      else
        write(6,4)
    4   format(1x,'Enter the semi-MINOR axis, b = ',$)
        read(5,*)bnorm
        string = ' '
        write (string,*) bnorm
        call log (string)
        finvnorm = anorm/(anorm - bnorm)
      endif

      return
      end
c------------------------------------------------------------------------------------
      subroutine getmsl (dorth,b88)

c - Subroutine to find a bias to ADD to H(DTED) to get H(true)
c - H(DTED) + dorth = H(true)

      implicit real*8(a-h,o-z)
      character*1 yesno
      character   string*80


    2 write(6,1)
    1 format(1x,'Which system are your orthometric heights in?',/
     *       1x,'1 = Best global mean sea level',/
     *       1x,'2 = NAVD 88',/
     *       1x,'3 = other',/,1x,' ',$)
      read(5,*)ians
      string = ' '
      write (string,*) ians
      call log (string)
      if(ians.lt.1 .or. ians.gt.3)goto 2
      if(ians.eq.1)then
        dorth = 0.d0
        return
      endif
      if(ians.eq.2)then
        write(6,3)b88*100
    3   format(/1x,'As of 2/6/97, the bias in NAVD 88 is believed'/
     *         1x,'to be ',f5.1,' cm.  Shall I apply this bias '/
     *         1x,'to your DTED? :',$)
        read(5,'(a)')yesno
        string = ' '
        write (string,*) yesno
        call log (string)
        if(yesno.eq.'y' .or. yesno.eq.'Y')then
          dorth = b88
          return
        endif
      endif

      write(6,4)
    4 format(1x,'What bias value (meters) shall I add to your'
     *       1x,'DTED to make it agree with best global mean'
     *       1x,'sea level? : ',$)
      read(5,*) dorth
      string = ' '
      write (string,*) dorth
      call log (string)
      return
      end
c-------------------------------------------------------------------------------------
      subroutine getdgtyp(idgtyp)
      implicit real*8(a-h,o-z)
      character   string*80
      write(6,1)
    1 format(/1x,'The following are ways in which I can',/,
     *       1x,'calculate the gravity anomaly.  You need',/,
     *       1x,'to pick which way:')
      write(6,2)
    2 format(1x,'# 1: ',/,
     *       1x,' anomaly = - dT/dr - 2*T/r + 2*delta_W / r',/,
     *       1x,'Advantages:  Is 100% compatable with the ',/,
     *       1x,'  spherical harmonic representation of ',/,
     *       1x,'  geoid undulations if you pump this formula',/,
     *       1x,'  through the Stokes integral.',/
     *       1x,'Disadvantages: This is the spherical approximation',/,
     *       1x,'  to gravity anomalies, and is less compatable with',/,
     *       1x,'  real data than other formulas for computing the',/,
     *       1x,'  gravity anomaly.')
      write(6,3)
    3 format(1x,'# 2: ',/,
     *       1x,' anomaly = -dT/dh + T*(dgamma/dh)/gamma',/,
     *       1x,'            - delta_W*(dgamma/dh)/gamma',/,
     *       1x,'Advantages:  Is a more accurate anomaly than the',/,
     *       1x,'  spherical approximation',/,
     *       1x,'Disadvantages:  Is not compatable with the ',/,
     *       1x,'  spherical harmonic representation of geoid ',/,
     *       1x,'  undulations if you pump this formula through ',/,
     *       1x,'  Stokes integral.')
    5 write(6,4)
    4 format(/,1x,' Which gravity anomaly computation method? ',$)
      read(5,*) idgtyp
      string = ' '
      write (string,*) idgtyp
      call log (string)
      if(idgtyp.lt.1 .or. idgtyp.gt.2)goto 5

      return
      end
C---------------------------------------------------------------------------
      subroutine log (name)

      implicit none
      integer*4  n,n1
      character  name*80,char*80,format1*6

      call remove_leading_spaces_or_tabs (name)
      call extract_name (name,n)
      write (char,'(i3)') n
      call remove_leading_spaces_or_tabs (char)
      call extract_name (char,n1)
      format1 = '(a'//char(1:n1)//')'
      write (101,format1) name(1:n)

      return
      end
C----------------------------------------------------------------------------
      subroutine remove_leading_spaces_or_tabs(line)

      implicit none

      integer*4   i,m
      character   line*80,temp*80

      temp = ' '
      m = 0
      do i=1,80
        if (line(i:i) /= ' ' .and. line(i:i) /= '       ') then  !space or tab
          m = m + 1
          temp(m:m) = line(i:i)
        endif
      enddo
      line(1:m) = temp(1:m)
      line(m+1:80) = ' '

      return
      end
C----------------------------------------------------------------------------
      subroutine extract_name (name,n)

      implicit none

      integer*4 i,n
      character old_name*80,name*80,char*1

      n = 0
      old_name = ' '
      old_name = name
      do i=1,80
        char = old_name(i:i)
        if (char /= ' ') then
          n = n + 1
          name(n:n) = char
        endif
      enddo

      return
      end

