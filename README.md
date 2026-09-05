[README.md](https://github.com/user-attachments/files/31869039/README.md)
# Sistema de Gestion de Lista de Espera para Restaurante

## Descripcion y Eficiencia

Este programa gestiona la lista de espera de un restaurante utilizando el **TDA Cola (Queue)** de la carpeta `tdas/`.

### ¿Por que usamos 3 colas independientes?

Para organizar a los clientes segun su prioridad, usamos 3 colas separadas:
1. `colaVIP`: Clientes VIP (Maxima prioridad).
2. `colaGrande`: Grupos grandes de personas.
3. `colaEstandar`: Clientes en orden regular de llegada.

**Ventajas de este diseño:**
- **Atencion rapida en tiempo constante O(1):** Para asignar una mesa, el programa solo revisa el primer cliente de la cola VIP; si no hay nadie, revisa la cola Grande, y finalmente la Estandar. No necesita ordenar ni buscar en toda una lista, atendiendo de inmediato.
- **Registro inmediato O(1):** Cuando llega un grupo nuevo, se agrega directamente al final de la cola estandar con `queue_insert()` en tiempo O(1).
- **Uso eficiente de los TDAs:** Usamos las funciones de `tdas/queue.h` (`queue_insert`, `queue_remove`, `queue_front`) y de `tdas/list.h` (`list_first`, `list_next`, `list_popCurrent`) para recorrer y mover clientes sin crear funciones complejas.

---

## Compilacion y Ejecucion

### Compilar con GCC

Abre una terminal en la carpeta del proyecto y escribe:

```bash
gcc -Wall main.c tdas/list.c tdas/extra.c -Itdas -o restaurante
```

### Ejecutar

- **En Windows:**
  ```powershell
  .\restaurante.exe
  ```

- **En Linux / Mac:**
  ```bash
  ./restaurante
  ```

---

## Estado de la Aplicacion

Todas las opciones del menu estan funcionando correctamente:

- **1. Registrar grupo:** Pide el nombre/ID y cantidad de personas. Entra por defecto a la cola Estandar y guarda la hora de llegada.
- **2. Asignar prioridad:** Permite buscar un cliente por su ID y moverlo a la cola VIP, Grande o Estandar.
- **3. Mostrar lista de espera:** Muestra a los clientes ordenados por prioridad (primero VIP, luego Grande, luego Estandar) con su hora en formato `HH:MM:SS`.
- **4. Asignar mesa:** Atiende al cliente con mayor prioridad disponible en tiempo O(1) y libera su memoria con `free()`.
- **5. Buscar reserva:** Busca un cliente por su ID e indica en que cola esta y su posicion en la fila.
- **6. Salir:** Libera toda la memoria utilizada antes de cerrar el programa.

### Manejo de errores
- Si el usuario ingresa letras en lugar de numeros en el menu, el programa no se congela ni colapsa; limpia la entrada y vuelve a pedir la opcion.
- Los mensajes en pantalla no usan tildes para evitar problemas de visualizacion en la consola.

---

## Ejemplo de Uso

```text
===========================================
  GESTION DE LISTA DE ESPERA (RESTAURANTE)
===========================================
1. Registrar grupo
2. Asignar prioridad
3. Mostrar lista de espera
4. Asignar mesa (Atender en O(1))
5. Buscar reserva
6. Salir
Seleccione una opcion: 1

--- REGISTRAR NUEVO GRUPO ---
Ingrese Nombre o ID: Carlos Perez
Cantidad de personas: 4
-> [EXITO] Grupo 'Carlos Perez' registrado en Cola Estandar.

Seleccione una opcion: 1

--- REGISTRAR NUEVO GRUPO ---
Ingrese Nombre o ID: Ana Lopez
Cantidad de personas: 2
-> [EXITO] Grupo 'Ana Lopez' registrado en Cola Estandar.

Seleccione una opcion: 2

--- ASIGNAR PRIORIDAD ---
Ingrese Nombre o ID del cliente a reclasificar: Ana Lopez
Cliente encontrado: 'Ana Lopez' (Prioridad actual: Estandar)
Seleccione la nueva prioridad:
1. VIP
2. Grande
3. Estandar
Opcion: 1
-> [EXITO] 'Ana Lopez' movido a Cola VIP.

Seleccione una opcion: 3

==================== LISTA DE ESPERA ====================

--- COLA VIP ---
  Turno #1 | [VIP] Ana Lopez - 2 personas | Hora Reg: 12:30:15

--- COLA Grande ---
  (No hay clientes en esta cola)

--- COLA Estandar ---
  Turno #2 | [Estandar] Carlos Perez - 4 personas | Hora Reg: 12:29:40
=========================================================

Seleccione una opcion: 4

[MESA ASIGNADA CON EXITO]
-> Atendiendo a : Ana Lopez
-> Comensales   : 2 personas
-> Prioridad    : VIP
-> Hora llegada : 12:30:15
-> Memoria del cliente liberada con free().

Seleccione una opcion: 6

Liberando memoria y saliendo del sistema...
Hasta pronto!
```

---

## Contribuciones

- **Nombre:** Vicente Gonzalez
- **Asignatura:** Estructuras de Datos
- **Universidad:** Pontificia Universidad Catolica de Valparaiso
