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
			InitializeConstraintTypes();
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
		// Элементы управления
		System::Windows::Forms::TextBox^ textBoxVars;
		System::Windows::Forms::TextBox^ textBoxConstraints;
		System::Windows::Forms::Button^ buttonCreateTable;
		System::Windows::Forms::Button^ buttonSolve;
		System::Windows::Forms::Button^ buttonBack;
		System::Windows::Forms::DataGridView^ dataGridViewTable;
		System::Windows::Forms::RichTextBox^ textBoxResults;
		System::Windows::Forms::ComboBox^ comboBoxGoal;
		System::Windows::Forms::Label^ labelVars;
		System::Windows::Forms::Label^ labelConstraints;
		System::Windows::Forms::Label^ labelGoal;
		System::Windows::Forms::DataGridViewComboBoxColumn^ constraintTypeColumn;

		// Данные для симплекс-метода
		int variablesCount;
		int constraintsCount;
		bool isMaximization;
		array<double, 2>^ simplexTable;
		array<String^>^ constraintTypes;

		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->textBoxVars = (gcnew System::Windows::Forms::TextBox());
			this->textBoxConstraints = (gcnew System::Windows::Forms::TextBox());
			this->buttonCreateTable = (gcnew System::Windows::Forms::Button());
			this->buttonSolve = (gcnew System::Windows::Forms::Button());
			this->buttonBack = (gcnew System::Windows::Forms::Button());
			this->dataGridViewTable = (gcnew System::Windows::Forms::DataGridView());
			this->textBoxResults = (gcnew System::Windows::Forms::RichTextBox());
			this->comboBoxGoal = (gcnew System::Windows::Forms::ComboBox());
			this->labelVars = (gcnew System::Windows::Forms::Label());
			this->labelConstraints = (gcnew System::Windows::Forms::Label());
			this->labelGoal = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewTable))->BeginInit();
			this->SuspendLayout();
			// 
			// textBoxVars
			// 
			this->textBoxVars->Location = System::Drawing::Point(150, 20);
			this->textBoxVars->Name = L"textBoxVars";
			this->textBoxVars->Size = System::Drawing::Size(50, 20);
			this->textBoxVars->TabIndex = 0;
			this->textBoxVars->Text = L"2";
			// 
			// textBoxConstraints
			// 
			this->textBoxConstraints->Location = System::Drawing::Point(150, 50);
			this->textBoxConstraints->Name = L"textBoxConstraints";
			this->textBoxConstraints->Size = System::Drawing::Size(50, 20);
			this->textBoxConstraints->TabIndex = 1;
			this->textBoxConstraints->Text = L"2";
			// 
			// buttonCreateTable
			// 
			this->buttonCreateTable->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)),
				static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)));
			this->buttonCreateTable->Location = System::Drawing::Point(220, 20);
			this->buttonCreateTable->Name = L"buttonCreateTable";
			this->buttonCreateTable->Size = System::Drawing::Size(100, 50);
			this->buttonCreateTable->TabIndex = 2;
			this->buttonCreateTable->Text = L"Создать таблицу";
			this->buttonCreateTable->UseVisualStyleBackColor = false;
			this->buttonCreateTable->Click += gcnew System::EventHandler(this, &simpleks::buttonCreateTable_Click);
			// 
			// buttonSolve
			// 
			this->buttonSolve->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)),
				static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->buttonSolve->Location = System::Drawing::Point(330, 20);
			this->buttonSolve->Name = L"buttonSolve";
			this->buttonSolve->Size = System::Drawing::Size(100, 50);
			this->buttonSolve->TabIndex = 3;
			this->buttonSolve->Text = L"Решить";
			this->buttonSolve->UseVisualStyleBackColor = false;
			this->buttonSolve->Click += gcnew System::EventHandler(this, &simpleks::buttonSolve_Click);
			// 
			// buttonBack
			// 
			this->buttonBack->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
				static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(192)));
			this->buttonBack->Location = System::Drawing::Point(440, 20);
			this->buttonBack->Name = L"buttonBack";
			this->buttonBack->Size = System::Drawing::Size(100, 50);
			this->buttonBack->TabIndex = 4;
			this->buttonBack->Text = L"Назад";
			this->buttonBack->UseVisualStyleBackColor = false;
			this->buttonBack->Click += gcnew System::EventHandler(this, &simpleks::buttonBack_Click);
			// 
			// dataGridViewTable
			// 
			this->dataGridViewTable->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridViewTable->Location = System::Drawing::Point(20, 100);
			this->dataGridViewTable->Name = L"dataGridViewTable";
			this->dataGridViewTable->Size = System::Drawing::Size(520, 200);
			this->dataGridViewTable->TabIndex = 5;
			// 
			// textBoxResults
			// 
			this->textBoxResults->Location = System::Drawing::Point(20, 320);
			this->textBoxResults->Name = L"textBoxResults";
			this->textBoxResults->Size = System::Drawing::Size(520, 120);
			this->textBoxResults->TabIndex = 6;
			this->textBoxResults->Text = L"";
			// 
			// comboBoxGoal
			// 
			this->comboBoxGoal->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBoxGoal->FormattingEnabled = true;
			this->comboBoxGoal->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"Максимизация", L"Минимизация" });
			this->comboBoxGoal->Location = System::Drawing::Point(150, 80);
			this->comboBoxGoal->Name = L"comboBoxGoal";
			this->comboBoxGoal->Size = System::Drawing::Size(100, 21);
			this->comboBoxGoal->TabIndex = 7;
			// 
			// labelVars
			// 
			this->labelVars->AutoSize = true;
			this->labelVars->Location = System::Drawing::Point(20, 23);
			this->labelVars->Name = L"labelVars";
			this->labelVars->Size = System::Drawing::Size(124, 13);
			this->labelVars->TabIndex = 8;
			this->labelVars->Text = L"Число переменных:";
			// 
			// labelConstraints
			// 
			this->labelConstraints->AutoSize = true;
			this->labelConstraints->Location = System::Drawing::Point(20, 53);
			this->labelConstraints->Name = L"labelConstraints";
			this->labelConstraints->Size = System::Drawing::Size(124, 13);
			this->labelConstraints->TabIndex = 9;
			this->labelConstraints->Text = L"Число ограничений:";
			// 
			// labelGoal
			// 
			this->labelGoal->AutoSize = true;
			this->labelGoal->Location = System::Drawing::Point(20, 83);
			this->labelGoal->Name = L"labelGoal";
			this->labelGoal->Size = System::Drawing::Size(124, 13);
			this->labelGoal->TabIndex = 10;
			this->labelGoal->Text = L"Целевая функция:";
			// 
			// simpleks
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::Silver;
			this->ClientSize = System::Drawing::Size(564, 461);
			this->Controls->Add(this->labelGoal);
			this->Controls->Add(this->labelConstraints);
			this->Controls->Add(this->labelVars);
			this->Controls->Add(this->comboBoxGoal);
			this->Controls->Add(this->textBoxResults);
			this->Controls->Add(this->dataGridViewTable);
			this->Controls->Add(this->buttonBack);
			this->Controls->Add(this->buttonSolve);
			this->Controls->Add(this->buttonCreateTable);
			this->Controls->Add(this->textBoxConstraints);
			this->Controls->Add(this->textBoxVars);
			this->Name = L"simpleks";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Симплекс метод с искусственным базисом";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewTable))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private:
		// Типы ограничений
		void InitializeConstraintTypes()
		{
			constraintTypes = gcnew array<String^> {
				"≤ (Меньше или равно)",
					"= (Равно)",
					"≥ (Больше или равно)"
			};
		}

		// Создание таблицы для ввода данных
		void buttonCreateTable_Click(System::Object^ sender, System::EventArgs^ e) {
			try
			{
				variablesCount = Int32::Parse(textBoxVars->Text);
				constraintsCount = Int32::Parse(textBoxConstraints->Text);
				isMaximization = (comboBoxGoal->SelectedIndex == 0);

				if (comboBoxGoal->SelectedIndex == -1)
					comboBoxGoal->SelectedIndex = 0;

				CreateDataGridView();

				textBoxResults->Text = "Таблица создана. Введите данные:\n";
				textBoxResults->AppendText("- Столбцы x1, x2... - коэффициенты при переменных\n");
				textBoxResults->AppendText("- Столбец 'Тип' - тип ограничения\n");
				textBoxResults->AppendText("- Столбец 'B' - правые части ограничений\n");
				textBoxResults->AppendText("- Строка 'C' - коэффициенты целевой функции\n");
			}
			catch (Exception^ ex)
			{
				MessageBox::Show("Ошибка при создании таблицы: " + ex->Message);
			}
		}

		// Создание DataGridView для ввода данных
		void CreateDataGridView()
		{
			dataGridViewTable->Rows->Clear();
			dataGridViewTable->Columns->Clear();

			// Добавляем столбцы для переменных
			for (int i = 0; i < variablesCount; i++)
			{
				dataGridViewTable->Columns->Add("x" + (i + 1), "x" + (i + 1));
			}

			// Добавляем столбец для типа ограничения
			DataGridViewComboBoxColumn^ typeColumn = gcnew DataGridViewComboBoxColumn();
			typeColumn->HeaderText = "Тип";
			typeColumn->Name = "Type";
			for each (String ^ type in constraintTypes)
			{
				typeColumn->Items->Add(type);
			}
			dataGridViewTable->Columns->Add(typeColumn);

			// Добавляем столбец для правых частей
			dataGridViewTable->Columns->Add("B", "B");

			// Добавляем строки для ограничений
			for (int i = 0; i < constraintsCount; i++)
			{
				int rowIndex = dataGridViewTable->Rows->Add();
				dataGridViewTable->Rows[rowIndex]->HeaderCell->Value = "Огр. " + (i + 1);
				// Устанавливаем тип по умолчанию
				dataGridViewTable->Rows[rowIndex]->Cells["Type"]->Value = constraintTypes[0];
			}

			// Добавляем строку для целевой функции
			dataGridViewTable->Rows->Add();
			dataGridViewTable->Rows[constraintsCount]->HeaderCell->Value = "C";

			// Настройка внешнего вида
			dataGridViewTable->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
		}

		// Решение задачи симплекс-методом
		void buttonSolve_Click(System::Object^ sender, System::EventArgs^ e) {
			try
			{
				if (dataGridViewTable->Rows->Count == 0)
				{
					MessageBox::Show("Сначала создайте таблицу!");
					return;
				}

				SolveWithArtificialBasis();

			}
			catch (Exception^ ex)
			{
				MessageBox::Show("Ошибка при решении: " + ex->Message);
			}
		}

		// Двухфазный симплекс-метод с искусственным базисом
		void SolveWithArtificialBasis()
		{
			textBoxResults->Clear();
			textBoxResults->AppendText("=== ДВУХФАЗНЫЙ СИМПЛЕКС-МЕТОД ===\n\n");

			// Получаем данные из таблицы
			array<double, 2>^ originalTable = GetDataFromGrid();
			array<int>^ constraintTypes = GetConstraintTypes();

			// Фаза 1: Минимизация суммы искусственных переменных
			textBoxResults->AppendText("ФАЗА 1: Поиск начального базисного решения\n");
			bool phase1Result = PerformPhase1(originalTable, constraintTypes);

			if (!phase1Result)
			{
				textBoxResults->AppendText("✗ Задача не имеет допустимого решения!\n");
				return;
			}

			// Фаза 2: Решение исходной задачи
			textBoxResults->AppendText("\nФАЗА 2: Решение исходной задачи\n");
			PerformPhase2(originalTable);
		}

		// Получение данных из DataGridView
		array<double, 2>^ GetDataFromGrid()
		{
			int rows = constraintsCount + 1;
			int cols = variablesCount + 1; // +1 для столбца B

			array<double, 2>^ table = gcnew array<double, 2>(rows, cols);

			for (int i = 0; i < rows; i++)
			{
				for (int j = 0; j < variablesCount; j++)
				{
					String^ value = dataGridViewTable->Rows[i]->Cells[j]->Value == nullptr ?
						"" : dataGridViewTable->Rows[i]->Cells[j]->Value->ToString();
					if (String::IsNullOrEmpty(value))
						table[i, j] = 0;
					else
						table[i, j] = Double::Parse(value);
				}

				// Столбец B (правые части)
				String^ bValue = dataGridViewTable->Rows[i]->Cells["B"]->Value == nullptr ?
					"" : dataGridViewTable->Rows[i]->Cells["B"]->Value->ToString();
				if (String::IsNullOrEmpty(bValue))
					table[i, variablesCount] = 0;
				else
					table[i, variablesCount] = Double::Parse(bValue);
			}

			// Для целевой функции меняем знак для минимизации
			if (!isMaximization)
			{
				for (int j = 0; j < variablesCount; j++)
				{
					table[constraintsCount, j] = -table[constraintsCount, j];
				}
			}

			return table;
		}

		// Получение типов ограничений
		array<int>^ GetConstraintTypes()
		{
			array<int>^ types = gcnew array<int>(constraintsCount);

			for (int i = 0; i < constraintsCount; i++)
			{
				String^ typeValue = dataGridViewTable->Rows[i]->Cells["Type"]->Value->ToString();
				if (typeValue == "= (Равно)")
					types[i] = 1;
				else if (typeValue == "≥ (Больше или равно)")
					types[i] = 2;
				else
					types[i] = 0; // "≤ (Меньше или равно)"
			}

			return types;
		}

		// Фаза 1: Поиск начального базисного решения
		bool PerformPhase1(array<double, 2>^ originalTable, array<int>^ constraintTypes)
		{
			// Подсчитываем количество искусственных переменных
			int artificialVars = 0;
			for (int i = 0; i < constraintsCount; i++)
			{
				if (constraintTypes[i] == 1 || constraintTypes[i] == 2) // "=" или "≥"
					artificialVars++;
			}

			if (artificialVars == 0)
			{
				textBoxResults->AppendText("✓ Искусственные переменные не требуются\n");
				simplexTable = originalTable;
				return true;
			}

			// Создаем таблицу для фазы 1
			int totalVars = variablesCount + artificialVars;
			int phase1Rows = constraintsCount + 1;
			int phase1Cols = totalVars + 1;

			array<double, 2>^ phase1Table = gcnew array<double, 2>(phase1Rows, phase1Cols);

			// Копируем исходные данные
			int artificialIndex = 0;
			for (int i = 0; i < constraintsCount; i++)
			{
				// Копируем коэффициенты при исходных переменных
				for (int j = 0; j < variablesCount; j++)
				{
					phase1Table[i, j] = originalTable[i, j];
				}

				// Добавляем искусственные переменные
				if (constraintTypes[i] == 1) // "="
				{
					phase1Table[i, variablesCount + artificialIndex] = 1;
					artificialIndex++;
				}
				else if (constraintTypes[i] == 2) // "≥"
				{
					phase1Table[i, variablesCount + artificialIndex] = -1;
					phase1Table[i, variablesCount + artificialIndex + 1] = 1;
					artificialIndex += 2;
				}

				// Копируем правую часть
				phase1Table[i, totalVars] = originalTable[i, variablesCount];
			}

			// Создаем целевую функцию для фазы 1 (минимизация суммы искусственных переменных)
			for (int j = 0; j < totalVars; j++)
			{
				phase1Table[constraintsCount, j] = 0;
			}

			// Для искусственных переменных ставим коэффициент -1
			artificialIndex = 0;
			for (int i = 0; i < constraintsCount; i++)
			{
				if (constraintTypes[i] == 1) // "="
				{
					phase1Table[constraintsCount, variablesCount + artificialIndex] = -1;
					artificialIndex++;
				}
				else if (constraintTypes[i] == 2) // "≥"
				{
					phase1Table[constraintsCount, variablesCount + artificialIndex + 1] = -1;
					artificialIndex += 2;
				}
			}

			textBoxResults->AppendText("Создано " + artificialVars + " искусственных переменных\n");

			// Решаем фазу 1 симплекс-методом
			simplexTable = phase1Table;
			bool result = SolveSimplexAlgorithm(true);

			if (result && Math::Abs(simplexTable[constraintsCount, totalVars]) < 1e-6)
			{
				textBoxResults->AppendText("✓ Найдено допустимое базисное решение\n");
				return true;
			}
			else
			{
				return false;
			}
		}

		// Фаза 2: Решение исходной задачи
		void PerformPhase2(array<double, 2>^ originalTable)
		{
			// Используем найденное базисное решение из фазы 1
			// и подставляем исходную целевую функцию

			// Копируем таблицу из фазы 1 (без искусственных переменных)
			int rows = constraintsCount + 1;
			int cols = variablesCount + 1;

			array<double, 2>^ phase2Table = gcnew array<double, 2>(rows, cols);

			// Копируем коэффициенты при ограничениях
			for (int i = 0; i < constraintsCount; i++)
			{
				for (int j = 0; j < variablesCount; j++)
				{
					phase2Table[i, j] = simplexTable[i, j];
				}
				phase2Table[i, variablesCount] = simplexTable[i, simplexTable->GetLength(1) - 1];
			}

			// Устанавливаем исходную целевую функцию
			for (int j = 0; j < variablesCount; j++)
			{
				phase2Table[constraintsCount, j] = originalTable[constraintsCount, j];
			}
			phase2Table[constraintsCount, variablesCount] = originalTable[constraintsCount, variablesCount];

			// Пересчитываем целевую функцию для текущего базиса
			simplexTable = phase2Table;
			RecalculateObjectiveFunction();

			// Решаем фазу 2
			SolveSimplexAlgorithm(false);
		}

		// Основной алгоритм симплекс-метода
		bool SolveSimplexAlgorithm(bool isPhase1)
		{
			int iteration = 0;
			const int MAX_ITERATIONS = 100;

			while (iteration < MAX_ITERATIONS)
			{
				iteration++;
				textBoxResults->AppendText("Итерация " + iteration + ":\n");

				int pivotCol = FindPivotColumn();
				if (pivotCol == -1)
				{
					textBoxResults->AppendText("✓ Достигнуто оптимальное решение!\n");

					if (!isPhase1)
					{
						DisplayOptimalSolution();
					}

					return true;
				}

				int pivotRow = FindPivotRow(pivotCol);
				if (pivotRow == -1)
				{
					textBoxResults->AppendText("✗ Задача неограничена!\n");
					return false;
				}

				textBoxResults->AppendText(String::Format("  Разрешающий элемент: [{0},{1}] = {2:F3}\n",
					pivotRow + 1, pivotCol + 1, simplexTable[pivotRow, pivotCol]));

				UpdateSimplexTable(pivotRow, pivotCol);

				if (iteration <= 3) // Выводим только первые 3 итерации для краткости
				{
					DisplayCurrentTable(iteration);
				}
			}

			textBoxResults->AppendText("✗ Превышено максимальное число итераций!\n");
			return false;
		}

		// Пересчет целевой функции для текущего базиса
		void RecalculateObjectiveFunction()
		{
			int rows = simplexTable->GetLength(0);
			int cols = simplexTable->GetLength(1);
			int lastRow = rows - 1;

			// Находим базисные переменные и пересчитываем целевую функцию
			for (int j = 0; j < cols - 1; j++)
			{
				if (Math::Abs(simplexTable[lastRow, j]) > 1e-6)
				{
					// Находим строку, где эта переменная в базисе
					for (int i = 0; i < rows - 1; i++)
					{
						if (Math::Abs(simplexTable[i, j] - 1) < 1e-6)
						{
							// Вычитаем из целевой функции
							double factor = simplexTable[lastRow, j];
							for (int k = 0; k < cols; k++)
							{
								simplexTable[lastRow, k] -= factor * simplexTable[i, k];
							}
							break;
						}
					}
				}
			}
		}

		// Остальные методы остаются без изменений
		int FindPivotColumn()
		{
			double maxPositive = 1e-6;
			int pivotCol = -1;
			int lastRow = simplexTable->GetLength(0) - 1;
			int cols = simplexTable->GetLength(1) - 1;

			for (int j = 0; j < cols; j++)
			{
				if (simplexTable[lastRow, j] > maxPositive)
				{
					maxPositive = simplexTable[lastRow, j];
					pivotCol = j;
				}
			}

			return pivotCol;
		}

		int FindPivotRow(int pivotCol)
		{
			double minRatio = Double::MaxValue;
			int pivotRow = -1;
			int rows = simplexTable->GetLength(0) - 1;
			int lastCol = simplexTable->GetLength(1) - 1;

			for (int i = 0; i < rows; i++)
			{
				if (simplexTable[i, pivotCol] > 1e-6)
				{
					double ratio = simplexTable[i, lastCol] / simplexTable[i, pivotCol];
					if (ratio < minRatio && ratio >= 0)
					{
						minRatio = ratio;
						pivotRow = i;
					}
				}
			}

			return pivotRow;
		}

		void UpdateSimplexTable(int pivotRow, int pivotCol)
		{
			double pivotElement = simplexTable[pivotRow, pivotCol];
			int rows = simplexTable->GetLength(0);
			int cols = simplexTable->GetLength(1);

			// Нормализация разрешающей строки
			for (int j = 0; j < cols; j++)
			{
				simplexTable[pivotRow, j] /= pivotElement;
			}

			// Пересчет остальных строк
			for (int i = 0; i < rows; i++)
			{
				if (i != pivotRow)
				{
					double factor = simplexTable[i, pivotCol];
					for (int j = 0; j < cols; j++)
					{
						simplexTable[i, j] -= factor * simplexTable[pivotRow, j];
					}
				}
			}
		}

		void DisplayCurrentTable(int iteration)
		{
			textBoxResults->AppendText("  Текущая таблица:\n");
			int rows = simplexTable->GetLength(0);
			int cols = simplexTable->GetLength(1);

			for (int i = 0; i < rows; i++)
			{
				String^ row = "  ";
				for (int j = 0; j < cols; j++)
				{
					row += String::Format("{0,8:F2}", simplexTable[i, j]);
				}
				textBoxResults->AppendText(row + "\n");
			}
			textBoxResults->AppendText("\n");
		}

		void DisplayOptimalSolution()
		{
			int rows = simplexTable->GetLength(0) - 1;
			int cols = simplexTable->GetLength(1) - 1;

			array<double>^ solution = gcnew array<double>(variablesCount);

			// Находим базисные переменные
			for (int j = 0; j < variablesCount; j++)
			{
				int basicRow = -1;
				bool isUnitColumn = true;

				for (int i = 0; i < rows; i++)
				{
					if (Math::Abs(simplexTable[i, j] - 1) < 1e-6)
					{
						if (basicRow == -1)
							basicRow = i;
						else
							isUnitColumn = false;
					}
					else if (Math::Abs(simplexTable[i, j]) > 1e-6)
					{
						isUnitColumn = false;
					}
				}

				if (isUnitColumn && basicRow != -1)
				{
					solution[j] = simplexTable[basicRow, cols];
				}
				else
				{
					solution[j] = 0;
				}
			}

			// Вывод решения
			textBoxResults->AppendText("ОПТИМАЛЬНОЕ РЕШЕНИЕ:\n");
			for (int i = 0; i < variablesCount; i++)
			{
				textBoxResults->AppendText(String::Format("  x{0} = {1:F3}\n", i + 1, solution[i]));
			}

			double objectiveValue = isMaximization ?
				-simplexTable[rows, cols] :
				simplexTable[rows, cols];

			textBoxResults->AppendText(String::Format("\nЗначение целевой функции: {0:F3}\n", objectiveValue));
		}

		// Кнопка назад
		void buttonBack_Click(System::Object^ sender, System::EventArgs^ e) {
			this->Close();
		}
	};
}
