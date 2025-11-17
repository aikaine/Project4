#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
using namespace std;

// ================== КЛАСС FRACTION ======================
long long gcdll(long long a, long long b)
{
	if (b == 0) return a;
	return gcdll(b, a % b);
}

class Fraction {
public:
	long long x, y;

	Fraction(long long a = 0, long long b = 1) {
		long long g = gcdll(a, b);
		x = a / g;
		y = b / g;
		if (y < 0) { y = -y; x = -x; }
	}

	Fraction operator + (Fraction b) const {
		return Fraction(x * b.y + y * b.x, y * b.y);
	}
	Fraction operator - (Fraction b) const {
		return Fraction(x * b.y - y * b.x, y * b.y);
	}
	Fraction operator * (Fraction b) const {
		return Fraction(x * b.x, y * b.y);
	}
	Fraction operator / (Fraction b) const {
		return Fraction(x * b.y, y * b.x);
	}
	bool operator < (Fraction b) const {
		return x * b.y < b.x * y;
	}
	bool operator > (Fraction b) const {
		return x * b.y > b.x * y;
	}
	bool operator == (Fraction b) const {
		return x * b.y == b.x * y;
	}
	operator double() const {
		return (double)x / (double)y;
	}
	string tostring() const {
		if (y == 1) return to_string(x);
		return to_string(x) + "/" + to_string(y);
	}
};

// ================== КЛАСС LPP ============================
class LPP {
public:
	int rows, columns;
	vector<vector<Fraction>> A;
	vector<int> basis;
	vector<Fraction> roots;
	Fraction value;
	bool is_max;

	LPP(int r, int c, bool max = true) {
		rows = r;
		columns = c;
		A.assign(r + 1, vector<Fraction>(c + 1, 0));
		basis.assign(r, -1);
		is_max = max;
	}

	// Поиск ведущего столбца
	int pivot_col() {
		if (is_max) {
			// Для максимизации ищем отрицательный коэффициент в целевой строке
			for (int j = 0; j < columns; j++) {
				if (A[rows][j] < Fraction(0)) {
					return j;
				}
			}
		}
		else {
			// Для минимизации ищем положительный коэффициент в целевой строке
			for (int j = 0; j < columns; j++) {
				if (A[rows][j] > Fraction(0)) {
					return j;
				}
			}
		}
		return -1; // Решение найдено
	}

	// остальные методы остаются без изменений...
	int pivot_row(int col) {
		int row = -1;
		Fraction best(-1, 1);
		for (int i = 0; i < rows; i++) {
			if (A[i][col] > Fraction(0)) {
				Fraction t = A[i][columns] / A[i][col];
				if (row == -1 || t < best) {
					best = t;
					row = i;
				}
			}
		}
		return row;
	}

	void make_iter(int r, int c) {
		Fraction pivot = A[r][c];
		for (int j = 0; j <= columns; j++)
			A[r][j] = A[r][j] / pivot;

		for (int i = 0; i <= rows; i++) {
			if (i != r) {
				Fraction k = A[i][c];
				for (int j = 0; j <= columns; j++)
					A[i][j] = A[i][j] - A[r][j] * k;
			}
		}
		basis[r] = c;
	}

	bool solve_table() {
		int iterations = 0;
		const int MAX_ITERATIONS = 100;

		while (iterations++ < MAX_ITERATIONS) {
			int col = pivot_col();
			if (col == -1) break;

			int row = pivot_row(col);
			if (row == -1) return false;

			make_iter(row, col);
		}
		return true;
	}
};
// ================== ОБОЛОЧКА solve_lpp ====================
struct Solutions {
	string res;
	vector<Fraction> roots;
	Fraction value;
};

