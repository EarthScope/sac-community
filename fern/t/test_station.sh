./fern -S -t 1999/01/01 +1day -n IU  > t/station.txt.test
diff t/station.txt.test t/station.txt
