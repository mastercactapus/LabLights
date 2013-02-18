#define BUFFER 12

void setup_serial(void);
void send_byte(uint8_t b);
void send_bytes(uint8_t *buf, uint8_t len);
uint8_t read_byte(void);
void read_bytes(uint8_t *buf, uint8_t len);
char* read_line(uint8_t maxlen);
void send_string(char *str);
uint8_t peek_byte(void);
void flush(void);

typedef struct {
	volatile uint8_t byte[BUFFER];
	volatile uint8_t count;
} SBUF;

extern SBUF *s_buffer;

