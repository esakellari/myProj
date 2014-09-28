{------------------------------
	SAKELLARI ELISAVET 
	  1115200600152

-------------------------------}

upper [] 	= []
upper (x:xs) 	= upper2 x xs:upper xs

upper2 x [] 	= 0
upper2 x (y:ys) = if (y>x) then 1+(upper2 x ys)
			else upper2 x ys

{-------------------------------------------------------------------}
find_max (x:[])	= x
find_max (x:y:ys)= if(x>y) then (find_max (x:ys))
			else (find_max (y:ys))

maxsegment (x:xs) = find_max(max_help (x:xs):(find_max(max_help2 (x:xs) 0)):max_help3 xs) 


max_help3 [] = []
max_help3 (x:xs) = (find_max(max_help2 (x:xs) 0)):max_help3 xs

max_help (x:xs) = find_max(x:xs)

max_help2 [] count = []
max_help2 (x:xs) count = ((x+count): max_help2 xs (x+count))
												
{---------------------------------------------------------------------}
f(i,j,k) = i+j+k 
				
sssum f a b c	= sssum2 f a 1 b 1 c 1


sssum2 f a i b j c k = 	if(c == k)
			then 
			if (b == j)
				then 
				if (a == i)
					then f(a,b,c)
					else 
					 (f(i,j,k)) + (sssum2 f a (i+1) b 1 c 1)
					else 
					 (f(i,j,k)) + (sssum2 f a i b (j+1) c 1)
					else 
					 (f(i,j,k)) + (sssum2 f a i b j c (k+1))
							
	
{----------------------------------------------------------------------}	
member x [] 	= False
member x (y:xs) = if (x==y) then True
		  else (member x xs)

						
dfa (start,(x:xs),(y:ys)) (z:zs)= dfa_help start (x:xs) (x:xs) (y:ys) (z:zs)
							

dfa_help q1 del delta finals []	= if ((member q1 finals) == True) then True else False

dfa_help q1 ((q2,m,q3):xs) delta finals (z:zs)	= if ((q1==q2 && z==m)==True)
						  then dfa_help q3 delta delta finals zs 
						  else dfa_help q1 xs delta finals (z:zs)
						
				
	
