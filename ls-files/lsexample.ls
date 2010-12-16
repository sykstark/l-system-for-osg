#lsystem TernaryTree          // unik�tn� ID L-syst�mu

#type 0L                      // typ L-syst�mu
#define LeafPitch 65          // konstanta
#include data\ls\leaf.ls      // cesta k pod��zen�mu L-syst�mu

// parametry
#set Iteration=14             
#set TurtleType=STRAIGHT_PIPE
#set DefaultAngle=30.0

// axiom
#axiom
F'(0.65)A
#endaxiom

// p�episovac� pravidla
#rules 
A:*->!(0.577)'(0.87)[/(90.74)B][/(-132.63)B]B
B:*->^(33.95)[^(LeafPitch)#{Leaf}][#{Leaf}]Z[^(LeafPitch)#{Leaf}][#{Leaf}]ZA
#endrules

#endlsystem