Solutions solve_lpp(int n, vector<string> names, vector<Fraction> f,
	vector<vector<Fraction>> A, vector<Fraction> b,
	vector<char> signs, vector<char> constraints, bool is_max)
{
	int rows = A.size();
	int columns = n;

	LPP lp(rows, columns, is_max);

	// Загружаем ограничения
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < n; j++) {
			lp.A[i][j] = A[i][j];
		}
		lp.A[i][columns] = b[i];
	}

	// Целевая функция
	if (is_max) {
		// Для максимизации: F -> -F в симплекс-таблице
		for (int j = 0; j < n; j++) {
			lp.A[rows][j] = f[j] * Fraction(-1);
		}
	}
	else {
		// Для минимизации: F -> F в симплекс-таблице
		for (int j = 0; j < n; j++) {
			lp.A[rows][j] = f[j];
		}
	}
	lp.A[rows][columns] = 0;

	bool ok = lp.solve_table();

	Solutions out;
	if (!ok) {
		out.res = "Решения нет или не удалось найти решение";
		return out;
	}

	out.res = "Оптимальное решение:\n";
	out.roots.assign(n, Fraction(0));

	// Извлекаем решение
	for (int i = 0; i < rows; i++) {
		if (lp.basis[i] >= 0 && lp.basis[i] < n) {
			out.roots[lp.basis[i]] = lp.A[i][columns];
		}
	}

	for (int i = 0; i < n; i++)
		out.res += names[i] + " = " + out.roots[i].tostring() + "\n";

	// Вычисляем значение целевой функции
	out.value = Fraction(0);
	for (int i = 0; i < n; i++) {
		out.value = out.value + f[i] * out.roots[i];
	}

	out.res += "F = " + out.value.tostring();
	return out;
}


// ================== WINDOWS FORM INTERFACE ====================
#pragma once

