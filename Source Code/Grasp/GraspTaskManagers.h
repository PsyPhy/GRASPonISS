// GraspTaskManagers.h
//

#define MAX_GRASP_TRIALS 64

namespace Grasp {

	typedef enum { unknown = 0, VTOV, VTOK, KTOK, VTOVK, VKTOVK } Paradigm;
	typedef enum { NullState = 0, StartBlock = 10, BlockCompleted = 19, 
					StartTrial = 20, 
					StraightenHead = 31, AlignHead = 32, 
					PresentTarget = 33, TiltHead = 34, ObtainResponse = 35, 
					ProvideFeedback = 36, TrialCompleted = 39, 
					TrialInterrupted = 40, ExitStateMachine = 99 } GraspTrialState;
	typedef enum { RAISE_HAND_TIMEOUT = 1, LOWER_HAND_TIMEOUT, RAISED_HAND_VIOLATION, HAND_TOO_SOON, ALIGN_HAND_TIMEOUT, HEAD_ALIGNMENT_TIMEOUT, HEAD_TILT_TIMEOUT, HEAD_MISALIGNMENT, RESPONSE_TIMEOUT } Anomaly;

	class GraspTaskManager : public GraspVR {


	public:

		static double indicatorDisplayDuration;
		static double alignHeadTimeout;
		static double tiltHeadTimeout;
		static double responseTimeout;
		static double alignHandTimeout;
		static double handPromptDelay;
		static double handErrorDelay;

		static int maxRetries;

		// List of paramters for each trial.
		struct {
			double	targetHeadTilt;
			double	targetHeadTiltTolerance;
			double	targetHeadTiltDuration;
			double	targetOrientation;
			double  hapticTargetOrientationTolerance;
			double	targetPresentationDuration;
			double	responseHeadTilt;
			double	responseHeadTiltTolerance;
			double	responseHeadTiltDuration;
			double  responseTimeout;
			double	conflictGain;
			bool	provideFeedback;
		} trialParameters[MAX_GRASP_TRIALS];
		
		int nTrials;				// Total number of trials to be performed. This gets incremented if a trial is set to be repeated.
		int currentTrial;			// Where we are in the list of trials.
		int retriesRemaining;		// Erroneous trials get repeated at the end of the block, but only up to a certain number of repetitions.

		// Derived classes must provide a method identifying which paradigm it implements.
		virtual Paradigm GetParadigm( void ) = 0;

		int LoadTrialParameters( char *filename );
		void RepeatTrial( int trial );
		virtual void Prepare( void ) {}
		int RunTrialBlock( char *sequence_filename, char *output_filename_root );
		void ShowProgress( int phase, int characteristic = 0 );

		FILE *response_fp;
		char responseFilename[FILENAME_MAX];
		FILE *pose_fp;
		char poseFilename[FILENAME_MAX];

		// State Machine
		GraspTrialState previousState, currentState, nextState;
		bool UpdateStateMachine( void );
		DexServices *dexServices;

		// Timers used by various different states.
		// It would have been nice if each timer could be restricted in scope to the 3 methods associated 
		//  with the associated state, but that would mean making a class for each state. I may do that 
		//  someday, but in the mean time I give them names here to help keep track of which state is 
		//  using which time and for what.
		::Timer presentTargetTimer;
		::Timer straightenHeadTimer;
		::Timer alignHeadTimer;
		::Timer alignHandTimer;
		::Timer tiltHeadTimer;
		::Timer responseTimer;
		::Timer trialCompletedTimer;
		::Timer blockTimer;

		// Detect action of the subject to record a response.
		bool Validate( void ) {
			ovrInputState state;
			ovr_GetInputState(	oculusMapper->session,  ovrControllerType_Remote, &state );
			return( (oculusDisplay->Button[MOUSE_LEFT]) || oculusDisplay->Button[MOUSE_MIDDLE] || oculusDisplay->Button[MOUSE_RIGHT] || (state.Buttons & ovrButton_Enter) );
		}

		// 
		// Now define the handlers for each possible GraspTrialState.
		// All of these methods are declared virtual, with the expectation
		// that they will be overridden by derived classes.
		//

		// StartBlock
		// The system waits until the subject presses a button to start a block of trials.
		virtual void EnterStartBlock( void );
		virtual GraspTrialState UpdateStartBlock( void );
		virtual void ExitStartBlock( void );

		// StartTrial
		// Trial parameters are set.
		virtual void EnterStartTrial( void );
		virtual GraspTrialState UpdateStartTrial( void );
		virtual void ExitStartTrial( void );

		// StraightenHead
		// Conflict gain is applied with a black scene.
		virtual void EnterStraightenHead( void );
		virtual GraspTrialState UpdateStraightenHead( void );
		virtual void ExitStraightenHead( void );

