#ifndef PYPULSEDTHREAD_H
#define PYPULSEDTHREAD_H
#include <Python.h>
#include <pulsedThread.h>

/*****************************************************************************************************************
pyPulsedThread is code you can use to to wrap the C++ pulsedThread class into a Python external module. 
Other modules that wish to include pulsedThread need to provide an initialization function that returns a
PyCapsule containing a pointer to a pulsedThread object. The functions in pyPulsedThread can then
interact with the PyCapsule object asking it to do tasks and configure pulse/train timing. The code
is in the header file so the static functions can be incorporated into a module.

see pyPulsedThread_SimpleGPIO for an example that controls single pulses and trains of pulses on raspberry pi GPIO pins.
******************************************************************************************************************/

/*Function called automatically when PyCapsule object is deleted in Python */
static void  pulsedThread_del(PyObject * PyPtr){
    delete static_cast<pulsedThread *> (PyCapsule_GetPointer (PyPtr, "pulsedThread"));
}

/*Checks if a pulsedThread thread is busy */
static PyObject* pulsedThread_isBusy (PyObject *self, PyObject *PyPtr) {
    pulsedThread * threadPtr = static_cast<pulsedThread * > (PyCapsule_GetPointer(PyPtr, "pulsedThread"));
     return Py_BuildValue("i", threadPtr -> isBusy());
}

/*Waits on a pulsedThread thread (up to TmeOutSecs), returning 0 when it is no longer busy or 1 if TmeOutSecs elapsed*/
static PyObject* pulsedThread_waitOnBusy (PyObject *self, PyObject *args) {
	PyObject *PyPtr;
	float timeOutSecs;
	if (!PyArg_ParseTuple(args,"Of", &PyPtr, &timeOutSecs)) {
		PyErr_SetString (PyExc_RuntimeError, "Could not parse arguments for pulsedThread pointer and timeOut seconds.");
		return NULL;
	}
	pulsedThread * threadPtr = static_cast<pulsedThread * > (PyCapsule_GetPointer(PyPtr, "pulsedThread"));
	return Py_BuildValue("i", threadPtr -> waitOnBusy(timeOutSecs));
}

/*pulsedThread_doTask tells the pulsedThread object to perform whatever task it was configured to do */
static PyObject* pulsedThread_doTask (PyObject *self, PyObject *PyPtr) {
    pulsedThread * threadPtr = static_cast<pulsedThread * > (PyCapsule_GetPointer(PyPtr, "pulsedThread"));
    threadPtr->DoTask();
    Py_RETURN_NONE;
}

/*pulsedThread_doTasks tells the pulsedThread object to perform whatever task it was configured to do multiple times*/
static PyObject* pulsedThread_doTasks (PyObject *self, PyObject *args) {
	PyObject *PyPtr;
	int nTimes;
	if (!PyArg_ParseTuple(args,"Oi", &PyPtr, &nTimes)) {
		PyErr_SetString (PyExc_RuntimeError, "Could not parse arguments for pulsedThread pointer and number of times to do task");
		return NULL;
	}
	pulsedThread * threadPtr = static_cast<pulsedThread * > (PyCapsule_GetPointer(PyPtr, "pulsedThread"));
	threadPtr->DoTasks(nTimes);
	Py_RETURN_NONE;
}

/*pulsedThread_doTask tells the pulsedThread object to stop perform whatever tasks it was asked to do */
static PyObject* pulsedThread_unDoTasks (PyObject *self, PyObject *PyPtr) {
    pulsedThread * threadPtr = static_cast<pulsedThread * > (PyCapsule_GetPointer(PyPtr, "pulsedThread"));
    threadPtr->UnDoTasks();
    Py_RETURN_NONE;
}

  /*pulsedThread_startTrain tells  pulsedThread object configured as an infinite train to start ticking*/
static PyObject* pulsedThread_startTrain (PyObject *self, PyObject *PyPtr) {
    pulsedThread * threadPtr = static_cast<pulsedThread * > (PyCapsule_GetPointer(PyPtr, "pulsedThread"));
    threadPtr->startInfiniteTrain();
    Py_RETURN_NONE;
}

 /*pulsedThread_stopTrain tells  pulsedThread object configured as an infinite train to stop ticking*/
static PyObject* pulsedThread_stopTrain (PyObject *self, PyObject *PyPtr) {
    pulsedThread * threadPtr = static_cast<pulsedThread * > (PyCapsule_GetPointer(PyPtr, "pulsedThread"));
    threadPtr->stopInfiniteTrain();
    Py_RETURN_NONE;
}

