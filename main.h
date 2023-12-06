#include <X11/Xlib.h>

// * log macros
#define errorlog(...) { fprintf(stderr, ANSI_COLOR_RED "ERROR: " ANSI_COLOR_RESET); fprintf(stderr, __VA_ARGS__); }
#define simplelog(...) { printf(ANSI_COLOR_CYAN "LOG: " ANSI_COLOR_RESET); printf(__VA_ARGS__); }
#define warninglog(...) { printf(ANSI_COLOR_YELLOW "WARNING: " ANSI_COLOR_RESET); printf(__VA_ARGS__); }

typedef struct {
    char* name;
    Display* display;
    // todo if needed make const with stupid memcpy hacks
    Window root;
} WindowManager;

WindowManager* wm;

// * wm functions
WindowManager* create_wm();
void close_wm(WindowManager* wm);
void run_wm(WindowManager* wm);

// * event handlers


// ! error handlers
int on_wm_detected(Display* display, XErrorEvent* e);
int on_x_error(Display* display, XErrorEvent* e);