void setup_serial(void);
void send_byte(uint8_t b);
void send_bytes(uint8_t *buf, uint8_t len);
uint8_t read_byte(void);

typedef struct {
	uint8_t byte;
	uint8_t flag;
} SBUF;

extern SBUF *s_buffer;

