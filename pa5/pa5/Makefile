SRC = src
FILES = $(SRC)/Makefile $(SRC)/cachesim.c

normal:
	./grader.py cachesim:fifo -s $(SRC)

extra:
	./grader.py cachesim:lru -s $(SRC)

all:
	./grader.py -s $(SRC)

1:
	./grader.py --stop -s $(SRC)

clean:
	rm -rf build pa4.tar

tree: $(FILES)

$(SRC)/Makefile:
	mkdir -p $(dir $@)
	cp data/template.mk $@

$(SRC)/cachesim.c:
	mkdir -p $(dir $@)
	sed '{s/USER/$(LOGNAME)/;}' data/stub.c > $@

pa5.tar: $(FILES)
	tar -vczf $@ $^

archive: pa5.tar
	./grader.py -a $^
