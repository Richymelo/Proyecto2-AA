# Proyecto 2: Ordenando Números con Threads

## Descripción General
Este proyecto consiste en modificar el **Proyecto 1** para implementar hilos de ejecución utilizando la biblioteca **pthreads** en lenguaje **C** bajo **Linux**, junto con interfaces gráficas creadas con **GTK** y **Glade**.
Se ejecutarán **cuatro algoritmos de ordenamiento en paralelo**, cada uno en su propio hilo, mostrando su progreso en una **interfaz gráfica** compuesta de **cuatro discos visuales**.

---

## Funcionalidades
- **Ejecución simultánea** de cuatro algoritmos de ordenamiento sobre los mismos datos iniciales.
- Cada algoritmo tiene su propio **disco visual** donde se ve su progreso.
- Cada algoritmo muestra de manera **independiente**:
  - Número de **iteraciones**.
  - Número de **intercambios**.
- **Menú desplegable** para seleccionar el algoritmo de ordenamiento de cada hilo.
- Un único botón **"Iniciar"** que lanza los cuatro hilos a la vez.
- **Gráficas actualizadas dinámicamente** sin colisiones ni bloqueos entre hilos.

---

## Algoritmos de Ordenamiento Disponibles
- **Bubble Sort**
- **Cocktail Sort**
- **Exchange Sort**
- **Selection Sort**
- **Insertion Sort**
- **Merge Sort**
- **Quick Sort**
- **Shell Sort**
- **Gnome Sort**
- **Pancake Sort**
