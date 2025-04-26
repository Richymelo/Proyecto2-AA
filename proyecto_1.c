/*
                Proyecto 1: Ordenando Números
                Hecho por: Carmen Hidalgo Paz, Jorge Guevara Chavarría y Ricardo Castro Jiménez
                Fecha: Jueves 10 de abril del 2025

                Esta sección contiene el main, donde se indica lo que tiene que hacer
                cada objeto mostrado en la interfaz. Además, hay una función que muestra los
                rayos y el círculo editado con los valores que ingresa el usuario.
                Después se ordenan los rayos con el algoritmo de ordenamiento escogido
                por el usuario.
*/
#include <gtk/gtk.h>
#include <cairo.h>
#include <stdbool.h>
#include <math.h>     // Para calcular los rayos
#include <time.h>     // Para utilizar rand()
#include "Datos_usuario.h"
#include "ordenamientos.h"

#define PI 3.14159265358979323846

// Para que no se mueva la línea del panel
void fijar_panel(GtkPaned *panel, GParamSpec *pspec, gpointer user_data) {
    const int pos_fijada = 895;    // Posición donde se fija la división
    int current_pos = gtk_paned_get_position(panel);
    if (current_pos != pos_fijada) {
        gtk_paned_set_position(panel, pos_fijada);
    }
}
// Barajar datos de un vector (con el algoritmo de Fisher-Yates o Knuth)
void barajar_datos(int *datos, int size) {
    // Si el vector es de 1 solo valor o menos
    if (datos == NULL || size <= 1) return; 

    for (int i = size - 1; i > 0; i--) {
        // Se escoge una posición aleatoria
        int j = rand() % (i + 1);
        // Se cambian de posición el valor en la posición aleatoria y el de la posición actual
        int temp = datos[i];
        datos[i] = datos[j];
        datos[j] = temp;
    }
}
// Obtener datos nuevos
void desplegar_datos_nuevos(GtkButton *button, gpointer user_data) {
    DatosGenerales *general = (DatosGenerales *)user_data;
    GtkBuilder *builder = general->builder;
    DatosUsuario *datos = general->datos;

    // Número de rayos ingresados por el usuario
    GtkWidget *cantidad_rayos = GTK_WIDGET(gtk_builder_get_object(builder, "cantidad_rayos"));
    int N = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(cantidad_rayos));
    // Cantidad de datos ingresados por el usuario
    GtkWidget *cantidad_datos = GTK_WIDGET(gtk_builder_get_object(builder, "cantidad_datos"));
    int k = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(cantidad_datos));
    // Se guarda el primer color escogido
    GdkRGBA color_1;
    GtkWidget *primer_color = GTK_WIDGET(gtk_builder_get_object(builder, "color_1"));
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(primer_color), &color_1);
    // Se guarda el segundo color escogido
    GdkRGBA color_2;
    GtkWidget *segundo_color = GTK_WIDGET(gtk_builder_get_object(builder, "color_2"));
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(segundo_color), &color_2);

    // Datos del usuario
    datos->N = N;
    datos->k = k;
    datos->color_1 = color_1;
    datos->color_2 = color_2;
    datos->iterations = 0;
    datos->swaps = 0;
    
    // Limpiar memoria si ya había un vector
    if (datos->D != NULL) {
        free(datos->D);
    }
    // Crear un vector con k espacios en memoria dinámica
    datos->D = malloc(sizeof(int) * k);
    // Si el número ingresado es muy grande
    if (datos->D == NULL) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
        "No se puede asignar memoria para esta cantidad de dígitos. Por favor ingrese un valor más pequeño.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;  // El usuario puede volver a intentar
    }
    for (int i = 0; i < k; i++) {
        datos->D[i] = i + 1;  // Llenar el vector con datos desde 1 hasta k
    }
    // Dependiendo del orden escogido, así se llenan los datos
    GtkWidget *radio_ascendente = GTK_WIDGET(gtk_builder_get_object(builder, "orden_ascendente"));
    GtkWidget *radio_descendente = GTK_WIDGET(gtk_builder_get_object(builder, "orden_descendente"));
    GtkWidget *radio_aleatorio = GTK_WIDGET(gtk_builder_get_object(builder, "orden_aleatorio"));
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_ascendente))) {
        for (int i = 0; i < k; i++) {
            datos->D[i] = i + 1;  // Llenar el vector con datos desde 1 hasta k
        }
    }
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_descendente))) {
        for (int i = 0; i < k; i++) {
            datos->D[i] = k - i;  // Llenar el vector con datos desde k hasta 1
        }
    }
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_aleatorio))) {
        for (int i = 0; i < k; i++) {
            datos->D[i] = i + 1;  // Llenar el vector con datos desde 1 hasta k
        }
        barajar_datos(datos->D, k);   // Cambiar de orden los valores en el vector
    }
    datos->usar_copia = FALSE;
    
    // Volver a dibujar el círculo, esta vez con los rayos
    GtkWidget *area_circulo = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo"));
    gtk_widget_queue_draw(area_circulo);
}
// Desplegar datos ya creados
void desplegar_datos_iniciales(GtkButton *button, gpointer user_data) {
    DatosGenerales *general = (DatosGenerales *)user_data;
    GtkBuilder *builder = general->builder;
    DatosUsuario *datos = general->datos;

    // Se guarda el primer color escogido
    GdkRGBA color_1;
    GtkWidget *primer_color = GTK_WIDGET(gtk_builder_get_object(builder, "color_1"));
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(primer_color), &color_1);
    // Se guarda el segundo color escogido
    GdkRGBA color_2;
    GtkWidget *segundo_color = GTK_WIDGET(gtk_builder_get_object(builder, "color_2"));
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(segundo_color), &color_2);
    datos->usar_copia = FALSE;

    // Datos del usuario
    datos->color_1 = color_1;
    datos->color_2 = color_2;
    datos->iterations = 0;
    datos->swaps = 0;
    
    // Volver a dibujar el círculo, esta vez con los rayos
    GtkWidget *area_circulo = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo"));
    gtk_widget_queue_draw(area_circulo);
    // Esperar a que GTK procese lo que ocupa
    while (gtk_events_pending()) {
        gtk_main_iteration();
    }
    g_usleep(500000);  // Un delay para verlo bien
}
// Crea la lista de colores que va a corresponder a los rayos
void colorLinea(int *D, int cElementosV, int color1[3], int color2[3], int colores[][3]) {
	for (int i = 0; i < cElementosV; i++) {
		for (int j = 0; j < 3; j++) {
			colores[i][j] = color1[j] + ((D[i] - 1) * (color2[j] - color1[j])) / (cElementosV - 1);
		}
	}
}
// Ordenar datos
void sort(GtkButton *button, gpointer user_data) {
    DatosGenerales *general = (DatosGenerales *)user_data;
    GtkBuilder *builder = general->builder;
    DatosUsuario *datos = general->datos;

    GtkWidget *area_circulo = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo"));
    GtkWidget *algos_ordenamiento = GTK_WIDGET(gtk_builder_get_object(builder, "lista_algos")); // Get the combo box widget

    // Combo box con los algoritmos de ordenamiento
    const char *algo_seleccionado = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(algos_ordenamiento));

    desplegar_datos_iniciales(button, user_data);
    
    // Crear copia del arreglo original
    int k = datos->k;
    // Si copia_datos ya existe, liberar esa memoria
    if (datos->copia_datos != NULL) {
        free(datos->copia_datos);
    }

    // Crear un vector con k espacios en memoria dinámica
    datos->copia_datos = malloc(sizeof(int) * k);
    
    // Si el número ingresado es muy grande
    if (datos->copia_datos == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
            "No se puede asignar memoria para esta cantidad de dígitos. Por favor ingrese un valor más pequeño.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;  // El usuario puede volver a intentar
    }

    // Copiar elementos del arreglo inicial en el arreglo de copia
    for (int i = 0; i < k; i++) {
        datos->copia_datos[i] = datos->D[i];
    }
    // Se prende la bandera para que se utilize el arreglo de copia
    datos->usar_copia = TRUE;

    // Escoger el algoritmo seleccionado por el usuario
    if (strcmp(algo_seleccionado, "Bubble Sort") == 0) {
        bubbleSort(datos->copia_datos, k, user_data);   // El Bubble Sort
    } else if (strcmp(algo_seleccionado, "Cocktail Sort") == 0) {
        cocktailSort(datos->copia_datos, k, user_data);  // El Cocktail Sort
    } else if (strcmp(algo_seleccionado, "Exchange Sort") == 0) {
        exchangeSort(datos->copia_datos, k, user_data);  // El Exchange Sort
    } else if (strcmp(algo_seleccionado, "Selection Sort") == 0) {
        selectionSort(datos->copia_datos, k, user_data);  // El Selection Sort
    } else if (strcmp(algo_seleccionado, "Insertion Sort") == 0) {
        insertionSort(datos->copia_datos, k, user_data);  // El Insertion Sort
    } else if (strcmp(algo_seleccionado, "Merge Sort") == 0) {
        mergeSort(datos->copia_datos, k, user_data);  // El Merge Sort
    } else if (strcmp(algo_seleccionado, "Quick Sort") == 0) {
        quickSort(datos->copia_datos, user_data, 0, k-1);  // El Quick Sort
    } else if (strcmp(algo_seleccionado, "Shell Sort") == 0) {
        shellSort(datos->copia_datos, k, user_data);  // El Shell Sort
    } else if (strcmp(algo_seleccionado, "Gnome Sort") == 0) {
        gnomeSort(datos->copia_datos, k, user_data);  // El Gnome Sort
    } else if (strcmp(algo_seleccionado, "Pancake Sort") == 0) {
        pancakeSort(datos->copia_datos, k, user_data);  // El Pancake Sort
    }

    // Mostrar mensaje cuando terminó el sort
    GtkWidget *sort_terminado = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
        "¡Ha terminado el sort!");
    gtk_dialog_run(GTK_DIALOG(sort_terminado));
    gtk_widget_destroy(sort_terminado);

    // Liberar memoria del string del combo box
    g_free((gpointer)algo_seleccionado);
}
// Área de dibujo
gboolean dibujar_area(GtkWidget *area, cairo_t *cr, gpointer user_data) {
    DatosUsuario *datos = (DatosUsuario *)user_data;
    // Obtener el centro del área de dibujo
    int xc = gtk_widget_get_allocated_width(area) / 2;
    int yc = gtk_widget_get_allocated_height(area) / 2;
    int R = MIN(xc, yc); // Radio del círculo
    
    // Limpiar el área de dibujo (fondo blanco)
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);
    
    // Dibujar círculo guía
    cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
    cairo_arc(cr, xc, yc, R, 0, 2 * PI);
    cairo_stroke(cr);
    
    // Si el usuario ha agregado datos, dibujar los rayos
    if (datos->D != NULL) {
        // Datos ingresados
        int N = datos->N;     // Cantidad de rayos
        int k = datos->k;     // Cantidad de datos
        int *D = datos->usar_copia ? datos->copia_datos : datos->D; // Use sorted or original array
        
        // Configurar grosor de las líneas
        cairo_set_line_width(cr, 5);

        // Pasar de RBGA a RGB
        int color1[3] = {
            (int)(datos->color_1.red * 255),
            (int)(datos->color_1.green * 255),
            (int)(datos->color_1.blue * 255)
        };
        int color2[3] = {
            (int)(datos->color_2.red * 255),
            (int)(datos->color_2.green * 255),
            (int)(datos->color_2.blue * 255)
        };
        // Arreglo con los colores para todas los rayos
        int colores[datos->k][3];
        // Función que llena el arreglo con colores
        colorLinea(D, datos->k, color1, color2, colores);
        
        // Dibujar los rayos desde el centro
        for (int i = 0; i < k; i++) {
            double angle = (i / (double)N) * 2 * PI;    // Ángulo en radianes
            double length = (D[i] / (double)k) * R;     // Longitud proporcional
            double x_end = xc + length * cos(angle);
            double y_end = yc - length * sin(angle);    // Negativo porque Y crece hacia abajo en GTK
            
            // Convierte los valores del RGB para que cairo los pueda utilizar
            double r = colores[i][0] / 255.0;
            double g = colores[i][1] / 255.0;
            double b = colores[i][2] / 255.0;
            
            cairo_set_source_rgb(cr, r, g, b);
            cairo_move_to(cr, xc, yc);
            cairo_line_to(cr, x_end, y_end);
            cairo_stroke(cr);
        }
    }
    int width = gtk_widget_get_allocated_width(area);
    int height = gtk_widget_get_allocated_height(area);

    // Color y tipo de letra
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_select_font_face(cr, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 16);

    // Dibujar contador de iteración
    char iteration_text[50];
    snprintf(iteration_text, sizeof(iteration_text), "Iteraciones: %d", datos->iterations);
    cairo_move_to(cr, width - 180, height - 50);  // Adjust 180 to fit your text
    cairo_show_text(cr, iteration_text);

    // Dibujar contador de intercambios
    char swap_text[50];
    snprintf(swap_text, sizeof(swap_text), "Intercambios: %d", datos->swaps);
    cairo_move_to(cr, width - 180, height - 20);  // Adjust 180 to fit your text
    cairo_show_text(cr, swap_text);

    return FALSE;
}

