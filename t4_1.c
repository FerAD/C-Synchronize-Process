#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>



void * produce(int []);
int * consume(int []);

int tamBuffer;	//tamaño del buffer
int espaciosVacios;	//numero de espacios vacios en el buffer
int espaciosLLenos;	//numero de espacios llenos en el buffer


sem_t *sem_cont, *sem_vac;	// apuntadores de los semaforos

int main(int args, int * argv[]){	//argv[0] numero de consumidores y productores, argv[1] tamaño del buffer
	
	srand(time(NULL));	//no repetir valores random

	if(atoi(argv[1])<=1){	//validaciones de consumidor,productore y tamaño del buffer
		printf("El numero de consumidores\ndebe ser mayor que 1\n");
		return -1;
	}
	if(atoi(argv[2])<=1){
		printf("El numero de productores\ndebe ser mayor que 1\n");
		return -1;	
	}
	if(atoi(argv[3])<=1){
		printf("El tamaño del buffer debe ser\nmayor que 1\n");
		return -1;
	}

	

	int numConsumidores = atoi(argv[1]);	//numero de consumidores
	int numProductores = atoi(argv[2]);	//numero de productoreos
	tamBuffer = atoi(argv[3]);	//asignamos el tamaño del buffer
	int buffer[tamBuffer];	//buffer donde se guardan los datos Region critica
	espaciosVacios = tamBuffer;	//al iniciar el numero de espacios vacios es igual al tamaño del buffer
	espaciosLLenos = 0;	//al iniciar el numero de espacios llenos es 0

	
	int valor1, valor2; //valor1 y valor2 son los valores iniciales que tomaran los semaforos, pueden ser modificados por el usuario para cambiar los valores de los semaforos contadores
	valor1 = 0; //se establece valor1 como 0
	valor2 = tamBuffer - valor1; //al ser valor2 los espacios vacios que quedan en el buffer, valor2 va a ser el tamano total del buffer menos el valor de valor1, que es el valor de enteros dentro del buffer

	int i;	//for para crear consumidores
	int j;	//for para crear productores
	int k;	//recorre el buffer e imprime el contenido
	


	if((sem_cont=sem_open("/sem_cont", O_CREAT, 0644, valor1))==(sem_t *)-1)
		perror("Error, no se ha podido crear el semaforo de cuenta de valores");

	if((sem_vac=sem_open("/sem_vac", O_CREAT, 0644, valor2))==(sem_t *)-1)
		perror("Error, no se ha podido crear el semaforo de cuenta de espacios");


	sem_post(sem_vac);	
	while(1){
		pthread_t productores[numProductores];	//hilos productores
		for(j = 0; j<numProductores; j++){	//for para crear los hilos que ejecutaran el metodo produce
			pthread_create(&productores[j],NULL,(void*)&produce,&buffer);	//creo el hilo y ejecuto su funcion
			pthread_join(productores[j],NULL);
		}
		printf("Produce ");	//imprime el buffer
		for(k = 0; k<tamBuffer; k++){
			printf(":%d ",buffer[k]);
		}
		sleep(1);	//duerme un segundo
		printf("\n");
		pthread_t consumidores[numConsumidores];	//hilos consumidores
		for(i = 0; i<numConsumidores; i++){	//for para crera los hilos que ejecutaran el metodo consume
			pthread_create(&consumidores[i],NULL,(void*)&consume,&buffer);	//creo el hilo y ejecuto su funcion
			pthread_join(consumidores[i],NULL);
		}
		printf("Consume ");	//imprime el buffer 
		for(k = 0; k<tamBuffer; k++){
			printf(":%d ",buffer[k]);
		}
		sleep(1);	//duerme un segundo
		printf("\n");
	}
	
	return 1;
}



void * produce(int buf[]){
	int valor;	//valor que se agregara al buffer
	valor = rand()%10 + 1;	//asignacion random al valor

	if(espaciosVacios > 0){	//mientras no este lleno
		sem_wait(sem_vac); //espera al semaforo sem_vac, decrementa uno y sigue
		sem_post(sem_cont); //incrementa uno al semaforo sem_Vac, pues hay 1 espacio vacio mas
		espaciosVacios--;	//quito un espaico vacio
		buf[espaciosLLenos] = valor;	//agrego el valor a un espacio
		espaciosLLenos++;	//incremento los espacios llenos

	}
	sem_close(sem_cont); // los semaforos se cierran para que no sean interrumpidos entre proceso y proceso
	sem_close(sem_vac);
}

int * consume(int buf[]){
	
	if(espaciosVacios!=tamBuffer){	//mientras no este vacio
		sem_wait(sem_cont);	//espera al semaforo sem_cont, decrementa uno y sigue
		sem_post(sem_vac);	//incrementa uno al semaforo sem_vac, pues hay 1 espacio vacio mas
		buf[espaciosLLenos-1] = 0;	//consumo igualando a 0 
		espaciosLLenos--;	//decremento los espacios llenos
		espaciosVacios++;	//y aumento los espacios vacios
	}
	sem_close(sem_cont); // los semaforos se cierran para que no sean interrumpidos entre proceso y proceso
	sem_close(sem_vac);

}