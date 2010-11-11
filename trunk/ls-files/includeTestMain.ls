#grammar TEST01
#set iteration=10 // hloubka rekurze
#set default_length=1.0 
#set default_angle=3 // implicitni uhel natoceni zelvy 
#set default_radius=1.0 // tloustka vetvi    
#set length_multiplier=0.9
#set contour_detail=25
#include d:\HCI\VRECKO\bin\data\ls\leaf01.ls
#include d:\HCI\VRECKO\bin\data\ls\includeTestSub02.ls
#set diffuse_texture=data\ls\tex\bark\bark_20091031_05_512x512_seamless.jpg
#type 0L
#set turtle_type=STRAIGHT_PIPE 
#axiom 
F+#{Leaf01} //#{SUB2}----
#endaxiom
#rules  
A(y):*->+FA(y+1):4
A(x):x>4->-FA(x+1):x
#endrules
#endgrammar
