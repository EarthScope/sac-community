./fern -D sac   -e 'usgs:usp0006dzc' -n XE -v -d +30m -c BHZ -s DOOR -o t/output_sac.request.test

diff t/output_sac.request t/output_sac.request.test || exit -1

SACFILE=XE.DOOR..BHZ.M.1994.160.003321.sac
if [ ! -e $SACFILE ]; then
    echo "expected sac file does not exist: $SACFILE"
    exit -1
fi

function command_find() {
    for cmd in $*; do
        OUT=$(command -v $cmd)
        RETVAL=$?
        if [ $RETVAL == 0 ]; then
            echo ${OUT}
            exit 0
        fi
    done
    exit -1
}

function md5_file() {
    FILE="$1"
    V=$(command -v md5)
    RETVAL=$?
    if [ $RETVAL -eq 0 ]; then
        md5 -q "$1"
        exit 0
    fi
    V=$(command -v md5sum)
    RETVAL=$?
    if [ $RETVAL -eq 0 ]; then
        md5sum "$1" | awk '{print $1}'
        exit 0
    fi
    exit -1
}
# Check File
md5_file=$(md5_file $SACFILE) || echo >&2 "Error: Could not find md5 program, aborting"

# Generated MD5 Sum on original file
md5_check="070b60dfae8994551085b09f19eb3f7c" # v6 original
md5_check="fe008fb3eb6a8a513d66df52e4801fc0" # with v7 header, but still v6
md5_check="f52f935f2f0219214d7716754c74d7f6" # with v7, unused27 = true
if [[ $md5_check != $md5_file ]]; then
    echo "MD5 FILE:     $md5_file"
    echo "MD5 EXPECTED: $md5_check"
    ls -l $SACFILE
    exit -1
fi

rm -f $SACFILE



