./fern -E -m 9/10 -t 1950/01/01 2020/01/01 > t/events.txt.test
diff t/events.txt t/events.txt.test
