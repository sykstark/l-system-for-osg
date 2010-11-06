#grammar TEST01
#set iteration=10 // hloubka rekurze
#set default_length=1.0 
#set default_angle=0.3 // implicitni uhel natoceni zelvy 
#set default_radius=1.0 // tloustka vetvi    
#set contour_detail=10   
#type 2L
#set turtle_type=STRAIGHT_PIPE 
#axiom 
B(1)A(2,3)C(4,5,6)
#endaxiom
#rules  
B(x)<A(y,z)>C(u,v,w):*->C(w,v,u)A(x,y)A(z,x)
C(a,b,c)<A(d,e)>A(f,g):*->B(a)A(b,c)C(d,g,f)
#endrules
#endgrammar