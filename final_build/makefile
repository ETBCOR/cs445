cc = g++
ccopts = -Wall -O3 -g -std=c++11
lex = lex
lexopts =
lexgens = lex.yy.c
yacc = bison
yaccopts = -d
yaccgens = parser.tab.c parser.tab.h
libs = main.h node.h error.h yyerror.h ourgetopt.h symbolTable.h emitcode.h scanType.h
gens = main.o node.o error.o yyerror.o ourgetopt.o symbolTable.o emitcode.o
bin = parser
out = c-
ver = corg07-20

path = data/UnitTests
file = c05

dif = diffs/$(file)-$(ver)-diff.txt

$(out): $(lexgens) $(yaccgens) $(gens) tm
	$(cc) $(ccopts) $(gens) $(lexgens) $(yaccgens) -o $(out)

$(yaccgens): $(bin).y
	$(yacc) $(yaccopts) $(bin).y

$(lexgens): $(bin).l $(yaccgens)
	$(lex) $(lexopts) $(bin).l

%.o: %.cpp $(libs)
	$(cc) $(ccopts) -c $<

tm:
	$(cc) -O3 -std=c++11 -o tm tm.c

clean:
	rm -f $(lexgens) $(yaccgens) $(out) $(bin).tab.h $(bin).tab.c $(gens) tm myout *.mem *.out

edit:
	vim $(bin).l $(bin).y main.h main.cpp node.h node.cpp error.h error.cpp yyerror.h yyerror.cpp symbolTable.h symbolTable.cpp emitcode.h emitcode.cpp scanType.h makefile

test: $(out) tm
	./$(out) $(path)/$(file).c- > $(file).out
	./$(out) -M $(path)/$(file).c- > $(file).mem
	./tm $(path)/$(file) < $(path)/runExamples.in >> $(file).out
	echo "SCRIPT RUNNING WITH \"$(file)\"" > $(dif)
	cat $(path)/$(file).c- >> $(dif)
	echo "---- MEM DIFF ----" >> $(dif)
	-sdiff -a -w200 $(file).mem $(path)/$(file).mem.f21 >> $(dif)
	echo "---- TMCODE DIFF ----" >> $(dif)
	-sdiff -a -w200 $(path)/$(file).tm $(path)/$(file).tm.f21 >> $(dif)
	echo "---- OUTPUT DIFF ----" >> $(dif)
	-sdiff -a -w200 $(file).out $(path)/$(file).expected >> $(dif)
	echo "END DIFFILE GEN"
	clear && cat $(dif)

tar:
	tar -cf backups/$(ver).tar $(bin).l $(bin).y tm.c *.cpp *.h makefile 
