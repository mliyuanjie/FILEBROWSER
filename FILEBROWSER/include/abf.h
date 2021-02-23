#ifndef ABF_H
#define ABF_H

#include "abffiles.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <QtCore/qobject.h>
#include <QtCharts/qlineseries.h>


QT_CHARTS_USE_NAMESPACE

class ABF:public QObject {
	Q_OBJECT 
public:
	ABF(std::string f, QObject* parent = 0, unsigned int n = 1024 * 16);
	int Channel;
	int Sweep;
	float Interval;
	std::vector<float> data;

	void setSeries(QLineSeries* s);
	std::pair<float, float> getLimit();
	~ABF();
public slots:
	void readData(int channel = 0, int sweep = 1, bool m = true);
	void save(std::vector<unsigned int>& start, std::vector<unsigned int>& end);
	void draw(int s, int e);

private:
	typedef int(_stdcall* pABF_ReadOpen)(const char* szFileName, int* phFile, UINT uFlags, ABFFileHeader* pFH, UINT* puMaxSamples, DWORD* pdwMaxEpi, int* pnError);
	typedef int(_stdcall* pABF_ReadChannel)(int hFile, ABFFileHeader* pFH, int nChannel, unsigned long dwEpisode, float* pfBuffer, unsigned int* puNumsAMPLES, int* pnError);
	typedef int(_stdcall* pABF_Close)(int hFile, int* pnError);
	typedef int(_stdcall* pABF_GetWaveform)(int hFile, ABFFileHeader* pFH, int nChannel, unsigned long dwEpisode, float* pfBuffer, int* pnError);
	typedef int(_stdcall* pABF_WriteOpen)(const char* szFileName, int* phFile, UINT uFlags, ABFFileHeader* pFH, int* pnError);
	typedef int(_stdcall* pABF_MultiplexWrite)(int hFile, ABFFileHeader* pFH, UINT uFlags, void* pvBuffer, DWORD dwEpiStart, UINT uNumSamples, int* pnError);
	typedef int(_stdcall* pABF_WriteDACFileEpi)(int hFile, ABFFileHeader* pFH, short* pnDACArray, int* pnError);
	typedef int(_stdcall* pABF_MultiplexRead)(int hFile, ABFFileHeader* pFH, DWORD dwEpisode, void* pvBuffer, UINT* puNumSamples, int* pnError);
	typedef int(_stdcall* pABF_ReadDACFileEpi)(int hFile, ABFFileHeader* pFH, short* pnDACArray, DWORD dwEpisode, int* pnError);
	typedef int(_stdcall* pABF_UpdateHeader)(int hFile, ABFFileHeader* pFH, int* pnError);      
	typedef int(_stdcall* pABF_SynchCountFromEpisode)(int hFile, const ABFFileHeader* pFH, DWORD dwEpisode, DWORD* pdwSynchCount, int* pnError);


	ABFFileHeader fh;
	std::string fn;
	int hfile;
	int error;
	DWORD synstart;
	unsigned int maxsamples;
	unsigned long maxepi;
	HINSTANCE module;
	QLineSeries* series;
	bool filetype;

	pABF_ReadOpen ABF_ReadOpen;
	pABF_ReadChannel ABF_ReadChannel;
	pABF_Close ABF_Close;
	pABF_GetWaveform ABF_GetWaveform;
	pABF_WriteOpen ABF_WriteOpen;
	pABF_MultiplexWrite ABF_MultiplexWrite;
	pABF_WriteDACFileEpi ABF_WriteDACFileEpi;
	pABF_UpdateHeader ABF_UpdateHeader;
	pABF_SynchCountFromEpisode ABF_SynchCountFromEpisode;
	pABF_MultiplexRead ABF_MultiplexRead;
	pABF_ReadDACFileEpi ABF_ReadDACFileEpi;
};


#endif //ABF_H