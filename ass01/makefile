cc = g++
ccopts =#-ly
lex = lex
lexopts =
lexgens = lex.yy.c
yacc = yacc
yaccopts = -d
yaccgens = y.tab.c y.tab.h
prj = parser

$(prj): clean $(lexgens) $(yaccgens)
	clear
	$(cc) $(lexgens) $(yaccgens) $(ccopts) -o $(prj)

clean:
	rm -f $(lexgens) $(yaccgens) $(prj) $(prj).tab.h $(prj).tab.c

$(yaccgens): $(prj).y
	$(yacc) $(yaccopts) $(prj).y

$(lexgens): $(prj).l $(yaccgens)
	$(lex) $(lexopts) $(prj).l

edit:
	vim $(prj).l $(prj).y scanType.h
