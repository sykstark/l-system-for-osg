#grammar TEST01
#define len 30
#define high 20.2
#set texture=testik
#set default_angle=0.4
#set default_length=2.0
#set default_thickness=1.0
#type 0L
#set turtle_type=JOINTED_PIPE
#axiom 
+(0)FFF[+FF[FFF]+F]FFFF
#endaxiom
#rules  
F:*->FF
#endrules
#endgrammar