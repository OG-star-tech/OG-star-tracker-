#include "bsc5_notes.h"
#include "uart.h"
#include "unishox2.h"

BSC5Notes bsc5_notes(bsc5_ybsc5_notes_start, bsc5_ybsc5_notes_end);

BSC5Notes::BSC5Notes(const uint8_t *start, const uint8_t *end):
	_start(start), _end(end)
{

}

std::list<Note> BSC5Notes::search(const String query)
{
	std::list<Note> notes;

	int stop = 0;
	int id = 0;
	char flags[10] = {0};
	char description[200] = {0};
	int n;
//	unsigned long int maxstrlen = 0;
//	unsigned long int maxstrlen2 = 0;
//	unsigned long int total = 0, total2 = 0;
//	unsigned long int names_count = 0;
//	char output_buf[200] = {0};

	const char* current_pos = (const char *)_start;

	while(!stop && current_pos < (const char *)_end)
	{
		if((n = sscanf(current_pos, "%d %[^:]: %[^\n]", &id, flags, description)) != 3)
		{
			if(n != -1)
			{
				print_out_nonl("Error parsing input: %d\n", n);
			}
			stop = 1;
			break;
		}
		current_pos = strchr(current_pos, '\n') + 1;

		if (strcmp(flags, "1N") != 0)
		{
			continue;
		}
		if (strstr(description, "See HR") != NULL)
		{
			continue;
		}

		if(strcasestr(description, query.c_str()) != NULL)
		{
			Note note(id, String(description));
			notes.push_front(note);
		}

//		names_count++;
//		bzero(output_buf, 200);
//		if((n = unishox2_compress_simple(description, strlen(description), output_buf)) == -1) {
//			print_out_nonl("error compressing: %d\n", n);
//		} else {
//			maxstrlen = (strlen(description) > maxstrlen) ? strlen(description) : maxstrlen;
//			total += strlen(description);
//			maxstrlen2 = (strlen(output_buf) > maxstrlen2) ? strlen(output_buf) : maxstrlen2;
//			total2 += strlen(output_buf);
//		}
//		print_out_nonl("%d %s: %ld: %ld: %s\n", id, flags, strlen(description), strlen(output_buf), description);
//		print_out_nonl("%d %s: %s\n", id, flags, description);
//		vTaskDelay(1);
	}
//	print_out_nonl("maxstrlen: %ld %ld\n", maxstrlen, maxstrlen2);
//	print_out_nonl("total: %ld %ld\n", total, total2);

//	print_out_nonl("names_count: %ld\n", names_count);


	return notes;
}
