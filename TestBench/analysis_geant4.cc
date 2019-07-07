void analysis_geant4()
{
    Int_t nbinsx = 50;
    Double_t xlow = 0.;
    Double_t xup = 1500;

    TFile *_file = TFile::Open("NGTHEP.root");
    TTree *tree = (TTree*)_file->Get("NGTHEP");

    TCanvas *c = new TCanvas( "c" , "c" );
    TH1D *h = new TH1D("h", "Geant4 histgram;Energy [keV];Entries", nbinsx, xlow, xup);
    
    tree->Draw( "Edep_br * 1000 >> h" , "Edep_br > 0." );
    h->Draw();

    //gStyle->SetStatW(0.3);
    //gStyle->SetOptStat(2210);
    //gStyle->SetOptStat(221112211);

    c->Print("NGTHEP.png");
}