namespace Project4 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class simpleks : public System::Windows::Forms::Form
	{
	public:
		simpleks(void)
		{
			InitializeComponent();
		}

	protected:
		~simpleks()
		{
			if (components)
			{
				delete components;
			}
		}

	private:

		Label^ lblVars;
		Label^ lblCons;
		TextBox^ txtVars;
		TextBox^ txtCons;

		Button^ btnCreate;
		Button^ btnSolve;
		Button^ btnBack;

		Label^ lblTarget;
		ComboBox^ comboTarget;

		DataGridView^ table;
		TextBox^ txtOutput;

		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->lblVars = gcnew System::Windows::Forms::Label();
			this->lblCons = gcnew System::Windows::Forms::Label();
			this->txtVars = gcnew System::Windows::Forms::TextBox();
			this->txtCons = gcnew System::Windows::Forms::TextBox();
			this->btnCreate = gcnew System::Windows::Forms::Button();
			this->btnSolve = gcnew System::Windows::Forms::Button();
			this->btnBack = gcnew System::Windows::Forms::Button();
			this->lblTarget = gcnew System::Windows::Forms::Label();
			this->comboTarget = gcnew System::Windows::Forms::ComboBox();
			this->table = gcnew System::Windows::Forms::DataGridView();
			this->txtOutput = gcnew System::Windows::Forms::TextBox();

			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->table))->BeginInit();
			this->SuspendLayout();

			//
			// lblVars
			//
			this->lblVars->AutoSize = true;
			this->lblVars->Location = System::Drawing::Point(20, 20);
			this->lblVars->Text = L"Число переменных:";

			//
			// txtVars
			//
			this->txtVars->Location = System::Drawing::Point(150, 17);
			this->txtVars->Size = System::Drawing::Size(50, 20);
			this->txtVars->Text = "2";

			//
			// lblCons
			//
			this->lblCons->AutoSize = true;
			this->lblCons->Location = System::Drawing::Point(20, 50);
			this->lblCons->Text = L"Число ограничений:";

			//
			// txtCons
			//
			this->txtCons->Location = System::Drawing::Point(150, 47);
			this->txtCons->Size = System::Drawing::Size(50, 20);
			this->txtCons->Text = "2";

			//
			// btnCreate
			//
			this->btnCreate->BackColor = System::Drawing::Color::LightGreen;
			this->btnCreate->Location = System::Drawing::Point(220, 20);
			this->btnCreate->Size = System::Drawing::Size(120, 40);
			this->btnCreate->Text = L"Создать таблицу";
			this->btnCreate->UseVisualStyleBackColor = false;
			this->btnCreate->Click += gcnew System::EventHandler(this, &simpleks::btnCreate_Click);

			//
			// btnSolve
			//
			this->btnSolve->BackColor = System::Drawing::Color::LightSkyBlue;
			this->btnSolve->Location = System::Drawing::Point(350, 20);
			this->btnSolve->Size = System::Drawing::Size(120, 40);
			this->btnSolve->Text = L"Решить";
			this->btnSolve->UseVisualStyleBackColor = false;
			this->btnSolve->Click += gcnew System::EventHandler(this, &simpleks::btnSolve_Click);

			//
			// btnBack
			//
			this->btnBack->BackColor = System::Drawing::Color::LightCoral;
			this->btnBack->Location = System::Drawing::Point(480, 20);
			this->btnBack->Size = System::Drawing::Size(120, 40);
			this->btnBack->Text = L"Назад";
			this->btnBack->UseVisualStyleBackColor = false;
			this->btnBack->Click += gcnew System::EventHandler(this, &simpleks::btnBack_Click);

			//
			// lblTarget
			//
			this->lblTarget->AutoSize = true;
			this->lblTarget->Location = System::Drawing::Point(20, 80);
			this->lblTarget->Text = L"Целевая функция:";

			//
			// comboTarget
			//
			this->comboTarget->Location = System::Drawing::Point(150, 77);
			this->comboTarget->Size = System::Drawing::Size(70, 21);
			this->comboTarget->Items->AddRange(gcnew cli::array<System::Object^>{ L"Max", L"Min" });
			this->comboTarget->SelectedIndex = 0;

			//
			// table
			//
			this->table->Location = System::Drawing::Point(20, 110);
			this->table->Size = System::Drawing::Size(580, 250);
			this->table->BackgroundColor = System::Drawing::Color::Gainsboro;

			//
			// txtOutput
			//
			this->txtOutput->Location = System::Drawing::Point(20, 370);
			this->txtOutput->Size = System::Drawing::Size(580, 80);
			this->txtOutput->Multiline = true;
			this->txtOutput->ScrollBars = ScrollBars::Vertical;

			//
			// simpleks window
			//
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(630, 470);
			this->Controls->Add(this->lblVars);
			this->Controls->Add(this->txtVars);
			this->Controls->Add(this->lblCons);
			this->Controls->Add(this->txtCons);
			this->Controls->Add(this->btnCreate);
			this->Controls->Add(this->btnSolve);
			this->Controls->Add(this->btnBack);
			this->Controls->Add(this->lblTarget);
			this->Controls->Add(this->comboTarget);
			this->Controls->Add(this->table);
			this->Controls->Add(this->txtOutput);

			this->Text = L"Симплекс метод";
			this->StartPosition = FormStartPosition::CenterScreen;

			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->table))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();
		}
