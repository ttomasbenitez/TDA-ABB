#include "abb.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define ERROR 0
#define NULO NULL

/**
 * POST: Reserva memoria para un abb_t y le asigna el comparador al abb creado. Devuelve
 * un puntero a la dirección de memoria del abb creado. 
 * En caso de que el comparador sea inválido o falle al reservar la memoria, devuelve
 * NULO.
 */
abb_t* abb_crear(abb_comparador comparador){
  if(!comparador) return NULO;

  abb_t* arbol = calloc(1, sizeof(abb_t));
  if(!arbol) return NULO;

  arbol->comparador = comparador;
  return arbol;
}


/**
 * POST: Reserva memoria para un nodo_abb_t y le asigna el elemento recibido. Devuelve
 * un puntero a la dirección de memoria del nodo creado.
 * En caso de fallar al reservar la memoria, devuelve NULO.
 */ 
nodo_abb_t* crear_y_cargar_nodo(void* elemento){
  nodo_abb_t* nuevo_nodo = calloc(1, sizeof(nodo_abb_t));
  if(!nuevo_nodo) return NULO;

  nuevo_nodo->elemento = elemento;
  return nuevo_nodo;
}

/**
 * PRE: Recibe nodo_actual, arbol y elemento validos.
 * POST: Compara el elemento recibido con el elemento de nodo_actual, si la comparación
 * devuelve mayor a cero se creará un nuevo nodo al que se le cargará el elemento recibido 
 * y lo asignará a la derecha del nodo_actual, si la comparación da menor o igual a cero,
 * se asignará el nuevo nodo a la izquierda de nodo_actual.
 */ 
nodo_abb_t* insertar_nuevo_nodo(nodo_abb_t* nodo_actual, abb_t* arbol, void* elemento){
  if(!nodo_actual){
		nodo_abb_t* nodo_nuevo = crear_y_cargar_nodo(elemento);
    (arbol->tamanio)++;
		return nodo_nuevo;
	}

	int comparacion = arbol->comparador(elemento, nodo_actual->elemento);

	if(comparacion > 0){
		nodo_actual->derecha = insertar_nuevo_nodo(nodo_actual->derecha, arbol, elemento);
  }
  else{ 
		nodo_actual->izquierda = insertar_nuevo_nodo(nodo_actual->izquierda, arbol, elemento);
  }

  return nodo_actual;
}



/**
 * POST: Recibe un elemento y lo asigna a un nodo que luego se carga al árbol.
 * Devuelve el árbol en caso de éxito o NULO en caso de error.
 */
abb_t* abb_insertar(abb_t* arbol, void* elemento){
  if(!arbol) return NULO;

  arbol->nodo_raiz = insertar_nuevo_nodo(arbol->nodo_raiz, arbol, elemento);

  return arbol;
}

/**
 * POST: Compara el elemento_buscado con el elemento del nodo_actual, si la comparación
 * resulta mayor a cero, se llama a nodo_actual como nodo_actual->derecha y si resulta
 * menor a cero, nodo_actual se llama como nodo_actual->izquierda.
 * Cuando la comparación da cero, se devuelve el nodo_actual.
 * En caso de error devuelve NULO.   
 */ 
nodo_abb_t* buscar_nodo_segun_elemento(abb_t* arbol, nodo_abb_t* nodo_actual, void* elemento_buscado){
  if(!nodo_actual) return NULO;
  int comparacion = arbol->comparador(elemento_buscado, nodo_actual->elemento);

  if(comparacion == 0){
    return nodo_actual;
  }
  else if(comparacion > 0){
    return buscar_nodo_segun_elemento(arbol, nodo_actual->derecha, elemento_buscado);
  }
  else{
    return buscar_nodo_segun_elemento(arbol, nodo_actual->izquierda, elemento_buscado);
  }
}


/**
 * POST: Compara el elemento_buscado con el elemento del nodo_actual, si la comparación
 * resulta mayor a cero, se llama a nodo_actual como nodo_actual->derecha y si resulta
 * menor a cero, nodo_actual se llama como nodo_actual->izquierda.
 * Cuando la comparación da cero, se devuelve el nodo anterior al nodo_actual.
 * En caso de nodo_actual ser raíz, devuelve NULO.   
 */ 