int main(int argc, char *argv[]) {
    GtkBuilder *builder;        // Utilizado para obtener los objetos de glade
    GtkWidget *ventana;         // La ventana
    GtkWidget *area_circulo;    // El área donde se dibuja el círculo
    GtkWidget *panel;           // El panel que divide el área de dibujo y el área de interacción
    GtkWidget *boton_salida;    // Botón para terminar el programa
    GtkWidget *boton_desplegar_nuevos; // Botón para mostrar datos nuevos
    GtkWidget *boton_desplegar_iniciales; // Botón para mostrar los datos originales
    GtkWidget *boton_sort;      // Botón para ordenar los datos
    GtkWidget *cantidad_rayos;  // Espacio para ingresar cantidad de rayos N
    GtkWidget *cantidad_datos;  // Espacio para ingresar la cantidad de datos k
    GtkWidget *color_1;         // Primer color escogido
    GtkWidget *color_2;         // Segundo color escogido

    srand(time(NULL));  // Para poder utilizar rand() al barajar los números
    gtk_init(&argc, &argv);

    // Cargar la interfaz de Glade
    builder = gtk_builder_new_from_file("interfaz.glade");

    // Se asigna memoria para la estructura de DatosUsuario
    DatosUsuario *datos = malloc(sizeof(DatosUsuario));
    if (!datos) {
        fprintf(stderr, "No se pudo asignar memoria dinámica.\n");
        return EXIT_FAILURE;
    }
    // Se guardan valores en las variables
    datos->D = NULL;
    datos->copia_datos = NULL;
    datos->N = 0;
    datos->k = 0;
    datos->usar_copia = FALSE;
    datos->color_1 = (GdkRGBA){0, 0, 0, 1};
    datos->color_2 = (GdkRGBA){0, 0, 0, 1};
    datos->iterations = 0;
    datos->swaps = 0;

    // Se asigna memoria para la estructura de DatosGenerales
    DatosGenerales *general = malloc(sizeof(DatosGenerales));
    if (!general) {
        fprintf(stderr, "No se pudo asignar memoria dinámica.\n");
        free(datos);
        return EXIT_FAILURE;
    }
    general->builder = builder;
    general->datos = datos;

    // La ventana
    ventana = GTK_WIDGET(gtk_builder_get_object(builder, "ventana"));
    g_signal_connect(ventana, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // El panel divisor
    panel = GTK_WIDGET(gtk_builder_get_object(builder, "division"));
    g_signal_connect(panel, "notify::position", G_CALLBACK(fijar_panel), NULL);
    // El círculo
    area_circulo = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo"));
    g_signal_connect(area_circulo, "draw", G_CALLBACK(dibujar_area), datos);
    // Botón para desplegar los datos nuevos
    boton_desplegar_nuevos = GTK_WIDGET(gtk_builder_get_object(builder, "boton_desplegar_nuevos"));
    g_signal_connect(boton_desplegar_nuevos, "clicked", G_CALLBACK(desplegar_datos_nuevos), general);
    // Botón para volver a utilizar los datos ya creados
    boton_desplegar_iniciales = GTK_WIDGET(gtk_builder_get_object(builder, "boton_desplegar_iniciales"));
    g_signal_connect(boton_desplegar_iniciales, "clicked", G_CALLBACK(desplegar_datos_iniciales), general);
    // Botón para ordenar los datos del usuario
    boton_sort = GTK_WIDGET(gtk_builder_get_object(builder, "boton_sort"));
    g_signal_connect(boton_sort, "clicked", G_CALLBACK(sort), general);
    // El bóton de terminación del programa
    boton_salida = GTK_WIDGET(gtk_builder_get_object(builder, "boton_terminar"));
    g_signal_connect(boton_salida, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    // Mostrar ventana
    gtk_widget_show_all(ventana);
    // Que la ventana utilize toda la pantalla
    gtk_window_fullscreen(GTK_WINDOW(ventana));

    // Correr GTK
    gtk_main();

    // Limpiar la memoria
    if (datos->D != NULL) {
        free(datos->D);
    }
    if (datos->copia_datos != NULL) {
        free(datos->copia_datos);
    }
    free(datos);
    free(general);
    g_object_unref(builder);

    return 0;
}