#include "hw_info.h"
hw_info::hw_info()
{
	//File locations
	_file_cpu_info = "/proc/cpuinfo"; // No use
	_file_mem_info = "/proc/meminfo";
	_file_stats = "/proc/stat";
	// _file_cpu_temp = "/sys/class/hwmon/hwmon3"; // For my PC this is coretemp (CPU temp)

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

	cout << "Total Mem: " << meminfo[0] << endl;
	cout << "Free Mem: " << meminfo[1] << endl;
	cout << "Cached: " << meminfo[2] << endl;
	//cout << "Cpu %: " << cpu[0] << endl;
	_meminfo_file.seekg(0);
	_cpu_stat_file.seekg(0);
	//regex_search(total_mem, m, r);
}

void hw_info::_find_coretemp_sensor()
{
    
}