nodo_abb_t* buscar_nodo_padre(abb_t* arbol, nodo_abb_t* nodo_actual, nodo_abb_t* nodo_padre, void* elemento_buscado){
  int comparacion = arbol->comparador(elemento_buscado, nodo_actual->elemento);

  if(comparacion == 0){
    if(!nodo_padre) return NULO;

    return nodo_padre;
  }
  else if(comparacion > 0){
    return buscar_nodo_padre(arbol, nodo_actual->derecha, nodo_actual, elemento_buscado);
  }
  else{
    return buscar_nodo_padre(arbol, nodo_actual->izquierda, nodo_actual, elemento_buscado);
  }
}

/**
 * POST: Compara el elemento_buscado y el elemento del nodo_padre, devuelve true si la
 * comparación resulta menor a cero, caso contrario devuelve false.
 */ 
bool nodo_a_eliminar_es_hijo_izquierdo(abb_t* arbol, nodo_abb_t* nodo_padre, void* elemento_buscado){
  int comparacion = arbol->comparador(elemento_buscado, nodo_padre->elemento);

  return (comparacion < 0);
}

/**
 * PRE: Recibe el nodo_padre del nodo_a_eliminar.
 * POST: Si nodo_padre no existe, asigna NULO a nodo_raiz.
 * Si nodo_a_eliminar es hijo izquierdo de nodo_padre, asigna NULO  a nodo_padre->izquierda.
 * Y si es hijo derecho asigna NULO a nodo_padre->derecha.
 */ 
void borrar_nodo_hoja(abb_t* arbol, nodo_abb_t* nodo_padre, void* elemento){
  if(!nodo_padre){
    arbol->nodo_raiz = NULO;
  }
  else if(nodo_a_eliminar_es_hijo_izquierdo(arbol, nodo_padre, elemento)){
    nodo_padre->izquierda = NULO;
  }
  else{
    nodo_padre->derecha = NULO;
  }
}

/**
 * PRE: Recibe el nodo_padre del nodo_a_eliminar.
 * POST: Asigna los nodos como correspondan según el caso.
 */ 
void borrar_nodo_con_un_hijo(abb_t* arbol, nodo_abb_t* nodo_padre, nodo_abb_t* nodo_a_eliminar, void* elemento){
  if(!nodo_a_eliminar->izquierda){
    if(!nodo_padre){    
      arbol->nodo_raiz = nodo_a_eliminar->derecha;
    }
    else if(nodo_a_eliminar_es_hijo_izquierdo(arbol, nodo_padre, elemento)){
      nodo_padre->izquierda = nodo_a_eliminar->derecha;
    }
    else{
      nodo_padre->derecha = nodo_a_eliminar->derecha;
    }
  }
  else{
    if(!nodo_padre){        
      arbol->nodo_raiz = nodo_a_eliminar->izquierda;
    }
    else if(nodo_a_eliminar_es_hijo_izquierdo(arbol, nodo_padre, elemento)){
      nodo_padre->izquierda = nodo_a_eliminar->izquierda;
    }
    else{
      nodo_padre->derecha = nodo_a_eliminar->izquierda;
    }
  }
}

/**
 * PRE: Recibe el nodo a la izquierda del nodo_a_eliminar.
 * POST: Devuelve el nodo más a la derecha del nodo recibido.
 */ 
nodo_abb_t* buscar_predecesor_inorden(nodo_abb_t* actual){
  if(!actual->derecha) return actual;
  
  return buscar_predecesor_inorden(actual->derecha);
}

/**
 * PRE: Recibe el nodo_a_eliminar y su nodo_padre.
 * POST: Asigna los nodos como correspondan según el caso.
 */ 
