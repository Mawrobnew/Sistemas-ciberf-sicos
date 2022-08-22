

Requerimientos:

El NodeMCU A deberá ser capaz de medir una señal proveniente de un sensor analógico o digital.
El NodeMCU B deberá ser capas de leer una señal proveniente de un switch digital.
El NodeMCU B deberá encender el LED 2 cuando reciba el mensaje "Enciende" de un tópico designado con el nombre de tu equipo de trabajo. Y apagar el LED 2 cuando reciba el mensaje "Apagar" proveniente del mismo tópico.
El NodeMCU A deberá encender el LED 1 cuando el Switch del NodeMCU B pase a un estado digital bajo o 0V.
El NodeMCU A y NodeMCU B publicar al Broker el estado del Sensor y Switch, respectivamente, cada 7 segundos. En ambos casos deberán usar el tópico correspondiente a la matrícula de uno de los miembros del equipo.