#pragma endregion

	private:

		System::Void btnCreate_Click(System::Object^ sender, System::EventArgs^ e)
		{
			try
			{
				int vars = Convert::ToInt32(txtVars->Text);
				int cons = Convert::ToInt32(txtCons->Text);

				table->Columns->Clear();
				table->Rows->Clear();

				// Отключаем автоматическое добавление пустой строки
				table->AllowUserToAddRows = false;

				// создаём столбцы для переменных
				for (int i = 0; i < vars; i++)
					table->Columns->Add("x" + i, "x" + i);

				// столбец для свободных членов
				table->Columns->Add("b", "Св. член");

				// добавляем строки для ограничений
				for (int i = 0; i < cons; i++)
				{
					int rowIndex = table->Rows->Add();
					table->Rows[rowIndex]->HeaderCell->Value = "Огр." + (i + 1);
				}

				// строка для целевой функции
				int funcRowIndex = table->Rows->Add();
				table->Rows[funcRowIndex]->HeaderCell->Value = "F";

				// очищаем вывод
				txtOutput->Text = "Таблица создана. Заполните коэффициенты.";
			}
			catch (Exception^ ex)
			{
				txtOutput->Text = "Ошибка при создании таблицы: " + ex->Message;
			}
		}

		//----------------------------------------------------------
		// Кнопка "Решить" - интеграция с симплекс-методом
		//----------------------------------------------------------
		System::Void btnSolve_Click(System::Object^ sender, System::EventArgs^ e)
		{
			try
			{
				int vars = Convert::ToInt32(txtVars->Text);
				int cons = Convert::ToInt32(txtCons->Text);

				// Проверяем, что таблица создана с правильным количеством строк
				int expectedRows = cons + 1;
				if (table->Rows->Count != expectedRows)
				{
					txtOutput->Text = "Сначала создайте таблицу с правильным количеством строк!";
					return;
				}

				// Проверяем, что все ячейки заполнены
				for (int i = 0; i < expectedRows; i++)
				{
					for (int j = 0; j < vars + 1; j++)
					{
						if (table->Rows[i]->Cells[j]->Value == nullptr ||
							String::IsNullOrEmpty(table->Rows[i]->Cells[j]->Value->ToString()))
						{
							if (i == cons && j == vars) {
								table->Rows[i]->Cells[j]->Value = "0";
							}
							else {
								txtOutput->Text = "Заполните все ячейки таблицы!";
								return;
							}
						}
					}
				}

				// Подготавливаем данные
				vector<string> names;
				vector<Fraction> f;
				vector<vector<Fraction>> A;
				vector<Fraction> b;
				vector<char> signs;
				vector<char> constraints;

				// Имена переменных
				for (int i = 0; i < vars; i++)
					names.push_back("x" + to_string(i));

				// Определяем тип задачи
				bool is_max = (comboTarget->SelectedItem->ToString() == "Max");

				// Целевая функция
				for (int j = 0; j < vars; j++)
				{
					String^ valueStr = table->Rows[cons]->Cells[j]->Value->ToString();
					String^ cleanedValueStr = valueStr->Replace(",", ".");
					double value = Convert::ToDouble(cleanedValueStr);
					f.push_back(Fraction(value));
				}

				// Ограничения
				for (int i = 0; i < cons; i++)
				{
					vector<Fraction> row;
					for (int j = 0; j < vars; j++)
					{
						String^ valueStr = table->Rows[i]->Cells[j]->Value->ToString();
						String^ cleanedValueStr = valueStr->Replace(",", ".");
						double value = Convert::ToDouble(cleanedValueStr);
						row.push_back(Fraction(value));
					}
					A.push_back(row);

					String^ bStr = table->Rows[i]->Cells[vars]->Value->ToString();
					String^ cleanedBStr = bStr->Replace(",", ".");
					double bValue = Convert::ToDouble(cleanedBStr);
					b.push_back(Fraction(bValue));

					signs.push_back('<');
					constraints.push_back('R');
				}

				// Вызываем симплекс-метод
				Solutions result = solve_lpp(vars, names, f, A, b, signs, constraints, is_max);

				// Выводим результат
				txtOutput->Text = gcnew String(result.res.c_str());
			}
			catch (FormatException^)
			{
				txtOutput->Text = "Ошибка: введены некорректные числовые значения!";
			}
			catch (exception& ex)
			{
				txtOutput->Text = "Ошибка C++: " + gcnew String(ex.what());
			}
			catch (Exception^ ex)
			{
				txtOutput->Text = "Ошибка при решении: " + ex->Message;
			}
		}

		//----------------------------------------------------------
		// Кнопка "Назад"
		//----------------------------------------------------------
		System::Void btnBack_Click(System::Object^ sender, System::EventArgs^ e)
		{
			this->Close();
		}

	};
}