all: clean bison flex swig compile

cpp: clean bison flex swig compile compile_cpp

debug_parser: clean bison flex swig compile_debug_parser compile_cpp

bison: tecplot_loader.y
	bison -d tecplot_loader.y

flex: tecplot_loader.l tecplot_loader.tab.h
	flex tecplot_loader.l

swig: tecplot_loader.i
	swig -Wall -c++ -python tecplot_loader.i

compile: lex.yy.c tecplot_loader.tab.c tecplot_loader.tab.h
	g++ -fpic -Wall -Wextra -shared zone.cpp mesh.cpp tecplot_loader.tab.c lex.yy.c -lfl -o libparser.so
	g++ -fpic -Wall -Wextra -shared tecplot_loader_wrap.cxx -o _tecplot_loader.so -L. -lparser -lfl -I/usr/include/python2.7/

compile_debug_parser: lex.yy.c tecplot_loader.tab.c tecplot_loader.tab.h
	g++ -g -fpic -Wall -Wextra -shared zone.cpp mesh.cpp tecplot_loader.tab.c lex.yy.c -DYYDEBUG=1 -lfl -o libparser.so
	g++ -g -fpic -Wall -Wextra -shared tecplot_loader_wrap.cxx -o _tecplot_loader.so -L. -lparser -lfl -I/usr/include/python2.7/

compile_cpp: libparser.so main.cpp
	g++ -g zone.cpp mesh.cpp main.cpp -L. -lparser -o tecplot_loader

clean:
	rm -rf *.o *.so *.pyc lex.yy.c *.tab.c *.tab.h *_wrap.cxx tecplot_loader tecplot_loader.py __pycache__/
