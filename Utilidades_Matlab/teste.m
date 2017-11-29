
%Seja um motor:
Ra = 10;
kb = 1E-2;
k = kb;
Jm = 1E-4;
fm = 1E-5;

%Carga
Jc = 1E-3;
fc = 1E-3;

%Tensão max = 100 Volts

%1. Determinar função de transferencia do motor:

syms s
H = k / ((Jm+Jc)*Ra*s + (fm+fc)*Ra + k*kb);
pretty(H);

syms Kp
G1 = Kp*k / ((Jm+Jc)*Ra*s + (fm+fc)*Ra + k*kb);
pretty(G1);
%F = G1/(1+G1);
F = (Kp/100)/((11/1000)*s+(51/5000)+(Kp/100));
pretty(F);

Erro = 1 - (0.01*Kp)/((51/5000)+(0.01*Kp));
pretty(Erro);

Kps = 0:1:100;
erros = 1 - ...
    (0.01.*Kps)./((51./5000)+(0.01.*Kps));
plot(Kps,erros);


figure();
s = tf('s');
H = (1/100)/((11/1000)*s + (51/5000));

stepplot(H);
