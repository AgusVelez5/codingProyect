#ifndef CODINGPROYECT_NODOARBOL_H
#define CODINGPROYECT_NODOARBOL_H

template<class T, class K>
class NodoArbol {
private:
    T cod; // dato codificado
    K desc; // dato decodificado
    int fe; // factor de equilibrio
    NodoArbol *izq;
    NodoArbol *der;
    NodoArbol *padre;
    friend class ArbolBinarioCod;
    friend class ArbolBinarioDesc;


public:
    NodoArbol() {
        izq = nullptr;
        der = nullptr;
        padre = nullptr;
        fe = 0;
    }

    NodoArbol(T c, K d) {
        izq = nullptr;
        der = nullptr;
        padre = nullptr;
        cod = c;
        desc = d;
        fe = 0;
    }

    NodoArbol(T c, K d, NodoArbol *pad) {
        izq = nullptr;
        der = nullptr;
        padre = pad;
        cod = c;
        desc = d;
        fe = 0;
    }

    T getCod() const {
        return cod;
    }

    K getDesc() const {
        return desc;
    }

    void setCod(T c) {
        NodoArbol::cod = c;
    }

    void setDesc(T d) {
        NodoArbol::desc = d;
    }

    NodoArbol *getIzq() const {
        return izq;
    }

    void setIzq(NodoArbol *izq) {
        NodoArbol::izq = izq;
    }

    NodoArbol *getDer() const {
        return der;
    }

    void setDer(NodoArbol *der) {
        NodoArbol::der = der;
    }

    NodoArbol *getPad() const {
        return padre;
    }

    void setPad(NodoArbol *pad){
        NodoArbol::padre = pad;
    }
};

#endif //CODINGPROYECT_NODOARBOL_H
