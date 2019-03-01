# templates for flex & bison

collection of templates for flex/bison.

there are 12 basic types, from 3 different dimensions:
- only lexer, only parser, both lexer and parser.
- non-reentrant or reentrant. The reentrant variant is named *pure*, and so I named the alternative *impure*
- with default ```yy``` prefix, or with a custom prefix ```_cust```, to more than one parser coexist in the same program


Only parser really means without flex parser, and it is meant to understand what bison does and for uses in other fields.
For other fields, see for example "Speech and Language Processing", by Dan Jurafsky and James H. Martin.


Other options to check:
- use locations
- use AST
(see also todo.txt)

good point to start:
- "flex & bison", by John Levine
