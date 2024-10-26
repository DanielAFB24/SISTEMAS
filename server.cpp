#include "utils.h"
#include <iostream>
#include <string>
#include <thread>
#include <list>
#include <string>
#include <algorithm>


 void atiendeConexion(int clientId, std::list<int> &users){

   std::vector<unsigned char> buffer;
   //Desempaquetamos todas las variables que emaqueto el usuario
   recvMSG(clientId, buffer);

   std::string nombreUsuario;
   std::string mensaje;

   //Desempaquetamos el nombre del usuario
   nombreUsuario.resize(unpack<int>(buffer));
   unpackv<char> (buffer, (char*) nombreUsuario.data(), nombreUsuario.size());

   //Mostramos el nombre de los usuarios
   std::cout << "Usuario conectado :" << nombreUsuario << std::endl;

   //Añadimos el cliente a la lista de usuarios
   users.push_back(clientId);

   //Bucle repetir mientra no reciba el mensaje del texto exit() del cliente

   do{
      //Recibe el mensaje de texto del cliente
      recvMSG(clientId, buffer);
      //Si hay datos en el buffer recibir

      if(buffer.size()){
         //Desempaquetamos el mensaje
         mensaje.resize(unpack<int>(buffer));
         unpackv<char> (buffer, (char*) mensaje.data(), mensaje.size());

         //Mostramos el mensaje recibido
         std::cout << "Mensaje recibido: " << nombreUsuario << ": " << mensaje << std::endl;

         //Reenviamos mensaje cada uno de los usuarios
         //empaquetar tamaño
         pack(buffer, (int) nombreUsuario.size()); 
         pack(buffer, (int) mensaje.size());
         //empaquetamos contenido mensaje
         packv(buffer, (char*)nombreUsuario.data(), nombreUsuario.size());
         packv(buffer, (char*)mensaje.data(), mensaje.size());

         //bucle por cada iteracion de los usuarios
         for(auto& userId : users){
            //Reenviamos mensajes sin tener en cuenta al que no los ha enviado
            if(clientId != userId){
               sendMSG(userId, buffer);
            }
            
        }

        buffer.clear();

      }else{
         std::cout << "El cliente cerror sin enviar mensaje de exit()" << std::endl;
         mensaje = "exit()";
      }
   }while(mensaje != "exit()");
    
   //Eliminar cliente de la lista 
   users.erase(std::find(users.begin(), users.end(), clientId));

   //Cerrar conexion con el cliente
   closeConnection(clientId);

    

 }


int main(int argc, char** argv)
{

     auto conn = initServer(3000);

     std::list<int> userList;

     while(1){
        while(!checkClient()) usleep(100);
         auto clientId = getLastClientID();
         std::thread * th = new std::thread(atiendeConexion, clientId, std::ref(userList));
     }

    close(conn);
     
    

 return 0;
}