void borrar_nodo_con_dos_hijos(abb_t* arbol, nodo_abb_t* nodo_padre, nodo_abb_t* nodo_a_eliminar, void* elemento){
  
  nodo_abb_t* reemplazante = buscar_predecesor_inorden(nodo_a_eliminar->izquierda);
  nodo_abb_t* padre_reemplazante = buscar_nodo_padre(arbol, arbol->nodo_raiz, NULL, reemplazante->elemento);


  reemplazante->derecha = nodo_a_eliminar->derecha;
  padre_reemplazante->derecha = reemplazante->izquierda;

  if( (padre_reemplazante != nodo_a_eliminar) && (nodo_padre) ){
    reemplazante->izquierda = padre_reemplazante;
  }
  else if(padre_reemplazante != nodo_a_eliminar){
    reemplazante->izquierda = nodo_a_eliminar->izquierda;
  }


  if(!nodo_padre){
    arbol->nodo_raiz = reemplazante;
  }
  else if( (nodo_padre->izquierda) == nodo_a_eliminar ){
    nodo_padre->izquierda = reemplazante;
  }
  else{
    nodo_padre->derecha = reemplazante;
  }
  
}

/**
 * POST: Distingue entre los casos posibles de eliminación y llama al procedimiento
 * correspondiente, luego libera la memoria ocupada por el nodo_a_eliminar y resta 
 * uno al tamanio del árbol.
 * Devuelve el elemento del nodo que se eliminó. En caso de error devuelve NULO.
 */
void* borrar_nodo(abb_t* arbol, void* elemento){
  nodo_abb_t* nodo_a_eliminar = buscar_nodo_segun_elemento(arbol, arbol->nodo_raiz, elemento);
  if(!nodo_a_eliminar) return NULO;

  void* elemento_quitado = nodo_a_eliminar->elemento;
  
  nodo_abb_t* nodo_padre = buscar_nodo_padre(arbol, arbol->nodo_raiz, NULO, elemento);
  

  if( (!nodo_a_eliminar->izquierda) && (!nodo_a_eliminar->derecha) ){
    borrar_nodo_hoja(arbol, nodo_padre, elemento);
  } 
  else if( (!nodo_a_eliminar->izquierda) || (!nodo_a_eliminar->derecha) ){
    borrar_nodo_con_un_hijo(arbol, nodo_padre, nodo_a_eliminar, elemento);
  }
  else{
    borrar_nodo_con_dos_hijos(arbol, nodo_padre, nodo_a_eliminar, elemento);
  }

  free(nodo_a_eliminar);
  (arbol->tamanio)--;

  return elemento_quitado;
}   


/**
 * POST: Se elimina el nodo que contenga un elemento igual al recibido, de acuerdo a que
 * clase de hijo es y se devuelve el elemento del nodo_eliminado.
 * En caso de error devuelve NULO.
 */
void* abb_quitar(abb_t* arbol, void *elemento){
  if( abb_vacio(arbol) || (!elemento) ) return NULO;

  void* elemento_eliminado = borrar_nodo(arbol, elemento);
  if(!elemento_eliminado) return NULO;

  return elemento_eliminado;
}


/**
 * POST: Se busca el nodo que contenga un elemento igual al recibido, y se devuelve su 
 * elemento.
 * En caso de error devuelve NULO.
 */
void* abb_buscar(abb_t* arbol, void* elemento){
  if(abb_vacio(arbol) || (!elemento) ) return NULO;

  nodo_abb_t* nodo_buscado = buscar_nodo_segun_elemento(arbol, arbol->nodo_raiz, elemento);
  if(!nodo_buscado) return NULO;

  return (nodo_buscado->elemento);
}

/**
 * POST: Devuelve true si el árbol no existe o no tiene elementos.
 * Caso contrario devuelve false.
 */
bool abb_vacio(abb_t* arbol){
  return ( (!arbol) || (arbol->tamanio == 0) );
}

/**
 * POST: Devuelve la cantidad de elementos almacenados en el árbol o ERROR si el árbol 
 * no existe.
 */
size_t abb_tamanio(abb_t *arbol){
  if(!arbol) return ERROR;

  return (arbol->tamanio);
}


/**
 * POST: Se recorre el árbol de izquierda a derecha y si existe el destructor se lo
 * invoca en cada elemento. Luego se libera la memoria ocupada por el nodo_actual.
 */ 
void destruir_nodos(nodo_abb_t* nodo_actual, void (*destructor)(void *)){
  if(!nodo_actual){
    return;
  }
 
  destruir_nodos(nodo_actual->izquierda, destructor);
  destruir_nodos(nodo_actual->derecha, destructor);

  if(destructor){
    destructor(nodo_actual->elemento);
  }

  free(nodo_actual);
}


/**
 * POST: Libera toda la memoria ocupada por el árbol.
 */
