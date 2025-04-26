/*
            En esta sección se tienen las estructuras utilizadas en el programa.
            La estructura DatosUsuario guarda todos los datos que el usuario puede
            modificar. La estructura DatosGenerales es una estructura que guarda la
            estructura previamente mencionada y el builder de GTK, para que estos
            dos puedan ser llamados con un solo puntero.
*/

// Estructura que guarda los datos que ingresa el usuario
typedef struct {
    int *D;              // Puntero al vector de datos original
    int *copia_datos;    // Puntero a la copia que se ordenará
    int N;               // Cantidad de rayos
    int k;               // Cantidad de datos
    bool usar_copia;     // Para saber cual vector de datos utilizar
    GdkRGBA color_1;     // Primer color elegido
    GdkRGBA color_2;     // Segundo color elegido
    int iterations;      // Contador para iteraciones
    int swaps;           // Contador para intercambios
    char algoritmo[100]; // Nombre del algoritmo de ordenamiento seleccionado
    GtkWidget *area;     // Área de cada grupo de datos
} DatosUsuario;
// Estructura que guarda los datos del usuario y los del builder
typedef struct {
    GtkBuilder *builder;
    DatosUsuario *datos;
} DatosGenerales;
// Se tienen todos los sorts
typedef enum {
    SORT_BUBBLE,
    SORT_COCKTAIL,
    SORT_EXCHANGE,
    SORT_SELECTION,
    SORT_INSERTION,
    SORT_MERGE,
    SORT_QUICK,
    SORT_SHELL,
    SORT_GNOME,
    SORT_PANCAKE
} SortType;
// Estructura que maneja todos los valores que los sorts puedan ocupar
typedef struct {
    int i, j;
    int n;
    int start, end;
    int low, high;
    int gap;
    int index;
    int max_index;
    int phase;
    int *temp;          // para el merge sort
    int temp_l, temp_r; // para los rangos de los arreglos del merge
    int *stack;         // recursión para el quick sort
    int top;            // puntero al stack del quick sort
    DatosUsuario *datos;
    SortType current_algorithm;
} SortState;
