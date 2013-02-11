void setup_serial(void);
void send_byte(uint8_t b);
void send_bytes(uint8_t *buf, uint8_t len);
uint8_t read_byte(void);
void _read_byte(void);


extern uint8_t data_flag;
extern uint8_t data_byte;
