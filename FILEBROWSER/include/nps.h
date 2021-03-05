#ifndef NPS_H
#define NPS_H

#include <vector>
#include <string>
#include <unordered_map>
#include <QtCore/qobject.h>
#include <QtCore/qvector.h>
#include <fstream>

struct Peak {
	bool eventtype;
	unsigned int start;
	unsigned int end;
	float currentmax;
	float currentbase;
	float Imax;
	float Imin;
	float volume;
	float shape;	
};

class NPS: QObject {
	Q_OBJECT;	

public slots:
	void trace(int, int);
	void hist(int);
	void setCounter(int);
	void multiFit();
	void singleFit();
	void save();
	void setPhysics(float, float, float, float, float);
	void load(std::string&);

signals:
	void sendtrace(QVector<QPointF>);
	void sendhist(QVector<QPointF>);
	void sendfit(QVector<QPointF>);

private:
	std::vector<std::string> filelist;
	std::vector<Peak> siglist;
	std::unordered_map<std::string, std::vector<int>> datatosig;
	std::unordered_map<int, int> sigtodata;
	std::vector<float> data;
	int counter = -1;
};

#endif 