// solver.cpp
//
// Mario Konrad
// 2006-05-30

#include <wx/wx.h>

enum {
	 ID_EXIT  = 2
	,ID_ABOUT = wxID_ABOUT
	,ID_ON_CLEAR = 10
	,ID_ON_SOLVE = 11
	,ID_TEXT
};

// --------------------------------------------------------

class SudokuSolverFrame : public wxFrame
{
	private:
		enum { NS = 3, N = NS*NS };
		typedef int Element;
		typedef Element Field[N][N];
	private:
		wxTextCtrl * txt[N][N];
		bool update;
	private:
		wxTextCtrl * create(void);
		void add(wxSizer *, size_t, size_t);
		bool row_contains(const Field, int, int, Element);
		bool col_contains(const Field, int, int, Element);
		bool sub_contains(const Field, int, int, Element);
		bool solve(Field &);
		bool check(const Field);
		void randomize(Element [], size_t);
	public:
		SudokuSolverFrame(); 
		void on_exit(wxCommandEvent &);
		void on_about(wxCommandEvent &);
		void on_clear(wxCommandEvent &);
		void on_solve(wxCommandEvent &);
		void on_text(wxCommandEvent &);
	private:
		DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(SudokuSolverFrame, wxFrame)
	EVT_MENU(ID_EXIT, SudokuSolverFrame::on_exit)
	EVT_MENU(ID_ABOUT, SudokuSolverFrame::on_about)
	EVT_MENU(ID_ON_CLEAR, SudokuSolverFrame::on_clear)
	EVT_MENU(ID_ON_SOLVE, SudokuSolverFrame::on_solve)
	EVT_TEXT(ID_TEXT, SudokuSolverFrame::on_text)
	EVT_BUTTON(ID_ON_CLEAR, SudokuSolverFrame::on_clear)
	EVT_BUTTON(ID_ON_SOLVE, SudokuSolverFrame::on_solve)
END_EVENT_TABLE()

SudokuSolverFrame::SudokuSolverFrame()
	: wxFrame(0, -1, wxT("Sudoku Solver"),
	  wxDefaultPosition, wxDefaultSize, wxSYSTEM_MENU | wxCAPTION),
	  update(true)
{
	// menu
	wxMenu * menu_file = new wxMenu;
	menu_file->Append(ID_ABOUT, wxT("About ..."));
	menu_file->AppendSeparator();
	menu_file->Append(ID_EXIT, wxT("E&xit\tCtrl-C"));

	wxMenuBar * menubar = new wxMenuBar;
	menubar->Append(menu_file, wxT("&File"));
	SetMenuBar(menubar);

	// status bar
	SetStatusBar(new wxStatusBar(this, -1, 0));
	SetStatusText(wxT(""));

	wxSizer * sizer_top = new wxBoxSizer(wxVERTICAL);

	// field
	wxSizer * sizer_field = new wxGridSizer(NS, NS);
	for (size_t row = 0; row < N; ++row) {
		for (size_t col = 0; col < N; ++col) {
			txt[row][col] = create();
		}
	}

	for (size_t row = 0; row < NS; ++row) {
		for (size_t col = 0; col < NS; ++col) {
			add(sizer_field, row * NS, col * NS);
		}
	}
	sizer_field->SetMinSize(300, 300);

	// buttons
	wxSizer * sizer_button = new wxBoxSizer(wxHORIZONTAL);
	wxButton * btn_clear = new wxButton(this, ID_ON_CLEAR, wxT("Clear"));
	wxButton * btn_solve = new wxButton(this, ID_ON_SOLVE, wxT("Solve"));
	sizer_button->Add(btn_clear, 1, wxGROW | wxLEFT | wxRIGHT, 2);
	sizer_button->Add(btn_solve, 1, wxGROW | wxLEFT | wxRIGHT, 2);

	sizer_top->Add(sizer_field, 1, wxGROW | wxALL, 2);
	sizer_top->Add(sizer_button, 0, wxGROW | wxLEFT | wxRIGHT, 2);

	SetSizer(sizer_top);
	sizer_top->Fit(this);
}

wxTextCtrl * SudokuSolverFrame::create(void)
{
	wxTextCtrl * txt = new wxTextCtrl(this, ID_TEXT, wxT(""),
		wxDefaultPosition, wxDefaultSize,
		wxTE_CENTRE
		);
	txt->SetSize(10, 10);
	txt->SetMaxLength(1);
	return txt;
}

void SudokuSolverFrame::add(wxSizer * sizer, size_t row, size_t col)
{
	int sep = 0;

	wxSizer * s = new wxGridSizer(NS, NS);
	s->Add(txt[row+0][col+0], 1, wxGROW | wxALL, sep);
	s->Add(txt[row+0][col+1], 1, wxGROW | wxALL, sep);
	s->Add(txt[row+0][col+2], 1, wxGROW | wxALL, sep);
	s->Add(txt[row+1][col+0], 1, wxGROW | wxALL, sep);
	s->Add(txt[row+1][col+1], 1, wxGROW | wxALL, sep);
	s->Add(txt[row+1][col+2], 1, wxGROW | wxALL, sep);
	s->Add(txt[row+2][col+0], 1, wxGROW | wxALL, sep);
	s->Add(txt[row+2][col+1], 1, wxGROW | wxALL, sep);
	s->Add(txt[row+2][col+2], 1, wxGROW | wxALL, sep);
	sizer->Add(s, 1, wxGROW | wxALL, 2);
}

void SudokuSolverFrame::on_exit(wxCommandEvent &)
{
	Close(true);
}

void SudokuSolverFrame::on_about(wxCommandEvent &)
{
	wxMessageBox(
		wxT("Sudoku Solver\n\n(C) Mario Konrad\nMay 2006"),
		wxT("About"),
		wxOK | wxICON_INFORMATION,
		this
		);
}

void SudokuSolverFrame::on_text(wxCommandEvent & event)
{
	wxTextCtrl * txt = dynamic_cast<wxTextCtrl *>(event.GetEventObject());
	wxString s = event.GetString();
	if (s == wxT("")) {
		if (update) {
			txt->SetBackgroundColour(*wxWHITE);
			txt->Refresh();
		}
		return;
	}
	unsigned long val = 0;
	if (s.ToULong(&val, 10) && (val > 0) && (val <= 9)) {
		// ok
		if (update) {
			txt->SetBackgroundColour(wxColour(0xd0, 0xd0, 0xd0));
			txt->Refresh();
		}
	} else {
		txt->SetValue(wxT(""));
		if (update) {
			txt->SetBackgroundColour(*wxWHITE);
			txt->Refresh();
		}
	}
}

void SudokuSolverFrame::on_clear(wxCommandEvent &)
{
	for (size_t row = 0; row < N; ++row) {
		for (size_t col = 0; col < N; ++col) {
			txt[row][col]->SetValue(wxT(""));
			txt[row][col]->SetBackgroundColour(*wxWHITE);
		}
	}
	SetStatusText(wxT(""));
}

void SudokuSolverFrame::on_solve(wxCommandEvent &)
{
	// prepare field
	Field f;
	for (int row = 0; row < N; ++row) {
		for (int col = 0; col < N; ++col) {
			unsigned long val = 0;
			txt[row][col]->GetValue().ToULong(&val, 10);
			f[row][col] = val;
		}
	}

	// calculate solution
	if (check(f) && solve(f)) {
		update = false;
		// show solution
		for (int row = 0; row < N; ++row) {
			for (int col = 0; col < N; ++col) {
				txt[row][col]->SetValue(
					wxString::Format(wxT("%d"), f[row][col]));
			}
		}
		update = true;
		SetStatusText(wxT("Solution found!"));
	} else {
		SetStatusText(wxT("No Solution possible!"));
	}
}

bool SudokuSolverFrame::row_contains(const Field g, int row,
	int ignore_col, Element e)
{
	for (int col = 0; col < N; ++col)
		if ((col != ignore_col) && (g[row][col] == e)) return true;
	return false;
}

bool SudokuSolverFrame::col_contains(const Field g, int ignore_row,
	int col, Element e)
{
	for (int row = 0; row < N; ++row)
		if ((row != ignore_row) && (g[row][col] == e)) return true;
	return false;
}

bool SudokuSolverFrame::sub_contains(const Field g, int row, int col,
	Element e)
{
	int sx = col - (col % NS);
	int sy = row - (row % NS);
	for (int y = 0; y < NS; ++y) {
		for (int x = 0; x < NS; ++x) {

			// do not check itself
			if (((sy+y) == row) && ((sx+x) == col)) continue;

			// check
			if (g[sy+y][sx+x] == e) return true;
		}
	}
	return false;
}

bool SudokuSolverFrame::check(const Field f)
{
	for (int row = 0; row < N; ++row) {
		for (int col = 0; col < N; ++col) {
			if (f[row][col] == 0) continue;
			if (col_contains(f, row, col, f[row][col])) return false;
			if (row_contains(f, row, col, f[row][col])) return false;
			if (sub_contains(f, row, col, f[row][col])) return false;
		}
	}
	return true;
}

void SudokuSolverFrame::randomize(Element e[], size_t n)
{
	srand(time(0)); // seed

	size_t a;
	size_t b;
	for (size_t i = 100 + (rand() % 500); i; --i) {
		a = rand() % n;
		b = rand() % n;
		Element tmp = e[a];
		e[a] = e[b];
		e[b] = tmp;
	}
}

bool SudokuSolverFrame::solve(Field & g)
{
	for (int y = 0; y < N; ++y) {
		for (int x = 0; x < N; ++x) {

			if (g[y][x] != 0) continue; // already occupied?

			// prepare elements
			Element e[N];
			Element elem = 1;
			for (size_t i = 0; i < N; ++i, ++elem) e[i] = elem;
			randomize(e, N);

			// try elements
			bool solution = false;
			for (size_t i = 0; !solution && (i < N); ++i) {
				g[y][x] = e[i];

				// check
				if (   row_contains(g, y, x, e[i])
				    || col_contains(g, y, x, e[i])
				    || sub_contains(g, y, x, e[i])
				    ) {
					solution = false;
				} else {
					// if not found, search deeper
					solution = solve(g);
				}
			}

			// check for (sub)solution
			if (!solution) {
				g[y][x] = 0; // then restore
				return false; // still found!
			}
		}
	}
	return true; // no duplicates found, might be a good solution
}

// --------------------------------------------------------

class SudokuSolverApp : public wxApp
{
	public:
		virtual bool OnInit();
};

IMPLEMENT_APP(SudokuSolverApp)

bool SudokuSolverApp::OnInit()
{
	SudokuSolverFrame * frame = new SudokuSolverFrame;
	frame->Show(true);
	return true;
}

