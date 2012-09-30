#include "Chobot.h"
#include "ngx-queue.h"

int main(int argc, char **argv)
{
	Chobot *bot;

	bot = Chobot::getInstance();
	bot->putPlugin();
	bot->run();

	delete bot;
	return 0;
}
