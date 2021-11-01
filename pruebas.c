#include "src/abb.h"
#include <stdio.h>
#include "pa2mm.h"
#include "string.h"


int comparar_elementos(void* elemento_1, void* elemento_2){
  return ( (*(int*)(elemento_1)) - (*(int*)(elemento_2)) );
}


void Puedo_Insertar_Buscar_y_Quitar_Elementos(){
  abb_t* arbol = abb_crear(comparar_elementos);

  int CERO=0, UNO=1, DOS=2, TRES=3, TREINTA=30;

  //############# INSERTAR ELEMENTOS, BÚSQUEDA, ÁRBOL VACÍO Y TAMAÑO ÁRBOL #############
  
  int vacio = abb_vacio(arbol);
  pa2m_afirmar(vacio != 0, "No hay elementos, el árbol está vacío");

  size_t tamanio = abb_tamanio(arbol);
  pa2m_afirmar(tamanio == 0, "Árbol está vacío y tiene tamaño cero");

  abb_insertar(arbol, &DOS);
  pa2m_afirmar( (arbol->nodo_raiz->elemento) == &DOS, "Se insertó correctamente un elemento");

  vacio = abb_vacio(arbol);
  pa2m_afirmar(vacio == 0, "Agrego elementos, el árbol ya NO está vacío");

  abb_insertar(arbol, &UNO);
  pa2m_afirmar( (arbol->nodo_raiz->izquierda->elemento) == &UNO, "Se insertó correctamente un hijo izquierdo");

  tamanio = abb_tamanio(arbol);
  pa2m_afirmar(tamanio == 2, "Árbol tiene dos elementos y tamaño es dos");

  abb_insertar(arbol, &TRES);
  pa2m_afirmar( (arbol->nodo_raiz->derecha->elemento) == &TRES, "Se insertó correctamente un hijo derecho");

  abb_insertar(arbol, &CERO);
  pa2m_afirmar( (arbol->nodo_raiz->izquierda->izquierda->elemento) == &CERO, "Se insertó correctamente un ñieto izquierdo");

  int* elemento_encontrado = abb_buscar(arbol, &CERO);
  pa2m_afirmar((*elemento_encontrado) == CERO, "Se buscó correctamente un elemento");


  elemento_encontrado = abb_buscar(arbol, NULL);
  pa2m_afirmar(elemento_encontrado == NULL, "Busco elemento NULL, devuelve NULL");

  elemento_encontrado = abb_buscar(arbol, &TREINTA);
  pa2m_afirmar(elemento_encontrado == NULL, "Busco elemento inexistente, devuelve NULL");



  //############################ QUITAR NODOS HOJAS o con UN HIJO ############################
  int* elemento_quitado = abb_quitar(arbol, &TRES);
  pa2m_afirmar((arbol->nodo_raiz->derecha) == NULL, "Se quitó un nodo hoja y se actualizó el árbol correctamente");
  pa2m_afirmar((*elemento_quitado) == TRES, "Se devuelve correctamente el elemento quitado, del nodo hoja");
  
  elemento_quitado = abb_quitar(arbol, NULL);
  pa2m_afirmar(elemento_quitado == NULL, "Quiero quitar un elemento inválido, devuelve NULL");

  elemento_quitado = abb_quitar(arbol, &TREINTA);
  pa2m_afirmar(elemento_encontrado == NULL, "Quiero quitar un elemento inexistente, devuelve NULL");

  
  elemento_quitado = abb_quitar(arbol, &UNO);
  pa2m_afirmar((arbol->nodo_raiz->izquierda->elemento) == &CERO, "Se quitó un nodo con un hijo y se actualizó el árbol correctamente");
  pa2m_afirmar((*elemento_quitado) == UNO, "Se devuelve correctamente el elemento quitado, del nodo hijo");


  elemento_quitado = abb_quitar(arbol, &DOS);
  pa2m_afirmar((arbol->nodo_raiz->elemento) == &CERO, "Se quitó el nodo raíz con un hijo y se actualizó el árbol correctamente");
  pa2m_afirmar((*elemento_quitado) == DOS, "Se devuelve correctamente el elemento quitado, del nodo raíz");


  abb_destruir(arbol);
}

