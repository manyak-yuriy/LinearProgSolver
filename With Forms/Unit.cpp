//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm3 *Form3;


#include <fstream>
#include <iostream>
#include <string>
#include <stdlib>
#include "tMatrix.h"
#include "simplex.h"
#include "other.h"
#include "Unit1.h"

using namespace std;

void __fastcall TForm3::Normalize()
{
   StringGrid->Width=(StringGrid->DefaultColWidth)*(StringGrid->ColCount)+ StringGrid->GridLineWidth*(1+StringGrid->ColCount);
   StringGrid->Height=(StringGrid->DefaultRowHeight)*(StringGrid->RowCount)+ StringGrid->GridLineWidth*(1+StringGrid->RowCount);

   StringGrid1->Height=(StringGrid1->DefaultRowHeight)*(StringGrid1->RowCount)+ StringGrid1->GridLineWidth*(StringGrid1->RowCount);

   StringGrid2->Width=(StringGrid2->DefaultColWidth)*(StringGrid2->ColCount)+ StringGrid2->GridLineWidth*(1+StringGrid2->ColCount);
   StringGrid3->Height = (StringGrid3->DefaultRowHeight)*(StringGrid3->RowCount)+ StringGrid3->GridLineWidth*(StringGrid3->RowCount);;
}

__fastcall TForm3::TForm3(TComponent* Owner)
	: TForm(Owner)
{

}

void __fastcall TForm3::N1Click(TObject *Sender)
{
   Application->Terminate();
}

void __fastcall TForm3::FormCreate(TObject *Sender)
{
   Normalize();
   Label5->Caption="";
   Form3->Width=460;
}

void __fastcall TForm3::Edit1Change(TObject *Sender)
{
	int n = atoi(Edit1->Text.c_str());
	int m = atoi(Edit2->Text.c_str());
	StringGrid->RowCount = n;
	StringGrid1->RowCount = n;
	StringGrid2->ColCount = m;
	StringGrid->ColCount = m;
	StringGrid2->Height = 24;
	StringGrid3->RowCount = m;
	if (n <= 5 && m <= 6)
	   Normalize();
	else
	{
		StringGrid->Width = 250;
		StringGrid->Height = 150;

		StringGrid1->Height = 150;

		StringGrid2->Width = 250;
		StringGrid2->Height = 40;
		StringGrid3->Height = 150;
	}
	Image3->Left=StringGrid2->Left+StringGrid2->Width+3;
	Image1->Left=StringGrid->Left+StringGrid->Width+3;
	StringGrid1->Left=Image1->Left+Image1->Width+3;
	Form3->Width=460;
	Timer1->Enabled = false;
	Image1->Top=(StringGrid->Top + StringGrid->Height / 2) - Image1->Height / 4;
}

void __fastcall TForm3::Edit2Change(TObject *Sender)
{
	int n = atoi(Edit1->Text.c_str());
	int m = atoi(Edit2->Text.c_str());
	StringGrid->RowCount = n;
	StringGrid1->RowCount = n;
	StringGrid2->ColCount = m;
	StringGrid->ColCount = m;
	StringGrid2->Height = 24;
	StringGrid3->RowCount = m;
	if (n <= 5 && m <= 6)
	   Normalize();
	else
	{
		StringGrid->Width = 250;
		StringGrid->Height = 150;

		StringGrid1->Height = 150;

		StringGrid2->Width = 250;
		StringGrid2->Height = 40;
		StringGrid3->Height = 150;
	}
	Image3->Left=StringGrid2->Left+StringGrid2->Width+3;
	Image1->Left=StringGrid->Left+StringGrid->Width+3;
	StringGrid1->Left=Image1->Left+Image1->Width+3;
	Form3->Width=460;
	Timer1->Enabled = false;
	Image1->Top=(StringGrid->Top + StringGrid->Height / 2) - Image1->Height / 4;
}

