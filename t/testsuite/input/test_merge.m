
fg seismo
setbb STARTT &1,b
setbb ENDT &1,e
write test.merge.true

# Same Signal
fg seismo
write test.merge.1
write test.merge.2

read test.merge.1
merge test.merge.2
if &1,b ne %STARTT
   message "Error incorrect start time %STARTT &1,b "
endif
if &1,e ne %ENDT
   message "Error incorrect end time %ENDT &1,e "
endif
if %SACNFILES ne 1
   message "Error incorrect number of files %SACNFILES% "
endif
subf test.merge.true
if &1,depmax gt 0 
   message "Error merge vs expected MAX &1,depmax "
endif
if &1,depmin lt 0 
   message "Error merge vs expected MIN &1,depmin "
endif


# Incorrect Overlapped sequence
fg seismo
write test.merge.1
mul 2
write test.merge.2

read test.merge.1
#merge test.merge.2

# Second File Past end of First File
#  kztime and b time account for difference
fg seismo
write test.merge.1
ch allt 15
write test.merge.2

read test.merge.1
merge test.merge.2
if &1,b ne %STARTT
   message "Error incorrect start time %STARTT &1,b "
endif
if &1,e ne %ENDT
   message "Error incorrect end time %ENDT &1,e "
endif
if %SACNFILES ne 1
   message "Error incorrect number of files %SACNFILES% "
endif
subf test.merge.true
if &1,depmax gt 0 
   message "Error merge vs expected MAX &1,depmax "
endif
if &1,depmin lt 0 
   message "Error merge vs expected MIN &1,depmin "
endif

# Second File Past end of First File
#  kztime and b time account for difference
fg seismo
write test.merge.1
ch allt 5
write test.merge.2

read test.merge.1
merge test.merge.2
if &1,b ne %STARTT
   message "Error incorrect start time %STARTT &1,b "
endif
if &1,e ne %ENDT
   message "Error incorrect end time %ENDT &1,e "
endif
if %SACNFILES ne 1
   message "Error incorrect number of files %SACNFILES% "
endif
subf test.merge.true
if &1,depmax gt 0 
   message "Error merge vs expected MAX &1,depmax "
endif
if &1,depmin lt 0 
   message "Error merge vs expected MIN &1,depmin "
endif

# Second File before First File
# This should work, not be an error
fg seismo
write test.merge.1
ch allt -15
write test.merge.2

read test.merge.1
merge test.merge.2
if &1,b ne %STARTT
   message "Error incorrect start time %STARTT &1,b "
endif
if &1,e ne %ENDT
   message "Error incorrect end time %ENDT &1,e "
endif
if %SACNFILES ne 1
   message "Error incorrect number of files %SACNFILES% "
endif
subf test.merge.true
if &1,depmax gt 0 
   message "Error merge vs expected MAX &1,depmax "
endif
if &1,depmin lt 0 
   message "Error merge vs expected MIN &1,depmin "
endif

read test.merge.1 test.merge.2
merge
if &1,b ne %STARTT
   message "Error incorrect start time %STARTT &1,b "
endif
if &1,e ne %ENDT
   message "Error incorrect end time %ENDT &1,e "
endif
if %SACNFILES ne 1
   message "Error incorrect number of files %SACNFILES% "
endif
subf test.merge.true
if &1,depmax gt 0 
   message "Error merge vs expected MAX &1,depmax "
endif
if &1,depmin lt 0 
   message "Error merge vs expected MIN &1,depmin "
endif

dc 1
merge test.merge.1 test.merge.2
if &1,b ne %STARTT
   message "Error incorrect start time %STARTT &1,b "
endif
if &1,e ne %ENDT
   message "Error incorrect end time %ENDT &1,e "
endif
if %SACNFILES ne 1
   message "Error incorrect number of files %SACNFILES% "
endif
subf test.merge.true
if &1,depmax gt 0 
   message "Error merge vs expected MAX &1,depmax "
endif
if &1,depmin lt 0 
   message "Error merge vs expected MIN &1,depmin "
endif


dc 1
read test.merge.1 test.merge.2
merge
if &1,b ne %STARTT
   message "Error incorrect start time %STARTT &1,b "
