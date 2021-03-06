#pragma once

#include "../Useful/Useful.h"
#include "../Useful/fMessageBox.h"
#include "../Useful/fOutputDebugString.h"
#include "../VectorsMixin/VectorsMixin.h"
#include "../Useful/OpenGLUseful.h"

#include "../OpenGLObjects/OpenGLColors.h"
#include "../OpenGLObjects/OpenGLWindows.h"
#include "../OpenGLObjects/OpenGLObjects.h"
#include "../OpenGLObjects/OpenGLViewpoints.h"
#include "../OpenGLObjects/OpenGLTextures.h"
#include "../OpenGLObjects/OpenGLWindowsInForms.h"

#include "../Trackers/CodaRTnetTracker.h"
#include "../Trackers/CodaRTnetDaemonTracker.h"
#include "../Trackers/CodaPoseTracker.h"
#include "../GraspVR/GraspGLObjects.h"

#include "../DexServices/DexServices.h"

#define BODIES 3
#define HMD 0
#define HAND 1
#define CHEST 2

#define CODAS  2

namespace GraspHardwareStatus {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace PsyPhy;
	using namespace Grasp;

	/// <summary>
	/// The class that defines the main form for displaying the hardware status.
	/// It contains multiple window panes that are used to display OpenGLObject.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{

	private:

		OpenGLWindow *hmdWindow0;
		OpenGLWindow *hmdWindow1;
		OpenGLWindow *handWindow0;
		OpenGLWindow *handWindow1;
		OpenGLWindow *chestWindow0;
		OpenGLWindow *chestWindow1;

		OpenGLWindow *vrWindow0;
		OpenGLWindow *vrWindow1;

		Viewpoint *codaViewpoint0;
		Viewpoint *codaViewpoint1;
		Viewpoint *objectViewpoint;

		// A class that provides methods for making a lot of the 
		// OpenGLObjects that we need for displaying the status.
		GraspGLObjects *objects;
		// A visual representation of each marker structure.
		// These ones are always at the origin in the null orientation.
		MarkerStructureGLObject *hmdStationary0;
		MarkerStructureGLObject *handStationary0;
		MarkerStructureGLObject *chestStationary0;
		MarkerStructureGLObject *hmdStationary1;
		MarkerStructureGLObject *handStationary1;
		MarkerStructureGLObject *chestStationary1;
		// Another set of objects that will move around according to the tracker.
		MarkerStructureGLObject *hmdMobile;
		MarkerStructureGLObject *handMobile;
		MarkerStructureGLObject *chestMobile;

			 // Just a way to refer to all the mobile objects together.
		// It makes it easier to draw all of them. 
		Yoke *mobiles;

	public:

		CodaRTnetDaemonTracker *coda;
		CodaPoseTracker *hmdTracker;
		CodaPoseTracker *handTracker;
		CodaPoseTracker *chestTracker;
		MarkerFrame *markerFrame;

		String^	filenameRoot;

