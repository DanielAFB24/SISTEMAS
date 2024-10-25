#include "utils.h"
#include <iostream>
#include <string>
#include <thread>
#include <list>
#include <string>


 void atiendeConexion(int clientId){

    std::vector<unsigned char> buffer;
    std::string mensaje;

    recvMSG(clientId, buffer);

    mensaje.resize(unpack<int>(buffer));

    unpackv<char> (buffer, (char*) mensaje.data(), mensaje.size());

    std::cout<<"Mensaje recibido : "<<mensaje<<std::endl;

    closeConnection(clientId);

 }


int main(int argc, char** argv)
{

     auto conn = initServer(3000);

     while(1){
        while(!checkClient()) usleep(100);
         auto clientId = getLastClientID();
         std::thread * th = new std::thread(atiendeConexion, clientId);
     }

    close(conn);
     
    

 return 0;
}
