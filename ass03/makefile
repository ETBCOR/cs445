cc = g++
ccopts = -Wall -O3 -g -std=c++11
lex = lex
lexopts =
lexgens = lex.yy.c
yacc = bison
yaccopts = -d
yaccgens = parser.tab.c parser.tab.h
libs = main.h scanType.h node.h error.h ourgetopt.h symbolTable.h
gens = main.o node.o error.o ourgetopt.o symbolTable.o
bin = parser
out = c-
arc = corg03

tst = allgood
# allbad allgood array arrayops arrayx assignb assignBool
# assignChar assignInt break2 break3 break4 break call2
# call3 call callerr constants cover decl emptyline
# everythingF22 exp expL expR for2 fora forb
# for forscope funscope if ifNest init2 init3
# init4 init mixedControl2 mixedControl3 mixedControl nest ops
# optype overlapArray overlap overlapStatic part2 randomRoll return
# scope static strange types3 types undeclared uninit2
# uninit varAssign var varPair while whileif

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
	vim $(bin).l $(bin).y main.h main.cpp node.h node.cpp error.h error.cpp scanType.h makefile

run: $(out)
	./$(out) -p data/$(tst).c-

test: $(out)
	./$(out) -p data/$(tst).c- > myout
	sdiff -a myout data/$(tst).out

tar:
	tar -cf $(arc).tar $(bin).l $(bin).y *.cpp *.h makefile 
