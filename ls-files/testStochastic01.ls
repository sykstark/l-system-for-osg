#grammar TEST01
#set iteration=10 // hloubka rekurze
#set default_length=1.0 
#set default_angle=0.3 // implicitni uhel natoceni zelvy 
#set default_radius=3.0 // tloustka vetvi 
#axiom 
FA(1)
#endaxiom
#rules  
A(y):y<5->+FA(y+1):3
A(x):x>4->-FA(x+1):x
#endrules
#endgrammar