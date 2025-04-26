/*
            En esta sección se tienen los algoritmos de ordenamiento que el
            usuario puede escoger.
            Hay 10 tipos de algoritmos de ordenamiento, todos con secciones
            implementadas para que se muestre su paso a paso en el área
            de dibujo.
*/

#include <stdio.h>
#include <stdlib.h>

// Función de intercambiar dos valores
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
void area_inicial(){
    // Para poder ver el cambio en el circulo
    while (gtk_events_pending()) {
        gtk_main_iteration();  // Procesar los eventos de GTK
    }
    g_usleep(500000);  // Delay para ver cambios
}
void actualizar_area(){
    // Para poder ver el cambio en el contador de iteraciones
    while (gtk_events_pending()) {
        gtk_main_iteration();  // Procesar los eventos de GTK
    }
    //g_usleep(400000);   // Delay para ver cambios
}
// - - - - -
// BUBBLE SORT
// Iteraciones es total de los dos for loops. Iteraciones = (n-1) + (n-1)*n/2
// Intercambios es el if, que es una probabilidad.
// - - - - -
void bubbleSort(int *arr, int n, gpointer user_data) {
    // Para poder acceder a los datos de la interfaz y del usuario
    DatosGenerales *general = (DatosGenerales *)user_data;
    GtkBuilder *builder = general->builder;
    DatosUsuario *datos = general->datos;

    GtkWidget *area = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo"));
    gtk_widget_queue_draw(area);

    // Para poder ver el cambio en el circulo
    area_inicial();

    for (int i = 0; i < n - 1; i++) {
        datos->iterations++;
            // Después del swap dibujar otra vez el área del círculo
            gtk_widget_queue_draw(area);
            // Para poder ver el cambio en el contador de iteraciones
            actualizar_area();
        for (int j = 0; j < n - i - 1; j++) {
            datos->iterations++;
            // Después del swap dibujar otra vez el área del círculo
            gtk_widget_queue_draw(area);
            // Para poder ver el cambio en el contador de iteraciones
            actualizar_area();
            if (arr[j] < arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
                
                datos->swaps++;
                // Después del swap dibujar otra vez el área del círculo
                gtk_widget_queue_draw(area);
                // Para poder ver el cambio en el contador de intercambios y los rayos
                actualizar_area();
            }
        }
    }
}
// - - - - -
// COCKTAIL SORT
// Iteraciones es total de los dos for loops. Iteraciones = (n-1) + (n-1)*n/2
// Intercambios es el if, que es una probabilidad.
// - - - - -
void cocktailSort(int *arr, int n, gpointer user_data) {
    // Para poder acceder a los datos de la interfaz y del usuario
    DatosGenerales *general = (DatosGenerales *)user_data;
    GtkBuilder *builder = general->builder;
    DatosUsuario *datos = general->datos;

    GtkWidget *area = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo"));
    gtk_widget_queue_draw(area);

    // Para poder ver el cambio en el circulo
    area_inicial();

    int start = 0;
    int end = n - 1;

    for (int pass = 0; pass < n - 1; pass++) {
        datos->iterations++;
            // Después del swap dibujar otra vez el área del círculo
            gtk_widget_queue_draw(area);
            // Para poder ver el cambio en el contador de iteraciones
            actualizar_area();
        for (int i = start; i < end; ++i) {
            datos->iterations++;
            // Después del swap dibujar otra vez el área del círculo
            gtk_widget_queue_draw(area);
            // Para poder ver el cambio en el contador de iteraciones
            actualizar_area();
            if (arr[i] < arr[i + 1]) {
                swap(&arr[i], &arr[i + 1]);
                datos->swaps++;
                // Después del swap dibujar otra vez el área del círculo
                gtk_widget_queue_draw(area);
                // Para poder ver el cambio en el contador de intercambios y los rayos
                actualizar_area();
            }
        }
        --end;
        for (int i = end - 1; i >= start; --i) {
            datos->iterations++;
            // Después del swap dibujar otra vez el área del círculo
            gtk_widget_queue_draw(area);
            // Para poder ver el cambio en el contador de iteraciones
            actualizar_area();
            if (arr[i] < arr[i + 1]) {
                swap(&arr[i], &arr[i + 1]);
                datos->swaps++;
                // Después del swap dibujar otra vez el área del círculo
                gtk_widget_queue_draw(area);
                // Para poder ver el cambio en el contador de intercambios y los rayos
                actualizar_area();
            }
        }
        
        ++start;
    }
}
// - - - - -
// EXCHANGE SORT
// Iteraciones es total de los dos for loops. Iteraciones = (n-1) + (n-1)*n/2
// Intercambios es el if, que es una probabilidad.
// - - - - -
void exchangeSort(int *arr, int n, gpointer user_data) {
    // Para poder acceder a los datos de la interfaz y del usuario
    DatosGenerales *general = (DatosGenerales *)user_data;
    GtkBuilder *builder = general->builder;
    DatosUsuario *datos = general->datos;

    GtkWidget *area = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo"));
    gtk_widget_queue_draw(area);

    // Para poder ver el cambio en el circulo
    area_inicial();

    for (int i = 0; i < n - 1; i++) {
        datos->iterations++;
        // Después del swap dibujar otra vez el área del círculo
        gtk_widget_queue_draw(area);
        // Para poder ver el cambio en el contador de iteraciones
        actualizar_area();

        for (int j = i + 1; j < n; j++) {
            datos->iterations++;
            // Después del swap dibujar otra vez el área del círculo
            gtk_widget_queue_draw(area);
            // Para poder ver el cambio en el contador de iteraciones
            actualizar_area();
            if (arr[i] < arr[j]) {  // Changed to sort in descending order
                swap(&arr[i], &arr[j]);
                datos->swaps++;
                // Después del swap dibujar otra vez el área del círculo
                gtk_widget_queue_draw(area);
                // Para poder ver el cambio en el contador de intercambios y los rayos
                actualizar_area();
            }
        }
    }
}
// - - - - -
// SELECTION SORT
// Iteraciones es total de los dos for loops. Iteraciones = (n-1) + (n-1)*n/2
// Intercambios es (n-1), ya que se hace solo un intercambio por número.
// - - - - -
void selectionSort(int *arr, int n, gpointer user_data) {
    // Para poder acceder a los datos de la interfaz y del usuario
    DatosGenerales *general = (DatosGenerales *)user_data;
    GtkBuilder *builder = general->builder;
    DatosUsuario *datos = general->datos;

    GtkWidget *area = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo"));
    gtk_widget_queue_draw(area);

    // Para poder ver el cambio en el circulo
    area_inicial();

    for (int i = 0; i < n - 1; ++i) {
        int max_idx = i;  // Change min_idx to max_idx

        datos->iterations++;
        // Después del swap dibujar otra vez el área del círculo
        gtk_widget_queue_draw(area);
        // Para poder ver el cambio en el contador de iteraciones
        actualizar_area();

        for (int j = i + 1; j < n; ++j) {
            datos->iterations++;
            // Después del swap dibujar otra vez el área del círculo
            gtk_widget_queue_draw(area);
            // Para poder ver el cambio en el contador de iteraciones
            actualizar_area();

            if (arr[j] > arr[max_idx]) {  // Change the comparison to find the max element
                max_idx = j;
            }
        }
        swap(&arr[i], &arr[max_idx]);
        datos->swaps++;
        // Después del swap dibujar otra vez el área del círculo
        gtk_widget_queue_draw(area);
        // Para poder ver el cambio en el contador de intercambios y los rayos
        actualizar_area();
    }
}
// - - - - -
// INSERTION SORT
// Iteraciones es total del for loop y el while. Iteraciones = (n-1) + intercambios.
// Intercambios es el while, que es una probabilidad.
// - - - - -
void insertionSort(int *arr, int n, gpointer user_data) {
    // Para poder acceder a los datos de la interfaz y del usuario
    DatosGenerales *general = (DatosGenerales *)user_data;
    GtkBuilder *builder = general->builder;
    DatosUsuario *datos = general->datos;

    GtkWidget *area = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo"));
    gtk_widget_queue_draw(area);

    // Para poder ver el cambio en el circulo
    area_inicial();
    
    int i, j, temp;

    for (i = 1; i < n; i++) {
        temp = arr[i];
        j = i - 1;

        datos->iterations++;
        // Después del swap dibujar otra vez el área del círculo
        gtk_widget_queue_draw(area);
        // Para poder ver el cambio en el contador de iteraciones
        actualizar_area();

        // Change the condition to sort in descending order
        while (j >= 0 && arr[j] < temp) {  // arr[j] < temp to find larger elements
            arr[j + 1] = arr[j];
            j--;

            datos->iterations++;
            datos->swaps++;
            // Después del swap dibujar otra vez el área del círculo
            gtk_widget_queue_draw(area);
            // Para poder ver el cambio en el contador de intercambios y los rayos
            actualizar_area();
        }
        arr[j + 1] = temp;
    }
}
// - - - - -
// MERGE SORT
// Iteraciones es total de los for loops y whiles.
// Total = for loops para copiar datos + whiles para combinar los arreglos
// Intercambios es el if, que es una probabilidad.
// - - - - -
void merge(int* L, int n1, int* R, int n2, int* S, gpointer user_data) {
    // Para poder acceder a los datos de la interfaz y del usuario
    DatosGenerales *general = (DatosGenerales *)user_data;
    GtkBuilder *builder = general->builder;
    DatosUsuario *datos = general->datos;

    GtkWidget *area = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo"));

    int i = 0, j = 0, k = 0;

    // Combinar L y R en S en orden descendente
    while (i < n1 && j < n2) {
        datos->iterations++;
        // Después del swap dibujar otra vez el área del círculo
        gtk_widget_queue_draw(area);
        // Para poder ver el cambio en el contador de iteraciones
        actualizar_area();
        if (L[i] >= R[j]) {
            S[k++] = L[i++];
        } else {
            S[k++] = R[j++];
        }
        datos->swaps++;
        // Después del swap dibujar otra vez el área del círculo
        gtk_widget_queue_draw(area);
        // Para poder ver el cambio en el contador de intercambios y los rayos
        actualizar_area();
    }

    // Copiar elementos que quedan de L
    while (i < n1) {
        datos->iterations++;
        // Después del swap dibujar otra vez el área del círculo
        gtk_widget_queue_draw(area);
        // Para poder ver el cambio en el contador de iteraciones
        actualizar_area();

        S[k++] = L[i++];
    }
    // Copiar elementos que quedan de R
    while (j < n2) {
        datos->iterations++;
        // Después del swap dibujar otra vez el área del círculo
        gtk_widget_queue_draw(area);
        // Para poder ver el cambio en el contador de iteraciones
        actualizar_area();

        S[k++] = R[j++];
    }
}
void mergeSort(int* arr, int n, gpointer user_data) {
    // Para poder acceder a los datos de la interfaz y del usuario
    DatosGenerales *general = (DatosGenerales *)user_data;
    GtkBuilder *builder = general->builder;
    DatosUsuario *datos = general->datos;

    GtkWidget *area = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo"));
    gtk_widget_queue_draw(area);

    // Para poder ver el cambio en el circulo
    area_inicial();

    // Si el tamaño del arreglo es menor o igual a 1
    if (n <= 1) return;

    // Tamaño de los arreglos L y R
    int n1 = n / 2;
    int n2 = n - n1;

    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));

    if (!L || !R) {
        g_warning("No se pudo crear memoria para R y L en el merge sort.");
        return;
    }

    // Copiar números en L y R
    for (int i = 0; i < n1; ++i){
        L[i] = arr[i];
        
        datos->iterations++;
        // Después del swap dibujar otra vez el área del círculo
        gtk_widget_queue_draw(area);
        // Para poder ver el cambio en el contador de iteraciones
        actualizar_area();
    }
    for (int i = 0; i < n2; ++i){
        R[i] = arr[n1 + i];

        datos->iterations++;
        // Después del swap dibujar otra vez el área del círculo
        gtk_widget_queue_draw(area);
        // Para poder ver el cambio en el contador de iteraciones
        actualizar_area();
    }

    // Llamadas recursivas
    mergeSort(L, n1, user_data);
    mergeSort(R, n2, user_data);

    // Llamar a la función merge para ordenar a L y R
    merge(L, n1, R, n2, arr, user_data);

    // Soltar la memoria de L y R
    free(L);
    free(R);
}
// - - - - -
// QUICK SORT
// Iteraciones es total de los for loops y whiles.
// Intercambios es el if, que es una probabilidad.
// - - - - -
int partition(int* arr, int *p, gpointer user_data, int low, int high) {
    // Para poder acceder a los datos de la interfaz y del usuario
    DatosGenerales *general = (DatosGenerales *)user_data;
    GtkBuilder *builder = general->builder;
    DatosUsuario *datos = general->datos;

    GtkWidget *area = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo"));

    int i,j;
    // Se escoge el pivote
    int pivot_item = arr[low];
    j = low;

    // Crear arreglos con valores menores y mayores que el pivote
    for ( i = low + 1; i<= high; i++) {
        
        datos->iterations++;
        // Después del swap dibujar otra vez el área del círculo
        gtk_widget_queue_draw(area);
        // Para poder ver el cambio en el contador de iteraciones
        actualizar_area();

        if (arr[i] > pivot_item) {
            j++;
            swap(&arr[i], &arr[j]);
            
            datos->swaps++;
            // Después del swap dibujar otra vez el área del círculo
            gtk_widget_queue_draw(area);
            // Para poder ver el cambio en el contador de intercambios y los rayos
            actualizar_area();
        }
    }
    *p = j;
    swap(&arr[low], &arr[*p]);
    
    datos->swaps++;
    // Después del swap dibujar otra vez el área del círculo
    gtk_widget_queue_draw(area);
    // Para poder ver el cambio en el contador de intercambios y los rayos
    actualizar_area();
}

