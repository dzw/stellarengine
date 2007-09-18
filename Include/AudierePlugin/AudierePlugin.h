//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## NCXAudiere.h - Nuclex Audiere extension module      //
// ### # #      ###                                                      //
// # ### #      ###  Registers the extension module                      //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2003 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_AUDIERE_H
#define NUCLEX_AUDIERE_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/Synchronization.h"
#include "Nuclex/Kernel.h"

// Der folgende ifdef-Block zeigt die Standardlösung zur Erstellung von
// Makros, die das Exportieren aus einer DLL vereinfachen. Alle Dateien
// in dieser DLL wurden mit dem in der Befehlszeile definierten Symbol
// NUCLEX_EXPORTS kompiliert. Dieses Symbol sollte für kein Projekt definiert
// werden, das diese DLL verwendet. Auf diese Weise betrachtet jedes andere
// Projekt, dessen Quellcodedateien diese Datei einbeziehen, 
// NUCLEX_API-Funktionen als aus einer DLL importiert, während diese DLL mit
// diesem Makro definierte Symbole als exportiert betrachtet.
#ifdef AUDIEREPLUGIN_EXPORTS
  #define NUCLEXAUDIERE_API __declspec(dllexport)
#else
  #define NUCLEXAUDIERE_API __declspec(dllimport)
#endif

namespace Nuclex {

NUCLEXAUDIERE_API Mutex &getAudiereMutex();

} // namespace Nuclex

DECLARATIONPACKAGE(AudierePlugin);

#endif // NUCLEX_AUDIERE_H