void Puedo_Borrar_Elementos_con_Dos_Hijos(){
  abb_t* arbol = abb_crear(comparar_elementos);
  int CERO=0, UNO=1, DOS=2, TRES=3, CUATRO=4, CINCO=5, SEIS=6, SIETE=7, OCHO=8, NUEVE=9, DIEZ=10, ONCE=11;

  
  abb_insertar(arbol, &OCHO);
  abb_insertar(arbol, &TRES);
  abb_insertar(arbol, &UNO);
  abb_insertar(arbol, &CUATRO);
  abb_insertar(arbol, &CINCO);
  abb_insertar(arbol, &SIETE);
  abb_insertar(arbol, &SEIS);
  abb_insertar(arbol, &CERO);
  abb_insertar(arbol, &DOS);
  abb_insertar(arbol, &ONCE);
  abb_insertar(arbol, &NUEVE);
  abb_insertar(arbol, &DIEZ);

  void* elemento_quitado = abb_quitar(arbol, &TRES);
  pa2m_afirmar(elemento_quitado == &TRES,"Quito nodo con dos hijos y devuelvo el elemento quitado correctamente");
  pa2m_afirmar( ((arbol->nodo_raiz->izquierda->elemento) == &DOS) && ((arbol->nodo_raiz->izquierda->izquierda->elemento) == &UNO) && 
  ((arbol->nodo_raiz->izquierda->derecha->elemento) == &CUATRO), "Se actualizaron correctamente los nodos luego de la eliminación");

  abb_quitar(arbol, &OCHO);
  abb_quitar(arbol, &SIETE);
  abb_quitar(arbol, &SEIS);
  abb_quitar(arbol, &CINCO);
  abb_quitar(arbol, &CUATRO);
  abb_quitar(arbol, &DOS);
  abb_quitar(arbol, &UNO);
  abb_quitar(arbol, &CERO);
  abb_quitar(arbol, &ONCE);
  abb_quitar(arbol, &NUEVE);
  abb_quitar(arbol, &DIEZ);
  pa2m_afirmar((arbol->nodo_raiz) == NULL, "Puedo eliminar todos los elementos del árbol siempre del nodo raíz");

  free(arbol);
}

bool Recorro_Hasta_el_Elemento_UNO(void* elemento, void* aux){
  if( (*(int*)elemento) == 1){
    return false;
  }
  return true;
}

void Pruebo_Iteradores(){
  abb_t* arbol = abb_crear(comparar_elementos);
  int CERO=0, UNO=1, DOS=2, TRES=3, CUATRO=4, CINCO=5, SEIS=6, SIETE=7, OCHO=8, NUEVE=9, DIEZ=10, ONCE=11;

  abb_insertar(arbol, &OCHO);
  abb_insertar(arbol, &TRES);
  abb_insertar(arbol, &UNO);
  abb_insertar(arbol, &CUATRO);
  abb_insertar(arbol, &CINCO);
  abb_insertar(arbol, &SIETE);
  abb_insertar(arbol, &SEIS);
  abb_insertar(arbol, &CERO);
  abb_insertar(arbol, &DOS);
  abb_insertar(arbol, &ONCE);
  abb_insertar(arbol, &NUEVE);
  abb_insertar(arbol, &DIEZ);

  size_t iteraciones = abb_con_cada_elemento(arbol, PREORDEN, Recorro_Hasta_el_Elemento_UNO, NULL);
  pa2m_afirmar(iteraciones == 3, "Con iterador PREORDEN se llama a la función tantas veces como se debería");

  iteraciones = abb_con_cada_elemento(arbol, INORDEN, Recorro_Hasta_el_Elemento_UNO, NULL);
  pa2m_afirmar(iteraciones == 2, "Con iterador INORDEN se llama a la función tantas veces como se debería");

  iteraciones = abb_con_cada_elemento(arbol, POSTORDEN, Recorro_Hasta_el_Elemento_UNO, NULL);
  pa2m_afirmar(iteraciones == 3, "Con iterador POSTORDEN se llama a la función tantas veces como se debería");

  int* array[5];


  size_t cantidad_elementos = abb_recorrer(arbol, PREORDEN, (void**)array, 5);
  pa2m_afirmar(cantidad_elementos == 5, "Con iterador PREORDEN se agregan los elementos posibles en array");
  pa2m_afirmar( ((array[0] == &OCHO) && (array[1] == &TRES) && (array[2] == &UNO) && (array[3] == &CERO) && (array[4] == &DOS)),
  "Con iterador PREORDEN se agregaron en la posición correcta del array los elementos");
  


  cantidad_elementos = abb_recorrer(arbol, INORDEN, (void**)array, 5);
  pa2m_afirmar(cantidad_elementos == 5, "Con iterador INORDEN se agregan los elementos posibles en array");
  pa2m_afirmar( ((array[0] == &CERO) && (array[1] == &UNO) && (array[2] == &DOS) && (array[3] == &TRES) && (array[4] == &CUATRO)),
  "Con iterador INORDEN se agregaron en la posición correcta del array los elementos");
  


  cantidad_elementos = abb_recorrer(arbol, POSTORDEN, (void**)array, 5);
  pa2m_afirmar(cantidad_elementos == 5, "Con iterador POSTORDEN se agregan los elementos posibles en array");
  pa2m_afirmar( ((array[0] == &CERO) && (array[1] == &DOS) && (array[2] == &UNO) && (array[3] == &SEIS) && (array[4] == &SIETE)),
  "Con iterador POSTORDEN se agregaron en la posición correcta del array los elementos");
  
  abb_destruir(arbol);
}


int main(){
  pa2m_nuevo_grupo("Pruebas de Inserción, Búsqueda y Eliminación");
  Puedo_Insertar_Buscar_y_Quitar_Elementos();
  Puedo_Borrar_Elementos_con_Dos_Hijos();

  pa2m_nuevo_grupo("Pruebas de Iteradores");
  Pruebo_Iteradores();


  return pa2m_mostrar_reporte();
}