		// AlignHead
		// The subject is guided to align the head with the body axis.
		virtual void EnterAlignHead( void );
		virtual GraspTrialState UpdateAlignHead( void );
		virtual void ExitAlignHead( void );

		// PresentTarget
		// The target is diplayed to the subejct.
		virtual void EnterPresentTarget( void );
		virtual GraspTrialState UpdatePresentTarget( void );
		virtual void ExitPresentTarget( void );

		GraspTrialState UpdateVisualTarget( void );
		GraspTrialState UpdateKinestheticTarget( void );

		// TiltHead
		// The subject is guided to achieve a tilted head position.
		virtual void EnterTiltHead( void );
		virtual GraspTrialState UpdateTiltHead( void );
		virtual void ExitTiltHead( void );

		// ObtainResponse
		// The subject reproduces the remembered target orientation.
		virtual void EnterObtainResponse( void );
		virtual GraspTrialState UpdateObtainResponse( void );
		virtual void ExitObtainResponse( void );

		GraspTrialState UpdateVisualResponse( void );
		GraspTrialState UpdateKinestheticResponse( void );

		// ProvideFeedback
		// Show the subject the results of their response.
		virtual void EnterProvideFeedback( void );
		virtual GraspTrialState UpdateProvideFeedback( void );
		virtual void ExitProvideFeedback( void );

		// TrialCompleted
		// Show the subject the results of their response.
		virtual void EnterTrialCompleted( void );
		virtual GraspTrialState UpdateTrialCompleted( void );
		virtual void ExitTrialCompleted( void );

		// BlockCompleted
		// Show the subject the results of their response.
		virtual void EnterBlockCompleted( void );
		virtual GraspTrialState UpdateBlockCompleted( void );
		virtual void ExitBlockCompleted( void );

		// TrialInterrupted
		// The variable interruptCondition should have been set to indicate the cause.
		Anomaly interruptCondition;
		// A holder to remember which indicator has been turned on.
		Assembly *interrupt_indicator;
		virtual void EnterTrialInterrupted( void );
		virtual GraspTrialState UpdateTrialInterrupted( void );
		virtual void ExitTrialInterrupted( void );

	public:
		// Constructor, with initialization of some elements.
		GraspTaskManager( void ) : nTrials(0), retriesRemaining(0), response_fp(NULL), pose_fp(NULL) {}
		~GraspTaskManager(){}
		void Initialize( HINSTANCE instance, OculusDisplayOGL *display, OculusMapper *mapper, GraspTrackers *trkrs, DexServices *dex ) {
			dexServices = dex;
			GraspVR::Initialize( instance, display, mapper, trkrs );
		}
		void Release( void ) {
			GraspVR::Release();
		}

	};

	// V-V protocol. 
	class VtoV : public GraspTaskManager {
		Paradigm GetParadigm( void ) { return( VTOV ); }
		void Prepare( void ) { renderer->selectedTool = renderer->vTool; }
		GraspTrialState UpdatePresentTarget( void ) { return UpdateVisualTarget(); }
		GraspTrialState UpdateObtainResponse( void ) { return UpdateVisualResponse(); }
		void EnterObtainResponse( void );
		void EnterPresentTarget( void );
	};
	// V-VK protocol. 
	class VtoVK : public GraspTaskManager {
		Paradigm GetParadigm( void ) { return( VTOVK ); }
		void Prepare( void ) { renderer->selectedTool = renderer->hand; }
		GraspTrialState UpdatePresentTarget( void ) { 	return UpdateVisualTarget(); }
		GraspTrialState UpdateObtainResponse( void ) { return UpdateKinestheticResponse(); }
		void EnterPresentTarget( void );
		void EnterObtainResponse( void );
	};
	// V-K protocol. 
	class VtoK : public GraspTaskManager {
		Paradigm GetParadigm( void ) { return( VTOK ); }
		void Prepare( void ) { renderer->selectedTool = renderer->hand; }
		GraspTrialState UpdatePresentTarget( void ) { 	return UpdateVisualTarget(); }
		GraspTrialState UpdateObtainResponse( void ) { return UpdateKinestheticResponse(); }
		void EnterPresentTarget( void );
		void EnterObtainResponse( void );
	};
	// K-K protocol. 
	class KtoK : public GraspTaskManager {
		Paradigm GetParadigm( void ) { return( KTOK ); }
		void Prepare( void ) { renderer->selectedTool = renderer->hand; }
		GraspTrialState UpdatePresentTarget( void ) { return UpdateKinestheticTarget(); }
		GraspTrialState UpdateObtainResponse( void ) { return UpdateKinestheticResponse(); }
		void EnterPresentTarget( void );
		void EnterObtainResponse( void );
	};


};

