#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\Sample3DSceneRenderer.h"
//#include "Content\SampleFpsTextRenderer.h"

// Renders Direct2D and 3D content on the screen.
namespace $safeprojectname$
{
	ref class $safeprojectname$Main : public DX::IDeviceNotify
	{
	public:
		$safeprojectname$Main(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~$safeprojectname$Main();
		void CreateWindowSizeDependentResources();
		void Update();
		bool Render();

		void OnWindowSizeChanged();
		void OnSuspending();
		void OnResuming();
		void OnDeviceRemoved();
		void StartTracking() { m_sceneRenderer->StartTracking(); }
		void TrackingUpdate(float positionX) { m_pointerLocationX = positionX; }
		void StopTracking() { m_sceneRenderer->StopTracking(); }
		bool IsTracking() { return m_sceneRenderer->IsTracking(); }
		void StartRenderLoop();
		void StopRenderLoop();
		Concurrency::critical_section& GetCriticalSection() { return m_criticalSection; }

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

		// Accessors

		Sample3DSceneRenderer^ GetSceneRenderer(){ return m_sceneRenderer;}
		void SetSceneRender() {
			m_sceneRender = ref new SceneRender(m_deviceResources);

		void PauseRequested() {// if //(m_updateState == $safeprojectname$::UpdateEngineState::Dynamics) 
			m_pauseRequested = true; };
		void PressComplete() {// if (m_updateState == $safeprojectname$::UpdateEngineState::WaitingForPress) 
			m_pressComplete = true; };


		void WindowActivationChanged(Windows::UI::Core::CoreWindowActivationState activationState);



	private:
		// Process all input from the user before updating game state
		void ProcessInput();

		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// TODO: Replace with your own content renderers.
		Sample3DSceneRenderer^ m_sceneRenderer;
		//std::unique_ptr<SampleFpsTextRenderer> m_fpsTextRenderer;

		Windows::Foundation::IAsyncAction^ m_renderLoopWorker;
		Concurrency::critical_section m_criticalSection;

		// Rendering loop timer.
		DX::StepTimer m_timer;

		///////////////////////////////////////////////////
		bool                                                m_pauseRequested;
		bool                                                m_pressComplete;
		bool                                                m_renderNeeded;
		bool                                                m_haveFocus;
		bool                                                m_visible;
		///////////////////////////////////////////////////
		
		// Track current input pointer position.
		float m_pointerLocationX;
	};
}