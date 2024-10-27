push 0
pop ax ; x = 0

push 0
pop bx ; y = 0

push 25
pop cx ; R^2 = 25

4:

push ax 
push ax ; x^2
mull
pop ax
push ax
push 1
add    ; x^2 + 1
pop ax 

push ax
push 35
pop [ax] 


push bx
push bx 
mull    ; y^2
pop bx 
push bx
push 1
add
pop bx ; y^2 + 1

push bx 
push 35
pop[bx]

push ax
push bx
add
pop dx ; x^2 + y^2

push cx 
push dx ; x^2 + y^2 < R^2
jb 4:

ded_smesharik
hlt