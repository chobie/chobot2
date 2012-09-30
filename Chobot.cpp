#include "Chobot.h"

Chobot* Chobot::bot = NULL;

Chobot::Chobot()
{
	this->loop = uv_default_loop();
	uv_tcp_init(this->loop, &this->tcp);
}

uv_tcp_t *Chobot::getTcp()
{
	return &this->tcp;
}

uv_loop_t *Chobot::getLoop()
{
	return this->loop;
}

void Chobot::putPlugin()
{
}

void Chobot::run()
{
	int error = 0;
	error = uv_tcp_bind(&this->tcp, uv_ip4_addr("0.0.0.0", 4649));
	
	uv_listen((uv_stream_t*)&this->tcp, 100, Chobot::listen_cb);
	uv_run(this->loop);
}