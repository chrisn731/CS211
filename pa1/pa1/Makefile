ROMAN      = src/roman/Makefile src/roman/roman.c
PALINDROME = src/palindrome/Makefile src/palindrome/palindrome.c
RLE        = src/rle/Makefile src/rle/rle.c
LIST       = src/list/Makefile src/list/list.c
MEXP       = src/mexp/Makefile src/mexp/mexp.c
BST        = src/bst/Makefile src/bst/bst.c
SOURCES    = $(ROMAN) $(PALINDROME) $(RLE) $(LIST) $(MEXP) $(BST)

fulltest:
	./grader.py

1:
	./grader.py -1

clean:
	rm -rf obj pa1.tar

pa1.tar: $(SOURCES)
	tar -vzcf $@ $^

archive: pa1.tar
	./grader.py -a $^