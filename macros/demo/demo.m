*
* PURPOSE:  To provide a short introductory demo to SAC.
*

  echo off
  message ' '
  message 'See sac/macros/demo/README for an overview'
  message ' '
  sc \rm -R -f results
  sc mkdir results
  transcript create file ./results/demo_run.txt contents all
  window 1 y .01 .55
  xdiv power off ; xlabel 'Time (sec)'
  echo on
  macro read.m dir ./data/
  echo on
  macro plot.m dir ./data/
  echo on
  macro filter.m dir ./data/
  message 'macro demo.m is finished'
