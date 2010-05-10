#grammar MONOPODIALNI_STROM
#set recursion 10 // hloubka rekurze 
#set default_turtle_angle 22.5 // implicitni uhel natoceni zelvy 
#set default_pipe_width 30 // tloustka vetvi 
#axiom 
F'(0.25)A
#endaxiom
#rules  
A->!(0.577)'(0.85)[>(70.74)B][>(170.63)B][>(-84.63)B]B
B->&(43.95)[^^L][&&L]Z[^^L][&&L]ZC
C->!(0.577)'(0.85)[>(90.63)D][>(-132.0)D]D
D->&(33.95)[^^L][&&L]Z[^^L][&&L]ZA
#endrules
#homomorphisms
L:!(0.1)'(0.25)c(4)~F{'(0.5)-(120.0)f++f+(45.0)f+f+(15.0)fff+(115.0)fff+(15.0)f+f+(45.0)f}F
#endhomomorphisms
#endgrammar