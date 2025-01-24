COMPILED_FILE_NAME=ddns
BIN_BINARY=/usr/local/bin/ddns
SYSTEM_MD_FILE=/etc/systemd/system/ddns.service
# Echo with sudo. Doing it line by line since makefile 
# 	does not seem to support multiline strings
TEE_CMD = sudo tee -a $(SYSTEM_MD_FILE) > /dev/null

# OpenSSL paths for linux / raspberian (I used w/ pizero W)
OPENSSL_INCLUDE_PATH=/usr/include/openssl
OPENSSL_LIB_PATH=/usr/lib/x86_64-linux-gnu

# OpenSSL paths for mac/homebrew
#OPENSSL_INCLUDE_PATH=/opt/homebrew/opt/openssl@3/include
#OPENSSL_LIB_PATH=/opt/homebrew/opt/openssl@3/lib

.PHONY: all
all: compile run

.PHONY: compile
compile:
	@echo "Compiling $(COMPILED_FILE_NAME) ..."
	gcc -Wall -O3 -I$(OPENSSL_INCLUDE_PATH) -L$(OPENSSL_LIB_PATH) main.c -o $(COMPILED_FILE_NAME) -lssl -lcrypto

.PHONY: run
run: 
	@echo "Running $(COMPILED_FILE_NAME) ..."
	./$(COMPILED_FILE_NAME)

.PHONY: clean
clean: 
	@echo "Deleting files ..."
	rm $(COMPILED_FILE_NAME)

.PHONY: daemon
daemon: compile
	sudo cp $(COMPILED_FILE_NAME) $(BIN_BINARY) 
	sudo chmod +x $(BIN_BINARY)
	sudo rm -f $(SYSTEM_MD_FILE)
	@echo "Creating/Overwriting systemd service file..."
	echo "[Unit]" | $(TEE_CMD)
	echo "Description=DDNS Daemon" | $(TEE_CMD)
	echo "After=network-online.target" | $(TEE_CMD)
	echo "Wants=network-online.target" | $(TEE_CMD)
	echo "" | $(TEE_CMD)
	echo "[Service]" | $(TEE_CMD)
	echo "ExecStart=$(BIN_BINARY)" | $(TEE_CMD)
	echo "Restart=always" | $(TEE_CMD)
	echo "User=root" | $(TEE_CMD)
	echo "WorkingDirectory=/usr/local/bin" | $(TEE_CMD)
	echo "StandardOutput=null" | $(TEE_CMD)
	echo "StandardError=journal" | $(TEE_CMD)
	echo "" | $(TEE_CMD)
	echo "[Install]" | $(TEE_CMD)
	echo "WantedBy=multi-user.target" | $(TEE_CMD)
	sudo systemctl daemon-reload
	sudo systemctl enable ddns.service
	sudo systemctl start ddns.service
	sudo systemctl status ddns.service



.PHONY: help
help:
	@echo "Makefile commands:"
	@echo "  make                             - Compile main.c and run"
	@echo "  make compile                     - Compile main.c"
	@echo "  make run                         - Run the c-ddns executable"
	@echo "  make clean                       - Remove the executable"
	@echo "  make daemon                      - Set up and start the DDNS daemon"
	@echo "  make help                        - Show this help message"
