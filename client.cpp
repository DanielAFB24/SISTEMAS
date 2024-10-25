#include "utils.h"
#include <string>
#include <iostream>
#include <string>


//Funcion para recibir mensajes reenviados por el servidor
//Recibe por parametros ID del servidor para recibir datos y una variable compratida


void recibeMensajeClientes(int serverId, bool& salir){
    
    vector<unsigned char> buffer;
  
    
    std::string nombreUsuario;
   
    std::string mensaje;

    while(!salir){

        recvMSG(serverId, buffer);

        if(buffer.size( > 0)){
            //desempaquetar del buffer la longitud del nombre del usuario
            //redimencionar varibale nombre usuario
            nombreUsuario.resize(unpack<int>(buffer));
            //Almacenar nombre usuario
            unpackv<char> (buffer, (char*) nombreUsuario.data(), nombreUsuario.size());
            //redimencionar y almacenar mensaje del cliente
            mensaje.resize(unpack<int>(buffer));
            unpackv<char> (buffer, (char*) nombreUsuario.data(), nombreUsuario.size());

            //mostar mensaje recibido
            std::cout << "Mensaje recibido:" << nombreUsuario << "dice: " << mensaje std::endl
        }else{
            //Conexion cerrada y salir
            closeConnection(serverId);
            //TENGO QUE CAMBIAR LA VARIABLE BOOLEANA?
        }

    }
   

    

}

int main(int argc,char** argv)
{

    std::vector<unsigned char> buffer;
    
    std::string nombreUsuario;
    std::string mensaje;

    bool salir = false;

    //pedimos el nombre del usuario por terminal
    std::cout << "Introduzca el nombre de usuario" << std::endl;
    std::getline(std::cin, nombreUsuario);

    //Iniciar conexion al server en localhost 3000
    auto conn = initClient("127.0.0.1", 3000);

    //Iniciar thread "recibeMensajeCliente". Debe pasarse por paremetro las variables



    /**
    std::vector<unsigned char> buffer;
    std::string mensaje = "Hola mundo";
    auto conn = initClient("127.0.0.1", 3000);
    //Para reconstruir el mensaje se necesitan la longitud del mensaje y el texto
    pack(buffer, (int) mensaje.size());//empaquetar datos de tamaño fijo
    packv(buffer, (char*) mensaje.data(), mensaje.size()); //empaquetar datos de tamaño variable

    sendMSG(conn.serverId, buffer); // se envia el paquete buffer usando serverId
    closeConnection(conn.serverId);
    return 0;**/
}
