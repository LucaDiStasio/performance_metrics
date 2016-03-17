#include "perfmetrics.h"

//=====================  BODY  ==========================

//-----------------
// Constructors 
//-----------------

perfmetrics::perfmetrics(){}

//-----------------
//   Destructor 
//-----------------

perfmetrics::~perfmetrics(){}

//-----------------
// General tools 
//-----------------

// CPU

void perfmetrics::get_CPUthreads(){
    nthreads = thread::hardware_concurrency();
}

// RAM

void perfmetrics::get_systemRAM(){
    #if defined(_WIN32) && (defined(__CYGWIN__) || defined(__CYGWIN32__))
	/* Cygwin under Windows. ------------------------------------ */
	/* New 64-bit MEMORYSTATUSEX isn't available.  Use old 32.bit */
	MEMORYSTATUS status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatus( &status );
	memorySize = (size_t)status.dwTotalPhys;

    #elif defined(_WIN32)
    	/* Windows. ------------------------------------------------- */
    	/* Use new 64-bit MEMORYSTATUSEX, not old 32-bit MEMORYSTATUS */
    	MEMORYSTATUSEX status;
    	status.dwLength = sizeof(status);
    	GlobalMemoryStatusEx( &status );
    	memorySize = (size_t)status.ullTotalPhys;
    
    #elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
    	/* UNIX variants. ------------------------------------------- */
    	/* Prefer sysctl() over sysconf() except sysctl() HW_REALMEM and HW_PHYSMEM */
    
    #if defined(CTL_HW) && (defined(HW_MEMSIZE) || defined(HW_PHYSMEM64))
    	int mib[2];
    	mib[0] = CTL_HW;
    #if defined(HW_MEMSIZE)
    	mib[1] = HW_MEMSIZE;            /* OSX. --------------------- */
    #elif defined(HW_PHYSMEM64)
    	mib[1] = HW_PHYSMEM64;          /* NetBSD, OpenBSD. --------- */
    #endif
    	int64_t size = 0;               /* 64-bit */
    	size_t len = sizeof( size );
    	if ( sysctl( mib, 2, &size, &len, NULL, 0 ) == 0 )
    		return (size_t)size;
    	memorySize = 0L;			/* Failed? */
    
    #elif defined(_SC_AIX_REALMEM)
    	/* AIX. ----------------------------------------------------- */
    	memorySize = (size_t)sysconf( _SC_AIX_REALMEM ) * (size_t)1024L;
    
    #elif defined(_SC_PHYS_PAGES) && defined(_SC_PAGESIZE)
    	/* FreeBSD, Linux, OpenBSD, and Solaris. -------------------- */
    	memorySize = (size_t)sysconf( _SC_PHYS_PAGES ) *
    		(size_t)sysconf( _SC_PAGESIZE );
    
    #elif defined(_SC_PHYS_PAGES) && defined(_SC_PAGE_SIZE)
    	/* Legacy. -------------------------------------------------- */
    	memorySize = (size_t)sysconf( _SC_PHYS_PAGES ) *
    		(size_t)sysconf( _SC_PAGE_SIZE );
    
    #elif defined(CTL_HW) && (defined(HW_PHYSMEM) || defined(HW_REALMEM))
    	/* DragonFly BSD, FreeBSD, NetBSD, OpenBSD, and OSX. -------- */
    	int mib[2];
    	mib[0] = CTL_HW;
    #if defined(HW_REALMEM)
    	mib[1] = HW_REALMEM;		/* FreeBSD. ----------------- */
    #elif defined(HW_PYSMEM)
    	mib[1] = HW_PHYSMEM;		/* Others. ------------------ */
    #endif
    	unsigned int size = 0;		/* 32-bit */
    	size_t len = sizeof( size );
    	if ( sysctl( mib, 2, &size, &len, NULL, 0 ) == 0 )
    		return (size_t)size;
    	memorySize = 0L;			/* Failed? */
    #endif /* sysctl and sysconf variants */
    
    #else
    	memorySize = 0L;			/* Unknown OS. */
    #endif
}

