#pragma once

namespace OmniConfig {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
    private: System::Windows::Forms::TextBox^  txtboxEyeX;
    private: System::Windows::Forms::TextBox^  txtboxEyeY;
    private: System::Windows::Forms::TextBox^  txtboxEyeZ;
    private: System::Windows::Forms::Button^  btnOK;
    protected: 



    protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->txtboxEyeX = (gcnew System::Windows::Forms::TextBox());
            this->txtboxEyeY = (gcnew System::Windows::Forms::TextBox());
            this->txtboxEyeZ = (gcnew System::Windows::Forms::TextBox());
            this->btnOK = (gcnew System::Windows::Forms::Button());
            this->SuspendLayout();
            // 
            // txtboxEyeX
            // 
            this->txtboxEyeX->Location = System::Drawing::Point(12, 12);
            this->txtboxEyeX->Name = L"txtboxEyeX";
            this->txtboxEyeX->Size = System::Drawing::Size(100, 20);
            this->txtboxEyeX->TabIndex = 0;
            // 
            // txtboxEyeY
            // 
            this->txtboxEyeY->Location = System::Drawing::Point(12, 38);
            this->txtboxEyeY->Name = L"txtboxEyeY";
            this->txtboxEyeY->Size = System::Drawing::Size(100, 20);
            this->txtboxEyeY->TabIndex = 1;
            // 
            // txtboxEyeZ
            // 
            this->txtboxEyeZ->Location = System::Drawing::Point(12, 64);
            this->txtboxEyeZ->Name = L"txtboxEyeZ";
            this->txtboxEyeZ->Size = System::Drawing::Size(100, 20);
            this->txtboxEyeZ->TabIndex = 2;
            // 
            // btnOK
            // 
            this->btnOK->Location = System::Drawing::Point(37, 90);
            this->btnOK->Name = L"btnOK";
            this->btnOK->Size = System::Drawing::Size(75, 23);
            this->btnOK->TabIndex = 3;
            this->btnOK->Text = L"&OK";
            this->btnOK->UseVisualStyleBackColor = true;
            // 
            // MyForm
            // 
            this->AcceptButton = this->btnOK;
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(196, 121);
            this->Controls->Add(this->btnOK);
            this->Controls->Add(this->txtboxEyeZ);
            this->Controls->Add(this->txtboxEyeY);
            this->Controls->Add(this->txtboxEyeX);
            this->Name = L"MyForm";
            this->Text = L"OmniConfig";
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
	};
}
