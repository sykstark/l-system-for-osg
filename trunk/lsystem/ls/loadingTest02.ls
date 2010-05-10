#grammar MONOPODIALNI_STROM
#set recursion 10 // hloubka rekurze 
#set default_turtle_angle 22.5 // implicitni uhel natoceni zelvy 
#set default_pipe_width 30 // tloustka vetvi 
#axiom 
F'(0.25)A
#endaxiom
#rules  
A:*->!(0.577)'(0.85)[>(70.74)B][>(170.63)B][>(-84.63)B]B
B:*->&(43.95)[^^L][&&L]Z[^^L][&&L]ZC
C:*->!(0.577)'(0.85)[>(90.63)D][>(-132.0)D]D
D:*->&(33.95)[^^L][&&L]Z[^^L][&&L]ZA
#endrules
#endgrammar