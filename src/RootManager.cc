#include "RootManager.hh"
#include "TFile.h"

MyROOTManager* MyROOTManager::theInstance = 0;		//initialize "global" variables
int MyROOTManager::Inversion = 0;


MyROOTManager::MyROOTManager()
{}

MyROOTManager::~MyROOTManager()
{}

MyROOTManager* MyROOTManager::GetPointer()	
{
	if (!theInstance) theInstance = new  MyROOTManager(); 
	return theInstance;
}

void MyROOTManager::Initialize()
{
	
	//create output ROOT data objects

	Double_t a,b,c,d;
	tree = new TTree("MyTree", "Timepix hits");
	tree -> Branch("POSX", &a);
	tree -> Branch("POSY", &b);
	tree -> Branch("EDEP", &c);
	tree -> Branch("EKIN", &d);

	image = new TH2D("image", "Pixels", 256, -7, 7, 256, -7, 7);

}



	// fill methods to call after each event

void MyROOTManager::FillHist(Double_t x, Double_t y)
{
	image -> Fill(x, y);	
}	

void MyROOTManager::FillTree(Double_t x, Double_t y, Double_t e, Double_t k)
{
	tree -> SetBranchAddress("POSX", &x);
	tree -> SetBranchAddress("POSY", &y);
	tree -> SetBranchAddress("EDEP", &e);
	tree -> SetBranchAddress("EKIN", &k);

	tree -> Fill();
}	

void MyROOTManager::SetInversion(int x)
{
	Inversion = x;
	if (Inversion) printf("Mask inversion ON\n");
	else	printf("Mask inversion OFF\n");
}



void MyROOTManager::Finalize()
{
	TFile* f;
	if (Inversion == 0) 
	{
		f = new TFile("./SPECT.root", "recreate");
		printf("Results saved to SPECT.root\n");
	}	
	else 
	{
		f = new TFile("./SPECT_INV.root", "recreate");
		printf("Results saved to SPECT_INV.root\n");	
	}

	f->cd();
	tree->Write();
	image->Write();
	f -> Close();
}
	


