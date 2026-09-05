#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tdas/queue.h"
#include "tdas/extra.h"

// ESTRUCTURA DE DATOS PARA EL CLIENTE

// Estructura que representa la informacion de un grupo o cliente en espera
typedef struct {
    char id[50];                // Nombre o identificacion del cliente
    int personas;               // Cantidad de personas
    char prioridad[20];         // "VIP", "Grande" o "Estandar"
    time_t hora_registro;       // Hora de llegada (<time.h>)
} Cliente;

// 3 Colas globales usando el TDA Cola de la carpeta tdas
Queue *colaVIP = NULL;
Queue *colaGrande = NULL;
Queue *colaEstandar = NULL;

// FUNCIONES DEL MENU

// Opcion 1: Registrar grupo
// El grupo entra por defecto a la cola Estandar en O(1) usando queue_insert (insertar en la cola)
void registrar_grupo() {
    Cliente *nuevo = (Cliente *)malloc(sizeof(Cliente));
    if (nuevo == NULL) {
        printf("Error: Memoria insuficiente.\n");
        return;
    }

    printf("\n--- REGISTRAR NUEVO GRUPO ---\n");
    printf("Ingrese Nombre o ID: ");
    scanf(" %[^\n]", nuevo->id);

    printf("Cantidad de personas: ");
    while (scanf("%d", &nuevo->personas) != 1 || nuevo->personas <= 0) {
        printf("Por favor ingrese un numero entero mayor a 0: ");
        while (getchar() != '\n'); // Limpiar buffer
    }

    strcpy(nuevo->prioridad, "Estandar");
    nuevo->hora_registro = time(NULL);

    // Insercion directa en O(1) usando la funcion del TDA Queue
    queue_insert(colaEstandar, nuevo);

    printf("-> Grupo '%s' registrado en Cola Estandar.\n", nuevo->id);
}

// Opcion 2: Asignar o cambiar prioridad
// Busca al cliente, lo extrae con list_popCurrent y lo re-encola con queue_insert
void asignar_prioridad() {
    char id_buscado[50];
    printf("\n--- ASIGNAR PRIORIDAD ---\n");
    printf("Ingrese Nombre o ID del cliente a reclasificar: ");
    scanf(" %[^\n]", id_buscado);

    Cliente *encontrado = NULL;
    Queue *colas[] = {colaVIP, colaGrande, colaEstandar};

    // Buscamos y extraemos usando las funciones del TDA Lista
    for (int i = 0; i < 3; i++) {
        Cliente *c = (Cliente *)list_first(colas[i]);
        while (c != NULL) {
            if (strcmp(c->id, id_buscado) == 0) {
                encontrado = (Cliente *)list_popCurrent(colas[i]);
                break;
            }
            c = (Cliente *)list_next(colas[i]);
        }
        if (encontrado != NULL) break;
    }

    if (encontrado == NULL) {
        printf("-> No se encontro ninguna reserva con el ID '%s'.\n", id_buscado);
        return;
    }

    printf("Cliente encontrado: '%s' (Prioridad actual: %s)\n", encontrado->id, encontrado->prioridad);
    printf("Seleccione la nueva prioridad:\n");
    printf("1. VIP\n2. Grande\n3. Estandar\nOpcion: ");

    int opcion_prio;
    while (scanf("%d", &opcion_prio) != 1 || opcion_prio < 1 || opcion_prio > 3) {
        printf("Opcion invalida (1-3): ");
        while (getchar() != '\n');
    }

    // Insertar en la nueva cola usando queue_insert en O(1)
    if (opcion_prio == 1) {
        strcpy(encontrado->prioridad, "VIP");
        queue_insert(colaVIP, encontrado);
        printf("-> [EXITO] '%s' movido a Cola VIP.\n", encontrado->id);
    } else if (opcion_prio == 2) {
        strcpy(encontrado->prioridad, "Grande");
        queue_insert(colaGrande, encontrado);
        printf("-> [EXITO] '%s' movido a Cola Grande.\n", encontrado->id);
    } else {
        strcpy(encontrado->prioridad, "Estandar");
        queue_insert(colaEstandar, encontrado);
        printf("-> [EXITO] '%s' movido a Cola Estandar.\n", encontrado->id);
    }
}

// Opcion 3: Mostrar lista de espera
// Recorre las colas en orden estricto de prioridad usando list_first y list_next
void mostrar_lista() {
    Queue *colas[] = {colaVIP, colaGrande, colaEstandar};
    char *nombres[] = {"VIP", "Grande", "Estandar"};
    int turno = 1;
    char buffer_hora[20];

    printf("\n==================== LISTA DE ESPERA ====================\n");
    for (int i = 0; i < 3; i++) {
        printf("\n--- COLA %s ---\n", nombres[i]);
        if (queue_front(colas[i]) == NULL) {
            printf("  (No hay clientes en esta cola)\n");
            continue;
        }

        Cliente *actual = (Cliente *)list_first(colas[i]);
        while (actual != NULL) {
            struct tm *info = localtime(&actual->hora_registro);
            strftime(buffer_hora, sizeof(buffer_hora), "%H:%M:%S", info);

            printf("  Turno #%d | [%s] %s - %d personas | Hora Reg: %s\n",
                   turno, actual->prioridad, actual->id, actual->personas, buffer_hora);

            actual = (Cliente *)list_next(colas[i]);
            turno++;
        }
    }
    printf("=========================================================\n");
}

