#include "pch.h"
#include "..\Common\DeviceResources.h"
#include "$safeprojectname$Main.h"
#include "Content\Sample3DSceneRenderer.h"
#include "Common\DirectXHelper.h"


using namespace $safeprojectname$;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

// Loads and initializes application assets when the application is loaded.
$safeprojectname$Main::$safeprojectname$Main(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources), m_pointerLocationX(0.0f)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	// TODO: Replace this with your app's content initialization.
	SetSceneRenderer();

	//m_fpsTextRenderer = std::unique_ptr<SampleFpsTextRenderer>(new SampleFpsTextRenderer(m_deviceResources));

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
}

$safeprojectname$Main::~$safeprojectname$Main() 
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
	
	
}


// Updates application state when the window size changes (e.g. device orientation change)
void $safeprojectname$Main::CreateWindowSizeDependentResources() 
{
	// TODO: Replace this with the size-dependent initialization of your app's content.
	m_sceneRenderer->CreateWindowSizeDependentResources();
	
}

void $safeprojectname$Main::StartRenderLoop()
{
	// If the animation render loop is already running then do not start another thread.
	if (m_renderLoopWorker != nullptr && m_renderLoopWorker->Status == AsyncStatus::Started)
	{
		return;
	}

	// Create a task that will be run on a background thread.
	auto workItemHandler = ref new WorkItemHandler([this](IAsyncAction ^ action)
	{
		// Calculate the updated frame and render once per vertical blanking interval.
		while (action->Status == AsyncStatus::Started)
		{
			critical_section::scoped_lock lock(m_criticalSection);
			Update();
			if (Render())
			{
				m_deviceResources->Present();
			}
		}
	});

	// Run task on a dedicated high priority background thread.
	m_renderLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);
}

void $safeprojectname$Main::StopRenderLoop()
{
	m_renderLoopWorker->Cancel();
}

// Updates the application state once per frame.
void $safeprojectname$Main::Update() 
{
	ProcessInput();
	// Update scene objects.
	m_timer.Tick([&]()
	{
		// TODO: Replace this with your app's content update functions.
		m_sceneRenderer->Update(m_timer);
		//m_fpsTextRenderer->Update(m_timer);
	});
}


// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool $safeprojectname$Main::Render() 
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}
	
	
	//auto 
	ID3D12GraphicsCommandList*	context = m_sceneRenderer->GetComList();
	
	if (context == 0x0000000000000000)
	{
		return m_sceneRenderer->Render(); 
		//return false;
	}
	/*
	//auto context = m_deviceResources->GetD3DDeviceContext();
	// Reset the viewport to target the whole screen.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// Reset render targets to the screen.
	D3D12_CPU_DESCRIPTOR_HANDLE const depthStencilView = m_deviceResources->GetDepthStencilView();
	CD3DX12_CPU_DESCRIPTOR_HANDLE const targets[1] = { m_deviceResources->GetRenderTargetView() };// GetBackBufferRenderTargetView()};
	

	// Clear the back buffer and depth stencil view.
	context->ClearRenderTargetView(m_deviceResources->GetRenderTargetView(), DirectX::Colors::CornflowerBlue, 0, nullptr);
	context->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	
	context->OMSetRenderTargets(1, &m_deviceResources->GetRenderTargetView(), false, &depthStencilView);
	*/
	//m_fpsTextRenderer->Render();
	
	// Render the scene objects.
	// TODO: Replace this with your app's content rendering functions.
	return m_sceneRenderer->Render();
}

void $safeprojectname$::$safeprojectname$Main::OnWindowSizeChanged()
{
	// TODO: Replace this with the size-dependent initialization of your app's content.
	  m_sceneRenderer->CreateWindowSizeDependentResources();

}

void $safeprojectname$::$safeprojectname$Main::OnSuspending()
{
	// TODO: Replace this with your app's suspending logic.

    // Process lifetime management may terminate suspended apps at any time, so it is
    // good practice to save any state that will allow the app to restart where it left off.

    m_sceneRenderer->SaveState();
	
	// If your application uses video memory allocations that are easy to re-create,
	// consider releasing that memory to make it available to other applications.

}

void $safeprojectname$::$safeprojectname$Main::OnResuming()
{
	// TODO: Replace this with your app's resuming logic.
}

void $safeprojectname$::$safeprojectname$Main::OnDeviceRemoved()
{
	// TODO: Save any necessary application or renderer state and release the renderer
	// and its resources which are no longer valid.
	   m_sceneRenderer->SaveState();
	   m_sceneRenderer = nullptr;
}

// Notifies renderers that device resources need to be released.
void $safeprojectname$Main::OnDeviceLost()
{
	m_sceneRenderer->ReleaseDeviceDependentResources();
	//m_fpsTextRenderer->ReleaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be recreated.
void $safeprojectname$Main::OnDeviceRestored()
{
	m_sceneRenderer->CreateDeviceDependentResources();
	//m_fpsTextRenderer->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

void $safeprojectname$::$safeprojectname$Main::WindowActivationChanged(Windows::UI::Core::CoreWindowActivationState activationState)
{  /*
	if (activationState == CoreWindowActivationState::Deactivated)
	{
		m_haveFocus = false;

		switch (m_updateState)
		{
		case UpdateEngineState::Dynamics:
			// From Dynamic mode, when coming out of Deactivated rather than going directly back into Graphic play
			// go to the paused state waiting for user input to continue
			m_updateStateNext = UpdateEngineState::WaitingForPress;
			m_pressResult = PressResultState::ContinueLevel;
			SetGraphicInfoOverlay(GraphicInfoOverlayState::Pause);
			m_uiControl->ShowGraphicInfoOverlay();
			m_graphic->PauseGraphic();
			m_updateState = UpdateEngineState::Deactivated;
			m_uiControl->SetAction(GraphicInfoOverlayCommand::None);
			m_renderNeeded = true;
			break;

		case UpdateEngineState::WaitingForResources:
		case UpdateEngineState::WaitingForPress:
			m_updateStateNext = m_updateState;
			m_updateState = UpdateEngineState::Deactivated;
			m_uiControl->SetAction(GraphicInfoOverlayCommand::None);
			m_uiControl->ShowGraphicInfoOverlay();
			m_renderNeeded = true;
			break;
		}
		// Don't have focus so shutdown input processing
		m_controller->Active(false);
	}
	else if (activationState == CoreWindowActivationState::CodeActivated
		|| activationState == CoreWindowActivationState::PointerActivated)
	{
		m_haveFocus = true;

		if (m_updateState == UpdateEngineState::Deactivated)
		{
			m_updateState = m_updateStateNext;

			if (m_updateState == UpdateEngineState::WaitingForPress)
			{
				m_uiControl->SetAction(GraphicInfoOverlayCommand::TapToContinue);
				m_controller->WaitForPress(m_renderer->GraphicInfoOverlayUpperLeft(), m_renderer->GraphicInfoOverlayLowerRight());
			}
			else if (m_updateStateNext == UpdateEngineState::WaitingForResources)
			{
				m_uiControl->SetAction(GraphicInfoOverlayCommand::PleaseWait);
			}
		}
	}*/
}

void $safeprojectname$::$safeprojectname$Main::ProcessInput()
{
	// TODO: Add per frame input handling here.
		m_sceneRenderer->TrackingUpdate(m_pointerLocationX);
	
}
