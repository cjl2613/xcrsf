/*
 * MIT License
 *
 * Copyright (c) 2025 Vinzenz Weist
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <cstring>
#include <vector>
#include <lgpio.h>
#include <unistd.h>
#include "xcrsf/crossfire.h"
#include <thread>
#include <chrono>

    struct channel{
        int channel_value;
        int value 

        bool button() {
            if (value == 2012)
                return true;
            else
                return false;
        }

        int range() {
            return value;
        }

        int triState() {
            if (value == 2012)
                return 2;
            if (value == 988)
                return 0;
            else{
                return 1;
            }
        }
    };

channel GX12[16];

int main() {
    auto crossfire = crossfire::XCrossfire("/dev/ttyAMA0");
    if (crossfire.open_port()) { std::printf("Port opened...\n"); }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    for (int i = 0;i<16; i++){
        GX12[i].channel_value = i+1;
        GX12[i].value = 0;
    }

    while (true) {
        while (!crossfire.is_paired()) {
            std::printf("Waiting for reconnect...\n");
            auto _ = crossfire.open_port();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

        std::printf("Remote connected!\n");
        while (crossfire.is_paired()) {
            const auto channels = crossfire.get_channel_state();
            for (int i = 0; i < 16; i++) {
                if (channels.front() != 0) { 
                    std::printf("Channel %d: %u\n", i+1, channels[i]); 
                    GX12[i].value = channels[i];
                    printf("d%n", GX12[i].value);
                    
                }
            }
            const auto _ = crossfire.set_telemetry_battery(11.8, 25, 1200, 96);
            std::this_thread::sleep_for(std::chrono::milliseconds(2500));
        }
    }
    return 0;
}

void get_values(channel[] GX12){
    GX12[0].range();      // Rturn
    GX12[1].range();      // Rthrottle
    GX12[2].range();      // Lthrottle
    GX12[3].range();      // Lturn

    GX12[4].triState();   // Far_left_switch
    GX12[5].triState();   // Left_Switch
    GX12[6].triState();   // FarRightSwitch
    GX12[7].triState();   // RightSwitch

    GX12[8].button();     // LeftButton
    GX12[9].button();     // RightButton
    GX12[10].button();    // FarLeftButton1
    GX12[11].button();    // FarLeftButton2

    GX12[12].range();     // PotLeft
    GX12[13].range();     // PotRight
    GX12[14].range();     // DialLeft
    GX12[15].range();     // DialRight
}