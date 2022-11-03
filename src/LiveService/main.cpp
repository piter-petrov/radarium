#include "log.h"
#include "liveserver.h"

int main(int argc, char *argv[])
{
	LiveServer a(argc, argv);
	return a.exec();
}
