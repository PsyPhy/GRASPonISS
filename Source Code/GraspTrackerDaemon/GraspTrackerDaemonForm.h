#pragma once
#include "../GraspVR/GraspTrackers.h"
#include "../DexServices/DexServices.h"

// We need InteropServics in order to convert a String to a char *.
using namespace System::Runtime::InteropServices;

// We can use one of several different Tracker classes and methods
// to interface to the tracking hardware.
typedef enum{ CONTINUOUS, POLLING, LEGACY, FAKE } TrackingType;

namespace GraspTrackerDaemon {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace PsyPhy;
	using namespace Grasp;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:

		bool			autohide;
		TrackingType	tracking;

		PsyPhy::Tracker				*coda;
		Grasp::GraspDexTrackers		*trackers;
		Grasp::DexServices			*dex;

		// IP address of DEX for telemetry
		char	*dexServicesHost;	
		// .ini file for the tracker. 
		// Includes the IP address for the Coda RTnet server.
		char	*codaInitFile;		
		// .ini file for additional parameters like pose filtering.
		// Typically, this is Grasp.ini.
		char	*applicationInitFile;

		bool recording;
		unsigned int nPoseSamples;
		PsyPhy::VectorsMixin	*vm;

		Form1( array<System::String ^> ^args ) : tracking( CONTINUOUS ), autohide( false ), recording( false ), nPoseSamples( 0 )
		{
			InitializeComponent();
			// By default, the daemon uses a CodaRTnet tracker to interface to the tracking hardware.
			// One can change for other trackers by creating different cookie files.
			if ( FileExists( "FakeCoda.flg" ) ) tracking = FAKE;
			if ( FileExists( "LegacyCoda.flg") ) tracking = LEGACY;
			if ( FileExists( "PolledCoda.flg") ) tracking = POLLING;

			// Defaults for various parameter settings.
			dexServicesHost = DEX_ETD_SERVER;
			codaInitFile = "CodaRTnet.ini";
			applicationInitFile = "Grasp.ini";

			// Create an object that provides us with vector and matrix capabilities.
			vm = new VectorsMixin();

			// 
			// GraspTrackerDaemon also acts as a proxy to transmit information
			// to the DEX hardware for transmission from the ISS to ground. 
			// It is not needed when using this system locally, but it will not
			// fail either if not connected to the DEX hardware. So I just leave
			// it in place to keep things simple.
			//
			// Connect to dex for telemetry and snapshots.
			//
			dex = new Grasp::DexServices( dexServicesHost );
			// Initialize without a log file.
			dex->Initialize();
			dex->Connect();
			dex->InitializeProxySocket();

		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
		
#pragma region Windows Form Designer generated code
	private: 
		///
		/// Designer generated objects
		System::Windows::Forms::RadioButton^  clientConnectedButton;
		System::Windows::Forms::Button^  exitButton;
		System::Windows::Forms::GroupBox^  groupBox1;
		System::Windows::Forms::Label^  label2;
		System::Windows::Forms::Label^  label1;
		System::Windows::Forms::TextBox^  visibilityTextBox1;
		System::Windows::Forms::TextBox^  visibilityTextBox0;
		System::Windows::Forms::GroupBox^  groupBox2;
		System::Windows::Forms::TextBox^  timeTextBox0;
		System::Windows::Forms::TextBox^  timeTextBox1;
		System::Windows::Forms::Button^  startButton;
		System::Windows::Forms::Button^  stopButton;
		System::Windows::Forms::Button^  saveButton;
		System::Windows::Forms::SaveFileDialog^  saveFileDialog1;
		System::Windows::Forms::GroupBox^  groupBox3;
		System::Windows::Forms::TextBox^  chestPoseTextBox;
		System::Windows::Forms::TextBox^  handPoseTextBox;
		System::Windows::Forms::TextBox^  hmdPoseTextBox;
		System::Windows::Forms::Label^  label3;
		System::Windows::Forms::Label^  label5;
		System::Windows::Forms::Label^  label4;
		System::Windows::Forms::Label^  label6;
		System::Windows::Forms::Label^  label7;
		System::Windows::Forms::GroupBox^  groupBox4;
		System::Windows::Forms::TextBox^  protocolTextBox;
		System::Windows::Forms::TextBox^  userTextBox;
		System::Windows::Forms::Label^  label8;
		System::Windows::Forms::Label^  label9;
		System::Windows::Forms::TextBox^  scriptTextBox;
		System::Windows::Forms::TextBox^  stepTextBox;
		System::Windows::Forms::TextBox^  taskTextBox;
		System::Windows::Forms::Label^  label10;
		System::Windows::Forms::Label^  label11;
		System::Windows::Forms::TextBox^  trackerStatusTextBox;
		System::Windows::Forms::Label^  label13;
		/// Required designer variable.
		System::ComponentModel::Container ^components;

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->exitButton = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->visibilityTextBox1 = (gcnew System::Windows::Forms::TextBox());
			this->visibilityTextBox0 = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->timeTextBox1 = (gcnew System::Windows::Forms::TextBox());
			this->timeTextBox0 = (gcnew System::Windows::Forms::TextBox());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->chestPoseTextBox = (gcnew System::Windows::Forms::TextBox());
			this->handPoseTextBox = (gcnew System::Windows::Forms::TextBox());
			this->hmdPoseTextBox = (gcnew System::Windows::Forms::TextBox());
			this->startButton = (gcnew System::Windows::Forms::Button());
			this->stopButton = (gcnew System::Windows::Forms::Button());
			this->saveButton = (gcnew System::Windows::Forms::Button());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->trackerStatusTextBox = (gcnew System::Windows::Forms::TextBox());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->clientConnectedButton = (gcnew System::Windows::Forms::RadioButton());
			this->scriptTextBox = (gcnew System::Windows::Forms::TextBox());
			this->stepTextBox = (gcnew System::Windows::Forms::TextBox());
			this->taskTextBox = (gcnew System::Windows::Forms::TextBox());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->protocolTextBox = (gcnew System::Windows::Forms::TextBox());
			this->userTextBox = (gcnew System::Windows::Forms::TextBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->groupBox4->SuspendLayout();
			this->SuspendLayout();
			// 
			// exitButton
			// 
			this->exitButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->exitButton->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->exitButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->exitButton->Location = System::Drawing::Point(450, 206);
			this->exitButton->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->exitButton->Name = L"exitButton";
			this->exitButton->Size = System::Drawing::Size(100, 37);
			this->exitButton->TabIndex = 1;
			this->exitButton->Text = L"Exit";
			this->exitButton->UseVisualStyleBackColor = true;
			this->exitButton->Click += gcnew System::EventHandler(this, &Form1::exitButton_Click);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->label6);
			this->groupBox1->Controls->Add(this->label7);
			this->groupBox1->Controls->Add(this->visibilityTextBox1);
			this->groupBox1->Controls->Add(this->visibilityTextBox0);
			this->groupBox1->Location = System::Drawing::Point(185, 10);
			this->groupBox1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Padding = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox1->Size = System::Drawing::Size(401, 76);
			this->groupBox1->TabIndex = 4;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Marker Visibility";
			// 
			// label6
			// 
			this->label6->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label6->Location = System::Drawing::Point(3, 18);
			this->label6->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(122, 17);
			this->label6->TabIndex = 12;
			this->label6->Text = L"Tracker Camera 1";
			// 
			// label7
			// 
			this->label7->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label7->AutoSize = true;
			this->label7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label7->Location = System::Drawing::Point(3, 47);
			this->label7->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(122, 17);
			this->label7->TabIndex = 11;
			this->label7->Text = L"Tracker Camera 2";
			// 
			// visibilityTextBox1
			// 
			this->visibilityTextBox1->Font = (gcnew System::Drawing::Font(L"Courier New", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->visibilityTextBox1->Location = System::Drawing::Point(131, 39);
			this->visibilityTextBox1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->visibilityTextBox1->Name = L"visibilityTextBox1";
			this->visibilityTextBox1->Size = System::Drawing::Size(266, 22);
			this->visibilityTextBox1->TabIndex = 4;
			this->visibilityTextBox1->Text = L" 00000000  00000000  00000000 ";
			// 
			// visibilityTextBox0
			// 
			this->visibilityTextBox0->Font = (gcnew System::Drawing::Font(L"Courier New", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->visibilityTextBox0->Location = System::Drawing::Point(131, 13);
			this->visibilityTextBox0->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->visibilityTextBox0->Name = L"visibilityTextBox0";
			this->visibilityTextBox0->Size = System::Drawing::Size(266, 22);
			this->visibilityTextBox0->TabIndex = 3;
			this->visibilityTextBox0->Text = L" 00000000  00000000  00000000 ";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(28, 49);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(58, 17);
			this->label2->TabIndex = 6;
			this->label2->Text = L"Counter";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(4, 18);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(83, 17);
			this->label1->TabIndex = 5;
			this->label1->Text = L"Time Stamp";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->timeTextBox1);
			this->groupBox2->Controls->Add(this->timeTextBox0);
			this->groupBox2->Controls->Add(this->label2);
			this->groupBox2->Controls->Add(this->label1);
			this->groupBox2->Location = System::Drawing::Point(12, 10);
			this->groupBox2->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Padding = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox2->Size = System::Drawing::Size(169, 76);
			this->groupBox2->TabIndex = 5;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Frame";
			// 
			// timeTextBox1
			// 
			this->timeTextBox1->Font = (gcnew System::Drawing::Font(L"Arial", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->timeTextBox1->Location = System::Drawing::Point(96, 43);
			this->timeTextBox1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->timeTextBox1->Name = L"timeTextBox1";
			this->timeTextBox1->Size = System::Drawing::Size(69, 26);
			this->timeTextBox1->TabIndex = 7;
			this->timeTextBox1->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// timeTextBox0
			// 
			this->timeTextBox0->Font = (gcnew System::Drawing::Font(L"Arial", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->timeTextBox0->Location = System::Drawing::Point(96, 14);
			this->timeTextBox0->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->timeTextBox0->Name = L"timeTextBox0";
			this->timeTextBox0->Size = System::Drawing::Size(69, 26);
			this->timeTextBox0->TabIndex = 1;
			this->timeTextBox0->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->label5);
			this->groupBox3->Controls->Add(this->label4);
			this->groupBox3->Controls->Add(this->label3);
			this->groupBox3->Controls->Add(this->chestPoseTextBox);
			this->groupBox3->Controls->Add(this->handPoseTextBox);
			this->groupBox3->Controls->Add(this->hmdPoseTextBox);
			this->groupBox3->Location = System::Drawing::Point(12, 91);
			this->groupBox3->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Padding = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox3->Size = System::Drawing::Size(574, 109);
			this->groupBox3->TabIndex = 6;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Pose Trackers";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label5->Location = System::Drawing::Point(10, 80);
			this->label5->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(44, 17);
			this->label5->TabIndex = 10;
			this->label5->Text = L"Chest";
			this->label5->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label4->Location = System::Drawing::Point(14, 50);
			this->label4->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(42, 17);
			this->label4->TabIndex = 9;
			this->label4->Text = L"Hand";
			this->label4->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label3->Location = System::Drawing::Point(14, 20);
			this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(39, 17);
			this->label3->TabIndex = 8;
			this->label3->Text = L"HMD";
			this->label3->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// chestPoseTextBox
			// 
			this->chestPoseTextBox->Font = (gcnew System::Drawing::Font(L"Courier New", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->chestPoseTextBox->Location = System::Drawing::Point(56, 77);
			this->chestPoseTextBox->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->chestPoseTextBox->Name = L"chestPoseTextBox";
			this->chestPoseTextBox->Size = System::Drawing::Size(514, 22);
			this->chestPoseTextBox->TabIndex = 7;
			this->chestPoseTextBox->Text = L" 00000000  00000000  00000000 ";
			// 
			// handPoseTextBox
			// 
			this->handPoseTextBox->Font = (gcnew System::Drawing::Font(L"Courier New", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->handPoseTextBox->Location = System::Drawing::Point(56, 47);
			this->handPoseTextBox->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->handPoseTextBox->Name = L"handPoseTextBox";
			this->handPoseTextBox->Size = System::Drawing::Size(514, 22);
			this->handPoseTextBox->TabIndex = 6;
			this->handPoseTextBox->Text = L" 00000000  00000000  00000000 ";
			// 
			// hmdPoseTextBox
			// 
			this->hmdPoseTextBox->Font = (gcnew System::Drawing::Font(L"Courier New", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->hmdPoseTextBox->Location = System::Drawing::Point(56, 17);
			this->hmdPoseTextBox->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->hmdPoseTextBox->Name = L"hmdPoseTextBox";
			this->hmdPoseTextBox->Size = System::Drawing::Size(514, 22);
			this->hmdPoseTextBox->TabIndex = 5;
			this->hmdPoseTextBox->Text = L" 00000000  00000000  00000000 ";
			// 
			// startButton
			// 
			this->startButton->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->startButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->startButton->Location = System::Drawing::Point(33, 206);
			this->startButton->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->startButton->Name = L"startButton";
			this->startButton->Size = System::Drawing::Size(100, 37);
			this->startButton->TabIndex = 7;
			this->startButton->Text = L"Record";
			this->startButton->UseVisualStyleBackColor = true;
			this->startButton->Click += gcnew System::EventHandler(this, &Form1::startButton_Click);
			// 
			// stopButton
			// 
			this->stopButton->Enabled = false;
			this->stopButton->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->stopButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->stopButton->Location = System::Drawing::Point(138, 206);
			this->stopButton->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->stopButton->Name = L"stopButton";
			this->stopButton->Size = System::Drawing::Size(100, 37);
			this->stopButton->TabIndex = 8;
			this->stopButton->Text = L"Stop";
			this->stopButton->UseVisualStyleBackColor = true;
			this->stopButton->Click += gcnew System::EventHandler(this, &Form1::stopButton_Click);
			// 
			// saveButton
			// 
			this->saveButton->Enabled = false;
			this->saveButton->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->saveButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->saveButton->Location = System::Drawing::Point(242, 206);
			this->saveButton->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->saveButton->Name = L"saveButton";
			this->saveButton->Size = System::Drawing::Size(100, 37);
			this->saveButton->TabIndex = 9;
			this->saveButton->Text = L"Save";
			this->saveButton->UseVisualStyleBackColor = true;
			this->saveButton->Click += gcnew System::EventHandler(this, &Form1::saveButton_Click);
			// 
			// saveFileDialog1
			// 
			this->saveFileDialog1->DefaultExt = L"mrk";
			this->saveFileDialog1->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &Form1::saveFileDialog1_FileOk);
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->trackerStatusTextBox);
			this->groupBox4->Controls->Add(this->label13);
			this->groupBox4->Controls->Add(this->clientConnectedButton);
			this->groupBox4->Controls->Add(this->scriptTextBox);
			this->groupBox4->Controls->Add(this->stepTextBox);
			this->groupBox4->Controls->Add(this->taskTextBox);
			this->groupBox4->Controls->Add(this->label10);
			this->groupBox4->Controls->Add(this->label11);
			this->groupBox4->Controls->Add(this->protocolTextBox);
			this->groupBox4->Controls->Add(this->userTextBox);
			this->groupBox4->Controls->Add(this->label8);
			this->groupBox4->Controls->Add(this->label9);
			this->groupBox4->Location = System::Drawing::Point(12, 249);
			this->groupBox4->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Padding = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox4->Size = System::Drawing::Size(562, 54);
			this->groupBox4->TabIndex = 10;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"Housekeeping  ";
			// 
			// trackerStatusTextBox
			// 
			this->trackerStatusTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->trackerStatusTextBox->Location = System::Drawing::Point(512, 21);
			this->trackerStatusTextBox->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->trackerStatusTextBox->Name = L"trackerStatusTextBox";
			this->trackerStatusTextBox->Size = System::Drawing::Size(40, 26);
			this->trackerStatusTextBox->TabIndex = 16;
			this->trackerStatusTextBox->Text = L"000";
			this->trackerStatusTextBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label13->Location = System::Drawing::Point(455, 25);
			this->label13->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(57, 17);
			this->label13->TabIndex = 15;
			this->label13->Text = L"Tracker";
			this->label13->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// clientConnectedButton
			// 
			this->clientConnectedButton->AutoSize = true;
			this->clientConnectedButton->Location = System::Drawing::Point(431, -1);
			this->clientConnectedButton->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->clientConnectedButton->Name = L"clientConnectedButton";
			this->clientConnectedButton->Size = System::Drawing::Size(106, 17);
			this->clientConnectedButton->TabIndex = 14;
			this->clientConnectedButton->TabStop = true;
			this->clientConnectedButton->Text = L"Client Connected";
			this->clientConnectedButton->UseVisualStyleBackColor = true;
			// 
			// scriptTextBox
			// 
			this->scriptTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->scriptTextBox->Location = System::Drawing::Point(384, 21);
			this->scriptTextBox->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->scriptTextBox->Name = L"scriptTextBox";
			this->scriptTextBox->Size = System::Drawing::Size(56, 26);
			this->scriptTextBox->TabIndex = 13;
			this->scriptTextBox->Text = L"00000";
			this->scriptTextBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// stepTextBox
			// 
			this->stepTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->stepTextBox->Location = System::Drawing::Point(337, 21);
			this->stepTextBox->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->stepTextBox->Name = L"stepTextBox";
			this->stepTextBox->Size = System::Drawing::Size(42, 26);
			this->stepTextBox->TabIndex = 11;
			this->stepTextBox->Text = L"000";
			this->stepTextBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// taskTextBox
			// 
			this->taskTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->taskTextBox->Location = System::Drawing::Point(248, 21);
			this->taskTextBox->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->taskTextBox->Name = L"taskTextBox";
			this->taskTextBox->Size = System::Drawing::Size(42, 26);
			this->taskTextBox->TabIndex = 8;
			this->taskTextBox->Text = L"000";
			this->taskTextBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label10->Location = System::Drawing::Point(299, 25);
			this->label10->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(37, 17);
			this->label10->TabIndex = 10;
			this->label10->Text = L"Step";
			this->label10->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label11->Location = System::Drawing::Point(208, 25);
			this->label11->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(39, 17);
			this->label11->TabIndex = 9;
			this->label11->Text = L"Task";
			this->label11->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// protocolTextBox
			// 
			this->protocolTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->protocolTextBox->Location = System::Drawing::Point(158, 21);
			this->protocolTextBox->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->protocolTextBox->Name = L"protocolTextBox";
			this->protocolTextBox->Size = System::Drawing::Size(42, 26);
			this->protocolTextBox->TabIndex = 7;
			this->protocolTextBox->Text = L"000";
			this->protocolTextBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// userTextBox
			// 
			this->userTextBox->Font = (gcnew System::Drawing::Font(L"Arial", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->userTextBox->Location = System::Drawing::Point(44, 21);
			this->userTextBox->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->userTextBox->Name = L"userTextBox";
			this->userTextBox->Size = System::Drawing::Size(42, 26);
			this->userTextBox->TabIndex = 1;
			this->userTextBox->Text = L"000";
			this->userTextBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label8->Location = System::Drawing::Point(99, 25);
			this->label8->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(60, 17);
			this->label8->TabIndex = 6;
			this->label8->Text = L"Protocol";
			this->label8->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label9->Location = System::Drawing::Point(4, 25);
			this->label9->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(38, 17);
			this->label9->TabIndex = 5;
			this->label9->Text = L"User";
			this->label9->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// Form1
			// 
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::None;
			this->CancelButton = this->exitButton;
			this->ClientSize = System::Drawing::Size(597, 312);
			this->Controls->Add(this->groupBox4);
			this->Controls->Add(this->saveButton);
			this->Controls->Add(this->stopButton);
			this->Controls->Add(this->startButton);
			this->Controls->Add(this->groupBox3);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->exitButton);
			this->KeyPreview = true;
			this->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"GraspTrackerDaemon";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Form1::Form1_FormClosing);
			this->Shown += gcnew System::EventHandler(this, &Form1::Form1_Shown);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->groupBox4->ResumeLayout(false);
			this->groupBox4->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: 

		System::Void Form1_Shown(System::Object^  sender, System::EventArgs^  e) {

			// Disable the form to indicate that the buttons will not work
			// until we are up and running.
			Enabled = false;
			Refresh();

			visibilityTextBox0->Text = " Initializing TRACKER ... ";
			visibilityTextBox1->Text = "   (Please wait.)";

			// This thread will block until the coda initialization completes,
			// so we explicitly update the form since it will not get a periodic
			// refresh while we are waiting.
			Refresh();
			Application::DoEvents();

			InitializeSockets();
			InitializeTrackers();
			Refresh();
			Application::DoEvents();

			visibilityTextBox1->Text = "";
			visibilityTextBox0->Text = " Initializing TRACKER ... OK.";

			CreateRefreshTimer( 2 );
			StartRefreshTimer();

			Refresh();
			Application::DoEvents();

			// Renable the form now that we are ready to respond to the buttons.
			Enabled = true;

			// For flight operations we want this window to hide itself automatically.
			// But it is a command line option.
			if ( autohide ) WindowState = System::Windows::Forms::FormWindowState::Minimized;
			 
		}

		System::Void Form1_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {

			StopRefreshTimer();

			// Disable the form to indicate that the buttons will not work.
			Enabled = false;
			Refresh();

			// Let DEX know that we are stopping and so there is no 
			// valid information about the task, etc. to be had.
			dex->ResetTaskInfo();
			dex->Disconnect();
			dex->Release();

			visibilityTextBox0->Text = " Shutting down TRACKER ... ";
			visibilityTextBox1->Text = "   (Please wait.)";
			Refresh();

			ReleaseCoda();
		}

		// A timer to handle animations and screen refresh, and associated actions.
		static System::Windows::Forms::Timer^ refreshTimer;
		void OnTimerElapsed( System::Object^ source, System::EventArgs^ e ) {
			// Stop the timer so that we do not get overrun.
			StopRefreshTimer();
			if ( dex->HandleProxyConnection() ) {

				// The following is a hack to deal with the CODA RTnet server memory overrun.
				// Detect when we get a new client connection and restart the continuous CODA
				// acquisition at that moment. It is likely to be a moment where the
				// 1 second pause in data will not be too critical.
				if ( ! clientConnectedButton->Checked ) {
					
					coda->RestartContinuousAcquisition();

					 // Send the current tracker alignment in an RT science packet.
					 AlignClientBuffer clientBuffer;
					 Vector3	offsets[MAX_UNITS];
					 Matrix3x3	rotations[MAX_UNITS];
					 strncpy( clientBuffer.ID, "ALIGN", sizeof( clientBuffer.ID ) );
					 clientBuffer.prePost = CURRENT;
					 fOutputDebugString( "Retrieving tracker alignments.\n" );
					 coda->GetAlignmentTransforms( offsets, rotations );
					 for ( int unit = 0; unit < MAX_UNITS; unit++ ) {
						 coda->CopyVector( clientBuffer.offsets[unit], offsets[unit] );
						 coda->CopyMatrix( clientBuffer.rotations[unit], rotations[unit] );
					 }
					 fOutputDebugString( "Sending tracker alignments to ground.\n" );
					 dex->AddClientSlice( (unsigned char *) &clientBuffer, sizeof( clientBuffer ), true );
					 fOutputDebugString( "Done sending tracker alignments.\n" );

				}

				userTextBox->Text = dex->static_user.ToString();
				protocolTextBox->Text = dex->static_protocol.ToString();
				taskTextBox->Text = dex->static_task.ToString();
				stepTextBox->Text = dex->static_step.ToString();
				scriptTextBox->Text = dex->static_substep.ToString();
				clientConnectedButton->Checked = true;

			}
			else clientConnectedButton->Checked = false;
			// Actually process the tracker data.
			ProcessCodaInputs();
			// Restart the timer for the next cycle.
			StartRefreshTimer();
		}

		// A timer to trigger periodic updates.
		void CreateRefreshTimer( int interval ) {
			refreshTimer = gcnew( System::Windows::Forms::Timer );
			refreshTimer->Interval = interval;
			refreshTimer->Tick += gcnew EventHandler( this, &Form1::OnTimerElapsed );
		}
		void StartRefreshTimer( void ) {
			refreshTimer->Start();
		}
		void StopRefreshTimer( void ) {
			refreshTimer->Stop();
		}		


	private: System::Void exitButton_Click(System::Object^  sender, System::EventArgs^  e) {
			this->Enabled = false;
			Close();
		}
	private: System::Void startButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 startButton->Enabled = false;
				 stopButton->Enabled = true;
				 saveButton->Enabled = false;
				 // Start an acquisition that will last up to 1000 seconds.
				 // Typically one will make a shorter acquisition by pressing Stop.
				 trackers->codaTracker->StartAcquisition( 1000.0 );
				 nPoseSamples = 0;
				 recording = true;
			 }
	private: System::Void stopButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 startButton->Enabled = true;
				 stopButton->Enabled = false;
				 saveButton->Enabled = true;
				 trackers->codaTracker->StopAcquisition();
				 recording = false;
			 }
	private: System::Void saveButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 recording = false;
				 saveFileDialog1->ShowDialog();
			 }

	private: System::Void saveFileDialog1_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e);
	
	private:
		
		void InitializeTrackers( void );
		void ProcessCodaInputs( void );
		void ReleaseCoda( void );
		void InitializeBroadcastSocket( void );
		void InitializeSockets( void );
#ifdef RECEIVE_COMMANDS
		void InitializeReceiverSocket( void );
#endif

};
}

