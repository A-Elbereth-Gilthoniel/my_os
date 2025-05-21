enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

typedef struct {
    size_t row;
    size_t column;
    uint8_t color;
    uint16_t* buffer;
	char command[50];
} terminal;


uint8_t vga_entry_color(enum vga_color front_color, enum vga_color background_color);
static inline uint16_t vga_entry(unsigned char symb, uint8_t color);
void terminal_initialize();
void terminal_writestring(const char* text);
void terminal_putchar(char c);
char* uint_to_string(unsigned int value);
void terminal_write(const char* data, size_t size);
void getline(char* buffer, uint32_t row, uint32_t n);
static inline char vga_decode(uint16_t pixel);
void move_cursor(uint16_t position);
