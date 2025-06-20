# HLS_sqrt
Use Vivado HLS to create fully pipelined square root IP that has axis &amp; axi-lite interface.
The axis stream in interface will stream input data and the axis stream out interface will stream out result.
The size of each burst is sent via axi-lite interface. Based on this size, the tlast signal will know when to strobe.

The Size information is passed by AXI4-lite interface. It is memory map interface used for reading and writing register/memory 
![alt text](https://github.com/flyingbrids/HLS_sqrt/blob/main/axi4-lite.png?raw=true)

The input data is passed by AXIS stream in interface.
![alt text](https://github.com/flyingbrids/HLS_sqrt/blob/main/axis-in.png?raw=true)

The output data is passed by AXIS stream out interfacec
![alt text](https://github.com/flyingbrids/HLS_sqrt/blob/main/axis-out.png?raw=true)
