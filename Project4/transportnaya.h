#pragma once
#include <vector>
#include <algorithm>
#include <limits>
#include <iostream>
#include <tuple> 
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Text;

using namespace std;

// ============= STRUCT TransportData ===============
struct TransportData {
    vector<int> a, b;
    vector<vector<int>> c;
    bool balanced;
    int balanceDiff;

    TransportData(vector<int> a, vector<int> b, vector<vector<int>> c)
        : a(a), b(b), c(c) {
        balanced = checkBalance();
    }

    bool checkBalance() {
        int totalA = 0, totalB = 0;
        for (int val : a) totalA += val;
        for (int val : b) totalB += val;
        balanceDiff = totalB - totalA;
        return totalA == totalB;
    }

    void autoBalance() {
        if (balanced) return;

        if (balanceDiff > 0) {
            a.push_back(balanceDiff);
            vector<int> newRow(b.size(), 0);
            c.push_back(newRow);
        }
        else {
            b.push_back(-balanceDiff);
            for (int i = 0; i < c.size(); i++) {
                c[i].push_back(0);
            }
        }
        balanced = true;
    }
};

// ============= CLASS TransportPlan =================
class TransportPlan {
public:
    TransportData data;
    vector<vector<int>> x;
    bool usedFictitiousSupplier;
    bool usedFictitiousConsumer;

    TransportPlan(TransportData d) : data(d) {
        usedFictitiousSupplier = false;
        usedFictitiousConsumer = false;

        data.autoBalance();

        int n = data.a.size();
        int m = data.b.size();
        x.assign(n, vector<int>(m, 0));

        if (data.balanceDiff > 0) usedFictitiousSupplier = true;
        if (data.balanceDiff < 0) usedFictitiousConsumer = true;
    }

    // Северо-западный угол
    void northwest() {
        vector<int> a = data.a, b = data.b;

        int i = 0, j = 0;
        while (i < a.size() && j < b.size()) {
            int val = min(a[i], b[j]);
            x[i][j] = val;

            a[i] -= val;
            b[j] -= val;

            if (a[i] == 0) i++;
            if (b[j] == 0) j++;
        }
    }

    // Метод минимального элемента (альтернатива С-З углу)
    void minimalCost() {
        vector<int> a = data.a;
        vector<int> b = data.b;

        // Создаем список всех ячеек, отсортированных по стоимости
        vector<tuple<int, int, int>> cells; // (cost, i, j)
        for (int i = 0; i < data.a.size(); i++) {
            for (int j = 0; j < data.b.size(); j++) {
                cells.push_back(make_tuple(data.c[i][j], i, j));
            }
        }

        // Сортируем по возрастанию стоимости
        sort(cells.begin(), cells.end());

        // Заполняем начиная с самых дешевых ячеек
        for (auto& cell : cells) {
            int cost_val = get<0>(cell);
            int i = get<1>(cell);
            int j = get<2>(cell);

            if (a[i] > 0 && b[j] > 0) {
                int val = min(a[i], b[j]);
                x[i][j] = val;
                a[i] -= val;
                b[j] -= val;
            }
        }
    }

    // Упрощенный но надежный метод улучшения
    void simpleImprovement() {
        int n = data.a.size();
        int m = data.b.size();

        // Сохраняем исходный план
        vector<vector<int>> original = x;
        int originalCost = cost();

        // Пробуем несколько случайных перестановок
        for (int attempt = 0; attempt < 100; attempt++) {
            // Выбираем случайный цикл 2x2
            int i1 = rand() % n;
            int i2 = (i1 + 1 + rand() % (n - 1)) % n;
            int j1 = rand() % m;
            int j2 = (j1 + 1 + rand() % (m - 1)) % m;

            if (i1 == i2 || j1 == j2) continue;

            // Проверяем, можно ли улучшить этот квадрат
            if (x[i1][j1] > 0 && x[i2][j2] > 0 &&
                x[i1][j2] == 0 && x[i2][j1] == 0) {

                int currentCost = data.c[i1][j1] + data.c[i2][j2];
                int newCost = data.c[i1][j2] + data.c[i2][j1];

                if (newCost < currentCost) {
                    // Находим максимально возможный transfer
                    int transfer = min(x[i1][j1], x[i2][j2]);

                    // Сохраняем текущее состояние
                    vector<vector<int>> backup = x;

                    // Применяем изменение
                    x[i1][j1] -= transfer;
                    x[i2][j2] -= transfer;
                    x[i1][j2] += transfer;
                    x[i2][j1] += transfer;

                    // Проверяем, что баланс сохранился
                    if (!isValid()) {
                        // Откатываем изменения если баланс нарушен
                        x = backup;
                    }
                }
            }
        }
    }

