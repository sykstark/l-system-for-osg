#grammar monopodialni_strom
#set DefaultAngle=30.0
#set DefaultLength=10.0
#set ContourDetail=10
#set DefaultRadius=1.0
#set AngleVariance=0
#include d:\HCI\VRECKO\bin\data\ls\leaf01.ls
#set DiffuseTexture=data\ls\tex\bark\bark_20091031_05_512x512_seamless.jpg
#type 0L
#set TurtleType=STRAIGHT_PIPE
#axiom
F'(0.65)A
#endaxiom
#rules 
A:*->!(0.577)'(0.87)[/(90.74)B][/(-132.63)B]B
B:*->&(33.95)[^^#{Leaf01}][&&#{Leaf01}]Z[^^#{Leaf01}][&&#{Leaf01}]ZA
#endrules
#endgrammar