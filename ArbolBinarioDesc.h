#ifndef CODINGPROYECT_ARBOLBINARIODESC_H
#define CODINGPROYECT_ARBOLBINARIODESC_H

#include "NodoArbol.h"
#include <iostream>

using namespace std;

template<class T, class K>
class ArbolBinarioDesc {
protected:
    enum {IZQUIERDO, DERECHO};
    NodoArbol<T, K> *raiz;
    NodoArbol<T, K> *actual;
    int contador;
    int altura;

public:

    ArbolBinarioDesc();

    ~ArbolBinarioDesc();

    void Insertar( K d, T c);

    bool Buscar(K d);

    T BuscarT( K d);

    bool Vacio(NodoArbol<T, K> *r) { return r==NULL; }

    void Borrar(K d);

    bool EsHoja(NodoArbol<T, K> *r) { return !r->der && !r->izq; }

    const int NumeroNodos();

    const int AlturaArbol();

    int Altura( K d);

    T &ValorActual() { return actual->desc; }

    void Raiz() { actual = raiz; }

    void InOrden(void (*func)(T&, int) , NodoArbol<T, K> *nodo=NULL, bool r=true);
    void PreOrden(void (*func)(T&, int) , NodoArbol<T, K> *nodo=NULL, bool r=true);
    void PostOrden(void (*func)(T&, int) , NodoArbol<T, K> *nodo=NULL, bool r=true);


private:

    // Funciones de equilibrado:
    void Equilibrar(NodoArbol<T, K> *nodo, int, bool);
    void RSI(NodoArbol<T, K>* nodo);
    void RSD(NodoArbol<T, K>* nodo);
    void RDI(NodoArbol<T, K>* nodo);
    void RDD(NodoArbol<T, K>* nodo);
    // Funciones auxiliares
    void Podar(NodoArbol<T, K>* &);
    void auxContador(NodoArbol<T, K>*);
    void auxAltura(NodoArbol<T, K>*, int);


};


/**
 * Constructor del Arbol
 * @tparam K Clave por la cual va a ordenar el árbol
 * @tparam T Valor guardado por el árbol
 */
template<class T, class K>
ArbolBinarioDesc<T, K>::ArbolBinarioDesc() {
    raiz = nullptr;
    actual = nullptr;
}


/**
 * Destructor del Arbol
 */
template<class T, class K>
ArbolBinarioDesc<T, K>::~ArbolBinarioDesc() {
    Podar(raiz);
}


// Poda: borrar todos los nodos a partir de uno, incluido
template<class T, class K>
void ArbolBinarioDesc<T, K>::Podar(NodoArbol<T, K>* &nodo)
{
    // Algoritmo recursivo, recorrido en postorden
    if(nodo) {
        Podar(nodo->izq); // Podar izquierdo
        Podar(nodo->der);   // Podar derecho
        delete nodo;            // Eliminar nodo
        nodo = NULL;
    }
}

template<class T, class K>
void ArbolBinarioDesc<T, K>::Insertar(const K d, T c) {
    NodoArbol<T, K> *padre = NULL;

    cout << "Insertar: " << d << endl;
    actual = raiz;
    // Buscar el dato en el árbol, manteniendo un puntero al nodo padre
    while(!Vacio(actual) && d != actual->desc) {
        padre = actual;
        if(d > actual->desc) actual = actual->der;
        else if(d < actual->desc) actual = actual->izq;
    }

    // Si se ha encontrado el elemento, regresar sin insertar
    if(!Vacio(actual)) return;
    // Si padre es NULL, entonces el árbol estaba vacío, el nuevo nodo será
    // el nodo raiz
    if(Vacio(padre)) raiz = new NodoArbol<T, K>(c, d);
        // Si el dato es menor que el que contiene el nodo padre, lo insertamos
        // en la rama izquierda
    else if(d < padre->desc) {
        padre->izq = new NodoArbol<T, K>(c, d ,padre);
        Equilibrar(padre, IZQUIERDO, true);
    }
        // Si el dato es mayor que el que contiene el nodo padre, lo insertamos
        // en la rama derecha
    else if(d > padre->desc) {
        padre->der = new NodoArbol<T, K>(c, d,padre);
        Equilibrar(padre, DERECHO, true);
    }
}


