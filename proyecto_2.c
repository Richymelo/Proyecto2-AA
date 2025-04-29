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
    const int pos_fijada = 1040;    // Posición donde se fija la división
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
void desplegar_datos_nuevos(GtkButton *button, gpointer user_data) {
    DatosGenerales *general = (DatosGenerales *)user_data;
    GtkBuilder *builder = general->builder;
    DatosUsuario *datos = general->datos;

    // Leer lo que el usuario ingresó
    GtkWidget *cantidad_rayos = GTK_WIDGET(gtk_builder_get_object(builder, "cantidad_rayos"));
    int N = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(cantidad_rayos));

    GtkWidget *cantidad_datos = GTK_WIDGET(gtk_builder_get_object(builder, "cantidad_datos"));
    int k = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(cantidad_datos));

    GdkRGBA color_1;
    GtkWidget *primer_color = GTK_WIDGET(gtk_builder_get_object(builder, "color_1"));
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(primer_color), &color_1);

    GdkRGBA color_2;
    GtkWidget *segundo_color = GTK_WIDGET(gtk_builder_get_object(builder, "color_2"));
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(segundo_color), &color_2);

    // Create un arreglo principal
    int *master_array = malloc(sizeof(int) * k);
    if (!master_array) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
            "No se puede asignar memoria para los datos iniciales.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    for (int i = 0; i < k; i++) {
        master_array[i] = i + 1;
    }

    // Ordenar los valores del arreglo principal
    GtkWidget *radio_ascendente = GTK_WIDGET(gtk_builder_get_object(builder, "orden_ascendente"));
    GtkWidget *radio_descendente = GTK_WIDGET(gtk_builder_get_object(builder, "orden_descendente"));
    GtkWidget *radio_aleatorio = GTK_WIDGET(gtk_builder_get_object(builder, "orden_aleatorio"));

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_descendente))) {
        for (int i = 0; i < k; i++) {
            master_array[i] = k - i;
        }
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_aleatorio))) {
        barajar_datos(master_array, k);
    }

    // Copiar datos para cada círculo
    for (int idx = 0; idx < 4; idx++) {
        datos[idx].N = N;
        datos[idx].k = k;
        datos[idx].color_1 = color_1;
        datos[idx].color_2 = color_2;
        datos[idx].iterations = 0;
        datos[idx].swaps = 0;

        // Liberar memoria antigua
        if (datos[idx].D != NULL) {
            free(datos[idx].D);
        }

        // Alocar nueva memoria
        datos[idx].D = malloc(sizeof(int) * k);
        if (datos[idx].D == NULL) {
            GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                "No se pudo asignar memoria para el vector de datos.");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            free(master_array);
            return;
        }

        // Copiar el arreglo principal
        for (int i = 0; i < k; i++) {
            datos[idx].D[i] = master_array[i];
        }
        // Poner la bandera del arreglo copia en falso
        datos[idx].usar_copia = FALSE;
    }

    // Liberar memoria del arreglo principal
    free(master_array);

    // Dibujar los 4 círculos
    for (int idx = 0; idx < 4; idx++) {
        gtk_widget_queue_draw(datos[idx].area);
    }
}
// Desplegar datos ya creados
void desplegar_datos_iniciales(GtkButton *button, gpointer user_data) {
    DatosGenerales *general = (DatosGenerales *)user_data;
    GtkBuilder *builder = general->builder;
    DatosUsuario *datos = general->datos;

    // Guardar los dos colores
    GdkRGBA color_1;
    GtkWidget *primer_color = GTK_WIDGET(gtk_builder_get_object(builder, "color_1"));
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(primer_color), &color_1);

    GdkRGBA color_2;
    GtkWidget *segundo_color = GTK_WIDGET(gtk_builder_get_object(builder, "color_2"));
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(segundo_color), &color_2);

    // Modificar los datos de los círculos
    for (int i = 0; i < 4; i++) {
        datos[i].usar_copia = FALSE;
        datos[i].color_1 = color_1;
        datos[i].color_2 = color_2;
        datos[i].iterations = 0;
        datos[i].swaps = 0;
        // DIbujar el área
        gtk_widget_queue_draw(datos[i].area);
    }

    while (gtk_events_pending()) {
        gtk_main_iteration();
    }
    g_usleep(500000);  // Para ver el cambio
}
// Crea la lista de colores que va a corresponder a los rayos
void colorLinea(int *D, int cElementosV, int color1[3], int color2[3], int colores[][3]) {
	for (int i = 0; i < cElementosV; i++) {
		for (int j = 0; j < 3; j++) {
			colores[i][j] = color1[j] + ((D[i] - 1) * (color2[j] - color1[j])) / (cElementosV - 1);
		}
	}
}
// Aquí el thread escoge el sort
void* sorting_threads(void* arg) {
    // Datos de un círculo
    DatosUsuario* datos = (DatosUsuario*)arg;
    // Crea memoria para la estructura que va a guardar todos los datos
    // necesarios para todos los sorts
    SortState *state = malloc(sizeof(SortState));
    if (!state) {
        printf("Error allocating SortState\n");
        pthread_exit(NULL);
    }

    // Inicializar datos
    memset(state, 0, sizeof(SortState));  // Inicializa todo en 0
    state->n = datos->k;
    state->datos = datos;
    state->gap = datos->k / 2;    // para el shell
    state->low = 0;               // para el merge
    state->start = 0;             // para el cocktail
    state->end = datos->k - 1;    // para el cocktail
    state->index = 0;             // para el gnome
    state->temp = NULL;           // para el merge
    state->stack = NULL;          // para el quick sort
    state->top = -1;
    state->phase = 0;             // Usado por algunos sorts

    // Se escoge el sort
    if (strcmp(datos->algoritmo, "Bubble Sort") == 0) {
        state->current_algorithm = SORT_BUBBLE;
    } else if (strcmp(datos->algoritmo, "Cocktail Sort") == 0) {
        state->current_algorithm = SORT_COCKTAIL;
    } else if (strcmp(datos->algoritmo, "Exchange Sort") == 0) {
        state->current_algorithm = SORT_EXCHANGE;
    } else if (strcmp(datos->algoritmo, "Selection Sort") == 0) {
        state->current_algorithm = SORT_SELECTION;
        state->max_index = 0; // Para selection
    } else if (strcmp(datos->algoritmo, "Insertion Sort") == 0) {
        state->current_algorithm = SORT_INSERTION;
    } else if (strcmp(datos->algoritmo, "Merge Sort") == 0) {
        state->current_algorithm = SORT_MERGE;
    } else if (strcmp(datos->algoritmo, "Quick Sort") == 0) {
        state->current_algorithm = SORT_QUICK;
    } else if (strcmp(datos->algoritmo, "Shell Sort") == 0) {
        state->current_algorithm = SORT_SHELL;
    } else if (strcmp(datos->algoritmo, "Gnome Sort") == 0) {
        state->current_algorithm = SORT_GNOME;
    } else if (strcmp(datos->algoritmo, "Pancake Sort") == 0) {
        state->current_algorithm = SORT_PANCAKE;
    } else {
        printf("Error: algoritmo desconocido '%s'\n", datos->algoritmo);
        free(state);
        pthread_exit(NULL);
    }

    // Se empieza el paso del algoritmo
    g_idle_add(sort_step, state);
    // Se termina el thread
    pthread_exit(NULL);
}
// Ordenar datos
void sort(GtkButton *button, gpointer user_data) {
    DatosGenerales *general = (DatosGenerales *)user_data;
    GtkBuilder *builder = general->builder;

    pthread_t threads[4];

    desplegar_datos_iniciales(button, general);
    while (gtk_events_pending()) {
        gtk_main_iteration();
    }
    g_usleep(500000);  // Para ver el cambio
    // Combobox de todos los círculos
    GtkWidget *algos_ordenamiento1 = GTK_WIDGET(gtk_builder_get_object(builder, "lista_algos1"));
    GtkWidget *algos_ordenamiento2 = GTK_WIDGET(gtk_builder_get_object(builder, "lista_algos2"));
    GtkWidget *algos_ordenamiento3 = GTK_WIDGET(gtk_builder_get_object(builder, "lista_algos3"));
    GtkWidget *algos_ordenamiento4 = GTK_WIDGET(gtk_builder_get_object(builder, "lista_algos4"));

    const char *algo_seleccionado1 = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(algos_ordenamiento1));
    const char *algo_seleccionado2 = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(algos_ordenamiento2));
    const char *algo_seleccionado3 = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(algos_ordenamiento3));
    const char *algo_seleccionado4 = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(algos_ordenamiento4));
    // Crear la copia de los datos en todos los círculos
    for (int i = 0; i < 4; i++) {
        if (general->datos[i].copia_datos != NULL) {
            free(general->datos[i].copia_datos);
        }
    
        general->datos[i].copia_datos = malloc(sizeof(int) * general->datos[i].k);
    
        for (int j = 0; j < general->datos[i].k; j++) {
            general->datos[i].copia_datos[j] = general->datos[i].D[j];
        }
    
        general->datos[i].usar_copia = TRUE;
    }
    // Asignar a cual círculo le tocó cual algoritmo
    strcpy(general->datos[0].algoritmo, algo_seleccionado1);
    strcpy(general->datos[1].algoritmo, algo_seleccionado2);
    strcpy(general->datos[2].algoritmo, algo_seleccionado3);
    strcpy(general->datos[3].algoritmo, algo_seleccionado4);

    // Se crean los threads
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, sorting_threads, &general->datos[i]);
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }
    // Liberar memoria de los algoritmos seleccionados del combo box
    g_free((gpointer)algo_seleccionado1);
    g_free((gpointer)algo_seleccionado2);
    g_free((gpointer)algo_seleccionado3);
    g_free((gpointer)algo_seleccionado4);
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
    cairo_set_font_size(cr, 10);
    
    // Dibujar contador de iteración
    char iteration_text[50];
    snprintf(iteration_text, sizeof(iteration_text), "Iteraciones: %d", datos->iterations);
    cairo_move_to(cr, width - 112, height - 15);
    cairo_show_text(cr, iteration_text);

    // Dibujar contador de intercambios
    char swap_text[50];
    snprintf(swap_text, sizeof(swap_text), "Intercambios: %d", datos->swaps);
    cairo_move_to(cr, width - 112, height - 5);
    cairo_show_text(cr, swap_text);

    return FALSE;
}

