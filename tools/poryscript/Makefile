# Poryscript 3.6.1 (https://github.com/huderlem/poryscript), fetched as a
# checksum-pinned release for the host platform. The binary is ignored by Git;
# `make tools` (or `make -C tools/poryscript`) downloads it. font_config.json
# and command_config.json are tracked here and passed on every compile.

VERSION := 3.6.1
BASE_URL := https://github.com/huderlem/poryscript/releases/download/$(VERSION)

UNAME_S := $(shell uname -s)
ifeq ($(OS),Windows_NT)
  ZIP := poryscript-windows.zip
  SHA256 := 6a1ad9f54b1621b2b13f5cfc3c78f56c2c8bcb4e54afafbd82b5a430d35cd477
  BIN := poryscript.exe
else ifeq ($(UNAME_S),Darwin)
  ZIP := poryscript-mac.zip
  SHA256 := 7e211ef02a5b966f385f8ee7a7a10fff3caba5288df9837c2388519ecbb7422f
  BIN := poryscript
else
  ZIP := poryscript-linux.zip
  SHA256 := 249f9adeb3b6d7f2e33347c1d0bce5f3ff8fb06e914c75b81d08187f8404c4ff
  BIN := poryscript
endif
DIR := $(basename $(ZIP))

.PHONY: all clean

all: $(BIN)

$(ZIP):
	curl -fsSL -o $@ $(BASE_URL)/$(ZIP)

$(BIN): $(ZIP)
	@echo "$(SHA256)  $(ZIP)" | (sha256sum -c - >/dev/null 2>&1 || shasum -a 256 -c - >/dev/null)
	unzip -o -q $(ZIP) '$(DIR)/$(BIN)'
	mv -f $(DIR)/$(BIN) $(BIN)
	rmdir $(DIR) 2>/dev/null || true
	chmod +x $(BIN)
	./$(BIN) -v

clean:
	$(RM) -r poryscript poryscript.exe poryscript-linux poryscript-mac poryscript-windows *.zip
