.PHONY gen test build

gen:
	./codegen all

test:
	make gen
	gcc -o test/codegen-test test/main.c
	./codegen-test

build:
	gcc -o codegen main.c
	make test