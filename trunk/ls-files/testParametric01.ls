#grammar TEST01
#set iteration=10 // hloubka rekurze
#set default_length=1.0 
#set default_angle=0.3 // implicitni uhel natoceni zelvy 
#set default_radius=0.1 // tloustka vetvi 
#set length_increment=0.9
#set contour_detail=4
#type 0L
#set turtle_type=JOINTED_PIPE
#axiom 
FA(1)
#endaxiom
#rules  
A(y):y<4->+FA(y+1)
A(x):x>6->A(0)
A(x):x>3->-FA(x+1)
#endrules
#endgrammar