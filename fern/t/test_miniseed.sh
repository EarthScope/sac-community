./fern -D miniseed   -e 'usgs:usp0006dzc' -n XE -d +30m -c BHZ -p t/test_miniseed -o t/test_miniseed.request.test

# Check Request
diff t/test_miniseed.request.test t/test_miniseed.request || exit -1


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
md5_file=$(md5_file t/test_miniseed.*.IRISDMC.mseed) || echo >&2 "Error: Could not find md5 program, aborting"

# Generated MD5 Sum on original file
md5_check="f65705013ecd6b0e944a83e0c54fcbf2"
if [[ $md5_check != $md5_file ]]; then
    echo "MD5 FILE:     $md5_file"
    echo "MD5 EXPECTED: $md5_check"
    ls -l t/test_miniseed.*.mseed
    exit -1
fi
rm -f t/test_miniseed*mseed