void abb_destruir(abb_t *arbol){
  if(!arbol){
    printf("No se pudo destruir el árbol\n");
    return;
  }

  destruir_nodos(arbol->nodo_raiz, NULL);
  free(arbol);
}


/**
 * POST: Libera toda la memoria ocupada por el árbol. Llamando a la función destructor
 * en cada elemento.
 */
void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *)){
  if( (!destructor) || (!arbol) ){
    printf("No se pudo destruir el árbol y sus elementos\n");
    return;
  }

  destruir_nodos(arbol->nodo_raiz, destructor);
  free(arbol);
}


/**
 * POST: Agrega un elemento_a_agregar al array y devuelve true.
 * Si no se pudo agregar porque no hay más lugar en el array o ya se agregaron todos los
 * elementos devuelve false.
 */ 
bool agregar_a_array(void* elemento_a_agregar, size_t tamanio_arbol, void** array, size_t tamanio_array, size_t cantidad_elementos){  
  if( (cantidad_elementos >= tamanio_array) || (tamanio_arbol == cantidad_elementos)  ) return false;
  
  array[cantidad_elementos] = elemento_a_agregar;
  return true;
}

/**
 * POST: Recorre el árbol con un recorrido preorden y carga los elementos posibles al array.
 */ 
void recorrer_preorden(size_t tamanio_arbol, nodo_abb_t* nodo_actual, void** array, size_t tamanio_array, size_t* cantidad_elementos){
  if(!nodo_actual) return;

  if(!agregar_a_array(nodo_actual->elemento, tamanio_arbol, array, tamanio_array, *cantidad_elementos)) return;
  (*cantidad_elementos)++;
  
  recorrer_preorden(tamanio_arbol, nodo_actual->izquierda, array, tamanio_array, cantidad_elementos);
  recorrer_preorden(tamanio_arbol, nodo_actual->derecha, array, tamanio_array, cantidad_elementos);
}


/**
 * POST: Recorre el árbol con un recorrido inorden y carga los elementos posibles al array.
 */ 
void recorrer_inorden(size_t tamanio_arbol, nodo_abb_t* nodo_actual, void** array, size_t tamanio_array, size_t* cantidad_elementos){
  if(!nodo_actual) return;

  recorrer_inorden(tamanio_arbol, nodo_actual->izquierda, array, tamanio_array, cantidad_elementos);
  
  if(!agregar_a_array(nodo_actual->elemento, tamanio_arbol, array, tamanio_array, *cantidad_elementos)) return;
  (*cantidad_elementos)++;

  recorrer_inorden(tamanio_arbol, nodo_actual->derecha, array, tamanio_array, cantidad_elementos);
}


/**
 * POST: Recorre el árbol con un recorrido postorden y carga los elementos posibles al array.
 */ 
void recorrer_postorden(size_t tamanio_arbol, nodo_abb_t* nodo_actual, void** array, size_t tamanio_array, size_t* cantidad_elementos){
  if(!nodo_actual) return;
  
  recorrer_postorden(tamanio_arbol, nodo_actual->izquierda, array, tamanio_array, cantidad_elementos);
  recorrer_postorden(tamanio_arbol, nodo_actual->derecha, array, tamanio_array, cantidad_elementos);

 
  if(!agregar_a_array(nodo_actual->elemento, tamanio_arbol, array, tamanio_array, *cantidad_elementos)) return;
  (*cantidad_elementos)++;
}


/**
 * POST: Recorre el árbol con un recorrido preorden y llama a funcion con cada elemento
 * del árbol mientras esta devuelva true.
 */ 
void abb_con_cada_elemento_preorden(nodo_abb_t* nodo_actual, bool (*funcion)(void *, void *), void *aux, int* seguir_iterando, size_t* llamadas_funcion){
  if( (!nodo_actual) || (*seguir_iterando == 0) ) return;

  (*llamadas_funcion)++;
  if(!funcion(nodo_actual->elemento, aux)){
    (*seguir_iterando) = 0;
    return;
  }
  
  abb_con_cada_elemento_preorden(nodo_actual->izquierda, funcion, aux, seguir_iterando, llamadas_funcion);
  abb_con_cada_elemento_preorden(nodo_actual->derecha, funcion, aux, seguir_iterando, llamadas_funcion);
}

