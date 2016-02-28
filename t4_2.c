
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define TAMANIO 10

int bufer [TAMANIO];	//memoria compartida donde se escribira y leeran los datos 

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;  // Los mutex para hacer la
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;  // sincronización.

void * productor();	//metodo productor
void * consumidor();	//metodo consumidor

int main (){	
	srand(time(NULL));

	pthread_t h1,h2;	//se crean dos hilos
	pthread_mutex_lock(&m2);	
	pthread_create(&h1,NULL,&productor,NULL);	//se ejecutan los hilos con sus funciones
	pthread_create(&h2,NULL,&consumidor,NULL);
	
	pthread_join(h1,NULL);	
	pthread_join(h2,NULL);

	printf("BUFFER :\n");	//se imprime el buffer completo al final del programa
	int i;
	for(i = 0; i<10; i++){
		printf("%d",bufer[i]);
	}
	printf("\n");
}

void * productor(){	//agrega un valor aleatorio a el buffer 
	int i;
	for(i = 0; i<10; i++){
		int valor = rand()% 9 + 1;
		pthread_mutex_lock(&m1);
		bufer[i] = valor;
		pthread_mutex_unlock(&m2);
	}

}

void * consumidor(){	//lee un valor del buffer 
	int dato;
	int i;
	
	for(i = 0; i<10; i++){
		pthread_mutex_lock(&m2);
		dato = bufer[i];
		printf("Consume: %d\n",dato);
		bufer[i] = 0;
		pthread_mutex_unlock(&m1);
	}
	
}