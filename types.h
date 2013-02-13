#define _DEFAULT_SETTINGS {0,0,0,0,0xffff}

typedef enum {
	NOP,
	DIM_LEVEL,
	POWER_LEVEL,
	STATUS,
	DIM_DELAY,
	OFF_DELAY
}COMMAND;

typedef enum {
	OFF,
	ON ,
	DIM 
}POWER_STATUS;

typedef struct 
{
	uint8_t read_write :1;
	uint8_t light_AB   :1;
	COMMAND command    :6;
	uint16_t value;
	uint8_t chk; //sum of all + 43
}REQUEST;

typedef struct 
{
	volatile uint8_t  auto_off_delay; //minutes 0 = disabled
	volatile uint8_t  auto_dim_delay; //minutes 0 = disabled
	volatile uint8_t  power_level   ; //power level when on
	volatile uint8_t  dim_level     ; //power level to dim (down) to
	volatile uint16_t checksum      ;
}LIGHT_SETTINGS;


typedef struct 
{
	LIGHT_SETTINGS *settings;
	volatile POWER_STATUS    status;
  volatile uint8_t         minutes ;
}LIGHT;

extern LIGHT *lightA;
extern LIGHT *lightB;
