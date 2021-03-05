#include "nps.h" 

void NPS::load(std::string& fn) {
	std::ifstream file;
	file.open(fn);
	for (std::string i; std::getline(file, i);) {
		std::string i_sig = "csv";
		i_sig.insert(0,i,0,i.size()-3);
		std::ifstream file_sig(i_sig);
		if (file_sig.good()) {
			filelist.push_back(i);
			Peak sig = 
		}
	}
}