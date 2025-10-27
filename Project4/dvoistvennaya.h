#pragma once

namespace Project4 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class dvoistvennaya : public System::Windows::Forms::Form
	{
	public:
		dvoistvennaya(void)
		{
			InitializeComponent();
		}

	protected:
		~dvoistvennaya()
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
		System::Windows::Forms::Button^ buttonBuildDual;
		System::Windows::Forms::Button^ buttonBack;
		System::Windows::Forms::DataGridView^ dataGridViewTable;
		System::Windows::Forms::RichTextBox^ textBoxResults;
		System::Windows::Forms::ComboBox^ comboBoxGoal;
		System::Windows::Forms::Label^ labelVars;
		System::Windows::Forms::Label^ labelConstraints;
		System::Windows::Forms::Label^ labelGoal;
		System::Windows::Forms::Label^ labelTitle;

		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->textBoxVars = (gcnew System::Windows::Forms::TextBox());
			this->textBoxConstraints = (gcnew System::Windows::Forms::TextBox());
			this->buttonCreateTable = (gcnew System::Windows::Forms::Button());
			this->buttonBuildDual = (gcnew System::Windows::Forms::Button());
			this->buttonBack = (gcnew System::Windows::Forms::Button());
			this->dataGridViewTable = (gcnew System::Windows::Forms::DataGridView());
			this->textBoxResults = (gcnew System::Windows::Forms::RichTextBox());
			this->comboBoxGoal = (gcnew System::Windows::Forms::ComboBox());
			this->labelVars = (gcnew System::Windows::Forms::Label());
			this->labelConstraints = (gcnew System::Windows::Forms::Label());
			this->labelGoal = (gcnew System::Windows::Forms::Label());
			this->labelTitle = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewTable))->BeginInit();
			this->SuspendLayout();
			// 
			// textBoxVars
			// 
			this->textBoxVars->Location = System::Drawing::Point(150, 50);
			this->textBoxVars->Name = L"textBoxVars";
			this->textBoxVars->Size = System::Drawing::Size(50, 20);
			this->textBoxVars->TabIndex = 0;
			this->textBoxVars->Text = L"2";
			// 
			// textBoxConstraints
			// 
			this->textBoxConstraints->Location = System::Drawing::Point(150, 80);
			this->textBoxConstraints->Name = L"textBoxConstraints";
			this->textBoxConstraints->Size = System::Drawing::Size(50, 20);
			this->textBoxConstraints->TabIndex = 1;
			this->textBoxConstraints->Text = L"2";
			// 
			// buttonCreateTable
			// 
			this->buttonCreateTable->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)),
				static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)));
			this->buttonCreateTable->Location = System::Drawing::Point(220, 50);
			this->buttonCreateTable->Name = L"buttonCreateTable";
			this->buttonCreateTable->Size = System::Drawing::Size(100, 50);
			this->buttonCreateTable->TabIndex = 2;
			this->buttonCreateTable->Text = L"Создать таблицу";
			this->buttonCreateTable->UseVisualStyleBackColor = false;
			this->buttonCreateTable->Click += gcnew System::EventHandler(this, &dvoistvennaya::buttonCreateTable_Click);
			// 
			// buttonBuildDual
			// 
			this->buttonBuildDual->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)),
				static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->buttonBuildDual->Location = System::Drawing::Point(330, 50);
			this->buttonBuildDual->Name = L"buttonBuildDual";
			this->buttonBuildDual->Size = System::Drawing::Size(100, 50);
			this->buttonBuildDual->TabIndex = 3;
			this->buttonBuildDual->Text = L"Построить двойственную";
			this->buttonBuildDual->UseVisualStyleBackColor = false;
			this->buttonBuildDual->Click += gcnew System::EventHandler(this, &dvoistvennaya::buttonBuildDual_Click);
			// 
			// buttonBack
			// 
			this->buttonBack->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
				static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(192)));
			this->buttonBack->Location = System::Drawing::Point(440, 50);
			this->buttonBack->Name = L"buttonBack";
			this->buttonBack->Size = System::Drawing::Size(100, 50);
			this->buttonBack->TabIndex = 4;
			this->buttonBack->Text = L"Назад";
			this->buttonBack->UseVisualStyleBackColor = false;
			this->buttonBack->Click += gcnew System::EventHandler(this, &dvoistvennaya::buttonBack_Click);
			// 
			// dataGridViewTable
			// 
			this->dataGridViewTable->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridViewTable->Location = System::Drawing::Point(20, 130);
			this->dataGridViewTable->Name = L"dataGridViewTable";
			this->dataGridViewTable->Size = System::Drawing::Size(520, 200);
			this->dataGridViewTable->TabIndex = 5;
			// 
			// textBoxResults
			// 
			this->textBoxResults->Location = System::Drawing::Point(20, 350);
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
			this->comboBoxGoal->Location = System::Drawing::Point(150, 110);
			this->comboBoxGoal->Name = L"comboBoxGoal";
			this->comboBoxGoal->Size = System::Drawing::Size(100, 21);
			this->comboBoxGoal->TabIndex = 7;
			// 
			// labelVars
			// 
			this->labelVars->AutoSize = true;
			this->labelVars->Location = System::Drawing::Point(20, 53);
			this->labelVars->Name = L"labelVars";
			this->labelVars->Size = System::Drawing::Size(124, 13);
			this->labelVars->TabIndex = 8;
			this->labelVars->Text = L"Число переменных:";
			// 
			// labelConstraints
			// 
			this->labelConstraints->AutoSize = true;
			this->labelConstraints->Location = System::Drawing::Point(20, 83);
			this->labelConstraints->Name = L"labelConstraints";
			this->labelConstraints->Size = System::Drawing::Size(124, 13);
			this->labelConstraints->TabIndex = 9;
			this->labelConstraints->Text = L"Число ограничений:";
			// 
			// labelGoal
			// 
			this->labelGoal->AutoSize = true;
			this->labelGoal->Location = System::Drawing::Point(20, 113);
			this->labelGoal->Name = L"labelGoal";
			this->labelGoal->Size = System::Drawing::Size(124, 13);
			this->labelGoal->TabIndex = 10;
			this->labelGoal->Text = L"Целевая функция:";
			// 
			// labelTitle
			// 
			this->labelTitle->AutoSize = true;
			this->labelTitle->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->labelTitle->Location = System::Drawing::Point(180, 15);
			this->labelTitle->Name = L"labelTitle";
			this->labelTitle->Size = System::Drawing::Size(200, 20);
			this->labelTitle->TabIndex = 11;
			this->labelTitle->Text = L"Построение двойственной задачи";
			// 
			// dvoistvennaya
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::Silver;
			this->ClientSize = System::Drawing::Size(564, 491);
			this->Controls->Add(this->labelTitle);
			this->Controls->Add(this->labelGoal);
			this->Controls->Add(this->labelConstraints);
			this->Controls->Add(this->labelVars);
			this->Controls->Add(this->comboBoxGoal);
			this->Controls->Add(this->textBoxResults);
			this->Controls->Add(this->dataGridViewTable);
			this->Controls->Add(this->buttonBack);
			this->Controls->Add(this->buttonBuildDual);
			this->Controls->Add(this->buttonCreateTable);
			this->Controls->Add(this->textBoxConstraints);
			this->Controls->Add(this->textBoxVars);
			this->Name = L"dvoistvennaya";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Построение двойственной задачи ЛП";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewTable))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private:
		// Создание таблицы для ввода данных
		void buttonCreateTable_Click(System::Object^ sender, System::EventArgs^ e) {
			try
			{
				int variablesCount = Int32::Parse(textBoxVars->Text);
				int constraintsCount = Int32::Parse(textBoxConstraints->Text);

				if (comboBoxGoal->SelectedIndex == -1)
					comboBoxGoal->SelectedIndex = 0;

				CreateDataGridView(variablesCount, constraintsCount);

				textBoxResults->Text = "Таблица создана. Введите данные прямой задачи:\n";
				textBoxResults->AppendText("- Столбцы x1, x2... - коэффициенты при переменных\n");
				textBoxResults->AppendText("- Столбец 'B' - правые части ограничений\n");
				textBoxResults->AppendText("- Строка 'C' - коэффициенты целевой функции\n");
			}
			catch (Exception^ ex)
			{
				MessageBox::Show("Ошибка при создании таблицы: " + ex->Message);
			}
		}

		// Создание DataGridView для ввода данных
		void CreateDataGridView(int variablesCount, int constraintsCount)
		{
			dataGridViewTable->Rows->Clear();
			dataGridViewTable->Columns->Clear();

			// Добавляем столбцы для переменных
			for (int i = 0; i < variablesCount; i++)
			{
				dataGridViewTable->Columns->Add("x" + (i + 1), "x" + (i + 1));
			}

			// Добавляем столбец для правых частей
			dataGridViewTable->Columns->Add("B", "B");

			// Добавляем строки для ограничений
			for (int i = 0; i < constraintsCount; i++)
			{
				int rowIndex = dataGridViewTable->Rows->Add();
				dataGridViewTable->Rows[rowIndex]->HeaderCell->Value = "Огр. " + (i + 1);
			}

			// Добавляем строку для целевой функции
			dataGridViewTable->Rows->Add();
			dataGridViewTable->Rows[constraintsCount]->HeaderCell->Value = "C";

			// Настройка внешнего вида
			dataGridViewTable->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
		}

		// Построение двойственной задачи
		void buttonBuildDual_Click(System::Object^ sender, System::EventArgs^ e) {
			try
			{
				if (dataGridViewTable->Rows->Count == 0)
				{
					MessageBox::Show("Сначала создайте таблицу!");
					return;
				}

				BuildDualProblem();

			}
			catch (Exception^ ex)
			{
				MessageBox::Show("Ошибка при построении двойственной задачи: " + ex->Message);
			}
		}

		// Построение двойственной задачи
		void BuildDualProblem()
		{
			textBoxResults->Clear();
			textBoxResults->AppendText("=== ПОСТРОЕНИЕ ДВОЙСТВЕННОЙ ЗАДАЧИ ===\n\n");

			// Получаем данные из таблицы
			int variablesCount = Int32::Parse(textBoxVars->Text);
			int constraintsCount = Int32::Parse(textBoxConstraints->Text);
			bool isMaximization = (comboBoxGoal->SelectedIndex == 0);

			// Получаем коэффициенты целевой функции
			array<double>^ objectiveCoefficients = gcnew array<double>(variablesCount);
			for (int j = 0; j < variablesCount; j++)
			{
				String^ value = dataGridViewTable->Rows[constraintsCount]->Cells[j]->Value == nullptr ?
					"" : dataGridViewTable->Rows[constraintsCount]->Cells[j]->Value->ToString();
				if (String::IsNullOrEmpty(value))
					objectiveCoefficients[j] = 0;
				else
					objectiveCoefficients[j] = Double::Parse(value);
			}

			// Получаем матрицу ограничений и правые части
			array<double, 2>^ constraintMatrix = gcnew array<double, 2>(constraintsCount, variablesCount);
			array<double>^ rightHandSide = gcnew array<double>(constraintsCount);

			for (int i = 0; i < constraintsCount; i++)
			{
				for (int j = 0; j < variablesCount; j++)
				{
					String^ value = dataGridViewTable->Rows[i]->Cells[j]->Value == nullptr ?
						"" : dataGridViewTable->Rows[i]->Cells[j]->Value->ToString();
					if (String::IsNullOrEmpty(value))
						constraintMatrix[i, j] = 0;
					else
						constraintMatrix[i, j] = Double::Parse(value);
				}

				String^ bValue = dataGridViewTable->Rows[i]->Cells["B"]->Value == nullptr ?
					"" : dataGridViewTable->Rows[i]->Cells["B"]->Value->ToString();
				if (String::IsNullOrEmpty(bValue))
					rightHandSide[i] = 0;
				else
					rightHandSide[i] = Double::Parse(bValue);
			}

			// Выводим исходную задачу
			textBoxResults->AppendText("ИСХОДНАЯ ЗАДАЧА:\n");
			if (isMaximization)
			{
				textBoxResults->AppendText("Максимизировать: ");
			}
			else
			{
				textBoxResults->AppendText("Минимизировать: ");
			}

			String^ objectiveStr = "";
			for (int j = 0; j < variablesCount; j++)
			{
				if (objectiveCoefficients[j] != 0)
				{
					if (!String::IsNullOrEmpty(objectiveStr))
						objectiveStr += " + ";
					objectiveStr += String::Format("{0:F2}x{1}", objectiveCoefficients[j], j + 1);
				}
			}
			textBoxResults->AppendText(objectiveStr + "\n\n");

			textBoxResults->AppendText("При ограничениях:\n");
			for (int i = 0; i < constraintsCount; i++)
			{
				String^ constraintStr = "";
				for (int j = 0; j < variablesCount; j++)
				{
					if (constraintMatrix[i, j] != 0)
					{
						if (!String::IsNullOrEmpty(constraintStr))
							constraintStr += " + ";
						constraintStr += String::Format("{0:F2}x{1}", constraintMatrix[i, j], j + 1);
					}
				}
				constraintStr += " ≤ " + String::Format("{0:F2}", rightHandSide[i]);
				textBoxResults->AppendText("  " + constraintStr + "\n");
			}

			// Строим двойственную задачу
			textBoxResults->AppendText("\nДВОЙСТВЕННАЯ ЗАДАЧА:\n");

			if (isMaximization)
			{
				textBoxResults->AppendText("Минимизировать: ");
			}
			else
			{
				textBoxResults->AppendText("Максимизировать: ");
			}

			String^ dualObjectiveStr = "";
			for (int i = 0; i < constraintsCount; i++)
			{
				if (rightHandSide[i] != 0)
				{
					if (!String::IsNullOrEmpty(dualObjectiveStr))
						dualObjectiveStr += " + ";
					dualObjectiveStr += String::Format("{0:F2}y{1}", rightHandSide[i], i + 1);
				}
			}
			textBoxResults->AppendText(dualObjectiveStr + "\n\n");

			textBoxResults->AppendText("При ограничениях:\n");
			for (int j = 0; j < variablesCount; j++)
			{
				String^ dualConstraintStr = "";
				for (int i = 0; i < constraintsCount; i++)
				{
					if (constraintMatrix[i, j] != 0)
					{
						if (!String::IsNullOrEmpty(dualConstraintStr))
							dualConstraintStr += " + ";
						dualConstraintStr += String::Format("{0:F2}y{1}", constraintMatrix[i, j], i + 1);
					}
				}

				if (isMaximization)
				{
					dualConstraintStr += " ≥ " + String::Format("{0:F2}", objectiveCoefficients[j]);
				}
				else
				{
					dualConstraintStr += " ≤ " + String::Format("{0:F2}", objectiveCoefficients[j]);
				}
				textBoxResults->AppendText("  " + dualConstraintStr + "\n");
			}

			textBoxResults->AppendText("\n✓ Двойственная задача построена успешно!\n");
		}

		// Кнопка назад
		void buttonBack_Click(System::Object^ sender, System::EventArgs^ e) {
			this->Close();
		}
	};
}