#grammar TEST01
#set texture=texture.bmp
#axiom 
A(10,10)
#endaxiom
#rules  
A(x,y):*->B(x+y,x-y)A(2*y,2*x)
B(x,y):*->A(1+x,y+1)
#endrules
#endgrammar