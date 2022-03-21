#ifndef SEPARATORDIALOG_H
#define SEPARATORDIALOG_H

//(*Headers(SeparatorDialog)
#include <wx/dialog.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
//*)

class SeparatorDialog: public wxDialog
{
	public:

		SeparatorDialog(wxWindow* parent);
		virtual ~SeparatorDialog();

		//(*Declarations(SeparatorDialog)
		wxRadioButton* mRadioButtonComma;
		wxRadioButton* mRadioButtonDouble;
		wxRadioButton* mRadioButtonNoEscape;
		wxRadioButton* mRadioButtonOther;
		wxRadioButton* mRadioButtonOtherEscape;
		wxRadioButton* mRadioButtonSemicolon;
		wxRadioButton* mRadioButtonSingle;
		wxRadioButton* mRadioButtonSpace;
		wxRadioButton* mRadioButtonTab;
		wxRadioButton* mRadioButtonVerticalBar;
		wxTextCtrl* mTextCtrlOtherEscape;
		wxTextCtrl* mTextCtrlOtherSeparator;
		//*)

	protected:

		//(*Identifiers(SeparatorDialog)
		static const long ID_RADIOBUTTON1;
		static const long ID_RADIOBUTTON2;
		static const long ID_RADIOBUTTON3;
		static const long ID_RADIOBUTTON4;
		static const long ID_RADIOBUTTON5;
		static const long ID_RADIOBUTTON6;
		static const long ID_TEXTCTRL1;
		static const long ID_RADIOBUTTON7;
		static const long ID_RADIOBUTTON8;
		static const long ID_RADIOBUTTON10;
		static const long ID_RADIOBUTTON9;
		static const long ID_TEXTCTRL2;
		//*)

	private:

		//(*Handlers(SeparatorDialog)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
