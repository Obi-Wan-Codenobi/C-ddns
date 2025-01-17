COMPILED_FILE_NAME=ddns

.PHONY: all
all: compile run

.PHONY: compile
compile:
	@echo "Compiling $(COMPILED_FILE_NAME) ..."
	gcc -Wall -O2 main.c -o $(COMPILED_FILE_NAME)

.PHONY: run
run: 
	@echo "Running $(COMPILED_FILE_NAME) ..."
	./$(COMPILED_FILE_NAME)

.PHONY: clean
clean: 
	@echo "Deleting files ..."
	rm $(COMPILED_FILE_NAME)
