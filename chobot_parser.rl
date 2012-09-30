#include "chobot_parser.h"

#define NUM_BUF_MAX 20
#define LEN(AT, FPC) (FPC - data - parser->AT)
#define MARK(M,FPC) (parser->M = (FPC) - data)
#define MARK_LEN(M, FPC) (FPC - (parser->M + data))
#define MARK_PTR(M) (parser->M + data)
#define PTR_TO(F) (data + parser->F)


#define SET_INTEGER(DST, M, FPC, STRFUNC) \
	do { \
		pos = MARK_PTR(M); \
		if(pos[0] == '0') { parser->DST = 0; } \
		else { \
			len = MARK_LEN(M, FPC); \
			if(len > NUM_BUF_MAX) { goto convert_error; } \
			memcpy(buf, pos, len); \
			buf[len] = '\0'; \
			parser->DST = STRFUNC(buf, NULL, 10); \
			if(parser->DST == 0) { goto convert_error; } \
		} \
	} while(0)

#define SET_UINT(DST, M, FPC) \
	SET_INTEGER(DST, M, FPC, strtoul)

#define SET_ULL(DST, M, FPC) \
	SET_INTEGER(DST, M, FPC, strtoull)

#define SET_MARK_LEN(DST, M, FPC) \
		parser->DST = MARK_LEN(M, FPC);



%%{
	machine chobot_parser;

	action reset {
		fprintf(stderr, "# reset\n");
		parser->keys = 0;
		parser->current = 0;
	}

	action mark_cmd {
		MARK(key_pos[parser->keys], fpc);
	}

	action cmd{
		SET_MARK_LEN(key_len[parser->keys], key_pos[parser->keys], fpc);
	}

	action mark_args{
		MARK(args, fpc);
	}

	action args{
		SET_UINT(args, args, fpc);
	}

	action data_start {
		fprintf(stderr, "data_start");
		MARK(data_pos, fpc+1);
		parser->data_count = parser->size;
		fcall data;
	}

	action data {
		if (--parser->data_count == 0) {
			//printf("mark %d\n", parser->data_pos);
			fprintf(stderr, "!data: %s\n", data+parser->data_pos);
			//printf("fpc %p\n", fpc);
			//printf("data %p\n", data);
			SET_MARK_LEN(data_len, data_pos, fpc+1);
			fret;
		}
	}

	action check_args{
		if (parser->args < 1) {
			goto convert_error;
		}
	}

	action check_arguments{
		fprintf(stderr, "# check arguments\n");
	}

	action mark_size{
		MARK(size, fpc);
	}

	action size{
		SET_UINT(size, size, fpc);
	}

	action check_cmd{
	}

	CRLF       = "\r\n";
	cmd        = ([a-zA-Z0-9][a-zA-Z0-9]*) > mark_cmd     %cmd;
	args       = ('0' | [1-9][0-9]*)       > mark_args    %args;
	size       = ('0' | [1-9][0-9]*)       > mark_size    %size;
	arguments  = size CRLF @data_start CRLF;

	chobot_command = cmd CRLF @check_cmd 
		args CRLF @check_args
		(arguments)+ @check_arguments;

main := (chobot_command > reset)+;
data := (any @data)*;

}%%

%% write data;

int chobot_parser_init(chobot_parser *parser) {
	int cs = 0;
	int top = 0;

	%% write init;

	memset(parser, 0, sizeof(chobot_parser));
	parser->cs = cs;
}

int chobot_parser_execute(chobot_parser* parser, const char* data, size_t len, size_t* off)
{
	const char* p = data + *off;
	const char* pe = data + len;
	const char* eof = pe;
	int cs  = parser->cs;
	int top = parser->top;
	int* stack = parser->stack;
	const char* pos;
	char buf[NUM_BUF_MAX+1];

	%% write exec;

ret:
	parser->cs = cs;
	parser->top = top;
	*off = p - data;

	if(cs == chobot_parser_error) {
		return -1;
	} else if(cs == chobot_parser_first_final) {
		return 1;
	} else {
		return 0;
	}

convert_error:
	return 1;
}
