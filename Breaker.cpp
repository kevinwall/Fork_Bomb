#include <iostream>
#include <string> 
#include <cstring>
#include <sstream>
#include <algorithm>

// Função que executa um comando no terminal e retorna o que seria printado
// como string
// Creditos:
// https://stackoverflow.com/questions/32039852/returning-output-from-bash-script-to-calling-c-function
std::string exec(char *cmd)
{
    FILE *pipe = popen(cmd, "r");
    if (!pipe)
        return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe))
    {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}

int main(int argc, char const *argv[])
{
    // Neste bloco capturamos o usuário padrão que é o principal suspeito de exxecutar o fork bomb
    std::string bomber;
    bomber = exec("whoami | grep -E -v '^$'");

    //std::cout << bomber << std::endl;

    // Aqui declaramos as variáveis que serão utilizadas para analizar o funcionamento do sistema
    int q_process, threshold{200};
    std::string table, ppid, user;
    while(true)
    {
        table = exec("ps -e -o ppid,user| sort | uniq -c"); // Verificando quais processos existem e seus respectivos usuários.

        std::istringstream oss(table); //Criando a stream para remover os dados da tabela; 

        while(oss >> q_process)
        {
            std::cout<<"Entrei na remoção "<<std::endl;

            oss >> ppid; // Aqui, pegamos o ppid do processo.
            oss >> user; // Aqui pegamos o usuário responsável por criar o processo.

            user = user + "\n"; // Adicionamos o "\n" para ele ficar compatível com o usuário "bomber" e conseguirmos realziar a comparãção com precisão.

            std::cout<<"Vou comparar o processo"<<std::endl;
            if(user.compare(std::string(bomber)) == 0) //Se o usuário que criou o processo atual for o mesmo que o usuário suspeito "bomber".
            {
                std::cout<<"Achei o user, vou matar o processo"<<std::endl;
                //std::cout<<"User: "<<user<<"Bomber:"<< bomber;
                if(q_process > threshold) // Se a quantidade de processos desse usuário for maior que um limiar, ele é o causador do fork bomb.
                {
                    // INformamos qual processo e usuário é o causador do bomb.
                    std::cout << "O processo " << ppid << " do usuário " << user << "é parte do programa malicioso" << std::endl;

                    // Matamos o processo.
                    std::string kill;

                    kill = "kill -15 " + ppid;

                    system(kill.c_str());

                    std::cout << "Processo malicioso interrompido..." << std::endl;
                }
            }
        }
    }


    return 0;
}