void quickSort(int* arr, gpointer user_data, int low, int high) {
    // Para poder acceder a los datos de la interfaz y del usuario
    DatosGenerales *general = (DatosGenerales *)user_data;
    GtkBuilder *builder = general->builder;
    DatosUsuario *datos = general->datos;

    GtkWidget *area = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo"));
    gtk_widget_queue_draw(area);
    // Para poder ver el cambio en el circulo
    area_inicial();

    int pivot;
    if (low < high) {
        // Se divide el arreglo
        partition(arr, &pivot, user_data, low, high);
        // Llamadas recursivas con nuevos arreglos
        quickSort(arr, user_data, low, pivot - 1);
        quickSort(arr, user_data, pivot + 1, high);
    }
}
// - - - - -
// SHELL SORT
// Iteraciones es total de los for loops.
// Intercambios están en el tercer for loop.
// - - - - -
void shellSort(int* arr, int n, gpointer user_data) {
    // Para poder acceder a los datos de la interfaz y del usuario
    DatosGenerales *general = (DatosGenerales *)user_data;
    GtkBuilder *builder = general->builder;
    DatosUsuario *datos = general->datos;

    GtkWidget *area = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo"));
    gtk_widget_queue_draw(area);

    // Para poder ver el cambio en el circulo
    area_inicial();

    for (int gap = n / 2; gap > 0; gap /= 2) {
        
        datos->iterations++;
        // Después del swap dibujar otra vez el área del círculo
        gtk_widget_queue_draw(area);
        // Para poder ver el cambio en el contador de iteraciones
        actualizar_area();

        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;

            datos->iterations++;
            // Después del swap dibujar otra vez el área del círculo
            gtk_widget_queue_draw(area);
            // Para poder ver el cambio en el contador de iteraciones
            actualizar_area();

            for (j = i; j >= gap && arr[j - gap] < temp; j -= gap) {
                arr[j] = arr[j - gap];

                datos->iterations++;
                datos->swaps++;
                // Después del swap dibujar otra vez el área del círculo
                gtk_widget_queue_draw(area);
                // Para poder ver el cambio en el contador de iteraciones
                actualizar_area();
            }
            arr[j] = temp;
        }
    }
}
// - - - - -
// GNOME SORT
// Iteraciones es el loop del while.
// Intercambios son un if, entonces es una probabilidad.
// - - - - -
void gnomeSort(int* arr, int n, gpointer user_data) {
    // Para poder acceder a los datos de la interfaz y del usuario
    DatosGenerales *general = (DatosGenerales *)user_data;
    GtkBuilder *builder = general->builder;
    DatosUsuario *datos = general->datos;

    GtkWidget *area = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo"));
    gtk_widget_queue_draw(area);

    // Para poder ver el cambio en el circulo
    area_inicial();

    int index = 0;

    while (index < n) {
        datos->iterations++;
        // Después del swap dibujar otra vez el área del círculo
        gtk_widget_queue_draw(area);
        // Para poder ver el cambio en el contador de iteraciones
        actualizar_area();

        if (index == 0)
            index++;
        if (arr[index] <= arr[index - 1])
            index++;
        else {
            swap(&arr[index], &arr[index - 1]);
            index--;

            datos->swaps++;
            // Después del swap dibujar otra vez el área del círculo
            gtk_widget_queue_draw(area);
            // Para poder ver el cambio en el contador de iteraciones
            actualizar_area();
        }
    }
}
// - - - - -
// PANCAKE SORT
// Iteraciones son los for loops y el loop del while.
// Intercambios están en la función flip.
// - - - - -
void flip(int* arr, int i, gpointer user_data) {
    // Para poder acceder a los datos de la interfaz y del usuario
    DatosGenerales *general = (DatosGenerales *)user_data;
    GtkBuilder *builder = general->builder;
    DatosUsuario *datos = general->datos;

    GtkWidget *area = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo"));

    int temp = 0;
    int start = datos->k - 1;
    while (start > i) {
        temp = arr[i];
        arr[i] = arr[start];
        arr[start] = temp;
        start--;
        i++;

        datos->iterations++;
        datos->swaps++;
        gtk_widget_queue_draw(area);
        // Para poder ver el cambio en el contador de iteraciones
        actualizar_area();
    }
}

