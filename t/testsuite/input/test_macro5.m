$KEYS COMPS FILE
message "$file"
message $comps
message "$comps"
setbb files ""
do c list $comps
  message "Appending $c"
  setbb files "%files $file$.$c"
  message "Files: %files"
enddo
message %files
message "%files"
read %files
getbb sacnfiles
