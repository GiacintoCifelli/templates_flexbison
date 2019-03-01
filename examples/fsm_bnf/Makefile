all: vending v2

vending: vending.l vending.y
	bison -x --graph vending.y
	xsltproc `bison --print-datadir`/xslt/xml2xhtml.xsl vending_parser.xml >vending.html
	dot -Tsvg -ovending.svg vending_parser.dot
	flex vending.l
	gcc -g -o vending vending_lexer.c vending_parser.c

v2: v2.l v2.y
	bison -x --graph v2.y
	xsltproc `bison --print-datadir`/xslt/xml2xhtml.xsl v2_parser.xml >v2.html
	dot -Tsvg -ov2.svg v2_parser.dot
	flex v2.l
	gcc -g -o v2 v2_lexer.c v2_parser.c

clean:
	@touch bogus_parser.c bogus_parser.h bogus_lexer.c bogus_lexer.h vending bogus.xml bogus.html vending.svg vending_parser.dot
	rm *_parser.[ch] *_lexer.[ch] vending v2 *.xml *.html *.svg *.dot