int main(int argc, char *argv[]) {
    GtkBuilder *builder;         // Utilizado para obtener los objetos de glade
    GtkWidget *ventana;          // La ventana
    GtkWidget *area_circulo1;    // El área donde se dibuja el círculo 1
    GtkWidget *area_circulo2;    // El área donde se dibuja el círculo 2
    GtkWidget *area_circulo3;    // El área donde se dibuja el círculo 3
    GtkWidget *area_circulo4;    // El área donde se dibuja el círculo 4
    GtkWidget *panel;            // El panel que divide el área de dibujo y el área de interacción
    GtkWidget *boton_salida;     // Botón para terminar el programa
    GtkWidget *boton_desplegar_nuevos;    // Botón para mostrar datos nuevos
    GtkWidget *boton_desplegar_iniciales; // Botón para mostrar los datos originales
    GtkWidget *boton_sort;       // Botón para ordenar los datos
    GtkWidget *cantidad_rayos;   // Espacio para ingresar cantidad de rayos N
    GtkWidget *cantidad_datos;   // Espacio para ingresar la cantidad de datos k
    GtkWidget *color_1;          // Primer color escogido
    GtkWidget *color_2;          // Segundo color escogido

    srand(time(NULL));  // Para poder utilizar rand() al barajar los números
    gtk_init(&argc, &argv);

    // Cargar la interfaz de Glade
    builder = gtk_builder_new_from_file("interfaz.glade");

    // Se asigna memoria para la estructura de DatosUsuario para los 4 círculos
    DatosUsuario *datos = malloc(4 * sizeof(DatosUsuario));
    if (!datos) {
        fprintf(stderr, "No se pudo asignar memoria dinámica.\n");
        return EXIT_FAILURE;
    }

    // Inicializar los datos para cada círculo
    for (int i = 0; i < 4; i++) {
        datos[i].D = NULL;
        datos[i].copia_datos = NULL;
        datos[i].N = 0;
        datos[i].k = 0;
        datos[i].usar_copia = FALSE;
        datos[i].color_1 = (GdkRGBA){0, 0, 0, 1};
        datos[i].color_2 = (GdkRGBA){0, 0, 0, 1};
        datos[i].iterations = 0;
        datos[i].swaps = 0;
        datos[i].algoritmo[0] = '\0';
        datos[i].area = NULL;
    }

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
    // El círculo 1
    area_circulo1 = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo1"));
    g_signal_connect(area_circulo1, "draw", G_CALLBACK(dibujar_area), &datos[0]);
    datos[0].area = area_circulo1;
    // El círculo 2
    area_circulo2 = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo2"));
    g_signal_connect(area_circulo2, "draw", G_CALLBACK(dibujar_area), &datos[1]);
    datos[1].area = area_circulo2;
    // El círculo 3
    area_circulo3 = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo3"));
    g_signal_connect(area_circulo3, "draw", G_CALLBACK(dibujar_area), &datos[2]);
    datos[2].area = area_circulo3;
    // El círculo 4
    area_circulo4 = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo4"));
    g_signal_connect(area_circulo4, "draw", G_CALLBACK(dibujar_area), &datos[3]);
    datos[3].area = area_circulo4;
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
    for (int i = 0; i < 4; i++) {
        free(general->datos[i].D);
        free(general->datos[i].copia_datos);
    }
    free(general->datos);
    free(general);
    g_object_unref(builder);

    return 0;
}