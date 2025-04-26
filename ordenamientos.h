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

// --- Sort Step Function Prototypes ---
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

// --- Helper Function Prototype ---
const char* sort_type_to_string(SortType type);


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

gboolean safe_redraw(gpointer data) {
    GtkWidget *widget = GTK_WIDGET(data);
    if (widget != NULL) {
        gtk_widget_queue_draw(widget);
    }
    return FALSE;  // only run once
}
gboolean sort_step(gpointer user_data) {
    SortState *state = (SortState *)user_data;

    if (state == NULL || state->datos == NULL) {
        return FALSE;
    }

    gboolean still_sorting = FALSE;

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

    if (!still_sorting) {
        // Finished sorting!

        g_idle_add(safe_redraw, state->datos->area);

        // Optional: show a GTK message
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "¡%s terminado!", sort_type_to_string(state->current_algorithm));

        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        // Free memory
        if (state->temp) free(state->temp);
        if (state->stack) free(state->stack);
        free(state);

        return FALSE; // No more steps needed
    }

    return TRUE; // Continue sorting
}
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
// - - - - -
gboolean bubble_sort_step(SortState *state) {
    int *arr = state->datos->usar_copia ? state->datos->copia_datos : state->datos->D;
    int steps = 0;
    int steps_per_frame = 1;

    if (arr == NULL) return FALSE;

    while (steps < steps_per_frame) {
        if (state->i >= state->n - 1) {
            return FALSE; // finished
        }

        if (state->j < state->n - state->i - 1) {
            state->datos->iterations++;
            if (arr[state->j] < arr[state->j + 1]) {
                swap(&arr[state->j], &arr[state->j + 1]);
                state->datos->swaps++;
            }
            state->j++;
        } else {
            state->j = 0;
            state->i++;
        }

        steps++;
    }

    g_idle_add(safe_redraw, state->datos->area);
    return TRUE; // continue sorting
}
// - - - - -
// COCKTAIL SORT
// Iteraciones es total de los dos for loops. Iteraciones = (n-1) + (n-1)*n/2
// Intercambios es el if, que es una probabilidad.
// - - - - -
gboolean cocktail_sort_step(SortState *state) {
    int *arr = state->datos->usar_copia ? state->datos->copia_datos : state->datos->D;
    int steps = 0;
    int steps_per_frame = 1;

    if (arr == NULL) return FALSE;

    while (steps < steps_per_frame) {
        if (state->start >= state->end) {
            return FALSE; // finished
        }

        if (state->phase == 0) { // Forward phase
            if (state->i < state->end) {
                state->datos->iterations++;
                if (arr[state->i] < arr[state->i + 1]) {
                    swap(&arr[state->i], &arr[state->i + 1]);
                    state->datos->swaps++;
                }
                state->i++;
            } else {
                state->end--;
                state->i = state->end - 1;
                state->phase = 1; // switch to backward
            }
        } else { // Backward phase
            if (state->i >= state->start) {
                state->datos->iterations++;
                if (arr[state->i] < arr[state->i + 1]) {
                    swap(&arr[state->i], &arr[state->i + 1]);
                    state->datos->swaps++;
                }
                state->i--;
            } else {
                state->start++;
                state->i = state->start;
                state->phase = 0; // switch to forward
            }
        }

        steps++;
    }

    g_idle_add(safe_redraw, state->datos->area);
    return TRUE; // continue sorting
}
// - - - - -
// EXCHANGE SORT
// Iteraciones es total de los dos for loops. Iteraciones = (n-1) + (n-1)*n/2
// Intercambios es el if, que es una probabilidad.
// - - - - -
gboolean exchange_sort_step(SortState *state) {
    int *arr = state->datos->usar_copia ? state->datos->copia_datos : state->datos->D;
    int steps = 0;
    int steps_per_frame = 1;

    if (arr == NULL) return FALSE;

    while (steps < steps_per_frame) {
        if (state->i >= state->n - 1) {
            return FALSE; // finished
        }

        if (state->j < state->n) {
            state->datos->iterations++;
            if (arr[state->i] < arr[state->j]) {
                swap(&arr[state->i], &arr[state->j]);
                state->datos->swaps++;
            }
            state->j++;
        } else {
            state->i++;
            state->j = state->i + 1;
        }

        steps++;
    }

    g_idle_add(safe_redraw, state->datos->area);
    return TRUE; // continue sorting
}
// - - - - -
// SELECTION SORT
// Iteraciones es total de los dos for loops. Iteraciones = (n-1) + (n-1)*n/2
// Intercambios es (n-1), ya que se hace solo un intercambio por número.
// - - - - -
gboolean selection_sort_step(SortState *state) {
    int *arr = state->datos->usar_copia ? state->datos->copia_datos : state->datos->D;
    int steps = 0;
    int steps_per_frame = 1;

    if (arr == NULL) return FALSE;

    while (steps < steps_per_frame) {
        if (state->i >= state->n - 1) {
            return FALSE; // finished
        }

        if (state->j < state->n) {
            state->datos->iterations++;
            if (arr[state->j] > arr[state->max_index]) { // descending order
                state->max_index = state->j;
            }
            state->j++;
        } else {
            swap(&arr[state->i], &arr[state->max_index]);
            state->datos->swaps++;

            state->i++;
            state->j = state->i + 1;
            state->max_index = state->i;
        }

        steps++;
    }

    g_idle_add(safe_redraw, state->datos->area);
    return TRUE; // continue sorting
}
// - - - - -
// INSERTION SORT
// Iteraciones es total del for loop y el while. Iteraciones = (n-1) + intercambios.
// Intercambios es el while, que es una probabilidad.
// - - - - -
gboolean insertion_sort_step(SortState *state) {
    int *arr = state->datos->usar_copia ? state->datos->copia_datos : state->datos->D;
    int steps = 0;
    int steps_per_frame = 1;

    if (arr == NULL) return FALSE;

    while (steps < steps_per_frame) {
        if (state->i >= state->n) {
            return FALSE; // finished
        }

        if (state->phase == 0) {
            // Start insertion
            state->max_index = arr[state->i];
            state->j = state->i - 1;
            state->phase = 1;
        } else if (state->phase == 1) {
            // Move backwards
            if (state->j >= 0 && arr[state->j] < state->max_index) {
                arr[state->j + 1] = arr[state->j];
                state->datos->iterations++;
                state->datos->swaps++;
                state->j--;
            } else {
                arr[state->j + 1] = state->max_index;
                state->i++;
                state->phase = 0;
            }
        }

        steps++;
    }

    g_idle_add(safe_redraw, state->datos->area);
    return TRUE; // continue sorting
}
// - - - - -
// MERGE SORT
// Iteraciones es total de los for loops y whiles.
// Total = for loops para copiar datos + whiles para combinar los arreglos
// Intercambios es el if, que es una probabilidad.
// - - - - -
gboolean merge_sort_step(SortState *state) {
    int *arr = state->datos->usar_copia ? state->datos->copia_datos : state->datos->D;
    int steps = 0;
    int steps_per_frame = 300;

    if (arr == NULL) return FALSE;

    // Initialize temp array if not yet done
    if (state->temp == NULL) {
        state->temp = malloc(sizeof(int) * state->n);
        if (!state->temp) {
            printf("Error allocating temp array for merge sort\n");
            return FALSE;
        }
        // Start merging subarrays of size 1
        state->gap = 1;
    }

    while (steps < steps_per_frame) {
        if (state->gap >= state->n) {
            // Finished merging all
            return FALSE;
        }

        if (state->low >= state->n - 1) {
            // Finished one full pass for current gap size
            state->gap *= 2;
            state->low = 0;
        } else {
            int mid = MIN(state->low + state->gap - 1, state->n - 1);
            int high = MIN(state->low + 2 * state->gap - 1, state->n - 1);

            // Merge arr[low..mid] and arr[mid+1..high] into temp
            int i = state->low, j = mid + 1, k = state->low;
            while (i <= mid && j <= high) {
                state->datos->iterations++;
                if (arr[i] >= arr[j]) {
                    state->temp[k++] = arr[i++];
                } else {
                    state->temp[k++] = arr[j++];
                }
            }
            while (i <= mid) {
                state->datos->iterations++;
                state->temp[k++] = arr[i++];
            }
            while (j <= high) {
                state->datos->iterations++;
                state->temp[k++] = arr[j++];
            }

            // Copy back into arr
            for (i = state->low; i <= high; i++) {
                arr[i] = state->temp[i];
                state->datos->swaps++;
            }
            g_idle_add(safe_redraw, state->datos->area);

            state->low += 2 * state->gap;
        }

        steps++;
    }

    g_idle_add(safe_redraw, state->datos->area);
    return TRUE;
}
// - - - - -
// QUICK SORT
// Iteraciones es total de los for loops y whiles.
// Intercambios es el if, que es una probabilidad.
// - - - - -
gboolean quick_sort_step(SortState *state) {
    int *arr = state->datos->usar_copia ? state->datos->copia_datos : state->datos->D;
    int steps = 0;
    int steps_per_frame = 300;

    if (arr == NULL) return FALSE;

    // Initialize stack if needed
    if (state->stack == NULL) {
        state->stack = malloc(sizeof(int) * 2 * state->n); // (low, high) pairs
        if (!state->stack) {
            printf("Error allocating stack for quicksort\n");
            return FALSE;
        }
        state->top = -1;

        // Push full array onto stack
        state->stack[++state->top] = 0;       // low
        state->stack[++state->top] = state->n - 1; // high
    }

    while (steps < steps_per_frame) {
        if (state->top < 0) {
            return FALSE; // Stack empty => finished
        }

        // Pop high and low
        int high = state->stack[state->top--];
        int low = state->stack[state->top--];

        if (low < high) {
            int pivot = arr[low];
            int i = low;

            for (int j = low + 1; j <= high; j++) {
                state->datos->iterations++;
                if (arr[j] > pivot) { // Descending
                    i++;
                    swap(&arr[i], &arr[j]);
                    state->datos->swaps++;
                }
            }
            swap(&arr[low], &arr[i]);
            state->datos->swaps++;

            // Push right subarray
            state->stack[++state->top] = i + 1;
            state->stack[++state->top] = high;

            // Push left subarray
            state->stack[++state->top] = low;
            state->stack[++state->top] = i - 1;
        }

        steps++;
    }

    g_idle_add(safe_redraw, state->datos->area);
    return TRUE;
}