// Equilibrar árbol AVL partiendo del nodo nuevo
template<class T, class K>
void ArbolBinarioDesc<T, K>::Equilibrar(NodoArbol<T, K> *nodo, int rama, bool nuevo)
{
    bool salir = false;

    // Recorrer camino inverso actualizando valores de FE:
    while(nodo && !salir) {
        if(nuevo)
            if(rama == IZQUIERDO) nodo->fe--; // Depende de si añadimos ...
            else                  nodo->fe++;
        else
        if(rama == IZQUIERDO) nodo->fe++; // ... o borramos
        else                  nodo->fe--;
        if(nodo->fe == 0) salir = true; // La altura de las rama que
            // empieza en nodo no ha variado,
            // salir de Equilibrar
        else if(nodo->fe == -2) { // Rotar a derechas y salir:
            if(nodo->izq->fe == 1) RDD(nodo); // Rotación doble
            else RSD(nodo);                         // Rotación simple
            salir = true;
        }
        else if(nodo->fe == 2) {  // Rotar a izquierdas y salir:
            if(nodo->der->fe == -1) RDI(nodo); // Rotación doble
            else RSI(nodo);                        // Rotación simple
            salir = true;
        }
        if(nodo->padre)
            if(nodo->padre->der == nodo) rama = DERECHO; else rama = IZQUIERDO;
        nodo = nodo->padre; // Calcular FE, siguiente nodo del camino.
    }
}

// Rotación doble a derechas
template<class T, class K>
void ArbolBinarioDesc<T, K>::RDD(NodoArbol<T, K>* nodo) {
    cout << "RDD" << endl;
    NodoArbol<T, K> *Padre = nodo->padre;
    NodoArbol<T, K> *P = nodo;
    NodoArbol<T, K> *Q = P->izq;
    NodoArbol<T, K> *R = Q->der;
    NodoArbol<T, K> *B = R->izq;
    NodoArbol<T, K> *C = R->der;

    if(Padre)
        if(Padre->der == nodo) Padre->der = R;
        else Padre->izq = R;
    else raiz = R;

    // Reconstruir árbol:
    Q->der = B;
    P->izq = C;
    R->izq = Q;
    R->der = P;

    // Reasignar padres:
    R->padre = Padre;
    P->padre = Q->padre = R;
    if(B) B->padre = Q;
    if(C) C->padre = P;

    // Ajustar valores de FE:
    switch(R->fe) {
        case -1: Q->fe = 0; P->fe = 1; break;
        case 0:  Q->fe = 0; P->fe = 0; break;
        case 1:  Q->fe = -1; P->fe = 0; break;
    }
    R->fe = 0;
}

// Rotación doble a izquierdas
template<class T, class K>
void ArbolBinarioDesc<T, K>::RDI(NodoArbol<T, K>* nodo) {
    cout << "RDI" << endl;
    NodoArbol<T, K> *Padre = nodo->padre;
    NodoArbol<T, K> *P = nodo;
    NodoArbol<T, K> *Q = P->der;
    NodoArbol<T, K> *R = Q->izq;
    NodoArbol<T, K> *B = R->izq;
    NodoArbol<T, K> *C = R->der;

    if(Padre)
        if(Padre->der == nodo) Padre->der = R;
        else Padre->izq = R;
    else raiz = R;

    // Reconstruir árbol:
    P->der = B;
    Q->izq = C;
    R->izq = P;
    R->der = Q;

    // Reasignar padres:
    R->padre = Padre;
    P->padre = Q->padre = R;
    if(B) B->padre = P;
    if(C) C->padre = Q;

    // Ajustar valores de FE:
    switch(R->fe) {
        case -1: P->fe = 0; Q->fe = 1; break;
        case 0:  P->fe = 0; Q->fe = 0; break;
        case 1:  P->fe = -1; Q->fe = 0; break;
    }
    R->fe = 0;
}

// Rotación simple a derechas
template<class T, class K>
void ArbolBinarioDesc<T, K>::RSD(NodoArbol<T, K>* nodo)
{
    cout << "RSD" << endl;
    NodoArbol<T, K> *Padre = nodo->padre;
    NodoArbol<T, K> *P = nodo;
    NodoArbol<T, K> *Q = P->izq;
    NodoArbol<T, K> *B = Q->der;

    if(Padre)
        if(Padre->der == P) Padre->der = Q;
        else Padre->izq = Q;
    else raiz = Q;

    // Reconstruir árbol:
    P->izq = B;
    Q->der = P;

    // Reasignar padres:
    P->padre = Q;
    if(B) B->padre = P;
    Q->padre = Padre;

    // Ajustar valores de FE:
    P->fe = 0;
    Q->fe = 0;
}

