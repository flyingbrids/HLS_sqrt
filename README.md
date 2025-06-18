# HLS_sqrt
Use Vivado HLS to create fully pipelined square root IP that has axis &amp; axi-lite interface.
The axis stream in interface will stream input data and the axis stream out interface will stream out result.
The size of each burst is sent via axi-lite interface. Based on this size, the tlast signal will know when to strobe.

