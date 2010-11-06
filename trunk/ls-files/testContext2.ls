#grammar context
#set iteration=10 // hloubka rekurze
#set default_length=1.0 
#set default_angle=0.3 // implicitni uhel natoceni zelvy 
#set default_radius=1.0 // tloustka vetvi    
#set contour_detail=10
#define L 3.0
#type 2L
#set turtle_type=STRAIGHT_PIPE 
#axiom 
X
#endaxiom
#rules  
<X>->FAP(positionX(),positionY())
<A>P(x,y):(!((x<-L/2)|(x>L/2)|(y<0)|(y>L)))->KF/(180)A
<A>P(x,y):((x<-L/2)|(x>L/2)|(y<0)|(y>L))->T%
<F>T->S
<F>S->SF
<S>->e
<K>S->[+FAP(positionX(),positionY())]
#endrules
#endgrammar