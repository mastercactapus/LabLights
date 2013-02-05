#define _HEADER   0b101
#define _REVISION 11
typedef enum //1 bit
{
	set,
	get
} CMD;

typedef enum //1 bit
{
	Front,
	Right
} LIGHT;

typedef enum // 3 bits
{
	auto_off,
	auto_dim,
	fade_set,
	fade_off,
	dim_level,
	current_level,
	resume
} SETGET;

typedef struct
{
	unsigned char header   : 3;
	CMD           command  : 1;
	SETGET        option   : 3;
	LIGHT         light    : 1;
	unsigned short PARAM   :16;
	unsigned char checksum : 8;
} MESSAGE;

typedef struct
{
	uint8_t  header           ;
	uint8_t  revision         ;
	uint16_t auto_off_delay   ; //seconds 0 = disabled
	uint16_t auto_dim_delay   ; //seconds 0 = disabled
	uint16_t fade_set_duration; //msec    0 = disabled
	uint16_t fade_off_duration; //msec    0 = disabled
	uint8_t  dim_level        ;
	uint8_t  current_level    ;
	uint8_t  resume_enable    ; //resume level on power-on
} SETTINGS;

const SETTINGS default_settings = {_HEADER,_REVISION,0,0,0,0,0,0,0};
