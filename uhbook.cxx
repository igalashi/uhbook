/*
 *
 */

#ifndef UHBOOK_CXX
#define UHBOOK_CXX

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

class UHnBook {
public:
	virtual ~UHnBook(){};
	void SetTitle(std::string &title) {m_title = title;};
	void SetTitle(const char *ctitle) {m_title = std::string(ctitle);};
	std::string& GetTitle() {return m_title;};

	void SetXLabel(std::string &label) {m_x_label = label;};
	void SetXLabel(const char *clabel) {m_x_label = std::string(clabel);};
	std::string& GetXLabel() {return m_x_label;};
	void SetYLabel(std::string &label) {m_y_label = label;};
	void SetYLabel(const char *clabel) {m_y_label = std::string(clabel);};
	std::string& GetYLabel() {return m_y_label;};

	int GetEntries() {return m_entry;};
protected:
	std::string m_title;
	std::string m_x_label;
	std::string m_y_label;
	int m_entry = 0;
private:
};

class UH1Book : public UHnBook {
public:
	UH1Book();	
	UH1Book(const char *, int, double, double);	
	UH1Book(std::string &, int, double, double);	
	virtual ~UH1Book();
	void Fill(double, double);
	void Fill(int val, double weight = 1.0)
		{Fill(static_cast<double>(val), weight);};
	void Fill(int val, int iweight = 1)
		{Fill(static_cast<double>(val), static_cast<double>(iweight));}
	void Reset();

#if 0
	void SetTitle(std::string &title) {m_title = title;};
	void SetTitle(char *ctitle) {m_title = std::string(ctitle);};
	std::string& GetTitle() {return m_title;};

	void SetXLabel(std::string &label) {m_x_label = label;};
	void SetXLabel(char *clabel) {m_x_label = std::string(clabel);};
	std::string& GetXLabel() {return m_x_label;};
	void SetYLabel(std::string &label) {m_y_label = label;};
	void SetYLabel(char *clabel) {m_y_label = std::string(clabel);};
	std::string& GetYLabel() {return m_y_label;};

	int GetEntries() {return m_entry;};
#endif

	void SetNbins(int bins) {if (m_entry == 0) {m_x_bins.resize(bins);}};
	int GetNbins() {return m_x_bins.size();};
	int GetBins() {return m_x_bins.size();};
	void SetMinimum(double min) {if (m_entry == 0) {m_x_min = min;}};
	double GetMinimum() {return m_x_min;};
	void SetMaximum(double max) {if (m_entry == 0) {m_x_max = max;}};
	double GetMaximum() {return m_x_max;};

	void SetBinContents(int ibin, double val) {m_x_bins[ibin] = val;};
	double GetBinContents(int ibin) {return m_x_bins[ibin];};
	std::vector<double>& GetBinContents() {return m_x_bins;};

	void Print();
	void Draw();
protected:
private:
	void Initialize(std::string &, int);
	void Initialize();

	std::vector<double> m_x_bins;
	double m_x_min;
	double m_x_max;

#if 0
	std::string m_title;
	std::string m_x_label;
	std::string m_y_label;

	int m_entry = 0;
#endif
	int m_uf = 0;
	int m_of = 0; 
};


UH1Book::UH1Book()
{
	return;
}

UH1Book::UH1Book(const char *title, int bins, double min, double max)
	: m_x_min(min), m_x_max(max)
{
	std::string stitle(title);
	Initialize(stitle, bins);

	return;
}

UH1Book::UH1Book(std::string &title, int bins, double min, double max)
	: m_x_min(min), m_x_max(max)
{
	Initialize(title, bins);

	return;
}

void UH1Book::Initialize(std::string &title, int bins = 0)
{
	m_title = title;
	m_x_bins.clear();
	m_x_bins.resize(bins);
	m_entry = 0;
	m_uf = 0;
	m_of = 0; 

	return;
}

