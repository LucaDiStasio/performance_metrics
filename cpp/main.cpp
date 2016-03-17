/*
 * =====================================================================================
 * 
 * Copyright (c) 2016 Université de Lorraine & Luleå tekniska universitet
 * Author: Luca Di Stasio <luca.distasio@gmail.com>
 *                        <luca.distasio@ingpec.eu>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * =====================================================================================
 */

#include <iostream>
#include <fstream>

#include "perfmetrics.h"

using namespace std;
//using namespace boost;
//using namespace boost::program_options;

//==================  MAIN  =======================
int main(int argc, char** argv){
    
    perfmetrics metrics;
    
    metrics.get_systemRAM();
    metrics.get_RAMusage();
    metrics.get_RAMpeak();
    metrics.get_CPUthreads();
    
    vector<double> vec;
    int N = 50000;
    
    for(int i=0; i<N; i++){
        vec.push_back(pow(0.001*i,-2));
    }
    
    cout << "System RAM:" << endl;
    cout << "-----------------------------" << endl;
    cout << metrics.provide_systemRAM() << "B" << endl;
    cout << metrics.provide_systemRAM(3) << "KB" << endl;
    cout << metrics.provide_systemRAM(6) << "MB" << endl;
    cout << metrics.provide_systemRAM(9) << "GB" << endl;
    cout << endl;
    
    cout << "RAM usage:" << endl;
    cout << "-----------------------------" << endl;
    cout << metrics.provide_RAMusage() << "B" << endl;
    cout << metrics.provide_RAMusage(3) << "KB" << endl;
    cout << metrics.provide_RAMusage(6) << "MB" << endl;
    cout << metrics.provide_RAMusage(9) << "GB" << endl;
    cout << endl;
    
    cout << "RAM peak:" << endl;
    cout << "-----------------------------" << endl;
    cout << metrics.provide_RAMpeak() << "B" << endl;
    cout << metrics.provide_RAMpeak(3) << "KB" << endl;
    cout << metrics.provide_RAMpeak(6) << "MB" << endl;
    cout << metrics.provide_RAMpeak(9) << "GB" << endl;
    cout << endl;
    
    if(metrics.is_ClockSteady()){
        cout << "The system clock is steady." << endl;
    }else{
        cout << "The system clock is not steady." << endl;
    }
    cout << endl;
    
    cout << "Number of CPU threads: " << metrics.provide_CPUthreads() << endl;
    cout << endl;
    
    metrics.get_wallClockRes();
    
    metrics.get_userTimeRes();
    

    
    return 0;
}