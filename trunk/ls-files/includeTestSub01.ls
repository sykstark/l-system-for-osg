#grammar SUB1
#type 0L
#set turtle_type=STRAIGHT_PIPE
#set iteration=10 // hloubka rekurze
#set default_length=1.0 
#set default_angle=0.3 // implicitni uhel natoceni zelvy 
#set default_radius=1.0 // tloustka vetvi    
#set length_multiplier=0.9
#set contour_detail=5 
#axiom 
FF
#endaxiom
#rules  
A(y):*->+FA(y+1):4
A(x):x>4->-FA(x+1):x
#endrules
#endgrammar
