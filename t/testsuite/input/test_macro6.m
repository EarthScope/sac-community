
fg seismo
lh user1
if &1,user1 eq UNDEFINED
  message "User1 should be undefined"
endif
if &1,user1 != UNDEFINED
  message "User1 should be undefined"
endif

lh kuser1
if &1,kuser1 eq UNDEFINED
  message "KUser1 should be undefined"
endif
if &1,kuser1 != UNDEFINED
  message "KUser1 should be undefined"
endif

