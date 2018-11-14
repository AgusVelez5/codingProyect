#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
#include "ArbolBinarioCod.h"
#include "ArbolBinarioDesc.h"
#include <cctype>
#include <cstring>
#include <cstdio>
#include <algorithm>


using namespace std;


ArbolBinarioCod aCod;

ArbolBinarioDesc aDesc;

void encode(string arc, string dic, string imp);
void decode(string arc, string dic, string imp);
void splitAndPut(string cut, bool cod);

int main(int argc, char* argv[]) {

    string foo = argv[1];

   if (foo.compare("-d") == 0){
       decode (argv[5], argv[3], argv[7]);
   } else {
       encode (argv[5], argv[3], argv[7]);
   }
    return 0;
}

/* 
 *  Codifica el archivo, toma las palabras decodificadas,
 *  las busca en el arbol, y escribe en el archivo de salida
 *  el valor devuelto por la búsqueda. 
 *  @param arc Archivo a codificar
 *  @param dic Diccionario utilizado
 *  @param impr Archivo de salida
*/

void encode(string arc, string dic, string imp) {
    ifstream fileDic(dic);
    ifstream fileArc(arc);
    ofstream fileImp(imp);
    string str;
    string word;

    while (getline(fileDic, str)) {
        splitAndPut(str, true);
    }

    while (fileArc >> word) {

        transform(word.begin(), word.end(), word.begin(), ::tolower);

        try {
            fileImp << aCod.BuscarK(word) + " ";
        } catch (...){
            fileImp << word + " ";
        }

    }

    fileArc.close();
    fileDic.close();
    fileImp.close();
}

/* 
* Recorre linea por linea el diccionario, ingresando
*   al arbol los pares de palabras en cada nodo
*   @param cut 
*   @param cod Indica si el programa debe codificar o 
*   descodificar el programa
*/

void splitAndPut(string cut, bool cod) {
    // Used to split string around spaces.
    istringstream ss(cut);

    string word[2];

    int i = 0;

    do {
        // Read a word
        ss >> word[i];
        i++;
    } while (ss);

    if (cod){
        aCod.Insertar(word[1], word[0]);
    } else {
        aDesc.Insertar(word[1], word[0]);
    }

    // word[0] = descodificada ---- word[1] = codificada

}

/* 
*   Decodifica el archivo, toma las palabras codificadas,
*   las busca en el arbol, y escribe en el archivo de salida
*   el valor devuelto por la busqueda
*   @param arc Archivo a descodificar
*   @param dic Diccionario a utilizar
*   @param imp Archivo de salida
*/

void decode(string arc, string dic, string imp) {
    ifstream fileDic(dic);
    ifstream fileArc(arc);
    ofstream fileImp(imp);
    string str;
    string word;

    while (getline(fileDic, str)) {
        splitAndPut(str, false);
    }

    while (fileArc >> word) {

        transform(word.begin(), word.end(), word.begin(), ::tolower);

        try {
            fileImp << aDesc.BuscarT(word) + " ";
        } catch (...){
            fileImp << word + " ";
        }
    }

    fileArc.close();
    fileDic.close();
    fileImp.close();
}
