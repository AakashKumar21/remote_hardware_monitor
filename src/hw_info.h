#include "Linux-Client/src/thinger/thinger.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <thread>
// #include <vector>
// #include <regex>
// #include <chrono>
#include <sstream>
#include <thread>
#include <chrono>
#include "credential.c"
#include "cpu_usage.h"
using namespace std;


// #define DEBUG_ON
#ifdef DEBUG_ON 
#define LOG(x) std::cerr<<x<<endl;
#else 
#define LOG(x)
#endif

// Class hw_info
class hw_info
{

	//File locations
	string _file_disk;
	string _file_cpu_temp;

	const string _file_mem_info = "/proc/meminfo";
	const string _file_stats = "/proc/stat";
	// only for cpu0 fow now
	const string _file_cpu_freq = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq"; 
	// _file_cpu_temp is set by _find_coretemp_sensor()

	// Functions
	void _find_coretemp_sensor(); // Find hwmon with sensor name=coretemp
	// To get int from meminfo file lines (single lines)
	int _get_int(const string &str);	 
	void _get_mem_stat();
	void _get_cpu_temp();
	void _get_cpu_usage();
	void _get_cpu_freq();

public:
	// 0: Total Mem
	// 1: Free Mem
	// 2: Available mem
	// 3: Cached
	// 4: Swap Total
	// 5: Swap Free
	int meminfo[6];

	int cpu_temp;
	float cpu_usage;
	int cpu_freq;

	//index
	enum
	{
		MemTotal = 0,
		MemFree = 1,
		MemAvailable = 2,
		Cached = 3,
		SwapTotal = 14,
		SwapFree = 15
	};

	void refresh();
	hw_info();

} hw_stat;

hw_info::hw_info()
{
	LOG("Class init")
	_find_coretemp_sensor();
}

void hw_info::refresh()
{
	LOG("refresh called")
	_get_mem_stat();
	_get_cpu_temp();
	_get_cpu_usage();
	_get_cpu_freq();
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
				_file_cpu_temp += hwmon_dir + "hwmon" + to_string(i) + "/temp1_input";
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

int hw_info::_get_int(const string &str) 
{
	stringstream s(str);
	string x;
	int n;
	string nul; // To store type of mem stat in meminfo file eg
				// in "MemFree:         1480184 kB"
				// store "MemFree:"
	s >> nul;
	s >> x; // Store proceding int in string above string
			// i.e proceeding "MemFree:" i.e 1480184
	LOG("in func _get_int, x = ")
	LOG(x)
	n = stoi(x);
	return n;
}

void hw_info::_get_mem_stat()
{
	LOG("_get_mem_stat() Called")
	static ifstream mem_stat_file(_file_mem_info);
	string line;
	for (short int i = 0; i < SwapFree; i++)
	{
		getline(mem_stat_file, line);
		switch (i)
		{
		case MemTotal:
			meminfo[0] = _get_int(line);
			break;
		case MemFree:
			meminfo[1] = _get_int(line);
			break;
		case MemAvailable:
			meminfo[2] = _get_int(line);
			break;
		case Cached:
			meminfo[3] = _get_int(line);
			break;
		case SwapTotal:
			meminfo[4] = _get_int(line);
			break;
		case SwapFree:
			meminfo[5] = _get_int(line);
			break;
		}
	}
	mem_stat_file.seekg(0);
}

void hw_info::_get_cpu_temp()
{
	ifstream ifs(_file_cpu_temp); // TODO static or not
	string tmp;
	ifs >> tmp;
	LOG("in func _get_cpu_temp, str tmp=")
	LOG(tmp)
	cpu_temp = stoi(tmp);
	ifs.close();
}

void hw_info::_get_cpu_usage()
{
	cpu_usage = get_cpu_usage();
}

void hw_info::_get_cpu_freq()
{
	ifstream ifs(_file_cpu_freq);
	string freq; // tmp to hold cpu freq in str format
	getline(ifs,freq);
	// cout << freq;
	cpu_freq = stoi(freq);
	// cout << cpu_freq << endl; 
	ifs.close();
}