// Opcion 4: Asignar mesa
// Atiende en O(1) desencolando de la cola de mayor prioridad disponible con queue_remove
void asignar_mesa() {
    Cliente *atendido = NULL;

    // Evaluacion en O(1) usando queue_front y queue_remove
    if (queue_front(colaVIP) != NULL) {
        atendido = (Cliente *)queue_remove(colaVIP);
    } else if (queue_front(colaGrande) != NULL) {
        atendido = (Cliente *)queue_remove(colaGrande);
    } else if (queue_front(colaEstandar) != NULL) {
        atendido = (Cliente *)queue_remove(colaEstandar);
    }

    if (atendido != NULL) {
        char buffer_hora[20];
        struct tm *info = localtime(&atendido->hora_registro);
        strftime(buffer_hora, sizeof(buffer_hora), "%H:%M:%S", info);

        printf("\n[MESA ASIGNADA CON EXITO]\n");
        printf("-> Atendiendo a : %s\n", atendido->id);
        printf("-> Comensales   : %d personas\n", atendido->personas);
        printf("-> Prioridad    : %s\n", atendido->prioridad);
        printf("-> Hora llegada : %s\n", buffer_hora);

        // Liberar la memoria dinamica del cliente atendido
        free(atendido);
        printf("-> Memoria del cliente liberada \n");
    } else {
        printf("\nNo hay clientes en espera. Todas las colas estan vacias.\n");
    }
}

// Opcion 5: Buscar reserva
// Recorre las colas usando list_first y list_next para encontrar al cliente
void buscar_reserva() {
    char id_buscado[50];
    printf("\n--- BUSCAR RESERVA ---\n");
    printf("Ingrese Nombre o ID a buscar: ");
    scanf(" %[^\n]", id_buscado);

    Queue *colas[] = {colaVIP, colaGrande, colaEstandar};
    char *nombres[] = {"VIP", "Grande", "Estandar"};
    int encontrado = 0;

    for (int i = 0; i < 3; i++) {
        Cliente *actual = (Cliente *)list_first(colas[i]);
        int posicion = 1;

        while (actual != NULL) {
            if (strcmp(actual->id, id_buscado) == 0) {
                char buffer_hora[20];
                struct tm *info = localtime(&actual->hora_registro);
                strftime(buffer_hora, sizeof(buffer_hora), "%H:%M:%S", info);

                printf("\n-> Reserva Encontrada\n");
                printf("   Cliente : %s\n", actual->id);
                printf("   Personas: %d\n", actual->personas);
                printf("   Ubicado : Cola %s (Posicion #%d en esta cola)\n", nombres[i], posicion);
                printf("   Hora Reg: %s\n", buffer_hora);
                encontrado = 1;
                break;
            }
            actual = (Cliente *)list_next(colas[i]);
            posicion++;
        }
        if (encontrado) break;
    }

    if (!encontrado) {
        printf("-> No se encontro ningun cliente registrado con el ID '%s'\n", id_buscado);
    }
}

// Libera todos los clientes y limpia las colas usando funciones del TDA
void liberar_todo() {
    Queue *colas[] = {colaVIP, colaGrande, colaEstandar};
    for (int i = 0; i < 3; i++) {
        Cliente *c = (Cliente *)list_first(colas[i]);
        while (c != NULL) {
            free(c);
            c = (Cliente *)list_next(colas[i]);
        }
        queue_clean(colas[i]);
        free(colas[i]);
    }
}

// MENU PRINCIPAL
int main() {
    // Inicializacion de las colas usando el TDA Queue
    colaVIP = queue_create(NULL);
    colaGrande = queue_create(NULL);
    colaEstandar = queue_create(NULL);

    int opcion = 0;

    do {
        printf("\n===========================================\n");
        printf("  GESTION DE LISTA DE ESPERA (RESTAURANTE)\n");
        printf("===========================================\n");
        printf("1. Registrar grupo\n");
        printf("2. Asignar prioridad\n");
        printf("3. Mostrar lista de espera\n");
        printf("4. Asignar mesa (Atender en O(1))\n");
        printf("5. Buscar reserva\n");
        printf("6. Salir\n");
        printf("Seleccione una opcion: ");

        // aqui solo valido que el dato de entraada este bien
        if (scanf("%d", &opcion) != 1) {
            printf("Por favor ingrese un numero del 1 al 6.\n");
            while (getchar() != '\n'); // Limpiar buffer
            continue;
        }

        //si leen esto tuve que cambiar todo el codigo para usar las colas y funciones que ya existian en el 
        //archivo de tdas xd

        switch (opcion) {
            case 1: registrar_grupo(); break;
            case 2: asignar_prioridad(); break;
            case 3: mostrar_lista(); break;
            case 4: asignar_mesa(); break;
            case 5: buscar_reserva(); break;
            case 6:
                printf("\nLiberando memoria y saliendo del sistema...\n");
                liberar_todo();
                printf("Hasta pronto\n\n");
                break;
            default:
                printf("Opcion no valida. Intente nuevamente.\n");
                break;
        }

    } while (opcion != 6);

    return 0;
}
