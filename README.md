# HomeAutomationServer
The Server consists of 2 layers 
1. A wifi layer for interacting with the users, implemented on esp8266
2. An rf 433MHz layer for interacting with the devices connected to each node,
implemented using pic16f628a
Each node can be configured either as a control or trigger node.The nodes are implemented
using pic12f683.
Control nodes deliver digital commands(on/off, toggle, flash) to the connected devices while
trigger nodes receive trigger commands from the user and broadcast to listening control nodes
