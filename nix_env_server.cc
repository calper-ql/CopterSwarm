/*
 *			Server for controlling environment in unix.
 *  
 *
 */

#include "CopterLib/Environment.h"
#include <string>

#define DEFAULT_PORT 30000

int main(int argc, char* argv[]){
	std::string server_ip = "127.0.0.1";

	if(argc > 1) server_ip = std::string(argv[1]);



}
