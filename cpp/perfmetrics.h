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

#ifndef PERFMETRICS_H
#define PERFMETRICS_H

    #if defined(_WIN32)
        #include <Windows.h>
        #include <psapi.h>
    #elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
        #include <unistd.h>
        #include <sys/resource.h>
        #include <sys/types.h>
        #include <sys/param.h>
        #if defined(BSD)
            #include <sys/sysctl.h>
        #endif
        #if defined(__APPLE__) && defined(__MACH__)
            #include <mach/mach.h>
        
        #elif (defined(_AIX) || defined(__TOS__AIX__)) || (defined(__sun__) || defined(__sun) || defined(sun) && (defined(__SVR4) || defined(__svr4__)))
            #include <fcntl.h>
            #include <procfs.h>
        
        #elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
            #include <stdio.h>
        #endif
    #endif

#include <boost/timer/timer.hpp>
#include <boost/chrono/chrono.hpp>
#include <cmath>
#include <complex>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <omp.h>
#include <random>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <thread>
#include <time.h>
#include <typeinfo>
#include <type_traits> // C++0x
//#include <tr1/type_traits> // C++03, use std::tr1
#include <vector>

using namespace std;

//============================================================================//
//============================================================================//
/*
                A class to measure performances of software
*/
//============================================================================//
//============================================================================//


//===================================================
//==================  HEADER  =======================
//===================================================

class perfmetrics {

    //===================================================  
    //                  Variables
    //===================================================
    private:

    // Control parameters


    // Output quantities
    size_t memorySize,                                                          // System RAM
           memoryUsage,                                                         // RAM use
           memoryPeak;                                                          // RAM peak use
           
    double wallClockTime,                                                       // Elapsed wall clock time
           userCPUTime,                                                         // User CPU time
           systemCPUTime,                                                       // System CPU time
           wallClockRes,                                                        // Wall clock time resolution
           userTimeRes;                                                         // User time resolution
    
    int nthreads;                                                               // Number of CPU threads

  
    //===================================================  
    //                      Methods
    //===================================================  
    public:
  
    // Constructor (default)
    perfmetrics();
  
  
    // Destructor
    ~perfmetrics();
  
    // General tools
    
    // CPU
    void get_CPUthreads();                                                      // Get CPU threads
    
    // RAM
    void get_systemRAM();                                                       // Measure system RAM
    void get_RAMusage();                                                        // Measure RAM use
    void get_RAMpeak();                                                         // Measure peak RAM use
    
    // Time
    bool is_ClockSteady();                                                      // Check if system clock is steady.
                                                                                // Steady clocks are defined by C++11 as clocks for which values
                                                                                // of time_point never decrease as physical time advances and for
                                                                                // which values of time_point advance at a steady rate relative to
                                                                                // real time. That is, the clock may not be adjusted.
    void get_wallClockRes();                                                    // Measure wall clock time resolution
    void get_userTimeRes();                                                     // Measure user time resolution
    
    
    // Energy
    
    //API
    int provide_CPUthreads();                                                   // Provide number of CPU threads
    
    double provide_systemRAM(double unit = 0.0);                                // Provide size of system RAM in (10^unit)bytes, i.e.
                                                                                // unit = 0  -> B   (bytes)
                                                                                // unit = 3  -> KB  (kilobytes)
                                                                                // unit = 6  -> MB  (megabytes)
                                                                                // unit = 9  -> GB  (gigabytes)
                                                                                // unit = 12 -> TB  (terabytes)
                                                                                // unit = 15 -> PB  (petabytes)
                                                                                // unit = 18 -> EB  (terabytes)
    double provide_RAMusage(double unit = 0.0);                                 // Provide RAM use in (10^unit)bytes, i.e.
                                                                                // unit = 0  -> B   (bytes)
                                                                                // unit = 3  -> KB  (kilobytes)
                                                                                // unit = 6  -> MB  (megabytes)
                                                                                // unit = 9  -> GB  (gigabytes)
                                                                                // unit = 12 -> TB  (terabytes)
                                                                                // unit = 15 -> PB  (petabytes)
                                                                                // unit = 18 -> EB  (terabytes)
    double provide_RAMpeak(double unit = 0.0);                                  // Provide peak RAM use in (10^unit)bytes, i.e.
                                                                                // unit = 0  -> B   (bytes)
                                                                                // unit = 3  -> KB  (kilobytes)
                                                                                // unit = 6  -> MB  (megabytes)
                                                                                // unit = 9  -> GB  (gigabytes)
                                                                                // unit = 12 -> TB  (terabytes)
                                                                                // unit = 15 -> PB  (petabytes)
                                                                                // unit = 18 -> EB  (terabytes)
    double provide_wallClockRes();                                              // Provide wall clock time resolution
    double provide_userTimeRes();                                              // Provide user time resolution
  
    // Overloading of operators
  
  
};

#endif