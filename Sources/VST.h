//---------------------------------------------------------------------------

#ifndef VSTH
#define VSTH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "VirtualTrees.hpp"
//---------------------------------------------------------------------------
class TVSTForm : public TForm
{
__published:	// IDE-managed Components
	TVirtualStringTree *VirtualStringTree;
	TButton *Delete;
	TButton *Clear;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall OnGetText(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
          TVSTTextType TextType, UnicodeString &CellText);
	void __fastcall DeleteClick(TObject *Sender);
	void __fastcall ClearClick(TObject *Sender);
	void __fastcall OnClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
	__fastcall TVSTForm(TComponent* Owner);
};
typedef struct {
	int id;
	UnicodeString name;
	UnicodeString job;
} employe;
//---------------------------------------------------------------------------
extern PACKAGE TVSTForm *VSTForm;
//---------------------------------------------------------------------------
#endif
