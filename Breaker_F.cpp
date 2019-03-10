#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <unistd.h>

int main(int argc, char const *argv[])
{

	int limit;

	//std::cout<<argc;

	if(argc < 2) // Caso o usuário não tenha fornecido a quantidade de processos maxima como parãmetro de execução.
	{
		std::cout << "Digite a quantidade maxima de processos: ";
		std::cin >> limit;
	}
	else
	{
		limit = std::atoi(argv[1]);
	}

    struct rlimit limite_mod; // Criamos a struct que vai guardar o limite superior e inferior de processos.

    getrlimit(RLIMIT_NPROC, &limite_mod); // Usamos a função getrlimit para receber o limite soft e hard de processos atuais.

    limite_mod.rlim_cur = limit; // Modificamos o limite soft de processos na nossa struct.

    setrlimit(RLIMIT_NPROC, &limite_mod); // Moficamos o limite de processos no sistema, utilizando a struct modificada

    pid_t pid = 0; // Variável responsável por guardar retorno da fork para saber se a modificação foi efetiva e o fork impedido.

    while (true) 
    {

        pid = fork(); // Tentamos executar o fork bomb e guardamos o resultado da operação em pid.

        if (pid == -1) // Caso haja um erro ao tentar criar o fork. 
        {
            if (errno == EAGAIN) // Se houve um erro, e este erro foi por exceder o limite de processos, a prevenção do fork foi um sucesso.
            {
                std::cout << "Limite de processos execido, pressione Ctrl + C para finalizar a execução." << std::endl;
                return 0;
            }
        }

        // Caso um processo pai ou filho tenha sido criado, damos um wait para que o print, caso ocorra, fique bem visível
        if (pid >=0 ) 
        {
			wait(NULL);
        } 
    }

    return 0;
}