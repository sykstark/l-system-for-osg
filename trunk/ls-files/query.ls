#grammar Query
#set turtle_type=STRAIGHT_PIPE
#type 0L

#set default_length=0.8
#set default_radius=0.4

#axiom 
FA
#endaxiom
#rules 
A:*->F(positionY())+A
#endrules
#endgrammar