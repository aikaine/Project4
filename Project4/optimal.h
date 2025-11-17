#pragma once

namespace Project4 {

    // Управляемая структура для результатов проверки оптимальности
    public value class PlanCheckResult {
    public:
        array<double>^ u;                    // potentials for rows
        array<double>^ v;                    // potentials for cols  
        array<double, 2>^ delta;             // reduced costs
        bool optimal;
        int enteringI;
        int enteringJ;

        PlanCheckResult(int m, int n) {
            u = gcnew array<double>(m);
            v = gcnew array<double>(n);
            delta = gcnew array<double, 2>(m, n);
            optimal = false;
            enteringI = -1;
            enteringJ = -1;

            // Инициализация нулями
            for (int i = 0; i < m; i++) u[i] = 0.0;
            for (int j = 0; j < n; j++) v[j] = 0.0;
            for (int i = 0; i < m; i++)
                for (int j = 0; j < n; j++)
                    delta[i, j] = 0.0;
        }
    };

    // Класс формы проверки оптимальности
    public ref class optimal : public System::Windows::Forms::Form
    {
    public:
        optimal(void)
        {
            InitializeComponent();
            InitializeDataGrids();
        }

    protected:
        ~optimal()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        // Элементы управления
        System::Windows::Forms::TextBox^ textBoxSuppliers;
        System::Windows::Forms::TextBox^ textBoxConsumers;
        System::Windows::Forms::Button^ buttonCreateTables;
        System::Windows::Forms::Button^ buttonCheckOptimality;
        System::Windows::Forms::Button^ buttonBack;
        System::Windows::Forms::DataGridView^ dataGridViewCost;
        System::Windows::Forms::DataGridView^ dataGridViewAllocation;
        System::Windows::Forms::RichTextBox^ textBoxResults;
        System::Windows::Forms::Label^ labelSuppliers;
        System::Windows::Forms::Label^ labelConsumers;
        System::Windows::Forms::Label^ labelCost;
        System::Windows::Forms::Label^ labelAllocation;

        // Данные
        int suppliersCount;
        int consumersCount;

        System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->textBoxSuppliers = (gcnew System::Windows::Forms::TextBox());
            this->textBoxConsumers = (gcnew System::Windows::Forms::TextBox());
            this->buttonCreateTables = (gcnew System::Windows::Forms::Button());
            this->buttonCheckOptimality = (gcnew System::Windows::Forms::Button());
            this->buttonBack = (gcnew System::Windows::Forms::Button());
            this->dataGridViewCost = (gcnew System::Windows::Forms::DataGridView());
            this->dataGridViewAllocation = (gcnew System::Windows::Forms::DataGridView());
            this->textBoxResults = (gcnew System::Windows::Forms::RichTextBox());
            this->labelSuppliers = (gcnew System::Windows::Forms::Label());
            this->labelConsumers = (gcnew System::Windows::Forms::Label());
            this->labelCost = (gcnew System::Windows::Forms::Label());
            this->labelAllocation = (gcnew System::Windows::Forms::Label());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewCost))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewAllocation))->BeginInit();
            this->SuspendLayout();
            // 
            // textBoxSuppliers
            // 
            this->textBoxSuppliers->Location = System::Drawing::Point(120, 20);
            this->textBoxSuppliers->Name = L"textBoxSuppliers";
            this->textBoxSuppliers->Size = System::Drawing::Size(50, 20);
            this->textBoxSuppliers->TabIndex = 0;
            this->textBoxSuppliers->Text = L"3";
            // 
            // textBoxConsumers
            // 
            this->textBoxConsumers->Location = System::Drawing::Point(120, 50);
            this->textBoxConsumers->Name = L"textBoxConsumers";
            this->textBoxConsumers->Size = System::Drawing::Size(50, 20);
            this->textBoxConsumers->TabIndex = 1;
            this->textBoxConsumers->Text = L"3";
            // 
            // buttonCreateTables
            // 
            this->buttonCreateTables->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)),
                static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)));
            this->buttonCreateTables->Location = System::Drawing::Point(190, 20);
            this->buttonCreateTables->Name = L"buttonCreateTables";
            this->buttonCreateTables->Size = System::Drawing::Size(120, 50);
            this->buttonCreateTables->TabIndex = 2;
            this->buttonCreateTables->Text = L"Создать таблицы";
            this->buttonCreateTables->UseVisualStyleBackColor = false;
            this->buttonCreateTables->Click += gcnew System::EventHandler(this, &optimal::buttonCreateTables_Click);
            // 
            // buttonCheckOptimality
            // 
            this->buttonCheckOptimality->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)),
                static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(255)));
            this->buttonCheckOptimality->Location = System::Drawing::Point(320, 20);
            this->buttonCheckOptimality->Name = L"buttonCheckOptimality";
            this->buttonCheckOptimality->Size = System::Drawing::Size(120, 50);
            this->buttonCheckOptimality->TabIndex = 3;
            this->buttonCheckOptimality->Text = L"Проверить оптимальность";
            this->buttonCheckOptimality->UseVisualStyleBackColor = false;
            this->buttonCheckOptimality->Click += gcnew System::EventHandler(this, &optimal::buttonCheckOptimality_Click);
            // 
            // buttonBack
            // 
            this->buttonBack->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
                static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(192)));
            this->buttonBack->Location = System::Drawing::Point(450, 20);
            this->buttonBack->Name = L"buttonBack";
            this->buttonBack->Size = System::Drawing::Size(120, 50);
            this->buttonBack->TabIndex = 4;
            this->buttonBack->Text = L"Назад";
            this->buttonBack->UseVisualStyleBackColor = false;
            this->buttonBack->Click += gcnew System::EventHandler(this, &optimal::buttonBack_Click);
            // 
            // dataGridViewCost
            // 
            this->dataGridViewCost->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->dataGridViewCost->Location = System::Drawing::Point(20, 120);
            this->dataGridViewCost->Name = L"dataGridViewCost";
            this->dataGridViewCost->Size = System::Drawing::Size(350, 200);
            this->dataGridViewCost->TabIndex = 5;
            // 
            // dataGridViewAllocation
            // 
            this->dataGridViewAllocation->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->dataGridViewAllocation->Location = System::Drawing::Point(390, 120);
            this->dataGridViewAllocation->Name = L"dataGridViewAllocation";
            this->dataGridViewAllocation->Size = System::Drawing::Size(350, 200);
            this->dataGridViewAllocation->TabIndex = 6;
            // 
            // textBoxResults
            // 
            this->textBoxResults->Location = System::Drawing::Point(20, 340);
            this->textBoxResults->Name = L"textBoxResults";
            this->textBoxResults->Size = System::Drawing::Size(720, 150);
            this->textBoxResults->TabIndex = 7;
            this->textBoxResults->Text = L"";
            // 
            // labelSuppliers
            // 
            this->labelSuppliers->AutoSize = true;
            this->labelSuppliers->Location = System::Drawing::Point(20, 23);
            this->labelSuppliers->Name = L"labelSuppliers";
            this->labelSuppliers->Size = System::Drawing::Size(94, 13);
            this->labelSuppliers->TabIndex = 8;
            this->labelSuppliers->Text = L"Поставщиков:";
            // 
            // labelConsumers
            // 
            this->labelConsumers->AutoSize = true;
            this->labelConsumers->Location = System::Drawing::Point(20, 53);
            this->labelConsumers->Name = L"labelConsumers";
            this->labelConsumers->Size = System::Drawing::Size(94, 13);
            this->labelConsumers->TabIndex = 9;
            this->labelConsumers->Text = L"Потребителей:";
            // 
            // labelCost
            // 
            this->labelCost->AutoSize = true;
            this->labelCost->Location = System::Drawing::Point(20, 100);
            this->labelCost->Name = L"labelCost";
            this->labelCost->Size = System::Drawing::Size(124, 13);
            this->labelCost->TabIndex = 10;
            this->labelCost->Text = L"Матрица стоимостей:";
            // 
            // labelAllocation
            // 
            this->labelAllocation->AutoSize = true;
            this->labelAllocation->Location = System::Drawing::Point(390, 100);
            this->labelAllocation->Name = L"labelAllocation";
            this->labelAllocation->Size = System::Drawing::Size(139, 13);
            this->labelAllocation->TabIndex = 11;
            this->labelAllocation->Text = L"Базисный план (перевозки):";
            // 
            // optimal
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->BackColor = System::Drawing::Color::Silver;
            this->ClientSize = System::Drawing::Size(764, 511);
            this->Controls->Add(this->labelAllocation);
            this->Controls->Add(this->labelCost);
            this->Controls->Add(this->labelConsumers);
            this->Controls->Add(this->labelSuppliers);
            this->Controls->Add(this->textBoxResults);
            this->Controls->Add(this->dataGridViewAllocation);
            this->Controls->Add(this->dataGridViewCost);
            this->Controls->Add(this->buttonBack);
            this->Controls->Add(this->buttonCheckOptimality);
            this->Controls->Add(this->buttonCreateTables);
            this->Controls->Add(this->textBoxConsumers);
            this->Controls->Add(this->textBoxSuppliers);
            this->Name = L"optimal";
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
            this->Text = L"Проверка оптимальности транспортной задачи";
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewCost))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewAllocation))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion

    private:
        void InitializeDataGrids()
        {
            textBoxSuppliers->Text = "3";
            textBoxConsumers->Text = "3";
        }

        void buttonCreateTables_Click(System::Object^ sender, System::EventArgs^ e) {
            try
            {
                suppliersCount = Int32::Parse(textBoxSuppliers->Text);
                consumersCount = Int32::Parse(textBoxConsumers->Text);

                CreateCostTable();
                CreateAllocationTable();

                textBoxResults->Text = "Таблицы созданы. Заполните данные:\n";
                textBoxResults->AppendText("- Левая таблица: стоимости перевозок\n");
                textBoxResults->AppendText("- Правая таблица: базисный план (перевозки > 0)\n");
                textBoxResults->AppendText("- Базисный план должен быть допустимым и невырожденным\n");
            }
            catch (Exception^ ex)
            {
                MessageBox::Show("Ошибка при создании таблиц: " + ex->Message);
            }
        }

        void CreateCostTable()
        {
            dataGridViewCost->Rows->Clear();
            dataGridViewCost->Columns->Clear();

            for (int j = 0; j < consumersCount; j++)
            {
                dataGridViewCost->Columns->Add("C" + (j + 1), "C" + (j + 1));
            }

            for (int i = 0; i < suppliersCount; i++)
            {
                dataGridViewCost->Rows->Add();
                dataGridViewCost->Rows[i]->HeaderCell->Value = "S" + (i + 1);
            }

            dataGridViewCost->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
        }

        void CreateAllocationTable()
        {
            dataGridViewAllocation->Rows->Clear();
            dataGridViewAllocation->Columns->Clear();

            for (int j = 0; j < consumersCount; j++)
            {
                dataGridViewAllocation->Columns->Add("C" + (j + 1), "C" + (j + 1));
            }

            for (int i = 0; i < suppliersCount; i++)
            {
                dataGridViewAllocation->Rows->Add();
                dataGridViewAllocation->Rows[i]->HeaderCell->Value = "S" + (i + 1);
            }

            dataGridViewAllocation->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
        }

        void buttonCheckOptimality_Click(System::Object^ sender, System::EventArgs^ e) {
            try
            {
                if (dataGridViewCost->Rows->Count == 0 || dataGridViewAllocation->Rows->Count == 0)
                {
                    MessageBox::Show("Сначала создайте таблицы!");
                    return;
                }

                CheckTransportationOptimality();

            }
            catch (Exception^ ex)
            {
                MessageBox::Show("Ошибка при проверке оптимальности: " + ex->Message);
            }
        }

        void CheckTransportationOptimality()
        {
            textBoxResults->Clear();
            textBoxResults->AppendText("=== ПРОВЕРКА ОПТИМАЛЬНОСТИ ТРАНСПОРТНОЙ ЗАДАЧИ ===\n\n");

            // Получаем данные из таблиц
            auto costMatrix = GetCostMatrix();
            auto allocationMatrix = GetAllocationMatrix();
            auto basicMask = GetBasicMask(allocationMatrix);

            // Проверяем оптимальность
            auto result = PerformOptimalityCheck(costMatrix, basicMask);

            // Выводим результаты
            DisplayResults(result, basicMask);
        }

        // Функция проверки оптимальности (метод потенциалов)
        PlanCheckResult PerformOptimalityCheck(array<double, 2>^ cost, array<bool, 2>^ basicMask)
        {
            int m = suppliersCount;
            int n = consumersCount;
            PlanCheckResult result(m, n);

            if (m == 0 || n == 0) return result;

            // Инициализация потенциалов (используем массивы для отслеживания вычисленных)
            array<bool>^ uCalculated = gcnew array<bool>(m);
            array<bool>^ vCalculated = gcnew array<bool>(n);

            for (int i = 0; i < m; i++) {
                result.u[i] = 0.0;
                uCalculated[i] = false;
            }
            for (int j = 0; j < n; j++) {
                result.v[j] = 0.0;
                vCalculated[j] = false;
            }

            // Начинаем с первого поставщика
            result.u[0] = 0.0;
            uCalculated[0] = true;

            // Вычисление потенциалов
            for (int iter = 0; iter < m + n; ++iter) {
                bool updated = false;
                for (int i = 0; i < m; ++i) {
                    for (int j = 0; j < n; ++j) {
                        if (!basicMask[i, j]) continue;
                        if (uCalculated[i] && !vCalculated[j]) {
                            result.v[j] = cost[i, j] - result.u[i];
                            vCalculated[j] = true;
                            updated = true;
                        }
                        else if (!uCalculated[i] && vCalculated[j]) {
                            result.u[i] = cost[i, j] - result.v[j];
                            uCalculated[i] = true;
                            updated = true;
                        }
                    }
                }
                if (!updated) break;
            }

            // Вычисление оценок
            bool optimal = true;
            double mostNeg = 0.0;
            result.enteringI = -1;
            result.enteringJ = -1;

            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    result.delta[i, j] = cost[i, j] - (result.u[i] + result.v[j]);
                    if (result.delta[i, j] < -1e-9) {
                        if (optimal) {
                            optimal = false;
                            mostNeg = result.delta[i, j];
                            result.enteringI = i;
                            result.enteringJ = j;
                        }
                        else if (result.delta[i, j] < mostNeg) {
                            mostNeg = result.delta[i, j];
                            result.enteringI = i;
                            result.enteringJ = j;
                        }
                    }
                }
            }

            result.optimal = optimal;
            return result;
        }

        void DisplayResults(PlanCheckResult result, array<bool, 2>^ basicMask)
        {
            textBoxResults->AppendText("РЕЗУЛЬТАТЫ ПРОВЕРКИ:\n");
            textBoxResults->AppendText("────────────────────\n");

            if (result.optimal)
            {
                textBoxResults->AppendText("✓ План ОПТИМАЛЕН!\n\n");
            }
            else
            {
                textBoxResults->AppendText("✗ План НЕ оптимален!\n");
                textBoxResults->AppendText(String::Format("Вводимая клетка: S{0}-C{1}\n\n",
                    result.enteringI + 1, result.enteringJ + 1));
            }

            // Выводим потенциалы
            textBoxResults->AppendText("ПОТЕНЦИАЛЫ:\n");
            textBoxResults->AppendText("───────────\n");

            textBoxResults->AppendText("Поставщики (u):\n");
            for (int i = 0; i < suppliersCount; i++)
            {
                textBoxResults->AppendText(String::Format("  u{0} = {1:F2}\n", i + 1, result.u[i]));
            }

            textBoxResults->AppendText("\nПотребители (v):\n");
            for (int j = 0; j < consumersCount; j++)
            {
                textBoxResults->AppendText(String::Format("  v{0} = {1:F2}\n", j + 1, result.v[j]));
            }

            // Выводим оценки
            textBoxResults->AppendText("\nОЦЕНКИ КЛЕТОК (Δ):\n");
            textBoxResults->AppendText("─────────────────\n");

            for (int i = 0; i < suppliersCount; i++)
            {
                for (int j = 0; j < consumersCount; j++)
                {
                    String^ cellType = basicMask[i, j] ? "[Баз.]" : "[Св.]";
                    String^ sign = result.delta[i, j] < -1e-9 ? "✗" : (result.delta[i, j] > 1e-9 ? " " : "✓");
                    textBoxResults->AppendText(String::Format("  S{0}-C{1} {2} Δ = {3,7:F2} {4}\n",
                        i + 1, j + 1, cellType, result.delta[i, j], sign));
                }
            }

            // Выводим рекомендации
            textBoxResults->AppendText("\nРЕКОМЕНДАЦИИ:\n");
            textBoxResults->AppendText("─────────────\n");

            if (result.optimal)
            {
                textBoxResults->AppendText("✓ Все оценки неотрицательные\n");
                textBoxResults->AppendText("✓ Текущий план является оптимальным\n");
                textBoxResults->AppendText("✓ Дальнейшие улучшения не требуются\n");
            }
            else
            {
                textBoxResults->AppendText("✗ Найдены отрицательные оценки\n");
                textBoxResults->AppendText(String::Format("✗ Рекомендуется ввести клетку S{0}-C{1} в базис\n",
                    result.enteringI + 1, result.enteringJ + 1));
                textBoxResults->AppendText("✗ Выполните пересчет по циклу для улучшения плана\n");
            }
        }

        // Вспомогательные методы для получения данных
        array<double, 2>^ GetCostMatrix()
        {
            array<double, 2>^ cost = gcnew array<double, 2>(suppliersCount, consumersCount);

            for (int i = 0; i < suppliersCount; i++)
            {
                for (int j = 0; j < consumersCount; j++)
                {
                    String^ value = dataGridViewCost->Rows[i]->Cells[j]->Value == nullptr ?
                        "" : dataGridViewCost->Rows[i]->Cells[j]->Value->ToString();
                    if (String::IsNullOrEmpty(value))
                        cost[i, j] = 0;
                    else
                        cost[i, j] = Double::Parse(value);
                }
            }

            return cost;
        }

        array<double, 2>^ GetAllocationMatrix()
        {
            array<double, 2>^ allocation = gcnew array<double, 2>(suppliersCount, consumersCount);

            for (int i = 0; i < suppliersCount; i++)
            {
                for (int j = 0; j < consumersCount; j++)
                {
                    String^ value = dataGridViewAllocation->Rows[i]->Cells[j]->Value == nullptr ?
                        "" : dataGridViewAllocation->Rows[i]->Cells[j]->Value->ToString();
                    if (String::IsNullOrEmpty(value))
                        allocation[i, j] = 0;
                    else
                        allocation[i, j] = Double::Parse(value);
                }
            }

            return allocation;
        }

        array<bool, 2>^ GetBasicMask(array<double, 2>^ allocation)
        {
            array<bool, 2>^ basicMask = gcnew array<bool, 2>(suppliersCount, consumersCount);

            for (int i = 0; i < suppliersCount; i++)
            {
                for (int j = 0; j < consumersCount; j++)
                {
                    basicMask[i, j] = (allocation[i, j] > 1e-9);
                }
            }

            return basicMask;
        }

        void buttonBack_Click(System::Object^ sender, System::EventArgs^ e) {
            this->Close();
        }
    };
}