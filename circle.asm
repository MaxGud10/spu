push 0
pop bx ; y = 0

20:

push 0
pop 
ax ;  x = 0 

15:

push ax
push 5
sub
push ax
push 5
sub
mull
push bx
push 5
sub
push bx
push 5
sub
mull
add
push 16
jb 12:

push bx
push 10
mull
push ax
add
pop dx
push 35
pop [dx]

12:
ded_smesharik

push ax 
push 1
add
pop ax ; x ++

push 10
push ax 
jb 15:

push bx 
push 1
add 
pop bx 

push 10
push bx
jb 20:

ded_smesharik
hlt 