/* ---------Modifiers for pulse timing based on individual pulses and numbers of pulses--------------
Modifies the delay of a pulse, or the "low" time of a train, value is in seconds*/
static PyObject*  pulsedThread_modDelay (PyObject *self, PyObject *args) {
    PyObject *PyPtr;
    float newDelay;
    if (!PyArg_ParseTuple(args,"Of", &PyPtr, &newDelay)) {
	    PyErr_SetString (PyExc_RuntimeError, "Could not parse arguments for pulsedThread pointer and pulse delay seconds.");
        return NULL;
    }
    pulsedThread * threadPtr = static_cast<pulsedThread * > (PyCapsule_GetPointer(PyPtr, "pulsedThread"));
    return Py_BuildValue("i", threadPtr ->modDelay ((unsigned int) round (1e06 * newDelay)));
}

/* modifies the duration of a pulse, or the "high" time of a train, value is in seconds*/
static PyObject*  pulsedThread_modDur (PyObject *self, PyObject *args) {
    PyObject *PyPtr;
    float newDur;
    if (!PyArg_ParseTuple(args,"Of", &PyPtr, &newDur)) {
	PyErr_SetString (PyExc_RuntimeError, "Could not parse arguments for pulsedThread pointer and pulse duration seconds.");
        return NULL;
    }
    pulsedThread * threadPtr = static_cast<pulsedThread * > (PyCapsule_GetPointer(PyPtr, "pulsedThread"));
    return Py_BuildValue("i", threadPtr ->modDur ((unsigned int) round (1e06 * newDur)));
}


/*modifies the number of pulses of a train */
static PyObject*  pulsedThread_modTrainLength(PyObject *self, PyObject *args) {
    PyObject *PyPtr;
    unsigned int newTrainLength;
    if (!PyArg_ParseTuple(args,"OI", &PyPtr, &newTrainLength)) {
	PyErr_SetString (PyExc_RuntimeError, "Could not parse arguments for pulsedThread pointer and number of pulses in train");
        return NULL;
    }
    pulsedThread * threadPtr = static_cast<pulsedThread * > (PyCapsule_GetPointer(PyPtr, "pulsedThread"));
    return Py_BuildValue("i", threadPtr ->modTrainLength (newTrainLength));
}


/* ---------Modifiers for pulse timing based on train characteristics of train duration, frequency, and duty cycle-------------

modifies the total duration of a train, new duration is in seconds */
static PyObject*  pulsedThread_modTrainDur (PyObject *self, PyObject *args) {
    PyObject *PyPtr;
    float newDur;
    if (!PyArg_ParseTuple(args,"Of", &PyPtr, &newDur)) {
	PyErr_SetString (PyExc_RuntimeError, "Could not parse arguments for pulsedThread pointer and train duration.");
        return NULL;
    }
    pulsedThread * threadPtr = static_cast<pulsedThread * > (PyCapsule_GetPointer(PyPtr, "pulsedThread"));
    return Py_BuildValue("i", threadPtr ->modTrainDur (newDur));
}

 /*modifies the frequency of a train, new frequency is in Hz */
static PyObject*  pulsedThread_modFreq(PyObject *self, PyObject *args) {
    PyObject *PyPtr;
    float newFreq;
    if (!PyArg_ParseTuple(args,"Of", &PyPtr, &newFreq)) {
	PyErr_SetString (PyExc_RuntimeError, "Could not parse arguments for pulsedThread pointer and train frequency.");
        return NULL;
    }
    pulsedThread * threadPtr = static_cast<pulsedThread * > (PyCapsule_GetPointer(PyPtr, "pulsedThread"));
    return Py_BuildValue("i", threadPtr ->modFreq (newFreq));
}

/* modifies the duty cycle of a train (ON time/(ON time + OFF time)) */
static PyObject*  pulsedThread_modDutyCycle (PyObject *self, PyObject *args) {
    PyObject *PyPtr;
    float newDutyCycle;
    if (!PyArg_ParseTuple(args,"Of", &PyPtr, &newDutyCycle)) {
	PyErr_SetString (PyExc_RuntimeError, "Could not parse arguments for pulsedThread pointer and train duty cycle.");
        return NULL;
    }
    pulsedThread * threadPtr = static_cast<pulsedThread * > (PyCapsule_GetPointer(PyPtr, "pulsedThread"));
    return Py_BuildValue("i", threadPtr ->modDutyCycle (newDutyCycle));
}


/* Calls getModCustomStatus, returns 1 if a modData or ModCustom function is waiting to be run, else 0 */
static PyObject* pulsedThread_modCustomStatus (PyObject *self, PyObject *PyPtr) {
	pulsedThread * threadPtr = static_cast<pulsedThread * > (PyCapsule_GetPointer(PyPtr, "pulsedThread"));
	return Py_BuildValue("i", threadPtr ->getModCustomStatus ());
}

