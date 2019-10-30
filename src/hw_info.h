#include "Linux-Client/src/thinger/thinger.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <thread>
#include <vector>
#include <regex>
#include <chrono>
#include <sstream>
#include "credential.c"
using namespace std;

// Class hw_info

class hw_info
{
	//Memory Related
	regex _digits;

	//CPU Related
	regex _cpu;

	// Disk Related
	regex _disk_size_total;
	vector<int> disk_free;
	vector<int> disk_usage;

	//File locations
	string _file_cpu_info;
	string _file_mem_info;
	string _file_stats;
	string _file_disk;
	string _file_cpu_temp;

    // Functions
    void _find_coretemp_sensor(); // Find hwmon with name=coretemp


public:
	// 0: Total Mem
	// 1: Free Mem
	// 3: Cached
	// 4: Swap Total
	// 5: Swap Free
	int meminfo[6];
	// 0: Cpu usage in %
	float cpu[4] = {0, 0, 0, 0};

	//index
	enum
	{
		mem_total_i = 0,
		mem_free_i = 1,
		cached_i = 3,
		swap_total_i = 13,
		swap_free_i = 14
	};

	void refresh();
	hw_info();

} hw_stat;