    // Комбинированная оптимизация
    void fullOptimize() {
        // Сохраняем лучший план
        vector<vector<int>> bestPlan = x;
        int bestCost = cost();

        // Пробуем разные начальные планы
        for (int method = 0; method < 2; method++) {
            // Сбрасываем план
            for (int i = 0; i < x.size(); i++)
                for (int j = 0; j < x[i].size(); j++)
                    x[i][j] = 0;

            // Строим начальный план
            if (method == 0) {
                northwest();
            }
            else {
                minimalCost();
            }

            // Улучшаем
            simpleImprovement();

            // Сохраняем если лучше
            int currentCost = cost();
            if (currentCost < bestCost && isValid()) {
                bestCost = currentCost;
                bestPlan = x;
            }
        }

        x = bestPlan;
    }

    int cost() {
        int sum = 0;
        for (int i = 0; i < x.size(); i++)
            for (int j = 0; j < x[i].size(); j++)
                sum += x[i][j] * data.c[i][j];
        return sum;
    }

    // Проверка корректности плана
    bool isValid() {
        int n = data.a.size();
        int m = data.b.size();

        // Проверка выполнения ограничений
        for (int i = 0; i < n; i++) {
            int rowSum = 0;
            for (int j = 0; j < m; j++) rowSum += x[i][j];
            if (rowSum != data.a[i]) return false;
        }

        for (int j = 0; j < m; j++) {
            int colSum = 0;
            for (int i = 0; i < n; i++) colSum += x[i][j];
            if (colSum != data.b[j]) return false;
        }

        return true;
    }
};

namespace Project4 {