/* ---------Getters for pulse timing based on individual pulses and numbers of pulses--------------*/
static PyObject* pulsedThread_getPulseDelay (PyObject *self, PyObject *PyPtr) {
    pulsedThread * threadPtr = static_cast<pulsedThread * > (PyCapsule_GetPointer(PyPtr, "pulsedThread"));
     return Py_BuildValue("f", (float) 1e-06 * threadPtr -> getpulseDelayUsecs());
}

static PyObject* pulsedThread_getPulseDuration (PyObject *self, PyObject *PyPtr) {
	pulsedThread * threadPtr = static_cast<pulsedThread * > (PyCapsule_GetPointer(PyPtr, "pulsedThread"));
	return Py_BuildValue("f", (float) 1e-06 * threadPtr -> getpulseDurUsecs());
}

static PyObject* pulsedThread_getPulseNumber (PyObject *self, PyObject *PyPtr) {
	pulsedThread * threadPtr = static_cast<pulsedThread * > (PyCapsule_GetPointer(PyPtr, "pulsedThread"));
    return Py_BuildValue("I", threadPtr -> getNpulses());
}

/* ---------Getters for pulse timing based on train duration, frequency, duty cycle--------------*/
static PyObject* pulsedThread_getTrainDuration (PyObject *self, PyObject *PyPtr) {
	pulsedThread * threadPtr = static_cast<pulsedThread * > (PyCapsule_GetPointer(PyPtr, "pulsedThread"));
    return Py_BuildValue("f", threadPtr -> getTrainDuration());
}

static PyObject* pulsedThread_getTrainFrequency (PyObject *self, PyObject *PyPtr) {
	pulsedThread * threadPtr = static_cast<pulsedThread * > (PyCapsule_GetPointer(PyPtr, "pulsedThread"));
    return Py_BuildValue("f", threadPtr -> getTrainFrequency());
}

static PyObject* pulsedThread_getTrainDutyCycle (PyObject *self, PyObject *PyPtr) {
    pulsedThread * threadPtr = static_cast<pulsedThread * > (PyCapsule_GetPointer(PyPtr, "pulsedThread"));
    return Py_BuildValue("f", threadPtr -> getTrainDutyCycle());
}

/* ***************************EndFunc utilities****************************************
we can check if an endFunc is installed and un-install an end func without knowing anything about the endFunc */

// checks if this pulsed thread is set to run an endFunc
static PyObject* pulsedThread_hasEndFunc (PyObject *self, PyObject *PyPtr) {
	pulsedThread * threadPtr = static_cast<pulsedThread * > (PyCapsule_GetPointer(PyPtr, "pulsedThread"));
	return Py_BuildValue("i", threadPtr ->hasEndFunc ());
}

// un-installs any installed endFunc
static PyObject* pulsedThread_UnSetEndFunc (PyObject *self, PyObject *PyPtr) {
	pulsedThread * threadPtr = static_cast<pulsedThread * > (PyCapsule_GetPointer(PyPtr, "pulsedThread"));	
	threadPtr->unSetEndFunc ();
	Py_RETURN_NONE;
}


/* *************************************** Support for Python Objects providing Hi, Lo, and endFuncs ********************************
Runs PyObject.HiFunc()  taskData is assumed to be a pointer to a python object that has methods called HiFunc and loFunc that take no arguments */
static void pulsedThread_RunPythonHiFunc (void * taskData){
	PyObject *PyObjPtr = (PyObject *) taskData;
	PyGILState_STATE state=PyGILState_Ensure();
	PyObject *result = PyObject_CallMethod (PyObjPtr, "HiFunc", NULL);
	Py_DECREF (result);
	PyGILState_Release(state);
}

// Runs PyObject.LoFunc()  taskData is assumed to be a pointer to a python object that has methods called HiFunc and LoFunc that take no arguments
static void pulsedThread_RunPythonLoFunc (void * taskData){
	PyObject *PyObjPtr = (PyObject *) taskData;
	PyGILState_STATE state=PyGILState_Ensure();
	PyObject *result = PyObject_CallMethod (PyObjPtr, "LoFunc", NULL);
	Py_DECREF (result);
	PyGILState_Release(state);
}

