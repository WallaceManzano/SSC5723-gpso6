EXEC = ./Main

#Compila e executa o Main.c
main:
	gcc -o Main Main.c
	$(EXEC)

compile:
	gcc -o Main Main.c

clean:
	rm -f Main

#Printa em tela todos os caracteres ascii presentes para print da imagem.
ascii:
	gcc -o Main Main.c
	$(EXEC) -ascii

#Testa para todas as imagens presente no vetor nomeImagens contino no Main.c
#e joga o resultado no arquivo Saida.txt
testAll:
	gcc -o Main Main.c
	$(EXEC) -testAll > Saida.txt

	
