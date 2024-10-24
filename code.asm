push 7 
pop bx



push 0
pop cx 

20:

push 0
pop ax

19:

push ax
push 1
add 
pop ax 

push bx 
push ax
jb 19:

push cx 
push 1
add
pop cx

push bx
push cx 
jb 20:

out 
hlt