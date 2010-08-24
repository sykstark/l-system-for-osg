#grammar TEST01
#define len 30
#define high 20.2
#set texture=testik
#axiom 
A(10,10)
#endaxiom
#rules  
A(x,y):x+y>10->B(x+y+len,x-y)A(2*y*high,2*x)
B(x,y):x>y->A(1+x/high,y+1+len)
#endrules
#endgrammar