﻿//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "VST.h"
#include "sqlite3.h"
#include <string>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma resource "*.dfm"
TVSTForm *VSTForm;
sqlite3 *database;
void errorMessage(sqlite3 *database, sqlite3_stmt *pStatement, const wchar_t *message)
{
Application->MessageBox(message, L"Warning", MB_OK);
sqlite3_close(database);
if(!pStatement)
	sqlite3_finalize(pStatement);
}
//---------------------------------------------------------------------------
__fastcall TVSTForm::TVSTForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TVSTForm::FormCreate(TObject *Sender)
{
const char *databasePath = "..\\..\\Sources\\oracle.db";
int result = sqlite3_open(databasePath, &database);
const char *empSelect = "select * from emp";
sqlite3_stmt *pState, *pStatement;
result = sqlite3_prepare_v2(database, empSelect, -1, &pStatement, NULL);
if (result != SQLITE_OK)
{
	errorMessage(database, pStatement, L"Could not execute sqlite select query.");
	return;
}
const char *rowCount = "select count(*) from emp;";
int rowsNumber = 0;
result = sqlite3_prepare_v2(database, rowCount, -1, &pState, NULL);
if (result != SQLITE_OK)
{
	errorMessage(database, pStatement, L"Could not execute sqlite count query.");
	return;
}
result = sqlite3_step(pState);
if (result == SQLITE_ROW)
{
	rowsNumber = sqlite3_column_int(pState, 0);
}
else
{
	errorMessage(database, pStatement, L"Could not execute sqlite table row number query.");
    return;
}
sqlite3_finalize(pState);
VirtualStringTree->NodeDataSize = sizeof(employe);
VirtualStringTree->BeginUpdate();
for (int i = 0; i < rowsNumber; i++) {
	PVirtualNode currentNode = VirtualStringTree->AddChild(VirtualStringTree->RootNode);
	employe *data = (employe *) VirtualStringTree->GetNodeData(currentNode);
	result = sqlite3_step(pStatement);
	if (result == SQLITE_ROW)
	{
		data->id = sqlite3_column_int(pStatement, 0);
		data->name = (UnicodeString)(char *)sqlite3_column_text(pStatement, 1);
		data->job = (UnicodeString)(char *)sqlite3_column_text(pStatement, 2);
	}
}
VirtualStringTree->EndUpdate();
sqlite3_finalize(pStatement);
}
//---------------------------------------------------------------------------
void __fastcall TVSTForm::OnGetText(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
          TVSTTextType TextType, UnicodeString &CellText)
{
if(!Node) return;
employe *data = (employe *)Sender->GetNodeData(Node);
switch(Column)
{
	case 0:
		CellText = data->id;
		break;
	case 1:
		CellText = data->name;
		break;
	case 2:
		CellText = data->job;
		break;
}
}
//---------------------------------------------------------------------------
void __fastcall TVSTForm::DeleteClick(TObject *Sender)
{
if (!VirtualStringTree->SelectedCount) {
	return;
}
sqlite3_stmt *pStatement;
PVirtualNode selected = VirtualStringTree->GetFirstSelected();
employe *data = (employe *)VirtualStringTree->GetNodeData(selected);
int row = data->id;
char *message = NULL;
for (int i = 0; i < VirtualStringTree->SelectedCount - 1; i++) {
	selected = VirtualStringTree->GetNextSelected(selected);
	employe *data = (employe *)VirtualStringTree->GetNodeData(selected);
	std::string empDeleteRow = "delete from emp where empno = ";
	empDeleteRow += std::to_string(row);
	int result = sqlite3_exec(database, empDeleteRow.c_str(), NULL, NULL, &message);
	if( result != SQLITE_OK ) {
		errorMessage(database, NULL, L"Could not perform delete operation.");
		return;
	}
    row = data->id;
}
VirtualStringTree->DeleteSelectedNodes();
}
//----------------------------------------------------e-----------------------
void __fastcall TVSTForm::ClearClick(TObject *Sender)
{
char *message = NULL;
std::string empClearTable = "delete from emp";
int result = sqlite3_exec(database, empClearTable.c_str(), NULL, NULL, &message);
if( result != SQLITE_OK ) {
	errorMessage(database, NULL, L"Could not perform delete operation.");
	return;
}
else
	VirtualStringTree->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TVSTForm::OnClose(TObject *Sender, TCloseAction &Action)
{
sqlite3_close(database);
}
//---------------------------------------------------------------------------
