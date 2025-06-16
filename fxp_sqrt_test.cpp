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

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

#include "fxp_sqrt_top.h"

#define NUM_TEST_ITERS 100 // 0000
#define MIN_ITER_IDX 0
#define MAX_ITER_IDX (MIN_ITER_IDX + NUM_TEST_ITERS)

#define MY_DRAND() (rand() * rand() / (double)(RAND_MAX * RAND_MAX + 1))

#define ABS_ERR_THRESH (0.0 / (double)(1ll << (OUT_BW - OUT_IW)))

// Test program for validating C-model functionality and RTL co-simulation
int main(int argc, char* argv[]) {    
    hls::stream<axis_t> in_stream;
    hls::stream<axis_t> out_stream;
    in_data_t test_val;
    unsigned err_cnt = 0;

    for (uint32_t i = MIN_ITER_IDX; i < MAX_ITER_IDX; i++) {
         test_val = i;
         axis_t in_word;
         in_word.data = test_val.range();
         in_word.keep = -1;
         in_word.last = (i== NUM_TEST_ITERS -1)? 1 : 0;
         in_stream.write(in_word);
    }
     
    // Run theVivado HLStop-level function
    fxp_sqrt_top(in_stream, out_stream, MAX_ITER_IDX);

    for (uint32_t i = MIN_ITER_IDX; i < MAX_ITER_IDX; i++) {
        test_val = i;        
        axis_t out_word = out_stream.read();
        out_data_t hw_outval;
        hw_outval.range() = out_word.data.range();

        ap_ufixed<OUT_BW, OUT_IW, AP_RND> sw_outval =
            sqrt(test_val.to_double());

        if (NUM_TEST_ITERS <= 100) {
            cout << "sqrt(" << test_val << ") = " << sw_outval << ";\t";
            cout << "sqrt_fxp(" << test_val << ") = " << hw_outval << endl;
        } else {
            if (i == 0)
                cout << "Running test.";
            else if (!(i % (NUM_TEST_ITERS / 100))) {
                cout << ".";
                fflush(stdout);
            }
        }
        if (fabs(hw_outval.to_double() - sw_outval.to_double()) >
            ABS_ERR_THRESH) {
            cout << "MISMATCH (" << i << "): \t";
            cout << "fxp_sqrt(" << test_val << ") = " << hw_outval;
            cout << "\tDelta = "
                 << (hw_outval.to_double() - sw_outval.to_double());
            cout << "\thw_outval/sw_outval = "
                 << (hw_outval.to_double() / sw_outval.to_double());
            cout << endl << endl;
            err_cnt++;
        }
    }

    cout << endl;
    if (err_cnt) {
        cout << "!!! ERROR: " << err_cnt << " mismatches detected !!!";
        cout << endl << endl;
    } else {
        cout << "*** Test passes ***" << endl << endl;
    }
    if (err_cnt)
        return 1;
    else
        return 0;
}
