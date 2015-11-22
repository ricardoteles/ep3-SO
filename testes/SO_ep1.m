clear all;
close all;
clc;

% Escalonadores - Esses são os números que vão ficar no eixo y
% E = [1, 2, 3, 4, 5, 6];
E = [1, 2, 4];

% ######### Dados ################3

% Poucos processos
dE1_poucos = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
dE2_poucos = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
%dE3_poucos = ;
dE4_poucos = [83,84,84,82,84,84,82,84,84,84,83,83,83,83,84,84,84,84,83,84,82,84,84,84,84,84,83,84,82,84];
%dE5_poucos = ;
%dE6_poucos = ;

% Médios processos
dE1_medios = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
dE2_medios = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
%dE3_medios = ;
dE4_medios = [184,184,184,184,184,184,184,184,184,184,184,184,184,184,184,184,184,184,184,184,184,184,184,184,184,184,184,184,184,184];
%dE5_medios = ;
%dE6_medios = ;

% Muitos processos
dE1_muitos = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
dE2_muitos = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
%dE3_muitos = ;
dE4_muitos = [397,400,399,397,397,400,399,397,399,399,397,400,399,397,399,399,400,397,399,397,400,397,399,397,399,399,400,397,399,397];
%dE5_muitos = ;
%dE6_muitos = ;


% ########### Medias ############
% Poucos processos
mE1_poucos = mean(dE1_poucos);
mE2_poucos = mean(dE2_poucos);
%mE3_poucos = mean(dE3_poucos);
mE4_poucos = mean(dE4_poucos);
%mE5_poucos = mean(dE5_poucos);
%mE6_poucos = mean(dE6_poucos);

% Médios processos
mE1_medios = mean(dE1_medios);
mE2_medios = mean(dE2_medios);
%mE3_medios = mean(dE3_medios);
mE4_medios = mean(dE4_medios);
%mE5_medios = mean(dE5_medios);
%mE6_medios = mean(dE6_medios);

% Muitos processos
mE1_muitos = mean(dE1_muitos);
mE2_muitos = mean(dE2_muitos);
%mE3_muitos = mean(dE3_muitos);
mE4_muitos = mean(dE4_muitos);
%mE5_muitos = mean(dE5_muitos);
%mE6_muitos = mean(dE6_muitos);

% #### Vetor das médias (apenas para facilitar na geração dos gráficos
% m_poucos = [mE1_poucos, mE2_poucos, mE3_poucos, mE4_poucos, mE5_poucos, mE6_poucos];
% m_medios = [mE1_medios, mE2_medios, mE3_medios, mE4_medios, mE5_medios, mE6_medios];
% m_muitos = [mE1_muitos, mE2_muitos, mE3_muitos, mE4_muitos, mE5_muitos, mE6_muitos];

m_poucos = [mE1_poucos, mE2_poucos, mE4_poucos];
m_medios = [mE1_medios, mE2_medios, mE4_medios];
m_muitos = [mE1_muitos, mE2_muitos, mE4_muitos];



% ##########  Desvio padrao  ##########
% Poucos processos
dev_padE1_poucos = std(dE1_poucos);
dev_padE2_poucos = std(dE2_poucos);
%dev_padE3_poucos = std(dE3_poucos);
dev_padE4_poucos = std(dE4_poucos);
%dev_padE5_poucos = std(dE5_poucos);
%dev_padE6_poucos = std(dE6_poucos);

% Médios processos
dev_padE1_medios = std(dE1_medios);
dev_padE2_medios = std(dE2_medios);
%dev_padE3_medios = std(dE3_medios);
dev_padE4_medios = std(dE4_medios);
%dev_padE5_medios = std(dE5_medios);
%dev_padE6_medios = std(dE6_medios);

% Muitos processos
dev_padE1_muitos = std(dE1_muitos);
dev_padE2_muitos = std(dE2_muitos);
%dev_padE3_muitos = std(dE3_muitos);
dev_padE4_muitos = std(dE4_muitos);
%dev_padE5_muitos = std(dE5_muitos);
%dev_padE6_muitos = std(dE6_muitos);



