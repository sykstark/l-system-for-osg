#grammar TEST01
#set iteration=10 // hloubka rekurze
#set default_length=3.0 
#set default_angle=30 // implicitni uhel natoceni zelvy 
#set default_radius=0.5 // tloustka vetvi    
#set contour_detail=5
#include d:\HCI\VRECKO\bin\data\ls\leaf01.ls
#type 0L
#set turtle_type=STRAIGHT_PIPE 
#axiom 
#{Leaf01}[F]
#endaxiom
#rules  
A(y):*->+FA(y+1):4
A(x):x>4->-FA(x+1):x
#endrules
#endgrammar