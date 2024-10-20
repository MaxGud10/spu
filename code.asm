push 0
push 0
pop ax 
pop bx

pushr ax 
pushr ax
mull 
pushr bx
add
pop bx

pushr ax 
push 1
add
pop ax

pushr bx
out 

push 6
pushr ax
jb 8

pushr bx
out 

hlt 
