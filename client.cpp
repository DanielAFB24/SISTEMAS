#include "utils.h"
#include <string>
#include <iostream>
#include <string>


int main(int argc,char** argv)
{

    std::vector<unsigned char> buffer;
    std::string mensaje = "Hola mundo";
    auto conn = initClient("127.0.0.1", 3000);
    //Para reconstruir el mensaje se necesitan la longitud del mensaje y el texto
    pack(buffer, (int) mensaje.size());//empaquetar datos de tamaño fijo
    packv(buffer, (char*) mensaje.data(), mensaje.size()); //empaquetar datos de tamaño variable

    sendMSG(conn.serverId, buffer); // se envia el paquete buffer usando serverId
    closeConnection(conn.serverId);
    return 0;
}
