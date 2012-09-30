#ifndef CHOBOT_PARSER_H
#define CHOBOT_PARSER_H

#include <stdio.h>
#include <string.h>

enum CHOBOT_COMMAND{
	CHOBOT_COMMAND_SEND = 1,
};

typedef struct {
	int command;
	int size;
	int bytes;
	int keys;
	int args;
	int current;
	int data_count;
	int data_pos;
	int data_len;
	int key_len[20];
	size_t key_pos[20];
	/* for ragel */
	int cs;
	int top;
	int stack[20];
} chobot_parser;


int chobot_parser_init(chobot_parser *parser);
int chobot_parser_execute(chobot_parser* parser, const char* data, size_t len, size_t* off);

#endif