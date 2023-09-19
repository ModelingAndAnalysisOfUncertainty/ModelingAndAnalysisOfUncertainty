# Data Connoisseur (Modeling and Analysis of Uncertainty)

## Overview

[Modeling and Analysis of Uncertainty](https://github.com/ModelingAndAnalysisOfUncertainty) is an open source probability, statistics, and regression analysis engineering application from the [Rensselaer Center for Open Source Software (RCOS)](https://rcos.io/) at [Rensselaer Polytechnic Institute](https://www.rpi.edu/).

The Modeling and Analysis of Uncertainty project is hosted on [GitHub](https://github.com/ModelingAndAnalysisOfUncertainty).

## Compile Dlib with MFC Project on Visual Studio

### Overview

Add `dlib/all/source.cpp` to project source files and add the folder containing the dlib folder to the `#include` search path of Solution's Properties' VC++ Dictionaries. The whole Dlib will be compiled with the project instead of installing Dlib as a precompiled library (such as a Dynamic Link Library or Static Library). The downside is the compile time would be longer since the whole Dlib will be compiled. But this method can avoid Visual Studio's incompatible runtimes, build Dlib as a precompiled library and use with Visual Studio would raise Dlib's compile time error. More information can be found [here](http://dlib.net/compile.html).

### Compilation Instructions

#### 1. Add Dlib Source to Project

- Navigate to `Solution Explorer -> Source Files`.
- Choose `Add -> Existing Items...`.
- Select `dlib/all/source.cpp`.

#### 2. Configure Include Path

- In Solution Explorer, go to `Modeling and Analysis of Uncertainty -> Properties -> VC++ Dictionaries -> Include Dictionaries`.
- Add the file path of `dlib-19.24/source` (DO NOT PUT IN `source/dlib` INCLUDE ONLY THE PATH OF `source` folder).

#### 3. Adjust Precompiled Headers Settings

- Navigate to `Solution Explorer -> Source Files -> source.cpp -> Properties -> C/C++ -> Precompiled Headers`.
- Set it to `Not Using Precompiled Headers`.

#### 4. Add "bigobj" Command Line

- Navigate to `Solution Explorer -> Source Files -> Modeling and Analysis of Uncertainty -> Properties -> C/C++ -> Command Line -> Additional Options`.
- Add `/bigobj`.

Perform the above steps and you should be able to compile and run Dlib with the MFC program and enabling the ANN functionalities. You have to repeat the steps for both the Debug and Release mode.

## Dlib's Deep Learning Tool Explained

### Overview

The program's Artificial Neural Network component use Dlib's deep learning tools `<dlib/dnn.h>`. Dlib has very good documentation that explains everything. Reference Dlib's [documentation](http://dlib.net/ml.html) on deep learning tools and deep learning code examples [part1](http://dlib.net/dnn_introduction_ex.cpp.html) [part2](http://dlib.net/dnn_introduction2_ex.cpp.html) [part3](http://dlib.net/dnn_introduction3_ex.cpp.html) for guide.

With Dlib's deep learning tools, we can declear the model's typology, modify the layer's type, define custome trainer, and save the model. Dlib's deep learning implementation utilize multithread by default, so the trainer will be training the network with CPU multithreading, speeding up the training process.

### ERROR: Using `double` for Dlib

Using `double` for Dlib's layer would cause compile error since Dlib's deep learning tools (as well as many other deep learning tools) does not handle data types with high percision. `float` and `unsign long` usually works well with Dlib.

### `train_one_step()` VS `train()`

Dlib trainer call `train()` will set up an auto trainer with auto learning rate decay. While this auto trainer is easy to implement, we will not be using it. We will use the trainer call `train_one_step()` instead, since we need to collect training status, such as losses and accuracies. Auto trainer will also output training status by calling `be_verbose()`, but this function call will only output status in terminal.

## `CModelingandAnalysisofUncertaintyDoc::OnANN()` Explained

### Overview
The `CModelingandAnalysisofUncertaintyDoc::OnANN()` handles the Artificial Neural Network of the MFC program. First, it takes in user training setting from the Dialogue model -- `Selection.DoModal()` and store them inside the `ANNStruct` data structure. Second, it creates worker threads for ANN training --`AfxBeginThread(ANN_EEA_ThreadProc, pData)` -- so the main thread will still be responsive when the worker thread is training. Third, it will wait for the worker thread event message --`MsgWaitForMultipleObjects(1, &hEvent, FALSE, INFINITE, QS_ALLINPUT)`--untill the worker thread finished training. At last, upon receiving the event message, it will close the event handler --`CloseHandle(hEvent);`-- and notify the View to updates --`UpdateAllViews(NULL);`.
