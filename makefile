
all:
	make clean
	make prepare
	make minic

prepare: minic.l minic.y
	flex -o minic_lex.c --header-file=minic_lex.h minic.l
	bison -d -o minic_yacc.c minic.y
	
minic:minic_lex.c minic_yacc.c option.c main.c
	gcc -o minic minic_lex.c minic_yacc.c option.c main.c
	
clean:
	rm -rf *.o
	rm -rf *.dSYM
	rm -f minic_lex.c minic_lex.h
	rm -f minic_yacc.c minic_yacc.h
	rm -f minic 
	rm -f tree.png
	rm -f tree.gv

ast:
	dot -Tpng tree.gv -o tree.png

flowGraph:
	dot -Tpng flowGraph.gv -o flowGraph.png




