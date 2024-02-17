
#define NOEVENT 0
#define LBUTTON 1
#define RBUTTON 2
#define UP 4
#define DOWN 8
#define LEFT 16
#define RIGHT 32
#define MOUSE_EVENTS 1
#define JOYSTICK_EVENTS 2
#define KEYBOARD_EVENTS 4
#define ENTER 0x1c
#define TABKEY 0x0f
#define UP_ARROW 0x48
#define DOWN_ARROW 0x50
#define RIGHT_ARROW 0x4d
#define LEFT_ARROW 0x4b

void init_events (void);	// Initialize event manager
void setmin (void);			// Set minimum joystick callibrations
void setmax (void);			// Set maximum joystick callibrations
void setcenter (void);		// Set center joystick callibrations
int getevent (int);	// Get events from selected devices
