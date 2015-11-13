function Intervalo()
	graphics_toolkit('gnuplot');
	clf;
	hold all;
	
	%% call one %%%%%
	one([2,2,3,4,5,2,5,8,8,4], 1);	
	one([2,2,3,4,5,2,5,8,8,4], 2);
	one([3,3,3,4,5,2,5,7,9,5], 3);		
	%%%%%%%%%%%%
	
	title(sprintf( 'Entrada: A B C\n[E = erro,  av = media]' ) );
	
	xlabel(['Tempo (s)']);
	ylabel(['# do Experimento']); 
	axis([0, 10,  0, 6], 'normal');
	grid on;
endfunction

function one(amostra, experimento)
	n = 10;
	raiz = sqrt(n);
	mi = mean(amostra);  % mean(col)
	sigma = std(amostra); %std(col)
	fator = (1.96*sigma)/raiz;
	vl = mi-fator;
	vd = mi+fator;
	
	x = [vl:0.1:vd];
	
	plot(x,  0*x + experimento);
	
	plot(vl, experimento, 'bo-');
	
	plot(mi, experimento, 'bo+');

	plot(vd, experimento,  'bo-');
	
	text(vl, experimento+0.2, cstrcat("E = " , num2str(fator)));
	text(vl, experimento-0.2, cstrcat("av = ", num2str(mi)));

endfunction