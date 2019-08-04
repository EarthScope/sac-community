./fern -D sac   -e 'usgs:usp0006dzc' -n XE -v -d +30m -c BHZ -s DOOR -o t/output_sac.request.test

diff t/output_sac.request t/output_sac.request.test

SACFILE=XE.DOOR..BHZ.M.1994.160.003321.sac
if [ ! -e $SACFILE ]; then
    echo "expected sac file does not exist: $SACFILE"
    exit -1
fi

# Check File
md5_file=$(md5 -q $SACFILE)

# Generated MD5 Sum on original file
md5_check="070b60dfae8994551085b09f19eb3f7c"
if [[ $md5_check != $md5_file ]]; then
    echo "MD5 FILE:     $md5_file"
    echo "MD5 EXPECTED: $md5_check"
    ls -l $SACFILE
    exit -1
fi

rm -f $SACFILE



