# Robot-Pigeon
## Description
In automated warehouses, for instance, robots are
responsible for storing and fetching goods, among other
missions. In these environments, especially when large
distances have to be covered, robots may be used to
carry physically, not only goods but messages and useful
data; they act similarly to pigeons at the times where
electronic circuits where not used for communications ...
The scenario we want to address is constituted by two
partitions of a given Wireless Sensor Network (**WSN**) separated by hundreds of
meters which should be interconnected by the use of a
couple of robots.

Each of the robots only goes halfway of the track
between the two **WSN** nets. Arriving there, it waits for
the other robot so that information can be transferred
from one to the other. After finishing this transfer each
of the robots will start move towards the WSN partition
it is serving.

## Authors
[Pedro Carmelo](pedro.carmelo@tecnico.ulisboa.pt "pedro.carmelo@tecnico.ulisboa.pt")

[Lénio Passos](lenio.passos@tecnico.ulisboa.pt "lenio.passos@tecnico.ulisboa.pt")
## Hardware
Partitioned Sensor Network Nodes: 2 or more motes 

Robots: 2x 3PI+mbed platform
## Development Stages
The project will be developed in three stages: 

1. Setting the **WSN** using ISTmotes or micaZ's
2. Splitting the WSN in two so that one part can no longer communicate with the other
3. Setup the 3PI+mbed robot to interact with the **WSN**

## Sequence Diagram
We expect to have:

1. Two threads in the transmission node (one for the serial communication and the other for the TRX)
2. Two threads in the robot (one for the movement and the other to handle the TRX/TRX)
3. One thread in the reception node.

In order to guarantee concurrency the threads interact to each other through messages described in following Sequence Diagram
[Sequence Diagram in UML](/seq_diagram/seq_diagram.pdf)

## Planning
[Development Phases](https://rawgit.com/lbpassos/Robot-Pigeon/master/gantt/robicch-jQueryGantt-86e2144/gantt.html)

*Please Note: Use Chrome and enable scripts.* 

The Gantt chart was built based on the work of [robicch](https://github.com/robicch/jQueryGantt). Changes were made in the Javascript code in the file [gantt.html](/gantt/robicch-jQueryGantt-86e2144/gantt.html) in order to configure and properly run in Markdown.

## Progress Status
27-03-2017 - Choosing which mote to use for the **WSN** nodes
