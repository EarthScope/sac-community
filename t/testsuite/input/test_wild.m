

mkdir test.wild.dir
cd test.wild.dir

touch newfile

do i = 0 , 400
  message "touching file test.wild.$i$"
  systemcommand touch test.wild.$i
enddo
  
message "finding test.wild.*"

do file wild test.wild.*
  message "found file: $file$"
enddo

cd ..

do file wild test.wild.dir/test.wild.1
  message "found file: $file$"
enddo

do file wild test.wild.dir/test.wild.?
  message "found file: $file$"
enddo

do file wild dir test.wild.dir test.wild.1
  message "found file: $file$"
enddo

do file wild dir test.wild.dir test.wild.?
  message "found file: $file$"
enddo

