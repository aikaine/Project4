#pragma once
#include <vector>
#include <string>
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Text;

// ================================================
// КЛАСС: построение двойственной задачи
// ================================================
public ref class DualBuilder
{
public:
    static void BuildDualProblem(
        cli::array<double, 2>^ A,
        cli::array<double>^ b,
        cli::array<double>^ c,
        cli::array<int>^ constraintTypes,
        bool isMaximization,
        System::String^% outputText
    )
    {
        if (A == nullptr || b == nullptr || c == nullptr)
            throw gcnew ArgumentNullException("Матрица или векторы не могут быть null");

        int m = A->GetLength(0);
        int n = A->GetLength(1);

        if (b->Length != m || c->Length != n)
            throw gcnew ArgumentException("Размерности A, b и c не согласованы");

        if (constraintTypes == nullptr || constraintTypes->Length != m)
        {
            constraintTypes = gcnew cli::array<int>(m);
            for (int i = 0; i < m; i++)
                constraintTypes[i] = 1;
        }

        // Построение двойственной задачи
        cli::array<double, 2>^ dualA = gcnew cli::array<double, 2>(n, m);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                dualA[i, j] = A[j, i];

        cli::array<double>^ dualC = gcnew cli::array<double>(m);
        for (int i = 0; i < m; i++)
            dualC[i] = b[i];

        cli::array<double>^ dualB = gcnew cli::array<double>(n);
        for (int i = 0; i < n; i++)
            dualB[i] = c[i];

        cli::array<int>^ dualVarSigns = gcnew cli::array<int>(m);
        for (int i = 0; i < m; i++)
        {
            switch (constraintTypes[i])
            {
            case 1: dualVarSigns[i] = 1; break;
            case -1: dualVarSigns[i] = -1; break;
            case 0: dualVarSigns[i] = 0; break;
            default: dualVarSigns[i] = 1; break;
            }
        }

        cli::array<int>^ dualConstraintTypes = gcnew cli::array<int>(n);
        for (int i = 0; i < n; i++)
            dualConstraintTypes[i] = 1;

        bool dualIsMin = isMaximization;

        // Формируем вывод
        StringBuilder^ sb = gcnew StringBuilder();
        sb->AppendLine("=== ДВОЙСТВЕННАЯ ЗАДАЧА ===");
        sb->AppendLine();

        sb->Append("Целевая функция: ");
        sb->Append(dualIsMin ? "min" : "max");
        sb->Append(" g(y) = ");

        bool firstTerm = true;
        for (int i = 0; i < m; i++)
        {
            if (dualC[i] != 0)
            {
                if (!firstTerm)
                    sb->Append(dualC[i] >= 0 ? " + " : " - ");
                else
                    firstTerm = false;

                double absVal = Math::Abs(dualC[i]);
                if (absVal != 1)
                    sb->AppendFormat("{0:F2}", absVal);
                sb->AppendFormat("y{0}", i + 1);
            }
        }
        if (firstTerm) sb->Append("0");
        sb->AppendLine();
        sb->AppendLine();

        sb->AppendLine("Ограничения:");
        for (int i = 0; i < n; i++)
        {
            firstTerm = true;
            for (int j = 0; j < m; j++)
            {
                if (dualA[i, j] != 0)
                {
                    if (!firstTerm)
                        sb->Append(dualA[i, j] >= 0 ? " + " : " - ");
                    else
                        firstTerm = false;

                    double absVal = Math::Abs(dualA[i, j]);
                    if (absVal != 1)
                        sb->AppendFormat("{0:F2}", absVal);
                    sb->AppendFormat("y{0}", j + 1);
                }
            }

            if (firstTerm) sb->Append("0");

            sb->Append(" >= ");
            sb->AppendFormat("{0:F2}", dualB[i]);
            sb->AppendLine();
        }
        sb->AppendLine();

        sb->AppendLine("Условия на переменные:");
        for (int i = 0; i < m; i++)
        {
            switch (dualVarSigns[i])
            {
            case 1: sb->AppendFormat("y{0} >= 0", i + 1); break;
            case -1: sb->AppendFormat("y{0} <= 0", i + 1); break;
            case 0: sb->AppendFormat("y{0} - свободная", i + 1); break;
            }
            sb->AppendLine();
        }

        outputText = sb->ToString();
    }
};

