#ifndef _BSC5_NOTES_H_
#define _BSC5_NOTES_H_ 1

#include <stdint.h>
#include <WString.h>
#include <list>
#include <ArduinoJson.h>

extern const uint8_t bsc5_ybsc5_notes_start[] asm("_binary_bsc5_ybsc5_notes_start");
extern const uint8_t bsc5_ybsc5_notes_end[] asm("_binary_bsc5_ybsc5_notes_end");

class Note {
public:
	Note(const uint16_t id, const String description):
		id(id), description(description) {}

	JsonObject toJson(JsonVariant parent);

	uint16_t id;
	String description;
};

class BSC5Notes {
public:
	BSC5Notes(const uint8_t *start, const uint8_t *end);
	std::list<Note> search(const String query);

private:
	const uint8_t *_start;
	const uint8_t *_end;
};

extern BSC5Notes bsc5_notes;

#endif /* _BSC5_NOTES_H_ */
