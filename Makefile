COMPILED_FILE_NAME=ddns

# OpenSSL paths for linux
# OPENSSL_INCLUDE_PATH=/usr/include/openssl
# OPENSSL_LIB_PATH=/usr/lib/x86_64-linux-gnu

# OpenSSL paths for mac/homebrew
OPENSSL_INCLUDE_PATH=/opt/homebrew/opt/openssl@3/include
OPENSSL_LIB_PATH=/opt/homebrew/opt/openssl@3/lib

.PHONY: all
all: compile run

.PHONY: compile
compile:
	@echo "Compiling $(COMPILED_FILE_NAME) ..."
	gcc -Wall -O2 -I$(OPENSSL_INCLUDE_PATH) -L$(OPENSSL_LIB_PATH) main.c -o $(COMPILED_FILE_NAME) -lssl -lcrypto

.PHONY: run
run: 
	@echo "Running $(COMPILED_FILE_NAME) ..."
	./$(COMPILED_FILE_NAME)

.PHONY: clean
clean: 
	@echo "Deleting files ..."
	rm $(COMPILED_FILE_NAME)


.PHONY: help
help:
	@echo "Makefile commands:"
	@echo "  make                             - Compile main.c and run"
	@echo "  make compile                     - Compile main.c"
	@echo "  make run                         - Run the c-ddns executable"
	@echo "  make clean                       - Remove the executable"
	@echo "  make help                        - Show this help message"