namespace Project4 {

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
        Label^ lblVars;
        Label^ lblCons;
        TextBox^ txtVars;
        TextBox^ txtCons;
        Button^ btnCreate;
        Button^ btnBuildDual;
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
            this->btnBuildDual = gcnew System::Windows::Forms::Button();
            this->btnBack = gcnew System::Windows::Forms::Button();
            this->lblTarget = gcnew System::Windows::Forms::Label();
            this->comboTarget = gcnew System::Windows::Forms::ComboBox();
            this->table = gcnew System::Windows::Forms::DataGridView();
            this->txtOutput = gcnew System::Windows::Forms::TextBox();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->table))->BeginInit();
            this->SuspendLayout();

            // lblVars
            this->lblVars->AutoSize = true;
            this->lblVars->Location = System::Drawing::Point(20, 20);
            this->lblVars->Name = L"lblVars";
            this->lblVars->Size = System::Drawing::Size(124, 13);
            this->lblVars->TabIndex = 0;
            this->lblVars->Text = L"Число переменных:";
            // 
            // txtVars
            // 
            this->txtVars->Location = System::Drawing::Point(150, 17);
            this->txtVars->Name = L"txtVars";
            this->txtVars->Size = System::Drawing::Size(50, 20);
            this->txtVars->TabIndex = 1;
            this->txtVars->Text = L"2";
            // 
            // lblCons
            // 
            this->lblCons->AutoSize = true;
            this->lblCons->Location = System::Drawing::Point(20, 50);
            this->lblCons->Name = L"lblCons";
            this->lblCons->Size = System::Drawing::Size(127, 13);
            this->lblCons->TabIndex = 2;
            this->lblCons->Text = L"Число ограничений:";
            // 
            // txtCons
            // 
            this->txtCons->Location = System::Drawing::Point(150, 47);
            this->txtCons->Name = L"txtCons";
            this->txtCons->Size = System::Drawing::Size(50, 20);
            this->txtCons->TabIndex = 3;
            this->txtCons->Text = L"2";
            // 
            // btnCreate
            // 
            this->btnCreate->BackColor = System::Drawing::Color::LightGreen;
            this->btnCreate->Location = System::Drawing::Point(220, 20);
            this->btnCreate->Name = L"btnCreate";
            this->btnCreate->Size = System::Drawing::Size(120, 40);
            this->btnCreate->TabIndex = 4;
            this->btnCreate->Text = L"Создать таблицу";
            this->btnCreate->UseVisualStyleBackColor = false;
            this->btnCreate->Click += gcnew System::EventHandler(this, &dvoistvennaya::btnCreate_Click);
            // 
            // btnBuildDual
            // 
            this->btnBuildDual->BackColor = System::Drawing::Color::LightSkyBlue;
            this->btnBuildDual->Location = System::Drawing::Point(350, 20);
            this->btnBuildDual->Name = L"btnBuildDual";
            this->btnBuildDual->Size = System::Drawing::Size(120, 40);
            this->btnBuildDual->TabIndex = 5;
            this->btnBuildDual->Text = L"Построить двойственную";
            this->btnBuildDual->UseVisualStyleBackColor = false;
            this->btnBuildDual->Click += gcnew System::EventHandler(this, &dvoistvennaya::btnBuildDual_Click);
            // 
            // btnBack
            // 
            this->btnBack->BackColor = System::Drawing::Color::LightCoral;
            this->btnBack->Location = System::Drawing::Point(480, 20);
            this->btnBack->Name = L"btnBack";
            this->btnBack->Size = System::Drawing::Size(120, 40);
            this->btnBack->TabIndex = 6;
            this->btnBack->Text = L"Назад";
            this->btnBack->UseVisualStyleBackColor = false;
            this->btnBack->Click += gcnew System::EventHandler(this, &dvoistvennaya::btnBack_Click);
            // 
            // lblTarget
            // 
            this->lblTarget->AutoSize = true;
            this->lblTarget->Location = System::Drawing::Point(20, 80);
            this->lblTarget->Name = L"lblTarget";
            this->lblTarget->Size = System::Drawing::Size(116, 13);
            this->lblTarget->TabIndex = 7;
            this->lblTarget->Text = L"Целевая функция:";
            // 
            // comboTarget
            // 
            this->comboTarget->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->comboTarget->FormattingEnabled = true;
            this->comboTarget->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"Max", L"Min" });
            this->comboTarget->Location = System::Drawing::Point(150, 77);
            this->comboTarget->Name = L"comboTarget";
            this->comboTarget->Size = System::Drawing::Size(70, 21);
            this->comboTarget->TabIndex = 8;
            // 
            // table
            // 
            this->table->AllowUserToAddRows = false;
            this->table->BackgroundColor = System::Drawing::Color::Gainsboro;
            this->table->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->table->Location = System::Drawing::Point(20, 110);
            this->table->Name = L"table";
            this->table->Size = System::Drawing::Size(580, 250);
            this->table->TabIndex = 9;
            // 
            // txtOutput
            // 
            this->txtOutput->Location = System::Drawing::Point(20, 370);
            this->txtOutput->Multiline = true;
            this->txtOutput->Name = L"txtOutput";
            this->txtOutput->ReadOnly = true;
            this->txtOutput->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
            this->txtOutput->Size = System::Drawing::Size(580, 80);
            this->txtOutput->TabIndex = 10;
            // 
            // dvoistvennaya
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(630, 470);
            this->Controls->Add(this->txtOutput);
            this->Controls->Add(this->table);
            this->Controls->Add(this->comboTarget);
            this->Controls->Add(this->lblTarget);
            this->Controls->Add(this->btnBack);
            this->Controls->Add(this->btnBuildDual);
            this->Controls->Add(this->btnCreate);
            this->Controls->Add(this->txtCons);
            this->Controls->Add(this->txtVars);
            this->Controls->Add(this->lblCons);
            this->Controls->Add(this->lblVars);
            this->Name = L"dvoistvennaya";
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
            this->Text = L"Построение двойственной задачи";
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->table))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion

    private:
        //----------------------------------------------------------
        // Кнопка "Создать таблицу"
        //----------------------------------------------------------
        System::Void btnCreate_Click(System::Object^ sender, System::EventArgs^ e)
        {
            try
            {
                int vars = Convert::ToInt32(txtVars->Text);
                int cons = Convert::ToInt32(txtCons->Text);

                table->Columns->Clear();
                table->Rows->Clear();
                table->AllowUserToAddRows = false;

                // создаём столбцы для переменных
                for (int i = 0; i < vars; i++)
                    table->Columns->Add("x" + i, "x" + i);

                // столбец для свободных членов
                table->Columns->Add("b", "Св. член");

                // столбец для типов ограничений
                DataGridViewComboBoxColumn^ typeColumn = gcnew DataGridViewComboBoxColumn();
                typeColumn->HeaderText = "Тип";
                typeColumn->Name = "Type";
                typeColumn->Items->AddRange("<=", "=", ">=");
                typeColumn->DefaultCellStyle->NullValue = "<=";
                table->Columns->Add(typeColumn);

                // добавляем строки для ограничений
                for (int i = 0; i < cons; i++)
                {
                    int rowIndex = table->Rows->Add();
                    table->Rows[rowIndex]->HeaderCell->Value = "Огр." + (i + 1);
                    table->Rows[rowIndex]->Cells["Type"]->Value = "<=";
                }

                // строка для целевой функции
                int funcRowIndex = table->Rows->Add();
                table->Rows[funcRowIndex]->HeaderCell->Value = "F";
                table->Rows[funcRowIndex]->Cells["Type"]->ReadOnly = true;
                table->Rows[funcRowIndex]->Cells["Type"]->Style->BackColor = Color::LightGray;

                // выбираем тип оптимизации по умолчанию
                if (comboTarget->Items->Count > 0 && comboTarget->SelectedIndex == -1)
                    comboTarget->SelectedIndex = 0;

                txtOutput->Text = "Таблица создана. Заполните коэффициенты прямой задачи.";
            }
            catch (Exception^ ex)
            {
                txtOutput->Text = "Ошибка при создании таблицы: " + ex->Message;
            }
        }

        //----------------------------------------------------------
        // Кнопка "Построить двойственную"
        //----------------------------------------------------------
        System::Void btnBuildDual_Click(System::Object^ sender, System::EventArgs^ e)
        {
            try
            {
                int vars = Convert::ToInt32(txtVars->Text);
                int cons = Convert::ToInt32(txtCons->Text);
                bool isMax = (comboTarget->SelectedItem->ToString() == "Max");

                // Проверяем, что таблица создана
                int expectedRows = cons + 1;
                if (table->Rows->Count != expectedRows)
                {
                    txtOutput->Text = "Сначала создайте таблицу с правильным количеством строк!";
                    return;
                }

                // Проверяем, что все ячейки заполнены
                for (int i = 0; i < expectedRows; i++)
                {
                    for (int j = 0; j < vars + 1; j++) // +1 для столбца свободных членов
                    {
                        if (table->Rows[i]->Cells[j]->Value == nullptr ||
                            String::IsNullOrEmpty(table->Rows[i]->Cells[j]->Value->ToString()))
                        {
                            if (i == cons && j == vars)
                            {
                                // Свободный член в целевой функции может быть пустым
                                table->Rows[i]->Cells[j]->Value = "0";
                            }
                            else
                            {
                                txtOutput->Text = "Заполните все ячейки таблицы! Пустая ячейка: строка " +
                                    (i + 1) + ", столбец " + (j + 1);
                                return;
                            }
                        }
                    }
                }

                // Подготавливаем данные для двойственной задачи
                array<double>^ c = gcnew array<double>(vars);
                array<double, 2>^ A = gcnew array<double, 2>(cons, vars);
                array<double>^ b = gcnew array<double>(cons);
                array<int>^ types = gcnew array<int>(cons);

                // Целевая функция (последняя строка таблицы)
                for (int j = 0; j < vars; j++)
                {
                    String^ valueStr = table->Rows[cons]->Cells[j]->Value->ToString();
                    String^ cleanedValueStr = valueStr->Replace(",", ".");
                    c[j] = Convert::ToDouble(cleanedValueStr);
                }

                // Ограничения
                for (int i = 0; i < cons; i++)
                {
                    for (int j = 0; j < vars; j++)
                    {
                        String^ valueStr = table->Rows[i]->Cells[j]->Value->ToString();
                        String^ cleanedValueStr = valueStr->Replace(",", ".");
                        A[i, j] = Convert::ToDouble(cleanedValueStr);
                    }

                    // Свободные члены
                    String^ bStr = table->Rows[i]->Cells[vars]->Value->ToString();
                    String^ cleanedBStr = bStr->Replace(",", ".");
                    b[i] = Convert::ToDouble(cleanedBStr);

                    // Типы ограничений
                    String^ typeStr = table->Rows[i]->Cells["Type"]->Value->ToString();
                    if (typeStr == "<=") types[i] = 1;
                    else if (typeStr == ">=") types[i] = -1;
                    else if (typeStr == "=") types[i] = 0;
                    else types[i] = 1;
                }

                String^ result;
                DualBuilder::BuildDualProblem(A, b, c, types, isMax, result);
                txtOutput->Text = result;
            }
            catch (FormatException^)
            {
                txtOutput->Text = "Ошибка: введены некорректные числовые значения! Убедитесь, что все ячейки содержат числа.";
            }
            catch (Exception^ ex)
            {
                txtOutput->Text = "Ошибка при построении двойственной задачи: " + ex->Message;
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