//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Nuclex.cpp - Nuclex base header                                           //
// ### # #      ###                                                                            //
// # ### #      ###  Root header to be included by all nuclex headers                          //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Nuclex.h"

#if defined(NUCLEX_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

/** @mainpage Nuclex documentation

    @section About About Nuclex
      @subsection Intro Introduction
        Nuclex is a complete object oriented framework for developing 
        real-time 3d games and other software. It has no quirks or hacks
        and uses the most advanced C++ to increase flexibility and
        to reduce code bloating management tasks.

        The entire engine uses a plug-in based approach to enable
        add-on modules to extend the functionality of nuclex in any
        way imageineable. As a cross-platform library, it also acts
        as a layer between the operating system and the application.

        Throughout nuclex, a lot of terms are standardized for a specific
        meaning. Care has been taked to select terms in accordance to common
        standards and to avoid duplicate meanings of any single word.
        Once known, this will greatly reduce the time needed to understand
        the interfaces of nuclex. For example, an xxxFactory is always an
        object which creates other objects of type xxx (factory pattern).
        A stream is an interface through which data can be read and written.

      @subsection Usage Using nuclex        
        The root object of Nuclex is the kernel. The Kernel connects all
        subsystems of the entire engine and is typically the first object
        created by the application.




         



        Nuclex itself is rather small. It only consists of a kernel which
        provides a hierarchical object tree, extension module services and a logging
        system. Based thereon it further hosts three main services:

        - StorageServer for managing resource accesses. Provides an abstracted
          file system consisting of storages (roughly equalling folders)
          which contain streams (roughly equalling files). Storages can be mapped
          as virtual drives, so, for example, the path 'data:file.dat' could
          refer to the storage labeled 'data' and its stream named 'file.dat'.
          The application doesn't need to know where the data comes from.

        - ScriptServer for managing scripting languages and scripts. Nuclex can use
          any number of scripting languages simultaneously, and enables scripts to
          seamlessly interact with any script-enabled class. Scriptlets can be used
          to place script objects into the hierarchical object tree.

        - EventServer for managing global inter-object notifications. Any object can
          subscribe itself as sink to an event and any object is allowed to fire events.
          The events are completely unbound, which means that event sinks may be
          registered before the event source is created and that multiple event
          sources can fire the same event. Event scripts can subscribe to events.
          
        Extension modules contain any number of extension objects. They can, for
        example, extend the StorageServer with the capability to read .zip files
        as if they were plain directories, add a new scripting language, or introduce
        a totally new object, which can either place itself in the object tree or
        be creatable at the users desire.

      @subsection Copy Copyright
        Nuclex was written by Markus Ewald @ LunaticSystems.
        The author can be contacted via nuclex@spam.lunaticsystems.de (remove the 'spam.')

        Nuclex is freely distributable. You are free to use it in your
        own applications, freeware and commercial ones in any form, without
        the requirement of making the source to your application public.

        See License.txt for further informations.
*/

#if defined(NUCLEX_WIN32)
// ####################################################################### //
// # DllMain()                                                           # // 
// ####################################################################### //
/** Dll main entry point

    @param  hPlugIn            Dll instance module handle
    @param  ul_reson_for_call  Call reason message
    @param  lpReserved         Varying
*/
BOOL APIENTRY DllMain(HANDLE hPlugIn, 
                      DWORD  ul_reason_for_call, 
                      LPVOID lpReserved) {
  switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
      break;
  }
 
  return TRUE;
}
#endif