/**
 * POST: Recorre el árbol con un recorrido inorden y llama a funcion con cada elemento
 * del árbol mientras esta devuelva true.
 */ 
void abb_con_cada_elemento_inorden(nodo_abb_t* nodo_actual, bool (*funcion)(void *, void *), void *aux, int* seguir_iterando, size_t* llamadas_funcion){
  if( (!nodo_actual) || (!seguir_iterando) ) return;

  abb_con_cada_elemento_inorden(nodo_actual->izquierda, funcion, aux, seguir_iterando, llamadas_funcion);
  
  if( *seguir_iterando == 0 ) return;

  (*llamadas_funcion)++;
  if(!funcion(nodo_actual->elemento, aux)){
    (*seguir_iterando) = 0;
    return;
  }
  
  abb_con_cada_elemento_inorden(nodo_actual->derecha, funcion, aux, seguir_iterando, llamadas_funcion);
}

/**
 * POST: Recorre el árbol con un recorrido postorden y llama a funcion con cada elemento
 * del árbol mientras esta devuelva true.
 */ 
void abb_con_cada_elemento_postorden(nodo_abb_t* nodo_actual, bool (*funcion)(void *, void *), void *aux, int* seguir_iterando, size_t* llamadas_funcion){
  if(!nodo_actual) return;

  abb_con_cada_elemento_postorden(nodo_actual->izquierda, funcion, aux, seguir_iterando, llamadas_funcion);
  abb_con_cada_elemento_postorden(nodo_actual->derecha, funcion, aux, seguir_iterando, llamadas_funcion);
  
  if( *seguir_iterando == 0 ) return;

  (*llamadas_funcion)++;
  if(!funcion(nodo_actual->elemento, aux)){
    (*seguir_iterando) = 0;
    return;
  }
}


/**
 * PRE: Se recibe un recorrido que especifica el tipo de recorrido a realizar.
 * POST: Recorre el árbol e invoca la funcion con cada elemento almacenado en el mismo
 * Si la función devuelve false, se finaliza el recorrido aún si quedan
 * elementos por recorrer. Si devuelve true se sigue recorriendo mientras queden
 * elementos.
 * Devuelve la cantidad de veces que fue invocada la función o ERROR en caso de error.
 */
size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido, bool (*funcion)(void *, void *), void *aux){
  if( abb_vacio(arbol) || !funcion ) return ERROR;
  
  size_t invocaciones_funcion = 0;
  int seguir_iterando = 1;

  if(recorrido == PREORDEN){
    abb_con_cada_elemento_preorden(arbol->nodo_raiz, funcion, aux, &seguir_iterando, &invocaciones_funcion);
  }
  else if(recorrido == INORDEN){
    abb_con_cada_elemento_inorden(arbol->nodo_raiz, funcion, aux, &seguir_iterando, &invocaciones_funcion);
  }
  else if(recorrido == POSTORDEN){
    abb_con_cada_elemento_postorden(arbol->nodo_raiz, funcion, aux, &seguir_iterando, &invocaciones_funcion);
  }
  
  return invocaciones_funcion;
}

/**
 * POST: Recorre el árbol según el recorrido especificado y va almacenando los
 * elementos en el array hasta completar el recorrido o quedarse sin espacio en
 * el array.
 * Devuelve la cantidad de elementos que fueron almacenados exitosamente en el
 * array o ERROR en caso de error.
 */
size_t abb_recorrer(abb_t* arbol, abb_recorrido recorrido, void** array, size_t tamanio_array){
  if(abb_vacio(arbol)) return ERROR;

  size_t elementos_almacenados = 0;

  if(recorrido == PREORDEN){
    recorrer_preorden(arbol->tamanio, arbol->nodo_raiz, array, tamanio_array, &elementos_almacenados);
  }
  else if(recorrido == INORDEN){
    recorrer_inorden(arbol->tamanio, arbol->nodo_raiz, array, tamanio_array, &elementos_almacenados);
  }
  else if(recorrido == POSTORDEN){
    recorrer_postorden(arbol->tamanio, arbol->nodo_raiz, array, tamanio_array, &elementos_almacenados);
  }

  return elementos_almacenados;
}