// Rotación simple a izquierdas
template<class T, class K>
void ArbolBinarioDesc<T, K>::RSI(NodoArbol<T, K>* nodo)
{
    cout << "RSI" << endl;
    NodoArbol<T, K> *Padre = nodo->padre;
    NodoArbol<T, K> *P = nodo;
    NodoArbol<T, K> *Q = P->der;
    NodoArbol<T, K> *B = Q->izq;

    if(Padre)
        if(Padre->der == P) Padre->der = Q;
        else Padre->izq = Q;
    else raiz = Q;

    // Reconstruir árbol:
    P->der = B;
    Q->izq = P;

    // Reasignar padres:
    P->padre = Q;
    if(B) B->padre = P;
    Q->padre = Padre;

    // Ajustar valores de FE:
    P->fe = 0;
    Q->fe = 0;
}

// Eliminar un elemento de un árbol AVL
template<class T, class K>
void ArbolBinarioDesc<T, K>::Borrar(const K d)
{
    NodoArbol<T, K> *padre = NULL;
    NodoArbol<T, K> *nodo;
    K aux;

    actual = raiz;
    // Mientras sea posible que el valor esté en el árbol
    while(!Vacio(actual)) {
        if(d == actual->desc) { // Si el valor está en el nodo actual
            if(EsHoja(actual)) { // Y si además es un nodo hoja: lo borramos
                if(padre) // Si tiene padre (no es el nodo raiz)
                    // Anulamos el puntero que le hace referencia
                    if(padre->der == actual) padre->der = NULL;
                    else if(padre->izq == actual) padre->izq = NULL;
                delete actual; // Borrar el nodo
                actual = NULL;
                // El nodo padre del actual puede ser ahora un nodo hoja, por lo tanto su
                // FE es cero, pero debemos seguir el camino a partir de su padre, si existe.
                if((padre->der == actual && padre->fe == 1) ||
                   (padre->izq == actual && padre->fe == -1)) {
                    padre->fe = 0;
                    actual = padre;
                    padre = actual->padre;
                }
                if(padre)
                    if(padre->der == actual) Equilibrar(padre, DERECHO, false);
                    else                         Equilibrar(padre, IZQUIERDO, false);
                return;
            }
            else { // Si el valor está en el nodo actual, pero no es hoja
                // Buscar nodo
                padre = actual;
                // Buscar nodo más izquierdo de rama derecha
                if(actual->der) {
                    nodo = actual->der;
                    while(nodo->izq) {
                        padre = nodo;
                        nodo = nodo->izq;
                    }
                }
                    // O buscar nodo más derecho de rama izquierda
                else {
                    nodo = actual->izq;
                    while(nodo->der) {
                        padre = nodo;
                        nodo = nodo->der;
                    }
                }
                // Intercambiar valores de no a borrar u nodo encontrado
                // y continuar, cerrando el bucle. El nodo encontrado no tiene
                // por qué ser un nodo hoja, cerrando el bucle nos aseguramos
                // de que sólo se eliminan nodos hoja.
                aux = actual->cod;
                actual->cod = nodo->cod;
                nodo->cod = aux;
                actual = nodo;
            }
        }
        else { // Todavía no hemos encontrado el valor, seguir buscándolo
            padre = actual;
            if(d > actual->cod) actual = actual->der;
            else if(d < actual->cod) actual = actual->izq;
        }
    }
}

// Recorrido de árbol en inorden, aplicamos la función func, que tiene
// el prototipo:
// template<class DATO> void func(DATO&);
template<class T, class K>
void ArbolBinarioDesc<T, K>::InOrden(void (*func)(T&, int) , NodoArbol<T, K> *nodo, bool r)
{
    if(r) nodo = raiz;
    if(nodo->izq) InOrden(func, nodo->izq, false);
    func(nodo->cod, nodo->fe);
    if(nodo->der) InOrden(func, nodo->der, false);
}

// Recorrido de árbol en preorden, aplicamos la función func, que tiene
// el prototipo:
// template<class DATO> void func(DATO&);
template<class T, class K>
void ArbolBinarioDesc<T, K>::PreOrden(void (*func)(T&, int), NodoArbol<T, K> *nodo, bool r)
{
    if(r) nodo = raiz;
    func(nodo->cod, nodo->fe);
    if(nodo->izq) PreOrden(func, nodo->izq, false);
    if(nodo->der) PreOrden(func, nodo->der, false);
}

