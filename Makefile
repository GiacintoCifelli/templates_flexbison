help:
	@echo "    use: make [l|p|m][ip][dr]"
	@echo "               l                  lexer only"
	@echo "                 p                parser only (with handwritten dummy lexer)"
	@echo "                   m              mix: both lexer and parser"
	@echo "                      i           non-reentrant (impure)"
	@echo "                       p          reentrant (pure)"
	@echo "                          d       default naming yy*"
	@echo "                           r      renamed as cust_*"
	@echo "    examples:"
	@echo "        make lid"
	@echo "        make mpr"
	@echo "    or run:"
	@echo "        make all - to build all 12 (3*2*2) of them"
all: lid lir lpd lpr pid pir ppd ppr mid mir mpd mpr

lid: lid.l
	flex lid.l
	gcc -g -o lid lid_lexer.c

pid: pid.y
	bison pid.y
	gcc -g -o pid pid_parser.c

mid: mid.l mid.y
	bison mid.y
	flex mid.l
	gcc -g -o mid mid_lexer.c mid_parser.c

check:
ifndef prog
	@echo "    use:   make check prog=[l|p|m][ip][dr]"
	@echo "           ./test [l|p|m][ip][dr]"
	@echo "    examples:"
	@echo "        make check prog=lid"
	@echo "        ./test lid"
else
	cd tests; ../$(prog) < test.c
endif

clean:
	@touch bogus_parser.c bogus_parser.h bogus_lexer.c lid
	rm *_parser.c *_parser.h *_lexer.c [lpm][ip][dr]

