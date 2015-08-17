* Macro plotone.m  Displays all waveforms in directory
*   with a chosen ending string (such as .sac) one at a time.
* Syntax:  m plotone.m .sac     called from within SAC.
* Cull bad waveforms in a directory - Reference is INLINE Help file
* Pauses after each waveform and then queries if the waveform
*   is okay or bad.  If bad, respond with an x.  In v101.6
*   just entering a carriage return returns okay.  Prior
*   versions required a keyed resopnse.
* The window with the responses should not overlap the plot
*   window.  The pausing is necessary to allow control to
*   move from the plot just displayed to the SAC> window.
* If the directory bad-files does not exist, it is created.
*
bd x
qdp 3000;xdiv power off
sc test -e bad-files || mkdir bad-files
do file wild *$1
  r $file
  rmean
  p1
  pause
  SETBB RESPONSE (REPLY "Enter x if bad [okay] ")
  IF %RESPONSE EQ "x" 
    sc mv $file bad-files/.
  endif
enddo
