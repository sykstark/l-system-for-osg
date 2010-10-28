#grammar TEST01
#set iteration=10 // hloubka rekurze
#set default_length=1.0 
#set default_angle=0.3 // implicitni uhel natoceni zelvy 
#set default_radius=1.0 // tloustka vetvi    
#set length_multiplier=0.9
#set contour_detail=5
#include d:\HCI\VRECKO\bin\data\ls\includeTestSub01.ls
#include d:\HCI\VRECKO\bin\data\ls\includeTestSub02.ls
#type 0L
#set turtle_type=STRAIGHT_PIPE 
#axiom 
F{SUB1}{SUB2}
#endaxiom
#rules  
A(y):*->+FA(y+1):4
A(x):x>4->-FA(x+1):x
#endrules
#endgrammar
