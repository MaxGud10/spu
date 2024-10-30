in
pop ax     ; a 
in
pop bx     ; b
in
pop cx     ; c 

push ax
push 0
je 2:     ; a == 0

call 5:   ; discriminant
out
hlt



2:         ; bx + c = 0 
push bx
push 0
jne 4:     ; b != 0

pop cx    ; ; a = 0, b = 0 , c = x
out 
hlt       

4:         ; bx + c = 0
push cx
push 0
je 13:     ; c == 0

push cx
push -1    ; -c
mull
pop cx 
push bx
push cx 
div        ; -c / b
pop cx     
out
hlt

13:       ; a = 0, b = x, c = 0
pop bx  
out 
hlt


5:          ; discriminant
push bx
push 0
je 17:     ; bx == 0 (ax * x ^ 2 + cx)



push bx 
push bx 
mull        ; b ^ 2
push 4
push ax
push cx
mull
mull
sub         ; b ^ 2 - 4 * a * c
push 0
je 6:       ; discriminant == 0
ja 7:       ; discriminant > 0
push 0      ; no root
out
hlt

17:         ; bx == 0 (ax * x ^ 2 + cx)
push cx 
push 0
je 31:      ; cx == 0 (ax * x ^ 2 = 0)

push -1
push cx 
mull 
push ax
div
sqrt 
pop dx      ; x = sqrt (-(cx) / (ax))
out
hlt

31:        ; bx = 0, cx = 0
pop ax 
out
hlt

6:         ; discriminant == 0
push 1
out

push -1
push bx
mull
push 2
push ax
div        ; x1 = x2 = -b / (2 * a)
pop dx   
out 
hlt 

7:         ; discriminant > 0
push 2
out

push 2
push ax
mull
push -1
push bx    ; -b
mull
push dx  
sqrt 
sub
div 
out       ; x1

push 2
push ax
mull
push -1
push bx    ; -b
mull
push dx  
sqrt 
add
div        ; x2
out 
ret