cc = g++
ccopts = -Wall -O3 -g -std=c++11
lex = lex
lexopts =
lexgens = lex.yy.c
yacc = bison
yaccopts = -d
yaccgens = parser.tab.c parser.tab.h
libs = scanType.h node.h ourgetopt.h
gens = node.o ourgetopt.o
bin = parser
out = c-
arc = corg02

tst = cover

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
	vim $(bin).l $(bin).y node.h node.cpp scanType.h makefile

run: $(out)
	./$(out) testData/$(tst).c-

test: $(out)
	./$(out) testData/$(tst).c- > myout
	sdiff -a myout testData/$(tst).out

tar:
	tar -cf $(arc).tar $(bin).l $(bin).y *.cpp *.h makefile 
