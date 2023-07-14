cc = g++
ccopts = -Wall -O3 -g -std=c++11
lex = lex
lexopts =
lexgens = lex.yy.c
yacc = bison
yaccopts = -d
yaccgens = parser.tab.c parser.tab.h
libs = main.h scanType.h node.h error.h sem.h ourgetopt.h symbolTable.h
gens = main.o node.o error.o sem.o ourgetopt.o symbolTable.o
bin = parser
out = c-
arc = corg04

tst = io

# allbad.c-      assignInt.c-   call3.c-      decl.c-           expL.c-       gcd2.c-    init5.c-      loopscope2.c-     nested.c-         param.c-         returnbad.c-    tinybad.c-     uninit2.c-
# allgood.c-     badarray.c-    call4.c-      decllists.c-      expR.c-       gcd.c-     initbad2.c-   loopscope.c-      nestedfors.c-     paramerr2.c-     return.c-       tiny.c-        uninit.c-
# array2.c-      badcontrol.c-  call.c-       defused.c-        for2.c-       if2.c-     initbad.c-    matchunmatch.c-   notused.c-        paramerr3.c-     scope.c-        tinygood.c-    uninitialized.c-
# array.c-       basicAll5.c-   callerr.c-    emptyline.c-      fora.c-       if.c-      init.c-       minus.c-          oparray.c-        paramerr.c-      simple.c-       tinyscope.c-   unused.c-
# arrayops.c-    break2.c-      compfunc.c-   everythingF22.c-  forb.c-       ifevil.c-  initconst.c-  mixedControl2.c-  ops.c-            part2.c-         small.c-        tinyType.c-    varAssign.c-
# arrayx.c-      break3.c-      constants.c-  exp2.c-           for.c-        ifNest.c-  initused.c-   mixedControl3.c-  optype.c-         precassoc.c-     static.c-       types3.c-      var.c-
# assignb.c-     break4.c-      control2.c-   exp3.c-           forerr.c-     init2.c-   io.c-         mixedControl.c-   overlapArray.c-   randomRoll.c-    strange.c-      types.c-       varPair.c-
# assignBool.c-  break.c-       control.c-    exp4.c-           forscope.c-   init3.c-   lhs.c-        nano.c-           overlap.c-        rangeifwhile.c-  stress.c-       undeclared.c-  while.c-
# assignChar.c-  call2.c-       cover.c-      exp.c-            funcscope.c-  init4.c-   loops.c-      nest.c-           overlapStatic.c-  redefined.c-     testExample.c-  undefined.c-   whileif.c-

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
	vim $(bin).l $(bin).y main.h main.cpp node.h node.cpp error.h error.cpp sem.h sem.cpp scanType.h makefile

run: $(out)
	./$(out) -P data/$(tst).c-

test: $(out)
	./$(out) -P data/$(tst).c- > myout
	diff -y -a -W 200 myout data/$(tst).out

tar:
	tar -cf $(arc).tar $(bin).l $(bin).y *.cpp *.h makefile 
