#include <iostream>
#include <list>
#include <string.h>
#include <sys/types.h>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <sstream>

using namespace std;
using namespace chrono;
using namespace this_thread;

typedef struct{
	string recipe_user;
	int recipe_process;
}UserAndProcess;

void numberProcess();

std::string exec(char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}

void numberProcess(){
	char * ps = "ps aux | wc -l";

	std::cout << " ========================================== " << endl;
	std::cout << " Inicializando o monitoramento periodico" << endl;
	std::cout << " Aperte CTRL+C para encerrar " << endl;
	while(true){
		std::cout << exec(ps);
		sleep_until(system_clock::now() + seconds(2));
	}
}

list<UserAndProcess> numberProcessByUser(){
	char * ps = "ps -e -o user | sort | uniq -c";

	string ps_user = exec(ps);

	std::istringstream reader(ps_user);

	list<UserAndProcess> count;

	int process;
	string user;

	UserAndProcess dummy;

	while( reader >> process ){
		reader >> user ;

		if( user == "USER" and process == 1)
			continue;

		dummy.recipe_process = process;
		dummy.recipe_user = user;

		count.push_back(dummy);
	}

	return count;
}

void print_numberProcessByUser(){

	list<UserAndProcess> count = numberProcessByUser();

	std::cout << " ========================================== " << endl;
	std::cout << " Inicializando o monitoramento periodico" << endl;
	std::cout << " Aperte CTRL+C para encerrar " << endl;
	
	while(true){
		for(auto i : count){
			std::cout << i.recipe_user << " : " << i.recipe_process << endl;
		}
		sleep_until(system_clock::now() + seconds(2));
	}
}

int main(void){
	//numberProcess();
	print_numberProcessByUser();
	
	return 0;
}
