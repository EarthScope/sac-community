#!/bin/bash

COM="html/commands"

DIRS="${COM}/sss.com ${COM}/spe.com ${COM}  html/manual html "

for z in $DIRS; do
    echo $z
    WD=$( pwd )
    cd $z
    \rm -f *.txt *.html *.aux *.log *.tex *.toc *.out sac_manual.pdf
    cd $WD
done
cd html
pwd
cp -p sac_manual.tex.in sac_manual.tex
cp -p sac_spectrogram.pdf.in sac_spectrogram.pdf
