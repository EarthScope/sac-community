
message "With Bad Pathname"
do file wild /dev/file_does_not_exist*
       message $file
enddo

message "With Bad Folder"
do file wild dir fakedir *
       message $file
enddo

mkdir test.wild.dir

message "With Folder, but empty"
do file wild dir test.wild.dir *
       message "File $file$"
enddo

message "With Folder, but nothing"
do file wild dir test.wild.dir 
       message "File $file$"
enddo

message "With Nothing"
do file wild 
       message "File $file$"
enddo


