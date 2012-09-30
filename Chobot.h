#ifndef CHOBOT_H
#define CHOBOT_H

#include "uv.h"
#include <stdio.h>
#include <iostream>

class Chobot {
public:
	static Chobot* bot;

	Chobot();
	void putPlugin();
	void run();
	uv_tcp_t *getTcp();
	uv_loop_t *getLoop();

	static Chobot* getInstance()
	{
		if (bot == NULL) {
			bot = new Chobot();
		}
		
		return bot;
	}
	
	static void close_cb(uv_handle_t* handle)
	{
		if (handle != NULL) {
			free(handle);
		}
	}

	static uv_buf_t alloc_cb(uv_handle_t* handle, size_t suggested_size)
	{
		uv_buf_t buf;

		buf.base = (char*)malloc(suggested_size);
		buf.len = suggested_size;
		
		return buf;
	}
	
	static void read_cb(uv_stream_t* stream, ssize_t nread, uv_buf_t buf)
	{
		if (nread < 0) {
			if (buf.base) {
				free(buf.base);
			}
			return;
		}
		
		std::cout << buf.base << std::endl;
		
		free(buf.base);
		return;
	}

	static void listen_cb(uv_stream_t* server, int status)
	{
		int error = 0;
		uv_tcp_t *tcp;

		Chobot *bot = Chobot::getInstance();
		
		tcp = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
		uv_tcp_init(bot->getLoop(), tcp);
		
		error = uv_accept(server, (uv_stream_t*)tcp);
		if (error) {
			std::cerr << "Error" << std::endl;
			uv_close((uv_handle_t*)tcp, Chobot::close_cb);
			return;
		}

		error = uv_read_start((uv_stream_t*)tcp, Chobot::alloc_cb, Chobot::read_cb);
		if (error) {
			std::cerr << "Error" << std::endl;
			uv_close((uv_handle_t*)tcp, Chobot::close_cb);
			return;
		}
	}

private:
	uv_tcp_t tcp;
	uv_loop_t *loop;
};

#endif