// Runs PyObject.endFunc()  taskData is assumed to be a pointer to a python object that has a method called endFunc that takes 4 int arguments 
static void pulsedThread_RunPythonEndFunc_p (void * endFuncData, taskParams * theTask){
	PyObject *PyObjPtr = (PyObject *) endFuncData;
	PyGILState_STATE state=PyGILState_Ensure();
	PyObject *result = PyObject_CallMethod (PyObjPtr, "EndFunc", "(iiii)",theTask->pulseDelayUsecs, theTask->pulseDurUsecs, theTask->nPulses, theTask->doTask);
	Py_DECREF (result);
	PyGILState_Release(state);
}

// Runs PyObject.endFunc(frequency, dutyCycle, train Duration, doTask)  taskData is assumed to be a pointer to a python object that has a method called EndFunc that takes 3 float arguments and 1 int arguments
static void pulsedThread_RunPythonEndFunc_f (void * endFuncData, taskParams * theTask){
	PyObject *PyObjPtr = (PyObject *) endFuncData;
	PyGILState_STATE state=PyGILState_Ensure();
	PyObject *result = PyObject_CallMethod (PyObjPtr, "EndFunc", "(fffi)",theTask->trainFrequency, theTask->trainDutyCycle, theTask->trainDuration, theTask->doTask);
	Py_DECREF (result);
	PyGILState_Release(state);
}


// installs pulsedThread_RunPythonEndFunc, either interger or float version,  as the endFunc, calling the endFunc of the same Python object used for HI and LO funcs
// the modFunc that is passed to pulsedThread->modCustom so we change the object when thread is not busy
int pulsedThread_modEndFuncObj (void * pyObjPtr, taskParams * taskDataP){
	taskDataP->endFuncData = pyObjPtr;
	return 0;
}

static PyObject* pulsedThread_SetPythonEndFuncObj (PyObject *self, PyObject *args) {
	PyObject *PyPtr;		// first argument is the Python pyCapsule that points to the pulsedThread
	PyObject *PyObjPtr;		// second argument is a Python object that better have an endFunc 
	int endFuncPulseDesc;	// 0 for calling endFunc with frequency,duration,train length info, non-zero for microseond pulse delay, duration, and pulse number description
	int isLocking;
	if (!PyArg_ParseTuple(args,"OOii", &PyPtr, &PyObjPtr, &endFuncPulseDesc, &isLocking)) {
		PyErr_SetString (PyExc_RuntimeError, "Could not parse arguments for pulsedThread pointer, Python Object providing endFunction, pulse description code, and isLocking.");
		return NULL;
	}
	pulsedThread * threadPtr = static_cast<pulsedThread * > (PyCapsule_GetPointer(PyPtr, "pulsedThread"));
	// set the endFunc data to the object
	threadPtr->modCustom (&pulsedThread_modEndFuncObj, (void *) PyObjPtr, isLocking);
	
	if (endFuncPulseDesc == 0){
		threadPtr->setEndFunc (&pulsedThread_RunPythonEndFunc_f);
	}else{
		threadPtr->setEndFunc (&pulsedThread_RunPythonEndFunc_p);
	}
	// Activate Python Thread Awareness
	if (!PyEval_ThreadsInitialized()){
		PyEval_InitThreads();
	}
	Py_RETURN_NONE;
}


// the modFunc that is passed to pulsedThread->modCustom
int pulsedThread_modTaskObj (void * pyObjPtr, taskParams * taskDataP){
	taskDataP->taskData = pyObjPtr;
	return 0;
}

// changes the object that is used when calling LO func and HI funcc
static PyObject* pulsedThread_SetPythonTaskObj (PyObject *self, PyObject *args) {
	PyObject *PyPtr;		// first argument is the Python pyCapsule that points to the pulsedThread
	PyObject *PyObjPtr;		// second argument is a Python object that better have HI and LO Functions
	int isLocking;
	if (!PyArg_ParseTuple(args,"OOi", &PyPtr, &PyObjPtr, &isLocking)) {
		PyErr_SetString (PyExc_RuntimeError, "Could not parse arguments for pulsedThread pointer, Python Object providing the HI and LO functions, and isLocking.");
		return NULL;
	}
	pulsedThread * threadPtr = static_cast<pulsedThread * > (PyCapsule_GetPointer(PyPtr, "pulsedThread"));
	// set the high and low functions
	threadPtr->setLowFunc (&pulsedThread_RunPythonLoFunc); // sets the function that is called on low part of cycle
	threadPtr->setHighFunc (&pulsedThread_RunPythonLoFunc); // sets the function that is called on high part of cycle
	// set the task data to the object
	threadPtr->modCustom (&pulsedThread_modTaskObj, (void *) PyObjPtr, isLocking);
	// Activate Python Thread Awareness
	if (!PyEval_ThreadsInitialized()){
		PyEval_InitThreads();
	}
	Py_RETURN_NONE;
}

