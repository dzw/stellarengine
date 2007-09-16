//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## PythonScript.cpp - Nuclex Python script             //
// ### # #      ###                                                      //
// # ### #      ###                                                      //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "PythonPlugin/Script/PythonScript.h"
#include "Nuclex/Kernel.h"
#include "Python/Python.h"

using namespace Nuclex;
using namespace Nuclex::Script;

namespace {

// ####################################################################### //
// # Nuclex::PythonDisplayHook()                                         # // 
// ####################################################################### //
PyObject *PythonDisplayHook(PyObject *pSelf, PyObject *pO) {
  if(pO != Py_None) {
    PyObject *pStrMessage = PyObject_Str(pO);
    Kernel::logMessage(
      Kernel::MT_INFO,
      PyString_AsString(pStrMessage)
    );
  }
  
  Py_INCREF(Py_None);
  return Py_None;
}

struct PyMethodDef DisplayHook_MethodDef[] = {
  { "displayhook", (PyCFunction)PythonDisplayHook, 1 },
  { NULL, NULL }
};

}

// ####################################################################### //
// # Nuclex::PythonScript::PythonScript()                    Constructor # // 
// ####################################################################### //
/** Initializes an instance of PythonScript
*/
PythonScript::PythonScript(const string &sScript) :
  m_pPythonInterpreter(PyInterpreterState_New()) {

  //PySys_SetObject("displayhook", );
  
  /* Compile Python code without running:
     _node *pNode = PyParser_SimpleParseString(sScript.c_str(), Py_file_input);
     PyCodeObject *pCompiledCode = PyNode_Compile(pNode, "Nuclex.Script");

     // Later: PyEval_EvalCode(pCompiledCode, pMainDictionary, pMainDictionary);
  */  

  if(sScript.length() > 0)
    parseScript(sScript);
}

// ####################################################################### //
// # Nuclex::PythonScript::~PythonScript()                    Destructor # // 
// ####################################################################### //
/** Destroys an instance of PythonScript
*/
PythonScript::~PythonScript() {
  PyInterpreterState_Delete(m_pPythonInterpreter);
}

// ####################################################################### //
// # Nuclex::PythonScript::parseScript()                                 # // 
// ####################################################################### //
/** Parses a script

    @param  sScript  Script to parse
*/
void PythonScript::parseScript(const string &sScript) {
//  PyImport_Import
  PyRun_SimpleString(sScript.c_str());
}

// ####################################################################### //
// # Nuclex::PythonScript::callProcedure()                               # // 
// ####################################################################### //
/** Calls a procedure in the script

    @param  sProc  Procedure to call
    @param  Args   Arguments to pass to the procedure
*/
Variant PythonScript::callProcedure(const string &sProc, const ArgumentList &Args) {
//  PyInterpre
//  PyInterpreterState *pOldInterpreter = PyThreadState_Swap(m_pPythonInterpreter);

  PyObject *pMainModule = PyImport_AddModule("__main__");
  if(!pMainModule)
    throw UnexpectedException("Nuclex::PythonScript::callProcedure()",
                              "The python __main__ module could not be imported");

  PyObject *pMainDictionary = PyModule_GetDict(pMainModule);

  PyObject *pFunction = PyDict_GetItemString(pMainDictionary, sProc.c_str());
  if(!pFunction)
    throw InvalidArgumentException("Nuclex::PythonScript::callProcedure()",
                                   string("Function '") + sProc + "' not found");

  if(!PyCallable_Check(pFunction))
    throw FailedException("Nuclex::PythonScript::callProcedure()",
                          string("'") + sProc + "' is not a procedure");

  PyObject *pArgs = PyTuple_New(Args.getNumArguments());
  
  for(unsigned long nArgument = 0; nArgument < Args.getNumArguments(); ++nArgument) {
    PyObject *pValue;
    switch(Args.getArgument(nArgument).getType()) {
      case Variant::T_NONE: {
        pValue = Py_None;
        break;
      }
      case Variant::T_BOOL: {
        pValue = PyBool_FromLong(Args.getArgument(nArgument).to<bool>());
        break;
      }
      case Variant::T_INT: case Variant::T_SIZE: {
        pValue = PyInt_FromLong(Args.getArgument(nArgument).to<int>());
        break;
      }
      case Variant::T_DOUBLE: {
        pValue = PyFloat_FromDouble(Args.getArgument(nArgument));
        break;
      }
      case Variant::T_STRING: {
        pValue = PyString_FromString(Args.getArgument(nArgument).to<string>().c_str());
        break;
      }
    }
    
    PyTuple_SetItem(pArgs, nArgument, pValue);
  }

  PyObject *pResult = PyObject_CallObject(pFunction, pArgs);
  if(!pResult)
    throw FailedException("Nuclex::PythonScript::executeCommand()",
                          "An exception was thrown by the script");

  Variant Result;
  if(pResult != Py_None) {
    PyObject *pStrResult = PyObject_Str(pResult);
    Result = string(PyString_AsString(pStrResult));
    
    Py_DECREF(pStrResult);
  }
  Py_DECREF(pResult);

  return Result;
}

// ####################################################################### //
// # Nuclex::PythonScript::executeCommand()                              # // 
// ####################################################################### //
/** Executes a command in the script

    @param  sCommand  Command to execute
    @return The command's result
*/
Variant PythonScript::executeCommand(const string &sCommand) {
  PyObject *pMainModule = PyImport_AddModule("__main__");
  if(!pMainModule)
    throw UnexpectedException("Nuclex::PythonScript::executeCommand()",
                              "The python __main__ module could not be imported");

  PyObject *pMainDictionary = PyModule_GetDict(pMainModule);

  PyObject *pResult = PyRun_String(sCommand.c_str(), Py_eval_input, 
                                   pMainDictionary, pMainDictionary);
  if(!pResult)
    throw FailedException("Nuclex::PythonScript::executeCommand()",
                          "An exception was thrown by the script");

  Variant Result;
  if(pResult != Py_None) {
    PyObject *pStrResult = PyObject_Str(pResult);
    Result = string(PyString_AsString(pStrResult));
    
    Py_DECREF(pStrResult);
  }
  Py_DECREF(pResult);

  //!!CHECK: Required ?  
  if(Py_FlushLine())
	PyErr_Clear();
	
  return Result;
}
