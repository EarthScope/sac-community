
#DEBUG=-g
CFLAGS=-Wall -Wextra $(DEBUG) -I. -I../../inc -D__TESTING__
PROG=expr
YACC=./lemon
OBJS=config.h expr_parse.o expr_lexer.o ../string/strings.o expr.o expr_util.o

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

lemon: lemon.c lempar.c
	$(CC) -Wall -Wextra -o lemon lemon.c

expr_lexer.c: lemon expr_lexer.l
expr_parse.c: lemon expr_parse.y
	$(YACC) -DMATH_NORMAL expr_parse.y
expr_parse_noop.c: lemon expr_parse.y
	cp -f expr_parse.y expr_parse_noop.y
	$(YACC) -DMATH_NONE expr_parse_noop.y
	rm -f expr_parse_noop.y expr_parse_noop.h

lib: expr_parse.c expr_lexer.c expr_parse_noop.c

.y.c:
	$(YACC) $<

config.h:
	touch config.h

clean:
	rm -rf $(PROG) $(PROG).dSYM $(OBJS) config.h *~ expr_parse*.out lemon

veryclean: clean
	rm -rf expr_parse.c expr_lexer.c expr_parse.h

test: $(PROG)
	./$(PROG) < test-expr.txt