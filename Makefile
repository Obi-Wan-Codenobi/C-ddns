COMPILED_FILE_NAME=ddns

# OpenSSL paths for linux
# OPENSSL_INCLUDE_PATH=/usr/include/openssl
# OPENSSL_LIB_PATH=/usr/lib/x86_64-linux-gnu

# OpenSSL paths for mac/homebrew
# OPENSSL_INCLUDE_PATH=/opt/homebrew/include/openssl
# OPENSSL_LIB_PATH=/opt/homebrew/opt

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









# ###### OLD MAKE FILE - need to integrate into current file
# # OpenSSL paths for linux
# OPENSSL_INCLUDE_PATH = /usr/include/openssl
# OPENSSL_LIB_PATH = /usr/lib/x86_64-linux-gnu

# # OpenSSL paths for mac/homebrew
# OPENSSL_INCLUDE_PATH = /opt/homebrew/include/openssl
# OPENSSL_LIB_PATH = /opt/homebrew/opt

# CC = gcc
# CFLAGS = -I$(OPENSSL_INCLUDE_PATH)
# LDFLAGS = -L$(OPENSSL_LIB_PATH) -lcrypto -lssl

# # Executable
# SERVER = server
# PINCRACKTEST = pincracktest

# # Source 
# SERVER_SRC = server.c
# PINCRACK_SRC = pincrack.c
# PINCRACKTEST_SRC = pincracktest.c

# # Object 
# SERVER_OBJ = $(SERVER_SRC:.c=.o)
# PINCRACK_OBJ = $(PINCRACK_SRC:.c=.o)
# PINCRACKTEST_OBJ = $(PINCRACKTEST_SRC:.c=.o)

# all: $(SERVER) $(PINCRACKTEST)

# # Linking for server
# $(SERVER): $(SERVER_OBJ)
# 	$(CC) $(CFLAGS) -o $@ $(SERVER_OBJ) $(LDFLAGS)

# # Linking for pincracktest
# $(PINCRACKTEST): $(PINCRACKTEST_SRC) $(PINCRACK_OBJ)
# 	$(CC) $(CFLAGS) -o $@ $(PINCRACKTEST_SRC) $(PINCRACK_OBJ) $(LDFLAGS)

# # Compilation
# %.o: %.c
# 	$(CC) $(CFLAGS) -c -o $@ $<

# # Clean up
# clean:
# 	rm -f $(SERVER) $(PINCRACKTEST) $(SERVER_OBJ) $(PINCRACK_OBJ) $(PINCRACKTEST_OBJ)

