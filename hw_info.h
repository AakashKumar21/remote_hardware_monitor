#include <regex>
using namespace std;

class hw_info
{
	//Memory Related
	regex _digits;

	//CPU Related
	regex _cpu_usage_avg;
	regex _cpu_freq_avg;
	regex _process_total;
	regex _cpu_temp;

	// Disk Related
	regex _disk_size_total;
	vector<int> disk_free;
	vector<int> disk_usage;

	struct cpu_info;
	struct disk_info;
	struct cpu_info;
	struct temp_info;

public:
	// Functions
	void refresh();

}hw_stat;
