Manual de Usuario

1. Requisitos y acceso a nuestro sistema

Se requiere el programa QEMU para poder emular el sistema operativo basado en x64BareBones. 
Para el armado y seteo del ambiente puede seguir el manual de instrucciones descrito previamente por los autores de RowDaBoat dentro del file Readme.txt ubicado en el x64BareBones project directory. Luego, una vez compilado el proyecto, correr dentro del mismo directorio el comando ./run.sh en la terminal para poder correr el sistema. En caso de solicitarle permisos de ejecución, puede correr previamente el comando chmod +x run.sh.

Una vez accedido a nuestro sistema, se encontrará con la siguiente ventana del programa QEMU: 
![Screenshot 2024-06-04 225403](https://github.com/clee0412/tpe_arqui/assets/117415072/8cf5d0cd-8196-4ab5-8ed0-c8066cb59c6c)

2. Lista de comandos

El usuario podrá insertar distintos comandos luego de los signos “$>”. El sistema ejecutará dicho comando una vez presionada la tecla “enter”. En caso contrario, la shell estará esperando el ingreso de un comando.
A continuación, se observa la lista de comandos y sus funcionalidades luego de ingresar el comando “help”:
![Screenshot 2024-06-04 225346](https://github.com/clee0412/tpe_arqui/assets/117415072/b5c75bff-d8fc-47f2-b99c-cc9f449f8350)

En caso de que se ingresé más de un argumento se tira el error: “No puede haber ni un espacio ni más de 1 argumento. Verificar los comandos de nuevo”. Si se ingresa algún otro comando o está mal escrito, aparecerá el mensaje de error: “Error: comando no disponible. Ingrese "help" para ver los comandos disponibles.” en pantalla. 
![Screenshot 2024-06-04 225417](https://github.com/clee0412/tpe_arqui/assets/117415072/cbbfe88d-06c9-4f78-94e9-f26c919af7c9)
