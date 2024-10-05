/*-----------------------------------------------------------------------------

 Copyright 2017 Hopsan Group

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.


 The full license is available in the file LICENSE.
 For details about the 'Hopsan Group' or information about Authors and
 Contributors see the HOPSANGROUP and AUTHORS files that are located in
 the Hopsan source code root directory.

-----------------------------------------------------------------------------*/

//!
//! @file   SignalSum.hpp
//! @author Peter Nordin <peter.nordin@liu.se>
//! @date   2011-04-04
//!
//! @brief Contains a mathematical multiport summator component
//!
//$Id$

// Header guard to avoid inclusion of the same code twice
// Every hpp file need to have its own UNIQUE header guard
#ifndef MYEXAMPLESIGNALSUM_HPP_INCLUDED
#define MYEXAMPLESIGNALSUM_HPP_INCLUDED

// Include the necessary header files from Hopsan
#include "ComponentEssentials.h"

// Put your component class inside the hopsan namespace (optional)
namespace hopsan {

// Define a new Class that inherits from ComponentC, ComponentQ or ComponentSignal
// This depends on the type of component you want to create, a C, Q or signal component
class MyExampleSignalSum : public ComponentSignal
{
private:
    // Private member variables
    size_t mnInputs;
    Port *mpMultiInPort, *mpOutPort;

public:
    // The creator function that is registered when a component lib is loaded into Hopsan
    // This static function is mandatory
    static Component *Creator()
    {
        return new MyExampleSignalSum();
    }

    // The Configure function that is run ONCE when a new object of the class is created
    // Use this function to set initial member variable values and to register Ports, Parameters and Startvalues
    // This function is mandatory
    void configure()
    {
        // Add ports to the component
        mpMultiInPort = addReadMultiPort("in", "NodeSignal", "", Port::NotRequired);
        mpOutPort = addOutputVariable("out", "The sum of inputs", "");
    }

    // The initialize function is called before simulation begins.
    // It may be called multiple times
    // In this function you can read or write from/to nodes
    // This function is optional but most likely needed
    void initialize()
    {
        // We assume that no-one will be disconnecting during simulation (that is not allowed)
        mnInputs = mpMultiInPort->getNumPorts();
        // Simulate one timestep in order to initialize the output
        simulateOneTimestep();
    }

    // The simulateOneTimestep() function is called ONCE every time step
    // This function contains the actual component simulation equations
    // This function is mandatory
    void simulateOneTimestep()
    {
        // Initialize sum variable
        double sum = 0;

        // Sum all input ports
        // This index i, represents each subport in the multiport
        for (size_t i=0; i<mnInputs; ++i)
        {
            sum += mpMultiInPort->readNode(NodeSignal::Value, i);
        }

        //Write value to output node
        mpOutPort->writeNode(NodeSignal::Value, sum);
    }

    // The finalize function is called after simulation ends
    // It may be called multiple times
    // Use this function to clean up of any custom allocations made in Initialize (if needed)
    // This function is optional
    //void finalize()
    //{

    //}

    // The deconfigure function is called just before a component is deleted
    // Use it to clean up after any custom allocation in the configure function
    // This function is optional
    //void deconfigure()
    //{
        // Nothing to deconfigure
    //}
};

}
#endif
