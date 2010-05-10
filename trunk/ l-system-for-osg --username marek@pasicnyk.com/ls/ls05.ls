# --- L-System Parser/Mutator --- Lj Lapre ---------------------------------- 
# 
5 # recursion 
20 # angle 
15 # thickness # 
#---------------------------------------- axioms 
# 
#&(60)C # use this as axiom to test a leave 
#b # use this as axiom to test a branch 
b>(60)b>(60)b>(60)b>(60)b>(60)b # the whole thing 
# 
# ---------------------------------------- rules 
# 
b=[&(30)A] 
# 
A=~(7)$t(.1)F[+(40)C][-(40)C]!(.95)~(7)t(.1)FA 
# 
C=~(10)$tF[+(60)L][-(60)L]C 
# 
L=[~(15)cc{-f+f+f-|-f+f}] 
# 
F='(1.3)F'(.77) f='(1.3)f'(.77) 
# 
~=_ # uncomment to remove random efx 
@