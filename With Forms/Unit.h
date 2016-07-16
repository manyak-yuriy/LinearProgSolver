//---------------------------------------------------------------------------

#ifndef UnitH
#define UnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <jpeg.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TForm3 : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TMenuItem *N1;
	TMenuItem *N2;
	TMenuItem *N3;
	TStatusBar *StatusBar1;
	TGroupBox *GroupBox1;
	TLabel *Label1;
	TLabel *Label2;
	TEdit *Edit1;
	TEdit *Edit2;
	TStringGrid *StringGrid;
	TGroupBox *GroupBox2;
	TLabel *Label3;
	TComboBox *ComboBox1;
	TLabel *Label4;
	TImage *Image1;
	TStringGrid *StringGrid1;
	TStringGrid *StringGrid2;
	TImage *Image2;
	TImage *Image3;
	TButton *Button1;
	TTimer *Timer1;
	TGroupBox *GroupBox3;
	TLabel *Label5;
	TImage *Image4;
	TStringGrid *StringGrid3;
	TImage *Image5;
	TLabel *Label6;
	TMenuItem *N4;
	TOpenDialog *OpenDialog1;
	TOpenDialog *OpenDialog2;
	TMenuItem *N5;
	void __fastcall N1Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Normalize();
	void __fastcall Edit1Change(TObject *Sender);
	void __fastcall Edit2Change(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall StringGridClick(TObject *Sender);
	void __fastcall N4Click(TObject *Sender);
	void __fastcall N5Click(TObject *Sender);
	void __fastcall N3Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm3(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm3 *Form3;
//---------------------------------------------------------------------------
#endif
