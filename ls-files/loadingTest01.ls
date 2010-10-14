#grammar TEST01
#define len 30
#define high 20.2
#set texture=testik
#set default_angle=30.0
#set default_length=2.0
#set default_thickness=1.0
#type 0L
#set turtle_type=JOINTED_PIPE
#axiom 
A(10,10)
#endaxiom
#rules  
A(x,y):x+y>10->B(x+y+len,x-y)A(2*y*high,2*x)
B(x,y):x>y->A(1+x/high,y+1+len)
#endrules
#endgrammar