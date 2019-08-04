# ./fern -E -t 1994/160 +1d -m 8/10 -q
./fern -S -e usgs:usp0006dzc -n IU,XE -r0/35 -q > t/station_event_1.txt.test
./fern -S -t 1994-06-09T00:33:16 +0m  -n IU,XE -O -67.55/-13.84  -r0/35 -q > t/station_event_2.txt.test
diff t/station_event_1.txt.test t/station_event_0.txt
diff t/station_event_2.txt.test t/station_event_0.txt
diff t/station_event_1.txt.test t/station_event_2.txt.test