void UH1Book::Initialize()
{
	m_x_bins.clear();
	m_x_bins.resize(0);
	m_entry = 0;
	m_uf = 0;
	m_of = 0; 

	return;
}

UH1Book::~UH1Book()
{
	return;
}

void UH1Book::Reset()
{
	//for (auto &i : m_x_bins )  i = 0.0;
	std::fill(m_x_bins.begin(), m_x_bins.end(), 0.0);
	m_entry = 0;
	m_uf = 0;
	m_of = 0; 

	return;
};

void UH1Book::Fill(double val, double weight = 1.0)
{
	if (val < m_x_min) m_uf++;
	if (val >= m_x_max) m_of++;
	if ((val >= m_x_min) && (val < m_x_max)) {
		int index = static_cast<int>(
			(val - m_x_min) / (m_x_max - m_x_min) * m_x_bins.size());
		//std::cout << "#D3 " << index << std::endl;
		m_x_bins[index] += weight;
		m_entry++;
	}

	return;
}

void UH1Book::Print()
{
	std::cout << "Title: " << m_title << std::endl;
	std::cout << "Entry:      " << m_entry << std::endl;
	std::cout << "Over flow:  " << m_of << std::endl;
	std::cout << "Under flow: " << m_uf << std::endl;

	return;
}


#include <sys/ioctl.h>
#include <unistd.h>

size_t get_terminal_width()
{
	size_t line_length = 0;
	struct winsize ws;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != -1 ) {
		printf("terminal_width  =%d\n", ws.ws_col);
		printf("terminal_height =%d\n", ws.ws_row);
		if ((ws.ws_col > 0) && (ws.ws_col == (size_t)ws.ws_col)) {
			line_length = ws.ws_col;
		}
	} 
	return line_length;
}

void UH1Book::Draw()
{
	int tlen = 100;
	int hlen = tlen - 8 - 8;

	double vmax = 0;
	for (size_t i = 0 ; i < m_x_bins.size() ; i++) {
		if (vmax < m_x_bins[i]) vmax = m_x_bins[i];
	}

	for (size_t i = 0 ; i < m_x_bins.size() ; i++) {
		int dnum;
		if (vmax < hlen) {
			dnum = static_cast<int>(m_x_bins[i]);
		} else {
			dnum = m_x_bins[i] / vmax * hlen;
		}
		double xindex = ((m_x_max - m_x_min) / m_x_bins.size() * i) + m_x_min;
		std::cout
			<< std::scientific << std::setprecision(1) 
			<< xindex << ":"
			<< m_x_bins[i] << "|";
		for (int j = 0 ; j < dnum ; j++) std::cout << "#";
		std::cout << std::endl;
	}

	std::cout << std::defaultfloat;

	return;
}


class UH2Book : public UHnBook {
public:
	UH2Book();
	UH2Book(const char *, int, double, double, int, double, double);	
	UH2Book(std::string &, int, double, double, int, double, double);	
	virtual ~UH2Book();
	void Fill(double, double, double);
	void Fill(int xval, int yval, double weight = 1.0)
		{Fill(static_cast<double>(xval), static_cast<double>(yval), weight);};
	void Fill(int xval, int yval, int iweight = 1)
		{Fill(static_cast<double>(xval),
			static_cast<double>(yval), static_cast<double>(iweight));}
	void Reset();

	void SetNbins(int xbins, int ybins) {
		if (m_entry == 0) {
			m_bins.resize(xbins);
			for (auto &i : m_bins) i.resize(ybins);
		}
	};
	int GetNBinsX() {return m_bins[0].size();};
	int GetNBinsY() {return m_bins.size();};

	void SetMinX(double min) {if (m_entry == 0) {m_x_min = min;}};
	double GetMinX() {return m_x_min;};
	void SetMinY(double min) {if (m_entry == 0) {m_y_min = min;}};
	double GetMinY() {return m_y_min;};
	void SetMaxX(double max) {if (m_entry == 0) {m_x_max = max;}};
	double GetMaxX() {return m_x_max;};
	void SetMaxY(double max) {if (m_entry == 0) {m_y_max = max;}};
	double GetMaxY() {return m_y_max;};

