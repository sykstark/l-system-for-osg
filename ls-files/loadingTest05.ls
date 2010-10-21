#grammar TEST01
  //prazdny radek
#set iteration=10 // hloubka rekurze 
#set default_angle=22.5 // implicitni uhel natoceni zelvy 
#set default_thickness=30 // tloustka vetvi 
#axiom 
A(2)
#endaxiom
#rules  
A(y):y<4->A(y+1):1
A(x):x>3->A(x+1):x
B(y):*->A(0)
#endrules
#endgrammar