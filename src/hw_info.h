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
	void _find_coretemp_sensor(); // Find hwmon with sensor name=coretemp

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

hw_info::hw_info()
{
	_find_coretemp_sensor();
	//File locations
	_file_cpu_info = "/proc/cpuinfo"; // No use
	_file_mem_info = "/proc/meminfo";
	_file_stats = "/proc/stat";

	// Regex
	_digits = regex("[0-9]+");
	_cpu = regex("[0-9]+");
}

void hw_info::refresh()
{
	ifstream _meminfo_file(_file_mem_info);
	ifstream _cpu_stat_file(_file_stats);
	ifstream _cpu_temp_file(_file_cpu_temp);

	// If any file reading fails throw exception
	if (_meminfo_file.fail() ||
		_cpu_stat_file.fail() ||
		_cpu_temp_file.fail())
	{
		cerr << "Failed to open one or more file:\n"
			 << _file_mem_info << ": " << _meminfo_file.is_open() << endl
			 << _file_stats << ": " << _cpu_stat_file.is_open() << endl
			 << _file_cpu_temp << ": " << _cpu_temp_file.is_open() << endl;
		throw std::runtime_error("One more file(s) cannot be opened");
	}

	// cout << "Total Mem: " << meminfo[0] << endl;
	// cout << "Free Mem: " << meminfo[1] << endl;
	// cout << "Cached: " << meminfo[2] << endl;
	//cout << "Cpu %: " << cpu[0] << endl;
	_meminfo_file.seekg(0);
	_cpu_stat_file.seekg(0);
	//regex_search(total_mem, m, r);
}

void hw_info::_find_coretemp_sensor()
{
	string hwmon_dir = "/sys/class/hwmon/";
	int i = 0;
	while (true)
	{
		string tmp = hwmon_dir;
		tmp += "hwmon";
		tmp += to_string(i);
		tmp += "/name";
		cout << "current file: " << tmp << endl;
		ifstream ifs;
		ifs.open(tmp);
		if (ifs.is_open())
		{
			string sen_name;
			ifs >> sen_name;
			cout << "sensor name: '" << sen_name << "'\n";
			if (sen_name == "coretemp")
			{
				ifs.close();
				// Save file location to
				_file_cpu_temp += hwmon_dir + to_string(i) + "/temp1_input";
				cout << "Found coretemp sensor at:"
					 << _file_cpu_temp << endl;
				break;
			}
			else
				cout << "Checking next file\n";
		}
		else
		{
			cout << "Coretemp not found in your system\n";
			ifs.close();
			break;
		}
		i++;
		ifs.close();
		if (i == 10)
			break;
	}
}