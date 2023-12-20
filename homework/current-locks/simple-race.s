.main
.var count
# this is a critical section
mov 2000(%bx:%cx:4), %ax   # get the value at the address
add $1, %ax          # increment it
mov %ax, 2000(%bx:%cx:4)   # store it back
halt
