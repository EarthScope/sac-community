./fern -D available  -e usgs:usp0006dzc -n XE -d +30m -c BHZ  > t/avail.txt.test
diff t/avail.txt.test t/avail.txt