	void SetBinContents(int xbin, int ybin, double val) {m_bins[xbin][ybin] = val;};
	double GetBinContents(int xbin, int ybin) {return m_bins[xbin][ybin];};
	std::vector< std::vector<double> >& GetBinContents() {return m_bins;};

	void Print();
	void Draw();
protected:
private:
	void Initialize(std::string &, int, int);
	void Initialize();

	std::vector< std::vector<double> > m_bins;
	double m_x_min;
	double m_x_max;
	double m_y_min;
	double m_y_max;

	//int m_ouflow[3][3] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	std::array<std::array<int, 3>, 3> m_ouflow = {0, 0, 0, 0, 0, 0, 0, 0, 0};

};

UH2Book::UH2Book()
{
	return;
}

UH2Book::UH2Book(const char *title,
	int xbins, double xmin, double xmax,
	int ybins, double ymin, double ymax)
	: m_x_min(xmin), m_x_max(xmax), m_y_min(ymin), m_y_max(ymax)
{
	std::string stitle(title);
	Initialize(stitle, xbins, ybins);

	return;
}

UH2Book::UH2Book(std::string &title,
	int xbins, double xmin, double xmax,
	int ybins, double ymin, double ymax)
	: m_x_min(xmin), m_x_max(xmax), m_y_min(ymin), m_y_max(ymax)
{
	Initialize(title, xbins, ybins);

	return;
}

void UH2Book::Initialize(std::string &title, int xbins = 0, int ybins = 0)
{
	m_title = title;
	m_bins.clear();
	m_bins.resize(xbins);
	for (auto &i : m_bins) i.resize(ybins);
	m_entry = 0;
	//m_ouflow = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	for (auto &i : m_ouflow) std::fill(i.begin(), i.end(), 0);

	return;
}

void UH2Book::Initialize()
{
	m_bins.clear();
	m_bins.resize(0);
	m_entry = 0;
	//m_ouflow = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	for (auto &i : m_ouflow) std::fill(i.begin(), i.end(), 0);

	return;
}

UH2Book::~UH2Book()
{
	return;
}

void UH2Book::Reset()
{
	for (auto &i : m_bins) std::fill(i.begin(), i.end(), 0.0);
	m_entry = 0;
	for (auto &i : m_ouflow) std::fill(i.begin(), i.end(), 0);
	
	return;
};

void UH2Book::Fill(double xval, double yval, double weight = 1.0)
{
	if ((xval <  m_x_min)                      && (yval <  m_y_min)) m_ouflow[0][0]++;
	if ((xval >= m_x_min) && (xval <  m_x_max) && (yval <  m_y_min)) m_ouflow[1][0]++;
	if ((xval >= m_x_max)                      && (yval <  m_y_min)) m_ouflow[2][0]++;

	if ((xval <  m_x_min) && (yval >= m_y_min) && (yval <  m_y_max)) m_ouflow[0][1]++;
	if ((xval >= m_x_max) && (yval >= m_y_min) && (yval <  m_y_max)) m_ouflow[2][1]++;

	if ((xval <  m_x_min)                      && (yval >= m_y_max)) m_ouflow[0][2]++;
	if ((xval >= m_x_max) && (xval <  m_x_max) && (yval >= m_y_max)) m_ouflow[1][2]++;
	if ((xval >= m_x_max)                      && (yval >= m_y_max)) m_ouflow[2][2]++;

	if (       (xval >= m_x_min) && (xval < m_x_max) 
		&& (yval >= m_y_min) && (yval < m_x_max)) {
		int ix = static_cast<int>(
			(xval - m_x_min) / (m_x_max - m_x_min) * m_bins.size());
		int iy = static_cast<int>(
			(yval - m_y_min) / (m_y_max - m_y_min) * m_bins.size());
		m_bins[ix][iy] += weight;
		m_entry++;
	} else {
	}

	return;
}

