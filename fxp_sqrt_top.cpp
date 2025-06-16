/*
 * Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
 * Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "fxp_sqrt_top.h"
/*
out_data_t fxp_sqrt_top(in_data_t& in_val) {
    out_data_t result;
    fxp_sqrt(result, in_val);
    return result;
}
*/
//hls::stream<packet>& A, hls::stream<packet>& B)

void fxp_sqrt_top(hls::stream<axis_t>& in_stream,
                  hls::stream<axis_t>& out_stream,
                  int size)
{
    #pragma HLS INTERFACE axis port=in_stream
    #pragma HLS INTERFACE axis port=out_stream
    #pragma HLS INTERFACE s_axilite port=size bundle=control
   // #pragma HLS INTERFACE ap_ctrl_none port=return
    for (int i =0; i< size; i++) {
        #pragma HLS PIPELINE II=1
        axis_t in_word = in_stream.read();
        axis_t out_word;
        in_data_t in_val;
        out_data_t out_val;

        // Convert stream word to fixed-point input
        in_val.range() = in_word.data.range(); // typecast
        fxp_sqrt(out_val, in_val);    

        // Set stream output
        out_word.data = out_val.range();
        out_word.keep = -1;
        out_word.last = (i == size - 1) ? 1 : 0;
        out_stream.write(out_word);
        
    }
}
