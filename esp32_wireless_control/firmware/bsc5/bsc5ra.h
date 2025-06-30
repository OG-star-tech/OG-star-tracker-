#ifndef _BSC5RA_H_
#define _BSC5RA_H_ 1

#include <stdint.h>

typedef struct bsc5_header {
    int32_t star0;
    int32_t star1;
    int32_t starn;
    int32_t stnum;
    int32_t mprop;
    int32_t nmag;
    uint32_t nbent;
} __attribute__ ((__packed__)) bsc5_header_t;

typedef struct bsc5_entry {
    union {
    	uint32_t _xno;
    	float xno;
    };
    union {
    	uint64_t _sra0;
    	double sra0;
    };
    union {
    	uint64_t _sdec0;
    	double sdec0;
    };
    char is[2];
    uint16_t mag;
    union {
    	uint32_t _xrpm;
    	float xrpm;
    };
    union {
    	uint32_t _xdpm;
    	float xdpm;
    };
} __attribute__ ((__packed__)) bsc5_entry_t;

extern const uint8_t bsc5_BSC5ra_bsc5_start[] asm("_binary_bsc5_BSC5ra_bsc5_start");
extern const uint8_t bsc5_BSC5ra_bsc5_end[] asm("_binary_bsc5_BSC5ra_bsc5_end");

class BSC5 {
public:
	BSC5(const uint8_t *start, const uint8_t *end);
	void printHeader();
	void printStar(int32_t index);

private:
	const uint8_t *_start;
	const uint8_t *_end;
};

#endif /* _BSC5RA_H_ */
