#ifndef NPS_H
#define NPS_H

#include <vector>
#include <string>
#include <unordered_map>
#include <QtCore/qobject.h>

struct Peak {
	unsigned int start;
	unsigned int end;
	float Imax;
	float Imin;
	float Ib;
	bool eventtype;
	float volume;
	float shape;
};

class NPS : QObject {
public:
	NPS(std::string&);

public slots:
	void trace(int, int);
	void hist(int);
	void setCounter(int);
	void multiFit();
	void singleFit();
	void save();
	void setPhysics(float, float, float, float, float);
private:
	std::vector<std::string> filelist;
	std::vector<Peak> siglist;
	std::unordered_map<std::string, std::vector<int>> datatosig;
	std::unordered_map<int, int> sigtodata;
	std::vector<float> data;
	int counter;




};

#endif 