    public ref class transportnaya : public System::Windows::Forms::Form
    {
    public:
        transportnaya(void)
        {
            InitializeComponent();
        }

    protected:
        ~transportnaya()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        Label^ lblSuppliers;
        Label^ lblConsumers;
        TextBox^ txtSuppliers;
        TextBox^ txtConsumers;
        Button^ btnCreateTable;
        Button^ btnNorthwest;
        Button^ btnMinimal;
        Button^ btnOptimize;
        Button^ btnBack;
        DataGridView^ tableSupply;
        DataGridView^ tableCost;
        DataGridView^ tableResult;
        TextBox^ txtOutput;
        System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->lblSuppliers = gcnew System::Windows::Forms::Label();
            this->lblConsumers = gcnew System::Windows::Forms::Label();
            this->txtSuppliers = gcnew System::Windows::Forms::TextBox();
            this->txtConsumers = gcnew System::Windows::Forms::TextBox();
            this->btnCreateTable = gcnew System::Windows::Forms::Button();
            this->btnNorthwest = gcnew System::Windows::Forms::Button();
            this->btnMinimal = gcnew System::Windows::Forms::Button();
            this->btnOptimize = gcnew System::Windows::Forms::Button();
            this->btnBack = gcnew System::Windows::Forms::Button();
            this->tableSupply = gcnew System::Windows::Forms::DataGridView();
            this->tableCost = gcnew System::Windows::Forms::DataGridView();
            this->tableResult = gcnew System::Windows::Forms::DataGridView();
            this->txtOutput = gcnew System::Windows::Forms::TextBox();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tableSupply))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tableCost))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tableResult))->BeginInit();
            this->SuspendLayout();

            // lblSuppliers
            this->lblSuppliers->AutoSize = true;
            this->lblSuppliers->Location = System::Drawing::Point(20, 20);
            this->lblSuppliers->Name = L"lblSuppliers";
            this->lblSuppliers->Size = System::Drawing::Size(95, 13);
            this->lblSuppliers->TabIndex = 0;
            this->lblSuppliers->Text = L"Поставщиков:";
            // 
            // txtSuppliers
            // 
            this->txtSuppliers->Location = System::Drawing::Point(120, 17);
            this->txtSuppliers->Name = L"txtSuppliers";
            this->txtSuppliers->Size = System::Drawing::Size(50, 20);
            this->txtSuppliers->TabIndex = 1;
            this->txtSuppliers->Text = L"3";
            // 
            // lblConsumers
            // 
            this->lblConsumers->AutoSize = true;
            this->lblConsumers->Location = System::Drawing::Point(20, 50);
            this->lblConsumers->Name = L"lblConsumers";
            this->lblConsumers->Size = System::Drawing::Size(94, 13);
            this->lblConsumers->TabIndex = 2;
            this->lblConsumers->Text = L"Потребителей:";
            // 
            // txtConsumers
            // 
            this->txtConsumers->Location = System::Drawing::Point(120, 47);
            this->txtConsumers->Name = L"txtConsumers";
            this->txtConsumers->Size = System::Drawing::Size(50, 20);
            this->txtConsumers->TabIndex = 3;
            this->txtConsumers->Text = L"5";
            // 
            // btnCreateTable
            // 
            this->btnCreateTable->BackColor = System::Drawing::Color::LightGreen;
            this->btnCreateTable->Location = System::Drawing::Point(190, 20);
            this->btnCreateTable->Name = L"btnCreateTable";
            this->btnCreateTable->Size = System::Drawing::Size(100, 40);
            this->btnCreateTable->TabIndex = 4;
            this->btnCreateTable->Text = L"Создать таблицы";
            this->btnCreateTable->UseVisualStyleBackColor = false;
            this->btnCreateTable->Click += gcnew System::EventHandler(this, &transportnaya::btnCreateTable_Click);
            // 
            // btnNorthwest
            // 
            this->btnNorthwest->BackColor = System::Drawing::Color::LightSkyBlue;
            this->btnNorthwest->Location = System::Drawing::Point(300, 20);
            this->btnNorthwest->Name = L"btnNorthwest";
            this->btnNorthwest->Size = System::Drawing::Size(100, 40);
            this->btnNorthwest->TabIndex = 5;
            this->btnNorthwest->Text = L"С-З угол";
            this->btnNorthwest->UseVisualStyleBackColor = false;
            this->btnNorthwest->Click += gcnew System::EventHandler(this, &transportnaya::btnNorthwest_Click);
            // 
            // btnMinimal
            // 
            this->btnMinimal->BackColor = System::Drawing::Color::LightSkyBlue;
            this->btnMinimal->Location = System::Drawing::Point(410, 20);
            this->btnMinimal->Name = L"btnMinimal";
            this->btnMinimal->Size = System::Drawing::Size(100, 40);
            this->btnMinimal->TabIndex = 6;
            this->btnMinimal->Text = L"Мин. стоимость";
            this->btnMinimal->UseVisualStyleBackColor = false;
            this->btnMinimal->Click += gcnew System::EventHandler(this, &transportnaya::btnMinimal_Click);
            // 
            // btnOptimize
            // 
            this->btnOptimize->BackColor = System::Drawing::Color::Gold;
            this->btnOptimize->Location = System::Drawing::Point(520, 20);
            this->btnOptimize->Name = L"btnOptimize";
            this->btnOptimize->Size = System::Drawing::Size(100, 40);
            this->btnOptimize->TabIndex = 7;
            this->btnOptimize->Text = L"Оптимизировать";
            this->btnOptimize->UseVisualStyleBackColor = false;
            this->btnOptimize->Click += gcnew System::EventHandler(this, &transportnaya::btnOptimize_Click);
            // 
            // btnBack
            // 
            this->btnBack->BackColor = System::Drawing::Color::LightCoral;
            this->btnBack->Location = System::Drawing::Point(630, 20);
            this->btnBack->Name = L"btnBack";
            this->btnBack->Size = System::Drawing::Size(100, 40);
            this->btnBack->TabIndex = 8;
            this->btnBack->Text = L"Назад";
            this->btnBack->UseVisualStyleBackColor = false;
            this->btnBack->Click += gcnew System::EventHandler(this, &transportnaya::btnBack_Click);
            // 
            // tableSupply
            // 
            this->tableSupply->AllowUserToAddRows = false;
            this->tableSupply->BackgroundColor = System::Drawing::Color::Gainsboro;
            this->tableSupply->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->tableSupply->Location = System::Drawing::Point(20, 80);
            this->tableSupply->Name = L"tableSupply";
            this->tableSupply->Size = System::Drawing::Size(250, 150);
            this->tableSupply->TabIndex = 9;
            // 
            // tableCost
            // 
            this->tableCost->AllowUserToAddRows = false;
            this->tableCost->BackgroundColor = System::Drawing::Color::Gainsboro;
            this->tableCost->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->tableCost->Location = System::Drawing::Point(280, 80);
            this->tableCost->Name = L"tableCost";
            this->tableCost->Size = System::Drawing::Size(350, 150);
            this->tableCost->TabIndex = 10;
            // 
            // tableResult
            // 
            this->tableResult->AllowUserToAddRows = false;
            this->tableResult->BackgroundColor = System::Drawing::Color::Gainsboro;
            this->tableResult->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->tableResult->Location = System::Drawing::Point(20, 240);
            this->tableResult->Name = L"tableResult";
            this->tableResult->Size = System::Drawing::Size(610, 150);
            this->tableResult->TabIndex = 11;
            // 
            // txtOutput
            // 
            this->txtOutput->Location = System::Drawing::Point(20, 400);
            this->txtOutput->Multiline = true;
            this->txtOutput->Name = L"txtOutput";
            this->txtOutput->ReadOnly = true;
            this->txtOutput->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
            this->txtOutput->Size = System::Drawing::Size(610, 80);
            this->txtOutput->TabIndex = 12;
            // 
            // transportnaya
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(750, 500);
            this->Controls->Add(this->txtOutput);
            this->Controls->Add(this->tableResult);
            this->Controls->Add(this->tableCost);
            this->Controls->Add(this->tableSupply);
            this->Controls->Add(this->btnBack);
            this->Controls->Add(this->btnOptimize);
            this->Controls->Add(this->btnMinimal);
            this->Controls->Add(this->btnNorthwest);
            this->Controls->Add(this->btnCreateTable);
            this->Controls->Add(this->txtConsumers);
            this->Controls->Add(this->txtSuppliers);
            this->Controls->Add(this->lblConsumers);
            this->Controls->Add(this->lblSuppliers);
            this->Name = L"transportnaya";
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
            this->Text = L"Транспортная задача";
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tableSupply))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tableCost))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tableResult))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion

    private:
        //----------------------------------------------------------
        // Кнопка "Создать таблицы"
        //----------------------------------------------------------
        System::Void btnCreateTable_Click(System::Object^ sender, System::EventArgs^ e)
        {
            try
            {
                int suppliers = Convert::ToInt32(txtSuppliers->Text);
                int consumers = Convert::ToInt32(txtConsumers->Text);

                CreateSupplyTable(suppliers, consumers);
                CreateCostTable(suppliers, consumers);
                CreateResultTable(suppliers, consumers);

                txtOutput->Text = "Таблицы созданы. Заполните данные:\n" +
                    "1. Запасы поставщиков и потребности потребителей\n" +
                    "2. Стоимости перевозок\n\n" +
                    "Затем используйте:\n" +
                    "- 'С-З угол' для быстрого начального плана\n" +
                    "- 'Мин. стоимость' для лучшего начального плана\n" +
                    "- 'Оптимизировать' для полной оптимизации";
            }
            catch (Exception^ ex)
            {
                txtOutput->Text = "Ошибка при создании таблиц: " + ex->Message;
            }
        }

        void CreateSupplyTable(int suppliers, int consumers)
        {
            tableSupply->Rows->Clear();
            tableSupply->Columns->Clear();

            tableSupply->Columns->Add("Supplier", "Поставщик");
            tableSupply->Columns->Add("Supply", "Запас");

            for (int i = 0; i < suppliers; i++)
            {
                tableSupply->Rows->Add();
                tableSupply->Rows[i]->Cells["Supplier"]->Value = "A" + (i + 1);
                tableSupply->Rows[i]->Cells["Supply"]->Value = "0";
            }

            tableSupply->Columns->Add("Empty", "");
            tableSupply->Columns["Empty"]->Width = 30;

            tableSupply->Columns->Add("Consumer", "Потребитель");
            tableSupply->Columns->Add("Demand", "Потребность");

            for (int i = 0; i < consumers; i++)
            {
                if (i < suppliers)
                {
                    tableSupply->Rows[i]->Cells["Consumer"]->Value = "B" + (i + 1);
                    tableSupply->Rows[i]->Cells["Demand"]->Value = "0";
                }
                else
                {
                    tableSupply->Rows->Add();
                    int rowIndex = tableSupply->Rows->Count - 1;
                    tableSupply->Rows[rowIndex]->Cells["Consumer"]->Value = "B" + (i + 1);
                    tableSupply->Rows[rowIndex]->Cells["Demand"]->Value = "0";
                }
            }
        }

        void CreateCostTable(int suppliers, int consumers)
        {
            tableCost->Rows->Clear();
            tableCost->Columns->Clear();

            for (int j = 0; j < consumers; j++)
                tableCost->Columns->Add("B" + (j + 1), "B" + (j + 1));

            for (int i = 0; i < suppliers; i++)
            {
                tableCost->Rows->Add();
                tableCost->Rows[i]->HeaderCell->Value = "A" + (i + 1);
                for (int j = 0; j < consumers; j++)
                    tableCost->Rows[i]->Cells[j]->Value = "0";
            }
        }

        void CreateResultTable(int suppliers, int consumers)
        {
            tableResult->Rows->Clear();
            tableResult->Columns->Clear();

            for (int j = 0; j < consumers; j++)
                tableResult->Columns->Add("B" + (j + 1), "B" + (j + 1));

            for (int i = 0; i < suppliers; i++)
            {
                tableResult->Rows->Add();
                tableResult->Rows[i]->HeaderCell->Value = "A" + (i + 1);
                for (int j = 0; j < consumers; j++)
                    tableResult->Rows[i]->Cells[j]->Value = "0";
            }
        }

        //----------------------------------------------------------
        // Кнопка "С-З угол"
        //----------------------------------------------------------
        System::Void btnNorthwest_Click(System::Object^ sender, System::EventArgs^ e)
        {
            try
            {
                TransportData data = ReadTransportData();

                String^ balanceInfo = "";
                if (!data.balanced) {
                    balanceInfo = "Задача не сбалансирована!\n" +
                        "Сумма запасов: " + (getTotalSupply(data)) + "\n" +
                        "Сумма потребностей: " + (getTotalDemand(data)) + "\n" +
                        "Разница: " + Math::Abs(data.balanceDiff) +
                        (data.balanceDiff > 0 ? " (добавлен фиктивный поставщик)" : " (добавлен фиктивный потребитель)") + "\n\n";
                }

                TransportPlan plan(data);
                plan.northwest();
                DisplayPlan(plan);

                String^ fictitiousInfo = "";
                if (plan.usedFictitiousSupplier) {
                    fictitiousInfo = "Фиктивный поставщик A" + (data.a.size()) + " добавлен автоматически.\n";
                }
                if (plan.usedFictitiousConsumer) {
                    fictitiousInfo = "Фиктивный потребитель B" + (data.b.size()) + " добавлен автоматически.\n";
                }

                txtOutput->Text = balanceInfo + fictitiousInfo +
                    "Метод северо-западного угла выполнен!\n" +
                    "Стоимость: " + plan.cost() + "\n" +
                    "План корректен: " + (plan.isValid() ? "Да" : "Нет") + "\n\n" +
                    "Нажмите 'Оптимизировать' для улучшения плана";
            }
            catch (Exception^ ex)
            {
                txtOutput->Text = "Ошибка при выполнении метода С-З угла: " + ex->Message;
            }
        }

        //----------------------------------------------------------
        // Кнопка "Мин. стоимость"
        //----------------------------------------------------------
        System::Void btnMinimal_Click(System::Object^ sender, System::EventArgs^ e)
        {
            try
            {
                TransportData data = ReadTransportData();

                String^ balanceInfo = "";
                if (!data.balanced) {
                    balanceInfo = "Задача не сбалансирована!\n" +
                        "Сумма запасов: " + (getTotalSupply(data)) + "\n" +
                        "Сумма потребностей: " + (getTotalDemand(data)) + "\n" +
                        "Разница: " + Math::Abs(data.balanceDiff) +
                        (data.balanceDiff > 0 ? " (добавлен фиктивный поставщик)" : " (добавлен фиктивный потребитель)") + "\n\n";
                }

                TransportPlan plan(data);
                plan.minimalCost();
                DisplayPlan(plan);

                String^ fictitiousInfo = "";
                if (plan.usedFictitiousSupplier) {
                    fictitiousInfo = "Фиктивный поставщик A" + (data.a.size()) + " добавлен автоматически.\n";
                }
                if (plan.usedFictitiousConsumer) {
                    fictitiousInfo = "Фиктивный потребитель B" + (data.b.size()) + " добавлен автоматически.\n";
                }

                txtOutput->Text = balanceInfo + fictitiousInfo +
                    "Метод минимальной стоимости выполнен!\n" +
                    "Стоимость: " + plan.cost() + "\n" +
                    "План корректен: " + (plan.isValid() ? "Да" : "Нет") + "\n\n" +
                    "Нажмите 'Оптимизировать' для улучшения плана";
            }
            catch (Exception^ ex)
            {
                txtOutput->Text = "Ошибка при выполнении метода минимальной стоимости: " + ex->Message;
            }
        }

        //----------------------------------------------------------
        // Кнопка "Оптимизировать"
        //----------------------------------------------------------
        System::Void btnOptimize_Click(System::Object^ sender, System::EventArgs^ e)
        {
            try
            {
                TransportData data = ReadTransportData();
                TransportPlan plan(data);

                // Сначала строим начальный план методом минимальной стоимости
                plan.minimalCost();
                int initialCost = plan.cost();
                bool initialValid = plan.isValid();

                String^ initialInfo = "Начальный план (мин. стоимость):\n" +
                    "Стоимость: " + initialCost + "\n" +
                    "План корректен: " + (initialValid ? "Да" : "Нет") + "\n\n";

                // Показываем начальный план
                DisplayPlan(plan);
                txtOutput->Text = initialInfo + "Начальный план показан в таблице.\nЗапускаю оптимизацию...";

                // Обновляем окно
                Application::DoEvents();
                System::Threading::Thread::Sleep(500);

                // Затем оптимизируем
                plan.fullOptimize();
                int finalCost = plan.cost();
                bool finalValid = plan.isValid();

                DisplayPlan(plan);

                String^ result = initialInfo +
                    "Оптимизация выполнена!\n" +
                    "Конечная стоимость: " + finalCost + "\n" +
                    "Экономия: " + (initialCost - finalCost) + "\n" +
                    "Конечный план корректен: " + (finalValid ? "Да" : "Нет") + "\n\n";

                // Показываем детали плана
                result += "Детали плана:\n";
                for (int i = 0; i < plan.data.a.size(); i++) {
                    int rowSum = 0;
                    for (int j = 0; j < plan.data.b.size(); j++) rowSum += plan.x[i][j];
                    String^ supplierName = "A" + (i + 1);
                    if (plan.usedFictitiousSupplier && i == plan.data.a.size() - 1) {
                        supplierName = "Aф";
                    }
                    result += "Поставщик " + supplierName + ": отгружено " + rowSum + " из " + plan.data.a[i] +
                        (rowSum == plan.data.a[i] ? " ✓" : " ✗") + "\n";
                }

                result += "\n";
                for (int j = 0; j < plan.data.b.size(); j++) {
                    int colSum = 0;
                    for (int i = 0; i < plan.data.a.size(); i++) colSum += plan.x[i][j];
                    String^ consumerName = "B" + (j + 1);
                    if (plan.usedFictitiousConsumer && j == plan.data.b.size() - 1) {
                        consumerName = "Bф";
                    }
                    result += "Потребитель " + consumerName + ": получено " + colSum + " из " + plan.data.b[j] +
                        (colSum == plan.data.b[j] ? " ✓" : " ✗") + "\n";
                }

                if (!finalValid) {
                    result += "\nВНИМАНИЕ: План требует ручной корректировки!";
                }

                txtOutput->Text = result;
            }
            catch (Exception^ ex)
            {
                txtOutput->Text = "Ошибка при оптимизации: " + ex->Message;
            }
        }

        int getTotalSupply(TransportData data) {
            int total = 0;
            for (int val : data.a) total += val;
            return total;
        }

        int getTotalDemand(TransportData data) {
            int total = 0;
            for (int val : data.b) total += val;
            return total;
        }

        TransportData ReadTransportData()
        {
            int suppliers = Convert::ToInt32(txtSuppliers->Text);
            int consumers = Convert::ToInt32(txtConsumers->Text);

            vector<int> a(suppliers);
            vector<int> b(consumers);
            vector<vector<int>> c(suppliers, vector<int>(consumers));

            for (int i = 0; i < suppliers; i++)
            {
                String^ value = tableSupply->Rows[i]->Cells["Supply"]->Value->ToString();
                a[i] = String::IsNullOrEmpty(value) ? 0 : Convert::ToInt32(value);
            }

            for (int j = 0; j < consumers; j++)
            {
                int rowIndex = (j < suppliers) ? j : suppliers;
                String^ value = tableSupply->Rows[rowIndex]->Cells["Demand"]->Value->ToString();
                b[j] = String::IsNullOrEmpty(value) ? 0 : Convert::ToInt32(value);
            }

            for (int i = 0; i < suppliers; i++)
            {
                for (int j = 0; j < consumers; j++)
                {
                    String^ value = tableCost->Rows[i]->Cells[j]->Value->ToString();
                    c[i][j] = String::IsNullOrEmpty(value) ? 0 : Convert::ToInt32(value);
                }
            }

            return TransportData(a, b, c);
        }

        void DisplayPlan(TransportPlan plan)
        {
            tableResult->Rows->Clear();
            tableResult->Columns->Clear();

            int suppliers = plan.data.a.size();
            int consumers = plan.data.b.size();

            for (int j = 0; j < consumers; j++) {
                String^ name = "B" + (j + 1);
                if (plan.usedFictitiousConsumer && j == consumers - 1) {
                    name = "Bф";
                }
                tableResult->Columns->Add(name, name);
            }

            for (int i = 0; i < suppliers; i++)
            {
                tableResult->Rows->Add();
                String^ name = "A" + (i + 1);
                if (plan.usedFictitiousSupplier && i == suppliers - 1) {
                    name = "Aф";
                }
                tableResult->Rows[i]->HeaderCell->Value = name;

                for (int j = 0; j < consumers; j++)
                {
                    tableResult->Rows[i]->Cells[j]->Value = plan.x[i][j].ToString();
                }
            }
        }

        System::Void btnBack_Click(System::Object^ sender, System::EventArgs^ e)
        {
            this->Close();
        }
    };
}