#!/bin/bash

#CSS="--stylesheet=../css/html2css1.css --link-stylesheet"
TPC=" --template=../tmpl/template.txt"
TPM=" --template=../tmpl/template_manual.txt"
TEX="--documentoptions=10pt,letter"

LINK_SS="--link-stylesheet"

PROG=$( basename $0 )

RST2HTML=""

# Check for a variety of 
for z in rst2html.py rst2html-2.7.py rst2html-3.5.py rst2html-3.6.py; do
    cmd="$(command -v $z)"
    retval=$?
    echo "Checking $z: " $cmd
    if [ $retval == 0 ]; then
        RST2HTML=$z
    fi
done

N=1

function usage {
    echo ""
    echo "Usage: help2html.sh [-pdf] [all | file]"
    echo ""
    echo "       Convert SAC help files to html files "
    echo "       all  - Convert everything from the aux/help directory"
    echo "       file - Convert a single help file"
    echo "              Replace file with relative path from this directory"
    echo "              Example: for fft, file would be ../aux/help/fft"
    echo "       -pdf  Convert all of file(s) instead to TeX format"
    echo "       "      
}

function msg {
    echo  "$PROG: $1"
}


function help2txt {
    IN=$1
    OUT=$2
    LINKS=$3
    DIR="$4"
    # echo "help2txt: $IN -> $OUT"
    if [ ! -e $IN ]; then
        msg "Error finding input help file: ${IN}"
        return
    fi
    WD=$( pwd )
    cd $DIR
    if [ ! -e $LINKS ]; then
        msg "Error finding links file: ${LINKS}"
        cd $WD
        return
    fi
    cd $WD

    cp $IN $OUT
    echo "" >> $OUT
    echo ".. include:: $LINKS" >> $OUT
    echo "" >> $OUT
}


function txt2html {
    DIR=$1
    TXT=$2
    HTML=$3
    TEMPLATE=$4
    CSS=$5
    # echo "txt2html: $TXT -> $HTML [ $DIR ]"
    WD=$( pwd )
    cd $DIR
    #if [ ! -e ${CSS} ]; then
    #    msg "Error finding CSS Stylesheet: ${CSS}"
    #    cd $WD
    #    return
    #fi
    if [ ! -e ${TEMPLATE} ]; then
        msg "Error finding HTML Template: ${TEMPLATE}"
        cd $WD
        return
    fi
    if [ ! -e ${TXT} ]; then
        msg "Error find input file: ${TXT}"
        cd $WD
        return
    fi
    $RST2HTML $CC --template=${TEMPLATE} --stylesheet-path=${CSS} ${LINK_SS} $TXT $HTML
    cd $WD
}

function help2pdf {
    local DIR="$1"
    local LINKS="$2"
    local TEMPLATE="$3"
    local CSS="$4"
    local FILES="$5"

    if [ ! -e "$DIR" ]; then
        echo "Output directory does not exist: $DIR"
        return
    fi
    for file in $FILES; do
        if [ -f $file ]; then
           f=$(basename $file)
           msg "$f [ $DIR ]"
           local LTXT="${f}.txt"
           local TXT="${DIR}/${LTXT}"
           local TEX="${f}.tex"
           cp $file ${TXT}
        else 
            msg "Error finding file: '$file'"
        fi
    done
    
}

function help2html {
    local DIR="$1"
    local LINKS="$2"
    local TEMPLATE="$3"
    local CSS="$4"
    local FILES="$5"

    if [ ! -e "$DIR" ]; then
        echo "Output directory does not exist: $DIR"
        return
    fi
    for file in $FILES; do
        if [ -f $file ]; then
           f=$(basename $file)
           msg "$f [ $DIR ]"
           local LTXT="${f}.txt"
           local TXT="${DIR}/${LTXT}"
           local HTML="${f}.html"
           help2txt $file $TXT $LINKS $DIR
           txt2html $DIR $LTXT $HTML $TEMPLATE "$CSS"
        else 
            msg "Error finding file: '$file'"
        fi
    done
}

$RST2HTML -h >& /dev/null || ( msg "${RST2HTML} not found, exiting" && exit -2)

DIR=""
LINKS=""
TEMPLATE=""
CSS=""
FILES=""
NO_ARGS=0

mapping=" 
.        html/commands
sss_dir  html/commands/sss.com
spe_dir  html/commands/spe.com
user_man html/manual
index    html
"

function everything {
    for doc in $( find html -name '*.doc' ); do
        . $doc
        msg "Working on documenation: ${doc}"
        if [ x${FORMAT} == x"html" ]; then
            help2html "$DIR" "$LINKS" "$TEMPLATE" "$CSS" "$FILES"
        fi
        if [ x${FORMAT} == x"pdf" ]; then
            help2pdf "$DIR" "$PDFLINKS" "$PDFTEMPLATE" "$CSS" "$FILES"
        fi
        DIR=""
        LINKS=""
        TEMPLATE=""
        CSS=""
        FILES=""
    done  
}

FORMAT="html"

if [ $# -ne $NO_ARGS ]; then
    HELP="../aux/help/"
    for arg in $* ; do
        if [ x"all" == x"$arg" ]; then
            everything
            exit
        elif [ x"-pdf" == x"$arg" ]; then
            FORMAT="pdf"
        elif [ x"-html" == x"$arg" ]; then
            FORMAT="html"
        else 
            msg "working on file: $arg"
            file=$(echo ${arg} | sed "s|.*${HELP}||" )
            if [ "x"${file} != "x" ]; then
                DX=$(dirname $file)
                OUT=$(echo "$mapping" | awk '$1==IN {print $2}' IN=$DX)
                VAR=$(find $OUT -maxdepth 1 -name '*.doc')
                . $VAR
                if [ x${FORMAT} == x"html" ]; then
                    help2html "$DIR" "$LINKS" "$TEMPLATE" "$CSS" "${arg}"
                fi
                if [ x${FORMAT} == x"pdf" ]; then
                    help2pdf "$DIR" "$PDFLINKS" "$PDFTEMPLATE" "$CSS" "${arg}"
                fi
            else 
                msg "Error finding file: $arg"
            fi
        fi
    done
else 
    usage 
    exit
fi

