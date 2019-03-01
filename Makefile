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

#non-reentrant, default-named
lid: lid.l
	flex lid.l
	gcc -g -o lid lid_lexer.c

pid: pid.y
	bison -x pid.y
	xsltproc `bison --print-datadir`/xslt/xml2xhtml.xsl pid_parser.xml >pid.html
	gcc -g -o pid pid_parser.c

mid: mid.l mid.y
	bison -x mid.y
	xsltproc `bison --print-datadir`/xslt/xml2xhtml.xsl mid_parser.xml >mid.html
	flex mid.l
	gcc -g -o mid mid_lexer.c mid_parser.c

#non-reentrant, custom-named
lir: lir.l
	flex lir.l
	gcc -g -o lir lir_lexer.c

pir: pir.y
	bison -x pir.y
	xsltproc `bison --print-datadir`/xslt/xml2xhtml.xsl pir_parser.xml >pir.html
	gcc -g -o pir pir_parser.c

mir: mir.l mir.y
	bison -x mir.y
	xsltproc `bison --print-datadir`/xslt/xml2xhtml.xsl mir_parser.xml >mir.html
	flex mir.l
	gcc -g -o mir mir_lexer.c mir_parser.c

#reentrant, default-named
lpd: lpd.l
	flex lpd.l
	gcc -g -o lpd lpd_lexer.c

ppd: ppd.y
	bison -x ppd.y
	xsltproc `bison --print-datadir`/xslt/xml2xhtml.xsl ppd_parser.xml >ppd.html
	gcc -g -o ppd ppd_parser.c

mpd: mpd.l mpd.y
	bison -x mpd.y
	xsltproc `bison --print-datadir`/xslt/xml2xhtml.xsl mpd_parser.xml >mpd.html
	flex mpd.l
	gcc -g -o mpd mpd_lexer.c mpd_parser.c

#reentrant, custom-named
lpr: lpr.l
	flex lpr.l
	gcc -g -o lpr lpr_lexer.c

ppr: ppr.y
	bison -x ppr.y
	xsltproc `bison --print-datadir`/xslt/xml2xhtml.xsl ppr_parser.xml >ppr.html
	gcc -g -o ppr ppr_parser.c

mpr: mpr.l mpr.y
	bison -x mpr.y
	xsltproc `bison --print-datadir`/xslt/xml2xhtml.xsl mpr_parser.xml >mpr.html
	flex mpr.l
	gcc -g -o mpr mpr_lexer.c mpr_parser.c


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
	@touch bogus_parser.c bogus_parser.h bogus_lexer.c bogus_lexer.h lid bogus.xml bogus.html
	rm *_parser.[ch] *_lexer.[ch] [lpm][ip][dr] *.xml *.html

