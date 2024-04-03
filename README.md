This simulator is made in the C++ language using the Qt creator work environment. 
In this context it was mainly used for managing the creation of the application.

To be able to view the simulator in action, you need to open the SO.pro file from QT. 

In Qt you will find the following scheme.

  ProcessPlanningSimulator
      ├── SO.pro
   	  ├──Headers 
      │   	├── algorithms.hpp
      │	    ├── concurrent_queue.hpp
      │	    ├── utils.hpp
      │	    ├── mainwidget.hpp
      │	    ├── menuwidget.hpp
      │   	├── process.hpp
      │ 	  ├── simulationwidget.hpp
      │	    ├── titlewidget.hpp
      └── Sources
   	        ├── algorithms.cpp
	          ├── utils.cpp
 	          ├── main.cpp
 	          ├── mainwidget.cpp
	          ├── menuwidget.cpp
   	        ├── process.cpp
	          ├── simulationwidget.cpp
	          └── titlewidget.cpp

