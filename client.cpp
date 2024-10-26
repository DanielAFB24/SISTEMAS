#include "utils.h"
#include <string>
#include <iostream>
#include <string>


//Funcion para recibir mensajes reenviados por el servidor
//Recibe por parametros ID del servidor para recibir datos y una variable compratida


void recibeMensajeClientes(int serverId, bool& salir){
    
    std::vector<unsigned char> buffer;
    
    
    std::string nombreUsuario;
   
    std::string mensaje;

    while(!salir){

        recvMSG(serverId, buffer);

        if(buffer.size() > 0){
            //desempaquetar del buffer la longitud del nombre del usuario
            //redimencionar varibale nombre usuario
            nombreUsuario.resize(unpack<int>(buffer));
            mensaje.resize(unpack<int>(buffer));
            //Almacenar nombre usuario
            unpackv<char> (buffer, (char*) nombreUsuario.data(), nombreUsuario.size());
            //redimencionar y almacenar mensaje del cliente
            unpackv<char> (buffer, (char*) mensaje.data(), mensaje.size());

            //SEÑAL QUE RECIBIMOS DEL SERVIDIOR PARA NOTIFICAR QUE DEBEMOS CERRAR LA CONEXION
            //DE ESTA FORMA EVITAMOS  QUE EL BUCLE SIGA ESPERANDO UN MENSAJE DE UNA CONEXION QUE YA HA SIDO CERRADA
            if(mensaje == "EXIT_SERVER"){
                salir = true;
                break;
            }

            //mostar mensaje recibido
            std::cout << "Mensaje recibido : " << nombreUsuario << " dice : " << mensaje << std::endl;
        }else{
            //Conexion cerrada y salir
            closeConnection(serverId);
            //TENGO QUE CAMBIAR LA VARIABLE BOOLEANA?
            salir = true;
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
    std::cout << "Para poder dirigir mensajes privados es necesario comenzar con un arroba seguido del usuario como se muestra en el suguiente ejemplo"<< std::endl;
    std::cout << "@Alex mensaje" << std::endl;
    std::cout << "Introduzca el nombre de usuario" << std::endl;
    std::getline(std::cin, nombreUsuario);

    //Iniciar conexion al server en localhost 3000
    auto conn = initClient("127.0.0.1", 3000);

    //Iniciar thread "recibeMensajeCliente". Debe pasarse por paremetro las variables
    std::thread * th = new std::thread(recibeMensajeClientes, conn.serverId, std::ref(salir));

    //Empaquetamos el tamaño del nombre de usuario
    pack(buffer, (int) nombreUsuario.size());
    //Empaquetamos nombre de usuario
    packv(buffer, (char*)nombreUsuario.data(), nombreUsuario.size());   

    //Enviar mensaje al servidor
    sendMSG(conn.serverId, buffer);

    //Limpiar buffer
    buffer.clear();

    //Bucle hasta que el usuario escriba exit()

    do{
        //leer el mensaje de texto del usuario
        std::getline(std::cin, mensaje);
        //empaquetar tamaño
        pack(buffer, (int) mensaje.size());
        //empaquetamos contenido mensaje
        packv(buffer, (char*)mensaje.data(), mensaje.size());
        // Enviar buffer
        sendMSG(conn.serverId, buffer);

        buffer.clear();


    }while(mensaje != "exit()");

    //Cambiar valor de la variable para detener hilo de recepcion de mensaje
    salir = true;
    //Sincronizamos con thread antes de cerrar la conexion
    th -> join();
    //Cerramos conexion con el servidor
    closeConnection(conn.serverId);

    return 0;
}
