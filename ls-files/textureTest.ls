#grammar TEXTURES
#set iteration=10 // hloubka rekurze
#set default_length=3.0 
//#set diffuse_texture=data\ls\tex\test01.png
#set default_angle=0.2  //1.57 // implicitni uhel natoceni zelvy 
#set default_radius=1.0 // tloustka vetvi    
#set length_multiplier=0.9
#set contour_detail=10
#type 0L
#set turtle_type=STRAIGHT_PIPE 
#axiom 
F+F+F+F+F
#endaxiom
#rules  
A(y):*->+FA(y+1):4
A(x):x>4->-FA(x+1):x
#endrules
#endgrammar