		Form1( void )
		{
			InitializeComponent();
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

#pragma region Windows Form Designer generated objects
		/// <summary>
		/// These Forms objects are generated in the Designer GUI.
		///
	private:
		System::Windows::Forms::GroupBox^  chestGroupBox;
		System::Windows::Forms::Panel^  chestPanel1;
		System::Windows::Forms::Panel^  chestPanel0;
		System::Windows::Forms::GroupBox^  handGroupBox;
		System::Windows::Forms::Panel^  handPanel1;
		System::Windows::Forms::Panel^  handPanel0;
		System::Windows::Forms::GroupBox^  hmdGroupBox;
		System::Windows::Forms::Panel^  hmdPanel1;
		System::Windows::Forms::Panel^  hmdPanel0;
		System::Windows::Forms::Label^  label3;
		System::Windows::Forms::Label^  label2;
		System::Windows::Forms::Label^  label1;
		System::Windows::Forms::Label^  label4;
		System::Windows::Forms::Label^  label5;
		System::Windows::Forms::Label^  label6;
		System::Windows::Forms::Label^  label7;
		System::Windows::Forms::GroupBox^  groupBox2;
		System::Windows::Forms::Label^  label8;
		System::Windows::Forms::Label^  label9;
		System::Windows::Forms::Panel^  vrPanel1;
		System::Windows::Forms::Panel^  vrPanel0;
		System::Windows::Forms::GroupBox^  groupBox1;
		System::Windows::Forms::Panel^  oculusPanel;
		System::Windows::Forms::Button^  closeButton;

	private:
		/// Required designer variable.
		System::ComponentModel::Container ^components;
#pragma endregion

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->chestGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->chestPanel1 = (gcnew System::Windows::Forms::Panel());
			this->chestPanel0 = (gcnew System::Windows::Forms::Panel());
			this->handGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->handPanel1 = (gcnew System::Windows::Forms::Panel());
			this->handPanel0 = (gcnew System::Windows::Forms::Panel());
			this->hmdGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->hmdPanel1 = (gcnew System::Windows::Forms::Panel());
			this->hmdPanel0 = (gcnew System::Windows::Forms::Panel());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->oculusPanel = (gcnew System::Windows::Forms::Panel());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->vrPanel1 = (gcnew System::Windows::Forms::Panel());
			this->vrPanel0 = (gcnew System::Windows::Forms::Panel());
			this->closeButton = (gcnew System::Windows::Forms::Button());
			this->chestGroupBox->SuspendLayout();
			this->handGroupBox->SuspendLayout();
			this->hmdGroupBox->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->oculusPanel->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->SuspendLayout();
			// 
			// chestGroupBox
			// 
			this->chestGroupBox->Controls->Add(this->label2);
			this->chestGroupBox->Controls->Add(this->label1);
			this->chestGroupBox->Controls->Add(this->chestPanel1);
			this->chestGroupBox->Controls->Add(this->chestPanel0);
			this->chestGroupBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->chestGroupBox->Location = System::Drawing::Point(9, 491);
			this->chestGroupBox->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->chestGroupBox->Name = L"chestGroupBox";
			this->chestGroupBox->Padding = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->chestGroupBox->Size = System::Drawing::Size(418, 228);
			this->chestGroupBox->TabIndex = 0;
			this->chestGroupBox->TabStop = false;
			this->chestGroupBox->Text = L"Chest Marker Visibility";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(244, 202);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(168, 17);
			this->label2->TabIndex = 6;
			this->label2->Text = L"Tracker Camera Bar 2";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(35, 202);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(168, 17);
			this->label1->TabIndex = 5;
			this->label1->Text = L"Tracker Camera Bar 1";
			// 
			// chestPanel1
			// 
			this->chestPanel1->Location = System::Drawing::Point(218, 27);
			this->chestPanel1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->chestPanel1->Name = L"chestPanel1";
			this->chestPanel1->Size = System::Drawing::Size(192, 167);
			this->chestPanel1->TabIndex = 2;
			// 
			// chestPanel0
			// 
			this->chestPanel0->Location = System::Drawing::Point(9, 27);
			this->chestPanel0->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->chestPanel0->Name = L"chestPanel0";
			this->chestPanel0->Size = System::Drawing::Size(192, 167);
			this->chestPanel0->TabIndex = 1;
			// 
			// handGroupBox
			// 
			this->handGroupBox->Controls->Add(this->label4);
			this->handGroupBox->Controls->Add(this->label5);
			this->handGroupBox->Controls->Add(this->handPanel1);
			this->handGroupBox->Controls->Add(this->handPanel0);
			this->handGroupBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->handGroupBox->Location = System::Drawing::Point(9, 250);
			this->handGroupBox->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->handGroupBox->Name = L"handGroupBox";
			this->handGroupBox->Padding = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->handGroupBox->Size = System::Drawing::Size(418, 228);
			this->handGroupBox->TabIndex = 1;
			this->handGroupBox->TabStop = false;
			this->handGroupBox->Text = L"Hand Marker Visibility";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label4->Location = System::Drawing::Point(244, 202);
			this->label4->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(168, 17);
			this->label4->TabIndex = 8;
			this->label4->Text = L"Tracker Camera Bar 2";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label5->Location = System::Drawing::Point(35, 202);
			this->label5->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(168, 17);
			this->label5->TabIndex = 7;
			this->label5->Text = L"Tracker Camera Bar 1";
			// 
			// handPanel1
			// 
			this->handPanel1->Location = System::Drawing::Point(218, 27);
			this->handPanel1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->handPanel1->Name = L"handPanel1";
			this->handPanel1->Size = System::Drawing::Size(192, 167);
			this->handPanel1->TabIndex = 2;
			// 
			// handPanel0
			// 
			this->handPanel0->Location = System::Drawing::Point(9, 27);
			this->handPanel0->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->handPanel0->Name = L"handPanel0";
			this->handPanel0->Size = System::Drawing::Size(192, 167);
			this->handPanel0->TabIndex = 1;
			// 
			// hmdGroupBox
			// 
			this->hmdGroupBox->Controls->Add(this->label6);
			this->hmdGroupBox->Controls->Add(this->label7);
			this->hmdGroupBox->Controls->Add(this->hmdPanel1);
			this->hmdGroupBox->Controls->Add(this->hmdPanel0);
			this->hmdGroupBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->hmdGroupBox->Location = System::Drawing::Point(9, 10);
			this->hmdGroupBox->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->hmdGroupBox->Name = L"hmdGroupBox";
			this->hmdGroupBox->Padding = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->hmdGroupBox->Size = System::Drawing::Size(418, 228);
			this->hmdGroupBox->TabIndex = 2;
			this->hmdGroupBox->TabStop = false;
			this->hmdGroupBox->Text = L"HMD Marker Visibility";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label6->Location = System::Drawing::Point(244, 202);
			this->label6->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(168, 17);
			this->label6->TabIndex = 10;
			this->label6->Text = L"Tracker Camera Bar 2";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label7->Location = System::Drawing::Point(35, 202);
			this->label7->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(168, 17);
			this->label7->TabIndex = 9;
			this->label7->Text = L"Tracker Camera Bar 1";
			// 
			// hmdPanel1
			// 
			this->hmdPanel1->Location = System::Drawing::Point(218, 27);
			this->hmdPanel1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->hmdPanel1->Name = L"hmdPanel1";
			this->hmdPanel1->Size = System::Drawing::Size(192, 167);
			this->hmdPanel1->TabIndex = 2;
			// 
			// hmdPanel0
			// 
			this->hmdPanel0->Location = System::Drawing::Point(9, 27);
			this->hmdPanel0->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->hmdPanel0->Name = L"hmdPanel0";
			this->hmdPanel0->Size = System::Drawing::Size(192, 167);
			this->hmdPanel0->TabIndex = 1;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->oculusPanel);
			this->groupBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->groupBox1->Location = System::Drawing::Point(444, 10);
			this->groupBox1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Padding = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox1->Size = System::Drawing::Size(639, 344);
			this->groupBox1->TabIndex = 6;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"VR Headset";
			// 
			// oculusPanel
			// 
			this->oculusPanel->Controls->Add(this->label3);
			this->oculusPanel->Location = System::Drawing::Point(13, 25);
			this->oculusPanel->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->oculusPanel->Name = L"oculusPanel";
			this->oculusPanel->Size = System::Drawing::Size(622, 305);
			this->oculusPanel->TabIndex = 0;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label3->Location = System::Drawing::Point(102, 111);
			this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label3->Name = L"label3";
			this->label3->Padding = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->label3->Size = System::Drawing::Size(463, 58);
			this->label3->TabIndex = 0;
			this->label3->Text = L"If no VR images appear, check cables to VR Headset\r\n           then double click " 
				L"on this message to retry.";
			this->label3->DoubleClick += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->label8);
			this->groupBox2->Controls->Add(this->label9);
			this->groupBox2->Controls->Add(this->vrPanel1);
			this->groupBox2->Controls->Add(this->vrPanel0);
			this->groupBox2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->groupBox2->Location = System::Drawing::Point(444, 358);
			this->groupBox2->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Padding = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->groupBox2->Size = System::Drawing::Size(639, 360);
			this->groupBox2->TabIndex = 7;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"3D Tracking";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label8->Location = System::Drawing::Point(369, 341);
			this->label8->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(243, 17);
			this->label8->TabIndex = 8;
			this->label8->Text = L"View from Tracker Camera Bar 2";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label9->Location = System::Drawing::Point(58, 341);
			this->label9->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(243, 17);
			this->label9->TabIndex = 7;
			this->label9->Text = L"View from Tracker Camera Bar 1";
			// 
			// vrPanel1
			// 
			this->vrPanel1->Location = System::Drawing::Point(323, 27);
			this->vrPanel1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->vrPanel1->Name = L"vrPanel1";
			this->vrPanel1->Size = System::Drawing::Size(306, 305);
			this->vrPanel1->TabIndex = 1;
			// 
			// vrPanel0
			// 
			this->vrPanel0->Location = System::Drawing::Point(13, 27);
			this->vrPanel0->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->vrPanel0->Name = L"vrPanel0";
			this->vrPanel0->Size = System::Drawing::Size(306, 305);
			this->vrPanel0->TabIndex = 0;
			// 
			// closeButton
			// 
			this->closeButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->closeButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->closeButton->Location = System::Drawing::Point(998, 730);
			this->closeButton->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->closeButton->Name = L"closeButton";
			this->closeButton->Size = System::Drawing::Size(86, 34);
			this->closeButton->TabIndex = 8;
			this->closeButton->Text = L"Close";
			this->closeButton->UseVisualStyleBackColor = true;
			this->closeButton->Click += gcnew System::EventHandler(this, &Form1::button1_Click_1);
			// 
			// Form1
			// 
			this->AcceptButton = this->closeButton;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->CancelButton = this->closeButton;
			this->ClientSize = System::Drawing::Size(1090, 774);
			this->Controls->Add(this->closeButton);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->hmdGroupBox);
			this->Controls->Add(this->handGroupBox);
			this->Controls->Add(this->chestGroupBox);
			this->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"GRASP Hardware Status";
			this->Shown += gcnew System::EventHandler(this, &Form1::Form1_Shown);
			this->chestGroupBox->ResumeLayout(false);
			this->chestGroupBox->PerformLayout();
			this->handGroupBox->ResumeLayout(false);
			this->handGroupBox->PerformLayout();
			this->hmdGroupBox->ResumeLayout(false);
			this->hmdGroupBox->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->oculusPanel->ResumeLayout(false);
			this->oculusPanel->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

	private: 

		// Draw the 3D graphics.
		void RenderWindow( OpenGLWindow *window, Viewpoint *viewpoint, OpenGLObject *object ) {
			window->Activate();
			window->Clear( 0.10, 0.10, 0.30 );
			viewpoint->Apply( window, CYCLOPS );
			object->Draw();
			window->Swap();
		}

		void Render( void ) {
		
			MarkerFrame unitMarkerFrame[2];	
			MarkerFrame markerFrame;
			TrackerPose pose;

			hmdTracker->SetMarkerFrameBuffer( &markerFrame );
			handTracker->SetMarkerFrameBuffer( &markerFrame );
			chestTracker->SetMarkerFrameBuffer( &markerFrame );

			// Show the visibility of each marker superimposed on it's marker 
			// structure, each structure in a separate window for each CODA unit.
			coda->GetCurrentMarkerFrameUnit( unitMarkerFrame[0], 0 );
			coda->GetCurrentMarkerFrameUnit( unitMarkerFrame[1], 1 );
			coda->ComputeAverageMarkerFrame( markerFrame, unitMarkerFrame, 2 );

			hmdTracker->GetCurrentPose( pose );
			hmdMobile->SetPose( pose.pose );
			handTracker->GetCurrentPose( pose );
			handMobile->SetPose( pose.pose );
			chestTracker->GetCurrentPose( pose );
			chestMobile->SetPose( pose.pose );

			hmdStationary0->ShowVisibility( unitMarkerFrame[0], CYAN );
			handStationary0->ShowVisibility( unitMarkerFrame[0], MAGENTA );
			chestStationary0->ShowVisibility( unitMarkerFrame[0], YELLOW );

			RenderWindow( hmdWindow0, objectViewpoint, hmdStationary0 );
			RenderWindow( handWindow0, objectViewpoint, handStationary0 );
			RenderWindow( chestWindow0, objectViewpoint, chestStationary0 );

			// Show the position and orientation of each marker structure
			// from the perspective of each CODA unit.
			hmdMobile->ShowVisibility( unitMarkerFrame[0], CYAN );
			handMobile->ShowVisibility( unitMarkerFrame[0], MAGENTA );
			chestMobile->ShowVisibility( unitMarkerFrame[0], YELLOW );
			RenderWindow( vrWindow0, codaViewpoint0, mobiles );

			// Show the visibility of each marker superimposed on it's marker 
			// structure, each structure in a separate window for each CODA unit.
			hmdStationary1->ShowVisibility( unitMarkerFrame[1], CYAN );
			handStationary1->ShowVisibility( unitMarkerFrame[1], MAGENTA );
			chestStationary1->ShowVisibility( unitMarkerFrame[1], YELLOW );

			RenderWindow( hmdWindow1, objectViewpoint, hmdStationary1 );
			RenderWindow( handWindow1, objectViewpoint, handStationary1 );
			RenderWindow( chestWindow1, objectViewpoint, chestStationary1 );

			// Show the position and orientation of each marker structure
			// from the perspective of each CODA unit.
			hmdMobile->ShowVisibility( unitMarkerFrame[1], CYAN );
			handMobile->ShowVisibility( unitMarkerFrame[1], MAGENTA );
			chestMobile->ShowVisibility( unitMarkerFrame[1], YELLOW );
			RenderWindow( vrWindow1, codaViewpoint1, mobiles );
		
		}

		// A timer to handle animations and screen refresh, and associated actions.
		static System::Windows::Forms::Timer^ refreshTimer;
		void OnTimerElapsed( System::Object^ source, System::EventArgs^ e ) {

			Render();

		}
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


		System::Void Form1_Shown(System::Object^  sender, System::EventArgs^  e) {

			Vector3	unitOffset[MAX_UNITS];
			Matrix3x3 unitRotation[MAX_UNITS];
			Matrix3x3 back;

			coda->GetAlignmentTransforms( unitOffset, unitRotation );

			// Create the required OpenGLWindows each linked to a pane in the Form.
			hmdWindow0 = CreateOpenGLWindowInForm( hmdPanel0 );
			hmdWindow1 = CreateOpenGLWindowInForm( hmdPanel1, hmdWindow0->hRC );
			handWindow0 = CreateOpenGLWindowInForm( handPanel0,hmdWindow0->hRC );
			handWindow1 = CreateOpenGLWindowInForm( handPanel1, hmdWindow0->hRC );
			chestWindow0 = CreateOpenGLWindowInForm( chestPanel0, hmdWindow0->hRC );
			chestWindow1 = CreateOpenGLWindowInForm( chestPanel1, hmdWindow0->hRC );
			vrWindow0 = CreateOpenGLWindowInForm( vrPanel0, hmdWindow0->hRC );
			vrWindow1 = CreateOpenGLWindowInForm( vrPanel1, hmdWindow0->hRC );

			// Create a viewpoint that looks at the origin from the negative Z axis.
			// This is the cannonical viewpoint for an object at zero position and orientation.
			objectViewpoint = new Viewpoint( 6.0, 10.0, 10.0, 10000.0);
			objectViewpoint->SetPosition( 0.0, 0.0, - 2000.0 );
			objectViewpoint->SetOrientation( 0.0, 0.0, 180.0 );

			// Create viewpoints that looks at the origin from the position of each CODA.
			codaViewpoint0 = new Viewpoint( 6.0, 25.0, 10.0, 10000.0);
			codaViewpoint0->SetPosition( unitOffset[0] );
			coda->CopyVector( back[Z], unitOffset[0] );
			coda->NormalizeVector( back[Z] );
			coda->ComputeCrossProduct( back[X], coda->jVector, back[Z] );
			coda->ComputeCrossProduct( back[Y], back[Z], back[X] );
			codaViewpoint0->SetOrientation( back );

			codaViewpoint1 = new Viewpoint( 6.0, 25.0, 10.0, 10000.0);
			codaViewpoint1->SetPosition( unitOffset[1] );
			coda->CopyVector( back[Z], unitOffset[1] );
			coda->NormalizeVector( back[Z] );
			coda->ComputeCrossProduct( back[X], coda->jVector, back[Z] );
			coda->ComputeCrossProduct( back[Y], back[Z], back[X] );
			codaViewpoint1->SetOrientation( back );

			// Create the OpenGLObjects that depict the marker array structure.
			objects = new Grasp::GraspGLObjects();
			hmdStationary0 = objects->CreateHmdMarkerStructure( "Bdy\\HMD.bdy" );
			handStationary0 = objects->CreateHandMarkerStructure( "Bdy\\Hand.bdy" );
			chestStationary0 = objects->CreateChestMarkerStructure( "Bdy\\Chest.bdy" );
			hmdStationary1 = objects->CreateHmdMarkerStructure( "Bdy\\HMD.bdy" );
			handStationary1 = objects->CreateHandMarkerStructure( "Bdy\\Hand.bdy" );
			chestStationary1 = objects->CreateChestMarkerStructure( "Bdy\\Chest.bdy" );
			hmdMobile = objects->CreateHmdMarkerStructure( "Bdy\\HMD.bdy" );
			handMobile = objects->CreateHandMarkerStructure( "Bdy\\Hand.bdy" );
			chestMobile = objects->CreateChestMarkerStructure( "Bdy\\Chest.bdy" );

			// Initially set the hand and chest away from the center.
			// This will be overidden later when the trackers are active, 
			// but it is convenient to do this here for testing when the trackers is not used.
			handMobile->SetPosition( 400.0, -100.0, -400.0 );
			chestMobile->SetPosition( 0.0, -500.0, 0.0 );
			mobiles = new Yoke();
			mobiles->AddComponent( hmdMobile );
			mobiles->AddComponent( handMobile );
			mobiles->AddComponent( chestMobile );

			CreateRefreshTimer( 500 );
			StartRefreshTimer();
			button1_Click( sender, e );

		}

		System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			char cmd[1024];
			HWND parent = static_cast<HWND>( oculusPanel->Handle.ToPointer() );
			char *root = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi( filenameRoot ).ToPointer();
			sprintf( cmd, "Start /min \"VR Test\" Executables\\Grasp.exe --parent=%d --demo --output=%s", parent, root );
			System::Runtime::InteropServices::Marshal::FreeHGlobal( IntPtr( root ) );
			system( cmd );
		}

		System::Void button1_Click_1(System::Object^  sender, System::EventArgs^  e) {
			 Close();
		 }
};

}