void UH2Book::Print()
{
	std::cout << "Title: " << m_title << std::endl;
	std::cout << "Entry: " << m_entry << std::endl;
	std::cout << "Over/Under flow:  "
		<< std::setw(6) << m_ouflow[0][2] << " "
		<< std::setw(6) << m_ouflow[1][2] << " "
		<< std::setw(6) << m_ouflow[2][2] << std::endl;
	std::cout << "               :  "
		<< std::setw(6) << m_ouflow[0][1] << " "
		<< std::setw(6) << m_entry << " "
		<< std::setw(6) << m_ouflow[2][1] << std::endl;
	std::cout << "               :  "
		<< std::setw(6) << m_ouflow[0][0] << " "
		<< std::setw(6) << m_ouflow[1][0] << " "
		<< std::setw(6) << m_ouflow[2][0] << std::endl;

	return;
}

void UH2Book::Draw()
{
	const int ngrade = 8;
	std::array<char, ngrade> dispchar = {
		' ', '.', '-', '+',
		'x', '*', '@', '#'};

	double vmax = 0;
	for (auto &i : m_bins) {
		for (auto &j : i) {
			if (vmax < j) vmax = j;
		}
	}
	if (vmax == 0.0) vmax = 1.0;

	for (size_t i = 0 ; i < m_bins.size() ; i++) {
		double xindex = ((m_x_max - m_x_min) / m_bins.size() * i) + m_x_min;
		std::cout << std::scientific << std::setprecision(1) 
			<< xindex << "|";
		for (size_t j = 0 ; j < m_bins[i].size() ; j++) {
			char v[2] = {0, 0};
			int dnum = static_cast<int>((m_bins[i][j] / vmax) * ngrade);
			v[0] = dispchar[dnum];
			std::cout << v;
		}
		std::cout << std::endl;
	}

	std::cout << std::defaultfloat;

	return;
}

#endif // UHBOOK_CXX


#ifdef TEST_MAIN 
#include <random>
#include <sstream>
int main(int argc, char* argv[])
{
	int nentry = 100;
	for (int i = 1 ; i < argc ; i++) {
		//std::string param(argv[i]);
		std::istringstream iss(argv[i]);
		iss >> nentry;
	}


	UH1Book h1("Hello", 30, 0.0, 200.0);

	//std::string title("Hello");
	//UH1Book h1(title, 100, 2.0, 500.0);
	//std::cout << "#D " << h1.getMin() << " " << h1.getMax()
	//	<< " " << h1.getBins() << std::endl;

	std::random_device rnd;
	std::mt19937 engine(rnd());
	std::normal_distribution<> dist(100.0, 20.0);
	for (int i = 0 ; i < nentry ; i++) {
		double val = dist(engine);
		h1.Fill(val);
	}

	h1.Print();
	h1.Draw();

	h1.Reset();
	for (int i = 0 ; i < nentry ; i++) {
		double val = dist(engine);
		h1.Fill(val);
	}
	h1.Print();
	h1.Draw();

	std::vector<double>& hcont = h1.GetBinContents();

	std::cout << "Contents:";
	for (auto &i : hcont) std::cout << " " << i;
	std::cout << std::endl;
	hcont[5] = 20.0;
	h1.Print();
	h1.Draw();


	UH2Book h2("Hello 2D", 40, 0.0, 200.0, 40, 0.0, 200.0);
	for (int i = 0 ; i < nentry * nentry ; i++) {
		double xval = dist(engine);
		double yval = dist(engine);
		h2.Fill(xval, yval);
	}
	h2.Print();
	h2.Draw();

	UH2Book h2d = h2;

	h2d.SetTitle("copyed Hello 2D");
	h2d.Print();
	h2d.Draw();

	return 0;
}
#endif
