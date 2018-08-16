#include "TTree.h"
#include "TH2D.h"

//Singleton class which manages output to ROOT files


class MyROOTManager
{
	

	private:
	
	MyROOTManager(); 					
	virtual ~MyROOTManager();
	TTree* tree;
	TH2D* image;
	


	static MyROOTManager* theInstance;
	


	static int Inversion;							//этой "глобальной" переменной здесь не место, но я индус. 

	public:
	static MyROOTManager* GetPointer();	
	void Initialize();
	void Finalize();

	void SetInversion(int);

	void FillHist(Double_t, Double_t);
	void FillTree(Double_t, Double_t, Double_t, Double_t);

	
	
	
	
};
	