void __fastcall TForm3::Button1Click(TObject *Sender)
{
   Timer1->Enabled=true;
   int n = atoi(Edit1->Text.c_str());
   int m = atoi(Edit2->Text.c_str());
   if (n > 0 && m > 0 && n < m)
   {
	  ofstream file("input.txt");
	  file << n << " " << m + 1 << endl;
	  for (int i = 0; i < n; i++)
	  {
		 for (int j = 0; j < m; j++)
		 {
			char *s = StringGrid->Cells[j][i].c_str();
			if (strcmp(s, "") == 0)
			   file << 0;
			else
			{
			   double d = atof(s);
			   if (d == 0 && !isdigit(s[0]))
				  return;
			   file << d;
			}
			file << " ";
		 }
		 char *s = StringGrid1->Cells[0][i].c_str();
		 if (strcmp(s, "") == 0)
			file << 0;
		 else
		 {
			double d = atof(s);
			if (d == 0 && !isdigit(s[0]))
		       return; 
			file << d;
		 }
		 file << endl;
      }
	  file.close();
   }
   char buf[100];
   int LOOKMAX = 1 - ComboBox1->ItemIndex;
   ofstream file("func.txt");
   file << m << endl;
   for (int j = 0; j < m; j++)
   {
	  char *s = StringGrid2->Cells[j][0].c_str();
	  if (strcmp(s, "") == 0)
		 file << 0;
	  else
	  {
		 double d = atof(s);
		 if (d == 0 && !isdigit(s[0]))
			return;
		 if (!LOOKMAX)
		    d = -d;
		 file << d;
	  }
	  file << " ";
   }
   file.close();

   try
   {
	  int var_cnt = 0;
	  int var[MAX];
	  tMatrix Sim(0, 0);
	  int n, m;
	  simplex(var, var_cnt, n, m, Sim);

	  Image4->Visible=true;
	  Image5->Visible=true;
	  StringGrid3->Visible=true;
	  Label6->Visible=true;
	  Label5->Caption="";
	  char s[100];
	  double res = Sim.at(n + 1, 1);
	  if (! LOOKMAX)
		 res = -res;
	  sprintf(s, "%.3lf", res);
	  Label6->Caption = s;
	  for (int var_ind = 1; var_ind <= var_cnt; var_ind++)
	  {
		 for (int i = 1; i <= var_cnt; i++)
			if (var[i] == var_ind)
			{
               if (i >= n + 1)
				  StringGrid3->Cells[0][var_ind - 1] = 0;
			   else
			   {
				  char s[100];
				  sprintf(s, "%.3lf", Sim.at(i, 1));
                  StringGrid3->Cells[0][var_ind - 1] = s;
			   }
			}
	  }
   }
   catch (enum error e)
   {
	  switch (e)
	  {
		 case UNLIM: Label5->Caption = "Цільова функція необмежена";
		 case INCOMP: Label5->Caption = "Система обмежень несумісна";
		 case ITER: Label5->Caption = "Цільова функція необмежена";
	  }
	  Image4->Visible=false;
	  Image5->Visible=false;
	  StringGrid3->Visible=false;
	  Label6->Visible=false;
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Timer1Timer(TObject *Sender)
{
   while (Form3->Width<740)
      Form3->Width += 3;
}
//---------------------------------------------------------------------------

void __fastcall TForm3::StringGridClick(TObject *Sender)
{
   Form3->Width=460;
   Timer1->Enabled = false;
}
//---------------------------------------------------------------------------


void __fastcall TForm3::N4Click(TObject *Sender)
{
   if (OpenDialog1->Execute(Form3->Handle))
   {
	  ifstream file(OpenDialog1->FileName.c_str());
	  OpenDialog1->InitialDir = OpenDialog1->FileName;
	  OpenDialog2->InitialDir = OpenDialog1->FileName;
	  int n, m;
	  file >> n >> m;
	  m--;
	  StringGrid->RowCount = n;
	  StringGrid->ColCount = m;
	  char s[100];
	  itoa(n, s, 10);
	  Edit1->Text=s;
	  itoa(m, s, 10);
	  Edit2->Text=s;
	  for (int i = 0; i < n; i++)
		 for (int j = 0; j < m + 1; j++)
		 {
		    char s[100];
			file >> s;
			if (j == m)
			   StringGrid1->Cells[0][i] = s;
			else
               StringGrid->Cells[j][i] = s;
		 }
	  file.close();
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::N5Click(TObject *Sender)
{
   if (OpenDialog2->Execute(Form3->Handle))
   {
	  ifstream file(OpenDialog2->FileName.c_str());
	  int m;
	  file >> m;
	  for (int j = 0; j < m; j++)
	  {
		 char s[100];
		 file >> s;
		 StringGrid2->Cells[j][0] = s;
	  }
	  file.close();
   }
}
//---------------------------------------------------------------------------
void __fastcall TForm3::N3Click(TObject *Sender)
{
   Form1->Show();
}
//---------------------------------------------------------------------------


