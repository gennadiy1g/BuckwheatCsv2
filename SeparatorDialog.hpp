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
		wxRadioButton* mRadioButtonOther;
		wxRadioButton* mRadioButtonSemicolon;
		wxRadioButton* mRadioButtonSpace;
		wxRadioButton* mRadioButtonTab;
		wxRadioButton* mRadioButtonVerticalBar;
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
		//*)

	private:

		//(*Handlers(SeparatorDialog)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
