SRC_DIR=src
BUILD_DIR=build
SOURCES=$(wildcard $(SRC_DIR)/*.c)
OBJECTS=$(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

CC=clang
CFLAGS=-lX11

# beauty vars
GREEN=\e[1;32m
BLUE=\e[1;34m
RED=\e[1;31m
CYAN=\e[1;36m
NC=\e[0m

build: $(BUILD_DIR)/xivonia


# linker
$(BUILD_DIR)/xivonia: $(OBJECTS)
	@echo "${BLUE}LINK FILES:${NC}"
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# compiler
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "${GREEN}COMPILE $<:${NC}"
	mkdir -p $(BUILD_DIR)
	$(CC) -c $< -o $@


# run in xephyr
run: $(BUILD_DIR)/xivonia
	@echo "${CYAN}RUN $< IN XEPHYR:${NC}"
	Xephyr -br -ac -noreset -screen 800x600 :1 &
	sleep 0.3	# wait for xephyr to start up
	DISPLAY=:1 ./$<

xephyr:
	@echo "${CYAN}RUN ONLY XEPHYR:${NC}"
	Xephyr -br -ac -noreset -screen 800x600 :1 &

launch-wm: $(BUILD_DIR)/xivonia
	@echo "${CYAN}LAUNCH WM ON DISPLAY :1${NC}"
	DISPLAY=:1 ./$<

# remove bin
clean:
	@echo "${RED}CLEAN BUILD DIR:${NC}"
	rm -f $(BUILD_DIR)/*