void perfmetrics::get_RAMusage(){
    #if defined(_WIN32)
    	/* Windows -------------------------------------------------- */
    	PROCESS_MEMORY_COUNTERS info;
    	GetProcessMemoryInfo( GetCurrentProcess( ), &info, sizeof(info) );
    	memoryUsage = (size_t)info.WorkingSetSize;
    
    #elif defined(__APPLE__) && defined(__MACH__)
    	/* OSX ------------------------------------------------------ */
    	struct mach_task_basic_info info;
    	mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
    	if ( task_info( mach_task_self( ), MACH_TASK_BASIC_INFO,
    		(task_info_t)&info, &infoCount ) != KERN_SUCCESS )
    		memoryUsage = (size_t)0L;		/* Can't access? */
    	memoryUsage = (size_t)info.resident_size;
    
    #elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
    	/* Linux ---------------------------------------------------- */
    	long rss = 0L;
    	FILE* fp = NULL;
    	if ( (fp = fopen( "/proc/self/statm", "r" )) == NULL )
    		memoryUsage = (size_t)0L;		/* Can't open? */
    	if ( fscanf( fp, "%*s%ld", &rss ) != 1 )
    	{
    		fclose( fp );
    		memoryUsage = (size_t)0L;		/* Can't read? */
    	}
    	fclose( fp );
    	memoryUsage = (size_t)rss * (size_t)sysconf( _SC_PAGESIZE);
    
    #else
    	/* AIX, BSD, Solaris, and Unknown OS ------------------------ */
    	memoryUsage = (size_t)0L;			/* Unsupported. */
    #endif
}

void perfmetrics::get_RAMpeak(){
    #if defined(_WIN32)
    	/* Windows -------------------------------------------------- */
    	PROCESS_MEMORY_COUNTERS info;
    	GetProcessMemoryInfo( GetCurrentProcess( ), &info, sizeof(info) );
    	memoryPeak = (size_t)info.PeakWorkingSetSize;
    
    #elif (defined(_AIX) || defined(__TOS__AIX__)) || (defined(__sun__) || defined(__sun) || defined(sun) && (defined(__SVR4) || defined(__svr4__)))
    	/* AIX and Solaris ------------------------------------------ */
    	struct psinfo psinfo;
    	int fd = -1;
    	if ( (fd = open( "/proc/self/psinfo", O_RDONLY )) == -1 )
    		memoryPeak = (size_t)0L;		/* Can't open? */
    	if ( read( fd, &psinfo, sizeof(psinfo) ) != sizeof(psinfo) )
    	{
    		close( fd );
    		memoryPeak = (size_t)0L;		/* Can't read? */
    	}
    	close( fd );
    	memoryPeak = (size_t)(psinfo.pr_rssize * 1024L);
    
    #elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
    	/* BSD, Linux, and OSX -------------------------------------- */
    	struct rusage rusage;
    	getrusage( RUSAGE_SELF, &rusage );
    #if defined(__APPLE__) && defined(__MACH__)
    	memoryPeak = (size_t)rusage.ru_maxrss;
    #else
    	memoryPeak = (size_t)(rusage.ru_maxrss * 1024L);
    #endif
    
    #else
    	/* Unknown OS ----------------------------------------------- */
    	memoryPeak = (size_t)0L;			/* Unsupported. */
    #endif
}

// Time

bool perfmetrics::is_ClockSteady(){
    return (boost::chrono::high_resolution_clock::is_steady ? true : false);
}

void perfmetrics::get_wallClockRes(){
    cpu_times start_time;
    start_time.clear();
    cpu_times current_time;
    cpu_timer cpu;
    for (int i = 0; i < 100; ++i)
    {
      cpu.start();
      start_time.wall = cpu.elapsed().wall;
      current_time.wall = start_time.wall;
      while (current_time.wall == start_time.wall)
      {
        current_time.wall = cpu.elapsed().wall;
      }
      cout << current_time.wall - start_time.wall << "ns ";
    }
}

void perfmetrics::get_userTimeRes(){
    cpu_times start_time;
    start_time.clear();
    cpu_times current_time;
    cpu_timer cpu;
    for (int i = 0; i < 3; ++i)
    {
      cpu.start();
      start_time = cpu.elapsed();
      current_time.user = start_time.user;
      while (current_time.user == start_time.user)
      {
        current_time = cpu.elapsed();
      }
      cout << current_time.user - start_time.user << "ns\n";
    }
}    
    
// Energy

//API

int perfmetrics::provide_CPUthreads(){
    return nthreads;
}

double perfmetrics::provide_systemRAM(double unit){
    if(unit!=0){
        return memorySize/(pow(10.0,unit));
    }else{
        return memorySize;
    }
}

double perfmetrics::provide_RAMusage(double unit){
    if(unit!=0){
        return memoryUsage/(pow(10.0,unit));
    }else{
        return memoryUsage;
    }
}

double perfmetrics::provide_RAMpeak(double unit){
    if(unit!=0){
        return memoryPeak/(pow(10.0,unit));
    }else{
        return memoryPeak;
    }
}