/*Function for passing an array to a train or pulse, and selecting one of the C++ endFunc that sets dutyCycle or Frequency from the array */
static PyObject* pulsedThread_setArrayFunc (PyObject *self, PyObject *args) {
	PyObject * PyPtr;	// pulsed thread object, either a train or a pulse
	PyObject * bufferObj; // the floating point array of dutyCycles or Frequencies
	int endFuncType;  // 0 for frequency, 1 for dutyCycle
	int isLocking; 
	
	if (!PyArg_ParseTuple(args,"OOii", &PyPtr, &bufferObj, &endFuncType, &isLocking)) {
		PyErr_SetString (PyExc_RuntimeError, "Could not parse arguments for pulsedThread pointer, floating point buffer,  endFunction type code, and isLocking.");
		return NULL;
	}
	if (PyObject_CheckBuffer (bufferObj) == 0){
		PyErr_SetString (PyExc_RuntimeError, "Error getting bufferObj from Python array.");
		return NULL;
	}
	Py_buffer buffer;
	if (PyObject_GetBuffer (bufferObj, &buffer, PyBUF_FORMAT)==-1){
		PyErr_SetString (PyExc_RuntimeError,"Error getting C array from bufferObj from Python array");
		return NULL;
	}
	//printf ("Buffer type is %s, length is %d bytes, and item size is %d.\n", buffer.format, buffer.len, buffer.itemsize);
	if (strcmp (buffer.format, "f") != 0){
		PyErr_SetString (PyExc_RuntimeError, "Error for bufferObj: data type of Python array is not float");
		return NULL;
	}
	
	float* arrayStart = static_cast <float *>(buffer.buf); // Now we have a pointer to the array from the passed in buffer
	pulsedThread * threadPtr = static_cast<pulsedThread * > (PyCapsule_GetPointer(PyPtr, "pulsedThread")); // get pointer to pulsedThread
	int errVar = threadPtr->setUpEndFuncArray (arrayStart, (unsigned int) buffer.len/buffer.itemsize, isLocking);	
	//int errVar = SimpleGPIO_setUpArray (threadPtr, arrayStart, (unsigned int) buffer.len/buffer.itemsize, isLocking); // adds a pointer to the array to the endFunc data
	if (errVar){
		PyErr_SetString (PyExc_RuntimeError, "Failed to set up the array for the endFunction");
		return NULL;
	}
	// install the endFunction that updates the duty cycle or frequency after every train of pulses
	if (endFuncType ==0){
		threadPtr->setEndFunc (&pulsedThreadDutyCycleFromArrayEndFunc);
	}else{
		threadPtr->setEndFunc (&pulsedThreadFreqFromArrayEndFunc);
	}
	PyBuffer_Release (&buffer); // we don't need the buffer object as we have a pointer to the array start, which is all we care about
	Py_RETURN_NONE;
}

static PyObject* pulsedThread_cosineDutyCycleArray (PyObject *self, PyObject *args) {
	PyObject * bufferObj; // a floating point array
	unsigned int period;
	float offset;
	float scaling;
	
	if (!PyArg_ParseTuple(args,"OIff", &bufferObj, &period, &offset, &scaling)){
		PyErr_SetString (PyExc_RuntimeError, "Could not parse arguments for floating point array, cos period, offset, and scaling");
		return NULL;
	}
	if (PyObject_CheckBuffer (bufferObj) == 0){
		PyErr_SetString (PyExc_RuntimeError, "Error getting bufferObj from Python array.");
		return NULL;
	}
	Py_buffer buffer;
	if (PyObject_GetBuffer (bufferObj, &buffer, PyBUF_FORMAT)==-1){
		PyErr_SetString (PyExc_RuntimeError,"Error getting C array from bufferObj from Python array");
		return NULL;
	}
	//printf ("Buffer type is %s, length is %d bytes, and item size is %d.\n", buffer.format, buffer.len, buffer.itemsize);
	if (strcmp (buffer.format, "f") != 0){
		PyErr_SetString (PyExc_RuntimeError, "Error for bufferObj: data type of Python array is not float");
		return NULL;
	}
	float* arrayData = static_cast <float *>(buffer.buf); // Now we have a pointer to the array from the passed in buffer	
	int errVar = pulsedThread::cosineDutyCycleArray  (arrayData, (unsigned int) buffer.len/buffer.itemsize, period,  offset,  scaling);
	if (errVar){
		PyErr_SetString (PyExc_RuntimeError, "Adjust offset and scaling so cosine is bounded by 0 and 1");
		return NULL;
	}
	Py_RETURN_NONE;
}


#endif
