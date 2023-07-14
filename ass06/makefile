cc = g++
ccopts = -Wall -O3 -g -std=c++11
lex = lex
lexopts =
lexgens = lex.yy.c
yacc = bison
yaccopts = -d
yaccgens = parser.tab.c parser.tab.h
libs = main.h node.h error.h sem.h yyerror.h ourgetopt.h symbolTable.h scanType.h
gens = main.o node.o error.o sem.o yyerror.o ourgetopt.o symbolTable.o
bin = parser
out = c-
arc = corg06-06

tst = fora

$(out): $(lexgens) $(yaccgens) $(gens)
	$(cc) $(ccopts) $(gens) $(lexgens) $(yaccgens) -o $(out)

$(yaccgens): $(bin).y
	$(yacc) $(yaccopts) $(bin).y

$(lexgens): $(bin).l $(yaccgens)
	$(lex) $(lexopts) $(bin).l

%.o: %.cpp $(libs)
	$(cc) $(ccopts) -c $<

clean:
	rm -f $(lexgens) $(yaccgens) $(out) $(bin).tab.h $(bin).tab.c $(gens) myout

edit:
	vim $(bin).l $(bin).y main.h main.cpp node.h node.cpp error.h error.cpp sem.h sem.cpp yyerror.h yyerror.cpp symbolTable.h symbolTable.cpp scanType.h makefile

run: $(out)
	./$(out) -P data/$(tst).c-

test: $(out)
	./$(out) -M data/$(tst).c- > myout
	diff -y -a -W 200 myout data/$(tst).out

tar:
	tar -cf $(arc).tar $(bin).l $(bin).y *.cpp *.h makefile 
