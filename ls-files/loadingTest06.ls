#grammar monopodialni_strom
#set default_angle=0.7853
#set default_length=10.0
#set contour_detail=10
#set default_radius=1.0
#set length_multiplier=0.9
#include d:\HCI\VRECKO\bin\data\ls\leaf01.ls
#set diffuse_texture=data\ls\tex\bark\bark_20091031_05_512x512_seamless.jpg
#type 0L
#set turtle_type=STRAIGHT_PIPE
#axiom
+(0)F'(0.25)A
#endaxiom
#rules 
A:*->!(0.577)'(0.85)[/(90.74)B][/(-132.63)B]B
B:*->&(33.95)[^^#{Leaf01}][&&#{Leaf01}][/(90)[^^#{Leaf01}][&&#{Leaf01}]]Z[^^#{Leaf01}][&&#{Leaf01}][/(90)[^^#{Leaf01}][&&#{Leaf01}]]ZA
#endrules
#endgrammar