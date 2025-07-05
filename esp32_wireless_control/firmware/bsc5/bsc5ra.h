#ifndef _BSC5RA_H_
#define _BSC5RA_H_ 1

#include <stdint.h>
#include <WString.h>
#include <optional>

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

class Entry {
public:
	Entry(float xno, double sra0, double sdec0, char is[2], uint16_t mag, float xrpm, float xdpm):
		xno(xno), sra0(sra0), sdec0(sdec0), is(is), mag(mag), xrpm(xrpm), xdpm(xdpm)
	{
	}
	float xno;
	double sra0;
	double sdec0;
	String is;
	uint16_t mag;
	float xrpm;
	float xdpm;
//	static Entry newFromIndex(int32_t index);
	void print();
};

class BSC5 {
public:
	BSC5(const uint8_t *start, const uint8_t *end);
	void printHeader();
	void printStar(int32_t index);
	std::optional<Entry> findByXno(float xno);

private:
	const uint8_t *_start;
	const uint8_t *_end;
};

extern BSC5 bsc5;

#endif /* _BSC5RA_H_ */