// Recorrido de árbol en postorden, aplicamos la función func, que tiene
// el prototipo:
// template<class DATO> void func(DATO&);
template<class T, class K>
void ArbolBinarioDesc<T, K>::PostOrden(void (*func)(T&, int), NodoArbol<T, K> *nodo, bool r)
{
    if(r) nodo = raiz;
    if(nodo->izq) PostOrden(func, nodo->izq, false);
    if(nodo->der) PostOrden(func, nodo->der, false);
    func(nodo->cod, nodo->fe);
}

// Buscar un valor en el árbol
template<class T, class K>
bool ArbolBinarioDesc<T, K>::Buscar(const K d) {
    actual = raiz;

    // Todavía puede aparecer, ya que quedan nodos por mirar
    while(!Vacio(actual)) {
        if(d == actual->desc) return true; // dato encontrado
        else if(d > actual->desc) actual = actual->der; // Seguir
        else if(d < actual->desc) actual = actual->izq;
    }
    return false; // No está en árbol
}

template<class T, class K>
T ArbolBinarioDesc<T, K>::BuscarT(const K d) {
    actual = raiz;

    // Todavía puede aparecer, ya que quedan nodos por mirar
    while(!Vacio(actual)) {
        if(d == actual->desc) return actual->cod; // dato encontrado
        else if(d > actual->desc) actual = actual->der; // Seguir
        else if(d < actual->desc) actual = actual->izq;
    }
    throw 1; // No está en árbol
}

// Calcular la altura del nodo que contiene el dato dat
template<class T, class K>
int ArbolBinarioDesc<T, K>::Altura(const K d)
{
    int altura = 0;
    actual = raiz;

    // Todavía puede aparecer, ya que quedan nodos por mirar
    while(!Vacio(actual)) {
        if(d == actual->desc) return altura; // dato encontrado
        else {
            altura++; // Incrementamos la altura, seguimos buscando
            if(d > actual->desc) actual = actual->der;
            else if(d < actual->desc) actual = actual->izq;
        }
    }
    return -1; // No está en árbol
}

// Contar el número de nodos
template<class T, class K>
const int ArbolBinarioDesc<T, K>::NumeroNodos()
{
    contador = 0;

    auxContador(raiz); // FUnción auxiliar
    return contador;
}

// Función auxiliar para contar nodos. Función recursiva de recorrido en
//   preorden, el proceso es aumentar el contador
template<class T, class K>
void ArbolBinarioDesc<T, K>::auxContador(NodoArbol<T, K> *nodo)
{
    contador++;  // Otro nodo
    // Continuar recorrido
    if(nodo->izq) auxContador(nodo->izq);
    if(nodo->der)   auxContador(nodo->der);
}

// Calcular la altura del árbol, que es la altura del nodo de mayor altura.
template<class T, class K>
const int ArbolBinarioDesc<T, K>::AlturaArbol()
{
    altura = 0;

    auxAltura(raiz, 0); // Función auxiliar
    return altura;
}

// Función auxiliar para calcular altura. Función recursiva de recorrido en
// postorden, el proceso es actualizar la altura sólo en nodos hojas de mayor
// altura de la máxima actual
template<class T, class K>
void ArbolBinarioDesc<T, K>::auxAltura(NodoArbol<T, K> *nodo, int a)
{
    // Recorrido postorden
    if(nodo->izq) auxAltura(nodo->izq, a+1);
    if(nodo->der)   auxAltura(nodo->der, a+1);
    // Proceso, si es un nodo hoja, y su altura es mayor que la actual del
    // árbol, actualizamos la altura actual del árbol
    if(EsHoja(nodo) && a > altura) altura = a;
}

/**
 * Muestra un árbol por consola
 */
// template<class T>
// void ArbolBinario<T>::print() {
//    void print(bool esDerecho, string identacion) {
//        if (der != NULL) {
//            der->print(true, identacion + (esDerecho ? "     " : "|    "));
//        }
//        cout << identacion;
//        if (esDerecho) {
//            cout << " /";
//        } else {
//            cout << " \\";
//        }
//        cout << "-- ";
//        cout << dato << endl;
//        if (izq != NULL) {
//            izq->print(false, identacion + (esDerecho ? "|    " : "     "));
//        }
//    }
//}


#endif //CODINGPROYECT_ARBOLBINARIODESC_H