% ###########################
% Parâmetro t de student (para um intervalo de confiança de 95% com 30 testes, ou seja, 29 graus de liberdade)
t_student = 2.045;

% ### Intervalos de confiança ###
% Poucos Processos
ic_E1_poucos = t_student*dev_padE1_poucos;
ic_E2_poucos = t_student*dev_padE1_poucos;
%ic_E3_poucos = t_student*dev_padE1_poucos;
ic_E4_poucos = t_student*dev_padE1_poucos;
%ic_E5_poucos = t_student*dev_padE1_poucos;
%ic_E6_poucos = t_student*dev_padE1_poucos;

% Médios Processos
ic_E1_medios = t_student*dev_padE1_medios;
ic_E2_medios = t_student*dev_padE1_medios;
%ic_E3_medios = t_student*dev_padE1_medios;
ic_E4_medios = t_student*dev_padE1_medios;
%ic_E5_medios = t_student*dev_padE1_medios;
%ic_E6_medios = t_student*dev_padE1_medios;

% Muitos Processos
ic_E1_muitos = t_student*dev_padE1_muitos;
ic_E2_muitos = t_student*dev_padE1_muitos;
%ic_E3_muitos = t_student*dev_padE1_muitos;
ic_E4_muitos = t_student*dev_padE1_muitos;
%ic_E5_muitos = t_student*dev_padE1_muitos;
%ic_E6_muitos = t_student*dev_padE1_muitos;


% ##### Vetor de intervalo de confiança (somente para facilitar a geração dos gráficos)
%ic_poucos = [ic_E1_poucos, ic_E2_poucos, ic_E3_poucos, ic_E4_poucos, ic_E5_poucos, ic_E6_poucos];
%ic_medios = [ic_E1_medios, ic_E2_medios, ic_E3_medios, ic_E4_medios, ic_E5_medios, ic_E6_medios];
%ic_muitos = [ic_E1_muitos, ic_E2_muitos, ic_E3_muitos, ic_E4_muitos, ic_E5_muitos, ic_E6_muitos];

ic_poucos = [ic_E1_poucos, ic_E2_poucos, ic_E4_poucos];
ic_medios = [ic_E1_medios, ic_E2_medios, ic_E4_medios];
ic_muitos = [ic_E1_muitos, ic_E2_muitos, ic_E4_muitos];


% ##### Gráfico

figura = figure(1);

% Sempre na ordem x, y, intervalo de confiança, parâmetros
% Não me lembro o que é o ~ Se quiser que apareça uma linha ligando os pontos, substitua o ~ por - para uma linha cheia ou por . para uma linha pontilhada
% O primeiro parâmetro é o tipo de símbolo. Pode ser . * x o s d ^ v > < p h
% O segundo parâmetro é a cor. Pode ser b w c k g r m
grafico = errorbar(E, m_poucos, ic_poucos, "~.b", E, m_medios, ic_medios, "~.g", E, m_muitos, ic_muitos, "~.r");


% Intervalo dos eixos
axis ([0, 7, -5, 430]);


% Para mudar a "grade"
% Valor de início : valor de fim : número de divisões (em quantas partes o intervalo será dividido)
%xtick = -1:1:33;

%set(gca, "xtick", xtick, "yminorgrid", "on");

% Para modificar o tamanho do "ponto" e a espessura da linha
set(grafico, "markersize", 4, "linewidth", 1.5);

% Para colocar a "grade
grid on;

% Para alterar o tamanho da fonte
FS = findall(figura,'-property','FontSize');
set(FS,'FontSize', 15);


% Título do gráfico
% O \bf deixa em negrito
title('Mudanca de contexto');

% Nome do eixo x
xlabel('Escalonador');

% Nome do eixo y
ylabel('Tempo');

% Legenda
legenda = legend('Poucos processos', 'Medios processos', 'Muitos processos');
% Posição da legenda
set (legenda, "location", "northwest", "fontweight", "bold");

% Para gerar uma figura em pdf
print figure1.pdf
