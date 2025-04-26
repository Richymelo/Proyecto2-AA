/*
            En esta sección se tienen los algoritmos de ordenamiento que el
            usuario puede escoger.
            Hay 10 tipos de algoritmos de ordenamiento, todos con secciones
            implementadas para que se muestre su paso a paso en el área
            de dibujo.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Para que el compilador sepa que existen estas funciones
gboolean bubble_sort_step(SortState *state);
gboolean cocktail_sort_step(SortState *state);
gboolean exchange_sort_step(SortState *state);
gboolean selection_sort_step(SortState *state);
gboolean insertion_sort_step(SortState *state);
gboolean merge_sort_step(SortState *state);
gboolean quick_sort_step(SortState *state);
gboolean shell_sort_step(SortState *state);
gboolean gnome_sort_step(SortState *state);
gboolean pancake_sort_step(SortState *state);
const char* sort_type_to_string(SortType type);


// Función de intercambiar dos valores
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
void delay(){
    //g_usleep(1);  // Delay para ver cambios
}
// Pedirle a GTK que vuelva a dibujar el círculo
gboolean safe_redraw(gpointer data) {
    GtkWidget *widget = GTK_WIDGET(data);
    if (widget != NULL) {
        gtk_widget_queue_draw(widget);
    }
    return FALSE;  // Se corre solo una vez
}
// Hacer el sort paso a paso para que se pueda mostrar en pantalla
gboolean sort_step(gpointer user_data) {
    SortState *state = (SortState *)user_data;

    if (state == NULL || state->datos == NULL) {
        return FALSE;
    }
    // Bandera para saber si ya se terminó
    gboolean still_sorting = FALSE;
    // Usar el sort escogido
    switch (state->current_algorithm) {
        case SORT_BUBBLE:
            still_sorting = bubble_sort_step(state);
            break;
        case SORT_COCKTAIL:
            still_sorting = cocktail_sort_step(state);
            break;
        case SORT_EXCHANGE:
            still_sorting = exchange_sort_step(state);
            break;
        case SORT_SELECTION:
            still_sorting = selection_sort_step(state);
            break;
        case SORT_INSERTION:
            still_sorting = insertion_sort_step(state);
            break;
        case SORT_MERGE:
            still_sorting = merge_sort_step(state);
            break;
        case SORT_QUICK:
            still_sorting = quick_sort_step(state);
            break;
        case SORT_SHELL:
            still_sorting = shell_sort_step(state);
            break;
        case SORT_GNOME:
            still_sorting = gnome_sort_step(state);
            break;
        case SORT_PANCAKE:
            still_sorting = pancake_sort_step(state);
            break;
    }
    // Se termina el sort
    if (!still_sorting) {
        // Dibujar en la pantalla
        g_idle_add(safe_redraw, state->datos->area);

        // Se menciona el sort que acaba de terminar
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "¡%s terminado!", sort_type_to_string(state->current_algorithm));

        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        // Se libera la memoria
        if (state->temp) free(state->temp);
        if (state->stack) free(state->stack);
        free(state);

        return FALSE; // No hay nada más que hacer
    }

    return TRUE; // Se continua con el sort
}
// Para escribir en la caja de dialogo de cuando termina un sort
const char* sort_type_to_string(SortType type) {
    switch (type) {
        case SORT_BUBBLE: return "Bubble Sort";
        case SORT_COCKTAIL: return "Cocktail Sort";
        case SORT_EXCHANGE: return "Exchange Sort";
        case SORT_SELECTION: return "Selection Sort";
        case SORT_INSERTION: return "Insertion Sort";
        case SORT_MERGE: return "Merge Sort";
        case SORT_QUICK: return "Quick Sort";
        case SORT_SHELL: return "Shell Sort";
        case SORT_GNOME: return "Gnome Sort";
        case SORT_PANCAKE: return "Pancake Sort";
        default: return "Unknown Sort";
    }
}
// - - - - -
// BUBBLE SORT
// Iteraciones es total de los dos for loops. Iteraciones = (n-1) + (n-1)*n/2
// Intercambios es el if, que es una probabilidad.
// Intercambia un par y se mueve al siguiente.
// - - - - -
gboolean bubble_sort_step(SortState *state) {
    // Para usar el arreglo adecuado
    int *arr = state->datos->usar_copia ? state->datos->copia_datos : state->datos->D;
    // Cantidad de pasos que se han hecho
    int steps = 0;
    // Cantidad de pasos necesarios antes de refrescar la pantalla
    int steps_per_frame = 1;
    // Por si no hay nada en el arreglo
    if (arr == NULL) return FALSE;
    // Mientras la cantidad de pasos sea menor que las necesarias para refrescar
    // la pantalla
    while (steps < steps_per_frame) {
        if (state->i >= state->n - 1) {
            return FALSE; // Ya terminó
        }
        // No ha terminado
        if (state->j < state->n - state->i - 1) {
            // Incrementar contador de iteraciones
            state->datos->iterations++;
            if (arr[state->j] < arr[state->j + 1]) {
                swap(&arr[state->j], &arr[state->j + 1]);
                // Incrementar contador de intercambios
                state->datos->swaps++;
            }
            // i y j son los loops
            state->j++;
        } else {
            state->j = 0;
            state->i++;
        }
        // Se ha llegado al final de un paso
        steps++;
    }
    // Se vuelve a dibujar la pantalla
    g_idle_add(safe_redraw, state->datos->area);
    delay(); // Por si se quiere apreciar más los swaps
    return TRUE; // Se continua con el sort
}
// - - - - -
// COCKTAIL SORT
// Iteraciones es total de los dos for loops. Iteraciones = (n-1) + (n-1)*n/2
// Intercambios es el if, que es una probabilidad.
// Como el bubble sort pero por delante y por detrás.
// - - - - -
gboolean cocktail_sort_step(SortState *state) {
    // Para usar el arreglo adecuado
    int *arr = state->datos->usar_copia ? state->datos->copia_datos : state->datos->D;
    // Cantidad de pasos que se han hecho
    int steps = 0;
    // Cantidad de pasos necesarios antes de refrescar la pantalla
    int steps_per_frame = 1;
    // Por si no hay nada en el arreglo
    if (arr == NULL) return FALSE;
    // Mientras la cantidad de pasos sea menor que las necesarias para refrescar
    // la pantalla
    while (steps < steps_per_frame) {
        if (state->start >= state->end) {
            return FALSE; // Ya terminó
        }
        // No ha terminado
        if (state->phase == 0) { // Se mueve a través del arreglo hacia adelante
            // Si no se ha llegado al final
            if (state->i < state->end) {
                // Se incrementa contador de iteraciones
                state->datos->iterations++;
                if (arr[state->i] < arr[state->i + 1]) {
                    swap(&arr[state->i], &arr[state->i + 1]);
                    // Se incrementa contador de intercambios
                    state->datos->swaps++;
                }
                state->i++;
            } else {
                state->end--;
                state->i = state->end - 1;
                state->phase = 1; // Ahora se mueve hacia atrás en el arreglo
            }
        } else { // Se va moviendo desde el final al inicio
            if (state->i >= state->start) {
                // Se incrementa el contador de iteraciones
                state->datos->iterations++;
                if (arr[state->i] < arr[state->i + 1]) {
                    swap(&arr[state->i], &arr[state->i + 1]);
                    // Se incrementa el contador de swaps
                    state->datos->swaps++;
                }
                state->i--;
            } else {
                state->start++;
                state->i = state->start;
                state->phase = 0; // Se vuelve a empezar desde el inicio
            }
        }
        // Se ha llegado al final de un paso
        steps++;
    }
    // Se vuelve a dibujar la pantalla
    g_idle_add(safe_redraw, state->datos->area);
    delay(); // Por si se quiere apreciar más los swaps
    return TRUE; // Se continua con el sort
}
// - - - - -
// EXCHANGE SORT
// Iteraciones es total de los dos for loops. Iteraciones = (n-1) + (n-1)*n/2
// Intercambios es el if, que es una probabilidad.
// Se prueban todos con el primer elemento y se van intercambiando.
// - - - - -
gboolean exchange_sort_step(SortState *state) {
    // Para usar el arreglo adecuado
    int *arr = state->datos->usar_copia ? state->datos->copia_datos : state->datos->D;
    // Cantidad de pasos que se han hecho
    int steps = 0;
    // Cantidad de pasos necesarios antes de refrescar la pantalla
    int steps_per_frame = 1;
    // Por si no hay nada en el arreglo
    if (arr == NULL) return FALSE;
    // Mientras la cantidad de pasos sea menor que las necesarias para refrescar
    // la pantalla
    while (steps < steps_per_frame) {
        if (state->i >= state->n - 1) {
            return FALSE; // Ya terminó
        }
        // No ha terminado
        if (state->j < state->n) {
            // se incrementa el contador de iteraciones
            state->datos->iterations++;
            if (arr[state->i] < arr[state->j]) {
                swap(&arr[state->i], &arr[state->j]);
                // Se incrementa el contador de intercambios
                state->datos->swaps++;
            }
            state->j++;
        } else {
            state->i++;
            state->j = state->i + 1;
        }
        // Se ha llegado al final de un paso
        steps++;
    }
    // Se vuelve a dibujar la pantalla
    g_idle_add(safe_redraw, state->datos->area);
    delay(); // Por si se quiere apreciar más los swaps
    return TRUE; // Se continua con el sort
}
// - - - - -
// SELECTION SORT
// Iteraciones es total de los dos for loops. Iteraciones = (n-1) + (n-1)*n/2
// Intercambios es (n-1), ya que se hace solo un intercambio por número.
// Se busca el mayor número de todos y se intercambia.
// - - - - -
gboolean selection_sort_step(SortState *state) {
    // Para usar el arreglo adecuado
    int *arr = state->datos->usar_copia ? state->datos->copia_datos : state->datos->D;
    // Cantidad de pasos que se han hecho
    int steps = 0;
    // Cantidad de pasos necesarios antes de refrescar la pantalla
    int steps_per_frame = 1;
    // Por si no hay nada en el arreglo
    if (arr == NULL) return FALSE;
    // Mientras la cantidad de pasos sea menor que las necesarias para refrescar
    // la pantalla
    while (steps < steps_per_frame) {
        if (state->i >= state->n - 1) {
            return FALSE; // Ya terminó
        }
        // No ha terminado
        if (state->j < state->n) {
            // Se incrementa el contador de iteraciones
            state->datos->iterations++;
            if (arr[state->j] > arr[state->max_index]) {
                // Se busca el número más grande
                state->max_index = state->j;
            }
            state->j++;
        } else {
            if (state->i != state->max_index) {
                swap(&arr[state->i], &arr[state->max_index]);
                // Se incrementa el contador de swaps
                state->datos->swaps++;
            }

            state->i++;
            state->j = state->i + 1;
            state->max_index = state->i;
        }
        // Se ha llegado al final de un paso
        steps++;
    }
    // Se vuelve a dibujar la pantalla
    g_idle_add(safe_redraw, state->datos->area);
    delay(); // Por si se quiere apreciar más los swaps
    return TRUE; // Se continua con el sort
}
// - - - - -
// INSERTION SORT
// Iteraciones es total del for loop y el while. Iteraciones = (n-1) + intercambios.
// Intercambios es el while, que es una probabilidad.
// Se va ordenando el arreglo conforme se va recorriendo.
// - - - - -
gboolean insertion_sort_step(SortState *state) {
    // Para usar el arreglo adecuado
    int *arr = state->datos->usar_copia ? state->datos->copia_datos : state->datos->D;
    // Cantidad de pasos que se han hecho
    int steps = 0;
    // Cantidad de pasos necesarios antes de refrescar la pantalla
    int steps_per_frame = 1;
    // Por si no hay nada en el arreglo
    if (arr == NULL) return FALSE;
    // Mientras la cantidad de pasos sea menor que las necesarias para refrescar
    // la pantalla
    while (steps < steps_per_frame) {
        if (state->i >= state->n) {
            return FALSE; // Ya terminó
        }
        // No ha terminado
        if (state->phase == 0) {
            // Empezar inserción
            state->max_index = arr[state->i];
            state->j = state->i - 1;
            state->phase = 1;
        } else if (state->phase == 1) {
            // Se mueve para atrás buscando el lugar correcto del número
            if (state->j >= 0 && arr[state->j] < state->max_index) {
                arr[state->j + 1] = arr[state->j];
                // Se incrementa contador de iteraciones
                state->datos->iterations++;
                // Se incrementa contador de intercambios
                state->datos->swaps++;
                state->j--;
            } else {
                arr[state->j + 1] = state->max_index;
                state->i++;
                state->phase = 0;
            }
        }
        // Se ha llegado al final de un paso
        steps++;
    }
    // Se vuelve a dibujar la pantalla
    g_idle_add(safe_redraw, state->datos->area);
    delay(); // Por si se quiere apreciar más los swaps
    return TRUE; // Se continua con el sort
}
// - - - - -
// MERGE SORT
// Iteraciones es total de los for loops y whiles.
// Total = for loops para copiar datos + whiles para combinar los arreglos
// Intercambios es el if, que es una probabilidad.
// Se va dividiendo el arreglo en mitades recursivamente.
// - - - - -
gboolean merge_sort_step(SortState *state) {
    // Para usar el arreglo adecuado
    int *arr = state->datos->usar_copia ? state->datos->copia_datos : state->datos->D;
    // Cantidad de pasos que se han hecho
    int steps = 0;
    // Cantidad de pasos necesarios antes de refrescar la pantalla
    int steps_per_frame = 1;
    // Por si no hay nada en el arreglo
    if (arr == NULL) return FALSE;
    // Se inicializa el arreglo temporal
    if (state->temp == NULL) {
        state->temp = malloc(sizeof(int) * state->n);
        if (!state->temp) {
            printf("Error creando memoria para el arreglo temp del merge sort\n");
            return FALSE;
        }
        // Condición piso
        state->gap = 1;
    }
    // Mientras la cantidad de pasos sea menor que las necesarias para refrescar
    // la pantalla
    while (steps < steps_per_frame) {
        if (state->gap >= state->n) {
            // Se terminó el merge
            return FALSE;
        }

        if (state->low >= state->n - 1) {
            // Una vez que se terminaron de combinar los arreglos, seguir a los
            // siguientes más grandes
            state->gap *= 2;
            state->low = 0;
        } else {
            // Buscar el medio y el punto más alto
            int mid = MIN(state->low + state->gap - 1, state->n - 1);
            int high = MIN(state->low + 2 * state->gap - 1, state->n - 1);

            // Hacer el merge en temp
            int i = state->low, j = mid + 1, k = state->low;
            while (i <= mid && j <= high) {
                // Incrementar el contador de iteraciones
                state->datos->iterations++;
                if (arr[i] >= arr[j]) {
                    state->temp[k++] = arr[i++];
                } else {
                    state->temp[k++] = arr[j++];
                }
            }
            // Si se terminó una parte, copiar el resto
            while (i <= mid) {
                state->datos->iterations++;
                state->temp[k++] = arr[i++];
            }
            while (j <= high) {
                state->datos->iterations++;
                state->temp[k++] = arr[j++];
            }

            // Copiar otra vez en el arreglo
            for (i = state->low; i <= high; i++) {
                arr[i] = state->temp[i];
                state->datos->swaps++;
                // Se vuelve a dibujar la pantalla
                g_idle_add(safe_redraw, state->datos->area);
                delay(); // Por si se quiere apreciar más los swaps
            }
            //Pasar al siguiente arreglo para hacer el merge
            state->low += 2 * state->gap;
        }
        // Se ha llegado al final de un paso
        steps++;
    }
    // Se vuelve a dibujar la pantalla
    g_idle_add(safe_redraw, state->datos->area);
    return TRUE; // Se continua el sort
}
// - - - - -
// QUICK SORT
// Iteraciones es total de los for loops y whiles.
// Intercambios es el if, que es una probabilidad.
// Se escoge un pivote y se va dividiendo el arreglo recursivamente con base
// en esto.
// - - - - -
gboolean quick_sort_step(SortState *state) {
    // Para usar el arreglo adecuado
    int *arr = state->datos->usar_copia ? state->datos->copia_datos : state->datos->D;
    // Cantidad de pasos que se han hecho
    int steps = 0;
    // Cantidad de pasos necesarios antes de refrescar la pantalla
    int steps_per_frame = 1;
    // Por si no hay nada en el arreglo
    if (arr == NULL) return FALSE;
    // Se inicializa el stack
    if (state->stack == NULL) {
        // Parejas de números que son los tamaños del arreglo
        state->stack = malloc(sizeof(int) * 2 * state->n);
        if (!state->stack) {
            printf("Error creando memoria para el stack del quicksort\n");
            return FALSE;
        }
        // Tener vacío el stack
        state->top = -1;

        // Push del arreglo al stack
        state->stack[++state->top] = 0;       // primera posición
        state->stack[++state->top] = state->n - 1; // última posición
    }
    // Mientras la cantidad de pasos sea menor que las necesarias para refrescar
    // la pantalla
    while (steps < steps_per_frame) {
        if (state->top < 0) {
            return FALSE; // Stack está vacía entonces se terminó el sort
        }

        // Obtener el par de números que son los extremos del arreglo
        int high = state->stack[state->top--];
        int low = state->stack[state->top--];
        // La partición de los elementos
        if (low < high) {
            // Se escoge el primer elemento como el pivote
            int pivot = arr[low];
            // Elementos mayores que el pivote
            int i = low;
            // Separar elementos como mayores y menores que el pivote
            for (int j = low + 1; j <= high; j++) {
                state->datos->iterations++;
                if (arr[j] > pivot) {
                    i++;
                    swap(&arr[i], &arr[j]);
                    // Incrementar el contador de intercambios
                    state->datos->swaps++;
                }
            }
            // Poner el pivote en el lugar correcto
            swap(&arr[low], &arr[i]);
            // Incrementar el contador de intercambios
            state->datos->swaps++;

            // Se ingresan los arreglos más pequeños en el stack
            state->stack[++state->top] = i + 1;
            state->stack[++state->top] = high;

            state->stack[++state->top] = low;
            state->stack[++state->top] = i - 1;
        }
        // Se ha llegado al final de un paso
        steps++;
    }
    // Se vuelve a dibujar la pantalla
    g_idle_add(safe_redraw, state->datos->area);
    delay(); // Por si se quiere apreciar más los swaps
    return TRUE; // Se continua el sort
}
// - - - - -
// SHELL SORT
// Iteraciones es total de los for loops.
// Intercambios están en el tercer for loop.
// Muy parecido al insertion sort solo que
// - - - - -
gboolean shell_sort_step(SortState *state) {
    // Para usar el arreglo adecuado
    int *arr = state->datos->usar_copia ? state->datos->copia_datos : state->datos->D;
    // Cantidad de pasos que se han hecho
    int steps = 0;
    // Cantidad de pasos necesarios antes de refrescar la pantalla
    int steps_per_frame = 1;
    // Por si no hay nada en el arreglo
    if (arr == NULL) return FALSE;
    // Mientras la cantidad de pasos sea menor que las necesarias para refrescar
    // la pantalla
    while (steps < steps_per_frame) {
        if (state->gap == 0) {
            return FALSE; // Ya terminó
        }
        // No ha terminado
        if (state->i < state->n) {
            // Se tiene el número que se quiere colocar
            int temp = arr[state->i];
            int j = state->i;
            // Ir moviendo los elementos que se encuentran que son mayores
            while (j >= state->gap && arr[j - state->gap] < temp) {
                arr[j] = arr[j - state->gap];
                j -= state->gap;
                // Se incrementa el contador de iteraciones
                state->datos->iterations++;
                // Se incrementa el contador de intercambios
                state->datos->swaps++;
            }
            // Se coloca el valor en el espacio correcto
            arr[j] = temp;
            // Se busca el siguiente elemento
            state->i++;
        } else {
            // Se decrementa el espacio que se toma
            state->gap /= 2;
            state->i = state->gap;
        }
        // Se ha llegado al final de un paso
        steps++;
    }
    // Se vuelve a dibujar la pantalla
    g_idle_add(safe_redraw, state->datos->area);
    delay(); // Por si se quiere apreciar más los swaps
    return TRUE; // Se continua el sort
}
// - - - - -
// GNOME SORT
// Iteraciones es el loop del while.
// Intercambios son un if, entonces es una probabilidad.
// Cuando hay un intercambio se devuelve a la posición anterior.
// - - - - -
gboolean gnome_sort_step(SortState *state) {
    // Para usar el arreglo adecuado
    int *arr = state->datos->usar_copia ? state->datos->copia_datos : state->datos->D;
    // Cantidad de pasos que se han hecho
    int steps = 0;
    // Cantidad de pasos necesarios antes de refrescar la pantalla
    int steps_per_frame = 1;
    // Por si no hay nada en el arreglo
    if (arr == NULL) return FALSE;
    // Mientras la cantidad de pasos sea menor que las necesarias para refrescar
    // la pantalla
    while (steps < steps_per_frame) {
        if (state->index >= state->n) {
            return FALSE; // Ya terminó
        }
        // No ha terminado
        if (state->index == 0) {
            state->index++;
        } else if (arr[state->index] <= arr[state->index - 1]) {
            state->index++;
        } else {
            swap(&arr[state->index], &arr[state->index - 1]);
            // Se incrementa el contador de intercambios
            state->datos->swaps++;
            // Se incrementa el contador de iteraciones
            state->datos->iterations++;
            state->index--;
        }
        // Se ha llegado al final de un paso
        steps++;
    }
    // Se vuelve a dibujar la pantalla
    g_idle_add(safe_redraw, state->datos->area);
    delay(); // Por si se quiere apreciar más los swaps
    return TRUE; // Se continua el sort
}
// - - - - -
// PANCAKE SORT
// Iteraciones son los for loops y el loop del while.
// Intercambios están en la función flip.
// - - - - -
// Se le da la vuelta al arreglo para que el digito que va primero quede de último 
void flip_array(int *arr, int idx, DatosUsuario *datos, SortState *state) {
    int start = datos->k - 1;
    int temp;
    while (start > idx) {
        temp = arr[idx];
        arr[idx] = arr[start];
        arr[start] = temp;
        start--;
        idx++;
        // Se incrementa el contador de iteraciones
        datos->iterations++;
        // Se incrementa el contador de intercambios
        datos->swaps++;
        // Se vuelve a dibujar la pantalla
        g_idle_add(safe_redraw, state->datos->area);
        delay(); // Por si se quiere apreciar más los swaps
    }
}
gboolean pancake_sort_step(SortState *state) {
    // Para usar el arreglo adecuado
    int *arr = state->datos->usar_copia ? state->datos->copia_datos : state->datos->D;
    // Cantidad de pasos que se han hecho
    int steps = 0;
    // Cantidad de pasos necesarios antes de refrescar la pantalla
    int steps_per_frame = 1;
    // Por si no hay nada en el arreglo
    if (arr == NULL) return FALSE;
    // Mientras la cantidad de pasos sea menor que las necesarias para refrescar
    // la pantalla
    while (steps < steps_per_frame) {
        if (state->i >= state->n) {
            return FALSE; // Ya terminó
        }
        // No ha terminado
        if (state->phase == 0) {
            // Encontrar el número más grande
            state->max_index = state->i;
            for (int j = state->i; j < state->n; j++) {
                // Incrementar el contador de iteraciones
                state->datos->iterations++;
                if (arr[j] > arr[state->max_index]) {
                    state->max_index = j;
                }
            }
            state->phase = 1;
        } else if (state->phase == 1) {
            // Llevar el número al final y después al inicio
            if (state->max_index != state->i) {
                flip_array(arr, state->max_index, state->datos, state);
                flip_array(arr, state->i, state->datos, state);
            }
            // Se sigue con el siguiente valor
            state->i++;
            state->phase = 0;
        }
        // Se ha llegado al final de un paso
        steps++;
    }
    // Se vuelve a dibujar la pantalla
    g_idle_add(safe_redraw, state->datos->area);
    delay(); // Por si se quiere apreciar más los swaps
    return TRUE; // Se continua el sort
}