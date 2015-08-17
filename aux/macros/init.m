* SAC macro to initialize environment each time SAC is started.
* Modify the following lines or add other commands to personalize it.
* If there are macros you want to call from sac in /usr/local/macros ...
setmacro /usr/local/macros
* Put this file in that directory.
* To apply, alias sac '${SACHOME}/bin/sac /usr/local/macros/init.m' and enter sac
*  Here are some possible settings
lh columns 2 files none; qdp 10000 ; xdiv power off ; xlabel 'Time (sec)'
transcript history file ./.sachist
*
* Uncomment one of the following lines to try different plot window sizes
*window 1 x 0.25 0.75 y 0.28 0.95
*window 1 x 0.15 0.95 y 0.40 0.95
* Next line is for a 22:17 aspect ratio on 16:10 monitor
* window 4 x 0.2 0.8 y 0.21 0.95 
* 22:17 for 4:3 monitor  
* window 5 x 0.2 0.8 y 0.33 0.95

