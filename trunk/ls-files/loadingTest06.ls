#grammar monopodialni_strom
#set default_angle=0.7853
#set default_length=10.0
#set contour_detail=4
#set default_radius=1.0
#set length_increment=0.9
#type 0L
#set turtle_type=STRAIGHT_PIPE
#axiom
+(0)F'(0.25)A
#endaxiom
#rules 
A:*->!(0.577)'(0.901)ZZ[&(0.42)ZZA]/(2.0)[&(0.42)ZZA]/(2.31)[&(0.42)ZZA]
#endrules
#endgrammar