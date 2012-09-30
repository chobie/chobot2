#include "chobot_parser.h"

int main(int argc, char **argv)
{
	int error = 0;
	const char *def = "Send\r\n2\r\n5\r\nHello\r\n5\r\nHello\r\n";
	size_t offset = 0;
	chobot_parser parser;
	chobot_parser_init(&parser);

	error = chobot_parser_execute(&parser,def, strlen(def), &offset);

	fprintf(stderr, "========================\n");
	fprintf(stderr, "error: %d\n", error);
	fprintf(stderr, "offset: %ld\n", offset);
	
	return 0;
}