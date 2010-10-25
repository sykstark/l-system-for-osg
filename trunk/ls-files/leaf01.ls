#grammar Leaf01
#set turtle_type=RECTANGLE
#type 0L

#set default_length=4.0
#set default_radius=2.0
#set diffuse_texture=data\ls\tex\transp.png

#axiom 
F
#endaxiom
#rules  
A(y):y<4->+FA(y+1)
A(x):x>6->A(0)
A(x):x>3->-FA(x+1)
#endrules
#endgrammar