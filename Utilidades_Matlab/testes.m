
%The same of h = tf([1 0],[1 2 10]);

s = tf('s');
H = s/(s^2 + 2*s + 10)

stepplot(H)
%rlocus(H)

%[p,z] = pzmap(H)
%h = pzplot(H)

%pretty(H);
%rlocus(mtf);

%Es =Cs - Rs;