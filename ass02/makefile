cc = g++
ccopts = -Wall -O3
lex = lex
lexopts =
lexgens = lex.yy.c
yacc = yacc
yaccopts = -d
yaccgens = y.tab.c y.tab.h
prj = parser
bin = c-
arc = corg02

tst = factor

$(bin): $(lexgens) $(yaccgens)
	$(cc) $(lexgens) $(yaccgens) $(ccopts) -o $(bin)

$(yaccgens): $(prj).y
	$(yacc) $(yaccopts) $(prj).y

$(lexgens): $(prj).l $(yaccgens)
	$(lex) $(lexopts) $(prj).l

clean:
	rm -f $(lexgens) $(yaccgens) $(bin) $(prj).tab.h $(prj).tab.c myout

edit:
	vim $(prj).l $(prj).y scanType.h makefile

run: $(bin)
	./$(bin) testData/$(tst).c-

test: $(bin)
	./$(bin) testData/$(tst).c- > myout
	sdiff -a myout testData/$(tst).out

tar:
	tar -cf $(arc).tar $(prj).l $(prj).y scanType.h makefile 
