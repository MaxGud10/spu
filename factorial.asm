in
pop ax

call 12:

push bx
out
hlt

12:   
          
push ax
push 0
jne 5:
push 1
pop bx
ret

5:
push ax

push ax
push 1
sub
pop ax
call 12:

push bx
mull
pop bx 
ret