int findMax(int* arr, int current, int size, gpointer user_data) {
    // Para poder acceder a los datos de la interfaz y del usuario
    DatosGenerales *general = (DatosGenerales *)user_data;
    GtkBuilder *builder = general->builder;
    DatosUsuario *datos = general->datos;

    GtkWidget *area = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo"));
    
    int max_index = current;
    for (int i = current; i < size; i++) {
        datos->iterations++;
        gtk_widget_queue_draw(area);
        // Para poder ver el cambio en el contador de iteraciones
        actualizar_area();
        
        if (arr[i] > arr[max_index]) {
            max_index = i;
        }
    }
    return max_index;
}

void pancakeSort(int* arr, int n, gpointer user_data) {
    // Para poder acceder a los datos de la interfaz y del usuario
    DatosGenerales *general = (DatosGenerales *)user_data;
    GtkBuilder *builder = general->builder;
    DatosUsuario *datos = general->datos;

    GtkWidget *area = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo"));
    gtk_widget_queue_draw(area);
    // Para poder ver el cambio en el contador de iteraciones
    area_inicial();

    for (int i = 0; i < n; i++) {
        datos->iterations++;
        gtk_widget_queue_draw(area);
        // Para poder ver el cambio en el contador de iteraciones
        actualizar_area();

        int max_index = findMax(arr, i, n, user_data);

        if (max_index != i) {
            // Llevar el elemento más grande hacia el final
            flip(arr, max_index, user_data);
            // Mover el elemento más grande hacia el frente, invirtiendo el arreglo
            flip(arr, i, user_data);
        }
    }
}