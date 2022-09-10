/*
 *
 */

#ifndef UHBOOK_HXX
#define UHBOOK_HXX

#include <iostream>
#include <vector>

class UH1Book1 {
public:
	UH1Book();	
	vertual ~UH1Book();
	UH1Book.Fill();
	UH1Book.Print();
	UH1Book.Draw();
procted:
private:
	std::vector<double> m_bins;
	std::string m_title;
	std::string m_xlabel;
	std::string m_ylabel;
};

#endif