// - - - - -
// SHELL SORT
// Iteraciones es total de los for loops.
// Intercambios están en el tercer for loop.
// - - - - -
gboolean shell_sort_step(SortState *state) {
    int *arr = state->datos->usar_copia ? state->datos->copia_datos : state->datos->D;
    int steps = 0;
    int steps_per_frame = 1;

    if (arr == NULL) return FALSE;

    while (steps < steps_per_frame) {
        if (state->gap == 0) {
            return FALSE; // finished
        }

        if (state->i < state->n) {
            int temp = arr[state->i];
            int j = state->i;

            while (j >= state->gap && arr[j - state->gap] < temp) { // descending
                arr[j] = arr[j - state->gap];
                j -= state->gap;
                state->datos->iterations++;
                state->datos->swaps++;
            }
            arr[j] = temp;

            state->i++;
        } else {
            state->gap /= 2;
            state->i = state->gap;
        }

        steps++;
    }

    g_idle_add(safe_redraw, state->datos->area);
    return TRUE; // continue sorting
}
// - - - - -
// GNOME SORT
// Iteraciones es el loop del while.
// Intercambios son un if, entonces es una probabilidad.
// - - - - -
gboolean gnome_sort_step(SortState *state) {
    int *arr = state->datos->usar_copia ? state->datos->copia_datos : state->datos->D;
    int steps = 0;
    int steps_per_frame = 1;

    if (arr == NULL) return FALSE;

    while (steps < steps_per_frame) {
        if (state->index >= state->n) {
            return FALSE; // finished
        }

        if (state->index == 0) {
            state->index++;
        } else if (arr[state->index] <= arr[state->index - 1]) {
            state->index++;
        } else {
            swap(&arr[state->index], &arr[state->index - 1]);
            state->datos->swaps++;
            state->datos->iterations++;
            state->index--;
        }

        steps++;
    }

    g_idle_add(safe_redraw, state->datos->area);
    return TRUE; // continue sorting
}
// - - - - -
// PANCAKE SORT
// Iteraciones son los for loops y el loop del while.
// Intercambios están en la función flip.
// - - - - -
void flip_array(int *arr, int idx, DatosUsuario *datos) {
    int start = datos->k - 1;
    int temp;
    while (start > idx) {
        temp = arr[idx];
        arr[idx] = arr[start];
        arr[start] = temp;
        start--;
        idx++;
        datos->iterations++;
        datos->swaps++;
    }
}
gboolean pancake_sort_step(SortState *state) {
    int *arr = state->datos->usar_copia ? state->datos->copia_datos : state->datos->D;
    int steps = 0;
    int steps_per_frame = 1;

    if (arr == NULL) return FALSE;

    while (steps < steps_per_frame) {
        if (state->i >= state->n) {
            return FALSE; // finished
        }

        if (state->phase == 0) {
            // Find the max index in the current subarray
            state->max_index = state->i;
            for (int j = state->i; j < state->n; j++) {
                state->datos->iterations++;
                if (arr[j] > arr[state->max_index]) {
                    state->max_index = j;
                }
            }
            state->phase = 1;
        } else if (state->phase == 1) {
            // Bring max to end
            if (state->max_index != state->i) {
                flip_array(arr, state->max_index, state->datos);
                flip_array(arr, state->i, state->datos);
            }
            state->i++;
            state->phase = 0;
        }

        steps++;
    }

    g_idle_add(safe_redraw, state->datos->area);
    return TRUE; // continue sorting
}