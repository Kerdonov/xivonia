# beauty vars
GREEN=\e[1;32m
BLUE=\e[1;34m
RED=\e[1;31m
CYAN=\e[1;36m
NC=\e[0m

build: xivonia.bin

# main file
xivonia.bin: main.c
	@echo "${GREEN}COMPILE $<:${NC}"
	gcc $< -o $@ -lX11


# run in xephyr
run: xivonia.bin
	@echo "${CYAN}RUN $< IN XEPHYR:${NC}"
	Xephyr -br -ac -noreset -screen 800x600 :1 &
	sleep 0.3	# wait for xephyr to start up
	DISPLAY=:1 ./$<

xephyr:
	@echo "${CYAN}RUN ONLY XEPHYR:${NC}"
	Xephyr -br -ac -noreset -screen 800x600 :1 &

launch-wm: xivonia.bin
	@echo "${CYAN}LAUNCH WM ON DISPLAY :1${NC}"
	DISPLAY=:1 ./$<

# remove bin
clean:
	@echo "${RED}CLEAN BUILD FILES:${NC}"
	rm -f xivonia.bin