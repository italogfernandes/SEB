

% Sendo b/(s^2 + bs + b)
% b = 100
% a = 15
% w0 = sqrt(b)
% zeta = (a/2)/w0
zeta = 0.25;
w0 = 3;
H = tf(w0^2,[1,2*zeta*w0,w0^2])
% s = tf('s');
% H = 9/(s*(s^2 + 9*s + 9))


if zeta < 0
    fprintf('Nao é um sistema estavel\n');
elseif zeta == 0
    fprintf('Reposta sem amortecimento\n');
elseif zeta < 1
    fprintf('Reposta sub-amortecida\n');
    %Calcular Instante de Pico a pico
    Tp = pi / (w0 * sqrt(1-zeta.^2));
    disp(['1. Instante de Pico - Tp: ' num2str(Tp), ' s']);
    %Ultrapassagem percentual - %UP
    Up = exp(-zeta*pi/sqrt(1-zeta.^2));
    disp(['2. Ultrapassagem percentual - %UP: ' num2str(Up*100), ' %']);
    %Tempo de assenstamento - Ts
    Ts = 4 / (zeta * w0);
    disp(['3. Tempo de assenstamento - Ts: ' num2str(Ts) ' s']);
    %Resposta ao impusso
    tempo = linspace(0,2*Ts,1000);
    resposta = step(H,tempo);
    t10percent = min(tempo(resposta > 0.1));
    t90percent = min(tempo(resposta > 0.9));
    
    %Tempo de subida - Tr
    Tr = t90percent - t10percent;
    disp(['4. Tempo de subida - Tr: ' num2str(Tr) ' s']);
    
    %Plotagem
    fig = figure();
    hold on;
    grid on;
    
    plot(tempo,resposta);
    
    %Tempo de Pico Tp
    scatter(Tp,max(resposta),'+');
    text(Tp,max(resposta)+0.1,...
        ['  Tp = ' num2str(Tp,2)]);
    
    %Ultrapassagem Percentual
    text(Tp,max(resposta),['  Max= ' num2str(max(resposta),3) ...
        ' %Up= ' num2str(Up*100,2)]);
    
    %3. Tempo de assentamento - Ts
    scatter(Ts,resposta(tempo == min(tempo(tempo > Ts))));
    text(Ts,0.95,[' t[e=2%]= ' num2str(Ts,2)]);
    
    %4. Tempo de subida Tr
    scatter(t10percent,0.1);
    text(t10percent,0.1,[' t[90%]= ' num2str(t10percent,2)]);
    scatter(t90percent,0.9);
    text(t90percent,0.9,[' t[10%]= ' num2str(t90percent,2)]);
    
    text(t90percent-t10percent,0.5,[' Tr= ' num2str(Tr,2)]);
    
   
    title('Resposta ao Degrau Unitário');
    xlabel('Tempo (segundos)');
    ylabel('Amplitude');
    axis([0,max(tempo),0,max(resposta)*1.1]);
    
elseif zeta == 1
    fprintf('Reposta criticamente-amortecida\n');
elseif zeta > 1
    fprintf('Reposta subper-amortecida\n');
end


% 
% p = pole(H)
% [z, gain] = zero(H)
% tempo = linspace(0,10,1000);
% reposta = step(H,t);
% 
% syms ss;
% F = 9/(ss*(ss^2 + 9*ss + 9));
% ht = ilaplace(F);
% pretty(ht);
% 
% rlocus(H)
%stepplot(H)