endif
if &1,e ne %ENDT
   message "Error incorrect end time %ENDT &1,e "
endif
if %SACNFILES ne 1
   message "Error incorrect number of files %SACNFILES% "
endif
subf test.merge.true
if &1,depmax gt 0 
   message "Error merge vs expected MAX &1,depmax "
endif
if &1,depmin lt 0 
   message "Error merge vs expected MIN &1,depmin "
endif



cut &1,b 15
read test.merge.1
write over
cut off

read test.merge.2
cut -2 &1,e
read test.merge.2
write over
cut off

message "READ file1 file2 MERGE"
read test.merge.1 test.merge.2
merge
write test.merge.9
if &1,b ne %STARTT
   message "Error incorrect start time %STARTT &1,b "
endif
if &1,e ne %ENDT
   message "Error incorrect end time %ENDT &1,e "
endif
if %SACNFILES ne 1
   message "Error incorrect number of files %SACNFILES% "
endif
subf test.merge.true
if &1,depmax gt 0 
   message "Error merge vs expected MAX &1,depmax "
endif
if &1,depmin lt 0 
   message "Error merge vs expected MIN &1,depmin "
endif

message "READ file2 file1 MERGE"
read test.merge.2 test.merge.1
merge
if &1,b ne %STARTT
   message "Error incorrect start time %STARTT &1,b "
endif
if &1,e ne %ENDT
   message "Error incorrect end time %ENDT &1,e "
endif
if %SACNFILES ne 1
   message "Error incorrect number of files %SACNFILES% "
endif
subf test.merge.true
if &1,depmax gt 0 
   message "Error merge vs expected MAX &1,depmax "
endif
if &1,depmin lt 0 
   message "Error merge vs expected MIN &1,depmin "
endif

dc 1
message "MERGE file1 file2"
merge test.merge.1 test.merge.2
write test.merge.8
if &1,b ne %STARTT
   message "Error incorrect start time %STARTT &1,b "
endif
if &1,e ne %ENDT
   message "Error incorrect end time %ENDT &1,e "
endif
if %SACNFILES ne 1
   message "Error incorrect number of files %SACNFILES% "
endif
subf test.merge.true
if &1,depmax gt 0 
   message "Error merge vs expected MAX &1,depmax "
endif
if &1,depmin lt 0 
   message "Error merge vs expected MIN &1,depmin "
endif

dc 1
message "MERGE file2 file1"
merge test.merge.2 test.merge.1
if &1,b ne %STARTT
   message "Error incorrect start time %STARTT &1,b "
endif
if &1,e ne %ENDT
   message "Error incorrect end time %ENDT &1,e "
endif
if %SACNFILES ne 1
   message "Error incorrect number of files %SACNFILES% "
endif
subf test.merge.true
if &1,depmax gt 0 
   message "Error merge vs expected MAX &1,depmax "
endif
if &1,depmin lt 0 
   message "Error merge vs expected MIN &1,depmin "
endif

message "READ file1 MERGE file2"
read test.merge.1
merge test.merge.2
if &1,b ne %STARTT
   message "Error incorrect start time %STARTT &1,b "
endif
if &1,e ne %ENDT
   message "Error incorrect end time %ENDT &1,e "
endif
if %SACNFILES ne 1
   message "Error incorrect number of files %SACNFILES% "
endif
subf test.merge.true
if &1,depmax gt 0 
   message "Error merge vs expected MAX &1,depmax "
endif
if &1,depmin lt 0 
   message "Error merge vs expected MIN &1,depmin "
endif

message "READ file2 MERGE file1"
read test.merge.2
merge test.merge.1
if &1,b ne %STARTT
   message "Error incorrect start time %STARTT &1,b "
endif
if &1,e ne %ENDT
   message "Error incorrect end time %ENDT &1,e "
endif
if %SACNFILES ne 1
   message "Error incorrect number of files %SACNFILES% "
endif
subf test.merge.true
if &1,depmax gt 0 
   message "Error merge vs expected MAX &1,depmax "
endif
if &1,depmin lt 0 
   message "Error merge vs expected MIN &1,depmin "
endif



