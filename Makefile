help:
	@echo "make [l|p|m][ip][dr]"
	@echo "      l                  lexer only"
	@echo "        p                parser only (with handwritten dummy lexer)"
	@echo "          m              mix: both lexer and parser"
	@echo "             i           non-reentrant (impure)"
	@echo "              p          reentrant (pure)"
	@echo "                 d       default naming yy*"
	@echo "                  r      renamed as cust_*"
	@echo "examples:"
	@echo "    make lid"
	@echo "    make mpr"
	@echo "or run:"
	@echo "    make all - to build all 12 (3*2*2) of them"
all: lid lir lpd lpr pid pir ppd ppr mid mir mpd mpr

lid:
	flex lid.l
	gcc -g -o lid lid_lexer.c

pid:
	bison pid.y
	gcc -g -o pid pid_parser.c

mid:
	flex mid.l
	bison mid.y
	gcc -g -o mid mid_lexer.c mid_parser.c

check:
	cd tests; ../basic < test.c

clean:
	rm *_parser.c *_lexer.c [lpm][ip][dr]
