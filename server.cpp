#include "utils.h"
#include <iostream>
#include <string>
#include <thread>
#include <list>
#include <string>
#include <algorithm>


 void atiendeConexion(int clientId, std::map<std::string, int> &userList){

   std::vector<unsigned char> buffer;
   //Mensajes para cuando no exista un usuario privado
   std::string userNoFound = "USUARIO NO  DISPONIBLE";
   std::string userServer = "SERVER";
   std::string errorFormato = "ERROR FORMATO ESCRIBIR MENSAJE PROVADO";

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
   //users.push_back(clientId);
   userList[nombreUsuario] = clientId;

   //Bucle repetir mientra no reciba el mensaje del texto exit() del cliente

   do{
      //Recibe el mensaje de texto del cliente
      recvMSG(clientId, buffer);
      //Si hay datos en el buffer recibir

      if(buffer.size() > 0){
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

         //verificamos es un mensaje publico o privado
         if (!mensaje.empty() && mensaje[0] == '@') {
                // Extraer el destinatario del mensaje
                size_t pos = mensaje.find(' ');
               //Verificamos de que se haya encontrado la cadena
               if(pos != std::string::npos){
                  //Extraemos el destinatario sin tener en cuenta el @
                  std::string destinatario = mensaje.substr(1, pos - 1);
                  //luego extraemos el mensaje privado
                  std::string subMensaje = mensaje.substr(pos + 1); 
                  //Buscamos si el usuario se encuentra
                  auto userPrivate = userList.find(destinatario);
                  //limpiamos el buffer para enviar mensaje de error al usuario
                  buffer.clear();
                  if(userPrivate != userList.end()){
                     std::string mensajePrivado = "[PRIVADO] " + subMensaje;
                     pack(buffer, (int) nombreUsuario.size());
                     pack(buffer,(int) mensajePrivado.size());
                     packv(buffer, (char*) nombreUsuario.data(), nombreUsuario.size());
                     packv(buffer, (char*) mensajePrivado.data(), mensajePrivado.size());
                     sendMSG(userPrivate->second, buffer);
                  }else{
                     //Empaquetamos el mensaje
                     pack(buffer, (int) userServer.size());
                     pack(buffer,(int) userNoFound.size());
                     packv(buffer, (char*) userServer.data(), userServer.size());
                     packv(buffer, (char*) userNoFound.data(), userNoFound.size());
                     sendMSG(clientId, buffer);
                  }


               }else{
                  //Si es un mensaje privado y no se encuentra el espacio entre el mensaje y el usuario se entiende que es un error de formato
                  //limpiamos el buffer para enviar mensaje de error al usuario
                     buffer.clear();
                     //Empaquetamos el mensaje
                     pack(buffer, (int) userServer.size());
                     pack(buffer,(int) errorFormato.size());
                     packv(buffer, (char*) userServer.data(), userServer.size());
                     packv(buffer, (char*) errorFormato.data(), errorFormato.size());
                     sendMSG(clientId, buffer);

               }
         }else{
             for(const auto& userId : userList){
            //Reenviamos mensajes sin tener en cuenta al que no los ha enviado
            if(clientId != userId.second){
               sendMSG(userId.second, buffer);
            }
         }
        
            
        }

        buffer.clear();

      }else{
         std::cout << "El cliente cerror sin enviar mensaje de exit()" << std::endl;
         mensaje = "exit()";
      }
   }while(mensaje != "exit()");

   //Creamos mensajes para cerrar el servidor
   std::string closeMessage = "EXIT_SERVER";
   //Empaquetamos el mensaje de error SOLUCION AL PROBLEMA 2 PUNTOS ADICIONALES
   pack(buffer, (int)nombreUsuario.size());
   pack(buffer, (int)closeMessage.size());
   packv(buffer, (char*)nombreUsuario.data(), nombreUsuario.size());
   packv(buffer, (char*)closeMessage.data(), closeMessage.size());
   //ENVIAMOS LA SEÑAL AL CLIENTE DE QUE VAMOS A CERRAR LA CONEXION
   sendMSG(clientId, buffer);

    
   //Eliminar cliente de la lista 
   userList.erase(nombreUsuario);

   //Cerrar conexion con el cliente
   closeConnection(clientId);

    

 }


int main(int argc, char** argv)
{

     auto conn = initServer(3000);

     //std::list<int> userList;
     std::map<std::string, int> userList; 

     while(1){
        while(!checkClient()) usleep(100);
         auto clientId = getLastClientID();
         std::thread * th = new std::thread(atiendeConexion, clientId, std::ref(userList));
     }

    close(conn);
     
    

 return 0;
}
