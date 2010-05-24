#grammar TEST01
  //prazdny radek
#set Recursion 10 // hloubka rekurze 
#set DefaultTurtleAngle 22.5 // implicitni uhel natoceni zelvy 
#set DefaultPipeWidth 30 // tloustka vetvi 
#axiom 
A
#endaxiom
#rules  
A:*->BCB
B:*->A
C:*->ABA
#endrules
#endgrammar